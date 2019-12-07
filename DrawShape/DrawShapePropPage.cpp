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
	, m_CkIsDrawGrid(FALSE)
	, m_EdGridSize(0)
	, m_CkIsDrawOrigin(FALSE)
	, m_EdOriginSize(0)
	, m_CkIsDrawAxis(FALSE)
	, m_EdAxisScale(0)
	, m_CkDrawArrow(FALSE)
	, m_CkIsDrawCenter(FALSE)
	, m_CkCanMouseDragPan(FALSE)
	, m_CkCanMouseWheelZoom(FALSE)
{
}

// CDrawShapePropPage::DoDataExchange - ページおよびプロパティ間でデータを移動します

void CDrawShapePropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);

	DDP_Text(pDX, IDC_ED_GRID_SIZE, m_EdGridSize, _T("GridSize"));
	DDX_Text(pDX, IDC_ED_GRID_SIZE, m_EdGridSize);
	DDV_MinMaxDouble(pDX, m_EdGridSize, 0.0, 1000.0);

	DDP_Text(pDX, IDC_ED_ORIGIN_SIZE, m_EdOriginSize, _T("OriginSize"));
	DDX_Text(pDX, IDC_ED_ORIGIN_SIZE, m_EdOriginSize);
	DDV_MinMaxLong(pDX, m_EdOriginSize, 0, 100);

	DDP_Text(pDX, IDC_ED_AXIS_SCALE, m_EdAxisScale, _T("AxisScale"));
	DDX_Text(pDX, IDC_ED_AXIS_SCALE, m_EdAxisScale);
	DDV_MinMaxDouble(pDX, m_EdAxisScale, 0.0, 1000.0);

	DDP_Check(pDX, IDC_CK_IS_DRAW_GRID, m_CkIsDrawGrid, _T("IsDrawGrid"));
	DDX_Check(pDX, IDC_CK_IS_DRAW_GRID, m_CkIsDrawGrid);

	DDP_Check(pDX, IDC_CK_IS_DRAW_ORIGIN, m_CkIsDrawOrigin, _T("IsDrawOrigin"));
	DDX_Check(pDX, IDC_CK_IS_DRAW_ORIGIN, m_CkIsDrawOrigin);

	DDP_Check(pDX, IDC_CK_IS_DRAW_AXIS, m_CkIsDrawAxis, _T("IsDrawAxis"));
	DDX_Check(pDX, IDC_CK_IS_DRAW_AXIS, m_CkIsDrawAxis);

	DDP_Check(pDX, IDC_IS_DRAW_ARROW, m_CkDrawArrow, _T("IsDrawArrow"));
	DDX_Check(pDX, IDC_IS_DRAW_ARROW, m_CkDrawArrow);

	DDP_Check(pDX, IDC_CK_IS_DRAW_CENTER, m_CkIsDrawCenter, _T("IsDrawCenter"));
	DDX_Check(pDX, IDC_CK_IS_DRAW_CENTER, m_CkIsDrawCenter);

	DDP_Check(pDX, IDC_CK_CAN_MOUSE_DRAG_PAN, m_CkCanMouseDragPan, _T("CanMouseDragPan"));
	DDX_Check(pDX, IDC_CK_CAN_MOUSE_DRAG_PAN, m_CkCanMouseDragPan);

	DDP_Check(pDX, IDC_CK_CAN_MOUSE_WHEEL_ZOOM, m_CkCanMouseWheelZoom, _T("CanMouseWheelZoom"));
	DDX_Check(pDX, IDC_CK_CAN_MOUSE_WHEEL_ZOOM, m_CkCanMouseWheelZoom);
}

// CDrawShapePropPage メッセージ ハンドラー
