// SetupDriverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TargetTool.h"
#include "SetupDriverDlg.h"


// CSetupDriverDlg dialog

IMPLEMENT_DYNCREATE(CSetupDriverDlg, CBCGPDialog)

CSetupDriverDlg::CSetupDriverDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CSetupDriverDlg::IDD, pParent)
{

}

CSetupDriverDlg::~CSetupDriverDlg()
{
}

void CSetupDriverDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetupDriverDlg, CBCGPDialog)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, &CSetupDriverDlg::OnBnClickedButtonView)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CSetupDriverDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_UNLOAD, &CSetupDriverDlg::OnBnClickedButtonUnload)
	ON_BN_CLICKED(IDC_BUTTON_FRESH, &CSetupDriverDlg::OnBnClickedButtonFresh)
END_MESSAGE_MAP()


// CSetupDriverDlg message handlers

void CSetupDriverDlg::OnBnClickedButtonView()
{
	// TODO: Add your control notification handler code here
}

void CSetupDriverDlg::OnBnClickedButtonLoad()
{
	// TODO: Add your control notification handler code here
}

void CSetupDriverDlg::OnBnClickedButtonUnload()
{
	// TODO: Add your control notification handler code here
}

void CSetupDriverDlg::OnBnClickedButtonFresh()
{
	// TODO: Add your control notification handler code here
}
