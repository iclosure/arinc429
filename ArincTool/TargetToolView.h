// TargetToolView.h : interface of the CTargetToolView class
//


#pragma once

class CMainFrame;
class CSkinDialog;

class CTargetToolView : public CView
{
protected: // create from serialization only
	CTargetToolView();
	DECLARE_DYNCREATE(CTargetToolView)

// Attributes
public:
	CTargetToolDoc* GetDocument() const;

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL DestroyWindow();

// Implementation
public:
	virtual ~CTargetToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	BOOL IsCurSelDlgValid() const { return m_pCurSelDlg != NULL; }
	CSkinDialog* const GetCurSelDlg() const
	{
		ASSERT(m_pCurSelDlg != NULL);
		ASSERT_VALID(m_pCurSelDlg);
		return m_pCurSelDlg;
	}

// Generated message map functions
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnComboPerBoardId();
	afx_msg void OnUpdateComboPerBoardId(CCmdUI* pCmdUI);
	afx_msg void OnComboPerLoopback();
	afx_msg void OnUpdateComboPerLoopback(CCmdUI* pCmdUI);
	afx_msg void OnComboPerBaudRate();
	afx_msg void OnUpdateComboPerBaudRate(CCmdUI* pCmdUI);
	afx_msg void OnComboPerParity();
	afx_msg void OnUpdateComboPerParity(CCmdUI* pCmdUI);
	afx_msg void OnButtonPerStart();
	afx_msg void OnUpdateButtonPerStart(CCmdUI* pCmdUI);
	afx_msg void OnButtonPerStop();
	afx_msg void OnUpdateButtonPerStop(CCmdUI* pCmdUI);

public:
	afx_msg LRESULT OnChangeRibbonCategory(WPARAM wParam, LPARAM lParam);
	
private:
	template<typename dialog> void OnSwitchDialog();

private:
	BOOL				m_isPerRunning;		// 
	CBCGPStaticLayout*	m_layout;			// 自动布局管理器
	CRect				m_rcWndCurSel;		// 视中当前活动对话框的位置和尺寸
	CSkinDialog*		m_pCurSelDlg;		// 视中当前活动对话框
	CRect				m_rcWndOutput;		// 信息输出对话框的位置和尺寸
	BOOL				m_bEnableOutput;	// 允许显示信息输出对话框？
	BOOL				m_bEnFlush;			// 数据刷新开关量
	BOOL				m_bEnFlushBn;		// 是否使能数据刷新开关
	BOOL				m_bEnQuitBn;		// 是否使能返回开关
	CMainFrame*			m_wndFrame;			// CMainFrame object
};

#ifndef _DEBUG  // debug version in TargetToolView.cpp
inline CTargetToolDoc* CTargetToolView::GetDocument() const
   { return reinterpret_cast<CTargetToolDoc*>(m_pDocument); }
#endif
