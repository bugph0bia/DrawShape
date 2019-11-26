#pragma once

// DrawShape.h : DrawShape.DLL のメイン ヘッダー ファイル

#if !defined( __AFXCTL_H__ )
#error "このファイルをインクルードする前に 'afxctl.h' をインクルードしてください。"
#endif

#include "resource.h"       // メイン シンボル


// CDrawShapeApp : 実装については DrawShape.cpp を参照してください。

class CDrawShapeApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

