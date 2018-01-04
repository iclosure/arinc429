// TxDataSourceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TargetTool.h"
#include "Arinc429.h"
#include "TxDataSourceDlg.h"
#include <stringconvert.h>
#include <string>
#include <stdlib.h>

// CTxDataSourceDlg dialog

IMPLEMENT_DYNAMIC(CTxDataSourceDlg, CBCGPDialog)

CTxDataSourceDlg::CTxDataSourceDlg(CArinc429* arApiEx, CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CTxDataSourceDlg::IDD, pParent)
	, m_arApiEx(arApiEx)
	, m_radioCurSel(0)
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

CTxDataSourceDlg::~CTxDataSourceDlg()
{
}

void CTxDataSourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_SPLIT_SET, m_radioCurSel);
	DDX_Control(pDX, IDC_EDIT_LABEL, m_editLabel);
	DDX_Control(pDX, IDC_EDIT_SDI, m_editSdi);
	DDX_Control(pDX, IDC_EDIT_DATA_START, m_editDataStart);
	DDX_Control(pDX, IDC_EDIT_DATA_END, m_editDataEnd);
	DDX_Control(pDX, IDC_EDIT_SSM, m_editSsm);
	DDX_Control(pDX, IDC_EDIT_PARITY, m_editParity);
	DDX_Control(pDX, IDC_EDIT_WORD_START, m_editWordStart);
	DDX_Control(pDX, IDC_EDIT_WORD_END, m_editWordEnd);
	DDX_Control(pDX, IDC_EDIT_COUNT_START, m_editCountStart);
	DDX_Control(pDX, IDC_EDIT_COUNT_END, m_editCountEnd);
}

BOOL CTxDataSourceDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// 初始化变量
	m_fontEdit.CreateFont(
		16, 8, 0, 0,               /*nOrientation*/
		FW_NORMAL,                 /*nWeight*/
		FALSE,                     /*bItalic*/
		FALSE,                     /*bUnderline*/
		FALSE,                     /*cStrikeOut*/
		DEFAULT_CHARSET,           /*nCharSet*/
		OUT_CHARACTER_PRECIS,      /*nOutPrecision*/
		CLIP_DEFAULT_PRECIS,       /*nClipPrecision*/
		ANTIALIASED_QUALITY,       /*nQuality*/
		FIXED_PITCH | FF_SWISS,    /*nPitchAndFamily*/
		_T("Segoe UI"));           /*lpszFacename*/
	
	m_editLabel.SetFont(&m_fontEdit);
	m_editSdi.SetFont(&m_fontEdit);
	m_editDataStart.SetFont(&m_fontEdit);
	m_editDataEnd.SetFont(&m_fontEdit);
	m_editSsm.SetFont(&m_fontEdit);
	m_editParity.SetFont(&m_fontEdit);
	m_editWordStart.SetFont(&m_fontEdit);
	m_editWordEnd.SetFont(&m_fontEdit);
	m_editCountStart.SetFont(&m_fontEdit);
	m_editCountEnd.SetFont(&m_fontEdit);

	//! LABEL
	m_editLabel.EnableMask(_T("AA"), _T("__"), _T('0'), _T("12345678990ABCDEFabcdef"));
	m_editLabel.SetWindowText(_T("00"));

	//! SDI
	m_editSdi.EnableMask(_T("A"), _T("_"), _T('0'), _T("0123"));
	m_editSdi.SetWindowText(_T("0"));

	//! DATA - Start
	m_editDataStart.EnableMask(_T("AAAAA"), _T("_____"), _T('0'), _T("12345678990ABCDEFabcdef"));
	m_editDataStart.SetWindowText(_T("00000"));

	//! DATA - End
	m_editDataEnd.EnableMask(_T("AAAAA"), _T("_____"), _T('0'), _T("12345678990ABCDEFabcdef"));
	m_editDataEnd.SetWindowText(_T("00000"));

	//! SSM
	m_editSsm.EnableMask(_T("A"), _T("_"), _T('0'), _T("0123"));
	m_editSsm.SetWindowText(_T("0"));

	//! PARITY
	m_editParity.EnableMask(_T("A"), _T("_"), _T('0'), _T("01"));
	m_editParity.SetWindowText(_T("0"));

	//! WORD - Start
	m_editWordStart.EnableMask(_T("AAAAAAAA"), _T("________"), _T('0'), _T("12345678990ABCDEFabcdef"));
	m_editWordStart.SetWindowText(_T("00000000"));

	//! WORD - End
	m_editWordEnd.EnableMask(_T("AAAAAAAA"), _T("________"), _T('0'), _T("12345678990ABCDEFabcdef"));
	m_editWordEnd.SetWindowText(_T("00000000"));

	//! Count - Start
	m_editCountStart.EnableMask(_T("AAAAAAAA"), _T("________"), _T('0'), _T("12345678990ABCDEFabcdef"));
	m_editCountStart.SetWindowText(_T("00000000"));

	//! Count - End
	m_editCountEnd.EnableMask(_T("AAAAAAAA"), _T("________"), _T('0'), _T("12345678990ABCDEFabcdef"));
	m_editCountEnd.SetWindowText(_T("00000000"));

	//! 
	ARINC_INFO & arInfo = m_arApiEx->arInfo();

	//! 
	m_radioCurSel = arInfo.TX.Info.EnCounts;
	UpdateData(FALSE);
	if (m_radioCurSel) {
		OnBnClickedRadioWordSet();
	} else {
		OnBnClickedRadioSplitSet();
	}

	((CComboBox*)GetDlgItem(IDC_COMBO_DATA_SEND_STYLE))->SetCurSel(arInfo.TX.Info.EnPeriod);
	OnCbnSelchangeComboSendStyle();

	CString text;
	DWORD data0 = arInfo.TX.Info.Data[0];

	//! label
	text.Format(_T("%02X"), (data0 & AR_WORD_LABEL_MASK) >> AR_WORD_LABEL_OFFSET);
	m_editLabel.SetWindowText(text);

	//! SDI
	text.Format(_T("%1X"), (data0 & AR_WORD_SDI_MASK) >> AR_WORD_SDI_OFFSET);
	m_editSdi.SetWindowText(text);

	//! DATA - Start
	text.Format(_T("%05X"), (data0 & AR_WORD_DATA_MASK) >> AR_WORD_DATA_OFFSET);
	m_editDataStart.SetWindowText(text);

	//! DATA - End
	if (arInfo.TX.Info.EnPeriod) {
		text.Format(_T("%05X"), (arInfo.TX.Info.Data[1] 
		& AR_WORD_DATA_MASK) >> AR_WORD_DATA_OFFSET);
		m_editDataEnd.SetWindowText(text);
	} else {
		((CButton*)GetDlgItem(IDC_LABEL_DATA_END))->ShowWindow(FALSE);
		m_editDataEnd.ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_LABEL_DATA_END_RANGE))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_LABEL_DATA_START))->SetWindowText(_T("发送值(HEX)："));
	}

	//! SSM
	text.Format(_T("%1X"), (data0 & AR_WORD_SSM_MASK) >> AR_WORD_SSM_OFFSET);
	((CBCGPMaskEdit*)GetDlgItem(IDC_EDIT_SSM))->SetWindowText(text);

	//! P
	text.Format(_T("%1X"), (data0 & AR_WORD_PARITY_MASK) >> AR_WORD_PARITY_OFFSET);
	((CBCGPMaskEdit*)GetDlgItem(IDC_EDIT_PARITY))->SetWindowText(text);

	//! WORD - Start
	text.Format(_T("%08X"), data0);
	m_editWordStart.SetWindowText(text);

	//! Count - Start
	((CEdit*)GetDlgItem(IDC_EDIT_COUNT_START))->SetWindowText(text);

	//! WORD - End
	if (arInfo.TX.Info.EnPeriod) {
		text.Format(_T("%08X"), arInfo.TX.Info.Data[1]);
		m_editWordEnd.SetWindowText(text);
	} else {
		((CButton*)GetDlgItem(IDC_LABEL_WORD_END))->ShowWindow(FALSE);
		m_editWordEnd.ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_LABEL_WORD_START))->SetWindowText(_T("发送值(HEX)："));
	}

	//! Count - End
	text.Format(_T("%08X"), arInfo.TX.Info.Data[1]);
	((CEdit*)GetDlgItem(IDC_EDIT_COUNT_END))->SetWindowText(text);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CTxDataSourceDlg, CBCGPDialog)
	ON_BN_CLICKED(IDC_RADIO_SPLIT_SET, &CTxDataSourceDlg::OnBnClickedRadioSplitSet)
	ON_BN_CLICKED(IDC_RADIO_WORD_SET, &CTxDataSourceDlg::OnBnClickedRadioWordSet)
	ON_CBN_SELCHANGE(IDC_COMBO_DATA_SEND_STYLE, &ThisClass::OnCbnSelchangeComboSendStyle)
	ON_EN_CHANGE(IDC_EDIT_LABEL, &ThisClass::OnEnChangeEditLabel)
	ON_EN_CHANGE(IDC_EDIT_SDI, &ThisClass::OnEnChangeEditSdi)
	ON_EN_CHANGE(IDC_EDIT_DATA_START, &ThisClass::OnEnChangeEditDataStart)
	ON_EN_CHANGE(IDC_EDIT_DATA_END, &ThisClass::OnEnChangeEditDataEnd)
	ON_EN_CHANGE(IDC_EDIT_SSM, &ThisClass::OnEnChangeEditSsm)
	ON_EN_CHANGE(IDC_EDIT_PARITY, &ThisClass::OnEnChangeEditParity)
	ON_EN_CHANGE(IDC_EDIT_WORD_START, &ThisClass::OnEnChangeEditWordStart)
	ON_EN_CHANGE(IDC_EDIT_WORD_END, &ThisClass::OnEnChangeEditWordEnd)
	ON_BN_CLICKED(IDOK, &ThisClass::OnBnClickedOk)
END_MESSAGE_MAP()


// CTxDataSourceDlg message handlers

void CTxDataSourceDlg::OnBnClickedRadioSplitSet()
{
	((CEdit*)GetDlgItem(IDC_EDIT_LABEL))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_SDI))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_COMBO_DATA_SEND_STYLE))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_DATA_START))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_DATA_END))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_SSM))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_PARITY))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_WORD_START))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_WORD_END))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_COUNT_START))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_COUNT_END))->EnableWindow(FALSE);

}

void CTxDataSourceDlg::OnBnClickedRadioWordSet()
{
	((CEdit*)GetDlgItem(IDC_EDIT_LABEL))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_SDI))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_COMBO_DATA_SEND_STYLE))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_DATA_START))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_DATA_END))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_SSM))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_PARITY))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_WORD_START))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_WORD_END))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_COUNT_START))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_COUNT_END))->EnableWindow(TRUE);
}

void CTxDataSourceDlg::OnCbnSelchangeComboSendStyle()
{
	//! 
	if (((CComboBox*)GetDlgItem(IDC_COMBO_DATA_SEND_STYLE))->GetCurSel() == 1) { // 周期
		((CButton*)GetDlgItem(IDC_LABEL_DATA_START))->SetWindowText(_T("起始值(HEX)："));
		((CButton*)GetDlgItem(IDC_LABEL_DATA_END))->ShowWindow(TRUE);
		m_editDataEnd.ShowWindow(TRUE);
		((CButton*)GetDlgItem(IDC_LABEL_DATA_END_RANGE))->ShowWindow(TRUE);
		((CButton*)GetDlgItem(IDC_LABEL_WORD_START))->SetWindowText(_T("起始值(HEX)："));
		((CButton*)GetDlgItem(IDC_LABEL_WORD_END))->ShowWindow(TRUE);
		m_editWordEnd.ShowWindow(TRUE);
	} else { // 单次
		((CButton*)GetDlgItem(IDC_LABEL_DATA_START))->SetWindowText(_T("发送值(HEX)："));
		((CButton*)GetDlgItem(IDC_LABEL_DATA_END))->ShowWindow(FALSE);
		m_editDataEnd.ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_LABEL_DATA_END_RANGE))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT_WORD_END))->SetWindowText(_T("发送值(HEX)："));
		((CButton*)GetDlgItem(IDC_LABEL_WORD_END))->ShowWindow(FALSE);
		m_editWordEnd.ShowWindow(FALSE);
	}
}

void CTxDataSourceDlg::OnEnChangeEditLabel()
{
	CString text0, text;
	ULONG data0, data;

	m_editLabel.GetEditText(text);
	data = strtoul(CString2Utf8(text), NULL, 16);

	//! WORD - Start
	m_editWordStart.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%08X"), (data0 & (~AR_WORD_LABEL_MASK))
		| ((data << AR_WORD_LABEL_OFFSET) & AR_WORD_LABEL_MASK));
	if (text != text0) {
		m_editWordStart.SetWindowText(text);
	}

	//! WORD - End
	m_editWordEnd.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%08X"), (data0 & (~AR_WORD_LABEL_MASK)) 
		| ((data << AR_WORD_LABEL_OFFSET) & AR_WORD_LABEL_MASK));
	if (text != text0) {
		m_editWordEnd.SetWindowText(text);
	}
}

void CTxDataSourceDlg::OnEnChangeEditSdi()
{
	CString text0, text;
	ULONG data0, data;

	m_editSdi.GetEditText(text);
	data = strtoul(CString2Utf8(text), NULL, 16);

	//! WORD - Start
	m_editWordStart.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%08X"), (data0 & (~AR_WORD_SDI_MASK))
		| ((data << AR_WORD_SDI_OFFSET) & AR_WORD_SDI_MASK));
	if (text != text0) {
		m_editWordStart.SetWindowText(text);
	}

	//! WORD - End
	m_editWordEnd.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%08X"), (data0 & (~AR_WORD_SDI_MASK)) 
		| ((data << AR_WORD_SDI_OFFSET) & AR_WORD_SDI_MASK));
	if (text != text0) {
		m_editWordEnd.SetWindowText(text);
	}
}

void CTxDataSourceDlg::OnEnChangeEditDataStart()
{
	CString text0, text;
	ULONG data0, data;

	m_editDataStart.GetEditText(text);
	data = strtoul(CString2Utf8(text), NULL, 16);

	//! WORD - Start
	m_editWordStart.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%08X"), (data0 & (~AR_WORD_DATA_MASK))
		| ((data << AR_WORD_DATA_OFFSET) & AR_WORD_DATA_MASK));
	if (text != text0) {
		m_editWordStart.SetWindowText(text);
	}
}

void CTxDataSourceDlg::OnEnChangeEditDataEnd()
{
	CString text0, text;
	ULONG data0, data;

	m_editDataEnd.GetEditText(text);
	data = strtoul(CString2Utf8(text), NULL, 16);

	//! WORD - End
	m_editWordEnd.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%08X"), (data0 & (~AR_WORD_DATA_MASK)) 
		| ((data << AR_WORD_DATA_OFFSET) & AR_WORD_DATA_MASK));
	if (text != text0) {
		m_editWordEnd.SetWindowText(text);
	}
}

void CTxDataSourceDlg::OnEnChangeEditSsm()
{
	CString text0, text;
	ULONG data0, data;

	m_editSsm.GetEditText(text);
	data = strtoul(CString2Utf8(text), NULL, 16);

	//! WORD - Start
	m_editWordStart.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%08X"), (data0 & (~AR_WORD_SSM_MASK))
		| ((data << AR_WORD_SSM_OFFSET) & AR_WORD_SSM_MASK));
	if (text != text0) {
		m_editWordStart.SetWindowText(text);
	}

	//! WORD - End
	m_editWordEnd.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%08X"), (data0 & (~AR_WORD_SSM_MASK)) 
		| ((data << AR_WORD_SSM_OFFSET) & AR_WORD_SSM_MASK));
	if (text != text0) {
		m_editWordEnd.SetWindowText(text);
	}
}

void CTxDataSourceDlg::OnEnChangeEditParity()
{
	CString text0, text;
	ULONG data0, data;

	m_editParity.GetEditText(text);
	data = strtoul(CString2Utf8(text), NULL, 16);

	//! WORD - Start
	m_editWordStart.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%08X"), (data0 & (~AR_WORD_PARITY_MASK))
		| ((data << AR_WORD_PARITY_OFFSET) & AR_WORD_PARITY_MASK));
	if (text != text0) {
		m_editWordStart.SetWindowText(text);
	}

	//! WORD - End
	m_editWordEnd.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%08X"), (data0 & (~AR_WORD_PARITY_MASK)) 
		| ((data << AR_WORD_PARITY_OFFSET) & AR_WORD_PARITY_MASK));
	if (text != text0) {
		m_editWordEnd.SetWindowText(text);
	}
}

void CTxDataSourceDlg::OnEnChangeEditWordStart()
{
	CString text0, text;
	ULONG data0, data;

	m_editWordStart.GetEditText(text);
	data = strtoul(CString2Utf8(text), NULL, 16);

	//! label
	m_editLabel.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%02X"), (data & AR_WORD_LABEL_MASK) >> AR_WORD_LABEL_OFFSET);
	if (text != text0) {
		m_editLabel.SetWindowText(text);
	}

	//! SDI
	m_editSdi.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%1X"), (data & AR_WORD_SDI_MASK) >> AR_WORD_SDI_OFFSET);
	if (text != text0) {
		m_editSdi.SetWindowText(text);
	}

	//! DATA - Start
	m_editDataStart.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%05X"), (data & AR_WORD_DATA_MASK) >> AR_WORD_DATA_OFFSET);
	if (text != text0) {
		m_editDataStart.SetWindowText(text);
	}

	//! SSM
	m_editSsm.GetWindowText(text0);
	if (text0.IsEmpty()) { return; }
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%1X"), (data & AR_WORD_SSM_MASK) >> AR_WORD_SSM_OFFSET);
	if (text != text0) {
		m_editSsm.SetWindowText(text);
	}

	//! P
	m_editParity.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%1X"), (data & AR_WORD_PARITY_MASK) >> AR_WORD_PARITY_OFFSET);
	if (text != text0) {
		m_editParity.SetWindowText(text);
	}

	//! WORD - End
	m_editWordEnd.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%08X"), (data0 & AR_WORD_DATA_MASK) | (data & (~AR_WORD_DATA_MASK)));
	if (text != text0) {
		m_editWordEnd.SetWindowText(text);
	}
}

void CTxDataSourceDlg::OnEnChangeEditWordEnd()
{
	CString text0, text;
	ULONG data0, data;

	m_editWordEnd.GetEditText(text);
	data = strtoul(CString2Utf8(text), NULL, 16);

	//! label
	m_editLabel.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%02X"), (data & AR_WORD_LABEL_MASK) >> AR_WORD_LABEL_OFFSET);
	if (text != text0) {
		m_editLabel.SetWindowText(text);
	}

	//! SDI
	m_editSdi.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%1X"), (data & AR_WORD_SDI_MASK) >> AR_WORD_SDI_OFFSET);
	if (text != text0) {
		m_editSdi.SetWindowText(text);
	}

	//! DATA - End
	m_editDataEnd.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%05X"), (data & AR_WORD_DATA_MASK) >> AR_WORD_DATA_OFFSET);
	if (text != text0) {
		m_editDataEnd.SetWindowText(text);
	}

	//! SSM
	m_editSsm.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%1X"), (data & AR_WORD_SSM_MASK) >> AR_WORD_SSM_OFFSET);
	if (text != text0) {
		m_editSsm.SetWindowText(text);
	}

	//! P
	m_editParity.GetWindowText(text0);
	if (text0.IsEmpty()) { return; }
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%1X"), (data & AR_WORD_PARITY_MASK) >> AR_WORD_PARITY_OFFSET);
	if (text != text0) {
		m_editParity.SetWindowText(text);
	}

	//! WORD - Start
	m_editWordStart.GetWindowText(text0);
	data0 = strtoul(CString2Utf8(text0), NULL, 16);
	text.Format(_T("%08X"), (data0 & AR_WORD_DATA_MASK) | (data & (~AR_WORD_DATA_MASK)));
	if (text != text0) {
		m_editWordStart.SetWindowText(text);
	}
}

void CTxDataSourceDlg::OnBnClickedOk()
{
	ARINC_INFO & arInfo = m_arApiEx->arInfo();

	//! 
	ULONG data0, data = 0;
	CString text;

	text.GetBuffer();

	//! 
	UpdateData();

	//! 
	if (m_radioCurSel == 0) {

		//! LABEL
		m_editLabel.GetWindowText(text);
		data0 = strtoul(CString2Utf8(text), NULL, 16);
		data |= (data0 << AR_WORD_LABEL_OFFSET) & AR_WORD_LABEL_MASK;

		//! SDI
		m_editSdi.GetWindowText(text);
		data0 = strtoul(CString2Utf8(text), NULL, 16);
		data |= (data0 << AR_WORD_SDI_OFFSET) & AR_WORD_SDI_MASK;

		//! SSM
		m_editSsm.GetWindowText(text);
		data0 = strtoul(CString2Utf8(text), NULL, 16);
		data |= (data0 << AR_WORD_SSM_OFFSET) & AR_WORD_SSM_MASK;

		//! P
		m_editParity.GetWindowText(text);
		data0 = strtoul(CString2Utf8(text), NULL, 16);
		data |= (data0 << AR_WORD_PARITY_OFFSET) & AR_WORD_PARITY_MASK;

		//! DATA - Start
		m_editDataStart.GetWindowText(text);
		data0 = strtoul(CString2Utf8(text), NULL, 16);
		arInfo.TX.Info.Data[0] = data | ((data0 << AR_WORD_DATA_OFFSET) & AR_WORD_DATA_MASK);

		//! DATA - End
		m_editDataEnd.GetWindowText(text);
		data0 = strtoul(CString2Utf8(text), NULL, 16);
		arInfo.TX.Info.Data[1] = data | ((data0 << AR_WORD_DATA_OFFSET) & AR_WORD_DATA_MASK);

		//! 
		arInfo.TX.Info.EnPeriod = ((CComboBox*)GetDlgItem(IDC_COMBO_DATA_SEND_STYLE))->GetCurSel();
		arInfo.TX.Info.EnCounts = FALSE;

	} else {

		//! Count - Start
		((CEdit*)GetDlgItem(IDC_EDIT_COUNT_START))->GetWindowText(text);
		arInfo.TX.Info.Data[0] = strtoul(CString2Utf8(text), NULL, 16);

		//! Count - End
		((CEdit*)GetDlgItem(IDC_EDIT_COUNT_END))->GetWindowText(text);
		arInfo.TX.Info.Data[1] = strtoul(CString2Utf8(text), NULL, 16);

		//! 
		arInfo.TX.Info.EnPeriod = TRUE;
		arInfo.TX.Info.EnCounts = TRUE;
	}

	SendMessage(WM_CLOSE);
}
