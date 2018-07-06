// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "ExternalObjectsControlCom_i.h"
#include "dllmain.h"

const TCHAR CExternalObjectsControlComModule::c_logSrc[] = _T("ExternalObjectsControlCom");

CExternalObjectsControlComModule _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
