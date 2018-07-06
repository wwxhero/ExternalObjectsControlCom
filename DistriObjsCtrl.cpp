// DistriObjsCtrl.cpp : Implementation of CDistriObjsCtrl

#include "stdafx.h"
#include "DistriObjsCtrl.h"
#include "dllmain.h"
// CDistriObjsCtrl



STDMETHODIMP CDistriObjsCtrl::CreateNetworkExternalObjectControl(LONG imple, LONG terminal)
{
	// TODO: Add your implementation code here
	m_pExternalCtrl = ::CreateNetworkExternalObjectControl((IMPLE)imple, (TERMINAL)terminal);
	HRESULT hr = (NULL != m_pExternalCtrl ? S_OK : E_NOTIMPL);
#ifdef _DEBUG
	const TCHAR* szImple[] = {"IGCOMM", "DISVRLINK"};
	const TCHAR* szTermi[] = {"edo_controller", "ado_controller", "ped_controller"};
	CString strLog;
	_com_error err(hr);
	strLog.Format(_T("%s = CreateNetworkExternalObjectControl(%s, %s)"), err.ErrorMessage(), szImple[imple], szTermi[terminal]);
	_AtlModule.LogEventEx(1, strLog);
#endif
	return hr;
}


STDMETHODIMP CDistriObjsCtrl::ReleaseNetworkExternalObjectControl(void)
{
	// TODO: Add your implementation code here
#ifdef _DEBUG
	CString strLog;
	strLog.Format(_T("ReleaseNetworkExternalObjectControl(%s)"), NULL == m_pExternalCtrl ? _T("NULL") : _T("Not NULL"));
	_AtlModule.LogEventEx(2, strLog);
#endif
	if (m_pExternalCtrl)
	{
		::ReleaseNetworkExternalObjectControl(m_pExternalCtrl);
		m_pExternalCtrl = NULL;
	}
	return S_OK;
}
