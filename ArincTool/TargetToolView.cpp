// TargetToolView.cpp : implementation of the CTargetToolView class
//

#include "stdafx.h"
#include "TargetTool.h"
#include "MainFrm.h"
#include "TargetToolDoc.h"
#include "TargetToolView.h"

#include "OutputDlg.h"
#include "PerformanceDlg.h"

#include "UserWinMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTargetToolView

IMPLEMENT_DYNCREATE(CTargetToolView, CView)

BEGIN_MESSAGE_MAP(CTargetToolView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(BCGM_ON_CHANGE_RIBBON_CATEGORY, &ThisClass::OnChangeRibbonCategory)
	ON_COMMAND(IDC_COMBO_PER_BOARD_ID, &ThisClass::OnComboPerBoardId)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_PER_BOARD_ID, &ThisClass::OnUpdateComboPerBoardId)
	ON_COMMAND(IDC_COMBO_PER_LOOPBACK, &ThisClass::OnComboPerLoopback)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_PER_LOOPBACK, &ThisClass::OnUpdateComboPerLoopback)
	ON_COMMAND(IDC_COMBO_PER_BAUDRATE, &ThisClass::OnComboPerBaudRate)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_PER_BAUDRATE, &ThisClass::OnUpdateComboPerBaudRate)
	ON_COMMAND(IDC_COMBO_PER_PARITY, &ThisClass::OnComboPerParity)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_PER_PARITY, &ThisClass::OnUpdateComboPerParity)
	ON_COMMAND(IDC_BUTTON_PER_START, &ThisClass::OnButtonPerStart)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_PER_START, &ThisClass::OnUpdateButtonPerStart)
	ON_COMMAND(IDC_BUTTON_PER_STOP, &ThisClass::OnButtonPerStop)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_PER_STOP, &ThisClass::OnUpdateButtonPerStop)
END_MESSAGE_MAP()

// CTargetToolView construction/destruction

CTargetToolView::CTargetToolView()
{
	m_layout		= NULL;
	m_pCurSelDlg	= NULL;

	m_bEnFlushBn	= FALSE;		// 默认不使能刷新按钮
	m_bEnQuitBn		= FALSE;		// 默认不使能返回按钮
	m_bEnFlush		= FALSE;		// 默认禁止刷新数据
}

CTargetToolView::~CTargetToolView()
{
	if (m_layout != NULL)
	{
		delete m_layout;
	}

	if (m_pCurSelDlg != NULL)
	{
		delete m_pCurSelDlg;
	}
}

BOOL CTargetToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CTargetToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetDocument()->SetTitle(_T("ArincTool"));

	// 
	((CMainFrame*)AfxGetMainWnd())->SetView(this);					// 初始化CMainFrm中的m_wndView
	m_wndFrame = reinterpret_cast<CMainFrame*> (AfxGetMainWnd());	// 初始化CMBITView中的m_wndFrame

	// 默认选择输出对话框
	OnSwitchDialog<COutputDlg>();

	// 
	((COutputDlg*)m_pCurSelDlg)->SetArApiEx(
		&m_wndFrame->m_arrayArApiEx[m_wndFrame->m_currChTx],
		&m_wndFrame->m_arrayArApiEx[m_wndFrame->m_currChRx]);

	// 
	m_wndFrame->OnAppColorTheme(theApp.GetInt(_T("ColorTheme"), ID_VIEW_COLOR_THEME_OLIVE));
}

// CTargetToolView drawing

void CTargetToolView::OnDraw(CDC* /*pDC*/)
{
	CTargetToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

BOOL CTargetToolView::DestroyWindow()
{
	if (m_pCurSelDlg != NULL)
	{
		m_pCurSelDlg->DestroyWindow();
	}

	return __super::DestroyWindow();
}


// CTargetToolView diagnostics

#ifdef _DEBUG
void CTargetToolView::AssertValid() const
{
	CView::AssertValid();
}

void CTargetToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTargetToolDoc* CTargetToolView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTargetToolDoc)));
	return (CTargetToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CTargetToolView message handlers


BOOL CTargetToolView::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

void CTargetToolView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if (m_layout != NULL)
	{
		if (m_pCurSelDlg != NULL)
		{
			m_pCurSelDlg->SetRedraw(FALSE);
		}

 		m_layout->AdjustLayout();

		if (m_pCurSelDlg != NULL)
		{
			m_pCurSelDlg->SetRedraw(TRUE);
			m_pCurSelDlg->RedrawWindow();
		}
	}
}

// WM_CHANGE_RIBBON_CATEGORY
LRESULT CTargetToolView::OnChangeRibbonCategory(WPARAM wParam, LPARAM lParam)
{
	CBCGPRibbonBar* ribbonBar = reinterpret_cast<CBCGPRibbonBar*>(lParam);
	int index = ribbonBar->GetCategoryIndex(ribbonBar->GetActiveCategory());
	switch (index)
	{
	case 1:
	{
		m_isPerRunning = FALSE;
		OnSwitchDialog<COutputDlg>();

		// 
		((COutputDlg*)m_pCurSelDlg)->SetArApiEx(
			&m_wndFrame->m_arrayArApiEx[m_wndFrame->m_currChTx],
			&m_wndFrame->m_arrayArApiEx[m_wndFrame->m_currChRx]);

		break;
	}
	case 2:
	{
		m_isPerRunning = FALSE;
		OnSwitchDialog<CPerformanceDlg>();

		CBCGPRibbonCategory* category = m_wndFrame->GetRibbonBar()->GetActiveCategory();

		CPerformanceTransmit &perTransmit = reinterpret_cast<CPerformanceDlg*>(m_pCurSelDlg)->getPerTransmit();

		// 初始化工具栏信息

		BOARD_PER_CONFIG & boardConfig = perTransmit.getBoardConfig();
		CString text;

		// board
		CBCGPRibbonEdit *pBoardCountEdit = DYNAMIC_DOWNCAST(CBCGPRibbonEdit,
			category->FindByID(IDC_EDIT_PER_BOARD_COUNT));
		text.Format(_T("%d"), boardConfig.BoardCount);
		pBoardCountEdit->SetEditText(text);

		CBCGPRibbonComboBox *pBoardIdCombo = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
			category->FindByID(IDC_COMBO_PER_BOARD_ID));
		pBoardIdCombo->RemoveAllItems();
		for (int i = 0; i < boardConfig.BoardCount; i++)
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

		pBoardIdCombo->SelectItem(boardConfig.Id);

		int index = -1;

		// Loopback
		CBCGPRibbonComboBox *pComboBoxLoopback = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
			category->FindByID(IDC_COMBO_PER_LOOPBACK));
		switch (boardConfig.Loopback)
		{
		case AR_WRAP_OFF: index = 0; break;
		case AR_WRAP_ON: index = 1; break;
		default: ASSERT(FALSE); break;
		}
		pComboBoxLoopback->SelectItem(index);

		// BaudRate
		CBCGPRibbonComboBox *pComboBoxBaudRate = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
			category->FindByID(IDC_COMBO_PER_BAUDRATE));
		switch (boardConfig.BaudRate)
		{
		case AR_12_5K: index = 0; break;
		case AR_50K: index = 1; break;
		case AR_100K: index = 2; break;
		default: ASSERT(FALSE); break;
		}
		pComboBoxBaudRate->SelectItem(index);

		// Parity
		CBCGPRibbonComboBox *pComboBoxParity = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
			category->FindByID(IDC_COMBO_PER_PARITY));
		switch (boardConfig.BaudRate)
		{
		case AR_NONE: index = 0; break;
		case AR_ODD: index = 1; break;
		case AR_EVEN: index = 2; break;
		default: ASSERT(FALSE); break;
		}
		pComboBoxParity->SelectItem(index);

		break;
	}
	default:break;
	}

	return 0;
}

void CTargetToolView::OnComboPerBoardId()
{
	// 
	m_pCurSelDlg->SendMessage(WM_USER_PER_SWITCH, FALSE, TRUE);

	CPerformanceTransmit &perTransmit = reinterpret_cast<CPerformanceDlg*>(m_pCurSelDlg)->getPerTransmit();
	BOARD_PER_CONFIG & boardConfig = perTransmit.getBoardConfig();
	CBCGPRibbonCategory* category = m_wndFrame->GetRibbonBar()->GetActiveCategory();

	CBCGPRibbonComboBox *pComboBoxBoardId = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		category->FindByID(IDC_COMBO_PER_BOARD_ID));
	boardConfig.Id = pComboBoxBoardId->GetCurSel();
}

void CTargetToolView::OnUpdateComboPerBoardId(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_isPerRunning);
}

void CTargetToolView::OnComboPerLoopback()
{
	CPerformanceTransmit &perTransmit = reinterpret_cast<CPerformanceDlg*>(m_pCurSelDlg)->getPerTransmit();
	BOARD_PER_CONFIG & boardConfig = perTransmit.getBoardConfig();
	CBCGPRibbonCategory* category = m_wndFrame->GetRibbonBar()->GetActiveCategory();

	CBCGPRibbonComboBox *pComboBoxLoopback = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		category->FindByID(IDC_COMBO_PER_LOOPBACK));
	switch (pComboBoxLoopback->GetCurSel())
	{
	case 0: boardConfig.Loopback = AR_WRAP_OFF; break;
	case 1: boardConfig.Loopback = AR_WRAP_ON; break;
	default: ASSERT(FALSE); break;
	}
	ar_set_config(boardConfig.Id, ARU_INTERNAL_WRAP, boardConfig.Loopback);
}

void CTargetToolView::OnUpdateComboPerLoopback(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CTargetToolView::OnComboPerBaudRate()
{
	CPerformanceTransmit &perTransmit = reinterpret_cast<CPerformanceDlg*>(m_pCurSelDlg)->getPerTransmit();
	BOARD_PER_CONFIG & boardConfig = perTransmit.getBoardConfig();
	CBCGPRibbonCategory* category = m_wndFrame->GetRibbonBar()->GetActiveCategory();

	CBCGPRibbonComboBox *pComboBoxBaudRate= DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		category->FindByID(IDC_COMBO_PER_BAUDRATE));
	switch (pComboBoxBaudRate->GetCurSel())
	{
	case 0: boardConfig.BaudRate = AR_12_5K; break;
	case 1: boardConfig.BaudRate = AR_50K; break;
	case 2: boardConfig.BaudRate = AR_100K; break;
	default: ASSERT(FALSE); break;
	}
	ar_set_config(boardConfig.Id, ARU_RECV_RATE, boardConfig.BaudRate);
	ar_set_config(boardConfig.Id, ARU_XMIT_RATE, boardConfig.BaudRate);
}

void CTargetToolView::OnUpdateComboPerBaudRate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CTargetToolView::OnComboPerParity()
{
	CPerformanceTransmit &perTransmit = reinterpret_cast<CPerformanceDlg*>(m_pCurSelDlg)->getPerTransmit();
	BOARD_PER_CONFIG & boardConfig = perTransmit.getBoardConfig();
	CBCGPRibbonCategory* category = m_wndFrame->GetRibbonBar()->GetActiveCategory();

	CBCGPRibbonComboBox *pComboBoxParity = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		category->FindByID(IDC_COMBO_PER_PARITY));
	switch (pComboBoxParity->GetCurSel())
	{
	case 0: boardConfig.Parity = AR_NONE; break;
	case 1: boardConfig.Parity = AR_ODD; break;
	case 2: boardConfig.Parity = AR_EVEN; break;
	default: ASSERT(FALSE); break;
	}
	ar_set_config(boardConfig.Id, ARU_PARITY, boardConfig.Parity);
}

void CTargetToolView::OnUpdateComboPerParity(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CTargetToolView::OnButtonPerStart()
{
	CPerformanceTransmit &perTransmit = reinterpret_cast<CPerformanceDlg*>(m_pCurSelDlg)->getPerTransmit();

	// 初始化工具栏信息

	BOARD_PER_CONFIG & boardConfig = perTransmit.getBoardConfig();

	if (boardConfig.BoardCount == 0)
	{
		BCGPMessageBox(_T("没有可用设备"));
		return;	// 返回
	}

	CBCGPRibbonCategory* category = m_wndFrame->GetRibbonBar()->GetActiveCategory();
	CString text;

	int index = 0;

	// Loopback
	CBCGPRibbonComboBox *pComboBoxLoopBack = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		category->FindByID(IDC_COMBO_PER_LOOPBACK));
	pComboBoxLoopBack->SelectItem((boardConfig.Loopback == AR_WRAP_ON) ? 1 : 0);
	// BaudRate
	CBCGPRibbonComboBox *pComboBoxBaudRate = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		category->FindByID(IDC_COMBO_PER_BAUDRATE));
	switch (boardConfig.BaudRate)
	{
	case AR_12_5K: index = 0; break;
	case AR_50K: index = 1; break;
	case AR_100K: index = 2; break;
	default: ASSERT(FALSE); break;
	}
	pComboBoxBaudRate->SelectItem(index);
	// Parity
	CBCGPRibbonComboBox *pComboBoxParity = DYNAMIC_DOWNCAST(CBCGPRibbonComboBox,
		category->FindByID(IDC_COMBO_PER_PARITY));
	switch (boardConfig.Parity)
	{
	case AR_NONE: index = 0; break;
	case AR_ODD: index = 1; break;
	case AR_EVEN: index = 2; break;
	default: ASSERT(FALSE); break;
	}
	pComboBoxParity->SelectItem(index);

	if (m_pCurSelDlg->SendMessage(WM_USER_PER_SWITCH, TRUE, FALSE))
	{
		m_isPerRunning = TRUE;
	}
}

void CTargetToolView::OnUpdateButtonPerStart(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_isPerRunning);
}

void CTargetToolView::OnButtonPerStop()
{
	m_pCurSelDlg->SendMessage(WM_USER_PER_SWITCH, FALSE, FALSE);
	m_isPerRunning = FALSE;
}

void CTargetToolView::OnUpdateButtonPerStop(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_isPerRunning);
}

template<typename dialog> void CTargetToolView::OnSwitchDialog()
{
	if (m_pCurSelDlg != NULL)
	{
		if (m_pCurSelDlg->IsKindOf(RUNTIME_CLASS(dialog)))
		{
			return;		// 之前已选中，直接返回
		}

		m_pCurSelDlg->DestroyWindow();
		delete m_pCurSelDlg;
	}
	
	if (m_layout != NULL)
	{
		delete m_layout;
	}

	// 
	m_pCurSelDlg = new dialog();
	m_pCurSelDlg->Create(dialog::IDD, this);

	CRect rcView; GetClientRect(rcView);
	m_pCurSelDlg->MoveWindow(&rcView);

	// 
	m_layout = new CBCGPStaticLayout;
	m_layout->Create(this);
	m_layout->AddAnchor(m_pCurSelDlg->m_hWnd, LAYOUT_MOVE_NONE, LAYOUT_SIZE_BOTH);

	m_pCurSelDlg->ShowWindow(SW_SHOW);					// 显示
}
