// DistriObjsCtrl.h : Declaration of the CDistriObjsCtrl

#pragma once
#include "resource.h"       // main symbols



#include "ExternalObjectsControlCom_i.h"

#include "LibExternalObjectIfNetwork.h"


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
			::ReleaseNetworkExternalObjectControl(m_pExternalCtrl);
		m_pExternalCtrl = NULL;
	}

public:
	STDMETHOD(CreateNetworkExternalObjectControl)(LONG imple, LONG terminal);
	STDMETHOD(ReleaseNetworkExternalObjectControl)(void);
private:
	CVED::IExternalObjectControl* m_pExternalCtrl;
public:
	STDMETHOD(Initialize)(BSTR pathScene);
};

OBJECT_ENTRY_AUTO(__uuidof(DistriObjsCtrl), CDistriObjsCtrl)
