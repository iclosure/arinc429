#pragma once

#include "device.h"
#include <vector>
#include <deque>
#include <string>
#include "../ArincDriver/public.h"

class CPCIDevice : public CDevice
{
public:
	CPCIDevice(int deviceIndex);
	~CPCIDevice();

	int boardType();
	int loadslv(int base_reg, int base_port, int ram_size);
	int go();
	int close();
	int reset();
	bool isOpen() { return m_isOpen; }

	int putWord(int channel, unsigned long value)
	{
		IF_DEVICE_INDEX_OVR_RETURN_INVBOARD(this);
		/*
		if (!isRunning(KPairThread::PairFirst))
		{
			return ARS_WINRTFAIL;
		}

		KMutexLocker locker(&m_mutexWrite);  // for multi-thread
		*/
		
		
		//TODO

		if (!isRunning(KPairThread::PairSecond))
		{
			return ARS_WINRTFAIL;
		}

		KMutexLocker locker(&m_mutexWrite);  // for multi-thread

		m_writeBuffer.channel = channel;
		m_writeBuffer.BufferSize = 1;
		m_writeBuffer.Buffer = &value;

		while(isRunning(KPairThread::PairSecond))
		{
			ULONG bytesReturned = 0;

			::WriteFile(m_hDevice, &m_writeBuffer, sizeof(ATOM_TDATA_INFO), &bytesReturned, NULL);

			if (bytesReturned == sizeof(ATOM_TDATA_INFO))
			{
				return ARS_NORMAL;
			}
			else
			{
				::Sleep(5);	// ÖØÐ´
			}
		}

		return ARS_WINRTFAIL;

		//TODO
		/*
		m_mutex.lock();

		m_writeBuffer.channel = channel;
		m_writeBuffer.BufferSize = 1;
		m_writeData = value;

		m_waitEvtWrite->setEvent();

		m_mutex.unlock();

		switch (waitObject(PairFirst, m_waitEvtWriteDone, INFINITE))
		{
		case WaitObjectFirst:	// quit from thread
		{
			m_waitEvtWriteDone->resetEvent();
			return ARS_WINRTFAIL;
		}
		case WaitObject0:		// has been written
		{
			m_waitEvtWriteDone->resetEvent();
			break;
		}
		case WaitTimeout: assert(false); return ARS_XMITOVRFLO; break;
		default: assert(false); return ARS_WINRTFAIL;
		}

		return ARS_NORMAL;
		*/
	}

	int putWords(int channel, unsigned long* buffer, int size, int* length)
	{
		IF_DEVICE_INDEX_OVR_RETURN_INVBOARD(this);

		if (!isRunning(KPairThread::PairSecond))
		{
			return ARS_WINRTFAIL;
		}

		if (buffer == NULL)
		{
			assert(false);
			return ARS_INVHARVAL;
		}

		KMutexLocker locker(&m_mutexWrite);

		ATOM_TDATA_INFO atomInfo = { channel, size, buffer };
		ULONG bytesReturned = 0;

		if (WriteFile(m_hDevice, &atomInfo, sizeof(ATOM_TDATA_INFO) + size - sizeof(ULONG), &bytesReturned, NULL))
		{
			if (length != NULL)
			{
				*length = bytesReturned / (sizeof(ATOM_TDATA_INFO)-sizeof(ULONG));
			}
		}
		else
		{
			return ARS_WINRTFAIL;
		}

		return ARS_NORMAL;
	}

	int deviceCount() { return m_deviceNames.size(); }

private:
	HR_DISABLE_COPY(CPCIDevice)

	enum ARINC_REG_BASE : ULONG
	{
		ARINC_CORE_REG_BASE = 0x00010000,
		ARINC_USER_REG_BASE = 0x00000000
	};

	BOOL readRegister(ULONG addr, ULONG* value, ARINC_REG_ADDR_TYPE = ARINC_REG_ADDR_CORE);
	BOOL writeRegister(ULONG addr, ULONG value, ARINC_REG_ADDR_TYPE = ARINC_REG_ADDR_CORE);
	BOOL enableLabel0(int channel, BOOL enable = TRUE);
	BOOL resetDevice(int code);
	inline bool workFirst();
	inline bool workSecond();

private:
	bool					m_isOpen;
	HANDLE					m_hDevice;
	KWaitEvent*				m_waitEvtRead;
	ATOM_RDATA_INFO			m_atomBuffer[512];
	std::vector<tstring>	m_deviceNames;

	// Asynchrony write
	KMutex					m_mutex;
	KMutex					m_mutexWrite;
	DWORD					m_writeData;
	ATOM_TDATA_INFO			m_writeBuffer;
	KWaitEvent*				m_waitEvtWrite;
	KWaitEvent*				m_waitEvtWriteDone;
};
