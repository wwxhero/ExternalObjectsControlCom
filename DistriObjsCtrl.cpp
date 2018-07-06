// DistriObjsCtrl.cpp : Implementation of CDistriObjsCtrl

#include "stdafx.h"
#include "DistriObjsCtrl.h"
#include "dllmain.h"

// CDistriObjsCtrl



STDMETHODIMP CDistriObjsCtrl::CreateNetworkExternalObjectControl(LONG imple, LONG terminal)
{
	// TODO: Add your implementation code here
	_AtlModule.LogEventEx(1, _T("CreateNetworkExternalObjectControl"));
	return S_OK;
}


STDMETHODIMP CDistriObjsCtrl::ReleaseNetworkExternalObjectControl(void)
{
	// TODO: Add your implementation code here

	return S_OK;
}
