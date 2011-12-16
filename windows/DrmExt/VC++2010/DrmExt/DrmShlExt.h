// DrmShlExt.h : CDrmShlExt の宣言

#pragma once
#include "resource.h"       // メイン シンボル
#include "DrmExt_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "DCOM の完全サポートを含んでいない Windows Mobile プラットフォームのような Windows CE プラットフォームでは、単一スレッド COM オブジェクトは正しくサポートされていません。ATL が単一スレッド COM オブジェクトの作成をサポートすること、およびその単一スレッド COM オブジェクトの実装の使用を許可することを強制するには、_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA を定義してください。ご使用の rgs ファイルのスレッド モデルは 'Free' に設定されており、DCOM Windows CE 以外のプラットフォームでサポートされる唯一のスレッド モデルと設定されていました。"
#endif

using namespace ATL;


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

    /*!
    * Good Practice:
    * 
    * IFACEMETHODIMP is used instead of STDMETHODIMP for the COM interface 
    * method impelmetnations. IFACEMETHODIMP includes "__override" that lets 
    * SAL check that you are overriding a method, so this should be used for 
    * all COM interface method implementations.
    */

    // IShellExtInit
    IFACEMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

    // IContextMenu
    IFACEMETHODIMP GetCommandString(UINT_PTR, UINT, UINT*, LPSTR, UINT);
    IFACEMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO);
    IFACEMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT);

protected:
	
	//list of selected files
	string_list fileList;

    // The name of the first selected file
    TCHAR m_szFileName[MAX_PATH];

    // The function that handles the "Decrypt" verb
    void OnDecrypt(HWND hWnd);



};

OBJECT_ENTRY_AUTO(__uuidof(DrmShlExt), CDrmShlExt)
