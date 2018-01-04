// PerformanceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TargetTool.h"
#include "PerformanceDlg.h"
#include "UserWinMsg.h"
#include <stdlib.h>
#include "arapi.h"

typedef long long           int64_t;
typedef unsigned long long  uint64_t;

/// 时间转换
static uint64_t file_time_2_utc(const FILETIME* ftime)
{
	LARGE_INTEGER li;

	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;
	return li.QuadPart;
}

/// 获得CPU的核数
static int get_processor_number()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return (int)info.dwNumberOfProcessors;
}

int get_cpu_usage(int pid)
{
	//cpu数量
	static int processor_count_ = -1;
	//上一次的时间
	static int64_t last_time_ = 0;
	static int64_t last_system_time_ = 0;

	FILETIME now;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME kernel_time;
	FILETIME user_time;
	int64_t system_time;
	int64_t time;
	int64_t system_time_delta;
	int64_t time_delta;

	int cpu = -1;

	if (processor_count_ == -1)
	{
		processor_count_ = get_processor_number();
	}

	GetSystemTimeAsFileTime(&now);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
	{
		CloseHandle(hProcess);
		hProcess = NULL;
		return -1;
	}
	CloseHandle(hProcess);
	hProcess = NULL;

	system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time))
		/ processor_count_;
	time = file_time_2_utc(&now);

	if ((last_system_time_ == 0) || (last_time_ == 0))
	{
		last_system_time_ = system_time;
		last_time_ = time;
		return -1;
	}

	system_time_delta = system_time - last_system_time_;
	time_delta = time - last_time_;

	if (time_delta == 0)
		return -1;

	cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
	last_system_time_ = system_time;
	last_time_ = time;
	return cpu;
}
/*
int main()
{
	while (1)
	{
		int cpu;
		// 参数为进程id
		cpu = get_cpu_usage(5160);
		printf("CPU使用率: %d%%\n", cpu);

		Sleep(1000);
	}
	return 0;
}
*/
inline double Rand(double dblStart, double dblFinish)
{
	double minVal = min(dblStart, dblFinish);
	double maxVal = max(dblStart, dblFinish);

	return (maxVal - minVal) * (double)rand() / (RAND_MAX + 1) + minVal;
}

// CPerformanceDlg dialog

IMPLEMENT_DYNAMIC(CPerformanceDlg, CSkinDialog)

CPerformanceDlg::CPerformanceDlg(CWnd* pParent /*=NULL*/)
: CSkinDialog(CPerformanceDlg::IDD, pParent)
{
	SetBackgroundColor(RGB(0xFF, 0xFA, 0xCD));
}

CPerformanceDlg::~CPerformanceDlg()
{
	
}

void CPerformanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHART_COUNT, m_wndChartCount);
	DDX_Control(pDX, IDC_CHART_SPEED, m_wndChartSpeed);
}

BOOL CPerformanceDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	CBCGPStaticLayout* layout = (CBCGPStaticLayout*)GetLayout();
	if (layout != NULL)
	{
		layout->AddAnchor(IDC_CHART_COUNT, LAYOUT_MOVE_NONE, LAYOUT_SIZE_BOTH, RPOINT(1, 1), RPOINT(1, 1 / 2));
		layout->AddAnchor(IDC_CHART_SPEED, LAYOUT_MOVE_VERT, LAYOUT_SIZE_BOTH, RPOINT(1, 1 / 2), RPOINT(1, 1 / 2));

		//! IDC_GROUP_RX
	}

	// 
	InitChartCount();
	InitChartSpeed();

	SetTimer(TIMER_ID_CHART_SPEED, TIMER_ELAPSE_CHART_SPEED, NULL);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CPerformanceDlg, CSkinDialog)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_USER_PER_SWITCH, &ThisClass::OnUserPerSwitch)
END_MESSAGE_MAP()


// CPerformanceDlg message handlers


// other functions


void CPerformanceDlg::UpdateChartColorTheme(int nTheme)
{/*
	CBCGPChartVisualObject* pChart = GetChart();
	if (pChart == NULL)
	{
		return;
	}

	if (nTheme < 0)
	{
		if (m_bIsDarkBackground)
		{
			m_bIsDarkBackground = FALSE;
			OnChangeBackgroundDarkness();
		}

		pChart->SetColors(theApp.GetCustomTheme(nTheme));

		m_bIsTexturedTheme = nTheme == -1;
	}
	else
	{
		CBCGPChartTheme::ChartTheme theme = (CBCGPChartTheme::ChartTheme)nTheme;

		BOOL bIsDarkBackground = FALSE;

		switch (theme)
		{
		case CBCGPChartTheme::CT_BLACK_AND_GOLD:
		case CBCGPChartTheme::CT_BLACK_AND_RED:
		case CBCGPChartTheme::CT_BLACK_AND_GREEN:
		case CBCGPChartTheme::CT_BLACK_AND_BLUE:
			bIsDarkBackground = TRUE;
			break;
		}

		if (m_bIsDarkBackground != bIsDarkBackground)
		{
			m_bIsDarkBackground = bIsDarkBackground;
			OnChangeBackgroundDarkness();
		}

		pChart->SetColors((CBCGPChartTheme::ChartTheme)nTheme);

		m_bIsTexturedTheme = FALSE;
	}

	CWnd* pGradientTypeCombo = GetDlgItem(IDC_FILL_GRADIENT_TYPE);
	if (pGradientTypeCombo->GetSafeHwnd() != NULL)
	{
		pGradientTypeCombo->EnableWindow(!m_bIsTexturedTheme);
	}*/
}

void CPerformanceDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_ID_CHART_COUNT:
	{
		CBCGPChartVisualObject* pChartCount = m_wndChartCount.GetChart();
		ASSERT_VALID(pChartCount);

		BOARD_PER_CONFIG &boardConfig = m_perTransmit.getBoardConfig();

		for (size_t i = 0; i < boardConfig.Channel.size(); i++)
		{
			CString strLabel;
			strLabel.Format(_T("CH%02d [TX:%ld,RX:%ld]"), i, m_perTransmit.getWriteCount(i), m_perTransmit.getReadCount(i));
			m_arSeries[i]->m_strSeriesName = strLabel;

			if (!m_perTransmit.isEmptyDeltaReadCount(i))
			{
				long delta = m_perTransmit.getDeltaReadCount(i); 
				if (!m_perTransmit.isRunning(KPairThread::PairFirst) && delta == 0)
				{
					continue;
				}
				m_arSeries[i]->AddDataPoint(delta);
			}
		}

		pChartCount->SetDirty(TRUE, TRUE);

		break;
	}
	case TIMER_ID_CHART_SPEED:
	{
		CBCGPChartVisualObject* pChartSpeed = m_wndChartSpeed.GetChart();
		ASSERT_VALID(pChartSpeed);

		CBCGPDoubleArray array;

		// 参数为进程id
		int rate = get_cpu_usage(GetCurrentProcessId());
		printf("CPU使用率: %d%%\n", rate);
		pChartSpeed->GetSeries(0)->AddDataPoint(rate);
		if (pChartSpeed->GetSeries(0)->GetDataPointCount() > 50) {
			pChartSpeed->GetSeries(0)->RemoveDataPoints(0, 1, TRUE);
		}

		pChartSpeed->SetDirty(TRUE, TRUE);
		
		break;
	}
	default:
		break;
	}
}

void CPerformanceDlg::OnClose()
{
	m_perTransmit.close();

	CSkinDialog::OnClose();
}

LRESULT CPerformanceDlg::OnUserPerSwitch(WPARAM wParam, LPARAM lParam)
{
	if (wParam)
	{
		if (!m_perTransmit.load())
		{
			CString text;
			text.Format(_T("设备 %d 加载失败！"), m_perTransmit.getBoardConfig().Id);
			BCGPMessageBox(text);
			return FALSE;
		}

		if (!m_perTransmit.open())
		{
			CString text;
			text.Format(_T("设备 %d 打开失败!"), m_perTransmit.getBoardConfig().Id);
			BCGPMessageBox(text);
			return FALSE;
		}

		SetTimer(TIMER_ID_CHART_COUNT, TIMER_ELAPSE_CHART_COUNT, NULL);
		m_perTransmit.start(true);
	}
	else
	{
		if (lParam)
		{
			KillTimer(TIMER_ID_CHART_COUNT);
			m_perTransmit.close();
		}
		else
		{
			m_perTransmit.stop(false);
		}
	}

	return TRUE;
}

void CPerformanceDlg::InitChartCount()
{
	// 
	CBCGPChartVisualObject* pChartCount = m_wndChartCount.GetChart();
	ASSERT_VALID(pChartCount);

	pChartCount->ShowAxisIntervalInterlacing(BCGP_CHART_Y_PRIMARY_AXIS);
	pChartCount->SetLegendPosition(BCGPChartLayout::LP_NONE);
	pChartCount->SetThemeOpacity(30);
	//pChartCount->SetChartType(BCGPChartLine);
	pChartCount->SetChartType(BCGPChartHistoricalLine);
	pChartCount->SetLegendPosition(BCGPChartLayout::LP_RIGHT);
	pChartCount->SetColors(CBCGPChartTheme::CT_OLIVE);
	pChartCount->SetZoomScrollConfig(BCGPChartMouseConfig::ZSO_WHEEL_PAN);
	//pChartCount->ShowDataMarkers(TRUE, 4, BCGPChartMarkerOptions::MS_CIRCLE);
	pChartCount->CleanUpChartData();
	
	CBCGPChartAxis* pAxisX = pChartCount->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	CBCGPChartAxis* pAxisY = pChartCount->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);

	double nDataPoints = 1000;
	CString strLabel;
	pAxisX->GetDisplayedLabel(nDataPoints, strLabel);
	pAxisX->m_strMaxDisplayedLabel = strLabel;
	pAxisX->SetAlwaysShowScrollBar();

	pAxisY->EnableScroll(FALSE);
	pAxisY->EnableLabelInterlacing(TRUE);
	pAxisY->m_strAxisName = _T("4 Bytes/s");
	pAxisY->m_bDisplayAxisName = TRUE;

	pChartCount->ShowAxisIntervalInterlacing(BCGP_CHART_Y_PRIMARY_AXIS);
	
	m_arSeries.resize(16);
	for (size_t i = 0; i < m_arSeries.size(); i++)
	{
		CString strLabel;
		strLabel.Format(_T("CH%02d"), i);

		m_arSeries[i] = pChartCount->CreateSeries(strLabel);
		m_arSeries[i]->SetSeriesLineWidth(1);
		m_arSeries[i]->SetCurveType(BCGPChartFormatSeries::CCT_SPLINE);
		pChartCount->SetAutoDestroy();
	}

	pChartCount->SetDirty(TRUE, TRUE);

	m_wndChartCount.SetFocus();
}

void CPerformanceDlg::InitChartSpeed()
{
	// 
	CBCGPChartVisualObject* pChartSpeed = m_wndChartSpeed.GetChart();
	ASSERT_VALID(pChartSpeed);

	pChartSpeed->SetLegendPosition(BCGPChartLayout::LP_NONE);
	pChartSpeed->SetThemeOpacity(30);
	pChartSpeed->SetChartType(BCGPChartArea);
	pChartSpeed->SetColors(CBCGPChartTheme::CT_OLIVE);
	pChartSpeed->SetZoomScrollConfig(BCGPChartMouseConfig::ZSO_WHEEL_PAN);
	pChartSpeed->SetLegendPosition(BCGPChartLayout::LP_NONE);
	pChartSpeed->CleanUpChartData();
	pChartSpeed->EnableAntialiasing();

	CBCGPChartAxis* pAxisX = pChartSpeed->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	CBCGPChartAxis* pAxisY = pChartSpeed->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
	
	pAxisX->m_axisLabelType = CBCGPChartAxis::ALT_NO_LABELS;
	pAxisX->m_majorTickMarkType = CBCGPChartAxis::TMT_NO_TICKS;
	pAxisX->m_axisDefaultPosition = CBCGPChartAxis::ADP_BOTTOM;

	pAxisY->m_strAxisName = _T("CPU (%)");
	pAxisY->m_bDisplayAxisName = TRUE;
	pAxisY->EnableScroll(FALSE);
	pAxisY->EnableLabelInterlacing(TRUE);

	pChartSpeed->ShowAxisIntervalInterlacing(BCGP_CHART_Y_PRIMARY_AXIS);

	pChartSpeed->GetSeries(0)->SetSeriesLineWidth(1);
	pChartSpeed->GetSeries(0)->SetCurveType(BCGPChartFormatSeries::CCT_SPLINE);

	CBCGPDoubleArray arArray;
	arArray.SetSize(50);
	pChartSpeed->GetSeries(0)->AddDataPoints(arArray);

	pChartSpeed->SetDirty(TRUE, TRUE);

	m_wndChartSpeed.SetFocus();
}
