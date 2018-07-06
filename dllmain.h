// dllmain.h : Declaration of module class.

class CExternalObjectsControlComModule : public ATL::CAtlDllModuleT< CExternalObjectsControlComModule >
{
public :
	DECLARE_LIBID(LIBID_ExternalObjectsControlComLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_EXTERNALOBJECTSCONTROLCOM, "{D566DFEF-343F-4DD3-8D79-F83CBB24E248}")
};

extern class CExternalObjectsControlComModule _AtlModule;
