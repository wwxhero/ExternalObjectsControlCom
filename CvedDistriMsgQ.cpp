#include "StdAfx.h"
#include "CvedDistriMsgQ.h"


CCvedDistriMsgQ::CCvedDistriMsgQ(CVED::IExternalObjectControl* pCtrl) : CVED::CCvedDistri(pCtrl)
{
}


CCvedDistriMsgQ::~CCvedDistriMsgQ(void)
{
}

bool CCvedDistriMsgQ::empty() const
{
	//todo: report if queue is empty
	return true;
}
EVT CCvedDistriMsgQ::front() const
{
	//todo: the front element in the queue
	return evtUndefined;
}
void CCvedDistriMsgQ::pop()
{
	//todo: pops front element
}
void CCvedDistriMsgQ::crtDynoParams(long* id_local, std::string& name, long* soldId
				, double *xSize, double *ySize, double *zSize
				, double *xPos, double *yPos, double *zPos
				, double *xTan, double *yTan, double *zTan
				, double *xLat, double *yLat, double *zLat)
{
	//todo: returns the crtDyno parameters
}
void CCvedDistriMsgQ::delDynoParams(long* id_local)
{
	//todo: returns the delDyno parameters
}

void CCvedDistriMsgQ::LocalDeleteDynObj( CVED::CDynObj* pObj)
{
	//todo: record a message for deleting dyn obj
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
	return CCvedDistri::LocalCreateDynObj(cName, type, cAttr, cpInitPos, cpInitTan, cpInitLat);
}
