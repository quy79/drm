// DrmShlExt.h : Declaration of the CDrmShlExt

#pragma once
#include "resource.h"       // main symbols

#include "DrmExt.h"

#define CRYPTOPP_DEFAULT_NO_DLL
// Crypto++ Includes
#include "cryptopp/config.h"
#include "cryptopp/hex.h"
#include "cryptopp/default.h"
#include "cryptopp/files.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/modes.h"
#include "cryptopp/osrng.h"
#include "cryptopp/filters.h"
#include "cryptopp/aes.h"
#include "cryptopp/des.h"
#include "cryptopp/blowfish.h" 

using namespace CryptoPP;

void EncryptFile(const char *in, const char *out, const char *passPhrase);
void DecryptFile(const char *in, const char *out, const char *passPhrase);

//We'll use a list of strings to store the filenames selected:
#include <string>
#include <list>
typedef std::list<std::wstring> string_list;


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CDrmShlExt

class ATL_NO_VTABLE CDrmShlExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDrmShlExt, &CLSID_DrmShlExt>,
	public IShellExtInit,
	public IContextMenu

{
public:
	CDrmShlExt()
	{
	}

	~CDrmShlExt()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DRMSHLEXT)

DECLARE_NOT_AGGREGATABLE(CDrmShlExt)

BEGIN_COM_MAP(CDrmShlExt)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	//for IShellExtInit:
	STDMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

	//for IContextMenu:
	STDMETHODIMP GetCommandString(UINT_PTR, UINT, UINT*, LPSTR, UINT cchMax);
	STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO);
	STDMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT);
protected:
	string_list fileList;

};

OBJECT_ENTRY_AUTO(__uuidof(DrmShlExt), CDrmShlExt)
