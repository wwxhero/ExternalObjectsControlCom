// DistriObjsCtrl.h : Declaration of the CDistriObjsCtrl

#pragma once
#include "resource.h"       // main symbols



#include "ExternalObjectsControlCom_i.h"

#include "LibExternalObjectIfNetwork.h"
#include "CvedDistriMsgQ.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CDistriObjsCtrl

class ATL_NO_VTABLE CDistriObjsCtrl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDistriObjsCtrl, &CLSID_DistriObjsCtrl>,
	public IDispatchImpl<IDistriObjsCtrl, &IID_IDistriObjsCtrl, &LIBID_ExternalObjectsControlComLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CDistriObjsCtrl() : m_pExternalCtrl(NULL)
					  , m_pCvedMsgQ(NULL)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DISTRIOBJSCTRL)


BEGIN_COM_MAP(CDistriObjsCtrl)
	COM_INTERFACE_ENTRY(IDistriObjsCtrl)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		if (NULL != m_pExternalCtrl)
		{
			::ReleaseNetworkExternalObjectControl(m_pExternalCtrl);
		}
		m_pExternalCtrl = NULL;
	}

public:
	STDMETHOD(CreateNetworkExternalObjectControl)(LONG imple, LONG terminal);
	STDMETHOD(ReleaseNetworkExternalObjectControl)(void);
	STDMETHOD(Initialize)(BSTR pathScene);
	STDMETHOD(UnInitialize)(void);
	STDMETHOD(QFrontEvent)(EVT* evt, VARIANT_BOOL* empty);
	STDMETHOD(QPopEvent)(void);
	STDMETHOD(GetcrtDynoTuple)(LONG *id_local, BSTR *name, LONG *solId,
							DOUBLE *xSize, DOUBLE *ySize, DOUBLE *zSize,
							DOUBLE *xPos, DOUBLE *yPos, DOUBLE *zPos,
							DOUBLE *xTan, DOUBLE *yTan, DOUBLE *zTan,
							DOUBLE *xLat, DOUBLE *yLat, DOUBLE *zLat);
	STDMETHOD(GetdelDynoTuple)(LONG *id_local);
	STDMETHOD(GetcrtPedTuple)(LONG *id_local, BSTR *name, LONG *solId,
							DOUBLE *xSize, DOUBLE *ySize, DOUBLE *zSize,
							DOUBLE *xPos, DOUBLE *yPos, DOUBLE *zPos,
							DOUBLE *xTan, DOUBLE *yTan, DOUBLE *zTan,
							DOUBLE *xLat, DOUBLE *yLat, DOUBLE *zLat,
							LONG *nParts);
	STDMETHOD(GetdelPedTuple)(LONG *id_local);
	STDMETHOD(GettelPedTuple)(LONG *id_local,
							DOUBLE *xPos, DOUBLE *yPos, DOUBLE *zPos,
							DOUBLE *xTan, DOUBLE *yTan, DOUBLE *zTan,
							DOUBLE *xLat, DOUBLE *yLat, DOUBLE *zLat);
	STDMETHOD(GetpegPedTuple)(LONG *id_parent, LONG *id_child);
	STDMETHOD(PreUpdateDynamicModels)(void);
	STDMETHOD(PostUpdateDynamicModels)(void);
	STDMETHOD(OnGetUpdate)(LONG id_local, VARIANT_BOOL *received
						, DOUBLE *xPos, DOUBLE *yPos, DOUBLE *zPos
						, DOUBLE *xTan, DOUBLE *yTan, DOUBLE *zTan
						, DOUBLE *xLat, DOUBLE *yLat, DOUBLE *zLat);
	STDMETHOD(GetcrtPedPartName)(LONG id_local
							, LONG id_part
							, BSTR* name_part);
	STDMETHOD(OnPreGetUpdateArt)(LONG id_local, VARIANT_BOOL *received
						, DOUBLE *xPos, DOUBLE *yPos, DOUBLE *zPos
						, DOUBLE *xTan, DOUBLE *yTan, DOUBLE *zTan
						, DOUBLE *xLat, DOUBLE *yLat, DOUBLE *zLat);
	STDMETHOD(OnGetUpdateArt)(LONG id_local, LONG id_part
							, DOUBLE* w, DOUBLE* x, DOUBLE* y, DOUBLE* z
							, DOUBLE* dx, DOUBLE* dy, DOUBLE* dz);
	STDMETHOD(OnGetUpdateArtDIGUY)(LONG id_local, FLOAT* oris, FLOAT* trans);
	STDMETHOD(OnPushUpdateArt)(LONG id_local, LONG id_part
							, DOUBLE w, DOUBLE x, DOUBLE y, DOUBLE z
							, DOUBLE dx, DOUBLE dy, DOUBLE dz);
	STDMETHOD(OnPostPushUpdateArt)(LONG id_local
						, DOUBLE xPos, DOUBLE yPos, DOUBLE zPos
						, DOUBLE xTan, DOUBLE yTan, DOUBLE zTan
						, DOUBLE xLat, DOUBLE yLat, DOUBLE zLat);
private:
	CCvedDistriMsgQ* m_pCvedMsgQ;
	CVED::IExternalObjectControl* m_pExternalCtrl;
	typedef struct
	{
		const char** names;
		TVector3D* angles;
		TVector3D* offsets;
		unsigned int num;
	} Joints;
	std::map<LONG, Joints> m_idLocal2jointAngles; //fixme: to be optimized to O(1) access time
};

OBJECT_ENTRY_AUTO(__uuidof(DistriObjsCtrl), CDistriObjsCtrl)
