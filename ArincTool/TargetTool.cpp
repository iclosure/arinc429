// TargetTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TargetTool.h"
#include "MainFrm.h"
#include "TargetToolDoc.h"
#include "TargetToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "version.lib")

// CAboutDlg dialog used for App About

class CAboutDlg : public CBCGPDialog
{
public:
	CAboutDlg(CWnd* pParent = NULL);

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };
	CBCGPURLLinkButton m_btnURL;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	// Implementation
protected:
	DECLARE_MESSAGE_MAP()

private:
	CString			m_labelVersion;
	CHARFORMAT2		m_cfmsEdit;					// 
};

// CTargetToolApp

BEGIN_MESSAGE_MAP(CTargetToolApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &ThisClass::OnAppAbout)
END_MESSAGE_MAP()


// CTargetToolApp construction

typedef int(__stdcall*FUNC_AR_BOARD_TYPE)(int board);

CTargetToolApp::CTargetToolApp() :
	CBCGPWorkspace (TRUE)
{
}

// The one and only CTargetToolApp object

CTargetToolApp theApp;


// CTargetToolApp initialization

BOOL CTargetToolApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	_CrtSetBreakAlloc(138);  //

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	// Initialize Richedit2 Library.
	if (!AfxInitRichEdit2())
	{
		AfxMessageBox(_T("Fail to Initialize Richedit2 Library."));
		return FALSE;
	}

	AfxEnableControlContainer();

	globalData.SetDPIAware ();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("CRDTech\\Driver Tools"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	SetRegistryBase (_T("Settings"));

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	InitContextMenuManager();
	InitKeyboardManager();

	// TODO: Remove this if you don't want extended tooltips:
	InitTooltipManager();

	CBCGPToolTipParams params;
	params.m_bVislManagerTheme = TRUE;

// 	CBCGPVisualManager2010::SetStyle (CBCGPVisualManager2010::VS2010_Black);
// 	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2010));
// 	CBCGPButton::EnableWinXPTheme(TRUE);
// 
// 	globalData.m_bUseVisualManagerInBuiltInDialogs = TRUE;

	theApp.GetTooltipManager ()->SetTooltipParams (
		BCGP_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS (CBCGPToolTipCtrl),
		&params);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTargetToolDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CTargetToolView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

int CTargetToolApp::ExitInstance() 
{
	//! 
	BCGCBProCleanUp();

	return CWinApp::ExitInstance();
}

// CTargetToolApp message handlers

// App command to run the dialog
void CTargetToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

CAboutDlg::CAboutDlg(CWnd* pParent/* = NULL*/)
	: CBCGPDialog(CAboutDlg::IDD, pParent)
	, m_labelVersion(_T(""))
{
	EnableVisualManagerStyle(TRUE, TRUE);
	EnableLayout();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_COMPANY_URL, m_btnURL);
	DDX_Text(pDX, IDC_LABEL_VERSION, m_labelVersion);
}

BOOL CAboutDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// 自动布局设置
	CBCGPStaticLayout* layout = reinterpret_cast<CBCGPStaticLayout*> ( GetLayout() );
	if (layout != NULL)
	{
		layout->AddAnchor(IDC_REDIT_ABOUT, LAYOUT_MOVE_NONE, LAYOUT_SIZE_BOTH);
		layout->AddAnchor(IDC_BUTTON_COMPANY_URL, LAYOUT_MOVE_VERT, LAYOUT_SIZE_NONE);
		layout->AddAnchor(IDOK,            LAYOUT_MOVE_HORZ, LAYOUT_SIZE_NONE);
	}

	// 初始化控件
	m_labelVersion = _T("ArincTool Version ") + theApp.GetAppVersion();
	UpdateData(FALSE);

	// 初始化变量
	m_cfmsEdit.cbSize = sizeof(m_cfmsEdit);
	m_cfmsEdit.dwMask = CFM_COLOR | CFM_BOLD | CFM_SIZE | CFM_FACE;
	m_cfmsEdit.dwEffects = CFM_BOLD;
	lstrcpy(m_cfmsEdit.szFaceName, _T("Segoe UI"));
	m_cfmsEdit.wWeight = FW_NORMAL;
	m_cfmsEdit.yHeight = 200;
	m_cfmsEdit.crTextColor = RGB(100, 64, 128);
	//m_cfmsEdit.crTextColor = globalData.clrWindowText;

	CRichEditCtrl	*rEditCh = reinterpret_cast<CRichEditCtrl*> (GetDlgItem(IDC_REDIT_ABOUT));		//
	ASSERT(rEditCh != NULL);
	rEditCh->SetBackgroundColor(FALSE, globalData.clrBtnLight);
	rEditCh->SetDefaultCharFormat(m_cfmsEdit);

	CStdioFile file;
	if(! file.Open(_T("ReadMe.txt"), CFile::modeRead | CFile::typeText))
	{
		BCGPMessageBox(_T("ReadMe.txt文件打开失败！"));
		PostMessage(WM_CLOSE);
		return TRUE;
	}

	char* oldLocale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");
	CString text;
	while (file.ReadString(text))
	{
		text.AppendChar(_T('\n'));
		rEditCh->SetSel(-1, -1);
		rEditCh->ReplaceSel(text);
	}
	setlocale(LC_CTYPE, oldLocale);
	free(oldLocale);

	file.Close();

	return TRUE;  // return TRUE unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBCGPDialog)
END_MESSAGE_MAP()

// CTargetToolApp message handlers


void CTargetToolApp::PreLoadState ()
{
}

CString CTargetToolApp::GetAppVersion()
{
	TCHAR szFullPath[MAX_PATH];
	DWORD dwVerInfoSize = 0;
	DWORD dwVerHwnd;
	VS_FIXEDFILEINFO* pFileInfo;
	GetModuleFileName(NULL, szFullPath, MAX_PATH);
	dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHwnd);
	if (dwVerInfoSize > 0)
	{
		HANDLE hMem;
		LPVOID lpvMem;
		unsigned int uInfoSize = 0;
		hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpvMem = GlobalLock(hMem);
		GetFileVersionInfo(szFullPath, dwVerHwnd, dwVerInfoSize, lpvMem);
		::VerQueryValue(lpvMem, _T("\\"), (void**)&pFileInfo, &uInfoSize);

		CString version;
		version.Format(_T("%d.%d.%d.%d"), HIWORD(pFileInfo->dwProductVersionMS), LOWORD(pFileInfo->dwProductVersionMS), 
			HIWORD(pFileInfo->dwProductVersionLS), LOWORD(pFileInfo->dwProductVersionLS));
		GlobalUnlock(hMem);
		GlobalFree(hMem);
		return version;
	}

	ASSERT(FALSE);

	return NULL;
}