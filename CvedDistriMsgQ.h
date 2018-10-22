#pragma once
#include <boost/circular_buffer.hpp>

#include "ExternalObjectsControlCom_i.h"
#include "cveddistri.h"
#define MAX_NAME_LEN 64
class CCvedDistriMsgQ :
	public CVED::CCvedDistri
{
public:
	CCvedDistriMsgQ(CVED::IExternalObjectControl* pCtrl);
	virtual ~CCvedDistriMsgQ(void);
	bool empty() const;
	EVT front() const;
	void pop();
	void crtDynoParams(long* id_local, std::string& name, long* soldId
					, double *xSize, double *ySize, double *zSize
					, double *xPos, double *yPos, double *zPos
					, double *xTan, double *yTan, double *zTan
					, double *xLat, double *yLat, double *zLat);
	void delDynoParams(long* id_local);
	void crtPedParams(long* id_local, std::string& name, long* soldId
					, double *xSize, double *ySize, double *zSize
					, double *xPos, double *yPos, double *zPos
					, double *xTan, double *yTan, double *zTan
					, double *xLat, double *yLat, double *zLat
					, long *nPart);
	void crtPedPartName(long id_local, long id_part, std::string& name);
	void delPedParams(long* id_local);

	void BindStateBuff(long id_local, cvTObjContInp** inp, cvTObjState** s);
protected:
	virtual CVED::CDynObj* LocalCreateEDO(
					const string&		cName,
					const cvTObjAttr&	cAttr,
					const CPoint3D*		cpInitPos=0,
					const CVector3D*	cpInitTan=0,
					const CVector3D*	cpInitLat=0);
	virtual CVED::CDynObj* LocalCreateADO(
					const string&		cName,
					const cvTObjAttr&	cAttr,
					const CPoint3D*		cpInitPos=0,
					const CVector3D*	cpInitTan=0,
					const CVector3D*	cpInitLat=0);
	virtual void LocalDeleteDynObj( CVED::CDynObj* );

	virtual CVED::CDynObj*	DistriCreateADO(const string&		cName,
								const cvTObjAttr&	cAttr,
								const CPoint3D*		cpInitPos=0,
								const CVector3D*	cpInitTan=0,
								const CVector3D*	cpInitLat=0);

	virtual void		DistriDeleteADO( CVED::CDynObj* );

	virtual CVED::CDynObj* LocalCreatePDO(
					bool				own,
					const string&		cName,
					const cvTObjAttr&	cAttr,
					const CPoint3D*		cpInitPos=0,
					const CVector3D*	cpInitTan=0,
					const CVector3D*	cpInitLat=0);

	virtual void LocalDeletePDO(CVED::CDynObj* );
private:
	union Param
	{
		struct evtDef
		{
			EVT evt;
		} ParamDef;
		struct crtDyno
		{
			EVT evt;
			long id_local;
			char name[MAX_NAME_LEN];
			long solId;
			double xSize, ySize, zSize;
			double xPos, yPos, zPos;
			double xTan, yTan, zTan;
			double xLat, yLat, zLat;
		} ParamCrtDyno;
		struct delDyno
		{
			EVT evt;
			long id_local;
		} ParamDelDyno;
		struct crtPed
		{
			EVT evt;
			long id_local;
			char name[MAX_NAME_LEN];
			long solId;
			double xSize, ySize, zSize;
			double xPos, yPos, zPos;
			double xTan, yTan, zTan;
			double xLat, yLat, zLat;
			unsigned int nPart;
			const char** partNames;
		} ParamCrtPed;
		struct delPed
		{
			EVT evt;
			long id_local;
		} ParamDelPed;
	};

	boost::circular_buffer<Param> m_msgQ;
};

