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
	return param.def.evt;
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
	ATLASSERT(param.def.evt == crtDyno);
	*id_local = param.crtDyno.id_local;
	name = param.crtDyno.name;
	*solId = param.crtDyno.solId;
	*xSize = param.crtDyno.xSize;
	*ySize = param.crtDyno.ySize;
	*zSize = param.crtDyno.zSize;
	*xPos = param.crtDyno.xPos;
	*yPos = param.crtDyno.yPos;
	*zPos = param.crtDyno.zPos;
	*xTan = param.crtDyno.xTan;
	*yTan = param.crtDyno.yTan;
	*zTan = param.crtDyno.zTan;
	*xLat = param.crtDyno.xLat;
	*yLat = param.crtDyno.yLat;
	*zLat = param.crtDyno.zLat;
}
void CCvedDistriMsgQ::delDynoParams(long* id_local)
{
	//todo: returns the delDyno parameters
	Param param = m_msgQ.front();
	ATLASSERT(param.def.evt == delDyno);
	*id_local = param.delDyno.id_local;
}

void CCvedDistriMsgQ::LocalDeleteDynObj( CVED::CDynObj* pObj)
{
	//todo: record a message for deleting dyn obj
	Param param;
	param.delDyno.evt = delDyno;
	param.delDyno.id_local = pObj->GetId();
	if (m_msgQ.full())
	{
		m_msgQ.resize(m_msgQ.size() + DELTA_BUFF_CNT);
	}
	m_msgQ.push_back(param);
	CCvedDistri::LocalDeleteDynObj(pObj);
}

CVED::CDynObj* CCvedDistriMsgQ::LocalCreateEDO(
										bool				own,
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
		param.crtDyno.evt = crtDyno;
		ATLASSERT(cName.length() <= MAX_NAME_LEN); //doesn't support long name
		strcpy(param.crtDyno.name, cName.c_str());
		param.crtDyno.id_local = obj->GetId();
		param.crtDyno.solId = cAttr.solId;
		param.crtDyno.xSize = cAttr.xSize;
		param.crtDyno.ySize = cAttr.ySize;
		param.crtDyno.zSize = cAttr.zSize;
		param.crtDyno.xPos = cpInitPos->m_x;
		param.crtDyno.yPos = cpInitPos->m_y;
		param.crtDyno.zPos = cpInitPos->m_z;
		param.crtDyno.xTan = cpInitTan->m_i;
		param.crtDyno.yTan = cpInitTan->m_j;
		param.crtDyno.zTan = cpInitTan->m_k;
		param.crtDyno.xLat = cpInitLat->m_i;
		param.crtDyno.yLat = cpInitLat->m_j;
		param.crtDyno.zLat = cpInitLat->m_k;
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
		param.crtDyno.evt = crtDyno;
		ATLASSERT(cName.length() <= MAX_NAME_LEN); //doesn't support long name
		strcpy(param.crtDyno.name, cName.c_str());
		param.crtDyno.id_local = obj->GetId();
		param.crtDyno.solId = cAttr.solId;
		param.crtDyno.xSize = cAttr.xSize;
		param.crtDyno.ySize = cAttr.ySize;
		param.crtDyno.zSize = cAttr.zSize;
		param.crtDyno.xPos = cpInitPos->m_x;
		param.crtDyno.yPos = cpInitPos->m_y;
		param.crtDyno.zPos = cpInitPos->m_z;
		param.crtDyno.xTan = cpInitTan->m_i;
		param.crtDyno.yTan = cpInitTan->m_j;
		param.crtDyno.zTan = cpInitTan->m_k;
		param.crtDyno.xLat = cpInitLat->m_i;
		param.crtDyno.yLat = cpInitLat->m_j;
		param.crtDyno.zLat = cpInitLat->m_k;
		if (m_msgQ.full())
		{
			m_msgQ.resize(m_msgQ.size() + DELTA_BUFF_CNT);
		}
		m_msgQ.push_back(param);
	}
	return obj;
}

void CCvedDistriMsgQ::PegPDOs()
{
	CVED::CCvedDistri::PegPDOs();
	CVED::CObjTypeMask maskPed;
	maskPed.Clear();
	maskPed.Set(eCV_AVATAR);
	maskPed.Set(eCV_EXTERNAL_AVATAR);

	cvTObj  *pO = BindObj(0);
	int i = 0;
	for (; i<cNUM_DYN_OBJS; i++, pO++) {
		if ( maskPed.Has(pO->type) )
		{
			CVED::CAvatarObj* avatar = static_cast<CVED::CAvatarObj*>(BindObjIdToClass2(i));
			int child_id = i;
			int parent_id = -1;
			if (NULL != avatar
				&& (parent_id = avatar->PegTo()) > 0)
			{
				Param param;
				param.pegPed.evt = pegPed;
				param.pegPed.id_parent = parent_id;
				param.pegPed.id_child = child_id;
				if (m_msgQ.full())
				{
					m_msgQ.resize(m_msgQ.size() + DELTA_BUFF_CNT);
				}
				m_msgQ.push_back(param);
			}
		}
	}
}


CVED::CDynObj* CCvedDistriMsgQ::LocalCreatePDO( bool 				own,
												const string&		cName,
												const cvTObjAttr&	cAttr,
												const CPoint3D*		cpInitPos,
												const CVector3D*	cpInitTan,
												const CVector3D*	cpInitLat)
{
	//todo: record a message for creating dyn obj
	cvEObjType type = (own ? eCV_EXTERNAL_AVATAR : eCV_AVATAR);
	CVED::CDynObj* obj = CCvedDistri::CreateDynObj(cName, type, cAttr, cpInitPos, cpInitTan, cpInitLat);
	if (NULL != obj)
	{
		Param param;
		param.crtPed.evt = crtPed;
		ATLASSERT(cName.length() <= MAX_NAME_LEN); //doesn't support long name
		strcpy(param.crtPed.name, cName.c_str());
		param.crtPed.id_local = obj->GetId();
		param.crtPed.solId = cAttr.solId;
		param.crtPed.xSize = cAttr.xSize;
		param.crtPed.ySize = cAttr.ySize;
		param.crtPed.zSize = cAttr.zSize;
		param.crtPed.xPos = cpInitPos->m_x;
		param.crtPed.yPos = cpInitPos->m_y;
		param.crtPed.zPos = cpInitPos->m_z;
		param.crtPed.xTan = cpInitTan->m_i;
		param.crtPed.yTan = cpInitTan->m_j;
		param.crtPed.zTan = cpInitTan->m_k;
		param.crtPed.xLat = cpInitLat->m_i;
		param.crtPed.yLat = cpInitLat->m_j;
		param.crtPed.zLat = cpInitLat->m_k;
		param.crtPed.nPart = CVED::CArtiJoints::GetNumParts();
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
	param.delPed.evt = delPed;
	param.delPed.id_local = pObj->GetId();
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
	ATLASSERT(param.def.evt == crtPed);
	*id_local = param.crtPed.id_local;
	name = param.crtPed.name;
	*solId = param.crtPed.solId;
	*xSize = param.crtPed.xSize;
	*ySize = param.crtPed.ySize;
	*zSize = param.crtPed.zSize;
	*xPos = param.crtPed.xPos;
	*yPos = param.crtPed.yPos;
	*zPos = param.crtPed.zPos;
	*xTan = param.crtPed.xTan;
	*yTan = param.crtPed.yTan;
	*zTan = param.crtPed.zTan;
	*xLat = param.crtPed.xLat;
	*yLat = param.crtPed.yLat;
	*zLat = param.crtPed.zLat;
	*nPart = param.crtPed.nPart;
}



void CCvedDistriMsgQ::delPedParams(long* id_local)
{
	//todo: returns the delDyno parameters
	Param param = m_msgQ.front();
	ATLASSERT(param.def.evt == delPed);
	*id_local = param.delPed.id_local;
}

void CCvedDistriMsgQ::pegPedParams(long* id_parent, long* id_child)
{
	Param param = m_msgQ.front();
	ATLASSERT(param.def.evt == pegPed);
	*id_parent = param.pegPed.id_parent;
	*id_child = param.pegPed.id_child;
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
	ATLASSERT( (		pO->type == eCV_VEHICLE
					|| pO->type == eCV_EXTERNAL_DRIVER
					|| pO->type == eCV_EXTERNAL_AVATAR
					|| pO->type == eCV_AVATAR
				)
			&& (		pO->phase == eALIVE
					|| pO->phase == eDYING
				)
			);

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

