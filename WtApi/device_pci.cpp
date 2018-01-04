
#include "stdafx.h"
#include <winioctl.h>
#include <setupapi.h>
#include "device_pci.h"
#include <assert.h>

#pragma comment(lib, "setupapi")

CPCIDevice::CPCIDevice(int deviceIndex) :
	CDevice(deviceIndex, _T(__FUNCTION__)),
	m_isOpen(FALSE),
	m_hDevice(NULL),
	m_waitEvtRead(new KWaitEvent(_T(__FUNCTION__)_T("-read"))),
	m_waitEvtWrite(new KWaitEvent(_T(__FUNCTION__)_T("-write"))),
	m_waitEvtWriteDone(new KWaitEvent(_T(__FUNCTION__)_T("-write-done")))
{
	ULONG nLength;
	HDEVINFO hDevInfo;
	DWORD memberIndex = 0;
	SP_DEVICE_INTERFACE_DATA devInterfaceData;

	hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_PCI429, NULL, NULL,
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE | DIGCF_ALLCLASSES);
	if (INVALID_HANDLE_VALUE == hDevInfo)
	{
		return;
	}

	while (TRUE)
	{
		devInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		if (!SetupDiEnumDeviceInterfaces(hDevInfo, 0, &GUID_DEVCLASS_PCI429, memberIndex, &devInterfaceData))
		{
			break;
		}

		DWORD nSize = 0;
		SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInterfaceData, NULL, 0, &nSize, NULL);
		if (nSize == 0)
		{
			continue;
		}

		PSP_DEVICE_INTERFACE_DETAIL_DATA devInterfaceDetailData;
		devInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LMEM_FIXED, nSize);
		if (NULL == devInterfaceDetailData)
		{
			break;
		}

		devInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		nLength = nSize;
		if (!SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInterfaceData,
			devInterfaceDetailData, nLength, &nSize, NULL))
		{
			LocalFree(devInterfaceDetailData);
			break;
		}

		m_deviceNames.push_back(devInterfaceDetailData->DevicePath);
		LocalFree(devInterfaceDetailData);
		memberIndex++;
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);
}

CPCIDevice::~CPCIDevice()
{
	this->close();

	CloseHandle(m_hDevice);

	delete m_waitEvtRead;
	delete m_waitEvtWrite;
	delete m_waitEvtWriteDone;
}

int CPCIDevice::boardType()
{
	return ARINC_BOARD_TYPE_PCI;
}

int CPCIDevice::loadslv(int base_reg, int base_port, int ram_size)
{
	IF_DEVICE_INDEX_OVR_RETURN_INVBOARD(this);

	if (m_isOpen)
	{
		return ARS_NORMAL;
	}

	// 打开设备
	m_hDevice = CreateFile(m_deviceNames[CDevice::deviceIndex()].c_str(),
		GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == m_hDevice)
	{
		m_hDevice = NULL;
		return ARS_WINRTFAIL;
	}

	// 复位事件
	m_waitEvtWrite->resetEvent();
	m_waitEvtRead->resetEvent();

	// 设置数据接收事件
	ULONG bytesReturned = 0;
	BOOL status = DeviceIoControl(m_hDevice, IOCTL_PCI429_SET_EVENT,
		&m_waitEvtRead->handle(), sizeof(PHANDLE), NULL, 0, &bytesReturned, NULL);
	if (status)
	{
		m_isOpen = TRUE;

		if (CDevice::loadslv(base_reg, base_port, ram_size) != ARS_NORMAL)
		{
			CloseHandle(m_hDevice);
			m_hDevice = NULL;
			status = FALSE;
		}

		// 清空FIFO，启动接收数据
		if (!resetDevice(ARINC_CODE_RESET_ON))
		{
			return ARS_WINRTFAIL;
		}
	}
	else
	{
		CloseHandle(m_hDevice);
		m_hDevice = NULL;
	}

	if (status)
	{
		return ARS_NORMAL;
	}
	else
	{
		m_isOpen = FALSE;
		return ARS_WINRTFAIL;
	}
}

int CPCIDevice::go()
{
	IF_DEVICE_INDEX_OVR_RETURN_INVBOARD(this);

	if (isRunning(KPairThread::PairSecond))
	{
		return ARS_NORMAL;
	}

	// 清空FIFO，启动接收数据
	if (!resetDevice(ARINC_CODE_RESET_ON))
	{
		return ARS_WINRTFAIL;
	}

	// 复位事件
	m_waitEvtWrite->resetEvent();
	m_waitEvtRead->resetEvent();

	// 
	CDevice::go();

	// 启动线程，进行数据传输
	return KPairThread::start(PairSecond) ? ARS_NORMAL : ARS_WINRTFAIL;
}

int CPCIDevice::close()
{
	IF_DEVICE_INDEX_OVR_RETURN_INVBOARD(this);

	if (!m_isOpen)
	{
		return ARS_NORMAL;
	}

	// 停止接收数据，清空FIFO
	if (!resetDevice(ARINC_CODE_RESET_OFF))
	{
		return ARS_WINRTFAIL;
	}

	// 退出线程，停止数据传输
	KPairThread::stop(PairSecond);

	// 复位事件
	m_waitEvtWrite->resetEvent();
	m_waitEvtRead->resetEvent();

	// 
	if (ARS_WINRTFAIL == CDevice::close())
	{
		return ARS_WINRTFAIL;
	}

	m_isOpen = FALSE;

	CloseHandle(m_hDevice);
	m_hDevice = NULL;

	return ARS_NORMAL;
}

int CPCIDevice::reset()
{
	IF_DEVICE_INDEX_OVR_RETURN_INVBOARD(this);

	if (!m_isOpen)
	{
		return ARS_WINRTFAIL;
	}

	// 停止接收数据，清空FIFO
	if (!resetDevice(ARINC_CODE_RESET_OFF))
	{
		return ARS_WINRTFAIL;
	}

	// 退出线程，停止数据传输
	KPairThread::stop(PairSecond);

	// 复位事件
	m_waitEvtWrite->resetEvent();
	m_waitEvtRead->resetEvent();
	
	return CDevice::reset();
}

BOOL CPCIDevice::readRegister(ULONG addr, ULONG* value, ARINC_REG_ADDR_TYPE type/* = ARINC_REG_ADDR_CORE*/)
{
	ULONG outBuff;
	ULONG bytesReturned = 0;

	addr += (type == ARINC_REG_ADDR_CORE) ? ARINC_CORE_REG_BASE : ARINC_USER_REG_BASE;

	BOOL status = DeviceIoControl(m_hDevice, IOCTL_PCI429_READ_REGISTER,
		&addr, sizeof(ULONG), &outBuff, sizeof(ULONG), &bytesReturned, NULL);
	if (status)
	{
		*value = outBuff;
	}

	return status;
}

BOOL CPCIDevice::writeRegister(ULONG addr, ULONG value, ARINC_REG_ADDR_TYPE type/* = ARINC_REG_ADDR_CORE*/)
{
	addr += (type == ARINC_REG_ADDR_CORE) ? ARINC_CORE_REG_BASE : ARINC_USER_REG_BASE;

	ULONG inBuff[2] = { addr, value };
	ULONG bytesReturned = 0;

	return DeviceIoControl(m_hDevice, IOCTL_PCI429_WRITE_REGISTER,
		inBuff, sizeof(ULONG)* 2, NULL, 0, &bytesReturned, NULL);
}

BOOL CPCIDevice::resetDevice(int code)
{
	ULONG dwParams[] = { (code == ARINC_CODE_RESET_ON) ? AR_ON : AR_OFF };
	ULONG bytesReturned = 0;

	if (DeviceIoControl(m_hDevice, IOCTL_PCI429_SET_INTERRUPT,
		dwParams, sizeof(ULONG)* 1, NULL, 0, &bytesReturned, NULL))
	{
		return CDevice::resetDevice(code);
	}
	else
	{
		return FALSE;
	}
}

BOOL CPCIDevice::enableLabel0(int channel, BOOL enable/* = TRUE*/)
{
	ULONG bytesReturned = 0;
	ULONG inBuffer[] = { (ULONG)channel, (ULONG)enable };

	if (DeviceIoControl(m_hDevice, IOCTL_PCI429_SET_LABEL0,
		inBuffer, sizeof(ULONG)* 2, NULL, 0, &bytesReturned, NULL))
	{
		return CDevice::enableLabel0(channel, enable);
	}
	else
	{
		return FALSE;
	}
}

bool CPCIDevice::workFirst()
{
	return false;
	/*
	WaitResult waitResult = waitObject(PairFirst, m_waitEvtWrite, INFINITE);

	switch (waitResult)
	{
	case WaitObjectFirst:	// 退出线程
	{
		m_waitEvtWrite->resetEvent();
		return false;
	}
	case WaitObject0:		// 有数据需要读取
	{
		ULONG bytesReturned = 0;
		KMutexLocker locker(&m_mutex);

		m_writeBuffer.Buffer = &m_writeData;

		::WriteFile(m_hDevice, &m_writeBuffer, sizeof(ATOM_TDATA_INFO), &bytesReturned, NULL);

		if (bytesReturned == sizeof(ATOM_TDATA_INFO))
		{
			m_waitEvtWrite->resetEvent();
			m_waitEvtWriteDone->setEvent();
		}
		else
		{
			::Sleep(5);		// 重写
		}

		break;
	}
	default: assert(false); break;
	}
	
	return true;*/
}

bool CPCIDevice::workSecond()
{
	WaitResult waitResult = waitObject(PairSecond, m_waitEvtRead, INFINITE);

	switch (waitResult)
	{
	case WaitObjectSecond:	// 退出线程
	{
		m_waitEvtRead->resetEvent();
		return false;
	}
	case WaitObject0:		// 有数据需要读取
	{
		ULONG recvCount = 0;

		::ReadFile(m_hDevice, m_atomBuffer, 512 * sizeof(ATOM_RDATA_INFO), &recvCount, NULL);

		if (recvCount != 512)
		{
			m_waitEvtRead->resetEvent();
		}

		if (recvCount > 0)
		{
			recvCount /= sizeof(ATOM_RDATA_INFO);

			ARINC_WORD_MSG wordMsg;

			for (ULONG i = 0; i < recvCount; i++)
			{
				wordMsg.data = m_atomBuffer[i].data;
				wordMsg.timestamp = m_atomBuffer[i].timestamp;
				CDevice::appendData(m_atomBuffer[i].channel, wordMsg);
				CDevice::callUserHandle();
			}
		}

		break;
	}
	case WaitFailed:
	default:
		assert(false);
		break;
	}

	::Sleep(5);

	return true;
}
