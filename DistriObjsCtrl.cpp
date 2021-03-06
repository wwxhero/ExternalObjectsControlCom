// DistriObjsCtrl.cpp : Implementation of CDistriObjsCtrl

#include "stdafx.h"
#include "DistriObjsCtrl.h"
#include "dllmain.h"
#include "CvedAdoCtrl.h"
#include "dynobj.h"
#define PI 3.1416

#define RAD2DEG(r)\
	(int)((r/PI)*180)

// CDistriObjsCtrl
extern void TaitBran2Quaternion(double i, double j, double k, double *w, double *x, double *y, double *z);
extern void Quaternion2Taitbran(double w, double x, double y, double z, double *i, double *j, double *k);


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
	const TCHAR* evtName[] = {"crtDyno", "delDyno", "crtPed", "delPed", "pegPed", "telPed", "evtUndefined"};
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

STDMETHODIMP CDistriObjsCtrl::GetpegPedTuple(LONG *id_parent, LONG *id_child)
{
	ATLASSERT(NULL != m_pCvedMsgQ);
	m_pCvedMsgQ->pegPedParams(id_parent, id_child);
#ifdef _DEBUG
	CString strLog;
	strLog.Format(_T("GetpegPedTuple(%d, %d)"), *id_parent, *id_child);
	_AtlModule.LogEventEx(20, strLog);
#endif
	return S_OK;
}

STDMETHODIMP CDistriObjsCtrl::GettelPedTuple(LONG *id
							, DOUBLE* xPos, DOUBLE* yPos, DOUBLE* zPos
							, DOUBLE* xTan, DOUBLE* yTan, DOUBLE* zTan
							, DOUBLE* xLat, DOUBLE* yLat, DOUBLE* zLat)
{
	ATLASSERT(NULL != m_pCvedMsgQ);
	m_pCvedMsgQ->telPedParams(id, xPos, yPos, zPos, xTan, yTan, zTan, xLat, yLat, zLat);
#ifdef _DEBUG
	CString strLog;
	strLog.Format(_T("GettelPedTuple(%d, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f)")
		, id
		, *xPos, *yPos, *zPos
		, *xTan, *yTan, *zTan
		, *xLat, *yLat, *zLat);
	_AtlModule.LogEventEx(21, strLog);
#endif
	return S_OK;
}

STDMETHODIMP CDistriObjsCtrl::GetcrtPedTuple(LONG *id_local, BSTR *name, LONG *solId,
							DOUBLE *xSize, DOUBLE *ySize, DOUBLE *zSize,
							DOUBLE *xPos, DOUBLE *yPos, DOUBLE *zPos,
							DOUBLE *xTan, DOUBLE *yTan, DOUBLE *zTan,
							DOUBLE *xLat, DOUBLE *yLat, DOUBLE *zLat,
							LONG *nParts)
{
	ATLASSERT(NULL != m_pCvedMsgQ);
	std::string strName;
	m_pCvedMsgQ->crtPedParams(id_local, strName, solId,
							xSize, ySize, zSize,
							xPos, yPos, zPos,
							xTan, yTan, zTan,
							xLat, yLat, zLat,
							nParts);
	_bstr_t bName(strName.c_str());
	*name = bName;

	const char** names_joint = NULL;
	unsigned int numJoints = 0;
	const CVED::CDynObj* pDynObj = m_pCvedMsgQ->BindObjIdToClass(*id_local);
	CVED::CArtiJoints::BFTAlloc(&names_joint, &numJoints);
	ATLASSERT(numJoints == *nParts);
	Joints joints = {names_joint, new TVector3D[*nParts], new TVector3D[*nParts], *nParts};
	ATLASSERT(m_idLocal2jointAngles.find(*id_local) == m_idLocal2jointAngles.end());
	m_idLocal2jointAngles[*id_local] = joints;
#ifdef _DEBUG
	CString strLog;
	strLog.Format(_T("GetcrtPedTuple(%d, %s, %d")
									_T("\n\t%f, %f, %f")
									_T("\n\t%f, %f, %f")
									_T("\n\t%f, %f, %f")
									_T("\n\t%f, %f, %f")
									_T("\n\t%d")
									, *id_local, strName.c_str(), *solId
									, *xSize, *ySize, *zSize
									, *xPos, *yPos, *zPos
									, *xTan, *yTan, *zTan
									, *xLat, *yLat, *zLat
									, *nParts);
	for (int i_part = 0; i_part < *nParts; i_part ++)
	{
		CString strPartName;
		strPartName.Format(_T("\n\t\t%d:%s"), i_part, names_joint[i_part]);
		strLog += strPartName;
	}
	_AtlModule.LogEventEx(9, strLog);
#endif
	return S_OK;
}

STDMETHODIMP CDistriObjsCtrl::GetcrtPedPartName(LONG id_local, LONG id_part, BSTR *name_part)
{
	auto it = m_idLocal2jointAngles.find(id_local);
	ATLASSERT(it != m_idLocal2jointAngles.end());
	Joints joints = it->second;
	_bstr_t bName(joints.names[id_part]);
	*name_part = bName;
#ifdef _DEBUG
	CString strLog;
	strLog.Format(_T("GetcrtPedPartName(%d, %s)")
				, id_part
				, joints.names[id_part]);
	_AtlModule.LogEventEx(15, strLog);
#endif
	return S_OK;
}

STDMETHODIMP CDistriObjsCtrl::GetdelPedTuple(LONG *id_local)
{
	ATLASSERT(NULL != m_pCvedMsgQ);
	m_pCvedMsgQ->delPedParams(id_local);
	auto it = m_idLocal2jointAngles.find(*id_local);
	ATLASSERT(it != m_idLocal2jointAngles.end());
	auto joints = it->second;
	const CVED::CDynObj* avatar = m_pCvedMsgQ->BindObjIdToClass(*id_local);
	CVED::CArtiJoints::BFTFree(joints.names, joints.num);
	delete [] joints.angles;
	delete [] joints.offsets;
	m_idLocal2jointAngles.erase(it);

#ifdef _DEBUG
	CString strLog;
	strLog.Format(_T("GetdelPedTuple(%d)"), *id_local);
	_AtlModule.LogEventEx(10, strLog);
#endif
	return S_OK;
}

STDMETHODIMP CDistriObjsCtrl::PreUpdateDynamicModels(void)
{
	ATLASSERT(NULL != m_pExternalCtrl && NULL != m_pCvedMsgQ);
	m_pExternalCtrl->PreUpdateDynamicModels();
	m_pCvedMsgQ->Maintainer();
#ifdef _DEBUG
	_AtlModule.LogEventEx(11, _T("PreUpdateDynamicModels"));
#endif
	return S_OK;
}

STDMETHODIMP CDistriObjsCtrl::PostUpdateDynamicModels(void)
{
	ATLASSERT(NULL != m_pExternalCtrl);
	m_pExternalCtrl->PostUpdateDynamicModels();
#ifdef _DEBUG
	_AtlModule.LogEventEx(12, _T("PostUpdateDynamicModels"));
#endif
	return S_OK;
}

STDMETHODIMP CDistriObjsCtrl::OnGetUpdate(LONG id_local, VARIANT_BOOL *received
						, DOUBLE *xPos, DOUBLE *yPos, DOUBLE *zPos
						, DOUBLE *xTan, DOUBLE *yTan, DOUBLE *zTan
						, DOUBLE *xLat, DOUBLE *yLat, DOUBLE *zLat)
{
	cvTObjContInp* inp = NULL; //for vrlink implementation, this parameter is useless
	cvTObjState* outp = NULL;
	m_pCvedMsgQ->BindStateBuff(id_local, &inp, &outp);
	ATLASSERT(NULL != m_pExternalCtrl);
	if (*received = m_pExternalCtrl->OnGetUpdate(id_local, inp, outp))
	{
		*xPos = outp->vehicleState.vehState.position.x;
		*yPos = outp->vehicleState.vehState.position.y;
		*zPos = outp->vehicleState.vehState.position.z;

		*xTan = outp->vehicleState.vehState.tangent.i;
		*yTan = outp->vehicleState.vehState.tangent.j;
		*zTan = outp->vehicleState.vehState.tangent.k;

		*xLat = outp->vehicleState.vehState.lateral.i;
		*yLat = outp->vehicleState.vehState.lateral.j;
		*zLat = outp->vehicleState.vehState.lateral.k;
	}
#ifdef _DEBUG
	CString strLog;
	strLog.Format(_T("%s = OnGetUpdate(%d, [%f, %f, %f], [%f, %f, %f], [%f, %f, %f])")
				, *received?_T("true"):_T("false"), id_local
				, *xPos, *yPos, *zPos
				, *xTan, *yTan, *zTan
				, *xLat, *yLat, *zLat);
	_AtlModule.LogEventEx(13, strLog);
#endif
	return S_OK;
}

STDMETHODIMP CDistriObjsCtrl::OnPreGetUpdateArt(LONG id_local, VARIANT_BOOL *received
						, DOUBLE *xPos, DOUBLE *yPos, DOUBLE *zPos
						, DOUBLE *xTan, DOUBLE *yTan, DOUBLE *zTan
						, DOUBLE *xLat, DOUBLE *yLat, DOUBLE *zLat)
{
	ATLASSERT(NULL != m_pExternalCtrl);
	cvTObjState* outp = NULL;
	cvTObjContInp* outInp = NULL;
	m_pCvedMsgQ->BindStateBuff(id_local, &outInp, &outp);
	if (*received = m_pExternalCtrl->OnGetUpdateArt(id_local, outp))
	{
		*xPos = outp->avatarState.position.x;
		*yPos = outp->avatarState.position.y;
		*zPos = outp->avatarState.position.z;

		*xTan = outp->avatarState.tangent.i;
		*yTan = outp->avatarState.tangent.j;
		*zTan = outp->avatarState.tangent.k;

		*xLat = outp->avatarState.lateral.i;
		*yLat = outp->avatarState.lateral.j;
		*zLat = outp->avatarState.lateral.k;

		auto it = m_idLocal2jointAngles.find(id_local);
		ATLASSERT(it != m_idLocal2jointAngles.end());
		Joints joints = it->second;
		CVED::CArtiJoints::BFTGetJoints(outp, joints.angles, joints.offsets, joints.num);
	}
#ifdef _DEBUG
	const CVED::CDynObj* pObj = m_pCvedMsgQ->BindObjIdToClass(id_local);
	ATLASSERT(cvEObjType::eCV_EXTERNAL_AVATAR == pObj->GetType()
			|| cvEObjType::eCV_AVATAR == pObj->GetType());
	CString strLog;
	strLog.Format(_T("%s = OnPreGetUpdateArt(%d, [%f, %f, %f], [%f, %f, %f], [%f, %f, %f])")
				, *received?_T("true"):_T("false"), id_local
				, *xPos, *yPos, *zPos
				, *xTan, *yTan, *zTan
				, *xLat, *yLat, *zLat);
	_AtlModule.LogEventEx(13, strLog);
#endif
	return S_OK;
}

STDMETHODIMP CDistriObjsCtrl::OnGetUpdateArt(LONG id_local, LONG id_part
							, DOUBLE* w, DOUBLE* x, DOUBLE* y, DOUBLE* z
							, DOUBLE* dx, DOUBLE* dy, DOUBLE* dz)
{
	ATLASSERT(NULL != m_pExternalCtrl);
	auto it = m_idLocal2jointAngles.find(id_local);
	ATLASSERT(it != m_idLocal2jointAngles.end());
	Joints joints = it->second;
	ATLASSERT(id_part < joints.num);
	const TVector3D& tb = joints.angles[id_part];
	TaitBran2Quaternion(tb.i, tb.j, tb.k
					, w, x, y, z);
	const TVector3D& delta = joints.offsets[id_part];
	*dx = delta.i;
	*dy = delta.j;
	*dz = delta.k;
#ifdef _DEBUG
	CString strLog;
	strLog.Format(_T("OnGetUpdateArt(%d, %d, %f, %f, %f, %f)")
				, id_local, id_part, *w, *x, *y, *z);
	_AtlModule.LogEventEx(16, strLog);
#endif
	return S_OK;
}

STDMETHODIMP CDistriObjsCtrl::OnGetUpdateArtDIGUY(LONG id_local, FLOAT* oris, FLOAT* trans)
{
#define NUM_JOINTS  15
	TVector3D angles[NUM_JOINTS] = {0};//fixme: this interface is for testing only
	TVector3D offsets[NUM_JOINTS] = {0};
	const char* names[NUM_JOINTS] = {NULL};
	const CVED::CDynObj* pObj = m_pCvedMsgQ->BindObjIdToClass(id_local);
	ATLASSERT(pObj->GetType() == eCV_AVATAR);
	const CVED::CAvatarObj* avatar = (const CVED::CAvatarObj*)pObj;
	unsigned int num = avatar->BFTGetJointsDiGuy(names, angles, offsets, NUM_JOINTS);
	ATLASSERT(num == NUM_JOINTS);
	for (int i_j = 0; i_j < NUM_JOINTS; i_j ++)
	{
		CString strLogItem;
		strLogItem.Format(" \n\t\t%2d:%20s\t=\t[%4d\t%4d\t%4d]\t [%4.2f\t%4.2f\t%4.2f]", i_j, names[i_j]
			, RAD2DEG(angles[i_j].k), RAD2DEG(angles[i_j].i), RAD2DEG(angles[i_j].j)
			, offsets[i_j].i, offsets[i_j].j, offsets[i_j].k);
		FLOAT* ori = oris + i_j * 3;
		ori[0] = angles[i_j].k;
		ori[1] = angles[i_j].i;
		ori[2] = angles[i_j].j;
		FLOAT* tran = trans + i_j * 3;
		tran[0] = offsets[i_j].i;
		tran[1] = offsets[i_j].j;
		tran[2] = offsets[i_j].k;
		_AtlModule.LogEventEx(19, strLogItem);
	}
	return S_OK;
#undef NUM_JOINTS
}

STDMETHODIMP CDistriObjsCtrl::OnPostPushUpdateArt(LONG id_local
						, DOUBLE xPos, DOUBLE yPos, DOUBLE zPos
						, DOUBLE xTan, DOUBLE yTan, DOUBLE zTan
						, DOUBLE xLat, DOUBLE yLat, DOUBLE zLat)
{
	cvTObjContInp* inp = NULL; //for vrlink implementation, this parameter is useless
	cvTObjState* s = NULL;

	m_pCvedMsgQ->BindStateBuff(id_local, &inp, &s);

	auto it = m_idLocal2jointAngles.find(id_local);
	ATLASSERT(it != m_idLocal2jointAngles.end());
	Joints joints = it->second;
	CVED::CArtiJoints::BFTSetJoints(s, joints.angles, joints.offsets, joints.num);

	s->avatarState.position.x = xPos;
	s->avatarState.position.y = yPos;
	s->avatarState.position.z = zPos;

	s->avatarState.tangent.i = xTan;
	s->avatarState.tangent.j = yTan;
	s->avatarState.tangent.k = zTan;

	s->avatarState.lateral.i = xLat;
	s->avatarState.lateral.j = yLat;
	s->avatarState.lateral.k = zLat;

	ATLASSERT(NULL != m_pExternalCtrl);
	m_pExternalCtrl->OnPushUpdateArt(id_local, s);

#ifdef _DEBUG
	const CVED::CDynObj* pObj = m_pCvedMsgQ->BindObjIdToClass(id_local);
	ATLASSERT(cvEObjType::eCV_EXTERNAL_AVATAR == pObj->GetType());
	CString strLog;
	strLog.Format(_T("OnPostPushUpdateArt(%d, [%f, %f, %f], [%f, %f, %f], [%f, %f, %f])")
				, id_local
				, xPos, yPos, zPos
				, xTan, yTan, zTan
				, xLat, yLat, zLat);
	_AtlModule.LogEventEx(17, strLog);
#endif
	return S_OK;
}




STDMETHODIMP CDistriObjsCtrl::OnPushUpdateArt(LONG id_local, LONG id_part
							, DOUBLE w, DOUBLE x, DOUBLE y, DOUBLE z
							, DOUBLE dx, DOUBLE dy, DOUBLE dz)
{
	ATLASSERT(NULL != m_pExternalCtrl);
	//m_pExternalCtrl->OnPushUpdateArt();
	auto it = m_idLocal2jointAngles.find(id_local);
	ATLASSERT(it != m_idLocal2jointAngles.end());
	Joints joints = it->second;
	ATLASSERT(id_part < joints.num);
	TVector3D& tb = joints.angles[id_part];
	Quaternion2Taitbran(w, x, y, z
					, &tb.i, &tb.j, &tb.k);
	TVector3D& os = joints.offsets[id_part];
	os.i = dx; os.j = dy; os.k = dz;
#ifdef _DEBUG
	CString strLog;
	strLog.Format(_T("OnPushUpdateArt(%d, %d, %f, %f, %f, %f)")
				, id_local, id_part, w, x, y, z);
	_AtlModule.LogEventEx(18, strLog);
#endif
	return S_OK;
}