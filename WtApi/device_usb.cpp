#include "stdafx.h"
#include <winioctl.h>
#include <InitGuid.h>
#include "device_usb.h"
#include <tchar.h>
#include <assert.h>

// 导入CyAPI库
#pragma comment(linker,"/NODEFAULTLIB:libcmt.lib")
#pragma comment(lib, "CyAPI.lib")

// {83AFE57F-53D9-4B85-A7E2-C9C0F1D48C9F}
DEFINE_GUID(GUID_DEVCLASS_USB429,
	0x83afe57f, 0x53d9, 0x4b85, 0xa7, 0xe2, 0xc9, 0xc0, 0xf1, 0xd4, 0x8c, 0x9f);

CUSBDevice::CUSBDevice(int deviceIndex) :
	CDevice(deviceIndex, _T(__FUNCTION__)_T("-thread")),
	CCyUSBDevice(NULL, GUID_DEVCLASS_USB429, FALSE)
{
	m_sysStartupTime = 0;
	m_bufferRevCount = 0;
	RtlZeroMemory(&m_writeOvLap, sizeof(m_writeOvLap));
	RtlZeroMemory(&m_readOvLap, sizeof(m_readOvLap));
	RtlZeroMemory(&m_regCtrl, sizeof(USB429_REG_CTRL_ST));

	m_waitEvtWrite = new KWaitEvent(_T("USB429_BULK_OUT"));
	m_waitEvtRead = new KWaitEvent(_T("USB429_BULK_IN"));
	m_writeOvLap.hEvent = m_waitEvtWrite;
	m_readOvLap.hEvent = m_waitEvtRead;
}

CUSBDevice::~CUSBDevice(void)
{
	this->close();
	delete m_waitEvtWrite;
	delete m_waitEvtRead;
}

int CUSBDevice::boardType()
{
	return ARINC_BOARD_TYPE_USB;
}

int CUSBDevice::loadslv(int base_reg, int base_port, int ram_size)
{
	IF_DEVICE_INDEX_OVR_RETURN_INVBOARD(this);

	if (CCyUSBDevice::IsOpen())
	{
		CCyUSBDevice::Close();
	}

	if (!CCyUSBDevice::Open(CDevice::deviceIndex()))
	{
		return ARS_WINRTFAIL;
	}

	if (NULL == CCyUSBDevice::ControlEndPt ||
		NULL == CCyUSBDevice::BulkInEndPt ||
		NULL == CCyUSBDevice::BulkOutEndPt)
	{
		assert(false);
		CCyUSBDevice::Close();
		return ARS_WINRTFAIL;
	}

	CCyUSBDevice::ControlEndPt->TimeOut = 1000;
	CCyUSBDevice::BulkInEndPt->TimeOut = 1000;
	CCyUSBDevice::BulkOutEndPt->TimeOut = 1000;

	// 记录当前系统时间及硬件时间计数值
	if (!logCurrentTime())
	{
		assert(false);
		CCyUSBDevice::Close();
		return ARS_WINRTFAIL;
	}

	if (CDevice::loadslv(base_reg, base_port, ram_size) != ARS_NORMAL)
	{
		CCyUSBDevice::Close();
		return ARS_WINRTFAIL;
	}

	// 启动读写
	KPairThread::start(PairAll);

	return ARS_NORMAL;
}

int CUSBDevice::go()
{
	IF_DEVICE_INDEX_OVR_RETURN_INVBOARD(this);

	if (!CCyUSBDevice::IsOpen())
	{
		return ARS_WINRTFAIL;
	}

	// 清空FIFO，启动接收数据
	if (!resetDevice(ARINC_CODE_RESET_ON))
	{
		return ARS_WINRTFAIL;
	}

	// 
	m_bufferRevCount = 0;

	// 
	int result = CDevice::go();
	if (result == ARS_NORMAL)
	{
		KPairThread::start(PairAll);
	}

	return result;
}

int CUSBDevice::close()
{
	IF_DEVICE_INDEX_OVR_RETURN_INVBOARD(this);

	if (!CCyUSBDevice::IsOpen())
	{
		return ARS_WINRTFAIL;
	}

	// 退出读写
	KPairThread::stop(PairAll);

	// 停止接收数据，清空FIFO
	if (!resetDevice(ARINC_CODE_RESET_CLOSE))
	{
		return ARS_WINRTFAIL;
	}

	// 关闭设备
	if (ARS_WINRTFAIL == CDevice::close())
	{
		return ARS_WINRTFAIL;
	}

	// 
	CCyUSBDevice::Close();

	//
	m_mutex.lock();
	m_writeQueue.clear();
	m_mutex.unlock();

	return ARS_NORMAL;
}

int CUSBDevice::reset()
{
	IF_DEVICE_INDEX_OVR_RETURN_INVBOARD(this);

	if (!CCyUSBDevice::IsOpen())
	{
		return ARS_WINRTFAIL;
	}

	// 退出读写
	KPairThread::stop(PairAll);

	// 停止接收数据，清空FIFO
	if (!resetDevice(ARINC_CODE_RESET_OFF))
	{
		return ARS_WINRTFAIL;
	}

	// 
	if (CDevice::reset() != ARS_NORMAL)
	{
		return ARS_WINRTFAIL;
	}

	// 复位设备
	if (!CCyUSBDevice::Reset())
	{
		return ARS_WINRTFAIL;
	}

	// 
	m_mutex.lock();
	m_writeQueue.clear();
	m_mutex.unlock();

	return ARS_NORMAL;
}

inline BOOL CUSBDevice::readRegister(ULONG addr, ULONG* value, ARINC_REG_ADDR_TYPE type/* = ARINC_REG_ADDR_CORE*/)
{
	switch (type)
	{
	case CDevice::ARINC_REG_ADDR_CORE:
	{
		USB429_CORE_ADDR_ST coreAddr;

		coreAddr.all = (UINT16)addr;

		switch (coreAddr.bits.RegID)
		{
		case ARINC_REG_CTRL_ID: *value = m_regCtrl.c.Ctrl[coreAddr.bits.ChID][coreAddr.bits.TR]; break;
		default: assert(false); return FALSE;
		}

		break;
	}
	case CDevice::ARINC_REG_ADDR_USER:
	{
		switch (addr)
		{
		case ARINC_USER_REG_LABEL0:
			m_regCtrl.u.EnableLabel0 = CDevice::getEnableLabel0();
			*value = m_regCtrl.u.EnableLabel0;
			break;
		case ARINC_USER_REG_BAUDGEAR: *value = m_regCtrl.u.BaudGear; break;
		case ARINC_USER_REG_RCHNUM: *value = m_regCtrl.u.RxChNum; break;
		case ARINC_USER_REG_TCHNUM: *value = m_regCtrl.u.TxChNum; break;
		default: assert(false); return FALSE;
		}

		break;
	}
	default: return FALSE;
	}

	return TRUE;
}

inline BOOL CUSBDevice::writeRegister(ULONG addr, ULONG value, ARINC_REG_ADDR_TYPE type/* = ARINC_REG_ADDR_CORE*/)
{
	switch (type)
	{
	case CDevice::ARINC_REG_ADDR_CORE:
	{
		USB429_CORE_ADDR_ST coreAddr;

		coreAddr.all = (UINT16)addr;

		switch (coreAddr.bits.RegID)
		{
		case ARINC_REG_CTRL_ID: m_regCtrl.c.Ctrl[coreAddr.bits.ChID][coreAddr.bits.TR] = (BYTE)value; break;
		case ARINC_REG_LABEL_ID: break;
		default: assert(false); return FALSE;
		}

		break;
	}
	case CDevice::ARINC_REG_ADDR_USER:
	{
		switch (addr)
		{
		case ARINC_USER_REG_LABEL0:
			m_regCtrl.u.EnableLabel0 = value;
			CDevice::setEnableLabel0(m_regCtrl.u.EnableLabel0);
			break;
		case ARINC_USER_REG_BAUDGEAR: m_regCtrl.u.BaudGear = value; break;
		case ARINC_USER_REG_RCHNUM: m_regCtrl.u.RxChNum = value; break;
		case ARINC_USER_REG_TCHNUM: m_regCtrl.u.TxChNum = value; break;
		default: assert(false); return FALSE;
		}

		addr |= ARINC_BYTE_INDEX_USER;

		break;
	}
	default: return FALSE;
	}

	USB429_WRITE_PRTC protocal = { (BYTE)(addr >> 1), value };

	m_mutex.lock();
	m_writeQueue.push_back(protocal);
	m_mutex.unlock();

	return TRUE;
}

BOOL CUSBDevice::resetDevice(int code)
{
	LONG transCount;

	USHORT data = 0;
	transCount = 2;
	CCyUSBDevice::ControlEndPt->Target = TGT_ENDPT;
	CCyUSBDevice::ControlEndPt->Direction = DIR_TO_DEVICE;
	CCyUSBDevice::ControlEndPt->ReqType = REQ_VENDOR;
	CCyUSBDevice::ControlEndPt->ReqCode = USB429_REQCODE_RSTDEVICE;
	CCyUSBDevice::ControlEndPt->Value = 0xC0 | code;
	CCyUSBDevice::ControlEndPt->Index = 0;
	if (!CCyUSBDevice::ControlEndPt->Write((PUCHAR)&data, transCount))
	{
		assert(false);
		return FALSE;
	}

	return TRUE;
}

BOOL CUSBDevice::enableLabel0(int channel, BOOL enable/* = TRUE*/)
{
	if (CDevice::enableLabel0(channel, enable))
	{
		return writeRegister(ARINC_USER_REG_LABEL0, CDevice::getEnableLabel0(), ARINC_REG_ADDR_USER);
	}
	else
	{
		assert(false);		// TODO
		return FALSE;
	}
}

BOOL CUSBDevice::logCurrentTime()
{
	//
	if (!resetDevice(ARINC_CODE_RESET_CLOSE))
	{
		return FALSE;
	}

	// 获取系统时间
	SYSTEMTIME sysTime;
	FILETIME fileTime;
	GetLocalTime(&sysTime);
	SystemTimeToFileTime(&sysTime, &fileTime);

	BYTE buffer[256];
	LONG transCount;
	PUCHAR context;

	// 发送数据
	buffer[0] = USB429_PRTC_HEADER_WR_TIME;
	RtlCopyMemory(&buffer[1], &fileTime, sizeof(FILETIME));
	transCount = 1 + sizeof(FILETIME);
	if (!CCyUSBDevice::BulkOutEndPt->XferData(buffer, transCount))
	{
		return FALSE;		// 调用失败
	}

	if (transCount != 1 + sizeof(FILETIME))
	{
		return FALSE;		// 发送数据量不对
	}

	// 接收数据
	RtlZeroMemory(buffer, 256);
	transCount = 0;
	OVERLAPPED inOvLap;
	RtlZeroMemory(&inOvLap, sizeof(inOvLap));
	inOvLap.hEvent = CreateEvent(NULL, FALSE, FALSE, _T("USB429_BULK_IN"));
	context = CCyUSBDevice::BulkInEndPt->BeginDataXfer(buffer, sizeof(USB429_REG_CTRL_ST), &inOvLap);
	CCyUSBDevice::BulkInEndPt->WaitForXfer(&inOvLap, 1000);  // ? ms
	CCyUSBDevice::BulkInEndPt->FinishDataXfer(buffer, transCount, &inOvLap, context);

	if (transCount != sizeof(USB429_REG_CTRL_ST))
	{
		assert(false);
		return FALSE;
	}

	RtlCopyMemory(&m_regCtrl, buffer, sizeof(USB429_REG_CTRL_ST));

	CDevice::setEnableLabel0(m_regCtrl.u.EnableLabel0);

	return TRUE;
}

bool CUSBDevice::workFirst()
{
	if (!m_writeQueue.empty())
	{
		int i;

		m_mutex.lock();

		for (i = m_bufferRevCount; i < 204 && !m_writeQueue.empty(); i++)
		{
			m_bufferWrite[i] = m_writeQueue.front();
			m_writeQueue.pop_front();
		}

		m_mutex.unlock();

		m_bufferRevCount = 0;

		PUCHAR context = CCyUSBDevice::BulkOutEndPt->BeginDataXfer((PUCHAR)m_bufferWrite, i * sizeof(USB429_WRITE_PRTC), &m_writeOvLap);

		WaitResult waitResult = waitObject(PairFirst, m_waitEvtWrite, INFINITE);

		switch (waitResult)
		{
		case WaitObjectFirst:	// 退出线程
		{
			CCyUSBDevice::BulkOutEndPt->Abort();
			CCyUSBDevice::BulkOutEndPt->Reset();
			return false;
		}
		case WaitObject0:		// 获取到数据
		{
			LONG transCount = 0;

			CCyUSBDevice::BulkOutEndPt->FinishDataXfer(NULL, transCount, &m_writeOvLap, context);

#if defined(_DEBUG)
			if (transCount % sizeof(USB429_WRITE_PRTC))
			{
				assert(false);
			}
#endif  // _DEBUG

			m_bufferRevCount = i - transCount / sizeof(USB429_WRITE_PRTC);

			if (m_bufferRevCount > 0)
			{
				assert(false);	// TODO
				RtlMoveMemory(m_bufferWrite, &m_bufferWrite[transCount / sizeof(USB429_WRITE_PRTC)], m_bufferRevCount);
			}

			break;
		}
		case WAIT_FAILED:
		default:
			assert(false);
			break;
		}
	}

	Sleep(5);

	return true;
}

bool CUSBDevice::workSecond()
{
	PUCHAR context = CCyUSBDevice::BulkInEndPt->BeginDataXfer((PUCHAR)m_bufferRead, sizeof(m_bufferRead), &m_readOvLap);

	WaitResult waitResult = waitObject(PairSecond, m_waitEvtRead, INFINITE);

	switch (waitResult)
	{
	case WaitObjectSecond:		// 退出线程
	{
		CCyUSBDevice::BulkInEndPt->Abort();
		CCyUSBDevice::BulkInEndPt->Reset();
		return false;
	}
	case WaitObject0:			// 获取到数据
	{
		LONG transCount = 0;

		CCyUSBDevice::BulkInEndPt->FinishDataXfer(NULL, transCount, &m_readOvLap, context);

#if defined(_DEBUG)
		if (transCount % sizeof(USB429_READ_PRTC))
		{
			assert(false);
		}
#endif  // _DEBUG

		transCount /= sizeof(USB429_READ_PRTC);

		USB429_READ_PRTC protocal;
		ARINC_WORD_MSG wordMsg;

		for (int i = 0; i < transCount; i++)
		{
			protocal = m_bufferRead[i];
			wordMsg.data = protocal.data;
			wordMsg.timestamp = protocal.timestamp;
			CDevice::appendData(protocal.addr.ChID, wordMsg);
			CDevice::callUserHandle();
		}

		break;
	}
	case WAIT_FAILED:
	default:
		assert(false);
		break;
	}

	Sleep(5);

	return true;
}
