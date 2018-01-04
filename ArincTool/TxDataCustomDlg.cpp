// TxDataCustomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TargetTool.h"
#include "TxDataCustomDlg.h"
#include "Arinc429.h"

// CTxDataCustomDlg dialog

IMPLEMENT_DYNAMIC(CTxDataCustomDlg, CSkinDialog)

CTxDataCustomDlg::CTxDataCustomDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CTxDataCustomDlg::IDD, pParent)
	, m_filePath(_T(""))
	, m_strInput(_T(""))
{

}

CTxDataCustomDlg::~CTxDataCustomDlg()
{
}

void CTxDataCustomDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DATA, m_editData);
	DDX_Text(pDX, IDC_EDIT_FILE_PATH, m_filePath);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_buttonOpen);
	DDX_Text(pDX, IDC_EDIT_DATA, m_strInput);
}

BOOL CTxDataCustomDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	// 自动布局设置
	CBCGPStaticLayout* layout = (CBCGPStaticLayout*)GetLayout();
	if (layout != NULL)
	{
		layout->AddAnchor(IDC_EDIT_DATA, LAYOUT_MOVE_NONE, LAYOUT_SIZE_BOTH);
		layout->AddAnchor(IDC_CHECK_FORMAT_HEX, LAYOUT_MOVE_VERT, LAYOUT_SIZE_NONE);
		layout->AddAnchor(IDC_BUTTON_CLEAR, LAYOUT_MOVE_VERT, LAYOUT_SIZE_NONE);
		layout->AddAnchor(IDC_LABEL_FILE, LAYOUT_MOVE_VERT, LAYOUT_SIZE_NONE);
		layout->AddAnchor(IDC_EDIT_FILE_PATH, LAYOUT_MOVE_VERT, LAYOUT_SIZE_HORZ);
		layout->AddAnchor(IDC_BUTTON_OPEN, LAYOUT_MOVE_BOTH, LAYOUT_SIZE_NONE);
		layout->AddAnchor(IDC_BUTTON_OK, LAYOUT_MOVE_BOTH, LAYOUT_SIZE_NONE);
		layout->AddAnchor(IDC_BUTTON_CANCEL, LAYOUT_MOVE_BOTH, LAYOUT_SIZE_NONE);
	}

	return TRUE;
}


BEGIN_MESSAGE_MAP(CTxDataCustomDlg, CSkinDialog)
	ON_BN_CLICKED(IDC_CHECK_FORMAT_HEX, &CTxDataCustomDlg::OnBnClickedCheckFormatHex)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CTxDataCustomDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &ThisClass::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CTxDataCustomDlg::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CTxDataCustomDlg::OnBnClickedButtonCancel)
	ON_WM_CLOSE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CTxDataCustomDlg message handlers

void CTxDataCustomDlg::OnBnClickedCheckFormatHex()
{
	// TODO: Add your control notification handler code here
}

void CTxDataCustomDlg::OnBnClickedButtonClear()
{
	m_editData.SetSel(0, -1);
	m_editData.Clear();
}

void CTxDataCustomDlg::OnBnClickedButtonOpen()
{
	UpdateData(TRUE);

	const TCHAR szFilter[] = _T("Text File(*.txt)|*.txt|All File(*.*)|*.*|");

	CFileDialog fileDialog(TRUE, _T("*.txt"), m_filePath, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, szFilter, this);
	if (fileDialog.DoModal() == IDOK)
	{
		CString text;

		m_filePath = fileDialog.GetPathName();
		UpdateData(FALSE);

		m_editData.SetSel(0, -1);
		m_editData.Clear();
		
		// Open file m_filePath
		CFile file;
		if (!file.Open(m_filePath.GetBuffer(), CFile::modeRead))
		{
			ASSERT(FALSE);

			text.Format(_T("文件 %s 打开失败！"), m_filePath);
			BCGPMessageBox(text, MB_ICONERROR);
			return;
		}

		// 
		file.Read(text.GetBuffer(), (UINT)file.GetLength());

		// Close file m_filePath
		file.Close();

		m_strInput.Format(_T("%s"), text);

		UpdateData(FALSE);
	}
}

void CTxDataCustomDlg::OnClose()
{
	CBCGPDialog::OnClose();
}

void CTxDataCustomDlg::OnBnClickedButtonOk()
{
	UpdateData(TRUE);

	CString strRegex("([0-9A-F]{8}[ ]?)+");

	CAtlRegExp<CAtlRECharTraits> reRule;
	
	REParseError status = reRule.Parse((const ATL::CAtlRegExp<CAtlRECharTraits>::RECHAR*)strRegex.GetBuffer());

	if (REPARSE_ERROR_OK != status)
	{
		ASSERT(FALSE);
		return;
	}

	CAtlREMatchContext<CAtlRECharTraits> mcRule;
	TCHAR* wt = (TCHAR*)(LPCTSTR)m_strInput;

	if (!reRule.Match((const ATL::CAtlRegExp<CAtlRECharTraits>::RECHAR*)wt, &mcRule))
	{
		BCGPMessageBox(_T("您输入的数据格式不对！"));
	}
}

void CTxDataCustomDlg::OnBnClickedButtonCancel()
{
	SendMessage(WM_CLOSE);
}
