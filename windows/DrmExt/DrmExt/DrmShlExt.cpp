// DrmShlExt.cpp : Implementation of CDrmShlExt

#include "stdafx.h"
#include "DrmShlExt.h"


// CDrmShlExt

STDMETHODIMP CDrmShlExt::Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID)
{
	FORMATETC fmt = {CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
	STGMEDIUM stg = {TYMED_HGLOBAL};
	HDROP hDrop;
	//Look for CF_HDROP data in the data object. If there
	//is no such data, return an error back to Explorer.

	if(FAILED(pDataObj->GetData(&fmt,&stg)))
		return E_INVALIDARG;
 
	//Get a pointer to the actual data.
	hDrop = (HDROP)GlobalLock(stg.hGlobal);

	//Make sure it worked.
	if(hDrop == 0)
		return E_INVALIDARG;

	UINT uNumFiles = DragQueryFile(hDrop,0xFFFFFFFF,NULL,0);
	HRESULT hr = S_OK;

	if(uNumFiles == 0)
	{
		GlobalUnlock(stg.hGlobal);
		ReleaseStgMedium(&stg);
		return E_INVALIDARG;
	}

	//store the list of filenames in our list
	unsigned long i;
	wchar_t tmpFile[MAX_PATH]; //temporary string to hold a file
	for(i=0; i<uNumFiles; i++)
	{
		if(DragQueryFile(hDrop,i,tmpFile,MAX_PATH) == 0)
			hr = E_INVALIDARG;
		else
			this->fileList.push_front(tmpFile);
	}

	GlobalUnlock(stg.hGlobal);
	ReleaseStgMedium(&stg);

	//return hr;
	return S_OK;
}

HRESULT CDrmShlExt::QueryContextMenu(HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd, UINT uidLastCmd, UINT uFlags)
{

	if(uFlags & CMF_DEFAULTONLY)
		return MAKE_HRESULT(SEVERITY_SUCCESS,FACILITY_NULL,0);


	unsigned long originalFirst = uidFirstCmd; //we'll compute how many items we added from this.
	HBITMAP hBitmap = (HBITMAP)LoadImage((HMODULE)_AtlBaseModule.m_hInst,
		MAKEINTRESOURCE(IDB_PAGE_UNLOCK) , IMAGE_BITMAP, 16, 16, 0);

	InsertMenu(hmenu,uMenuIndex,MF_BYPOSITION,uidFirstCmd++,L"Decrypt...");
	SetMenuItemBitmaps(hmenu, uMenuIndex, MF_BITMAP | MF_BYPOSITION, hBitmap, NULL);
	uMenuIndex++; //this corresponds to the top-level menu index
	
	
	///////////////////////////////////////////////////////////////////////////////////
	//if you just increment the values, return the number of added items.
	//return MAKE_HRESULT(SEVERITY_SUCCESS,FACILITY_NULL,uidFirstCmd-originalFirst);


	//sub-menus:

	////MENUITEMINFO mii;
	////HMENU hm = CreatePopupMenu();
	//now get the recent contacts and add items for each:

	////AppendMenu(hm,MF_STRING,uidFirstCmd++,L"Decrypt");
	////AppendMenu(hm,MF_STRING,uidFirstCmd++,L"About...");
	//AppendMenu(hm,MF_STRING,uidFirstCmd++,L"the third menu");

	//mii.cbSize = sizeof(MENUITEMINFO);
	//mii.fMask = MIIM_TYPE | MIIM_SUBMENU;
	//mii.fType = MFT_STRING;
	//mii.hSubMenu = hm;
	//mii.dwTypeData = L"DRM Decrypter...";
	//mii.cch = (unsigned int)strlen("DRM Decrypter...");


	//HBITMAP hBitmap = (HBITMAP)LoadImage((HMODULE)_AtlBaseModule.m_hInst,
	//	MAKEINTRESOURCE(IDB_PAGE_UNLOCK) , IMAGE_BITMAP, 24, 24, 0);

	//InsertMenuItem(hmenu,uMenuIndex++,MF_STRING | MF_BYPOSITION,&mii);
	//SetMenuItemBitmaps(hmenu, uMenuIndex, MF_BITMAP | MF_BYCOMMAND, hBitmap, NULL);
	

	//if you have fancy bitmaps, you would set them here, something like this:
	//if(NULL != this->m_hHandBmp)
	//	SetMenuItemBitmaps(hmenu,uMenuIndex,MF_BYPOSITION,m_hHandBmp,NULL);
	///////////////////////////////////////////////////////////////////////////////////

	//You could also add other tp level menu items here...

	//The return value seems to tell the shell how many items with commands we've added (leaf items).
	return MAKE_HRESULT(SEVERITY_SUCCESS,FACILITY_NULL,uidFirstCmd-originalFirst);
}


HRESULT CDrmShlExt::GetCommandString(UINT_PTR idCmd, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax)
{
	//If Explorer is asking for a help string, copy our string into the supplied buffer:
	wchar_t hlpText[256]; 
	bool idMatch = false; 

	if(uFlags & GCS_HELPTEXT)
	{
		if(idCmd < 2) 
		{
			idMatch = true;
			if(idCmd == 0)
				::wcscpy(hlpText,L"Decrypt the DRM files");
			else if(idCmd == 1)
				::wcscpy(hlpText,L"About the DRM Decrypter");
		}

		if(!idMatch) 
			return E_INVALIDARG;

		if(uFlags & GCS_UNICODE) //pszName is actually wchars, not chars.
			::wcsncpy(reinterpret_cast<wchar_t*>(pszName),hlpText,cchMax);
		else
			::wcstombs(pszName,hlpText,cchMax);

		return S_OK;
	}

	return E_INVALIDARG;
}

HRESULT CDrmShlExt::InvokeCommand(LPCMINVOKECOMMANDINFO pCmdInfo)
{

	if(0 != HIWORD(pCmdInfo->lpVerb))
		return E_INVALIDARG;

	//Note that the command index appears to be relative to items we added rather than absolute positions.
	unsigned long idCmd = LOWORD(pCmdInfo->lpVerb);

	if(idCmd < 3)
	{
		wchar_t digits[32];
		::_ltow(idCmd+1,digits,10);
		std::wstring msg = L"The following files were successful decrypted :\n";
		//msg += digits;
		//msg += L" on the following files:\n";
		string_list::iterator i;
		//for(i=this->fileList.begin(); i!=this->fileList.end(); i++)
		//	msg += (*i) + L"\n";
		for(i=this->fileList.begin(); i!=this->fileList.end(); i++){
			//decrypt handle
			//output decrypted file		
			char *encFile = (char*)(*i).c_str();
			std::wstring passkey = L"N67C9PpD,uqZRG(MxeQWzCdmzqezJGo8tnMk[4s(FpHkdWtY.t";
			std::wstring decFile;	
			decFile = (*i).substr(0, (*i).length() - 4);
			DecryptFile(encFile, (char*)decFile.c_str(), (char*)passkey.c_str());
			::MessageBox(0,L"File was successful decrypted !", L"Rights Network Decrypter",0);
			return S_OK;
			
		}

		::MessageBox(0,msg.c_str(),L"Rights Network Decrypter",0);
		return S_OK;
	}

	return E_INVALIDARG;
}

void EncryptFile(const char *in, const char *out, const char *passPhrase)
{
	FileSource f(in, true, new DefaultEncryptorWithMAC(passPhrase, new FileSink(out)));	
}

void DecryptFile(const char *in, const char *out, const char *passPhrase)
{
	FileSource f(in, true, new DefaultDecryptorWithMAC(passPhrase, new FileSink(out)));
}