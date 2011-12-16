// dllmain.h : モジュール クラスの宣言

class CDrmExtModule : public ATL::CAtlDllModuleT< CDrmExtModule >
{
public :
	DECLARE_LIBID(LIBID_DrmExtLib)
	//DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DRMEXT, "{5964F1DE-2A38-4A87-AE03-D24C0C8B4488}")
};

extern class CDrmExtModule _AtlModule;
