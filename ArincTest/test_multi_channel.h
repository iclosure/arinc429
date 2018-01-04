
#pragma once

#include <Windows.h>
#include <mmtimer.h>
#include "../ArincApi/arapi.h"
#include <cassert>

#pragma warning (disable: 4996)

typedef struct _CHANNEL_CONFIG_INFO
{
	int board;
	int channel;
	ULONG countTx;
	ULONG countRx;

} CHANNEL_CONFIG_INFO, *PCHANNEL_CONFIG_INFO;

class CArinc429 : public KPairThread
{
public:
	CArinc429(void) :
		KPairThread(_T("thread for arinc429's data transmitting")),
		  m_cancelSend(TRUE),
		  m_cancelRecv(TRUE)
	{
	}

	~CArinc429(void)
	{
		this->stop();
	}

	void setConfig(PCHANNEL_CONFIG_INFO config)
	{
		m_arInfo.board = config->board;
		m_arInfo.channel = config->channel;
		m_arInfo.countTx = config->countTx;
		m_arInfo.countRx = config->countRx;
	}

	void start()
	{
		m_mutexWrite.lock();
		m_cancelSend = FALSE;
		m_mutexWrite.unlock();

		m_mutexRead.lock();
		m_cancelRecv = FALSE;
		m_mutexRead.unlock();

		KPairThread::start(KPairThread::PairAll);
	}

	void stop()
	{
		m_mutexWrite.lock();
		m_cancelSend = TRUE;
		m_mutexWrite.unlock();

		m_mutexRead.lock();
		m_cancelRecv = TRUE;
		m_mutexRead.unlock();

		KPairThread::stop(KPairThread::PairAll);
	}

	ULONG countTx()
	{
		KMutexLocker locker(&m_mutexWrite);
		return m_arInfo.countTx;
	}

	ULONG countRx()
	{
		KMutexLocker locker(&m_mutexRead);
		return m_arInfo.countRx;
	}

	int board() const { return m_arInfo.board; }
	int channel() const { return m_arInfo.channel; }

private:

	bool workFirst()
	{
		DWORD end = 0x00196C98;
		DWORD index = 0;

		while (index++ < end)
		{
			if (m_cancelSend)
			{
				return false;
			}

			if (ar_putword(m_arInfo.board, m_arInfo.channel, index) != ARS_NORMAL)
			{
				//printf("write failed!!!\n");
				return false;
			}

			m_mutexWrite.lock();
			m_arInfo.countTx++;
			m_mutexWrite.unlock();
		}

		Sleep(5);

		return false;
	}

	bool CArinc429::workSecond()
	{
		int count = 2048;
		DWORD data;
		__int64 timestamp;

		while (count--)
		{
			if (m_cancelRecv)
			{
				break;
			}

			if (ar_getnextt(m_arInfo.board, m_arInfo.channel, &data, &timestamp) != ARS_GOTDATA)
			{
				//printf("read failed!!!\n");
				break;
			}

			m_mutexRead.lock();
			m_arInfo.countRx++;
			m_mutexRead.unlock();
		}

		Sleep(5);

		return true;
	}

	void saveToFile()
	{
		// 

		// 
		fflush(m_fileHandle);
	}

private:
	BOOL				m_cancelSend;
	BOOL				m_cancelRecv;
	CHANNEL_CONFIG_INFO m_arInfo;
	FILE*				m_fileHandle;
	KMutex				m_mutexWrite;
	KMutex				m_mutexRead;
};

CArinc429* _v_arinc429 = NULL;
int _v_chNum = 0;

void WINAPI MmTimerEventCount(PVOID dwUser)
{
	printf("--------------------------------------\n");

	for (int i = 0; i < _v_chNum; i++)
	{
		printf("BOARD: %d, CH: %2d, TX: %10d, RX: %10d\n", _v_arinc429[i].board(), i, _v_arinc429[i].countTx(), _v_arinc429[i].countRx());
	}
}

int test_multi_channel()
{
	printf("\n--------  begin to test multi-channel --------\n\n");

	short status;
	int board = 0;

	printf("input board id: ");
	scanf("%d", &board);

	if (board < 0 || board >= ar_board_num())
	{
		printf("invalid board: %d!\n", board);
	}

	status = ar_loadslv(board, 0, 0, 0);
	if (ARS_NORMAL != status)
	{
		assert(false);
		return 0;
	}

	status = ar_reset(board);

	int temp1 = 0;

	// loop back
	printf("set internal wrap (0-外连；1-回环): ");
	scanf("%d", &temp1);

	_v_chNum = ar_num_rchans(board);

	if (_v_chNum > ARINC_MAX_CH_NUM)
	{
		assert(false);
		return 0;
	}

	// 设置默认配置信息

	ar_set_config(board, ARU_RECV_RATE, AR_100K);
	ar_set_config(board, ARU_XMIT_RATE, AR_100K);
	ar_set_config(board, ARU_PARITY, AR_NONE);
	ar_set_config(board, ARU_INTERNAL_WRAP, AR_WRAP_ON);

	for (int i = 0; i < _v_chNum; i++)
	{
		if (i >= 4)
		{
			status = ar_set_config(board, ARU_CH01_LOOPBACK + i, AR_ON);
		}
		else
		{
			status = ar_set_config(board, ARU_CH01_LOOPBACK + i, temp1);
		}
		ar_label_filter(board, i, ARU_ALL_LABELS, ARU_FILTER_OFF);
		ar_set_config(board, ARU_CH01_DECODER + i, AR_OFF);
		ar_set_config(board, ARU_CH01_SDI + i, 0x00);
	}

	_v_arinc429 = new CArinc429[_v_chNum];

	for (int i = 0; i < _v_chNum; i++)
	{
		CHANNEL_CONFIG_INFO config;
		config.board = board;
		config.channel = i;
		config.countTx = 0;
		config.countRx = 0;
		_v_arinc429[i].setConfig(&config);
	}

	MmTimer m_mmTimer;
	m_mmTimer.set(TIME_PERIODIC, 1000, MmTimerEventCount, NULL);

	status = ar_go(board);

	for (int i = 0; i < _v_chNum; i++)
	{
		_v_arinc429[i].start();
	}
	m_mmTimer.start();

	getchar();
	getchar();

	m_mmTimer.stop();

	for (int i = 0; i < _v_chNum; i++)
	{
		_v_arinc429[i].stop();
	}

	delete[] _v_arinc429;

	status = ar_close(board);
	if (ARS_NORMAL != status)
	{
		assert(false);
		return 0;
	}

	return 0;
}
