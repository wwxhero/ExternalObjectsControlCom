#include "StdAfx.h"
#include "CvedDistriMsgQ.h"
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

CVED::CDynObj* CCvedDistriMsgQ::LocalCreateDynObj(const string&		cName,
												cvEObjType			type,
												const cvTObjAttr&	cAttr,
												const CPoint3D*		cpInitPos,
												const CVector3D*	cpInitTan,
												const CVector3D*	cpInitLat)
{
	//todo: record a message for creating dyn obj
	CVED::CDynObj* obj = CCvedDistri::LocalCreateDynObj(cName, type, cAttr, cpInitPos, cpInitTan, cpInitLat);
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

CVED::CDynObj*	CCvedDistriMsgQ::DistriCreateDynObj(const string&		cName,
								const cvTObjAttr&	cAttr,
								const CPoint3D*		cpInitPos,
								const CVector3D*	cpInitTan,
								const CVector3D*	cpInitLat)
{
	ATLASSERT(0); //it doesn't accept Distrixxxx calls
	return NULL;
}

void		CCvedDistriMsgQ::DistriDeleteDynObj(CVED::CDynObj*)
{
	ATLASSERT(0);
}