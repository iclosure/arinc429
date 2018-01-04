#pragma once

#include "device.h"
#include "CyAPI.h"
#include <deque>

//
#define USB429_PRTC_HEADER_WR_TIME		0xA1
#define USB429_PRTC_HEADER_RD_REG		0xA2
#define USB429_PRTC_HEADER_WR_REG		0xA3
#define USB429_PRTC_HEADER_WR_DATA		0xA4
#define USB429_PRTC_HEADER_RD_DATA		0xA5

#define USB429_REQCODE_RSTDEVICE		0xB3  // VEND_RSTDEVICE

#pragma pack(push, 1)

// 
typedef struct _USB429_REG_CTRL_ST
{
	struct {
		DWORD EnableLabel0;	// 
		DWORD BaudGear;		// 
		DWORD RxChNum;		// 
		DWORD TxChNum;		// 
	} u;

	struct {
		BYTE Ctrl[ARINC_MAX_CH_NUM][2];	//
	} c;

} USB429_REG_CTRL_ST, *PUSB429_REG_CTRL_ST;

// CPU Address Bit Positions ( )
typedef union _USB429_CORE_ADDR_ST
{
	struct {
		UINT16	ByteIndex : 2;	// Byte index
		UINT16	RegID : 2;		// Register index
		UINT16	TR : 1;			// TX/RX
		UINT16	ChID : 4;		// Channel Number
		UINT16	Spare : 7;		// 
	} bits;
	UINT16		all;			// All data

} USB429_CORE_ADDR_ST, *PUSB429_CORE_ADDR_ST;

// USB429传输数据结构 (13 bytes)
typedef struct _PUSB429_READ_PRTC
{
	struct {
		BYTE Type : 1;		// Type
		BYTE RegID : 2;		// Register index
		BYTE TR : 1;		// TX/RX
		BYTE ChID : 4;		// Channel Number
	}			addr;		// Address
	DWORD		data;		// Register content or Data
	INT64		timestamp;	// 时间戳 (单位：100ns) (8 bytes)

} USB429_READ_PRTC, *PUSB429_READ_PRTC;

// 发送寄存器结构信息 (5 bytes)
typedef struct _USB429_WRITE_PRTC
{
	BYTE addr;
	DWORD data;

} USB429_WRITE_PRTC, *PUSB429_WRITE_PRTC;

#pragma pack(pop)

class CUSBDevice : public CDevice, public CCyUSBDevice
{
public:
	CUSBDevice(int deviceIndex);
	~CUSBDevice();

	int boardType();
	int loadslv(int base_reg, int base_port, int ram_size);
	int go();
	int close();
	int reset();
	bool isOpen() { return CCyUSBDevice::IsOpen(); }

	int putWord(int channel, unsigned long value)
	{
		IF_DEVICE_INDEX_OVR_RETURN_INVBOARD(this);

		if (!CCyUSBDevice::IsOpen())
		{
			return ARS_WINRTFAIL;
		}

		KMutexLocker locker(&m_mutex);

		if (m_writeQueue.size() > 819)
		{
			return ARS_XMITOVRFLO;
		}

		USB429_WRITE_PRTC protocal = { (BYTE)((channel<<4) + 8), (DWORD)value };

		m_writeQueue.push_back(protocal);

		return ARS_NORMAL;
	}

	int putWords(int channel, unsigned long* buffer, int size, int* length)
	{
		IF_DEVICE_INDEX_OVR_RETURN_INVBOARD(this);

		if (!CCyUSBDevice::IsOpen())
		{
			return ARS_WINRTFAIL;
		}

		if (buffer == NULL)
		{
			assert(false);
			return ARS_INVHARVAL;
		}

		USB429_WRITE_PRTC protocal = { (BYTE)((channel << 4) + 8), 0 };
		int i;

		KMutexLocker locker(&m_mutex);

		for (i = 0; i < size; i++)
		{
			if (m_writeQueue.size() > 819)
			{
				break;
			}

			protocal.data = (DWORD)buffer[i];
			m_writeQueue.push_back(protocal);
		}

		if (length != NULL)
		{
			*length = i;
		}

		return ARS_NORMAL;
	}

	int deviceCount() { return CCyUSBDevice::DeviceCount(); }

private:
	HR_DISABLE_COPY(CUSBDevice)

	enum ARINC_REG_BASE : USHORT
	{
		ARINC_CORE_REG_BASE = 0x0000,
		ARINC_USER_REG_BASE = 0x8000
	};

	BOOL readRegister(ULONG addr, ULONG* value, ARINC_REG_ADDR_TYPE = ARINC_REG_ADDR_CORE);
	BOOL writeRegister(ULONG addr, ULONG value, ARINC_REG_ADDR_TYPE = ARINC_REG_ADDR_CORE);
	BOOL resetDevice(int code);
	BOOL enableLabel0(int channel, BOOL enable = TRUE);
	BOOL logCurrentTime();

	inline bool workFirst();		// Send
	inline bool workSecond();		// Receive

private:
	LONGLONG			m_sysStartupTime;
	OVERLAPPED			m_writeOvLap;
	OVERLAPPED			m_readOvLap;
	KWaitEvent*			m_waitEvtWrite;
	KWaitEvent*			m_waitEvtRead;
	USB429_REG_CTRL_ST	m_regCtrl;
	USB429_WRITE_PRTC	m_bufferWrite[204];
	int					m_bufferRevCount;
	USB429_READ_PRTC	m_bufferRead[78];
	KMutex				m_mutex;
	std::deque<USB429_WRITE_PRTC> m_writeQueue;
};
