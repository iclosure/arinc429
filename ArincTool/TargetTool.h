// TargetTool.h : main header file for the ArincTool application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

// CTargetToolApp:
// See ArincTool.cpp for the implementation of this class
//

class CTargetToolApp : public CWinApp,
	public CBCGPWorkspace
{
public:
	CTargetToolApp();

	// Override from CBCGPWorkspace
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void PreLoadState ();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	static CString GetAppVersion();

private:

};

extern CTargetToolApp theApp;