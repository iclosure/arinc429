// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TargetTool.h"
#include "MainFrm.h"
#include "TargetToolDoc.h"
#include "TargetToolView.h"
#include "Version.h"
#include "OutputDlg.h"
#include "PerformanceDlg.h"
#include "GeneralConfigDlg.h"
#include "SetupDriverDlg.h"
#include "TxDataSourceDlg.h"
#include "RxLabelFilterDlg.h"
#include "ChTxOutput.h"
#include "ChRxOutput.h"
#include "TxDataCustomDlg.h"
#include "UserWinMsg.h"
#include "Arinc429.h"
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 针对Arinc数据收发的定时器间隔（单位：ms）
typedef std::map<int, ULONG>				MAP_INDEX_PERIOD;
typedef MAP_INDEX_PERIOD::value_type		PAIR_INDEX_PERIOD;

const PAIR_INDEX_PERIOD __map_tx_value[] =
{
	PAIR_INDEX_PERIOD(0, 0),
	PAIR_INDEX_PERIOD(1, 1),
	PAIR_INDEX_PERIOD(2, 2),
	PAIR_INDEX_PERIOD(3, 3),
	PAIR_INDEX_PERIOD(4, 4),
	PAIR_INDEX_PERIOD(5, 5),
	PAIR_INDEX_PERIOD(6, 10),
	PAIR_INDEX_PERIOD(7, 15),
	PAIR_INDEX_PERIOD(8, 20),
	PAIR_INDEX_PERIOD(9, 50),
	PAIR_INDEX_PERIOD(10, 80),
	PAIR_INDEX_PERIOD(11, 100),
	PAIR_INDEX_PERIOD(12, 150),
	PAIR_INDEX_PERIOD(13, 200),
	PAIR_INDEX_PERIOD(14, 300),
	PAIR_INDEX_PERIOD(15, 400),
	PAIR_INDEX_PERIOD(16, 500),
	PAIR_INDEX_PERIOD(17, 800),
	PAIR_INDEX_PERIOD(18, 1000),
};

const PAIR_INDEX_PERIOD __map_rx_value[] =
{
	PAIR_INDEX_PERIOD(0, 1),
	PAIR_INDEX_PERIOD(1, 2),
	PAIR_INDEX_PERIOD(2, 3),
	PAIR_INDEX_PERIOD(3, 4),
	PAIR_INDEX_PERIOD(4, 5),
	PAIR_INDEX_PERIOD(5, 10),
	PAIR_INDEX_PERIOD(6, 15),
	PAIR_INDEX_PERIOD(7, 20),
	PAIR_INDEX_PERIOD(8, 50),
	PAIR_INDEX_PERIOD(9, 80),
	PAIR_INDEX_PERIOD(10, 100),
	PAIR_INDEX_PERIOD(11, 200),
	PAIR_INDEX_PERIOD(12, 500),
	PAIR_INDEX_PERIOD(13, 1000),
};

static MAP_INDEX_PERIOD _g_tx_index_periods(__map_tx_value,
	__map_tx_value + sizeof(__map_tx_value) / sizeof(__map_tx_value[0]));
static MAP_INDEX_PERIOD _g_rx_index_periods(__map_rx_value,
	__map_rx_value + sizeof(__map_rx_value) / sizeof(__map_rx_value[0]));

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(BCGM_ON_CHANGE_RIBBON_CATEGORY, &ThisClass::OnChangeRibbonCategory)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_VS2012_DARK, &ThisClass::OnAppLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_VS2012_DARK, &ThisClass::OnUpdateAppLook)
	ON_COMMAND_RANGE(ID_VIEW_COLOR_THEME_DEFAULT, ID_VIEW_COLOR_THEME_TEXTURES, &ThisClass::OnAppColorTheme)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_COLOR_THEME_DEFAULT, ID_VIEW_COLOR_THEME_TEXTURES, &ThisClass::OnUpdateAppColorTheme)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	// RibbonBar
	ON_COMMAND(IDC_BUTTON_DATASHEET, &ThisClass::OnButtonDataSheet)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_DATASHEET, &ThisClass::OnUpdateButtonDataSheet)
	ON_COMMAND(IDC_COMBO_BOARD_ID, &ThisClass::OnComboBoardId)
	ON_COMMAND(IDC_COMBO_TX_ID, &ThisClass::OnComboTxId)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_TX_ID, &ThisClass::OnUpdateComboTxId)
	ON_COMMAND(IDC_COMBO_TX_BAUD, &ThisClass::OnComboTxBaud)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_TX_BAUD, &ThisClass::OnUpdateComboTxBaud)
	ON_COMMAND(IDC_COMBO_TX_PARITY, &ThisClass::OnComboTxParity)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_TX_PARITY, &ThisClass::OnUpdateComboTxParity)
	ON_COMMAND(IDC_COMBO_TX_PERIOD, &ThisClass::OnComboTxPeriod)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_TX_PERIOD, &ThisClass::OnUpdateComboTxPeriod)
	ON_COMMAND(IDC_COMBO_TX_TEST_WAY, &ThisClass::OnComboTxTestWay)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_TX_TEST_WAY, &ThisClass::OnUpdateComboTxTestWay)
	ON_COMMAND(IDC_BUTTON_TX_DATA, &ThisClass::OnButtonTxData)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_TX_DATA, &ThisClass::OnUpdateButtonTxData)
	//ON_COMMAND(IDC_BUTTON_EDIT_DATA, &ThisClass::OnButtonTxDataEdit)
	//ON_UPDATE_COMMAND_UI(IDC_BUTTON_EDIT_DATA, &ThisClass::OnUpdateButtonTxDataEdit)
	ON_COMMAND(IDC_BUTTON_SEND, &ThisClass::OnButtonSendData)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_SEND, &ThisClass::OnUpdateButtonSendData)
	ON_COMMAND(IDC_BUTTON_TX_NEW, &ThisClass::OnButtonTxNew)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_TX_NEW, &ThisClass::OnUpdateButtonTxNew)
	ON_COMMAND(IDC_COMBO_RX_ID, &ThisClass::OnComboRxId)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_RX_ID, &ThisClass::OnUpdateComboRxId)
	ON_COMMAND(IDC_COMBO_RX_BAUD, &ThisClass::OnComboRxBaud)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_RX_BAUD, &ThisClass::OnUpdateComboRxBaud)
	ON_COMMAND(IDC_COMBO_RX_PARITY, &ThisClass::OnComboRxParity)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_RX_PARITY, &ThisClass::OnUpdateComboRxParity)
	ON_COMMAND(IDC_BUTTON_FILTER, &ThisClass::OnButtonFilter)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_FILTER, &ThisClass::OnUpdateButtonFilter)
	ON_COMMAND(IDC_CHECK_DECODER, &ThisClass::OnCheckDecoder)
	ON_UPDATE_COMMAND_UI(IDC_CHECK_DECODER, &ThisClass::OnUpdateCheckDecoder)
	ON_COMMAND(IDC_CHECK_SDI_BIT9, &ThisClass::OnCheckSdiBit9)
	ON_UPDATE_COMMAND_UI(IDC_CHECK_SDI_BIT9, &ThisClass::OnUpdateCheckSdiBit9)
	ON_COMMAND(IDC_CHECK_SDI_BIT10, &ThisClass::OnCheckSdiBit10)
	ON_UPDATE_COMMAND_UI(IDC_CHECK_SDI_BIT10, &ThisClass::OnUpdateCheckSdiBit10)
	ON_COMMAND(IDC_BUTTON_RX_NEW, &ThisClass::OnButtonRxNew)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_RX_NEW, &ThisClass::OnUpdateButtonRxNew)
	ON_COMMAND(IDC_BUTTON_AR_TURN, &ThisClass::OnButtonArTurn)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_AR_TURN, &ThisClass::OnUpdateButtonArTurn)
	ON_COMMAND(IDC_BUTTON_AR_PLAY, &ThisClass::OnButtonArPlay)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_AR_PLAY, &ThisClass::OnUpdateButtonArPlay)
	ON_COMMAND(IDC_BUTTON_AR_SYNC, &ThisClass::OnButtonArSync)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_AR_SYNC, &ThisClass::OnUpdateButtonArSync)
	ON_COMMAND(IDC_CHECK_DISPLAY, &ThisClass::OnCheckDislpay)
	ON_UPDATE_COMMAND_UI(IDC_CHECK_DISPLAY, &ThisClass::OnUpdateCheckDislpay)
	// StatusBar
	ON_COMMAND(IDC_BUTTON_RESET_COUNT, &ThisClass::OnButtonResetCount)

	ON_MESSAGE(WM_USER_DELETE_SUBDLG, &ThisClass::OnUserDeleteSubDlg)
	ON_MESSAGE(WM_USER_DELETE_ARAPI, &ThisClass::OnUserDeleteArApi)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_2010_1);

	m_bArTurnOn = FALSE;
	m_bArRun = FALSE;

	m_arrayChTxOutput = NULL;
	m_arrayChRxOutput = NULL;
	m_arrayArApiEx = NULL;

	m_txDataEditDlg = new CTxDataCustomDlg;

	m_oldBoardId = 0;
	m_currChTx = 0;
	m_currChRx = 0;

	m_bDisplay = theApp.GetInt(_T("DisplayOutput"), TRUE);
}

CMainFrame::~CMainFrame()
{
	m_txDataEditDlg->DestroyWindow();
	delete m_txDataEditDlg;

	if (m_arrayChTxOutput != NULL)
	{
		for (int i = 0; i < ARINC_MAX_CH_NUM; i++)
		{
			if (::IsWindowVisible(m_arrayChTxOutput[i].m_hWnd))
			{
				m_arrayChTxOutput[i].DestroyWindow();
			}
		}
		delete[] m_arrayChTxOutput;
		m_arrayChTxOutput = NULL;
	}

	if (m_arrayChRxOutput != NULL)
	{
		for (int i = 0; i < ARINC_MAX_CH_NUM; i++)
		{
			if (::IsWindowVisible(m_arrayChRxOutput[i].m_hWnd))
			{
				m_arrayChRxOutput[i].DestroyWindow();
			}
		}
		delete[] m_arrayChRxOutput;
		m_arrayChRxOutput = NULL;
	}

	if (m_arrayArApiEx != NULL)
	{
		delete[] m_arrayArApiEx;
		m_arrayArApiEx = NULL;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		TRACE0("Failed to Create CMainFrame Object\n");
		return -1;
	}

	CString versin;
	versin.Format(_T("Arinc429 通信测试 V%d.%d.%d"),
		ARINCTOOL_MAJOR_VER, ARINCTOOL_MINOR_VER, ARINCTOOL_PATCH_VER);
	SetTitle(versin);

	if (!CreateRibbonBar())
	{
		TRACE0("Failed to create ribbon bar\n");
		return -1;      // fail to create
	}

	if (!CreateStatusBar())
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);

	// 
	OnAppLook(m_nAppLook);

	// 初始化工具栏信息

	CString text;

	// board
	CBCGPRibbonEdit *pBoardCountEdit = DYNAMIC_DOWNCAST(CBCGPRibbonEdit,
		m_wndRibbonBar.FindByID(IDC_EDIT_BOARD_COUNT));
	text.Format(_T("%d"), ar_board_num());
	pBoardCountEdit->SetEditText(text);

	CBCGPRibbonComboBox *pBoardIdCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_BOARD_ID));
	pBoardIdCombo->RemoveAllItems();
	for (int i = 0; i < ar_board_num(); i++)
	{
		switch (ar_board_type(i))
		{
		case ARINC_BOARD_TYPE_PCI:
			text.Format(_T("PCI-%02d"), i);
			break;
		case ARINC_BOARD_TYPE_USB:
			text.Format(_T("USB-%02d"), i);
			break;
		case ARINC_BOARD_TYPE_UNKNOWN:
			text.Format(_T("UNK-%02d"), i);
			break;
		default:
			ASSERT(FALSE);
			break;
		}

		pBoardIdCombo->AddItem(text);
	}
	pBoardIdCombo->SelectItem(m_oldBoardId);

	// transmit period
	CBCGPRibbonComboBox *pTxPeriodCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_TX_PERIOD));
	CBCGPRibbonComboBox *pRxPeriodCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_RX_PERIOD));
	for (MAP_INDEX_PERIOD::iterator iter = _g_tx_index_periods.begin();
		iter != _g_tx_index_periods.end(); iter++)
	{
		text.Format(_T("%d ms"), iter->second);
		pTxPeriodCombo->AddItem(text);
	}

	// Status bar
	SetStatusBarPaneText(IDC_LABEL_COMMU_STATUS, _T("断开"));

	// create
	if (pBoardIdCombo->GetCount() > 0)
	{
		m_arrayChTxOutput = new CChTxOutput[ARINC_MAX_CH_NUM];
		m_arrayChRxOutput = new CChRxOutput[ARINC_MAX_CH_NUM];
		m_arrayArApiEx = new CArinc429[ARINC_MAX_CH_NUM];

		for (int i = 0; i < ARINC_MAX_CH_NUM; i++)
		{
			m_arrayArApiEx[i].arInfo().BoardID = m_oldBoardId;
			m_arrayArApiEx[i].arInfo().TX.ChID = i;
			m_arrayArApiEx[i].arInfo().RX.ChID = i;
			m_arrayArApiEx[i].enableDisplaySend(m_bDisplay);
			m_arrayArApiEx[i].enableDisplayRecv(m_bDisplay);
		}
	}

	// 
	m_txDataEditDlg->Create(CTxDataCustomDlg::IDD, this);
	m_txDataEditDlg->CenterWindow(this);

	// 启动系统时间显示的定时器
	SetTimer(TIMER_ID_SYS_TIME, TIMER_ELAPSE_SYS_TIME, NULL);
	SetTimer(TIMER_ID_UPDATE_COUNT, TIMER_ELAPSE_UPDATE_COUNT, NULL);

	// VISUAL_MANAGER
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	if (!CBCGPFrameWnd::PreCreateWindow(cs))
		return FALSE;

	WNDCLASS wndcls;

	if (GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndcls))
	{
		HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_MAINFRAME), RT_GROUP_ICON);
		HICON hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));

		// register a very similar WNDCLASS but without CS_HREDRAW and CS_VREDRAW styles:
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS, wndcls.hCursor, wndcls.hbrBackground, hIcon);
	}

	return TRUE;
}

BOOL CMainFrame::CreateStatusBar()
{
	if (!m_wndStatusBar.Create(this))
	{
		return FALSE;
	}

	if (!m_wndStatusBar.LoadFromXML(_T("IDR_BCGP_RIBBON_XML")))
	{
		return FALSE;
	}

	// USE_STATUS_DESIGNER

	return TRUE;
}

BOOL CMainFrame::SyncConfigToScreen()
{
	//
	// TX
	//

	{
		ARINC_INFO & arInfo = m_arrayArApiEx[m_currChTx].arInfo();
		CString text;

		// channel number
		CBCGPRibbonEdit *pTxChNumEdit = DYNAMIC_DOWNCAST(CBCGPRibbonEdit,
			m_wndRibbonBar.FindByID(IDC_EDIT_TX_CHNUM));
		text.Format(_T("%d"), arInfo.TX.ChNum);
		pTxChNumEdit->SetEditText(text);

		// channels
		CBCGPRibbonComboBox *pTxIdCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
			m_wndRibbonBar.FindByID(IDC_COMBO_TX_ID));
		pTxIdCombo->RemoveAllItems();
		for (int i = 0; i < arInfo.TX.ChNum; i++)
		{
			CString item;
			item.Format(_T("%d"), i);
			pTxIdCombo->AddItem(item);
		}
		pTxIdCombo->SelectItem((int)arInfo.TX.ChID);

		// baud rate
		CBCGPRibbonComboBox *pTxBaudCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
			m_wndRibbonBar.FindByID(IDC_COMBO_TX_BAUD));
		pTxBaudCombo->SelectItem((int)arInfo.TX.Info.BaudRate);

		// parity
		CBCGPRibbonComboBox *pTxParityCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
			m_wndRibbonBar.FindByID(IDC_COMBO_TX_PARITY));
		pTxParityCombo->SelectItem((int)arInfo.TX.Info.Parity);

		// transmit period
		CBCGPRibbonComboBox *pTxPeriodCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
			m_wndRibbonBar.FindByID(IDC_COMBO_TX_PERIOD));
		text.Format(_T("%d ms"), arInfo.TX.Info.Period.Value);
		arInfo.TX.Info.Period.Key = pTxPeriodCombo->FindItem(text);
		pTxPeriodCombo->SelectItem(arInfo.TX.Info.Period.Key);

		// test way
		CBCGPRibbonComboBox *pTxTestWayCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
			m_wndRibbonBar.FindByID(IDC_COMBO_TX_TEST_WAY));

		pTxTestWayCombo->SelectItem((int)arInfo.TX.Info.Loopback);
		SetStatusBarPaneText(IDC_LABEL_COMMU_MODE, pTxTestWayCombo->GetItem(pTxTestWayCombo->GetCurSel()));
	}

	//
	// RX
	//
	{
		ARINC_INFO & arInfo = m_arrayArApiEx[m_currChRx].arInfo();
		CString text;

		// channel number
		CBCGPRibbonEdit *pRxChNumEdit = DYNAMIC_DOWNCAST(CBCGPRibbonEdit,
			m_wndRibbonBar.FindByID(IDC_EDIT_RX_CHNUM));
		text.Format(_T("%d"), arInfo.RX.ChNum);
		pRxChNumEdit->SetEditText(text);

		// channels
		CBCGPRibbonComboBox *pRxIdCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
			m_wndRibbonBar.FindByID(IDC_COMBO_RX_ID));
		pRxIdCombo->RemoveAllItems();
		for (int i = 0; i < arInfo.RX.ChNum; i++)
		{
			CString item;
			item.Format(_T("%d"), i);
			pRxIdCombo->AddItem(item);
		}
		pRxIdCombo->SelectItem((int)arInfo.RX.ChID);

		// baud rate
		CBCGPRibbonComboBox *pRxBaudCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
			m_wndRibbonBar.FindByID(IDC_COMBO_RX_BAUD));
		pRxBaudCombo->SelectItem((int)arInfo.RX.Info.BaudRate);

		// parity
		CBCGPRibbonComboBox *pRxParityCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
			m_wndRibbonBar.FindByID(IDC_COMBO_RX_PARITY));
		pRxParityCombo->SelectItem((int)arInfo.RX.Info.Parity);

		// decoder
		m_bDecoder = arInfo.RX.Info.Decoder;
		m_bSdiBit9 = arInfo.RX.Info.SDI.bits.BIT9;
		m_bSdiBit10 = arInfo.RX.Info.SDI.bits.BIT10;

		// label filter
		m_bFilter = arInfo.RX.Info.Label.Enable;

		// 
		CBCGPRibbonButton *pFilterButton = DYNAMIC_DOWNCAST(CBCGPRibbonButton,
			m_wndRibbonBar.FindByID(IDC_BUTTON_FILTER));
		pFilterButton->SetToolTipText(m_bFilter ? _T("已启用过滤器") : _T("已禁用过滤器"));
	}

	// Others
	SetStatusBarPaneText(IDC_LABEL_INFO, _T(""));

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
}

#endif //_DEBUG


LRESULT CMainFrame::OnChangeRibbonCategory(WPARAM wParam, LPARAM lParam)
{
	int index = m_wndRibbonBar.GetCategoryIndex(m_wndRibbonBar.GetActiveCategory());

	switch (index)
	{
	case 1:
	{
		break;
	}
	case 2:
	{
		// 
		if (m_bArTurnOn)
		{
			SendMessage(WM_COMMAND, IDC_BUTTON_AR_TURN);
		}

		// delete sub-dialog
		//SendMessage(WM_USER_DELETE_SUBDLG, -1, -1);

		break;
	}
	default:
		break;
	}

	m_wndView->SendMessage(BCGM_ON_CHANGE_RIBBON_CATEGORY, wParam, lParam);

	return 0;
}

// CMainFrame message handlers

void CMainFrame::OnAppLook(UINT id)
{
	CBCGPDockManager::SetDockMode(BCGP_DT_SMART);

	m_nAppLook = id;

	BOOL bIsScenic = (m_nAppLook >= ID_VIEW_APPLOOK_2010_1 && m_nAppLook <= ID_VIEW_APPLOOK_2010_3)
		|| m_nAppLook == ID_VIEW_APPLOOK_WIN7;

	m_wndRibbonBar.SetBackstageMode();
	m_wndRibbonBar.SetScenicLook(bIsScenic);
	if (m_wndRibbonBar.GetMainButton() != NULL)
	{
		m_wndRibbonBar.GetMainButton()->SetScenicText(m_nAppLook != ID_VIEW_APPLOOK_WIN7 ? _T("ARINC") : NULL);
	}
	CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D;

	switch (m_nAppLook)
	{
	case ID_VIEW_APPLOOK_2000:
		// enable Office 2000 look:
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManager));
		break;

	case ID_VIEW_APPLOOK_XP:
		// enable Office XP look:
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManagerXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		// enable Windows XP look (in other OS Office XP look will be used):
		CBCGPWinXPVisualManager::m_b3DTabsXPTheme = TRUE;
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPWinXPVisualManager));
		break;

	case ID_VIEW_APPLOOK_2003:
		// enable Office 2003 look:
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManager2003));
		CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_2007:
	case ID_VIEW_APPLOOK_2007_1:
	case ID_VIEW_APPLOOK_2007_2:
	case ID_VIEW_APPLOOK_2007_3:
		// enable Office 2007 look:
		switch (m_nAppLook)
		{
		case ID_VIEW_APPLOOK_2007:
			CBCGPVisualManager2007::SetStyle(CBCGPVisualManager2007::VS2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_2007_1:
			CBCGPVisualManager2007::SetStyle(CBCGPVisualManager2007::VS2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_2007_2:
			CBCGPVisualManager2007::SetStyle(CBCGPVisualManager2007::VS2007_Silver);
			break;

		case ID_VIEW_APPLOOK_2007_3:
			CBCGPVisualManager2007::SetStyle(CBCGPVisualManager2007::VS2007_Aqua);
			break;
		}

		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManager2007));
		CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_2010_1:
	case ID_VIEW_APPLOOK_2010_2:
	case ID_VIEW_APPLOOK_2010_3:
		// enable Office 2010 look:
		switch (m_nAppLook)
		{
		case ID_VIEW_APPLOOK_2010_1:
			CBCGPVisualManager2010::SetStyle(CBCGPVisualManager2010::VS2010_Blue);
			break;

		case ID_VIEW_APPLOOK_2010_2:
			CBCGPVisualManager2010::SetStyle(CBCGPVisualManager2010::VS2010_Black);
			break;

		case ID_VIEW_APPLOOK_2010_3:
			CBCGPVisualManager2010::SetStyle(CBCGPVisualManager2010::VS2010_Silver);
			break;
		}
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManager2010));

		m_wndRibbonBar.SetScenicLook();
		m_wndRibbonBar.EnableMinimizeButton();
		CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WIN7:
		// enable Windows look:
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManagerScenic));

		m_wndRibbonBar.SetScenicLook();
		CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS2005:
		// enable VS 2005 look:
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManagerVS2005));
		CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS2008:
		// enable VS 2008 look:
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManagerVS2008));
		CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS2010:
		// enable VS 2010 look:
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManagerVS2010));
		CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS2012_LIGHT:
	case ID_VIEW_APPLOOK_VS2012_DARK:
		// enable VS 2012 look:
		CBCGPVisualManagerVS2012::SetStyle(m_nAppLook == ID_VIEW_APPLOOK_VS2012_LIGHT ?
			CBCGPVisualManagerVS2012::VS2012_Light : CBCGPVisualManagerVS2012::VS2012_Dark);
		CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManagerVS2012));
		CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
		break;
	}

	CBCGPDockManager* pDockManager = GetDockManager();
	if (pDockManager != NULL)
	{
		ASSERT_VALID(pDockManager);
		pDockManager->AdjustBarFrames();
	}

	CBCGPTabbedControlBar::ResetTabs();

	RecalcLayout();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), m_nAppLook);
}

void CMainFrame::OnUpdateAppLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(m_nAppLook == pCmdUI->m_nID);
}


void CMainFrame::OnAppColorTheme(UINT id)
{
	CWaitCursor wait;

	switch (id)
	{
	default:
	{
		if (m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(CPerformanceDlg)))
		{
			CPerformanceDlg* perpformanceDlg = DYNAMIC_DOWNCAST(CPerformanceDlg, m_wndView->GetCurSelDlg());
			ASSERT(perpformanceDlg);

			perpformanceDlg->UpdateChartColorTheme(id);

			if (perpformanceDlg->IsWindowVisible())
			{
				perpformanceDlg->RedrawWindow();
			}
		}
		break;
	}
	case ID_VIEW_COLOR_THEME_CUSTOM_COLOR:
	case ID_VIEW_COLOR_THEME_TEXTURES:
		break;
	}


	theApp.WriteInt(_T("ColorTheme"), id);
}

void CMainFrame::OnUpdateAppColorTheme(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	case TIMER_ID_SYS_TIME:
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		CString timeStr;
		timeStr.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time.wYear, time.wMonth,
			time.wDay, time.wHour, time.wMinute, time.wSecond);
		SetStatusBarPaneText(IDC_LABEL_SYS_TIME, timeStr);

		break;
	}
	case TIMER_ID_UPDATE_COUNT:
	{
		if (!m_bArRun)
		{
			break;
		}

		CString text;

		text.Format(_T("%ld"), m_arrayArApiEx[m_currChTx].getCountTx());
		SetStatusBarPaneText(IDC_LABEL_TX_COUNT, text);
		text.Format(_T("%ld"), m_arrayArApiEx[m_currChRx].getCountRx());
		SetStatusBarPaneText(IDC_LABEL_RX_COUNT, text);

		break;
	}
	default:
		break;
	}

	__super::OnTimer(nIDEvent);
}

void CMainFrame::OnClose()
{
	// 清空计数和界面显示
	if (m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)))
	{
		CWaitCursor waitCursor;

		SetStatusBarPaneText(IDC_LABEL_INFO, _T("请稍后，正在清除缓冲数据..."));
		m_wndStatusBar.RedrawWindow();

		((COutputDlg*)m_wndView->GetCurSelDlg())->SendMessage(WM_USER_CLEAN_TXT, COutputDlg::CHANNEL_INDEX_TX);
		((COutputDlg*)m_wndView->GetCurSelDlg())->SendMessage(WM_USER_CLEAN_TXT, COutputDlg::CHANNEL_INDEX_RX);
	}

	// delete
	SendMessage(WM_USER_DELETE_SUBDLG, -1, -1);

	// 
	m_wndView->DestroyWindow();

	SendMessage(WM_USER_DELETE_ARAPI, -1, -1);

	__super::OnClose();
}

CString GetModuleDir()
{
	HMODULE module = GetModuleHandle(0);
	TCHAR pFileName[MAX_PATH];
	GetModuleFileName(module, pFileName, MAX_PATH);

	CString csFullPath(pFileName);
	int nPos = csFullPath.ReverseFind(_T('\\'));
	if (nPos < 0)
		return CString("");
	else
		return csFullPath.Left(nPos);
}

void CMainFrame::OnButtonDataSheet()
{
	CString filePath("CRD_429_Px_IC数据手册.pdf");

#ifndef _DEBUG
	filePath.Insert(0, GetModuleDir() + _T("\\..\\Document\\"));
#endif  // !_DEBUG
	if (CFileFind().FindFile(filePath))
	{
		ShellExecute(m_hWnd, _T("Open"), filePath, NULL, NULL, SW_SHOW);
	}
	else
	{
		CString text;
		text.Format(_T("文件‘%s’不存在或路经不对！"), filePath);
		BCGPMessageBox(m_hWnd, text, _T("打开失败"), MB_OK);
	}
}

void CMainFrame::OnUpdateButtonDataSheet(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CMainFrame::OnComboBoardId()
{
	// 
	if (m_bArTurnOn)
	{
		((COutputDlg*)m_wndView->GetCurSelDlg())->TurnOff(FALSE);
		SendMessage(WM_COMMAND, IDC_BUTTON_AR_TURN);
	}

	// delete
	SendMessage(WM_USER_DELETE_ARAPI, -1, -1);

	// 
	CBCGPRibbonComboBox *pBoardIdCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_BOARD_ID));
	m_oldBoardId = pBoardIdCombo->GetCurSel();

	// 
	// create
	// 

	m_arrayChTxOutput = new CChTxOutput[ARINC_MAX_CH_NUM];
	m_arrayChRxOutput = new CChRxOutput[ARINC_MAX_CH_NUM];
	m_arrayArApiEx = new CArinc429[ARINC_MAX_CH_NUM];

	for (int i = 0; i < ARINC_MAX_CH_NUM; i++)
	{
		m_arrayArApiEx[i].arInfo().BoardID = m_oldBoardId;
		m_arrayArApiEx[i].arInfo().TX.ChID = i;
		m_arrayArApiEx[i].arInfo().RX.ChID = i;
		m_arrayArApiEx[i].enableDisplaySend(m_bDisplay);
		m_arrayArApiEx[i].enableDisplayRecv(m_bDisplay);
	}
	m_currChTx = 0;
	m_currChRx = 0;
	SyncConfigToScreen();

	((COutputDlg*)m_wndView->GetCurSelDlg())->SetArApiEx(&m_arrayArApiEx[m_currChTx], &m_arrayArApiEx[m_currChRx]);
	((COutputDlg*)m_wndView->GetCurSelDlg())->TurnOn(m_bDisplay);

	// 
	m_wndRibbonBar.RedrawWindow();
	m_wndRibbonBar.RecalcLayout();
}

void CMainFrame::OnComboTxId()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	CBCGPRibbonComboBox *pTxChIdCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_TX_ID));
	m_currChTx = pTxChIdCombo->GetCurSel();
	ARINC_INFO & arInfo = m_arrayArApiEx[m_currChTx].arInfo();
	arInfo.TX.ChID = m_currChTx;

	((COutputDlg*)m_wndView->GetCurSelDlg())->SetArApiEx(&m_arrayArApiEx[m_currChTx], &m_arrayArApiEx[m_currChRx]);

	// Log output
	CString text;
	text.Format(pTxChIdCombo->GetText() + CString("：%s"), pTxChIdCombo->GetItem(arInfo.TX.ChID));
	((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_TX, text);

	// 
	m_arrayArApiEx[m_currChTx].syncConfig();

	CBCGPRibbonComboBox *pTxBaudCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_TX_BAUD));
	CBCGPRibbonComboBox *pTxParityCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_TX_PARITY));
	CBCGPRibbonComboBox *pTxTestWayCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_TX_TEST_WAY));
	CBCGPRibbonComboBox *pTxPeriodCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_TX_PERIOD));
	pTxBaudCombo->SelectItem((int)arInfo.TX.Info.BaudRate);
	pTxParityCombo->SelectItem((int)arInfo.TX.Info.Parity);
	pTxPeriodCombo->SelectItem(arInfo.TX.Info.Period.Key);
	pTxTestWayCombo->SelectItem((int)arInfo.TX.Info.Loopback);
}

void CMainFrame::OnUpdateComboTxId(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn && !m_bArRun);
}

void CMainFrame::OnComboTxBaud()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	// 
	CBCGPRibbonComboBox *pTxBaudCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_TX_BAUD));

	int baudRate = pTxBaudCombo->GetCurSel();

	ARINC_INFO & arInfo = m_arrayArApiEx[m_currChTx].arInfo();

	// 
	int status = m_arrayArApiEx[m_currChTx].setConfig(ARU_TX_CH01_BIT_RATE + arInfo.TX.ChID, baudRate);
	if (ARS_NORMAL == status)
	{
		// Log output
		CString text;
		text.Format(pTxBaudCombo->GetText() + CString("：%s"), pTxBaudCombo->GetItem(baudRate));
		((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_TX, text);
	}
}

void CMainFrame::OnUpdateComboTxBaud(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn);
}

void CMainFrame::OnComboTxParity()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	// 
	CBCGPRibbonComboBox *pTxParityCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_TX_PARITY));

	int parity = pTxParityCombo->GetCurSel();

	ARINC_INFO & arInfo = m_arrayArApiEx[m_currChTx].arInfo();

	// 
	int status = m_arrayArApiEx[m_currChTx].setConfig(ARU_TX_CH01_PARITY + arInfo.TX.ChID, parity);
	if (ARS_NORMAL == status)
	{
		// Log output
		CString text;
		text.Format(pTxParityCombo->GetText() + CString("：%s"), pTxParityCombo->GetItem(parity));
		((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_TX, text);
	}
}

void CMainFrame::OnUpdateComboTxParity(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn);
}

void CMainFrame::OnComboTxPeriod()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	CBCGPRibbonComboBox *pTxPeriodCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_TX_PERIOD));
	ARINC_INFO & arInfo = m_arrayArApiEx[m_currChTx].arInfo();
	arInfo.TX.Info.Period.Key = pTxPeriodCombo->GetCurSel();
	arInfo.TX.Info.Period.Value = _g_tx_index_periods[arInfo.TX.Info.Period.Key];

	// Log output
	CString text;
	text.Format(pTxPeriodCombo->GetText() + CString("：%s"), pTxPeriodCombo->GetItem(arInfo.TX.Info.Period.Key));
	((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_TX, text);

	// 清空计数和界面显示
	SendMessage(WM_COMMAND, IDC_BUTTON_RESET_COUNT);
}

void CMainFrame::OnUpdateComboTxPeriod(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn);
}

void CMainFrame::OnComboTxTestWay()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	// 
	CBCGPRibbonComboBox *pTxTestWayCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_TX_TEST_WAY));

	int loopback = pTxTestWayCombo->GetCurSel();

	ARINC_INFO & arInfo = m_arrayArApiEx[m_currChTx].arInfo();

	int status = m_arrayArApiEx[m_currChTx].setConfig(ARU_CH01_LOOPBACK + arInfo.TX.ChID, loopback);
	if (ARS_NORMAL == status)
	{
		SetStatusBarPaneText(IDC_LABEL_COMMU_MODE, pTxTestWayCombo->GetItem(loopback));

		// Log output
		CString text;
		text.Format(pTxTestWayCombo->GetText() + CString("：%s"), pTxTestWayCombo->GetItem(loopback));
		((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_TX, text);
	}
}

void CMainFrame::OnUpdateComboTxTestWay(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn);
}

void CMainFrame::OnButtonTxData()
{
	CTxDataSourceDlg(&m_arrayArApiEx[m_currChTx], this).DoModal();
}

void CMainFrame::OnUpdateButtonTxData(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_arrayArApiEx != NULL && !m_arrayArApiEx[m_currChTx].isSendBusy());
}

void CMainFrame::OnButtonTxDataEdit()
{
	m_txDataEditDlg->SetArApiEx(&m_arrayArApiEx[m_currChTx]);
	m_txDataEditDlg->ShowWindow(SW_SHOWNORMAL);
	m_txDataEditDlg->SetFocus();
}

void CMainFrame::OnUpdateButtonTxDataEdit(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_arrayArApiEx[m_currChTx].isSendBusy());
}

void CMainFrame::OnButtonSendData()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	// 
	CBCGPRibbonButton *pSendDataButton = DYNAMIC_DOWNCAST(CBCGPRibbonButton,
		m_wndRibbonBar.FindByID(IDC_BUTTON_SEND));
	ARINC_INFO & arInfo = m_arrayArApiEx[m_currChTx].arInfo();

	m_arrayArApiEx[m_currChTx].enableSend();

	// Log output
	CString text;
	if (arInfo.TX.Info.EnPeriod)
	{
		text.Format(pSendDataButton->GetText() + CString("(HEX)：%08X ~ %08X"), arInfo.TX.Info.Data[0], arInfo.TX.Info.Data[1]);
	}
	else
	{
		text.Format(pSendDataButton->GetText() + CString("(HEX)：%08X"), arInfo.TX.Info.Data[0]);
	}

	((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_TX, text);
}

void CMainFrame::OnUpdateButtonSendData(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn && m_bArRun && !m_arrayArApiEx[m_currChTx].isSendBusy());
}

// Tx New
void CMainFrame::OnButtonTxNew()
{
	CBCGPRibbonComboBox *pTxIdCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_TX_ID));
	int currChannel = pTxIdCombo->GetCurSel();
	if (::IsWindowVisible(m_arrayChTxOutput[currChannel].m_hWnd))
	{
		m_arrayChTxOutput[currChannel].SetFocus();
	}
	else
	{
		m_arrayChTxOutput[currChannel].Create(CChTxOutput::IDD, GetDesktopWindow());
		m_arrayChTxOutput[currChannel].InitializeArInfo(&m_arrayArApiEx[currChannel]);
		m_arrayChTxOutput[currChannel].CenterWindow();
		m_arrayChTxOutput[currChannel].ShowWindow(SW_NORMAL);
	}

	// 
	((COutputDlg*)m_wndView->GetCurSelDlg())->EnableDisplay(FALSE);
}

void CMainFrame::OnUpdateButtonTxNew(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn && !m_bArRun);
}

void CMainFrame::OnComboRxId()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	CBCGPRibbonComboBox *pRxIdCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_RX_ID));
	m_currChRx = pRxIdCombo->GetCurSel();
	ARINC_INFO & arInfo = m_arrayArApiEx[m_currChRx].arInfo();
	arInfo.RX.ChID = m_currChRx;

	((COutputDlg*)m_wndView->GetCurSelDlg())->SetArApiEx(&m_arrayArApiEx[m_currChTx], &m_arrayArApiEx[m_currChRx]);

	// Log output
	CString text;
	text.Format(pRxIdCombo->GetText() + CString("：%s"), pRxIdCombo->GetItem(arInfo.RX.ChID));
	((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_RX, text);

	//
	m_arrayArApiEx[m_currChRx].syncConfig();

	CBCGPRibbonComboBox *pRxBaudCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_RX_BAUD));
	CBCGPRibbonComboBox *pRxParityCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_RX_PARITY));
	CBCGPRibbonComboBox *pRxPeriodCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_RX_PERIOD));

	pRxBaudCombo->SelectItem((int)arInfo.RX.Info.BaudRate);
	pRxParityCombo->SelectItem((int)arInfo.RX.Info.Parity);
	m_bDecoder = arInfo.RX.Info.Decoder;
	m_bSdiBit9 = arInfo.RX.Info.SDI.bits.BIT9;
	m_bSdiBit10 = arInfo.RX.Info.SDI.bits.BIT10;
	m_bFilter = arInfo.RX.Info.Label.Enable;

	// LABEL
	for (int i = 0; i < ARINC_LABEL_NUM; i++)
	{
		m_arrayArApiEx[m_currChRx].labelFilter(i, arInfo.RX.Info.Label.Data[i].Action);
	}
}

void CMainFrame::OnUpdateComboRxId(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn && !m_bArRun);
}

void CMainFrame::OnComboRxBaud()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	// 
	CBCGPRibbonComboBox *pRxBaudCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_RX_BAUD));
	ARINC_INFO & arInfo = m_arrayArApiEx[m_currChRx].arInfo();
	int baudRate = pRxBaudCombo->GetCurSel();

	int status = m_arrayArApiEx[m_currChRx].setConfig(ARU_RX_CH01_BIT_RATE + arInfo.RX.ChID, baudRate);
	if (ARS_NORMAL == status)
	{
		// Log output
		CString text;
		text.Format(pRxBaudCombo->GetText() + CString("：%s"), pRxBaudCombo->GetItem(baudRate));
		((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_RX, text);
	}
}

void CMainFrame::OnUpdateComboRxBaud(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn);
}

void CMainFrame::OnComboRxParity()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	// 
	CBCGPRibbonComboBox *pRxParityCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_RX_PARITY));
	ARINC_INFO & arInfo = m_arrayArApiEx[m_currChRx].arInfo();
	int parity = pRxParityCombo->GetCurSel();

	int status = m_arrayArApiEx[m_currChRx].setConfig(ARU_RX_CH01_PARITY + arInfo.RX.ChID, parity);
	if (ARS_NORMAL == status)
	{
		// Log output
		CString text;
		text.Format(pRxParityCombo->GetText() + CString("：%s"), pRxParityCombo->GetItem(parity));
		((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_RX, text);
	}
}

void CMainFrame::OnUpdateComboRxParity(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn);
}

void CMainFrame::OnButtonFilter()
{
	CRxLabelFilterDlg(&m_arrayArApiEx[m_currChRx], this).DoModal();
}

void CMainFrame::OnUpdateButtonFilter(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn && !m_arrayArApiEx[m_currChRx].isSendBusy());
}

void CMainFrame::OnCheckDecoder()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	//
	m_bDecoder = !m_bDecoder;
	ARINC_INFO & arInfo = m_arrayArApiEx[m_currChRx].arInfo();

	// 
	int status = m_arrayArApiEx[m_currChRx].setConfig(ARU_CH01_DECODER + arInfo.RX.ChID, m_bDecoder);
	CBCGPRibbonButton *pDecoderButton = DYNAMIC_DOWNCAST(CBCGPRibbonButton,
		m_wndRibbonBar.FindByID(IDC_CHECK_DECODER));
	if (status != ARS_NORMAL) {
		m_bDecoder = !m_bDecoder;
		BCGPMessageBox(CString(m_bDecoder ? "启用 " : "禁用 ") + pDecoderButton->GetText() + _T(" 失败！"));
		return;
	}

	// Log output
	((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_RX,
		CString(m_bDecoder ? "启用 " : "禁用 ") + pDecoderButton->GetText());
}

void CMainFrame::OnUpdateCheckDecoder(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn);
	pCmdUI->SetCheck(m_bDecoder);
}

void CMainFrame::OnCheckSdiBit9()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	//
	m_bSdiBit9 = !m_bSdiBit9;
	ARINC_INFO & arInfo = m_arrayArApiEx[m_currChRx].arInfo();

	ARINC429_SDI sdi;
	sdi.all = 0;
	sdi.bits.BIT9 = m_bSdiBit9;
	sdi.bits.BIT10 = arInfo.RX.Info.SDI.bits.BIT10;

	//
	int status = m_arrayArApiEx[m_currChRx].setConfig(ARU_CH01_SDI + arInfo.RX.ChID, sdi.all);
	CBCGPRibbonButton *pSdiBit9Button = DYNAMIC_DOWNCAST(CBCGPRibbonButton,
		m_wndRibbonBar.FindByID(IDC_CHECK_SDI_BIT9));
	if (status != ARS_NORMAL) {
		m_bSdiBit9 = !m_bSdiBit9;
		BCGPMessageBox(CString(m_bSdiBit9 ? "置位 " : "复位 ") + pSdiBit9Button->GetText() + _T(" 失败！"));
		return;
	}

	// Log output
	((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_RX,
		CString(m_bSdiBit9 ? "置位 " : "复位 ") + pSdiBit9Button->GetText());
}

void CMainFrame::OnUpdateCheckSdiBit9(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn && m_bDecoder);
	pCmdUI->SetCheck(m_bSdiBit9);
}

void CMainFrame::OnCheckSdiBit10()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	//! 
	m_bSdiBit10 = !m_bSdiBit10;
	ARINC_INFO & arInfo = m_arrayArApiEx[m_currChRx].arInfo();

	ARINC429_SDI sdi;
	sdi.all = 0;
	sdi.bits.BIT9 = arInfo.RX.Info.SDI.bits.BIT9;
	sdi.bits.BIT10 = m_bSdiBit10;

	// 
	int status = m_arrayArApiEx[m_currChRx].setConfig(ARU_CH01_SDI + arInfo.RX.ChID, sdi.all);
	CBCGPRibbonButton *pSdiBit10Button = DYNAMIC_DOWNCAST(CBCGPRibbonButton,
		m_wndRibbonBar.FindByID(IDC_CHECK_SDI_BIT10));
	if (status != ARS_NORMAL) {
		m_bSdiBit10 = !m_bSdiBit10;
		BCGPMessageBox(CString(m_bSdiBit10 ? "置位 " : "复位 ") + pSdiBit10Button->GetText() + _T(" 失败！"));
		return;
	}

	// Log output
	((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_RX,
		CString(m_bSdiBit10 ? "置位 " : "复位 ") + pSdiBit10Button->GetText());
}

void CMainFrame::OnUpdateCheckSdiBit10(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn && m_bDecoder);
	pCmdUI->SetCheck(m_bSdiBit10);
}

void CMainFrame::OnButtonSaveAs()
{
	// 用当前系统时间生成文件名称
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CString fileName;
	fileName.Format(_T("rou %4d-%02d-%02d %02d-%02d-%02d-%03d.txt"), sysTime.wYear, sysTime.wMonth,
		sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

	const TCHAR szFilter[] = _T("Text File(*.txt)|*.txt|");//Binary File(*.dat)|*.dat|");
	CFileDialog fileDlg(FALSE, _T("*.txt"), fileName, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, szFilter, this);
	if (fileDlg.DoModal() == IDOK)
	{
		CString	szFolderPath = fileDlg.GetPathName();
		int offset = szFolderPath.ReverseFind(_T('\\'));
		if (offset > 0)
		{
			szFolderPath.ReleaseBuffer(offset);
			CString saveDir = szFolderPath + _T("\\log\\");
		}
		else
		{
			BCGPMessageBox(_T("路径错误：！") + szFolderPath, MB_ICONERROR);
		}
	}
}

// Rx New
void CMainFrame::OnButtonRxNew()
{
	CBCGPRibbonComboBox *pRxIdCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		m_wndRibbonBar.FindByID(IDC_COMBO_RX_ID));
	int currChannel = pRxIdCombo->GetCurSel();
	if (::IsWindowVisible(m_arrayChRxOutput[currChannel].m_hWnd))
	{
		m_arrayChRxOutput[currChannel].SetFocus();
	}
	else
	{
		m_arrayChRxOutput[currChannel].Create(CChRxOutput::IDD, GetDesktopWindow());
		m_arrayChRxOutput[currChannel].InitializeArInfo(&m_arrayArApiEx[currChannel]);
		m_arrayChRxOutput[currChannel].CenterWindow();
		m_arrayChRxOutput[currChannel].ShowWindow(SW_NORMAL);
	}

	// 
	((COutputDlg*)m_wndView->GetCurSelDlg())->EnableDisplay(FALSE);
}

void CMainFrame::OnUpdateButtonRxNew(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn && !m_bArRun);
}

// Turn
void CMainFrame::OnButtonArTurn()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	if (ar_board_num() == 0)
	{
		BCGPMessageBox(_T("没有可用设备"));
		return;	// 返回
	}
	
	// 
	SendMessage(WM_USER_DELETE_SUBDLG, -1, -1);

	// 
	int status = m_bArTurnOn ? m_arrayArApiEx[m_currChTx].close() : m_arrayArApiEx[m_currChTx].loadslv();

	CString text;

	// 
	if (ARS_INVBOARD == status)
	{
		text.Format(_T("设备 %d 不存在！"), m_arrayArApiEx[m_currChTx].arInfo().BoardID);
		BCGPMessageBox(text);
		return;	// 返回
	}
	else if (ARS_BRDNOTLOAD == status)
	{
		text.Format(_T("设备 %d 没有加载！"), m_arrayArApiEx[m_currChTx].arInfo().BoardID);
		BCGPMessageBox(text);
		return;	// 返回
	}
	else if (ARS_WINRTFAIL == status)
	{
		text.Format(m_bArTurnOn ? _T("设备 %d 卸载失败！") : _T("设备 %d 加载失败！"),
			m_arrayArApiEx[m_currChTx].arInfo().BoardID);
		BCGPMessageBox(text);
		return;	// 返回
	}
	//
	m_bArTurnOn = !m_bArTurnOn;
	m_bArRun = FALSE;

	if (m_bArTurnOn)
	{
		// create
		ASSERT(m_arrayChTxOutput == NULL && m_arrayChRxOutput == NULL);
		m_arrayChTxOutput = new CChTxOutput[ARINC_MAX_CH_NUM];
		m_arrayChRxOutput = new CChRxOutput[ARINC_MAX_CH_NUM];

		SyncConfigToScreen();
	}

	// 
	m_arrayArApiEx[m_currChTx].enableSend(m_bArRun);

	//
	CBCGPRibbonButton *pArTurnButton = DYNAMIC_DOWNCAST(CBCGPRibbonButton,
		m_wndRibbonBar.FindByID(IDC_BUTTON_AR_TURN));
	pArTurnButton->SetText(m_bArTurnOn ? _T("卸载") : _T("加载"));
	pArTurnButton->SetToolTipText(m_bArTurnOn ? _T("卸载Arinc429") : _T("加载Arinc429"));
	CBCGPRibbonButton *pArPlayButton = DYNAMIC_DOWNCAST(CBCGPRibbonButton,
		m_wndRibbonBar.FindByID(IDC_BUTTON_AR_PLAY));
	pArPlayButton->SetImageIndex(m_bArRun ? 5 : 4, TRUE);
	pArPlayButton->SetText(m_bArRun ? _T("停止") : _T("启动"));
	pArPlayButton->SetToolTipText(m_bArRun ? _T("点击后，复位Arinc429，并停止数据传输") : _T("点击后，运行Arinc429，并启动数据传输"));

	// 清空计数和界面显示
	SendMessage(WM_COMMAND, IDC_BUTTON_RESET_COUNT);

	// 状态栏
	SetStatusBarPaneText(IDC_LABEL_COMMU_STATUS, m_bArTurnOn ? _T("连接") : _T("断开"));

	// Log output
	text.Format(m_bArTurnOn ? _T("加载设备 %d") : _T("卸载设备 %d"), m_arrayArApiEx[m_currChTx].arInfo().BoardID);
	((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_TX, text);
	((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_RX, text);
}

void CMainFrame::OnUpdateButtonArTurn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(ar_board_num() > 0);
}

// Start
void CMainFrame::OnButtonArPlay()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	//
	int status = m_bArRun ? m_arrayArApiEx[m_currChTx].reset() : m_arrayArApiEx[m_currChTx].go();

	CString text;

	//
	if (ARS_INVBOARD == status)
	{
		text.Format(_T("设备 %d 不存在！"), m_arrayArApiEx[m_currChTx].arInfo().BoardID);
		BCGPMessageBox(text);
		return;	// 返回
	}
	else if (ARS_BRDNOTLOAD == status)
	{
		text.Format(_T("设备 %d 没有加载！"), m_arrayArApiEx[m_currChTx].arInfo().BoardID);
		BCGPMessageBox(text);
		return;	// 返回
	}
	else if (ARS_WINRTFAIL == status)
	{
		text.Format(m_bArRun ? _T("设备 %d 无法复位！") : _T("设备 %d 无法运行！"),
			m_arrayArApiEx[m_currChTx].arInfo().BoardID);
		BCGPMessageBox(text);
		return;	// 返回
	}

	// 运行 <--> 复位
	m_bArRun = !m_bArRun;

	if (m_bArRun)
	{
		((COutputDlg*)m_wndView->GetCurSelDlg())->TurnOn(m_bDisplay);
	}
	else
	{
		((COutputDlg*)m_wndView->GetCurSelDlg())->TurnOff(m_bDisplay);
	}

	// 
	CBCGPRibbonButton *pArPlayButton = DYNAMIC_DOWNCAST(CBCGPRibbonButton,
		m_wndRibbonBar.FindByID(IDC_BUTTON_AR_PLAY));
	pArPlayButton->SetImageIndex(m_bArRun ? 5 : 4, TRUE);
	pArPlayButton->SetText(m_bArRun ? _T("停止") : _T("启动"));
	pArPlayButton->SetToolTipText(m_bArRun ? _T("点击后，复位Arinc429，并停止数据传输") : _T("点击后，运行Arinc429，并启动数据传输"));

	//! Log output
	text.Format(m_bArRun ? _T("运行设备 %d") : _T("复位设备 %d"), m_arrayArApiEx[m_currChTx].arInfo().BoardID);
	((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_TX, text);
	((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_RX, text);
}

void CMainFrame::OnUpdateButtonArPlay(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn);
}

void CMainFrame::OnButtonArSync()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	//
	m_arrayArApiEx[m_currChTx].syncConfig();
	m_arrayArApiEx[m_currChRx].syncConfig();

	// 
	SyncConfigToScreen();

	// Log output
	CBCGPRibbonButton *pArSyncButton = DYNAMIC_DOWNCAST(CBCGPRibbonButton,
		m_wndRibbonBar.FindByID(IDC_BUTTON_AR_SYNC));
	((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_TX, pArSyncButton->GetToolTipText());
	((COutputDlg*)m_wndView->GetCurSelDlg())->MyLogOut(COutputDlg::CHANNEL_INDEX_RX, pArSyncButton->GetToolTipText());
}

void CMainFrame::OnUpdateButtonArSync(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn);
}

void CMainFrame::OnCheckDislpay()
{
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));
	CBCGPRibbonCheckBox *pCheckDisplay = DYNAMIC_DOWNCAST(CBCGPRibbonCheckBox,
		m_wndRibbonBar.FindByID(IDC_CHECK_DISPLAY));
	m_bDisplay = !m_bDisplay;

	BOOL enableDisplay = m_bDisplay;

	for (int i = 0; i < ARINC_MAX_CH_NUM; i++)
	{
		m_arrayArApiEx[i].enableDisplaySend(m_bDisplay);
		m_arrayArApiEx[i].enableDisplayRecv(m_bDisplay);

		if (!enableDisplay || m_arrayChTxOutput != NULL)
		{
			if (::IsWindowVisible(m_arrayChTxOutput[i].m_hWnd))
			{
				enableDisplay = FALSE;
			}
		}

		if (!enableDisplay || m_arrayChRxOutput != NULL)
		{
			if (::IsWindowVisible(m_arrayChRxOutput[i].m_hWnd))
			{
				enableDisplay = FALSE;
			}
		}
	}

	((COutputDlg*)m_wndView->GetCurSelDlg())->EnableDisplay(enableDisplay);

	theApp.WriteInt(_T("DisplayOutput"), m_bDisplay);
}

void CMainFrame::OnUpdateCheckDislpay(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bArTurnOn);
	pCmdUI->SetCheck(m_bDisplay);
}

//! StatusBar

void CMainFrame::OnButtonResetCount()
{
	CWaitCursor waitCursor;

	if (!m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)))
	{
		return;
	}
	ASSERT(m_wndView->GetCurSelDlg()->IsKindOf(RUNTIME_CLASS(COutputDlg)));

	SetStatusBarPaneText(IDC_LABEL_INFO, _T("请稍后，正在清除缓冲数据..."));
	m_wndStatusBar.RedrawWindow();

	((COutputDlg*)m_wndView->GetCurSelDlg())->SendMessage(WM_USER_CLEAN_TXT, COutputDlg::CHANNEL_INDEX_TX);
	((COutputDlg*)m_wndView->GetCurSelDlg())->SendMessage(WM_USER_CLEAN_TXT, COutputDlg::CHANNEL_INDEX_RX);

	// 
	if (m_arrayArApiEx)
	{
		m_arrayArApiEx[m_currChTx].resetCountTx();
		m_arrayArApiEx[m_currChTx].resetCountRx();
		m_arrayArApiEx[m_currChRx].resetCountTx();
		m_arrayArApiEx[m_currChRx].resetCountRx();
	}

	// 
	SetStatusBarPaneText(IDC_LABEL_INFO, _T(""));
}

LRESULT CMainFrame::OnUserDeleteSubDlg(WPARAM wParam, LPARAM lParam)
{
	switch ((int)wParam)
	{
	case -1:
	{
		if (m_arrayChTxOutput != NULL)
		{
			for (int i = 0; i < ARINC_MAX_CH_NUM; i++)
			{
				if (::IsWindowVisible(m_arrayChTxOutput[i].m_hWnd))
				{
					m_arrayChTxOutput[i].DestroyWindow();
				}
			}
			delete[] m_arrayChTxOutput;
			m_arrayChTxOutput = NULL;
		}

		if (m_arrayChRxOutput != NULL)
		{
			for (int i = 0; i < ARINC_MAX_CH_NUM; i++)
			{
				if (::IsWindowVisible(m_arrayChRxOutput[i].m_hWnd))
				{
					m_arrayChRxOutput[i].DestroyWindow();
				}
			}
			delete[] m_arrayChRxOutput;
			m_arrayChRxOutput = NULL;
		}

		break;
	}
	case 0:
	{
		if (m_arrayChTxOutput == NULL)
		{
			return 0;
		}

		if ((int)lParam <= -1)
		{
			for (int i = 0; i < ARINC_MAX_CH_NUM; i++)
			{
				if (::IsWindowVisible(m_arrayChTxOutput[i].m_hWnd))
				{
					m_arrayChTxOutput[i].DestroyWindow();
				}
			}
			delete[] m_arrayChTxOutput;
			m_arrayChTxOutput = NULL;
		}
		else
		{
			if (ARINC_MAX_CH_NUM < (int)lParam)
			{
				ASSERT(FALSE);
				return 0;
			}
			else
			{
				if (::IsWindowVisible(m_arrayChTxOutput[lParam].m_hWnd))
				{
					m_arrayChTxOutput[lParam].DestroyWindow();
				}
			}
		}
		break;
	}
	case 1:
	{
		if (m_arrayChRxOutput == NULL)
		{
			return 0;
		}

		if ((int)lParam <= -1)
		{
			for (int i = 0; i < ARINC_MAX_CH_NUM; i++)
			{
				if (::IsWindowVisible(m_arrayChRxOutput[i].m_hWnd))
				{
					m_arrayChRxOutput[i].DestroyWindow();
				}
			}
			delete[] m_arrayChRxOutput;
			m_arrayChRxOutput = NULL;
		}
		else
		{
			if (ARINC_MAX_CH_NUM < (int)lParam)
			{
				ASSERT(FALSE);
				return 0;
			}
			else
			{
				if (::IsWindowVisible(m_arrayChRxOutput[lParam].m_hWnd))
				{
					m_arrayChRxOutput[lParam].DestroyWindow();
				}
			}
		}
		break;
	}
	default: ASSERT(FALSE); break;
	}

	return 0;
}

LRESULT CMainFrame::OnUserDeleteArApi(WPARAM wParam, LPARAM lParam)
{
	if (m_arrayArApiEx == NULL)
	{
		return 0;
	}

	if ((int)wParam == -1)
	{
		for (int i = 0; i < ARINC_MAX_CH_NUM; i++)
		{
			m_arrayArApiEx[i].close();
		}
		delete[] m_arrayArApiEx;
		m_arrayArApiEx = NULL;
	}
	else
	{
		if (ARINC_MAX_CH_NUM < (int)wParam)
		{
			ASSERT(FALSE);
			return 0;
		}
		else
		{
			m_arrayArApiEx[wParam].close();
		}
	}
	return 0;
}

BOOL CMainFrame::CreateRibbonBar()
{
	m_wndRibbonBar.EnableCustomization();

	if (!m_wndRibbonBar.Create(this))
	{
		return FALSE;
	}

	if (!m_wndRibbonBar.LoadFromXML(_T("IDR_BCGP_RIBBON_XML")))
	{
		return FALSE;
	}

	CBCGPRibbonCategory* pBackstageCategory = m_wndRibbonBar.GetBackstageCategory();

	if (pBackstageCategory != NULL)
	{
		CBCGPRibbonBackstageViewPanel* pBackstagePanel = DYNAMIC_DOWNCAST(CBCGPRibbonBackstageViewPanel,
			pBackstageCategory->GetPanel(0));

		if (pBackstagePanel != NULL)
		{
			pBackstagePanel->AttachViewToItem(0, new CBCGPRibbonBackstageViewItemForm(CGeneralConfigDlg::IDD,
				RUNTIME_CLASS(CGeneralConfigDlg)), FALSE);
		}
	}

	// USE_RIBBON_DESIGNER
	return TRUE;
}


// 设置状态栏中某一个Pane的文本
void CMainFrame::SetStatusBarPaneText(UINT nIDC, LPCTSTR text, BOOL reDraw/* = TRUE*/)
{
	ASSERT(text != NULL);
	CBCGPBaseRibbonElement* pElementTimer = m_wndStatusBar.FindByID(nIDC);
	ASSERT(pElementTimer != NULL);
	pElementTimer->SetText(text);
	if (reDraw) {
		m_wndStatusBar.RedrawWindow();
		m_wndStatusBar.RecalcLayout();
	}
}
