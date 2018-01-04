#pragma once

#include <vector>
#include "PerformanceTransmit.h"

// CPerformanceDlg dialog

class CPerformanceDlg : public CSkinDialog
{
	DECLARE_DYNAMIC(CPerformanceDlg)

public:
	CPerformanceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPerformanceDlg();

	// Dialog Data
	enum { IDD = IDD_PERFORMANCE };

	void UpdateChartColorTheme(int nTheme);

	CPerformanceTransmit & getPerTransmit() { return m_perTransmit; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();

	afx_msg LRESULT OnUserPerSwitch(WPARAM wParam, LPARAM lParam);

private:
	void InitChartCount();
	void InitChartSpeed();

private:
	CBCGPChartCtrl	m_wndChartCount;
	CBCGPChartCtrl	m_wndChartSpeed;
	std::vector<CBCGPChartSeries*> m_arSeries;
	CPerformanceTransmit m_perTransmit;

	enum {
		TIMER_ID_CHART_COUNT = 1,
		TIMER_ELAPSE_CHART_COUNT = 1000, /* ?ms/times*/

		TIMER_ID_CHART_SPEED = 2,
		TIMER_ELAPSE_CHART_SPEED = 1000, /* ?ms/times*/
	};
};
