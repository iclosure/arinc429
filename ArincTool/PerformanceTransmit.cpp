#include "stdafx.h"
#include "PerformanceTransmit.h"
#include <arapi.h>
#include <assert.h>

CPerformanceTransmit::CPerformanceTransmit() :
KPairThread(this)
{
	setObjectName(_T(__FUNCTION__)_T("-thread"));

	m_boardConfig.BoardCount = ar_board_num();
	m_boardConfig.Id = 0;
	m_boardConfig.BaudRate = AR_100K;		// 100 Kbps
	m_boardConfig.Parity = AR_NONE;			// 无校验位
	m_boardConfig.Loopback = AR_WRAP_ON;	// 回环模式
	m_boardConfig.SamplePeriod = 1000;		// 采样周期

	m_mmTimer.set(TIME_PERIODIC, m_boardConfig.SamplePeriod, MmTimerEventCount, this);
}

CPerformanceTransmit::~CPerformanceTransmit()
{
	this->close();
}

bool CPerformanceTransmit::load()
{
	// 加载板卡
	int status = ar_loadslv(m_boardConfig.Id, 0, 0, 0);
	if (status != ARS_NORMAL)
	{
		return false;
	}

	// 复位板卡
	status = ar_reset(m_boardConfig.Id);
	if (status != ARS_NORMAL)
	{
		return false;
	}

	// 获取接收端通道数
	int rchans = ar_num_rchans(m_boardConfig.Id);
	if (rchans > ARINC_MAX_CH_NUM)
	{
		rchans = 0;
	}
	m_boardConfig.Channel.resize(rchans);

	// 设置默认配置信息

	ar_set_config(m_boardConfig.Id, ARU_RECV_RATE, m_boardConfig.BaudRate);
	ar_set_config(m_boardConfig.Id, ARU_XMIT_RATE, m_boardConfig.BaudRate);
	ar_set_config(m_boardConfig.Id, ARU_PARITY, m_boardConfig.Parity);
	ar_set_config(m_boardConfig.Id, ARU_INTERNAL_WRAP, m_boardConfig.Loopback);

	for (size_t i = 0; i < m_boardConfig.Channel.size(); i++)
	{
		m_boardConfig.Channel[i].LastWriteCount = 0;
		m_boardConfig.Channel[i].LastReadCount = 0;

		ar_label_filter(m_boardConfig.Id, i, ARU_ALL_LABELS, ARU_FILTER_OFF);
		ar_set_config(m_boardConfig.Id, ARU_CH01_DECODER + i, AR_OFF);
		ar_set_config(m_boardConfig.Id, ARU_CH01_SDI + i, 0x00);
	}

	return true;
}

bool CPerformanceTransmit::open()
{
	// 启动设备
	int status = ar_go(m_boardConfig.Id);
	if (status != ARS_NORMAL)
	{
		return false;
	}

	for (size_t i = 0; i < m_boardConfig.Channel.size(); i++)
	{
		m_boardConfig.Channel[i].LastWriteCount = 0;
		m_boardConfig.Channel[i].LastReadCount = 0;
	}

	// 复位
	this->reset();

	return true;
}

void CPerformanceTransmit::reset()
{
	KMutexLocker locker(&m_mutex);

	for (size_t i = 0; i < m_boardConfig.Channel.size(); i++)
	{
		m_boardConfig.Channel[i].WriteCount = 0;
		m_boardConfig.Channel[i].ReadCount = 0;
		m_boardConfig.Channel[i].LastWriteCount = 0;
		m_boardConfig.Channel[i].LastReadCount = 0;
		m_boardConfig.Channel[i].SampleWriteCount.clear();
		m_boardConfig.Channel[i].SampleReadCount.clear();
	}
}

void CPerformanceTransmit::close()
{
	enableSend(FALSE);
	enableRecv(FALSE);

	int status = ar_close(m_boardConfig.Id);
	if (ARS_NORMAL == status)
	{
		KPairThread::stop();
		this->reset();
		m_mmTimer.stop();
	}
}

bool CPerformanceTransmit::start(bool stopRecv)
{
	for (size_t i = 0; i < m_boardConfig.Channel.size(); i++)
	{
		m_boardConfig.Channel[i].LastWriteCount = 0;
		m_boardConfig.Channel[i].LastReadCount = 0;
	}

	// 复位
	this->reset();

	enableSend();
	enableRecv();
	m_mmTimer.start();

	// 启动线程
	return KPairThread::start();
}

bool CPerformanceTransmit::stop(bool stopRecv)
{
	enableSend(FALSE);
	if (stopRecv)
	{
		enableRecv(FALSE);
	}

	KPairThread::stop(PairFirst);

	if (stopRecv)
	{
		KPairThread::stop(PairSecond);
		m_mmTimer.stop();
	}

	return true;
}

void CPerformanceTransmit::enableSend(BOOL enable /* = TRUE */)
{
	KMutexLocker locker(&m_mutex);
	m_cancelSend = !enable;
}

void CPerformanceTransmit::enableRecv(BOOL enable/* = TRUE*/)
{
	KMutexLocker locker(&m_mutex);
	m_cancelRecv = !enable;
}

unsigned long CPerformanceTransmit::getWriteCount(int channel)
{
	assert(0 <= channel && channel < (int)m_boardConfig.Channel.size());
	KMutexLocker locker(&m_mutex);
	return m_boardConfig.Channel[channel].WriteCount;
}

unsigned long CPerformanceTransmit::getReadCount(int channel)
{
	assert(0 <= channel && channel < (int)m_boardConfig.Channel.size());
	KMutexLocker locker(&m_mutex);
	return m_boardConfig.Channel[channel].ReadCount;
}

bool CPerformanceTransmit::isEmptyDeltaWriteCount(int channel)
{
	assert(0 <= channel && channel < (int)m_boardConfig.Channel.size());
	KMutexLocker locker(&m_mutex);
	return m_boardConfig.Channel[channel].SampleWriteCount.empty();
}

bool CPerformanceTransmit::isEmptyDeltaReadCount(int channel)
{
	assert(0 <= channel && channel < (int)m_boardConfig.Channel.size());
	KMutexLocker locker(&m_mutex);
	return m_boardConfig.Channel[channel].SampleReadCount.empty();
}

long CPerformanceTransmit::getDeltaWriteCount(int channel)
{
	assert(0 <= channel && channel < (int)m_boardConfig.Channel.size());

	KMutexLocker locker(&m_mutex);
	BOARD_PER_CHANNEL& channelInfo = m_boardConfig.Channel[channel];
	unsigned long curWriteCount = channelInfo.SampleWriteCount[0];
	unsigned long delta;

	channelInfo.SampleWriteCount.pop_front();
	delta = curWriteCount - channelInfo.LastWriteCount;
	channelInfo.LastWriteCount = curWriteCount;

	return delta * (32 + 4) / m_boardConfig.SamplePeriod;
}

long CPerformanceTransmit::getDeltaReadCount(int channel)
{
	assert(0 <= channel && channel < (int)m_boardConfig.Channel.size());

	KMutexLocker locker(&m_mutex);
	BOARD_PER_CHANNEL& channelInfo = m_boardConfig.Channel[channel];
	unsigned long curReadCount = channelInfo.SampleReadCount[0];
	unsigned long delta;

	channelInfo.SampleReadCount.pop_front();
	delta = curReadCount - channelInfo.LastReadCount;
	channelInfo.LastReadCount = curReadCount;

	//return delta * (32 + 4) / m_boardConfig.SamplePeriod;
	return delta * 1000 / m_boardConfig.SamplePeriod;
	//return m_boardConfig.SamplePeriod * 1000.0 / delta;
}

bool CPerformanceTransmit::workFirst()
{
	int count = 512;

	while (count--)
	{
		for (size_t i = 0; i < m_boardConfig.Channel.size(); i++)
		{
			if (m_cancelSend)
			{
				return false;
			}

			if (ar_putword(m_boardConfig.Id, i, rand()) != ARS_NORMAL)
			{
				//assert(false);
				return false;
			}

			m_mutex.lock();
			m_boardConfig.Channel[i].WriteCount++;
			m_mutex.unlock();
		}
	}

	Sleep(5);

	return true;
}

bool CPerformanceTransmit::workSecond()
{
	int count = 1024;
	unsigned long data;

	while (count--)
	{
		for (size_t i = 0; i < m_boardConfig.Channel.size(); i++)
		{
			if (m_cancelRecv)
			{
				return false;
			}

			if (ar_getnext(m_boardConfig.Id, i, &data) != ARS_GOTDATA)
			{
				continue;
			}

			m_mutex.lock();
			m_boardConfig.Channel[i].ReadCount++;
			m_mutex.unlock();
		}
	}

	Sleep(5);

	return true;
}

void WINAPI CPerformanceTransmit::MmTimerEventCount(PVOID dwUser)
{
	CPerformanceTransmit* _this = reinterpret_cast<CPerformanceTransmit*>(dwUser);
	assert(_this);

	BOARD_PER_CONFIG &boardConfig = _this->m_boardConfig;

	for (size_t i = 0; i < boardConfig.Channel.size(); i++)
	{
		if (_this->m_cancelRecv)
		{
			break;
		}

		BOARD_PER_CHANNEL& channel = boardConfig.Channel[i];
		_this->m_mutex.lock();
		channel.SampleWriteCount.push_back(channel.WriteCount);
		channel.SampleReadCount.push_back(channel.ReadCount);
		_this->m_mutex.unlock();
	}
}
