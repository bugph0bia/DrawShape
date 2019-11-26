// DrawShape.cpp : CDrawShapeApp および DLL 登録の実装。

#include "pch.h"
#include "framework.h"
#include "DrawShape.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CDrawShapeApp theApp;

const GUID CDECL _tlid = {0x549e9a52,0xd80b,0x4f80,{0x88,0x1a,0x68,0xbf,0xb2,0x43,0x05,0xa7}};
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CDrawShapeApp::InitInstance - DLL 初期化

BOOL CDrawShapeApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: この位置にモジュールの初期化処理コードを追加してください。
	}

	return bInit;
}



// CDrawShapeApp::ExitInstance - DLL 終了

int CDrawShapeApp::ExitInstance()
{
	// TODO: この位置にモジュールの終了処理を追加してください。

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - エントリをシステム レジストリに追加します。

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - エントリをレジストリから削除します。

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
