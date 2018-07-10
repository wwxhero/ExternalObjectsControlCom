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
	// TODO: connect cved msg queue to external object control
	ATLASSERT(NULL != m_pExternalCtrl);
	_bstr_t pathScene(a_pathScene, false);
	std::string strPathScene((LPCTSTR)pathScene);
	HRESULT hr = S_OK;
	CSnoParserDistri parser;
	bool initialized = parser.ParseFile(strPathScene);
	if (initialized)
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
			if( m_pCvedMsgQ ) delete m_pCvedMsgQ;
			m_pCvedMsgQ = new CCvedDistriMsgQ(m_pExternalCtrl);
			double behavDeltaT  = (float) (1.0f / 30.0f);
			int dynaMult     = 2;
			m_pCvedMsgQ->Configure( CVED::CCved::eCV_SINGLE_USER, behavDeltaT, dynaMult );
			std::string cvedErr;
			initialized = m_pCvedMsgQ->Init( pHdrBlk->GetLriFile(), cvedErr )
						&& m_pExternalCtrl->Initialize(static_cast<CHeaderDistriParseBlock&>(*pHdrBlk), m_pCvedMsgQ); //the configuration for localhost simulator will be identified
#ifdef _DEBUG
			if (!initialized)
			{
				CString strLog;
				strLog.Format(_T("External Control Initialization failed:%s"), cvedErr.c_str());
				_AtlModule.LogEventEx(3, strLog, EVENTLOG_ERROR_TYPE);
			}
#endif
		}

		if (!initialized)
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
	_AtlModule.LogEventEx(3, strLog, initialized ? EVENTLOG_INFORMATION_TYPE : EVENTLOG_ERROR_TYPE);
#endif
	return hr;
}


STDMETHODIMP CDistriObjsCtrl::UnInitialize(void)
{
	// TODO: unitialize external control
	if (m_pExternalCtrl)
		m_pExternalCtrl->UnInitialize();
	ATLASSERT(NULL != m_pCvedMsgQ);
	delete m_pCvedMsgQ;
	m_pCvedMsgQ = NULL;
#ifdef _DEBUG
	CString strLog;
	strLog.Format(_T("%s->UnInitialize()"), NULL == m_pExternalCtrl ? "NULL" : "Not NULL");
	_AtlModule.LogEventEx(4, strLog);
#endif
	return S_OK;
}


STDMETHODIMP CDistriObjsCtrl::QFrontEvent(EVT* evt, VARIANT_BOOL* empty)
{
	// TODO: front event from cved message queue
	ATLASSERT(NULL != m_pCvedMsgQ);
	if (!m_pCvedMsgQ->empty())
	{
		*empty = FALSE;
		*evt = m_pCvedMsgQ->front();
	}
	else
	{
		*empty = TRUE;
		*evt = evtUndefined;
	}
#ifdef _DEBUG
	CString strLog;
	const TCHAR* evtName[] = {"crtDyno", "delDyno", "evtUndefined"};
	const TCHAR* boolName[] = {"false", "true"};
	strLog.Format(_T("QFrontEvent(%s, %s)"), evtName[*evt], boolName[*empty]);
	_AtlModule.LogEventEx(5, strLog);
#endif
	return S_OK;
}

STDMETHODIMP CDistriObjsCtrl::QPopEvent(void)
{
	ATLASSERT(NULL != m_pCvedMsgQ);
	m_pCvedMsgQ->pop();
#ifdef _DEBUG
	_AtlModule.LogEventEx(6, _T("QPopEvent"));
#endif
	return S_OK;
}

STDMETHODIMP CDistriObjsCtrl::GetcrtDynoTuple(LONG *id_local, BSTR *name, LONG *solId,
							DOUBLE *xSize, DOUBLE *ySize, DOUBLE *zSize,
							DOUBLE *xPos, DOUBLE *yPos, DOUBLE *zPos,
							DOUBLE *xTan, DOUBLE *yTan, DOUBLE *zTan,
							DOUBLE *xLat, DOUBLE *yLat, DOUBLE *zLat)
{
	ATLASSERT(NULL != m_pCvedMsgQ);
	std::string strName;
	m_pCvedMsgQ->crtDynoParams(id_local, strName, solId,
							xSize, ySize, zSize,
							xPos, yPos, zPos,
							xTan, yTan, zTan,
							xLat, yLat, zLat);
	_bstr_t bName(strName.c_str());
	*name = bName;
#ifdef _DEBUG
	CString strLog;

	strLog.Format(_T("GetcrtDynoTuple(%d, %s, %d")
									_T("\n\t%f, %f, %f")
									_T("\n\t%f, %f, %f")
									_T("\n\t%f, %f, %f")
									_T("\n\t%f, %f, %f")
									, *id_local, strName.c_str(), *solId
									, *xSize, *ySize, *zSize
									, *xPos, *yPos, *zPos
									, *xTan, *yTan, *zTan
									, *xLat, *yLat, *zLat);
	_AtlModule.LogEventEx(7, strLog);
#endif
	return S_OK;
}

STDMETHODIMP CDistriObjsCtrl::GetdelDynoTuple(LONG *id_local)
{
	ATLASSERT(NULL != m_pCvedMsgQ);
	m_pCvedMsgQ->delDynoParams(id_local);
#ifdef _DEBUG
	CString strLog;
	strLog.Format(_T("GetdelDynoTuple(%d)"), *id_local);
	_AtlModule.LogEventEx(8, strLog);
#endif
	return S_OK;
}
