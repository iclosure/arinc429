// ChRxOutput.cpp : implementation file
//

#include "stdafx.h"
#include "TargetTool.h"
#include "Arinc429.h"
#include "ChRxOutput.h"
#include "RxLabelFilterDlg.h"
#include "MainFrm.h"

// CChRxOutput dialog

IMPLEMENT_DYNAMIC(CChRxOutput, CBCGPDialog)

CChRxOutput::CChRxOutput(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CChRxOutput::IDD, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);
	EnableLayout();
}

CChRxOutput::~CChRxOutput()
{
}

void CChRxOutput::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_comboBoxBaudRate);
	DDX_Control(pDX, IDC_COMBO_PAIRTY, m_comboBoxParity);
	DDX_Control(pDX, IDC_CHECK_ENABLE_SDI, m_checkBoxEnableSdi);
	DDX_Control(pDX, IDC_CHECK_SDI9, m_checkBoxSdi9);
	DDX_Control(pDX, IDC_CHECK_SDI10, m_checkBoxSdi10);
	DDX_Control(pDX, IDC_LIST_OUTPUT, m_listBoxData);
	CBCGPDialog::DoDataExchange(pDX);
}

BOOL CChRxOutput::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// 
	((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_STOP))->EnableWindow(FALSE);

	// 启动界面刷新定时器
	SetTimer(TIMER_FLUSH_ID, TIMER_FLUSH_PERIOD, NULL);

	PostMessage(WM_COMMAND, IDC_BUTTON_START);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CChRxOutput::PreTranslateMessage(MSG* pMsg)
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

BOOL CChRxOutput::DestroyWindow()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, IDC_BUTTON_AR_SYNC);

	//
	OnBnClickedButtonStop();

	return __super::DestroyWindow();
}

BEGIN_MESSAGE_MAP(CChRxOutput, CBCGPDialog)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &ThisClass::OnCbnSelchangeComboBaudrate)
	ON_CBN_SELCHANGE(IDC_COMBO_PAIRTY, &ThisClass::OnCbnSelchangeComboPairty)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_SDI, &ThisClass::OnBnClickedCheckEnableSdi)
	ON_BN_CLICKED(IDC_CHECK_SDI9, &ThisClass::OnBnClickedCheckSdi9)
	ON_BN_CLICKED(IDC_CHECK_SDI10, &ThisClass::OnBnClickedCheckSdi10)
	ON_BN_CLICKED(IDC_BUTTON_FILTER, &ThisClass::OnBnClickedButtonFilter)
	ON_BN_CLICKED(IDC_BUTTON_START, &ThisClass::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &ThisClass::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &ThisClass::OnBnClickedButtonClear)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CChRxOutput message handlers

#pragma warning(disable: 4996)
void CChRxOutput::OnTimer(UINT_PTR nIDEvent)
{
	int count = 150;
	switch (nIDEvent)
	{
	case TIMER_RX_ID:
		while(count--) {
			if (m_arApiEx->getWordRx(m_txrData))
			{
				GetLocalTimeString((PLARGE_INTEGER)&m_txrData.timetag);
				switch (m_arInfo->RX.Info.DataFormat)
				{
				case 0:	// 八进制
					if (0 == m_arInfo->RX.Info.DataSplit)  // 整体输出
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
					if (0 == m_arInfo->RX.Info.DataSplit) // 整体输出
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
					if (0 == m_arInfo->RX.Info.DataSplit) // 整体输出
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

			text.Format(_T("设备 %d 通道 %d 数据接收: %ld"), m_arInfo->BoardID,
				m_arInfo->RX.ChID, m_arApiEx->getCountRx());
			SetWindowText(text);
		}
		break;
	default: 
		__super::OnTimer(nIDEvent);
		break;
	}
}

void CChRxOutput::OnCbnSelchangeComboBaudrate()
{
	int baudRate = m_comboBoxBaudRate.GetCurSel();

	//! 
	int status = m_arApiEx->setConfig(ARU_RX_CH01_BIT_RATE + m_arInfo->RX.ChID, baudRate);
	if (status != ARS_NORMAL)
	{
		//! Log ouput
		//CString text;
		//text.Format(m_comboBoxBaudRate.GetLBText() + CString("：%s"), m_comboBoxBaudRate->(baudRate));
		BCGPMessageBox(_T("设置失败!!!"));
	}
}

void CChRxOutput::OnCbnSelchangeComboPairty()
{
	int parity = m_comboBoxParity.GetCurSel();

	//! 
	int status = m_arApiEx->setConfig(ARU_RX_CH01_PARITY + m_arInfo->RX.ChID, parity);
	if (status != ARS_NORMAL)
	{
		//CString text;
		//text.Format(m_comboBoxParity.GetLBText() + CString("：%s"), m_comboBoxParity->(parity));
		BCGPMessageBox(_T("设置失败!!!"));
	}
}

void CChRxOutput::OnBnClickedCheckEnableSdi()
{
	int decoder = m_checkBoxEnableSdi.GetCheck();

	//! 
	int status = m_arApiEx->setConfig(ARU_CH01_DECODER + m_arInfo->RX.ChID, decoder);
	if (status != ARS_NORMAL)
	{
		//CString text;
		//text.Format(m_checkBoxEnableSdi.GetLBText() + CString("：%s"), m_checkBoxEnableSdi->(decoder));
		BCGPMessageBox(_T("设置失败!!!"));
	}
}

void CChRxOutput::OnBnClickedCheckSdi9()
{
	ARINC429_SDI sdi;
	sdi.all = 0;
	sdi.bits.BIT9 = m_checkBoxSdi9.GetCheck();
	sdi.bits.BIT10 = m_arInfo->RX.Info.SDI.bits.BIT10;

	//! 
	int status = m_arApiEx->setConfig(ARU_CH01_SDI + m_arInfo->RX.ChID, sdi.all);
	if (status != ARS_NORMAL)
	{
		//CString text;
		//text.Format(m_checkBoxSdi9.GetLBText() + CString("：%s"), m_checkBoxSdi9->(sdi9));
		BCGPMessageBox(_T("设置失败!!!"));
	}
}

void CChRxOutput::OnBnClickedCheckSdi10()
{
	ARINC429_SDI sdi;
	sdi.all = 0;
	sdi.bits.BIT9 = m_arInfo->RX.Info.SDI.bits.BIT9;
	sdi.bits.BIT10 = m_checkBoxSdi10.GetCheck();

	//! 
	int status = m_arApiEx->setConfig(ARU_CH01_SDI + m_arInfo->RX.ChID, sdi.all);
	if (status != ARS_NORMAL)
	{
		//CString text;
		//text.Format(m_checkBoxSdi10.GetLBText() + CString("：%s"), m_checkBoxSdi10->(sdi10));
		BCGPMessageBox(_T("设置失败!!!"));
	}
}

void CChRxOutput::OnBnClickedButtonFilter()
{
	CRxLabelFilterDlg(m_arApiEx, this).DoModal();
}

void CChRxOutput::OnBnClickedButtonStart()
{
	CString text;

	// 启动板卡数据传输
	int status = m_arApiEx->go(CArinc429::CH_XMIT_RECV);
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
	SetTimer(TIMER_RX_ID, TIMER_RX_PERIOD, NULL);

	// 
	((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_STOP))->EnableWindow(TRUE);
}

void CChRxOutput::OnBnClickedButtonStop()
{
	// 
	KillTimer(TIMER_RX_ID);

	CString text;

	// 复位板卡
	int status = m_arApiEx->reset(CArinc429::CH_XMIT_RECV, false);
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

void CChRxOutput::OnBnClickedButtonClear()
{
	m_arApiEx->resetCountRx();
	m_listBoxData.ResetContent();
}

void CChRxOutput::OnClose()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, IDC_BUTTON_AR_SYNC);
	AfxGetMainWnd()->SendMessage(WM_USER_DELETE_SUBDLG, 1, LPARAM(m_arApiEx->arInfo().RX.ChID));
	CBCGPDialog::OnClose();
}

//////////////////////////////////////////////////////////////////

void CChRxOutput::InitializeArInfo(CArinc429* arApiEx)
{
	// 
	m_arApiEx = arApiEx;
	m_arInfo = &m_arApiEx->arInfo();

	// 
	CString text;
	text.Format(_T("设备 %d 通道 %d 数据接收"), m_arInfo->BoardID, m_arInfo->RX.ChID);
	SetWindowText(CString(text));

	//
	m_comboBoxBaudRate.SetCurSel(m_arInfo->RX.Info.BaudRate);
	m_comboBoxParity.SetCurSel(m_arInfo->RX.Info.Parity);
	m_checkBoxEnableSdi.SetCheck(m_arInfo->RX.Info.Decoder);
	m_checkBoxSdi9.SetCheck(m_arInfo->RX.Info.SDI.bits.BIT9);
	m_checkBoxSdi10.SetCheck(m_arInfo->RX.Info.SDI.bits.BIT10);
}
