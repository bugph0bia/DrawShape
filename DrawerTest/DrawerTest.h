
// DrawerTest.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CDrawerTestApp:
// このクラスの実装については、DrawerTest.cpp を参照してください
//

class CDrawerTestApp : public CWinApp
{
public:
	CDrawerTestApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CDrawerTestApp theApp;
