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
	void delPedParams(long* id_local);
	void pegPedParams(long* id_parent, long* id_child);
	void telPedParams(LONG* id_local
					, double *xSize, double *ySize, double *zSize
					, double *xPos, double *yPos, double *zPos
					, double *xTan, double *yTan, double *zTan);

	void BindStateBuff(long id_local, cvTObjContInp** inp, cvTObjState** s);
protected:
	virtual CVED::CDynObj* LocalCreateEDO(
					bool				own,
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
	virtual void PegPDOs() override;
	virtual void LocalTeleportPDO(CVED::CAvatarObj* obj, const CPoint3D* pos, const CVector3D* tan, const CVector3D* lat) override;
private:
	union Param
	{
		struct ParamDef
		{
			EVT evt;
		} def;
		struct ParamCrtDyno
		{
			EVT evt;
			long id_local;
			char name[MAX_NAME_LEN];
			long solId;
			double xSize, ySize, zSize;
			double xPos, yPos, zPos;
			double xTan, yTan, zTan;
			double xLat, yLat, zLat;
		} crtDyno;
		struct ParamDelDyno
		{
			EVT evt;
			long id_local;
		} delDyno;
		struct ParamCrtPed
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
		} crtPed;
		struct ParamDelPed
		{
			EVT evt;
			long id_local;
		} delPed;
		struct ParamPegPed
		{
			EVT evt;
			long id_parent;
			long id_child;
		} pegPed;
		struct ParamTelPed
		{
			EVT evt;
			long id_local;
			double xPos, yPos, zPos;
			double xTan, yTan, zTan;
			double xLat, yLat, zLat;
		} telPed;
	};

	boost::circular_buffer<Param> m_msgQ;
};

