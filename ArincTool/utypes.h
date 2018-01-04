
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
	int						BaudRate;	// ������
	int						Parity;		// У�鷽ʽ

	int						Decoder;	// 
	ARINC429_SDI			SDI;		// 

	struct {
		DWORD				Reload;
		BOOL				Enable;
		struct {
			DWORD			Action;
		}					Data[ARINC_LABEL_NUM];
	}						Label;

	BOOL					Dislpay;	// TRUE/FALSE����ʾ/����ʾ���յ����ݵ������
	//BOOL					EnRecv;		// ʹ��/��ֹ���ͣ�FALSE:��ֹ, TRUE:ʹ��; way=0ʱ���ã�
	int						Function;	// ���պ�����0:�Զ�����; 1:�û�������
	int						DataFormat;	// ֡���ݸ�ʽ���˽��ƣ�ʮ���ƣ�ʮ�����ƣ�
	int						DataSplit;	// ���ݲ��
	ULONG					Count;		// �ۼƽ������ݸ���

} ARINC_INFO_RXCHN_INFO, *PARINC_INFO_RXCHN_INFO;

//! 
typedef struct _ARINC_INFO_TXCHN_INFO
{
	int						BaudRate;	// ������
	int						Loopback;	// �ڲ��ػ�����
	int						Parity;		// У�鷽ʽ

	DWORD					Data[2];	// ���͵�����
	struct {
		int		Key;
		DWORD	Value;
	}						Period;		// ��������
	BOOL					Dislpay;	// TRUE/FALSE����ʾ/����ʾ���͵����ݵ������
	BOOL					EnPeriod;	// ���ͷ�ʽ��0:������1:���ڣ�
	BOOL					EnCounts;	// �����ͷ�ʽΪ���ڷ�ʽʱ��1����Ϊ������ʽ��0������Data������ֵ
	BOOL					EnSend;		// ʹ��/��ֹ���ͣ�FALSE:��ֹ, TRUE:ʹ��; way=0ʱ���ã�
	int						DataFormat;	// ֡���ݸ�ʽ���˽��ƣ�ʮ���ƣ�ʮ�����ƣ�
	int						DataSplit;	// ���ݲ��
	ULONG					Count;		// �ۼƷ������ݸ���

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
	int						BoardID;	// �豸��

	ARINC_INFO_TX			TX;			// 
	ARINC_INFO_RX			RX;			// 

} ARINC_INFO, *PARINC_INFO;

#endif	// USER_TYPEDEF_H
