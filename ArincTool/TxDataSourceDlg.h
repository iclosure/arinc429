#pragma once
#include "afxwin.h"

class CArinc429;

class CMyMaskEdit : public CBCGPMaskEdit
{
public:
	void GetEditText (CString& rstrString)
	{
		BOOL bSave = m_bPasteProcessing;

		m_bPasteProcessing = TRUE;
		GetWindowText (rstrString);
		m_bPasteProcessing = bSave;
	}
};

// CTxDataSourceDlg dialog

class CTxDataSourceDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CTxDataSourceDlg)

public:
	CTxDataSourceDlg(CArinc429* arApiEx, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTxDataSourceDlg();

	// Dialog Data
	enum { IDD = IDD_TX_DATA_SOURCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedRadioSplitSet();
	afx_msg void OnBnClickedRadioWordSet();
	afx_msg void OnCbnSelchangeComboSendStyle();
	afx_msg void OnEnChangeEditLabel();
	afx_msg void OnEnChangeEditSdi();
	afx_msg void OnEnChangeEditDataStart();
	afx_msg void OnEnChangeEditDataEnd();
	afx_msg void OnEnChangeEditSsm();
	afx_msg void OnEnChangeEditParity();
	afx_msg void OnEnChangeEditWordStart();
	afx_msg void OnEnChangeEditWordEnd();
	afx_msg void OnBnClickedOk();

private:
	CArinc429*		m_arApiEx;

	int				m_radioCurSel;

	CMyMaskEdit		m_editLabel;
	CMyMaskEdit		m_editSdi;
	CMyMaskEdit		m_editDataStart;
	CMyMaskEdit		m_editDataEnd;
	CMyMaskEdit		m_editSsm;
	CMyMaskEdit		m_editParity;

	CMyMaskEdit		m_editWordStart;
	CMyMaskEdit		m_editWordEnd;

	CMyMaskEdit		m_editCountStart;
	CMyMaskEdit		m_editCountEnd;

	CFont			m_fontEdit;
};
