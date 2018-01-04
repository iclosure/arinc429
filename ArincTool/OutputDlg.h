#pragma once
#include "afxcmn.h"

#include "utypes.h"

// COutputDlg dialog

class COutputDlg : public CSkinDialog
{
	DECLARE_DYNAMIC(COutputDlg)

public:
	COutputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COutputDlg();

// Dialog Data
	enum { IDD = IDD_OUTPUT };

	enum CFM_INDEX
	{
		CFM_INDEX_START_LINE = __LINE__ + 1,
		CFM_INDEX_ANS = 0,
		CFM_INDEX_CMD,
		CFM_INDEX_COUNT = __LINE__ - CFM_INDEX_START_LINE
	};

	enum CHANNEL_INDEX
	{
		CHANNEL_INDEX_START_LINE = __LINE__ + 1,
		CHANNEL_INDEX_TX = 0,
		CHANNEL_INDEX_RX,
		CHANNEL_INDEX_COUNT = __LINE__ - CHANNEL_INDEX_START_LINE
	};

	enum { 
		TIMER_TX_ID = 1,
		TIMER_TX_PERIOD = 50,

		TIMER_RX_ID = 2,
		TIMER_RX_PERIOD = 50,
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeComboTxDataFormat();
	afx_msg void OnCbnSelchangeComboRxDataFormat();
	afx_msg void OnCbnSelchangeComboTxDataSplit();
	afx_msg void OnCbnSelchangeComboRxDataSplit();
	afx_msg void OnBnClickedBnTxClearCache();
	afx_msg void OnBnClickedBnRxClearCache();

	afx_msg LRESULT	OnUserCleanTxt(WPARAM wParam, LPARAM lParam);	// WM_USER_CLEAN_TXT

public:
	void MyLogOut(UINT index, LPCTSTR text);
	void EnableDisplay(BOOL enable = TRUE);

	void TurnOn(BOOL enableDisplay = TRUE);
	void TurnOff(BOOL enableDisplay = TRUE);

	void SetArApiEx(CArinc429* arApiExTx, CArinc429* arApiExRx);

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
	CHARFORMAT  		m_fontLog;			// 
	CRichEditCtrl		m_rEditLog[CHANNEL_INDEX_COUNT];
	CBCGPListBox		m_listBoxDataTx;
	CBCGPListBox		m_listBoxDataRx;

	// 定时器数据显示用的变量
	char				m_strOutput[128];
	char				m_strSysTime[128];
	CArinc429*			m_arApiExTx;
	CArinc429*			m_arApiExRx;
	ARINC_INFO*			m_arInfoTx;
	ARINC_INFO*			m_arInfoRx;
	TXR_DATA_INFO		m_txrData;

};
