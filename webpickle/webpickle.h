/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sat Sep 23 17:56:42 2006
 */
/* Compiler settings for C:\AbInition\hs\webpickle\webpickle.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __webpickle_h__
#define __webpickle_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ICDHTML_FWD_DEFINED__
#define __ICDHTML_FWD_DEFINED__
typedef interface ICDHTML ICDHTML;
#endif 	/* __ICDHTML_FWD_DEFINED__ */


#ifndef __ICDHTMLUI_FWD_DEFINED__
#define __ICDHTMLUI_FWD_DEFINED__
typedef interface ICDHTMLUI ICDHTMLUI;
#endif 	/* __ICDHTMLUI_FWD_DEFINED__ */


#ifndef __CDHTML_FWD_DEFINED__
#define __CDHTML_FWD_DEFINED__

#ifdef __cplusplus
typedef class CDHTML CDHTML;
#else
typedef struct CDHTML CDHTML;
#endif /* __cplusplus */

#endif 	/* __CDHTML_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __ICDHTML_INTERFACE_DEFINED__
#define __ICDHTML_INTERFACE_DEFINED__

/* interface ICDHTML */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICDHTML;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("734BBD92-9922-4BA4-96D5-5D2A16F479F7")
    ICDHTML : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ICDHTMLVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICDHTML __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICDHTML __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICDHTML __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICDHTML __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICDHTML __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICDHTML __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICDHTML __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } ICDHTMLVtbl;

    interface ICDHTML
    {
        CONST_VTBL struct ICDHTMLVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICDHTML_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICDHTML_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICDHTML_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICDHTML_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICDHTML_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICDHTML_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICDHTML_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICDHTML_INTERFACE_DEFINED__ */


#ifndef __ICDHTMLUI_INTERFACE_DEFINED__
#define __ICDHTMLUI_INTERFACE_DEFINED__

/* interface ICDHTMLUI */
/* [unique][helpstring][uuid][dual][object] */ 


EXTERN_C const IID IID_ICDHTMLUI;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D80E41FB-FC35-4E35-A34C-2EA2E930D07C")
    ICDHTMLUI : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnClick( 
            /* [in] */ IDispatch __RPC_FAR *pdispBody,
            /* [in] */ VARIANT varColor) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICDHTMLUIVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICDHTMLUI __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICDHTMLUI __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICDHTMLUI __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICDHTMLUI __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICDHTMLUI __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICDHTMLUI __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICDHTMLUI __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnClick )( 
            ICDHTMLUI __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pdispBody,
            /* [in] */ VARIANT varColor);
        
        END_INTERFACE
    } ICDHTMLUIVtbl;

    interface ICDHTMLUI
    {
        CONST_VTBL struct ICDHTMLUIVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICDHTMLUI_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICDHTMLUI_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICDHTMLUI_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICDHTMLUI_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICDHTMLUI_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICDHTMLUI_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICDHTMLUI_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICDHTMLUI_OnClick(This,pdispBody,varColor)	\
    (This)->lpVtbl -> OnClick(This,pdispBody,varColor)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ICDHTMLUI_OnClick_Proxy( 
    ICDHTMLUI __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pdispBody,
    /* [in] */ VARIANT varColor);


void __RPC_STUB ICDHTMLUI_OnClick_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICDHTMLUI_INTERFACE_DEFINED__ */



#ifndef __WEBPICKLELib_LIBRARY_DEFINED__
#define __WEBPICKLELib_LIBRARY_DEFINED__

/* library WEBPICKLELib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_WEBPICKLELib;

EXTERN_C const CLSID CLSID_CDHTML;

#ifdef __cplusplus

class DECLSPEC_UUID("FB3F382A-602E-4590-BB74-38795644314E")
CDHTML;
#endif
#endif /* __WEBPICKLELib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
