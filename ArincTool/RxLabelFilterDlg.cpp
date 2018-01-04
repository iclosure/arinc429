// RxLabelFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TargetTool.h"
#include "Arinc429.h"
#include "RxLabelFilterDlg.h"

// CRxLabelFilterDlg dialog

IMPLEMENT_DYNAMIC(CRxLabelFilterDlg, CBCGPDialog)

CRxLabelFilterDlg::CRxLabelFilterDlg(CArinc429* arApiEx, CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CRxLabelFilterDlg::IDD, pParent)
	, m_arApiEx(arApiEx)
{
	EnableVisualManagerStyle(TRUE, TRUE);

	ASSERT(arApiEx != NULL);
}

CRxLabelFilterDlg::~CRxLabelFilterDlg()
{
}

void CRxLabelFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	for (int i = 0; i < ARINC_LABEL_NUM; i++)
	{
		DDX_Control(pDX, IDC_CHECK_LABEL0 + i, m_checkLabel[i]);
	}
}

BOOL CRxLabelFilterDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	CString text;
	ARINC_INFO & arInfo = m_arApiEx->arInfo();

	// 设置标题
	text.Format(_T("接收端->标识符设置（通道%d [范围：0 ~ %d]）"), arInfo.RX.ChID, arInfo.RX.ChNum - 1);
	SetWindowText(text);

	((CComboBox*)GetDlgItem(IDC_COMBO_NAME_FORMAT))->SetCurSel(arInfo.TX.Info.DataFormat);	// 
	OnCbnSelchangeComboNameFormat();

	for (int i = 0; i < ARINC_LABEL_NUM; i++)
	{
		m_checkLabel[i].SetFont(&globalData.fontTooltip);
		m_checkLabel[i].SetTextColor(RGB(0, 255, 255));
		m_checkLabel[i].SetTextHotColor(RGB(0, 0, 255));
		m_checkLabel[i].SetImage (globalData.bIsOSAlphaBlendingSupport ? IDB_CHECK_NO32 : IDB_CHECK_NO);
		m_checkLabel[i].SetCheckedImage (globalData.bIsOSAlphaBlendingSupport ? IDB_CHECK_YES32 : IDB_CHECK_YES);
		m_checkLabel[i].SetCheck(arInfo.RX.Info.Label.Data[i].Action == ARU_FILTER_OFF ? TRUE : FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CRxLabelFilterDlg, CBCGPDialog)
	ON_COMMAND_RANGE(IDC_CHECK_LABEL0, IDC_CHECK_LABEL255, &ThisClass::OnBnClickedCheckLabels)
	ON_BN_CLICKED(IDC_BN_ALL_DISLABLED, &CRxLabelFilterDlg::OnBnClickedBnAllDislabled)
	ON_BN_CLICKED(IDC_BN_ALL_ENABLE, &CRxLabelFilterDlg::OnBnClickedBnAllEnable)
	ON_CBN_SELCHANGE(IDC_COMBO_NAME_FORMAT, &CRxLabelFilterDlg::OnCbnSelchangeComboNameFormat)
END_MESSAGE_MAP()


// CRxLabelFilterDlg message handlers

void CRxLabelFilterDlg::OnBnClickedCheckLabels(UINT nID)
{
	int label = nID - IDC_CHECK_LABEL0;
	BOOL bChecked = m_checkLabel[label].IsChecked();

	short status = m_arApiEx->labelFilter(label, bChecked ? ARU_FILTER_OFF : ARU_FILTER_ON);
	if (status == ARS_NORMAL) {
		ARINC_INFO & arInfo = m_arApiEx->arInfo();
		arInfo.RX.Info.Label.Data[label].Action = bChecked ? ARU_FILTER_OFF : ARU_FILTER_ON;
	} else {
		m_checkLabel[label].SetCheck(!bChecked);

		CString text, format;
		int index = ((CComboBox*)GetDlgItem(IDC_COMBO_NAME_FORMAT))->GetCurSel();	//
		switch (index)
		{
		case 0: format = _T("%03o"); break;
		case 1: format = _T("%03d"); break;
		case 2: format = _T("%02X"); break;
		default: ASSERT(FALSE); break;
		}
		text.Format(_T("标识号 [ ") + format + _T(" ] 禁用失败"), label);
		BCGPMessageBox(text);
	}
}

void CRxLabelFilterDlg::OnBnClickedBnAllDislabled()
{
	short status = m_arApiEx->labelFilter(ARU_ALL_LABELS, ARU_FILTER_ON);
	if (ARS_NORMAL == status) {
		ARINC_INFO & arInfo = m_arApiEx->arInfo();
		for (int i = 0; i < ARINC_LABEL_NUM; i++)
		{
			m_checkLabel[i].SetCheck(0);
			arInfo.RX.Info.Label.Data[i].Action = ARU_FILTER_ON;
		}
	} else {
		BCGPMessageBox(_T("标识号禁用失败！"));
	}
}

void CRxLabelFilterDlg::OnBnClickedBnAllEnable()
{
	short status = m_arApiEx->labelFilter(ARU_ALL_LABELS, ARU_FILTER_OFF);
	if (ARS_NORMAL == status) {
		ARINC_INFO & arInfo = m_arApiEx->arInfo();
		for (int i = 0; i < ARINC_LABEL_NUM; i++)
		{
			m_checkLabel[i].SetCheck(1);
			arInfo.RX.Info.Label.Data[i].Action = ARU_FILTER_OFF;
		}
	} else {
		BCGPMessageBox(_T("标识号启用失败！"));
	}
}

void CRxLabelFilterDlg::OnCbnSelchangeComboNameFormat()
{
	CString format, text;

	switch (((CComboBox*)GetDlgItem(IDC_COMBO_NAME_FORMAT))->GetCurSel())
	{
	case 0: format = _T("%03o"); break;
	case 1: format = _T("%03d"); break;
	case 2: format = _T("%02X"); break;
	default: ASSERT(FALSE);
	}

	for (int i = 0; i < ARINC_LABEL_NUM; i++)
	{
		text.Format(format, i);
		m_checkLabel[i].SetWindowText(text);
	}
}