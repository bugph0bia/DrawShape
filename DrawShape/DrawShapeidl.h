

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for DrawShape.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __DrawShapeidl_h__
#define __DrawShapeidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DDrawShape_FWD_DEFINED__
#define ___DDrawShape_FWD_DEFINED__
typedef interface _DDrawShape _DDrawShape;

#endif 	/* ___DDrawShape_FWD_DEFINED__ */


#ifndef ___DDrawShapeEvents_FWD_DEFINED__
#define ___DDrawShapeEvents_FWD_DEFINED__
typedef interface _DDrawShapeEvents _DDrawShapeEvents;

#endif 	/* ___DDrawShapeEvents_FWD_DEFINED__ */


#ifndef __DrawShape_FWD_DEFINED__
#define __DrawShape_FWD_DEFINED__

#ifdef __cplusplus
typedef class DrawShape DrawShape;
#else
typedef struct DrawShape DrawShape;
#endif /* __cplusplus */

#endif 	/* __DrawShape_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_DrawShape_0000_0000 */
/* [local] */ 

#pragma warning(push)
#pragma warning(disable:4001) 
#pragma once
#pragma warning(push)
#pragma warning(disable:4001) 
#pragma once
#pragma warning(pop)
#pragma warning(pop)
#pragma region Desktop Family
#pragma endregion


extern RPC_IF_HANDLE __MIDL_itf_DrawShape_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DrawShape_0000_0000_v0_0_s_ifspec;


#ifndef __DrawShapeLib_LIBRARY_DEFINED__
#define __DrawShapeLib_LIBRARY_DEFINED__

/* library DrawShapeLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_DrawShapeLib;

#ifndef ___DDrawShape_DISPINTERFACE_DEFINED__
#define ___DDrawShape_DISPINTERFACE_DEFINED__

/* dispinterface _DDrawShape */
/* [uuid] */ 


EXTERN_C const IID DIID__DDrawShape;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("9987a44e-72fb-4e9a-a47f-936fcfe8db17")
    _DDrawShape : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DDrawShapeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DDrawShape * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DDrawShape * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DDrawShape * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DDrawShape * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DDrawShape * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DDrawShape * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DDrawShape * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _DDrawShapeVtbl;

    interface _DDrawShape
    {
        CONST_VTBL struct _DDrawShapeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DDrawShape_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DDrawShape_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DDrawShape_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DDrawShape_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DDrawShape_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DDrawShape_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DDrawShape_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DDrawShape_DISPINTERFACE_DEFINED__ */


#ifndef ___DDrawShapeEvents_DISPINTERFACE_DEFINED__
#define ___DDrawShapeEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DDrawShapeEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DDrawShapeEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("21a2e7b0-8b57-408c-bd42-9a7496d43d82")
    _DDrawShapeEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DDrawShapeEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DDrawShapeEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DDrawShapeEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DDrawShapeEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DDrawShapeEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DDrawShapeEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DDrawShapeEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DDrawShapeEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _DDrawShapeEventsVtbl;

    interface _DDrawShapeEvents
    {
        CONST_VTBL struct _DDrawShapeEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DDrawShapeEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DDrawShapeEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DDrawShapeEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DDrawShapeEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DDrawShapeEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DDrawShapeEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DDrawShapeEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DDrawShapeEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_DrawShape;

#ifdef __cplusplus

class DECLSPEC_UUID("92b77edb-2984-4ca7-b136-e337a67cc9bb")
DrawShape;
#endif
#endif /* __DrawShapeLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


