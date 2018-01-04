// ChTxOutput.cpp : implementation file
//

#include "stdafx.h"
#include "TargetTool.h"
#include "Arinc429.h"
#include "ChTxOutput.h"
#include "TxDataSourceDlg.h"
#include "MainFrm.h"

// CChTxOutput dialog

IMPLEMENT_DYNAMIC(CChTxOutput, CBCGPDialog)

CChTxOutput::CChTxOutput(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CChTxOutput::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);
	EnableLayout();
}

CChTxOutput::~CChTxOutput()
{
}

void CChTxOutput::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_comboBoxBaudRate);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_comboBoxParity);
	DDX_Control(pDX, IDC_COMBO_LOOPBACK, m_comboBoxLoopback);
	DDX_Control(pDX, IDC_LIST_OUTPUT, m_listBoxData);
	CBCGPDialog::DoDataExchange(pDX);
}

BOOL CChTxOutput::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// 
	((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_STOP))->EnableWindow(FALSE);

	// 启动界面刷新定时器
	SetTimer(TIMER_FLUSH_ID, TIMER_FLUSH_PERIOD, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CChTxOutput::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:		// ESC
		case VK_RETURN:		// ENTER
		{
			SendMessage(WM_CLOSE);
		}
		default:
			AfxGetMainWnd()->PostMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
			break;
		}
		break;
	case WM_SYSKEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_MENU:
			AfxGetMainWnd()->PostMessage(WM_SYSKEYDOWN, pMsg->wParam, pMsg->lParam);
			return TRUE;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return __super::PreTranslateMessage(pMsg);
}

BOOL CChTxOutput::DestroyWindow()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, IDC_BUTTON_AR_SYNC);

	//
	OnBnClickedButtonStop();

	return __super::DestroyWindow();
}

BEGIN_MESSAGE_MAP(CChTxOutput, CBCGPDialog)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &ThisClass::OnCbnSelchangeComboBaudrate)
	ON_CBN_SELCHANGE(IDC_COMBO_PARITY, &ThisClass::OnCbnSelchangeComboParity)
	ON_CBN_SELCHANGE(IDC_COMBO_LOOPBACK, &ThisClass::OnCbnSelchangeComboLoopback)
	ON_BN_CLICKED(IDC_BUTTON_TX_DATA, &ThisClass::OnBnClickedButtonTxData)
	ON_BN_CLICKED(IDC_BUTTON_START, &ThisClass::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &ThisClass::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &ThisClass::OnBnClickedButtonClear)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CChTxOutput message handlers

#pragma warning(disable: 4996)
void CChTxOutput::OnTimer(UINT_PTR nIDEvent)
{
	int count = 150;
	switch (nIDEvent)
	{
	case TIMER_TX_ID:
		while (count--)
		{
			if (m_arApiEx->getWordTx(m_txrData))
			{
				switch (m_arInfo->TX.Info.DataFormat)
				{
				case 0: // 八进制
					if (0 == m_arInfo->TX.Info.DataSplit)  // 整体输出
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
					if (0 == m_arInfo->TX.Info.DataSplit)  // 整体输出
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
					if (0 == m_arInfo->TX.Info.DataSplit)  // 整体输出
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
				
				m_listBoxData.InsertString(0, CString(m_strOutput));
				if (m_listBoxData.GetCount() > 1000)
				{
					m_listBoxData.DeleteString(m_listBoxData.GetCount() - 1);
				}
			}
		}
		break;
	case TIMER_FLUSH_ID:
		{
			// 
			CString text;

			text.Format(_T("设备 %d 通道 %d 数据发送: %ld"), m_arInfo->BoardID,
				m_arInfo->TX.ChID, m_arApiEx->getCountTx());
			SetWindowText(text);

			// 
			if (!m_arApiEx->isSendBusy())
			{
				((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(TRUE);
				((CButton*)GetDlgItem(IDC_BUTTON_STOP))->EnableWindow(FALSE);
			}
		}
		break;
	default: 
		__super::OnTimer(nIDEvent);
		break;
	}
}

void CChTxOutput::OnCbnSelchangeComboBaudrate()
{
	int baudRate = m_comboBoxBaudRate.GetCurSel();

	//! 
	int status = m_arApiEx->setConfig(ARU_TX_CH01_BIT_RATE + m_arInfo->TX.ChID, baudRate);
	if (status != ARS_NORMAL)
	{
		//CString text;
		//text.Format(m_comboBoxBaudRate.GetLBText() + CString("：%s"), m_comboBoxBaudRate->(baudRate));
		BCGPMessageBox(_T("设置失败!!!"));
	}
}

void CChTxOutput::OnCbnSelchangeComboParity()
{
	int parity = m_comboBoxParity.GetCurSel();

	//! 
	int status = m_arApiEx->setConfig(ARU_TX_CH01_PARITY + m_arInfo->TX.ChID, parity);
	if (status != ARS_NORMAL)
	{
		//CString text;
		//text.Format(m_comboBoxParity.GetLBText() + CString("：%s"), m_comboBoxParity->(parity));
		BCGPMessageBox(_T("设置失败!!!"));
	}
}

void CChTxOutput::OnCbnSelchangeComboLoopback()
{
	int Loopback = m_comboBoxLoopback.GetCurSel();

	//! 
	int status = m_arApiEx->setConfig(ARU_CH01_LOOPBACK + m_arInfo->TX.ChID, Loopback);
	if (status != ARS_NORMAL)
	{
		//CString text;
		//text.Format(m_comboBoxLoopback.GetLBText() + CString("：%s"), m_comboBoxLoopback->(Loopback));
		BCGPMessageBox(_T("设置失败!!!"));
	}
}

void CChTxOutput::OnBnClickedButtonTxData()
{
	CTxDataSourceDlg(m_arApiEx, this).DoModal();
}

void CChTxOutput::OnBnClickedButtonStart()
{
	CString text;

	// 启动板卡数据传输
	int status = m_arApiEx->go(CArinc429::CH_XMIT_SEND);
	if (ARS_INVBOARD == status)
	{
		text.Format(_T("设备 %d 不存在！"), m_arApiEx->arInfo().BoardID);
		BCGPMessageBox(text);
		return;	// 返回
	}
	else if (ARS_BRDNOTLOAD == status)
	{
		text.Format(_T("设备 %d 没有加载！"), m_arApiEx->arInfo().BoardID);
		BCGPMessageBox(text);
		return;	// 返回
	}
	else if (ARS_WINRTFAIL == status)
	{
		text.Format(_T("设备 %d 无法运行！"), m_arApiEx->arInfo().BoardID);
		BCGPMessageBox(text);
		return;	// 返回
	}

	// 启动显示定时器
	SetTimer(TIMER_TX_ID, TIMER_TX_PERIOD, NULL);

	m_arApiEx->enableSend();

	// 
	((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_STOP))->EnableWindow(TRUE);
}

void CChTxOutput::OnBnClickedButtonStop()
{
	// 
	KillTimer(TIMER_TX_ID);

	CString text;

	// 复位板卡
	int status = m_arApiEx->reset(CArinc429::CH_XMIT_SEND, false);
	if (ARS_INVBOARD == status)
	{
		text.Format(_T("设备 %d 不存在！"), m_arApiEx->arInfo().BoardID);
		BCGPMessageBox(text);
		return;	// 返回
	}
	else if (ARS_BRDNOTLOAD == status)
	{
		text.Format(_T("设备 %d 没有加载！"), m_arApiEx->arInfo().BoardID);
		BCGPMessageBox(text);
		return;	// 返回
	}
	else if (ARS_WINRTFAIL == status)
	{
		text.Format(_T("设备 %d 无法复位！"), m_arApiEx->arInfo().BoardID);
		BCGPMessageBox(text);
		return;	// 返回
	}

	// 
	((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_STOP))->EnableWindow(FALSE);
}

void CChTxOutput::OnBnClickedButtonClear()
{
	m_arApiEx->resetCountTx();
	m_listBoxData.ResetContent();
}

void CChTxOutput::OnClose()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, IDC_BUTTON_AR_SYNC);
	AfxGetMainWnd()->SendMessage(WM_USER_DELETE_SUBDLG, 0, LPARAM(m_arApiEx->arInfo().TX.ChID));
	CBCGPDialog::OnClose();
}

//////////////////////////////////////////////////////////////////

void CChTxOutput::InitializeArInfo(CArinc429* arApiEx)
{
	// 
	m_arApiEx = arApiEx;
	m_arInfo = &m_arApiEx->arInfo();

	// 
	CString text;
	text.Format(_T("设备 %d 通道 %d 数据发送"), m_arInfo->BoardID, m_arInfo->TX.ChID);
	SetWindowText(CString(text));

	//
	m_comboBoxBaudRate.SetCurSel(m_arInfo->TX.Info.BaudRate);
	m_comboBoxParity.SetCurSel(m_arInfo->TX.Info.Parity);
	m_comboBoxLoopback.SetCurSel(m_arInfo->TX.Info.Loopback);
}
