#pragma once

// CChTxOutput dialog

#include "utypes.h"

class CArinc429;

class CChTxOutput : public CBCGPDialog
{
	DECLARE_DYNAMIC(CChTxOutput)

public:
	CChTxOutput(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChTxOutput();

// Dialog Data
	enum { IDD = IDD_CHTX_OUTPUT };

	enum { 
		TIMER_TX_ID = 1,
		TIMER_TX_PERIOD = 50,

		TIMER_FLUSH_ID = 2,
		TIMER_FLUSH_PERIOD = 500,
	};

public:
	void InitializeArInfo(CArinc429* arApiEx);

	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeComboBaudrate();
	afx_msg void OnCbnSelchangeComboParity();
	afx_msg void OnCbnSelchangeComboLoopback();
	afx_msg void OnBnClickedButtonTxData();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnClose();

private:
#pragma warning(disable: 4996)
	void GetLocalTimeString(PLARGE_INTEGER timestamp)
	{
		FILETIME fileTime;
		SYSTEMTIME systemTime;
		fileTime.dwHighDateTime = timestamp->HighPart;
		fileTime.dwLowDateTime = timestamp->LowPart;
		FileTimeToSystemTime(&fileTime, &systemTime);

		sprintf(m_strSysTime, "%04d-%02d-%02d %02d:%02d:%02d %03d %03d", 
			systemTime.wYear,
			systemTime.wMonth,
			systemTime.wDay,
			systemTime.wHour,
			systemTime.wMinute,
			systemTime.wSecond,
			systemTime.wMilliseconds,
			(timestamp->QuadPart/10) % 1000);
	}

private:
	CBCGPListBox		m_listBoxData;

	// 定时器数据显示用的变量
	char				m_strOutput[128];
	char				m_strSysTime[128];
	CArinc429*			m_arApiEx;
	ARINC_INFO*			m_arInfo;
	TXR_DATA_INFO		m_txrData;

	CBCGPComboBox		m_comboBoxBaudRate;
	CBCGPComboBox		m_comboBoxParity;
	CBCGPComboBox		m_comboBoxLoopback;
};
