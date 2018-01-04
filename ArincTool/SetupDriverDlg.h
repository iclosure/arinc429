#pragma once


// CSetupDriverDlg dialog

class CSetupDriverDlg : public CBCGPDialog
{
	DECLARE_DYNCREATE(CSetupDriverDlg)

public:
	CSetupDriverDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupDriverDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_DRIVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnBnClickedButtonView();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonUnload();
	afx_msg void OnBnClickedButtonFresh();

	DECLARE_MESSAGE_MAP()

private:

};
