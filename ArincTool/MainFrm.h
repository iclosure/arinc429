// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "Arinc429.h"
#include <vector>

class CTargetToolView;
class CChTxOutput;
class CChRxOutput;
class CTxDataCustomDlg;

class CMainFrame : public CBCGPFrameWnd
{
	friend class CTargetToolView;

	DECLARE_DYNCREATE(CMainFrame)
	
protected: // create from serialization only
	CMainFrame();
	virtual ~CMainFrame();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnChangeRibbonCategory(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAppColorTheme(UINT id);
	afx_msg void OnUpdateAppColorTheme(CCmdUI* pCmdUI);
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	// RibbonBar
	afx_msg void OnButtonDataSheet();
	afx_msg void OnUpdateButtonDataSheet(CCmdUI* pCmdUI);
	afx_msg void OnComboBoardId();
	afx_msg void OnComboTxId();
	afx_msg void OnUpdateComboTxId(CCmdUI* pCmdUI);
	afx_msg void OnComboTxBaud();
	afx_msg void OnUpdateComboTxBaud(CCmdUI* pCmdUI);
	afx_msg void OnComboTxParity();
	afx_msg void OnUpdateComboTxParity(CCmdUI* pCmdUI);
	afx_msg void OnComboTxPeriod();
	afx_msg void OnUpdateComboTxPeriod(CCmdUI* pCmdUI);
	afx_msg void OnComboTxTestWay();
	afx_msg void OnUpdateComboTxTestWay(CCmdUI* pCmdUI);
	afx_msg void OnButtonTxData();
	afx_msg void OnUpdateButtonTxData(CCmdUI* pCmdUI);
	afx_msg void OnButtonTxDataEdit();
	afx_msg void OnUpdateButtonTxDataEdit(CCmdUI* PCmdUI);
	afx_msg void OnButtonSendData();
	afx_msg void OnUpdateButtonSendData(CCmdUI* pCmdUI);
	afx_msg void OnButtonTxNew();
	afx_msg void OnUpdateButtonTxNew(CCmdUI* pCmdUI);
	afx_msg void OnComboRxId();
	afx_msg void OnUpdateComboRxId(CCmdUI* pCmdUI);
	afx_msg void OnComboRxBaud();
	afx_msg void OnUpdateComboRxBaud(CCmdUI* pCmdUI);
	afx_msg void OnComboRxParity();
	afx_msg void OnUpdateComboRxParity(CCmdUI* pCmdUI);
	afx_msg void OnButtonFilter();
	afx_msg void OnUpdateButtonFilter(CCmdUI* pCmdUI);
	afx_msg void OnCheckDecoder();
	afx_msg void OnUpdateCheckDecoder(CCmdUI* pCmdUI);
	afx_msg void OnCheckSdiBit9();
	afx_msg void OnUpdateCheckSdiBit9(CCmdUI* pCmdUI);
	afx_msg void OnCheckSdiBit10();
	afx_msg void OnUpdateCheckSdiBit10(CCmdUI* pCmdUI);
	afx_msg void OnButtonSaveAs();
	afx_msg void OnUpdateButtonSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnButtonRxNew();
	afx_msg void OnUpdateButtonRxNew(CCmdUI* pCmdUI);
	afx_msg void OnButtonArTurn();
	afx_msg void OnUpdateButtonArTurn(CCmdUI* pCmdUI);
	afx_msg void OnButtonArPlay();
	afx_msg void OnUpdateButtonArPlay(CCmdUI* pCmdUI);
	afx_msg void OnButtonArSync();
	afx_msg void OnUpdateButtonArSync(CCmdUI* pCmdUI);
	afx_msg void OnCheckDislpay();
	afx_msg void OnUpdateCheckDislpay(CCmdUI* pCmdUI);
	// StatusBar
	afx_msg void OnButtonResetCount();

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnUserDeleteSubDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserDeleteArApi(WPARAM wParam, LPARAM lParam);

protected:  // control bar embedded members
	void ShowOptions (int nPage);

	BOOL CreateRibbonBar ();
	BOOL CreateStatusBar ();

	BOOL SyncConfigToScreen();

public:
	void SetStatusBarPaneText(UINT nIDC, LPCTSTR text, BOOL reDraw = TRUE);

	void SetView(CTargetToolView* value) { ASSERT(value != NULL); m_wndView = value; }
	CTargetToolView* GetView() { ASSERT(m_wndView != NULL); return m_wndView; }

protected:  // control bar embedded members
	CBCGPRibbonStatusBar	m_wndStatusBar;
	CBCGPRibbonBar			m_wndRibbonBar;
	CBCGPMenuBar			m_wndMenuBar;
	CBCGPToolBar			m_wndToolBar;

	UINT					m_nAppLook;

private:
	CTargetToolView*	m_wndView;
	CTxDataCustomDlg*	m_txDataEditDlg;

	CChTxOutput*  m_arrayChTxOutput;
	CChRxOutput*  m_arrayChRxOutput;
	CArinc429*    m_arrayArApiEx;

	int			m_currChTx;
	int			m_currChRx;
	int			m_oldBoardId;

	//
	BOOL		m_bArTurnOn;
	BOOL		m_bArRun;

	BOOL		m_bDecoder;
	BOOL		m_bSdiBit9;
	BOOL		m_bSdiBit10;
	BOOL		m_bFilter;
	BOOL		m_bDisplay;

	enum { 
		TIMER_ID_SYS_TIME = 1,
		TIMER_ELAPSE_SYS_TIME = 1000, /*500ms/times*/

		TIMER_ID_UPDATE_COUNT = 2,
		TIMER_ELAPSE_UPDATE_COUNT = 500  // 数据传输累计计数值显示
	};	// 系统时间显示定时器信息
};
