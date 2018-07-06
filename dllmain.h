// dllmain.h : Declaration of module class.

class CExternalObjectsControlComModule : public ATL::CAtlDllModuleT< CExternalObjectsControlComModule >
{
public :
	DECLARE_LIBID(LIBID_ExternalObjectsControlComLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_EXTERNALOBJECTSCONTROLCOM, "{D566DFEF-343F-4DD3-8D79-F83CBB24E248}")
	CExternalObjectsControlComModule()
	{
		m_hEventSource = RegisterEventSource(NULL, c_logSrc);
	}
	virtual ~CExternalObjectsControlComModule()
	{
		DeregisterEventSource(m_hEventSource);
	}
	void LogEventEx(
		_In_ int id,
		_In_opt_z_ LPCTSTR pszMessage=NULL,
		_In_ WORD type = EVENTLOG_INFORMATION_TYPE) throw()
	{
			/* Get a handle to use with ReportEvent(). */
			if (m_hEventSource != NULL)
			{
				/* Write to event log. */
				ReportEvent(m_hEventSource,
							type,
							(WORD)0,
							id,
							NULL,
							(WORD)(pszMessage != NULL ? 1 : 0),
							0,
							pszMessage != NULL ? &pszMessage : NULL,
							NULL);
			}
	}
private:
	HANDLE m_hEventSource;
	static const TCHAR c_logSrc[];
};

extern class CExternalObjectsControlComModule _AtlModule;
