// DrawShapePropPage.cpp : CDrawShapePropPage プロパティ ページ クラスの実装。

#include "pch.h"
#include "framework.h"
#include "DrawShape.h"
#include "DrawShapePropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CDrawShapePropPage, COlePropertyPage)

// メッセージ マップ

BEGIN_MESSAGE_MAP(CDrawShapePropPage, COlePropertyPage)
END_MESSAGE_MAP()

// クラス ファクトリおよび GUID を初期化します。

IMPLEMENT_OLECREATE_EX(CDrawShapePropPage, "MFCACTIVEXCONT.DrawShapePropPage.1",
	0x023f2dfb,0x61d2,0x42d9,0xab,0xb9,0x95,0x29,0xbc,0x9f,0xbd,0x07)

// CDrawShapePropPage::CDrawShapePropPageFactory::UpdateRegistry -
// CDrawShapePropPage のシステム レジストリ エントリを追加または削除します

BOOL CDrawShapePropPage::CDrawShapePropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_DRAWSHAPE_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, nullptr);
}

// CDrawShapePropPage::CDrawShapePropPage - コンストラクター

CDrawShapePropPage::CDrawShapePropPage() :
	COlePropertyPage(IDD, IDS_DRAWSHAPE_PPG_CAPTION)
{
}

// CDrawShapePropPage::DoDataExchange - ページおよびプロパティ間でデータを移動します

void CDrawShapePropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CDrawShapePropPage メッセージ ハンドラー
