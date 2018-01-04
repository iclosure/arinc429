#include "StdAfx.h"
#include "Arinc429.h"

#pragma comment(lib, "arapi.lib")

CArinc429::CArinc429(void) :
	KPairThread(_T(__FUNCTION__)_T("-thread"), this),
	m_cancelSend(TRUE),
	m_cancelRecv(TRUE)
{
	initialize();
}

CArinc429::~CArinc429(void)
{
	enableSend(FALSE);
	enableRecv(FALSE);
	KPairThread::stop();

	m_arrayDataTx.clear();
	m_arrayDataRx.clear();

	this->close();
}

void CArinc429::UserHandleFunc()
{

}

void CArinc429::resetCountTx()
{
	m_mutexWrite.lock();
	m_arInfo.TX.Info.Count = 0;
	m_arrayDataTx.clear();
	m_mutexWrite.unlock();

	m_mutexRead.lock();
	m_oldtimetag = 0;
	m_mutexRead.unlock();
}

void CArinc429::resetCountRx()
{
	m_mutexWrite.lock();
	m_arInfo.RX.Info.Count = 0;
	m_arrayDataRx.clear();
	m_mutexWrite.unlock();

	m_mutexRead.lock();
	m_oldtimetag = 0;
	m_mutexRead.unlock();
}

void CArinc429::enableSend(BOOL enable /* = TRUE */)
{
	KMutexLocker locker(&m_mutexWrite);
	m_arInfo.TX.Info.EnSend = TRUE;
	m_cancelSend = !enable;
}

void CArinc429::enableRecv(BOOL enable/* = TRUE*/)
{
	KMutexLocker locker(&m_mutexRead);
	m_cancelRecv = !enable;
}

void CArinc429::enableDisplaySend(BOOL enable /* = TRUE */)
{
	KMutexLocker locker(&m_mutexWrite);
	m_arInfo.TX.Info.Dislpay = enable;
}

void CArinc429::enableDisplayRecv(BOOL enable/* = TRUE*/)
{
	KMutexLocker locker(&m_mutexRead);
	m_arInfo.RX.Info.Dislpay = enable;
}

BOOL CArinc429::initialize()
{
	// 参数初始化

	RtlZeroMemory(&m_arInfo, sizeof(ARINC_INFO));

	m_arInfo.BoardID = 0;
	
	m_arInfo.TX.Info.Period.Value = 0;	// ms

	m_arInfo.TX.Info.EnPeriod = TRUE;
	m_arInfo.TX.Info.EnCounts = FALSE;

	m_arInfo.RX.Info.Function = 0;		// 自动接收

	m_arInfo.TX.Info.DataFormat = 2;	// 十六进制
	m_arInfo.RX.Info.DataFormat = 2;	// 十六进制

	m_arInfo.TX.Info.DataSplit = 0;		// 整体输出
	m_arInfo.RX.Info.DataSplit = 0;		// 整体输出

	m_arInfo.TX.Info.Data[0] = 0;
	m_arInfo.TX.Info.Data[1] = (0xF) << AR_WORD_DATA_OFFSET;

	m_arInfo.TX.Info.Count = 0;
	m_arInfo.RX.Info.Count = 0;

	m_arInfo.TX.ChID = 0;
	m_arInfo.RX.ChID = 0;

	resetCountTx();
	resetCountRx();

	EnableDisplay();

	return TRUE;
}

BOOL CArinc429::syncConfig()
{
	int status = 0;

	m_arInfo.TX.ChNum = numTxChans();
	m_arInfo.RX.ChNum = numRxChans();

	//
	getConfig(ARU_TX_CH01_BIT_RATE + m_arInfo.TX.ChID, &m_arInfo.TX.Info.BaudRate);
	getConfig(ARU_CH01_LOOPBACK + m_arInfo.TX.ChID, &m_arInfo.TX.Info.Loopback);
	getConfig(ARU_TX_CH01_PARITY + m_arInfo.TX.ChID, &m_arInfo.TX.Info.Parity);

	getConfig(ARU_RX_CH01_BIT_RATE + m_arInfo.RX.ChID, &m_arInfo.RX.Info.BaudRate);
	getConfig(ARU_RX_CH01_PARITY + m_arInfo.RX.ChID, &m_arInfo.RX.Info.Parity);
	getConfig(ARU_CH01_DECODER + m_arInfo.RX.ChID, &m_arInfo.RX.Info.Decoder);
	getConfig(ARU_CH01_SDI + m_arInfo.RX.ChID, &m_arInfo.RX.Info.SDI.all);

	// 不过滤所有LABEL
	labelFilter(ARU_ALL_LABELS, ARU_FILTER_OFF);
	for (int j = 0; j < AR_LABEL_NUM; j++)
	{
		m_arInfo.RX.Info.Label.Data[j].Action = ARU_FILTER_OFF;
	}

	return TRUE;
}

int CArinc429::setUserHandle(UserHandle pUser)
{
	return ar_set_userhandle(pUser);
}

int CArinc429::boardType(int board)
{
	return ar_board_type(board);
}

// 通用函数
int CArinc429::loadslv()
{
	int status = ar_loadslv(m_arInfo.BoardID, 0, 0, 0);

	if (ARS_NORMAL == status)
	{
		syncConfig();
	}

	return status;
}

int CArinc429::go(CH_XMIT_TYPE type/* = CH_XMIT_ALL*/)
{
	int status = ar_go(m_arInfo.BoardID);
	if (ARS_NORMAL == status)
	{
		switch(type)
		{
		case CH_XMIT_SEND:
		{
			resetCountTx();
			KPairThread::start(PairFirst);
			break;
		}
		case CH_XMIT_RECV:
		{
			resetCountRx();
			KPairThread::start(PairSecond);
			enableRecv();
			break;
		}
		case CH_XMIT_ALL:
		{
			resetCountTx();
			resetCountRx();
			enableRecv();
			KPairThread::start();
			break;
		}
		default:
			ASSERT(FALSE);
			break;
		}
	}

	return status;
}

int CArinc429::close()
{
	enableSend(FALSE);
	enableRecv(FALSE);
	
	KPairThread::stop();

	int status = ar_close(m_arInfo.BoardID);
	
	// 
	resetCountTx();
	resetCountRx();

	return status;
}

int CArinc429::reset(CH_XMIT_TYPE type/* = CH_XMIT_ALL*/, bool resetBoard/* = true*/)
{
	int status = ARS_NORMAL;
	if (resetBoard)
	{
		ar_reset(m_arInfo.BoardID);
	}
	if (ARS_NORMAL == status)
	{
		switch(type)
		{
		case CH_XMIT_SEND:
		{
			enableSend(FALSE);
			KPairThread::stop(PairFirst);
			//resetCountTx();
			break;
		}
		case CH_XMIT_RECV:
		{
			enableRecv(FALSE);
			KPairThread::stop(PairSecond);
			//resetCountRx();
			break;
		}
		case CH_XMIT_ALL:
		{
			enableSend(FALSE);
			enableRecv(FALSE);
			KPairThread::stop();
			//resetCountTx();
			//resetCountRx();
			break;
		}
		default:
			ASSERT(FALSE);
			break;
		}
	}

	return status;
}

// Arinc429函数
int CArinc429::setConfig(int item, int value)
{
	int _value = value;

	if (ARU_RECV_RATE == item)												// 设置所有接收通道波特率
	{
		switch (value)
		{
		case 0: _value = AR_12K5; break;
		case 1: _value = AR_50K; break;
		case 2: _value = AR_100K; break;
		default: _value = -1; break;
		}
	}
	else if (ARU_RX_CH01_BIT_RATE <= item && item <= ARU_RX_CH16_BIT_RATE)	// 设置接收通道波特率
	{
		switch (value)
		{
		case 0: _value = AR_12K5; break;
		case 1: _value = AR_50K; break;
		case 2: _value = AR_100K; break;
		default: _value = -1; break;
		}
	}
	else if (ARU_XMIT_RATE == item)											// 设置所有发送通道波特率
	{
		switch (value)
		{
		case 0: _value = AR_12K5; break;
		case 1: _value = AR_50K; break;
		case 2: _value = AR_100K; break;
		default: _value = -1; break;
		}
	}
	else if (ARU_TX_CH01_BIT_RATE <= item && item <= ARU_TX_CH16_BIT_RATE)	// 设置发送通道波特率
	{
		switch (value)
		{
		case 0: _value = AR_12K5; break;
		case 1: _value = AR_50K; break;
		case 2: _value = AR_100K; break;
		default: _value = -1; break;
		}
	}
	else if (ARU_RX_CH01_PARITY <= item && item <= ARU_RX_CH16_PARITY)		// 设置发送通道校验方式
	{
		switch (value)
		{
		case 0: _value = AR_OFF; break;
		case 1: _value = AR_ODD; break;
		case 2: _value = AR_EVEN; break;
		default: _value = -1; break;
		}
	}
	else if (ARU_TX_CH01_PARITY <= item && item <= ARU_TX_CH16_PARITY)		// 设置接收通道校验方式
	{
		switch (value)
		{
		case 0: _value = AR_OFF; break;
		case 1: _value = AR_ODD; break;
		case 2: _value = AR_EVEN; break;
		default: _value = -1; break;
		}
	}
	else if (ARU_PARITY == item)											// 设置所有发送、接收通道校验方式
	{
		switch (value)
		{
		case 0: _value = AR_OFF; break;
		case 1: _value = AR_ODD; break;
		case 2: _value = AR_EVEN; break;
		default: _value = -1; break;
		}
	}
	else if (ARU_CH01_DECODER <= item && item <= ARU_CH16_DECODER)			// 设置接收通道标号过滤开关
	{
		switch (value)
		{
		case 0: _value = AR_OFF; break;
		case 1: _value = AR_ON; break;
		default: _value = -1; break;
		}
	}
	else if (ARU_CH01_SDI <= item && item <= ARU_CH16_SDI)					// 设置接收通道标号过滤
	{
		_value = value;
	}
	else if (ARU_CH01_LOOPBACK <= item && item <= ARU_CH16_LOOPBACK)		// 设置发送通道闭环测试标志
	{
		switch (value)
		{
		case 0: _value = AR_WRAP_OFF; break;
		case 1: _value = AR_WRAP_ON; break;
		default: _value = -1; break;
		}
	}
	else if (ARU_INTERNAL_WRAP == item)										// 设置所有发送通道闭环测试标志
	{
		switch (value)
		{
		case 0: _value = AR_WRAP_OFF; break;
		case 1: _value = AR_WRAP_ON; break;
		default: _value = -1; break;
		}
	}
	else
	{
		_value = -1;
	}

	if (_value == -1)
	{
		return ARS_INVHARVAL;
	}

	int status = ar_set_config(m_arInfo.BoardID, item, _value);

	if (ARS_NORMAL == status)
	{
		if (ARU_RECV_RATE == item)												// 设置所有接收通道波特率
		{
			for (int i = 0; i < m_arInfo.RX.ChNum; i++)
			{
				m_arInfo.RX.Info.BaudRate = value;
			}
		}
		else if (ARU_RX_CH01_BIT_RATE <= item && item <= ARU_RX_CH16_BIT_RATE)	// 设置接收通道波特率
		{
			m_arInfo.RX.Info.BaudRate = value;
		}
		else if (ARU_XMIT_RATE == item)											// 设置所有发送通道波特率
		{
			for (int i = 0; i < m_arInfo.TX.ChNum; i++)
			{
				m_arInfo.TX.Info.BaudRate = value;
			}
		}
		else if (ARU_TX_CH01_BIT_RATE <= item && item <= ARU_TX_CH16_BIT_RATE)	// 设置发送通道波特率
		{
			m_arInfo.TX.Info.BaudRate = value;
		}
		else if (ARU_RX_CH01_PARITY <= item && item <= ARU_RX_CH16_PARITY)		// 设置发送通道校验方式
		{
			m_arInfo.RX.Info.Parity = value;
		}
		else if (ARU_TX_CH01_PARITY <= item && item <= ARU_TX_CH16_PARITY)		// 设置接收通道校验方式
		{
			m_arInfo.TX.Info.Parity = value;
		}
		else if (ARU_PARITY == item)											// 设置所有发送、接收通道校验方式
		{
			for (int i = 0; i < m_arInfo.RX.ChNum; i++)
			{
				m_arInfo.RX.Info.Parity = value;
			}
			
			for (int i = 0; i < m_arInfo.TX.ChNum; i++)
			{
				m_arInfo.TX.Info.Parity = value;
			}
		}
		else if (ARU_CH01_DECODER <= item && item <= ARU_CH16_DECODER)			// 设置接收通道标号过滤开关
		{
			m_arInfo.RX.Info.Decoder = value;
		}
		else if (ARU_CH01_SDI <= item && item <= ARU_CH16_SDI)					// 设置接收通道标号过滤
		{
			m_arInfo.RX.Info.SDI.all = value;
		}
		else if (ARU_CH01_LOOPBACK <= item && item <= ARU_CH16_LOOPBACK)		// 设置发送通道闭环测试标志
		{
			m_arInfo.TX.Info.Loopback = value;
		}
		else if (ARU_INTERNAL_WRAP == item)										// 设置所有发送通道闭环测试标志
		{
			m_arInfo.TX.Info.Loopback = value;
		}
	}

	return status;
}

int CArinc429::getConfig(int item, int* value)
{
	int status = ar_get_config(m_arInfo.BoardID, item, value);

	if (ARS_NORMAL == status)
	{
 		if (ARU_RX_CH01_BIT_RATE <= item && item <= ARU_RX_CH16_BIT_RATE)		// 获取接收通道波特率
		{
			switch (*value)
			{
			case AR_12K5: *value = 0; break;
			case AR_50K: *value = 1; break;
			case AR_100K: *value = 2; break;
			}
			
			m_arInfo.RX.Info.BaudRate = *value;
		}
		else if (ARU_TX_CH01_BIT_RATE <= item && item <= ARU_TX_CH16_BIT_RATE)	// 获取发送通道波特率
		{
			switch (*value)
			{
			case AR_12K5: *value = 0; break;
			case AR_50K: *value = 1; break;
			case AR_100K: *value = 2; break;
			}

			m_arInfo.TX.Info.BaudRate = *value;
		}
		else if (ARU_RX_CH01_PARITY <= item && item <= ARU_RX_CH16_PARITY)		// 获取发送通道校验方式
		{
			switch (*value)
			{
			case AR_OFF: *value = 0; break;
			case AR_ODD: *value = 1; break;
			case AR_EVEN: *value = 2; break;
			}

			m_arInfo.RX.Info.Parity = *value;
		}
		else if (ARU_TX_CH01_PARITY <= item && item <= ARU_TX_CH16_PARITY)		// 获取接收通道校验方式
		{
			switch (*value)
			{
			case AR_OFF: *value = 0; break;
			case AR_ODD: *value = 1; break;
			case AR_EVEN: *value = 2; break;
			}

			m_arInfo.TX.Info.Parity = *value;
		}
		else if (ARU_CH01_DECODER <= item && item <= ARU_CH16_DECODER)			// 获取接收通道标号过滤开关
		{
			switch (*value)
			{
			case AR_OFF: *value = 0; break;
			case AR_ON: *value = 1; break;
			}

			m_arInfo.RX.Info.Decoder = *value;
		}
		else if (ARU_CH01_SDI <= item && item <= ARU_CH16_SDI)					// 获取接收通道标号过滤
		{
			m_arInfo.RX.Info.SDI.all = *value;
		}
		else if (ARU_CH01_LOOPBACK <= item && item <= ARU_CH16_LOOPBACK)		// 获取发送通道闭环测试标志
		{
			switch (*value)
			{
			case AR_WRAP_OFF: *value = 0; break;
			case AR_WRAP_ON: *value = 1; break;
			}

			m_arInfo.TX.Info.Loopback = *value;
		}
		else if (ARU_INTERNAL_WRAP == item)										// 获取所有发送通道闭环测试标志
		{
			switch (*value)
			{
			case AR_WRAP_OFF: *value = 0; break;
			case AR_WRAP_ON: *value = 1; break;
			}

			m_arInfo.TX.Info.Loopback = *value;
		}
	}
	
	return status;
}

int CArinc429::labelFilter(int label, int action)
{
	return ar_label_filter(m_arInfo.BoardID, m_arInfo.RX.ChID, label, action);
}

void CArinc429::getLatest(int label, void* data, int* seq_num)
{
	//ar_get_latest(m_arInfo.BoardID, m_arInfo.RX.ChID, label, data, seq_num);
}

int CArinc429::numTxChans()
{
	int num = ar_num_xchans(m_arInfo.BoardID);
	return num <= ARINC_MAX_CH_NUM ? num : 0;
}

int CArinc429::numRxChans()
{
	int num = ar_num_rchans(m_arInfo.BoardID);
	return num <= ARINC_MAX_CH_NUM ? num : 0;
}

bool CArinc429::SendWord(TXR_DATA_INFO & dataInfo)
{
	if (m_cancelSend)
	{
		return false;  // quit
	}

	if (putWord(dataInfo.word) != ARS_NORMAL)
	{
		//assert(false);
		return false;
	}

	DWORD msecs;

	m_mutexWrite.lock();

	if (m_arInfo.TX.Info.Dislpay)
	{
		dataInfo.count = m_arInfo.TX.Info.Count;
		m_arrayDataTx.push_back(dataInfo);
		m_arInfo.TX.Info.Count++;
	}
	else
	{
		m_arInfo.TX.Info.Count++;
	}

	msecs = m_arInfo.TX.Info.Period.Value;

	m_mutexWrite.unlock();

	if (msecs > 0)
	{
		Sleep(msecs);
	}

	return true;
}

bool CArinc429::workFirst()
{
	if (!m_cancelSend)
	{
		TXR_DATA_INFO txData = { 0 };
		if (m_arInfo.TX.Info.EnPeriod)			// 周期发送
		{
			m_mutexWrite.lock();

			DWORD nBegin = m_arInfo.TX.Info.Data[0];
			DWORD nEnd = m_arInfo.TX.Info.Data[1];
			DWORD delta = m_arInfo.TX.Info.EnCounts ? 1 : (1 << AR_WORD_DATA_OFFSET);

			m_mutexWrite.unlock();

			if (nBegin > nEnd)
			{
				for (LONGLONG i = nBegin; i >= nEnd; i -= delta)
				{
					txData.word = (DWORD)i;
					if (!SendWord(txData))
					{
						break;
					}
				}
			}
			else
			{
				for (LONGLONG i = nBegin; i <= nEnd; i += delta)
				{
					txData.word = (DWORD)i;
					if (!SendWord(txData))
					{
						break;
					}
				}
			}
		}
		else					// 单次发送
		{
			if (m_arInfo.TX.Info.EnSend)
			{
				m_mutexWrite.lock();
				txData.word = m_arInfo.TX.Info.Data[0];
				m_mutexWrite.unlock();

				if (!SendWord(txData))
				{
					assert(false);
				}

				m_mutexWrite.lock();
				m_arInfo.TX.Info.EnSend = FALSE;
				m_mutexWrite.unlock();
			}
		}

		m_mutexWrite.lock();
		m_cancelSend = TRUE;
		m_mutexWrite.unlock();
	}

	Sleep(5);

	return true;
}

bool CArinc429::workSecond()
{
	int count = 2048;
	TXR_DATA_INFO rxData = { 0 };

	while (count--)
	{
		if (m_cancelRecv)
		{
			break;
		}

		if (getNextt(&rxData.word, &rxData.timetag) != ARS_GOTDATA)
		{
			break;
		}

		m_mutexRead.lock();

		if (m_arInfo.RX.Info.Dislpay)
		{
			if (m_oldtimetag == 0)
			{
				rxData.timeDiff = 0;
			}
			else
			{
				rxData.timeDiff = (LONG)(rxData.timetag - m_oldtimetag) / 10;  // transform it to millisecond counter
			}

			m_oldtimetag = rxData.timetag;

			rxData.count = m_arInfo.RX.Info.Count;
			m_arrayDataRx.push_back(rxData);
			m_arInfo.RX.Info.Count++;

		}
		else
		{
			m_arInfo.RX.Info.Count++;
		}

		m_mutexRead.unlock();
	}

	Sleep(5);

	return true;
}
