#pragma once

#include "arapi.h"
#include <assert.h>

#define ENABLE_MAX_BUFFER  1		// 

#if ENABLE_MAX_BUFFER
#include <circfifo.h>
typedef hr::circfifo<ARINC_WORD_MSG, 1000> ARINC_WORD_MSG_ARRAY;
#else
#include <deque>
typedef std::deque<ARINC_WORD_MSG> ARINC_WORD_MSG_ARRAY;
#endif  // ENABLE_MAX_BUFFER

// 设备号的边界检查
#define IF_DEVICE_INDEX_OVR_RETURN_INVBOARD(_thisclass_)  \
	if (_thisclass_->deviceCount() == 0 || \
		__super::deviceIndex() < 0 || \
		__super::deviceIndex() >= _thisclass_->deviceCount()) \
	{ \
		return ARS_INVBOARD; \
	}

class CDevice : public KPairThread
{
public:
	CDevice(int deviceIndex, tstring name);
	~CDevice();

	enum ARINC_REG_ADDR_TYPE
	{
		ARINC_REG_ADDR_CORE,	// Core429内部寄存器地址
		ARINC_REG_ADDR_USER		// FPGA自定义地址
	};

	virtual int boardType();
	int setUserHandler(UserHandle userHandler);
	virtual int loadslv(int base_reg, int base_port, int ram_size);
	virtual int go();
	virtual int close();
	virtual int reset();
	virtual bool isOpen() = 0;
	virtual int putWord(int channel, unsigned long value) = 0;
	virtual int putWords(int channel, unsigned long* buffer, int size, int* length) = 0;

	int getNext(int channel, unsigned long* value)
	{
		KMutexLocker locker(&m_mutex);

		ARINC_WORD_MSG_ARRAY &array = m_channelRxMsg[channel];

		if (array.empty())
		{
			return ARS_NODATA;
		}

		ARINC_WORD_MSG msg = array.front();
		*value = msg.data;
		array.pop_front();

		return ARS_GOTDATA;
	}

	int getNextt(int channel, unsigned long* value, void* timetag)
	{
		KMutexLocker locker(&m_mutex);

		ARINC_WORD_MSG_ARRAY &array = m_channelRxMsg[channel];

		if (array.empty())
		{
			return ARS_NODATA;
		}

		ARINC_WORD_MSG &msg = array.front();
		*value = msg.data;
		*(PLONGLONG)timetag = msg.timestamp;
		array.pop_front();

		return ARS_GOTDATA;
	}

	int getWords(int channel, unsigned long* buffer, int size, int* length)
	{
		KMutexLocker locker(&m_mutex);

		if (buffer == NULL)
		{
			assert(false);
			return ARS_INVHARVAL;
		}

		ARINC_WORD_MSG_ARRAY &array = m_channelRxMsg[channel];

		if (m_channelRxMsg[channel].empty())
		{
			return ARS_NODATA;
		}

		int i;

		for (i = 0; i < size && !array.empty(); i++)
		{
			buffer[i] = array.front().data;
			array.pop_front();
		}

		if (length != NULL)
		{
			*length = i;
		}

		return ARS_GOTDATA;
	}

	int getWordst(int channel, ARINC_WORD_MSG* buffer, int size, int* length)
	{
		KMutexLocker locker(&m_mutex);

		if (buffer == NULL)
		{
			assert(false);
			return ARS_INVHARVAL;
		}

		ARINC_WORD_MSG_ARRAY &array = m_channelRxMsg[channel];

		if (array.empty())
		{
			return ARS_NODATA;
		}

		int i;

		for (i = 0; i < size && !array.empty(); i++)
		{
			ARINC_WORD_MSG &msg = array.front();
			buffer[i].data = msg.data;
			buffer[i].timestamp = msg.timestamp;
			array.pop_front();
		}

		if (length != NULL)
		{
			*length = i;
		}

		return ARS_GOTDATA;
	}

	int setConfig(int item, int value);
	int getConfig(int item, int* value);
	int labelFilter(int channel, int label, int action);
	int getLatest(int channel, int label, void *data, int *seq_num);
	int numRChans();
	int numTChans();
	virtual int deviceCount() = 0;

protected:
	virtual BOOL readRegister(ULONG addr, ULONG* value, ARINC_REG_ADDR_TYPE type = ARINC_REG_ADDR_CORE) = 0;
	virtual BOOL writeRegister(ULONG addr, ULONG value, ARINC_REG_ADDR_TYPE type = ARINC_REG_ADDR_CORE) = 0;
	virtual BOOL resetDevice(int code);
	virtual BOOL enableLabel0(int channel, BOOL enable = TRUE);
	DWORD getEnableLabel0() { return m_enableLabel0; }
	void setEnableLabel0(DWORD value) { m_enableLabel0 = value; }

	// 数据入栈
	void appendData(int channel, ARINC_WORD_MSG & data)
	{
		KMutexLocker locker(&m_mutex);
		assert(0 <= channel && channel < m_rxChNum);
		m_channelRxMsg[channel].push_back(data);
	}

	void callUserHandle()
	{
		if (m_onDataRecv != NULL)
		{
			m_onDataRecv();
		}
	}

	int deviceIndex() const { return m_deviceIndex; }

private:
	HR_DISABLE_COPY(CDevice)

	// 清空缓存
	void clearData()
	{
		KMutexLocker locker(&m_mutex);

		//! 清空接收缓冲资源
		for (int i = 0; i < m_rxChNum; i++)
		{
			m_channelRxMsg[i].clear();
		}
	}

	int setBaudRate(int channel, int trx, int value);
	int setParity(int channel, int trx, int value);
	int setDecoder(int channel, int value);
	int setSDI(int channel, int value);
	int setLookBack(int channel, int value);
	int setLabelFilter(int channel, int label, int action);
	int setBitsOfRegister(int channel, int trx, ULONG value, ULONG offset, ULONG mask);

	int getBaudRate(int channel, int trx, int* value);
	int getParity(int channel, int trx, int* value);
	int getDecoder(int channel, int* value);
	int getSDI(int channel, int* value);
	int getLookBack(int channel, int* value);
	int getBitsOfRegister(int channel, int trx, ULONG* value, ULONG offset, ULONG mask);

private:
	int				m_deviceIndex;
	int				m_txChNum;
	int				m_rxChNum;
	ULONG			m_enableLabel0;
	UCHAR(*m_labelTables)[ARINC_LABEL_NUM];
	UserHandle		m_onDataRecv;
	ARINC_WORD_MSG_ARRAY*	m_channelRxMsg;
	KMutex			m_mutex;
};
