#pragma once
#include "afxwin.h"
#include "BCGPButton.h"


// CTxDataCustomDlg dialog

class CArinc429;

class CTxDataCustomDlg : public CSkinDialog
{
	DECLARE_DYNAMIC(CTxDataCustomDlg)

public:
	CTxDataCustomDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTxDataCustomDlg();

	void SetArApiEx(CArinc429* arApiEx) { m_arApiEx = arApiEx; }

// Dialog Data
	enum { IDD = IDD_TX_DATA_CUSTOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedCheckFormatHex();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnClose();

private:
	CArinc429*		m_arApiEx;
	CString			m_filePath;
	CString			m_strInput;
	CBCGPEdit		m_editData;
	CBCGPButton		m_buttonOpen;
};
