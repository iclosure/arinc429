#pragma once


// CRxLabelFilterDlg dialog

class CArinc429;

class CRxLabelFilterDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CRxLabelFilterDlg)

public:
	CRxLabelFilterDlg(CArinc429* arApiEx, CWnd* pParent = NULL);   // standard constructor
	virtual ~CRxLabelFilterDlg();

// Dialog Data
	enum { IDD = IDD_RX_LABEL_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedCheckLabels(UINT nID);
	afx_msg void OnBnClickedBnAllDislabled();
	afx_msg void OnBnClickedBnAllEnable();
	afx_msg void OnCbnSelchangeComboNameFormat();

private:
	CArinc429*		m_arApiEx;
	CBCGPButton		m_checkLabel[ARINC_LABEL_NUM];
};
