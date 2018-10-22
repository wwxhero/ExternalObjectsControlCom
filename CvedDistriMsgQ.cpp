#include "StdAfx.h"
#include "CvedDistriMsgQ.h"
#include "cvedstrc.h"
#define INIT_BUFF_CNT 32
#define DELTA_BUFF_CNT 32

CCvedDistriMsgQ::CCvedDistriMsgQ(CVED::IExternalObjectControl* pCtrl) : CVED::CCvedDistri(pCtrl)
																	, m_msgQ(INIT_BUFF_CNT)
{
}


CCvedDistriMsgQ::~CCvedDistriMsgQ(void)
{
}

bool CCvedDistriMsgQ::empty() const
{
	//todo: report if queue is empty
	return m_msgQ.empty();
}
EVT CCvedDistriMsgQ::front() const
{
	//todo: the front element in the queue
	Param param = m_msgQ.front();
	return param.ParamDef.evt;
}
void CCvedDistriMsgQ::pop()
{
	//todo: pops front element
	Param param = m_msgQ.front();
	m_msgQ.pop_front();
}
void CCvedDistriMsgQ::crtDynoParams(long* id_local, std::string& name, long* solId
				, double *xSize, double *ySize, double *zSize
				, double *xPos, double *yPos, double *zPos
				, double *xTan, double *yTan, double *zTan
				, double *xLat, double *yLat, double *zLat)
{
	//todo: returns the crtDyno parameters
	Param param = m_msgQ.front();
	ATLASSERT(param.ParamDef.evt == crtDyno);
	*id_local = param.ParamCrtDyno.id_local;
	name = param.ParamCrtDyno.name;
	*solId = param.ParamCrtDyno.solId;
	*xSize = param.ParamCrtDyno.xSize;
	*ySize = param.ParamCrtDyno.ySize;
	*zSize = param.ParamCrtDyno.zSize;
	*xPos = param.ParamCrtDyno.xPos;
	*yPos = param.ParamCrtDyno.yPos;
	*zPos = param.ParamCrtDyno.zPos;
	*xTan = param.ParamCrtDyno.xTan;
	*yTan = param.ParamCrtDyno.yTan;
	*zTan = param.ParamCrtDyno.zTan;
	*xLat = param.ParamCrtDyno.xLat;
	*yLat = param.ParamCrtDyno.yLat;
	*zLat = param.ParamCrtDyno.zLat;
}
void CCvedDistriMsgQ::delDynoParams(long* id_local)
{
	//todo: returns the delDyno parameters
	Param param = m_msgQ.front();
	ATLASSERT(param.ParamDef.evt == delDyno);
	*id_local = param.ParamDelDyno.id_local;
}

void CCvedDistriMsgQ::LocalDeleteDynObj( CVED::CDynObj* pObj)
{
	//todo: record a message for deleting dyn obj
	Param param;
	param.ParamDelDyno.evt = delDyno;
	param.ParamDelDyno.id_local = pObj->GetId();
	if (m_msgQ.full())
	{
		m_msgQ.resize(m_msgQ.size() + DELTA_BUFF_CNT);
	}
	m_msgQ.push_back(param);
	CCvedDistri::LocalDeleteDynObj(pObj);
}

CVED::CDynObj* CCvedDistriMsgQ::LocalCreateEDO(
										const string&		cName,
										const cvTObjAttr&	cAttr,
										const CPoint3D*		cpInitPos,
										const CVector3D*	cpInitTan,
										const CVector3D*	cpInitLat)
{
	CVED::CDynObj* obj = CCvedDistri::CreateDynObj(cName, eCV_VEHICLE, cAttr, cpInitPos, cpInitTan, cpInitLat);
	if (NULL != obj)
	{
		Param param;
		param.ParamCrtDyno.evt = crtDyno;
		ATLASSERT(cName.length() <= MAX_NAME_LEN); //doesn't support long name
		strcpy(param.ParamCrtDyno.name, cName.c_str());
		param.ParamCrtDyno.id_local = obj->GetId();
		param.ParamCrtDyno.solId = cAttr.solId;
		param.ParamCrtDyno.xSize = cAttr.xSize;
		param.ParamCrtDyno.ySize = cAttr.ySize;
		param.ParamCrtDyno.zSize = cAttr.zSize;
		param.ParamCrtDyno.xPos = cpInitPos->m_x;
		param.ParamCrtDyno.yPos = cpInitPos->m_y;
		param.ParamCrtDyno.zPos = cpInitPos->m_z;
		param.ParamCrtDyno.xTan = cpInitTan->m_i;
		param.ParamCrtDyno.yTan = cpInitTan->m_j;
		param.ParamCrtDyno.zTan = cpInitTan->m_k;
		param.ParamCrtDyno.xLat = cpInitLat->m_i;
		param.ParamCrtDyno.yLat = cpInitLat->m_j;
		param.ParamCrtDyno.zLat = cpInitLat->m_k;
		if (m_msgQ.full())
		{
			m_msgQ.resize(m_msgQ.size() + DELTA_BUFF_CNT);
		}
		m_msgQ.push_back(param);
	}
	return obj;
}

CVED::CDynObj* CCvedDistriMsgQ::LocalCreateADO(
										const string&		cName,
										const cvTObjAttr&	cAttr,
										const CPoint3D*		cpInitPos,
										const CVector3D*	cpInitTan,
										const CVector3D*	cpInitLat)
{
	//todo: record a message for creating dyn obj
	CVED::CDynObj* obj = CCvedDistri::CreateDynObj(cName, eCV_VEHICLE, cAttr, cpInitPos, cpInitTan, cpInitLat);
	if (NULL != obj)
	{
		Param param;
		param.ParamCrtDyno.evt = crtDyno;
		ATLASSERT(cName.length() <= MAX_NAME_LEN); //doesn't support long name
		strcpy(param.ParamCrtDyno.name, cName.c_str());
		param.ParamCrtDyno.id_local = obj->GetId();
		param.ParamCrtDyno.solId = cAttr.solId;
		param.ParamCrtDyno.xSize = cAttr.xSize;
		param.ParamCrtDyno.ySize = cAttr.ySize;
		param.ParamCrtDyno.zSize = cAttr.zSize;
		param.ParamCrtDyno.xPos = cpInitPos->m_x;
		param.ParamCrtDyno.yPos = cpInitPos->m_y;
		param.ParamCrtDyno.zPos = cpInitPos->m_z;
		param.ParamCrtDyno.xTan = cpInitTan->m_i;
		param.ParamCrtDyno.yTan = cpInitTan->m_j;
		param.ParamCrtDyno.zTan = cpInitTan->m_k;
		param.ParamCrtDyno.xLat = cpInitLat->m_i;
		param.ParamCrtDyno.yLat = cpInitLat->m_j;
		param.ParamCrtDyno.zLat = cpInitLat->m_k;
		if (m_msgQ.full())
		{
			m_msgQ.resize(m_msgQ.size() + DELTA_BUFF_CNT);
		}
		m_msgQ.push_back(param);
	}
	return obj;
}

CVED::CDynObj* CCvedDistriMsgQ::LocalCreatePDO( bool 				own,
												const string&		cName,
												const cvTObjAttr&	cAttr,
												const CPoint3D*		cpInitPos,
												const CVector3D*	cpInitTan,
												const CVector3D*	cpInitLat)
{
	//todo: record a message for creating dyn obj
	CVED::CDynObj* obj = CCvedDistri::CreateDynObj(cName, eCV_EXTERNAL_AVATAR, cAttr, cpInitPos, cpInitTan, cpInitLat);
	if (NULL != obj)
	{
		Param param;
		param.ParamCrtPed.evt = crtPed;
		ATLASSERT(cName.length() <= MAX_NAME_LEN); //doesn't support long name
		strcpy(param.ParamCrtPed.name, cName.c_str());
		param.ParamCrtPed.id_local = obj->GetId();
		param.ParamCrtPed.solId = cAttr.solId;
		param.ParamCrtPed.xSize = cAttr.xSize;
		param.ParamCrtPed.ySize = cAttr.ySize;
		param.ParamCrtPed.zSize = cAttr.zSize;
		param.ParamCrtPed.xPos = cpInitPos->m_x;
		param.ParamCrtPed.yPos = cpInitPos->m_y;
		param.ParamCrtPed.zPos = cpInitPos->m_z;
		param.ParamCrtPed.xTan = cpInitTan->m_i;
		param.ParamCrtPed.yTan = cpInitTan->m_j;
		param.ParamCrtPed.zTan = cpInitTan->m_k;
		param.ParamCrtPed.xLat = cpInitLat->m_i;
		param.ParamCrtPed.yLat = cpInitLat->m_j;
		param.ParamCrtPed.zLat = cpInitLat->m_k;
		param.ParamCrtPed.nPart = static_cast<CVED::CExternalAvatarObj*>(obj)->GetNumParts();
		if (m_msgQ.full())
		{
			m_msgQ.resize(m_msgQ.size() + DELTA_BUFF_CNT);
		}
		m_msgQ.push_back(param);
	}
	return obj;
}


void CCvedDistriMsgQ::LocalDeletePDO(CVED::CDynObj* pObj)
{
	//todo: record a message for deleting dyn obj
	Param param;
	param.ParamDelPed.evt = delPed;
	param.ParamDelPed.id_local = pObj->GetId();
	if (m_msgQ.full())
	{
		m_msgQ.resize(m_msgQ.size() + DELTA_BUFF_CNT);
	}
	m_msgQ.push_back(param);
	CCvedDistri::LocalDeleteDynObj(pObj);
}

void CCvedDistriMsgQ::crtPedParams(long* id_local, std::string& name, long* solId
							, double *xSize, double *ySize, double *zSize
							, double *xPos, double *yPos, double *zPos
							, double *xTan, double *yTan, double *zTan
							, double *xLat, double *yLat, double *zLat
							, long* nPart)
{
	//todo: returns the crtDyno parameters
	Param param = m_msgQ.front();
	ATLASSERT(param.ParamDef.evt == crtPed);
	*id_local = param.ParamCrtPed.id_local;
	name = param.ParamCrtPed.name;
	*solId = param.ParamCrtPed.solId;
	*xSize = param.ParamCrtPed.xSize;
	*ySize = param.ParamCrtPed.ySize;
	*zSize = param.ParamCrtPed.zSize;
	*xPos = param.ParamCrtPed.xPos;
	*yPos = param.ParamCrtPed.yPos;
	*zPos = param.ParamCrtPed.zPos;
	*xTan = param.ParamCrtPed.xTan;
	*yTan = param.ParamCrtPed.yTan;
	*zTan = param.ParamCrtPed.zTan;
	*xLat = param.ParamCrtPed.xLat;
	*yLat = param.ParamCrtPed.yLat;
	*zLat = param.ParamCrtPed.zLat;
	*nPart = param.ParamCrtPed.nPart;
}

void CCvedDistriMsgQ::crtPedPartName(long id_local, long id_part, std::string& name)
{
	Param param = m_msgQ.front();
	ATLASSERT(param.ParamDef.evt == crtPed
		&& id_local == param.ParamCrtPed.id_local);
	name = param.ParamCrtPed.partNames[id_part];
}

void CCvedDistriMsgQ::delPedParams(long* id_local)
{
	//todo: returns the delDyno parameters
	Param param = m_msgQ.front();
	ATLASSERT(param.ParamDef.evt == delPed);
	*id_local = param.ParamDelPed.id_local;
}

CVED::CDynObj*	CCvedDistriMsgQ::DistriCreateADO(const string&		cName,
								const cvTObjAttr&	cAttr,
								const CPoint3D*		cpInitPos,
								const CVector3D*	cpInitTan,
								const CVector3D*	cpInitLat)
{
	ATLASSERT(0); //it doesn't accept Distrixxxx calls
	return NULL;
}

void		CCvedDistriMsgQ::DistriDeleteADO(CVED::CDynObj*)
{
	ATLASSERT(0);
}

void CCvedDistriMsgQ::BindStateBuff(long id_local, cvTObjContInp** inp, cvTObjState** s)
{
	TObj* pO = BindObj(id_local);
	ATLASSERT((pO->type == eCV_VEHICLE || pO->type == eCV_EXTERNAL_DRIVER)
			&& (pO->phase == eALIVE || pO->phase == eDYING));

	if( (m_pHdr->frame & 1) == 0 )
	{
		// even frame
		*inp = &pO->stateBufA.contInp;
		//currState    = pO->stateBufB.state;
		*s = &pO->stateBufB.state;
	}
	else
	{
		// odd frame
		*inp = &pO->stateBufB.contInp;
		//currState    = pO->stateBufA.state;
		*s = &pO->stateBufA.state;
	}
}

