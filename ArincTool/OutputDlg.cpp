// OutputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TargetTool.h"
#include "MainFrm.h"
#include "Arinc429.h"
#include "OutputDlg.h"
#include "UserWinMsg.h"


// COutputDlg dialog

IMPLEMENT_DYNAMIC(COutputDlg, CSkinDialog)

COutputDlg::COutputDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(COutputDlg::IDD, pParent)
{
}

COutputDlg::~COutputDlg()
{
}

void COutputDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TX_DATA, m_listBoxDataTx);
	DDX_Control(pDX, IDC_LIST_RX_DATA, m_listBoxDataRx);
	DDX_Control(pDX, IDC_REDIT_TX_LOG, m_rEditLog[CHANNEL_INDEX_TX]);
	DDX_Control(pDX, IDC_REDIT_RX_LOG, m_rEditLog[CHANNEL_INDEX_RX]);
}

BOOL COutputDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	// 自动布局设置
	CBCGPStaticLayout* layout = (CBCGPStaticLayout*) GetLayout();
	if (layout != NULL)
	{
		layout->AddAnchor(IDC_GROUP_TX, LAYOUT_MOVE_NONE, LAYOUT_SIZE_BOTH, RPOINT(  1, 1), RPOINT(1/2, 1));
		layout->AddAnchor(IDC_GROUP_RX, LAYOUT_MOVE_HORZ, LAYOUT_SIZE_BOTH, RPOINT(1/2, 1), RPOINT(1/2, 1));

		//! IDC_GROUP_TX
		layout->AddAnchor(IDC_REDIT_TX_LOG,  LAYOUT_MOVE_NONE, LAYOUT_SIZE_HORZ, RPOINT(1, 1), RPOINT(1/2, 1));
		layout->AddAnchor(IDC_LIST_TX_DATA, LAYOUT_MOVE_NONE, LAYOUT_SIZE_BOTH, RPOINT(1, 1), RPOINT(1/2, 1));
		layout->AddAnchor(IDC_LABEL_TX_DATA_FORMAT,LAYOUT_MOVE_VERT, LAYOUT_SIZE_NONE, RPOINT(1, 1));
		layout->AddAnchor(IDC_COMBO_TX_DATA_FORMAT,LAYOUT_MOVE_VERT, LAYOUT_SIZE_NONE, RPOINT(1, 1));
		layout->AddAnchor(IDC_LABEL_TX_DATA_SPLIT, LAYOUT_MOVE_VERT, LAYOUT_SIZE_NONE, RPOINT(1, 1));
		layout->AddAnchor(IDC_COMBO_TX_DATA_SPLIT, LAYOUT_MOVE_VERT, LAYOUT_SIZE_NONE, RPOINT(1, 1));
		layout->AddAnchor(IDC_BN_TX_CLEAR_CACHE,   LAYOUT_MOVE_BOTH, LAYOUT_SIZE_NONE, RPOINT(1/2, 1));

		//! IDC_GROUP_RX
		layout->AddAnchor(IDC_REDIT_RX_LOG,  LAYOUT_MOVE_HORZ, LAYOUT_SIZE_HORZ, RPOINT(1/2, 1/2), RPOINT(1/2, 1));
		layout->AddAnchor(IDC_LIST_RX_DATA, LAYOUT_MOVE_HORZ, LAYOUT_SIZE_BOTH, RPOINT(1/2,   1), RPOINT(1/2, 1));
		layout->AddAnchor(IDC_LABEL_RX_DATA_FORMAT,LAYOUT_MOVE_BOTH, LAYOUT_SIZE_NONE, RPOINT(1/2, 1));
		layout->AddAnchor(IDC_COMBO_RX_DATA_FORMAT,LAYOUT_MOVE_BOTH, LAYOUT_SIZE_NONE, RPOINT(1/2, 1));
		layout->AddAnchor(IDC_LABEL_RX_DATA_SPLIT, LAYOUT_MOVE_BOTH, LAYOUT_SIZE_NONE, RPOINT(1/2, 1));
		layout->AddAnchor(IDC_COMBO_RX_DATA_SPLIT, LAYOUT_MOVE_BOTH, LAYOUT_SIZE_NONE, RPOINT(1/2, 1));
		layout->AddAnchor(IDC_BN_RX_CLEAR_CACHE,   LAYOUT_MOVE_BOTH, LAYOUT_SIZE_NONE, RPOINT(1, 1));
	}

	// 初始化变量
	m_fontLog.cbSize = sizeof(m_fontLog);
	m_fontLog.dwMask = CFM_COLOR | CFM_SIZE | CFM_FACE;
	m_fontLog.dwEffects = CFM_BOLD;
	lstrcpy(m_fontLog.szFaceName, _T("Segoe UI"));
	m_fontLog.yHeight = 180;
	m_fontLog.crTextColor = RGB(0x00, 0x00, 0xA2);

	// 
	m_rEditLog[CHANNEL_INDEX_TX].SetDefaultCharFormat(m_fontLog);
	m_rEditLog[CHANNEL_INDEX_RX].SetDefaultCharFormat(m_fontLog);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(COutputDlg, CSkinDialog)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_TX_DATA_FORMAT, &ThisClass::OnCbnSelchangeComboTxDataFormat)
	ON_CBN_SELCHANGE(IDC_COMBO_RX_DATA_FORMAT, &ThisClass::OnCbnSelchangeComboRxDataFormat)
	ON_CBN_SELCHANGE(IDC_COMBO_TX_DATA_SPLIT, &ThisClass::OnCbnSelchangeComboTxDataSplit)
	ON_CBN_SELCHANGE(IDC_COMBO_RX_DATA_SPLIT, &ThisClass::OnCbnSelchangeComboRxDataSplit)
	ON_BN_CLICKED(IDC_BN_TX_CLEAR_CACHE, &ThisClass::OnBnClickedBnTxClearCache)
	ON_BN_CLICKED(IDC_BN_RX_CLEAR_CACHE, &ThisClass::OnBnClickedBnRxClearCache)
	ON_MESSAGE(WM_USER_CLEAN_TXT, &ThisClass::OnUserCleanTxt)
END_MESSAGE_MAP()


// COutputDlg message handlers

#pragma warning(disable: 4996)
void COutputDlg::OnTimer(UINT_PTR nIDEvent)
{
	int count = 150;

	switch (nIDEvent)
	{
	case TIMER_TX_ID:
		while (count--)
		{
			if (m_arApiExTx->getWordTx(m_txrData))
			{
				switch (m_arInfoTx->TX.Info.DataFormat)
				{
				case 0: // 八进制
					if (0 == m_arInfoTx->TX.Info.DataSplit)  // 整体输出
					{
						sprintf(m_strOutput, "%d: %08o", m_txrData.count, m_txrData.word);
					}
					else  // 分段拆分
					{
						sprintf(m_strOutput, "%d: P: %o, SSM: %o, DATA: %6o, SDI: %o, LABEL: %3o",
							m_txrData.count,
							m_txrData.PARITY, m_txrData.SSM, m_txrData.DATA, m_txrData.SDI, m_txrData.LABEL);
					}
					break;
				case 1: // 十进制
					if (0 == m_arInfoTx->TX.Info.DataSplit)  // 整体输出
					{
						sprintf(m_strOutput, "%d: %08u", m_txrData.count, m_txrData.word);
					}
					else  // 分段拆分
					{
						sprintf(m_strOutput, "%d: P: %u, SSM: %u, DATA: %6u, SDI: %u, LABEL: %3u",
							m_txrData.count,
							m_txrData.PARITY, m_txrData.SSM, m_txrData.DATA, m_txrData.SDI, m_txrData.LABEL);
					}
					break;
				case 2: // 十六进制
					if (0 == m_arInfoTx->TX.Info.DataSplit)  // 整体输出
					{ 
						sprintf(m_strOutput, "%d: %08X", m_txrData.count, m_txrData.word);
					}
					else  // 分段拆分
					{  
						sprintf(m_strOutput, "%d: P: %X, SSM: %X, DATA: %05X, SDI: %X, LABEL: %02X",
							m_txrData.count,
							m_txrData.PARITY, m_txrData.SSM, m_txrData.DATA, m_txrData.SDI, m_txrData.LABEL);
					}
					break;
				default: ASSERT(FALSE);
				}

				m_listBoxDataTx.InsertString(0, CString(m_strOutput));
				if (m_listBoxDataTx.GetCount() > 1000)
				{
					m_listBoxDataTx.DeleteString(m_listBoxDataTx.GetCount() - 1);
				}
			}
		}
		break;
	case TIMER_RX_ID:
		while(count--) {
			if (m_arApiExRx->getWordRx(m_txrData))
			{
				GetLocalTimeString((PLARGE_INTEGER)&m_txrData.timetag);
				switch (m_arInfoRx->RX.Info.DataFormat)
				{
				case 0:	// 八进制
					if (0 == m_arInfoRx->RX.Info.DataSplit)  // 整体输出
					{ 
						sprintf(m_strOutput, "%d: %08o <%I64u (%ld us)>", 
							m_txrData.count, m_txrData.word, m_txrData.timetag, m_txrData.timeDiff);
					}
					else // 分段拆分
					{  
						sprintf(m_strOutput, "%d: P: %o, SSM: %o, DATA: %o, SDI: %o, LABEL: %o <%I64u (%ld us)>",
							m_txrData.count,
							m_txrData.PARITY, m_txrData.SSM, m_txrData.DATA, m_txrData.SDI, m_txrData.LABEL,
							m_txrData.timetag, m_txrData.timeDiff);
					}
					break;
				case 1:	// 十进制
					if (0 == m_arInfoRx->RX.Info.DataSplit) // 整体输出
					{  
						sprintf(m_strOutput, "%d: %08u <%I64u (%l us)>", 
							m_txrData.count, m_txrData.word, m_txrData.timetag, m_txrData.timeDiff);
					}
					else // 分段拆分
					{  
						sprintf(m_strOutput, "%d: P: %u, SSM: %u, DATA: %6u, SDI: %u, LABEL: %3u <%I64u (%ld us)>",
							m_txrData.count,
							m_txrData.PARITY, m_txrData.SSM, m_txrData.DATA, m_txrData.SDI, m_txrData.LABEL,
							m_txrData.timetag, m_txrData.timeDiff);
					}
					break;
				case 2: // 十六进制
					if (0 == m_arInfoRx->RX.Info.DataSplit) // 整体输出
					{
						sprintf(m_strOutput, "%d: %08X <%s (%ld us)>", 
							m_txrData.count, m_txrData.word, m_strSysTime, m_txrData.timeDiff);
					}
					else  // 分段拆分
					{ 
						sprintf(m_strOutput, "%d: P: %X, SSM: %X, DATA: %05X, SDI: %X, LABEL: %02X <%I64u (%ld us)>",
							m_txrData.count,
							m_txrData.PARITY, m_txrData.SSM, m_txrData.DATA, m_txrData.SDI, m_txrData.LABEL,
							m_txrData.timetag, m_txrData.timeDiff);
					}
					break;
				default: ASSERT(FALSE);
				}

				m_listBoxDataRx.InsertString(0, CString(m_strOutput));
				if (m_listBoxDataRx.GetCount() > 1000)
				{
					m_listBoxDataRx.DeleteString(m_listBoxDataRx.GetCount() - 1);
				}
			}
		}
		break;
	default:
		__super::OnTimer(nIDEvent);
		break;
	}
}

void COutputDlg::OnCbnSelchangeComboTxDataFormat()
{
	m_arInfoTx->TX.Info.DataFormat = 
		((CComboBox*)GetDlgItem(IDC_COMBO_TX_DATA_FORMAT))->GetCurSel();
}

void COutputDlg::OnCbnSelchangeComboRxDataFormat()
{
	m_arInfoRx->RX.Info.DataFormat =
		((CComboBox*)GetDlgItem(IDC_COMBO_RX_DATA_FORMAT))->GetCurSel();
}

void COutputDlg::OnCbnSelchangeComboTxDataSplit()
{
	m_arInfoTx->TX.Info.DataSplit =
		((CComboBox*)GetDlgItem(IDC_COMBO_TX_DATA_SPLIT))->GetCurSel();
}

void COutputDlg::OnCbnSelchangeComboRxDataSplit()
{
	m_arInfoRx->RX.Info.DataSplit =
		((CComboBox*)GetDlgItem(IDC_COMBO_RX_DATA_SPLIT))->GetCurSel();
}

void COutputDlg::OnBnClickedBnTxClearCache()
{
	CWaitCursor waitCursor;
	m_arApiExTx->resetCountTx();
	SendMessage(WM_USER_CLEAN_TXT, COutputDlg::CHANNEL_INDEX_TX);
}

void COutputDlg::OnBnClickedBnRxClearCache()
{
	CWaitCursor waitCursor;
	m_arApiExRx->resetCountRx();
	SendMessage(WM_USER_CLEAN_TXT, COutputDlg::CHANNEL_INDEX_RX);
}

/// 消息：	WM_USER_CLEAN_TXT
/// 种类：	用户自定义消息
/// 功能：	清除编辑框文本
/// 参数：	wParam, 无效
///			lParam, 无效
/// 返回：	无效
LRESULT	COutputDlg::OnUserCleanTxt(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case CHANNEL_INDEX_TX:
		m_listBoxDataTx.ResetContent();
		break;
	case CHANNEL_INDEX_RX:
		m_listBoxDataRx.ResetContent();
		break;
	default:
		ASSERT(FALSE);
	}

	return 0;
}

void COutputDlg::MyLogOut(UINT index, LPCTSTR text)
{
	ASSERT(CHANNEL_INDEX_TX <= index && index <= CHANNEL_INDEX_RX);

	SYSTEMTIME time;
	GetLocalTime(&time);
	CString timeStr;
	timeStr.Format(_T("[%02d/%02d %02d:%02d:%02d]: "), time.wMonth,
		time.wDay, time.wHour, time.wMinute, time.wSecond);

	m_rEditLog[index].SetSel(-1, -1);
	m_rEditLog[index].ReplaceSel(timeStr + text + _T("\n"));
	m_rEditLog[index].SendMessage(WM_VSCROLL, SB_BOTTOM);
}

void COutputDlg::EnableDisplay(BOOL enable /* = TRUE */)
{
	if (enable)
	{
		SetTimer(TIMER_TX_ID, TIMER_TX_PERIOD, NULL);
		SetTimer(TIMER_RX_ID, TIMER_RX_PERIOD, NULL);
		m_arApiExTx->EnableDisplay(TRUE);
		m_arApiExRx->EnableDisplay(TRUE);
	}
	else
	{
		KillTimer(TIMER_TX_ID);
		KillTimer(TIMER_RX_ID);
	}
}

void COutputDlg::TurnOn(BOOL enableDisplay /* = TRUE */)
{
	//! 
	((CComboBox*)GetDlgItem(IDC_COMBO_TX_DATA_FORMAT))->SetCurSel(m_arInfoTx->TX.Info.DataFormat);
	((CComboBox*)GetDlgItem(IDC_COMBO_RX_DATA_FORMAT))->SetCurSel(m_arInfoTx->RX.Info.DataFormat);
	((CComboBox*)GetDlgItem(IDC_COMBO_TX_DATA_SPLIT))->SetCurSel(m_arInfoRx->TX.Info.DataSplit);
	((CComboBox*)GetDlgItem(IDC_COMBO_RX_DATA_SPLIT))->SetCurSel(m_arInfoRx->RX.Info.DataSplit);

	EnableDisplay(enableDisplay);
}

void COutputDlg::TurnOff(BOOL enableDisplay /* = TRUE */)
{
	EnableDisplay(enableDisplay);
}

void COutputDlg::SetArApiEx(CArinc429* arApiExTx, CArinc429* arApiExRx)
{
	m_arApiExTx = arApiExTx;
	m_arApiExRx = arApiExRx;
	m_arInfoTx = &m_arApiExTx->arInfo();
	m_arInfoRx = &m_arApiExRx->arInfo();
}
