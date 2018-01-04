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
	CBCGPStaticLayout*	m_layout;			// �Զ����ֹ�����
	CRect				m_rcWndCurSel;		// ���е�ǰ��Ի����λ�úͳߴ�
	CSkinDialog*		m_pCurSelDlg;		// ���е�ǰ��Ի���
	CRect				m_rcWndOutput;		// ��Ϣ����Ի����λ�úͳߴ�
	BOOL				m_bEnableOutput;	// ������ʾ��Ϣ����Ի���
	BOOL				m_bEnFlush;			// ����ˢ�¿�����
	BOOL				m_bEnFlushBn;		// �Ƿ�ʹ������ˢ�¿���
	BOOL				m_bEnQuitBn;		// �Ƿ�ʹ�ܷ��ؿ���
	CMainFrame*			m_wndFrame;			// CMainFrame object
};

#ifndef _DEBUG  // debug version in TargetToolView.cpp
inline CTargetToolDoc* CTargetToolView::GetDocument() const
   { return reinterpret_cast<CTargetToolDoc*>(m_pDocument); }
#endif
