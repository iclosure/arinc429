#pragma once

#include "arapi.h"
#include "utypes.h"
#include <deque>

class CArinc429 : public KPairThread
{
public:
	CArinc429(void);
	virtual ~CArinc429(void);

	typedef enum _CH_XMIT_TYPE { 
		CH_XMIT_SEND = 0,
		CH_XMIT_RECV,
		CH_XMIT_ALL
	} CH_XMIT_TYPE;

public:
	BOOL initialize();
	BOOL syncConfig();
	ARINC_INFO & arInfo() { return m_arInfo; }

	void EnableDisplay(BOOL enable = TRUE)
	{
		m_mutexWrite.lock();
		m_arInfo.TX.Info.Dislpay = enable;
		m_arrayDataTx.clear();
		m_mutexWrite.unlock();

		m_mutexRead.lock();
		m_arInfo.RX.Info.Dislpay = enable;
		m_arrayDataRx.clear();
		m_mutexRead.unlock();
	}

	BOOL getWordTx(TXR_DATA_INFO & word)
	{
		KMutexLocker locker(&m_mutexWrite);

		if (m_arrayDataTx.empty())
		{
			return FALSE;
		}

		word = m_arrayDataTx.front();
		m_arrayDataTx.pop_front();

		return TRUE;
	}

	BOOL getWordRx(TXR_DATA_INFO & word)
	{
		KMutexLocker locker(&m_mutexRead);

		if (m_arrayDataRx.empty())
		{
			return FALSE;
		}

		word = m_arrayDataRx.front();
		m_arrayDataRx.pop_front();

		return TRUE;
	}

	ULONG getCountTx()
	{
		KMutexLocker locker(&m_mutexWrite);
		return m_arInfo.TX.Info.Count;
	}

	ULONG getCountRx()
	{
		KMutexLocker locker(&m_mutexRead);
		return m_arInfo.RX.Info.Count;
	}

	void resetCountTx();
	void resetCountRx();
	void enableSend(BOOL enable = TRUE);
	void enableRecv(BOOL enable = TRUE);

	void enableDisplaySend(BOOL enable = TRUE);
	void enableDisplayRecv(BOOL enable = TRUE);

	BOOL isSendBusy()
	{
		KMutexLocker locker(&m_mutexWrite);
		return !m_cancelSend;
	}

public:
	int setUserHandle(UserHandle pUser);
	int boardType(int board);

	// 通用函数
	int loadslv();
	int go(CH_XMIT_TYPE type = CH_XMIT_ALL);
	int close();
	int reset(CH_XMIT_TYPE type = CH_XMIT_ALL, bool resetBoard = true);

	int setConfig(int item, int value);
	int getConfig(int item, int* value);
	int labelFilter(int label, int action);
	void getLatest(int label, void* data, int* seq_num);

	int numTxChans();
	int numRxChans();

	// Arinc429函数
	int putWord(unsigned long value)
	{
		return ar_putword(m_arInfo.BoardID, m_arInfo.TX.ChID, value);
	}

	int getNext(unsigned long* value)
	{
		return ar_getnext(m_arInfo.BoardID, m_arInfo.RX.ChID, value);
	}

	int getNextt(unsigned long* data, PLONGLONG timetag)
	{
		return ar_getnextt(m_arInfo.BoardID, m_arInfo.RX.ChID, data, timetag);
	}

private:
	inline bool SendWord(TXR_DATA_INFO & dataInfo);
	inline bool workFirst();
	inline bool workSecond();

	void UserHandleFunc();

private:
	BOOL						m_cancelSend;
	BOOL						m_cancelRecv;
	ARINC_INFO					m_arInfo;
	std::deque<TXR_DATA_INFO>	m_arrayDataTx;	// 
	std::deque<TXR_DATA_INFO>	m_arrayDataRx;	// 
	KMutex						m_mutexWrite;
	KMutex						m_mutexRead;
	LONGLONG					m_oldtimetag;
};
