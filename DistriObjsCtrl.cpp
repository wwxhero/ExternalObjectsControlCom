// DistriObjsCtrl.cpp : Implementation of CDistriObjsCtrl

#include "stdafx.h"
#include "DistriObjsCtrl.h"
#include "dllmain.h"
#include "CvedAdoCtrl.h"
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
	strLog.Format(_T("%s = CreateNetworkExternalObjectControl(%s, %s)"), err.ErrorMessage(), szImple[imple], szTermi[terminal], NULL != m_pExternalCtrl ? EVENTLOG_INFORMATION_TYPE : EVENTLOG_ERROR_TYPE);
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
	_AtlModule.LogEventEx(2, strLog, NULL != m_pExternalCtrl ? EVENTLOG_INFORMATION_TYPE : EVENTLOG_WARNING_TYPE);
#endif
	if (m_pExternalCtrl)
	{
		::ReleaseNetworkExternalObjectControl(m_pExternalCtrl);
		m_pExternalCtrl = NULL;
	}
	return S_OK;
}


STDMETHODIMP CDistriObjsCtrl::Initialize(BSTR a_pathScene)
{
	// TODO: Add your implementation code here
	if( m_pExternalCtrl )
	{
		::ReleaseNetworkExternalObjectControl(m_pExternalCtrl);
		m_pExternalCtrl = NULL;
	}
	_bstr_t pathScene(a_pathScene, false);
	std::string strPathScene((LPCTSTR)pathScene);
	HRESULT hr = S_OK;
	CSnoParserDistri parser;
	bool Initialized = parser.ParseFile(strPathScene);
	if (Initialized)
	{
		CSnoParser::TIterator pBlock = parser.Begin();
		bool scenFileError = (
				pBlock == parser.End() ||
				pBlock->GetBlockName() != string( "Header" )
				);
		if( scenFileError )
		{
#ifdef _DEBUG
			_AtlModule.LogEventEx(3, _T("File is incomplete, or first block is not the header."), EVENTLOG_WARNING_TYPE);
#endif
			hr = E_INVALIDARG;
		}
		else
		{
			CHeaderParseBlock* pHdrBlk = new CHeaderDistriParseBlock( *pBlock );
			if (m_pCved) delete m_pCved;
			m_pCved = new CVED::CCvedADOCtrl(m_pExternalCtrl);
			//float   behavDeltaT = (float) (1.0f / 30.0f);			// behaviors execution period
			//int     dynaMult = 2;				// how many times dynamics runs per behavior
			//m_pCved->Configure(CVED::CCved::eCV_SINGLE_USER, behavDeltaT, dynaMult);
			string cvedErr;
			bool success = m_pCved->Init( pHdrBlk->GetLriFile(), cvedErr );
			if( !success )
			{
#ifdef _DEBUG
				CString strLog;
				strLog.Format(_T("Cved::Init failed: %s"), cvedErr.c_str());
				_AtlModule.LogEventEx(3, strLog, EVENTLOG_ERROR_TYPE);
#endif
				hr = E_UNEXPECTED;
			}
			else
			{
				Initialized = m_pExternalCtrl->Initialize(static_cast<CHeaderDistriParseBlock&>(*pHdrBlk), static_cast<CVED::CCvedDistri*>(m_pCved)); //the configuration for localhost simulator will be identified
#ifdef _DEBUG
				if (!Initialized)
				{
					CString strLog;
					strLog.Format(_T("External Control Initialization failed"));
					_AtlModule.LogEventEx(3, strLog, EVENTLOG_ERROR_TYPE);
				}
#endif

			}
		}

		if (!Initialized)
		{
			::ReleaseNetworkExternalObjectControl(m_pExternalCtrl);
			m_pExternalCtrl = NULL;
			hr = E_FAIL;
		}
	}
	else
		 hr = E_INVALIDARG;
#ifdef _DEBUG
	CString strLog;
	_com_error err(hr);
	strLog.Format(_T("%s = Initialize(%s)"), err.ErrorMessage(), (LPCTSTR)pathScene);
	_AtlModule.LogEventEx(3, strLog);
#endif
	return hr;
}


STDMETHODIMP CDistriObjsCtrl::UnInitialize(void)
{
	// TODO: Add your implementation code here
	if (m_pExternalCtrl)
		m_pExternalCtrl->UnInitialize();
#ifdef _DEBUG
	CString strLog;
	strLog.Format(_T("%s->Initialize(%s)"), NULL == m_pExternalCtrl ? "NULL" : "Not NULL");
	_AtlModule.LogEventEx(4, strLog);
#endif
	return S_OK;
}
