
#ifndef USER_TYPEDEF_H
#define USER_TYPEDEF_H

#include "arapi.h"

// 
typedef union _ARINC429_SDI
{
	struct {
		int			BIT9 : 1,	//
					BIT10 : 1;	//
	} bits;
	int				all;
} ARINC429_SDI;

// 
typedef struct _TXR_DATA_INFO
{
	union {
		struct {
			DWORD	LABEL : 8,
					SDI : 2,
					DATA : 19,
					SSM : 2,
					PARITY : 1;
		};

		DWORD		word;
	};

	ULONG			channel;
	ULONG			count;
	LONGLONG		timetag;
	LONG			timeDiff;	// 
} TXR_DATA_INFO, *PTXR_DATA_INFO;

//! 
typedef struct _ARINC_INFO_RXCHN_INFO
{
	int						BaudRate;	// 波特率
	int						Parity;		// 校验方式

	int						Decoder;	// 
	ARINC429_SDI			SDI;		// 

	struct {
		DWORD				Reload;
		BOOL				Enable;
		struct {
			DWORD			Action;
		}					Data[ARINC_LABEL_NUM];
	}						Label;

	BOOL					Dislpay;	// TRUE/FALSE：显示/不显示接收的数据到输出栏
	//BOOL					EnRecv;		// 使能/禁止发送（FALSE:禁止, TRUE:使能; way=0时可用）
	int						Function;	// 接收函数（0:自动接收; 1:用户函数）
	int						DataFormat;	// 帧数据格式（八进制；十进制；十六进制）
	int						DataSplit;	// 数据拆解
	ULONG					Count;		// 累计接收数据个数

} ARINC_INFO_RXCHN_INFO, *PARINC_INFO_RXCHN_INFO;

//! 
typedef struct _ARINC_INFO_TXCHN_INFO
{
	int						BaudRate;	// 波特率
	int						Loopback;	// 内部回环测试
	int						Parity;		// 校验方式

	DWORD					Data[2];	// 发送的数据
	struct {
		int		Key;
		DWORD	Value;
	}						Period;		// 发送周期
	BOOL					Dislpay;	// TRUE/FALSE：显示/不显示发送的数据到输出栏
	BOOL					EnPeriod;	// 发送方式（0:触发、1:周期）
	BOOL					EnCounts;	// 当发送方式为周期方式时，1代表为计数方式，0代表以Data的连续值
	BOOL					EnSend;		// 使能/禁止发送（FALSE:禁止, TRUE:使能; way=0时可用）
	int						DataFormat;	// 帧数据格式（八进制；十进制；十六进制）
	int						DataSplit;	// 数据拆解
	ULONG					Count;		// 累计发送数据个数

} ARINC_INFO_TXCHN_INFO, *PARINC_INFO_TXCHN_INFO;

typedef struct _AR_TX_CHANNEL_INFO
{
	int						ChNum;		// ARINC_MAX_CH_NUM
	int						ChID;		// 0 ~ ARINC_MAX_CH_NUM - 1
	ARINC_INFO_TXCHN_INFO	Info;		// 

} ARINC_INFO_TX, *PARINC_INFO_TX;

//! 
typedef struct _AR_RX_CHANNEL_INFO
{
	int						ChNum;		// ARINC_MAX_CH_NUM
	int						ChID;		// 0 ~ ARINC_MAX_CH_NUM - 1

	ARINC_INFO_RXCHN_INFO	Info;		// 

} ARINC_INFO_RX, *PARINC_INFO_RX;

//! 
typedef struct _ARINC_INFO
{
	int						BoardID;	// 设备号

	ARINC_INFO_TX			TX;			// 
	ARINC_INFO_RX			RX;			// 

} ARINC_INFO, *PARINC_INFO;

#endif	// USER_TYPEDEF_H
