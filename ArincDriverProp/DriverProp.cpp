// DriverProp.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "resource.h"
#include <prsht.h>
#include "DriverProp.h"

#pragma comment(lib, "comctl32.lib")

HINSTANCE  _g_hModule;		// 

// 
typedef struct _PP_PARAMS_STRUCT
{
	HKEY    drvConfigKey;
	BOOL    drvConfigKeyValid;
	HKEY    svcConfigKey;
	BOOL    svcConfigKeyValid;

	ULONG   timeoutScaling;
	ULONG   statusLines;
	ULONG   activeConnectOnly;

	ULONG   oldTimeoutScaling;
	ULONG   oldStatusLines;
	ULONG   oldActiveConnectOnly;

} PP_PARAMS_STRUCT;

EXTERN_C BOOL CALLBACK PropDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
EXTERN_C UINT CALLBACK PropSheetPageCallback(HWND hwnd, UINT uMsg, LPPROPSHEETPAGE lpPropSheetPage);

VOID CheckSettings(HWND hwnd);
VOID ReadConfig(HWND hwnd);
VOID WriteConfig(HWND hwnd);

EXTERN_C VOID WINAPI SetHInstance(HINSTANCE hModule)
{
	_g_hModule = hModule;
}

EXTERN_C BOOL WINAPI ArincPropPageProvider(
	LPVOID pInfo, LPFNADDPROPSHEETPAGE addProp,
	LPARAM lParam)
{
	PROPSHEETPAGE	propSheetPage;
	HPROPSHEETPAGE	hPropSheetPage;
	UINT			dialogID;

	HR_PRINT_METHOD_BEGIN();

	switch (GetSystemDefaultLCID())
	{
// 	case 0x0404:	// МЁНе
// 		dialogID = IDD_COMM_CONFIG_EN;
// 		break;
	case 0x0409:	// US
		dialogID = IDD_COMM_CONFIG_EN;
		break;
	case 0x0804:	// Chinese Simplify
		dialogID = IDD_COMM_CONFIG_ZH;
		break;
	default:
		dialogID = IDD_COMM_CONFIG_EN;
		break;
	}

	propSheetPage.dwSize = sizeof(PROPSHEETPAGE);
	propSheetPage.dwFlags = PSP_USECALLBACK/* | PSP_HASHELP | PSP_USEICONID | PSP_USETITLE*/;
	propSheetPage.hInstance = _g_hModule;
	propSheetPage.pszTemplate = MAKEINTRESOURCE(dialogID);
	propSheetPage.pszIcon = MAKEINTRESOURCE(IDI_ICON1);
	propSheetPage.pfnDlgProc = PropDialogProc;
	propSheetPage.lParam = 0;		// (LPARAM)lParam;
	propSheetPage.pfnCallback = PropSheetPageCallback;

	hPropSheetPage = CreatePropertySheetPage(&propSheetPage);
	if (hPropSheetPage == NULL)
	{
		return FALSE;
	}

	if (!(*addProp)(hPropSheetPage, lParam))
	{
		DestroyPropertySheetPage(hPropSheetPage);
		return FALSE;
	}

	HR_PRINT_METHOD_END();

	return TRUE;
}

EXTERN_C BOOL CALLBACK PropDialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			
			CheckSettings(hwnd);
		}
		break;
	case WM_COMMAND:
		{
		}
		break;
	case WM_NOTIFY:
		{
		}
		break;
	default:
		break;
	}
	return TRUE;
}

EXTERN_C UINT CALLBACK PropSheetPageCallback(HWND hwnd, UINT uMsg, LPPROPSHEETPAGE lpPropSheetPage)
{
	switch (uMsg)
	{
	case PSPCB_CREATE:
		break;
	case PSPCB_ADDREF:
		break;
	case PSPCB_RELEASE:
		{/*
			PP_PARAMS_STRUCT* pParams;
			pParams = (PP_PARAMS_STRUCT*)lpPropSheetPage->lParam;
			if (pParams->drvConfigKeyValid)
			{
				RegCloseKey(pParams->drvConfigKey);
			}

			if (pParams->svcConfigKeyValid)
			{
				RegCloseKey(pParams->svcConfigKey);
			}

			LocalFree(pParams);*/
		}
		break;
	default:
		break;
	}

	return TRUE;
}

VOID CheckSettings(HWND hwnd)
{

}

VOID ReadConfig(HWND hwnd)
{

}

VOID WriteConfig(HWND hwnd)
{

}
