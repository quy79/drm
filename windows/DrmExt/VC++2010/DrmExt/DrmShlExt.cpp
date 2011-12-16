// DrmShlExt.cpp : CDrmShlExt ‚ÌŽÀ‘•
#pragma region Includes
#include "stdafx.h"
#include "DrmShlExt.h"
#include <strsafe.h>
#include <atlconv.h>
#pragma endregion

// CDrmShlExt

/////////////////////////////////////////////////////////////////////////////
// CDrmShlExt IShellExtInit methods.
// 

//
//   FUNCTION: CDrmShlExt::Initialize(LPCITEMIDLIST, LPDATAOBJECT, 
//             HKEY)
//
//   PURPOSE: Initializes the context menu extension.
//
IFACEMETHODIMP CDrmShlExt::Initialize(
    LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID)
{
    HRESULT hr = E_INVALIDARG;
    
    if (NULL == pDataObj)
    {
        return hr;
    }

    FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM stm;

    // pDataObj contains the objects being acted upon. In this project, 
    // we get an HDROP handle for enumerating the selected files.
    if (SUCCEEDED(pDataObj->GetData(&fe, &stm)))
    {
        // Get an HDROP handle.
        HDROP hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
        if (hDrop != NULL)
        {
            // Determine how many files are involved in this operation.
            UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
            if (nFiles != 0)
            {
                // Get the name of the first file.
                if (0 != DragQueryFile(hDrop, 0, m_szFileName, MAX_PATH))
                {
                    hr = S_OK;
                }

                // [-or-]
				//store the list of filenames in our list				
				wchar_t tmpFile[MAX_PATH]; //temporary string to hold a file
				for(UINT i = 0; i < nFiles; i++)
				{
					if (0 == DragQueryFile(hDrop, i, tmpFile, MAX_PATH))
						hr = E_INVALIDARG;
					else
						this->fileList.push_front(tmpFile);
				}
                //hr = S_OK;
            }

            GlobalUnlock(stm.hGlobal);
        }

        ReleaseStgMedium(&stm);
    }

    // If any value other than S_OK is returned from the method, the context 
    // menu is not displayed.
    return hr;
}


/////////////////////////////////////////////////////////////////////////////
// CDrmShlExt IContextMenu methods.
// 

#define IDM_DECRYPT			0			// The command's identifier offset. 
#define VERB_DECRYPTA		"Decrypt"	// The command's ANSI verb string
#define VERB_DECRYPTW		L"Decrypt"	// The command's Unicode verb string


//
//   FUNCTION: CDrmShlExt::OnDecrypt(HWND)
//
//   PURPOSE: OnDecrypt handles the "Decrypt" verb of the shell extension.
//
void CDrmShlExt::OnDecrypt(HWND hWnd)
{
    /*TCHAR szMessage[300];
    _stprintf_s(szMessage, 300, _T("The following files were successful decrypted:\n\n%s"), 
        m_szFileName); 

    MessageBox(hWnd, szMessage, _T("Rights Network File Decrypter"), 
        MB_ICONINFORMATION);
	*/
	//DWORD dwLastErr;
	std::wstring msg = L"The following files were successful decrypted :\n";
	string_list::iterator i;
	for(i=this->fileList.begin(); i!=this->fileList.end(); i++){
		//msg += (*i) + _T("\n");
	
		char *encFile = (char*)(*i).c_str();
		std::wstring decFile = (*i).substr(0, (*i).length() - 4) + L"\n";
		//std::wstring passkey = L"N67C9PpD,uqZRG(MxeQWzCdmzqezJGo8tnMk[4s(FpHkdWtY.t";
		std::wstring passkey = L"nothing";
		const char* keyStr = "nothing";
		DecryptFile(encFile, (char*)decFile.c_str(), keyStr);
		//dwLastErr = GetLastError();
		
		msg += decFile + L"\n";
		

	
	}
		

	MessageBox(0, msg.c_str(), _T("Rights Network File Decrypter"), 0);
	
}


//
//   FUNCTION: CDrmShlExt::QueryContextMenu(HMENU, UINT, UINT, UINT, 
//             UINT)
//
//   PURPOSE: The Shell calls IContextMenu::QueryContextMenu to allow the 
//            context menu handler to add its menu items to the menu. It 
//            passes in the HMENU handle in the hmenu parameter. The 
//            indexMenu parameter is set to the index to be used for the 
//            first menu item that is to be added.
//
IFACEMETHODIMP CDrmShlExt::QueryContextMenu(
    HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
    // If uFlags include CMF_DEFAULTONLY then we should not do anything
    if (CMF_DEFAULTONLY & uFlags)
    {
        return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
    }

	//unsigned long originalFirst = idCmdFirst; //we'll compute how many items we added from this.
	HBITMAP hBitmap = (HBITMAP)LoadImage((HMODULE)_AtlBaseModule.m_hInst,
		MAKEINTRESOURCE(IDB_PAGE_UNLOCK) , IMAGE_BITMAP, 16, 16, 0);

	InsertMenu(hMenu, indexMenu, MF_STRING | MF_BYPOSITION, idCmdFirst + IDM_DECRYPT, _T("&Decrypt"));
	SetMenuItemBitmaps(hMenu, indexMenu, MF_BITMAP | MF_BYPOSITION, hBitmap, NULL);
	indexMenu++; //this corresponds to the top-level menu index

	// Use either InsertMenu or InsertMenuItem tSo add menu items to the list
    //InsertMenu(hMenu, indexMenu, MF_STRING | MF_BYPOSITION, idCmdFirst + 
    //    IDM_DECRYPT, _T("&Decrypt"));

    // Return an HRESULT value with the severity set to SEVERITY_SUCCESS. 
    // Set the code value to the offset of the largest command identifier 
    // that was assigned, plus one (1)
    return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(IDM_DECRYPT + 1));
}


//
//   FUNCTION: CDrmShlExt::GetCommandString(UINT, UINT, LPUINT, 
//             LPSTR, UINT)
//
//   PURPOSE: If a user highlights one of the items added by a context menu 
//            handler, the handler's IContextMenu::GetCommandString method is 
//            called to request a Help text string that will be displayed on 
//            the Windows Explorer status bar. This method can also be called 
//            to request the verb string that is assigned to a command. 
//            Either ANSI or Unicode verb strings can be requested.
//
IFACEMETHODIMP CDrmShlExt::GetCommandString(
    UINT_PTR idCommand, UINT uFlags, LPUINT lpReserved, LPSTR pszName, 
    UINT uMaxNameLen)
{
    HRESULT hr = E_INVALIDARG;

    // For the command "&Decrypt" (IDM_DECRYPT)
    if (idCommand == IDM_DECRYPT)
    {
        switch (uFlags)
        {
        case GCS_HELPTEXTA:
            hr = StringCchCopyNA(pszName, 
                lstrlenA(pszName) / sizeof(pszName[0]), 
                "Rights Network File Decrypter", 
                uMaxNameLen);
            break;

        case GCS_HELPTEXTW:
            hr = StringCchCopyNW((LPWSTR)pszName, 
                lstrlenW((LPWSTR)pszName) / sizeof(pszName[0]), 
                L"Rights Network File Decrypter", 
                uMaxNameLen);
            break;

        case GCS_VERBA:
            hr = StringCchCopyNA(pszName, 
                lstrlenA(pszName) / sizeof(pszName[0]), 
                VERB_DECRYPTA, uMaxNameLen);
            break;

        case GCS_VERBW:
            hr = StringCchCopyNW((LPWSTR)pszName, 
                lstrlenW((LPWSTR)pszName) / sizeof(pszName[0]), 
                VERB_DECRYPTW, uMaxNameLen);
            break;

        default:
            hr = S_OK;
        }
    }

    // If the command (idCommand) is not supported by this context menu 
    // extension handler, return E_INVALIDARG.

    return hr;
}


//
//   FUNCTION: CDrmShlExt::InvokeCommand(LPCMINVOKECOMMANDINFO)
//
//   PURPOSE: This method is called when a user clicks a menu item to tell 
//            the handler to run the associated command. The lpcmi parameter 
//            points to a structure that contains the needed information.
//
IFACEMETHODIMP CDrmShlExt::InvokeCommand(LPCMINVOKECOMMANDINFO lpcmi)
{
    BOOL fEx = FALSE;
    BOOL fUnicode = FALSE;

    // Determines which structure is being passed in, CMINVOKECOMMANDINFO or 
    // CMINVOKECOMMANDINFOEX based on the cbSize member of lpcmi. Although 
    // the lpcmi parameter is declared in Shlobj.h as a CMINVOKECOMMANDINFO 
    // structure, in practice it often points to a CMINVOKECOMMANDINFOEX 
    // structure. This struct is an extended version of CMINVOKECOMMANDINFO 
    // and has additional members that allow Unicode strings to be passed.
    if (lpcmi->cbSize == sizeof(CMINVOKECOMMANDINFOEX))
    {
        fEx = TRUE;
        if((lpcmi->fMask & CMIC_MASK_UNICODE))
        {
            fUnicode = TRUE;
        }
    }

    // Determines whether the command is identified by its offset or verb.
    // There are two ways to identify commands:
    //   1) The command's verb string 
    //   2) The command's identifier offset
    // If the high-order word of lpcmi->lpVerb (for the ANSI case) or 
    // lpcmi->lpVerbW (for the Unicode case) is nonzero, lpVerb or lpVerbW 
    // holds a verb string. If the high-order word is zero, the command 
    // offset is in the low-order word of lpcmi->lpVerb.

    // For the ANSI case, if the high-order word is not zero, the command's 
    // verb string is in lpcmi->lpVerb. 
    if (!fUnicode && HIWORD(lpcmi->lpVerb))
    {
        // Is the verb supported by this context menu extension?
        if (StrCmpIA(lpcmi->lpVerb, VERB_DECRYPTA) == 0)
        {
            OnDecrypt(lpcmi->hwnd);
        }
        else
        {
            // If the verb is not recognized by the context menu handler, it 
            // must return E_FAIL to allow it to be passed on to the other 
            // context menu handlers that might implement that verb.
            return E_FAIL;
        }
    }

    // For the Unicode case, if the high-order word is not zero, the 
    // command's verb string is in lpcmi->lpVerbW. 
    else if(fUnicode && HIWORD(((CMINVOKECOMMANDINFOEX*)lpcmi)->lpVerbW))
    {
        // Is the verb supported by this context menu extension?
        if (StrCmpIW(((CMINVOKECOMMANDINFOEX*)lpcmi)->lpVerbW, 
            VERB_DECRYPTW) == 0)
        {
            OnDecrypt(lpcmi->hwnd);
        }
        else
        {
            // If the verb is not recognized by the context menu handler, it 
            // must return E_FAIL to allow it to be passed on to the other 
            // context menu handlers that might implement that verb.
            return E_FAIL;
        }
    }

    // If the command cannot be identified through the verb string, then 
    // check the identifier offset.
    else
    {
        // Is the command identifier offset supported by this context menu 
        // extension?
        if (LOWORD(lpcmi->lpVerb) == IDM_DECRYPT)
        {
            OnDecrypt(lpcmi->hwnd);
        }
        else
        {
            // If the verb is not recognized by the context menu handler, it 
            // must return E_FAIL to allow it to be passed on to the other 
            // context menu handlers that might implement that verb.
            return E_FAIL;
        }
    }

    return S_OK;
}

void EncryptFile(const char *in, const char *out, const char *passPhrase)
{
	FileSource f(in, true, new DefaultEncryptorWithMAC(passPhrase, new FileSink(out)));	
}

void DecryptFile(const char *in, const char *out, const char *passPhrase)
{
	FileSource f(in, true, new DefaultDecryptorWithMAC(passPhrase, new FileSink(out)));
}


