// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

extern DWORD __DeviceInitialize();
extern DWORD __DeviceCleanup();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	BOOL status = TRUE;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);
		status = __DeviceInitialize();
		break;
	}
	case DLL_PROCESS_DETACH:
	{
		status = __DeviceCleanup();
		break;
	}
	case DLL_THREAD_ATTACH:
	{
		break;
	}
	case DLL_THREAD_DETACH:
	{
		break;
	}
	default:
		break;
	}
	return status;
}

