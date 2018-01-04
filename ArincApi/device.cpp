
#include "stdafx.h"
#include "device.h"

CDevice::CDevice(int deviceIndex, tstring name) :
	KPairThread(name.empty() ? _T(__FUNCTION__) : name),
	m_deviceIndex(deviceIndex),
	m_txChNum(0),
	m_rxChNum(0),
	m_enableLabel0(-1),
	m_labelTables(NULL),
	m_onDataRecv(NULL),
	m_channelRxMsg(NULL)
{
}

CDevice::~CDevice()
{
	this->close();

	if (m_channelRxMsg != NULL)
	{
		delete[] m_channelRxMsg;
	}

	if (m_labelTables != NULL)
	{
		delete[] m_labelTables;
	}
}

int CDevice::boardType()
{
	return ARINC_BOARD_TYPE_UNKNOWN;
}

int CDevice::setUserHandler(UserHandle userHandler)
{
	KMutexLocker locker(&m_mutex);
	
	if (this->isOpen())
	{
		m_onDataRecv = userHandler;
		return FALSE;
	}
	else
	{
		m_onDataRecv = NULL;
		return TRUE;
	}
}

int CDevice::loadslv(int base_reg, int base_port, int ram_size)
{
	// 获取通道数
	m_txChNum = numTChans();
	m_rxChNum = numRChans();

	if (m_txChNum <= 0 || m_txChNum > ARINC_MAX_CH_NUM
		|| m_rxChNum <= 0 || m_rxChNum > ARINC_MAX_CH_NUM)
	{
		m_txChNum = 0;
		m_rxChNum = 0;

		return ARS_WINRTFAIL;
	}
	else
	{
		// 初始化数据接收缓冲区
		m_channelRxMsg = new ARINC_WORD_MSG_ARRAY[m_rxChNum];

		// LABEL
		m_labelTables = new UCHAR[m_rxChNum][ARINC_LABEL_NUM];

		for (int i = 0; i < m_rxChNum; i++)
		{
			setLabelFilter(i, ARU_ALL_LABELS, ARU_FILTER_OFF);
		}
	}

	return ARS_NORMAL;
}

int CDevice::go()
{
	return ARS_NORMAL;
}

int CDevice::close()
{
	// 释放资源
	if (m_channelRxMsg != NULL)
	{
		delete[] m_channelRxMsg;
		m_channelRxMsg = NULL;
	}

	if (m_labelTables != NULL)
	{
		delete[] m_labelTables;
		m_labelTables = NULL;
	}

	return ARS_NORMAL;
}

int CDevice::reset()
{
	// 清空缓存
	clearData();

	return ARS_NORMAL;
}

inline BOOL CDevice::resetDevice(int code)
{
	clearData();

	return TRUE;
}

inline BOOL CDevice::enableLabel0(int channel, BOOL enable/* = TRUE*/)
{
	ULONG _enable = enable ? 1 : 0;

	CONFIG_REG_BIT_ULONG(m_enableLabel0, channel, _enable);

	return ARS_NORMAL; 
}
