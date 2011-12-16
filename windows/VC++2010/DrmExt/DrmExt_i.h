

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Dec 16 17:18:28 2011
 */
/* Compiler settings for DrmExt.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __DrmExt_i_h__
#define __DrmExt_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDrmShlExt_FWD_DEFINED__
#define __IDrmShlExt_FWD_DEFINED__
typedef interface IDrmShlExt IDrmShlExt;
#endif 	/* __IDrmShlExt_FWD_DEFINED__ */


#ifndef __DrmShlExt_FWD_DEFINED__
#define __DrmShlExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class DrmShlExt DrmShlExt;
#else
typedef struct DrmShlExt DrmShlExt;
#endif /* __cplusplus */

#endif 	/* __DrmShlExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IDrmShlExt_INTERFACE_DEFINED__
#define __IDrmShlExt_INTERFACE_DEFINED__

/* interface IDrmShlExt */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDrmShlExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7AE4C307-30FB-4129-B87F-4CB6E3B256ED")
    IDrmShlExt : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDrmShlExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDrmShlExt * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDrmShlExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDrmShlExt * This);
        
        END_INTERFACE
    } IDrmShlExtVtbl;

    interface IDrmShlExt
    {
        CONST_VTBL struct IDrmShlExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDrmShlExt_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDrmShlExt_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDrmShlExt_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDrmShlExt_INTERFACE_DEFINED__ */



#ifndef __DrmExtLib_LIBRARY_DEFINED__
#define __DrmExtLib_LIBRARY_DEFINED__

/* library DrmExtLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_DrmExtLib;

EXTERN_C const CLSID CLSID_DrmShlExt;

#ifdef __cplusplus

class DECLSPEC_UUID("9F3119E3-1615-4036-AB7E-77F8DDC5DD08")
DrmShlExt;
#endif
#endif /* __DrmExtLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


