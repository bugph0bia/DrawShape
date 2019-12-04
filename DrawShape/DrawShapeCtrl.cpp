// DrawShapeCtrl.cpp : CDrawShapeCtrl ActiveX コントロール クラスの実装。

#include "pch.h"
#include "framework.h"
#include "DrawShape.h"
#include "DrawShapeCtrl.h"
#include "DrawShapePropPage.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CDrawShapeCtrl, COleControl)

// メッセージ マップ

BEGIN_MESSAGE_MAP(CDrawShapeCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// ディスパッチ マップ

BEGIN_DISPATCH_MAP(CDrawShapeCtrl, COleControl)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "BaseColor", dispidBaseColor, GetBaseColor, SetBaseColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "GridColor", dispidGridColor, GetGridColor, SetGridColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "GridSize", dispidGridSize, GetGridSize, SetGridSize, VT_R8)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "OriginColor", dispidOriginColor, GetOriginColor, SetOriginColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "OriginSize", dispidOriginSize, GetOriginSize, SetOriginSize, VT_I4)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "AxisColor", dispidAxisColor, GetAxisColor, SetAxisColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "AxisScale", dispidAxisScale, GetAxisScale, SetAxisScale, VT_R8)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "IsDrawGrid", dispidIsDrawGrid, GetIsDrawGrid, SetIsDrawGrid, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "IsDrawOrigin", dispidIsDrawOrigin, GetIsDrawOrigin, SetIsDrawOrigin, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "IsDrawAxis", dispidIsDrawAxis, GetIsDrawAxis, SetIsDrawAxis, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "IsDrawArrow", dispidIsDrawArrow, GetIsDrawArrow, SetIsDrawArrow, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "IsDrawCenter", dispidIsDrawCenter, GetIsDrawCenter, SetIsDrawCenter, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "CurrentLayerNo", dispidCurrentLayerNo, GetCurrentLayerNo, SetCurrentLayerNo, VT_I4)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "LayerCount", dispidLayerCount, GetLayerCount, SetLayerCount, VT_I4)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "CanMouseDragPan", dispidCanMouseDragPan, GetCanMouseDragPan, SetCanMouseDragPan, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "CanMouseWheelZoom", dispidCanMouseWheelZoom, GetCanMouseWheelZoom, SetCanMouseWheelZoom, VT_BOOL)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "Redraw", dispidRedraw, Redraw, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "Clear", dispidClear, Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "SaveBitmap", dispidSaveBitmap, SaveBitmap, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "CopyBitmap", dispidCopyBitmap, CopyBitmap, VT_BOOL, VTS_NONE)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "CanvasToControl", dispidCanvasToControl, CanvasToControl, VT_EMPTY, VTS_R8 VTS_R8 VTS_PI4 VTS_PI4)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "ControlToCanvas", dispidControlToCanvas, ControlToCanvas, VT_EMPTY, VTS_I4 VTS_I4 VTS_PR8 VTS_PR8)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "InsertLayer", dispidInsertLayer, InsertLayer, VT_BOOL, VTS_I4)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "ClearCurrentLayer", dispidClearCurrentLayer, ClearCurrentLayer, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "DeleteCurrentLayer", dispidDeleteCurrentLayer, DeleteCurrentLayer, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "SetEnableCurrentLayer", dispidSetEnableCurrentLayer, SetEnableCurrentLayer, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "GetEnableCurrentLayer", dispidGetEnableCurrentLayer, GetEnableCurrentLayer, VT_BOOL, VTS_NONE)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "Zoom", dispidZoom, Zoom, VT_BOOL, VTS_R8 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "Pan", dispidPan, Pan, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "Fit", dispidFit, Fit, VT_EMPTY, VTS_R8)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "ChangePen", dispidChangePen, ChangePen, VT_EMPTY, VTS_I4 VTS_I4 VTS_COLOR)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "ChangeBrush", dispidChangeBrush, ChangeBrush, VT_EMPTY, VTS_I4 VTS_COLOR VTS_I4)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddLine", dispidAddLine, AddLine, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8 VTS_R8)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddInfiniteLine2Point", dispidAddInfiniteLine2Point, AddInfiniteLine2Point, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8 VTS_R8)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddInfiniteLine1PointAngle", dispidAddInfiniteLine1PointAngle, AddInfiniteLine1PointAngle, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddPoint", dispidAddPoint, AddPoint, VT_EMPTY, VTS_R8 VTS_R8 VTS_I4)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddArc", dispidAddArc, AddArc, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_BOOL)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddCircle", dispidAddCircle, AddCircle, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddPolygon", dispidAddPolygon, AddPolygon, VT_EMPTY, VTS_PR8 VTS_I4 VTS_BOOL)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddSector", dispidAddSector, AddSector, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_BOOL VTS_BOOL)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddGrid", dispidAddGrid, AddGrid, VT_EMPTY, VTS_R8 VTS_R8)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddAxis", dispidAddAxis, AddAxis, VT_EMPTY, VTS_R8 VTS_R8)
END_DISPATCH_MAP()

// イベント マップ

BEGIN_EVENT_MAP(CDrawShapeCtrl, COleControl)
END_EVENT_MAP()

// プロパティ ページ

// TODO: プロパティ ページを追加して、BEGIN 行の最後にあるカウントを増やしてください。
BEGIN_PROPPAGEIDS(CDrawShapeCtrl, 1)
	PROPPAGEID(CDrawShapePropPage::guid)
END_PROPPAGEIDS(CDrawShapeCtrl)

// クラス ファクトリおよび GUID を初期化します。

IMPLEMENT_OLECREATE_EX(CDrawShapeCtrl, "MFCACTIVEXCONTRO.DrawShapeCtrl.1",
	0x92b77edb,0x2984,0x4ca7,0xb1,0x36,0xe3,0x37,0xa6,0x7c,0xc9,0xbb)

// タイプ ライブラリ ID およびバージョン

IMPLEMENT_OLETYPELIB(CDrawShapeCtrl, _tlid, _wVerMajor, _wVerMinor)

// インターフェイス ID

const IID IID_DDrawShape = {0x9987a44e,0x72fb,0x4e9a,{0xa4,0x7f,0x93,0x6f,0xcf,0xe8,0xdb,0x17}};
const IID IID_DDrawShapeEvents = {0x21a2e7b0,0x8b57,0x408c,{0xbd,0x42,0x9a,0x74,0x96,0xd4,0x3d,0x82}};

// コントロールの種類の情報

static const DWORD _dwDrawShapeOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CDrawShapeCtrl, IDS_DRAWSHAPE, _dwDrawShapeOleMisc)

// CDrawShapeCtrl::CDrawShapeCtrlFactory::UpdateRegistry -
// CDrawShapeCtrl のシステム レジストリ エントリを追加または削除します

BOOL CDrawShapeCtrl::CDrawShapeCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: コントロールが apartment モデルのスレッド処理の規則に従っていることを
	// 確認してください。詳細については MFC のテクニカル ノート 64 を参照してください。
	// アパートメント モデルのスレッド処理の規則に従わないコントロールの場合は、6 番目
	// のパラメーターを以下のように変更してください。
	// afxRegApartmentThreading を 0 に設定します。

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_DRAWSHAPE,
			IDB_DRAWSHAPE,
			afxRegApartmentThreading,
			_dwDrawShapeOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


// CDrawShapeCtrl::CDrawShapeCtrl - コンストラクター

CDrawShapeCtrl::CDrawShapeCtrl() :
	m_BaseColor(0),
	m_GridColor(0),
	m_GridSize(0),
	m_OriginColor(0),
	m_OriginSize(0),
	m_AxisColor(0),
	m_AxisScale(0),
	m_IsDrawGrid(0),
	m_IsDrawOrigin(0),
	m_IsDrawAxis(0),
	m_IsDrawArrow(0),
	m_IsDrawCenter(0),
	m_CanMouseDragPan(0),
	m_CanMouseWheelZoom(0),
	m_pOldBmp(nullptr),
	m_IsDragging(0)
{
	InitializeIIDs(&IID_DDrawShape, &IID_DDrawShapeEvents);
	// TODO: この位置にコントロールのインスタンス データの初期化処理を追加してください
}

// CDrawShapeCtrl::~CDrawShapeCtrl - デストラクタ―

CDrawShapeCtrl::~CDrawShapeCtrl()
{
	// TODO: この位置にコントロールのインスタンス データの後処理用のコードを追加してください
}

// CDrawShapeCtrl::OnDraw - 描画関数

void CDrawShapeCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */)
{
	if (!pdc)
		return;

	// TODO: 以下のコードを描画用のコードに置き換えてください
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// CDrawShapeCtrl::DoPropExchange - 永続性のサポート

void CDrawShapeCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 永続属性を持つ各カスタム プロパティ用の PX_ 関数を呼び出します。
}


// CDrawShapeCtrl::OnResetState - コントロールを既定の状態にリセットします

void CDrawShapeCtrl::OnResetState()
{
	COleControl::OnResetState();  // DoPropExchange を呼び出して既定値にリセット

	// TODO: この位置にコントロールの状態をリセットする処理を追加してください
}


// CDrawShapeCtrl メッセージ ハンドラー


int CDrawShapeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ここに特定な作成コードを追加してください。

	// 初期化処理
	Initialize();

	return 0;
}


void CDrawShapeCtrl::OnDestroy()
{
	COleControl::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。

	// 終了処理
	Terminate();
}


void CDrawShapeCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。

	// 実行モード
	if (AmbientUserMode()) {
		// デバイスコンテキスト取得
		CDC* pDC = GetDC();

		// コントロールの矩形領域の再設定
		CRect rect;
		GetClientRect(&rect);
		// メモリデバイスコンテキストの実体を削除
		m_memDC.SelectObject(m_pOldBmp);
		m_memBmp.DeleteObject();
		// 新しいサイズに合わせてメモリデバイスコンテキストを再設定
		m_memBmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		m_pOldBmp = m_memDC.SelectObject(&m_memBmp);
		// デバイスコンテキスト解放
		ReleaseDC(pDC);

		// 描画管理オブジェクトを再設定
		m_pDrawManager->ResetCanvas(&m_memDC, rect);

		// 再描画
		Redraw();
	}
}


void CDrawShapeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	// ドラッグによるパンを許可している場合
	if (m_CanMouseDragPan) {
		// ドラッグ中であることを覚える
		m_IsDragging = TRUE;
		m_pntDraggingBasePos = point;

		// ドラッグ中カーソル：全方向矢印
		SetCursor(::LoadCursor(NULL, IDC_SIZEALL));

		// マウスカーソルの移動範囲をコントロール内に制限
		CRect rect;
		GetWindowRect(&rect);
		::ClipCursor(&rect);
	}

	COleControl::OnLButtonDown(nFlags, point);
}


void CDrawShapeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	// ドラッグによるパンを許可している場合
	if (m_CanMouseDragPan) {
		// ドラッグ終了とする
		m_IsDragging = FALSE;
		m_pntDraggingBasePos = POINT{ 0, 0 };

		// カーソルを戻す：十字
		SetCursor(::LoadCursor(NULL, IDC_CROSS));

		// マウスカーソルの移動範囲の制限を解除
		::ClipCursor(NULL);
	}

	COleControl::OnLButtonUp(nFlags, point);
}


void CDrawShapeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	// ドラッグによるパンを許可 かつ ドラッグ中
	if (m_CanMouseDragPan && m_IsDragging) {
		// パン
		if (!Pan((point.x - m_pntDraggingBasePos.x), (point.y - m_pntDraggingBasePos.y))) {
			// これ以上ドラッグさせないようにカーソルをドラッグ開始位置に設定
			CPoint	dragStartPosScreen = m_pntDraggingBasePos;
			ClientToScreen(&dragStartPosScreen);
			SetCursorPos(dragStartPosScreen.x, dragStartPosScreen.y);
			point = m_pntDraggingBasePos;
		}
		else {
			// 基準カーソル位置を更新
			m_pntDraggingBasePos = point;
		}
	}

	COleControl::OnMouseMove(nFlags, point);
}


BOOL CDrawShapeCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	// マウスホイールによるズームを許可
	if (m_CanMouseWheelZoom) {
		// カーソル位置をクライアント座標系に変換
		CPoint pntClient = pt;
		ScreenToClient(&pntClient);

		// 拡大
		if (zDelta > 0) Zoom(ZOOM_UP_RATIO, pntClient.x, pntClient.y);
		// 縮小
		else Zoom(ZOOM_DOWN_RATIO, pntClient.x, pntClient.y);
	}

	return COleControl::OnMouseWheel(nFlags, zDelta, pt);
}

// TODO: ↓未実装

OLE_COLOR CDrawShapeCtrl::GetBaseColor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return RGB(0, 0, 0);
}


void CDrawShapeCtrl::SetBaseColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


OLE_COLOR CDrawShapeCtrl::GetGridColor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return RGB(0, 0, 0);
}


void CDrawShapeCtrl::SetGridColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


DOUBLE CDrawShapeCtrl::GetGridSize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return 0;
}


void CDrawShapeCtrl::SetGridSize(DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


OLE_COLOR CDrawShapeCtrl::GetOriginColor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return RGB(0, 0, 0);
}


void CDrawShapeCtrl::SetOriginColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


LONG CDrawShapeCtrl::GetOriginSize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return 0;
}


void CDrawShapeCtrl::SetOriginSize(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


OLE_COLOR CDrawShapeCtrl::GetAxisColor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return RGB(0, 0, 0);
}


void CDrawShapeCtrl::SetAxisColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


DOUBLE CDrawShapeCtrl::GetAxisScale()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return 0;
}


void CDrawShapeCtrl::SetAxisScale(DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


VARIANT_BOOL CDrawShapeCtrl::GetIsDrawGrid()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return VARIANT_TRUE;
}


void CDrawShapeCtrl::SetIsDrawGrid(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


VARIANT_BOOL CDrawShapeCtrl::GetIsDrawOrigin()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return 0;
}


void CDrawShapeCtrl::SetIsDrawOrigin(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


VARIANT_BOOL CDrawShapeCtrl::GetIsDrawAxis()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return 0;
}


void CDrawShapeCtrl::SetIsDrawAxis(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


VARIANT_BOOL CDrawShapeCtrl::GetIsDrawArrow()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return VARIANT_TRUE;
}


void CDrawShapeCtrl::SetIsDrawArrow(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


VARIANT_BOOL CDrawShapeCtrl::GetIsDrawCenter()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return VARIANT_TRUE;
}


void CDrawShapeCtrl::SetIsDrawCenter(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


LONG CDrawShapeCtrl::GetCurrentLayerNo()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return 0;
}


void CDrawShapeCtrl::SetCurrentLayerNo(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


LONG CDrawShapeCtrl::GetLayerCount()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return 0;
}


void CDrawShapeCtrl::SetLayerCount(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


VARIANT_BOOL CDrawShapeCtrl::GetCanMouseDragPan()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return VARIANT_TRUE;
}


void CDrawShapeCtrl::SetCanMouseDragPan(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


VARIANT_BOOL CDrawShapeCtrl::GetCanMouseWheelZoom()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return VARIANT_TRUE;
}


void CDrawShapeCtrl::SetCanMouseWheelZoom(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	SetModifiedFlag();
}


void CDrawShapeCtrl::Redraw()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


void CDrawShapeCtrl::Clear()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


VARIANT_BOOL CDrawShapeCtrl::SaveBitmap(BSTR filePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return VARIANT_TRUE;
}


VARIANT_BOOL CDrawShapeCtrl::CopyBitmap()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return VARIANT_TRUE;
}


void CDrawShapeCtrl::CanvasToControl(DOUBLE canvasX, DOUBLE canvasY, LONG* pCtrlX, LONG* pCtrlY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


void CDrawShapeCtrl::ControlToCanvas(LONG ctrlX, LONG ctrlY, DOUBLE* pCanvasX, DOUBLE* pCanvasY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


VARIANT_BOOL CDrawShapeCtrl::InsertLayer(LONG insertNo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return VARIANT_TRUE;
}


void CDrawShapeCtrl::ClearCurrentLayer()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


LONG CDrawShapeCtrl::DeleteCurrentLayer()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return 0;
}


void CDrawShapeCtrl::SetEnableCurrentLayer(VARIANT_BOOL enable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


VARIANT_BOOL CDrawShapeCtrl::GetEnableCurrentLayer()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return VARIANT_TRUE;
}


VARIANT_BOOL CDrawShapeCtrl::Zoom(DOUBLE ratio, LONG ctrlBaseX, LONG ctrlBaseY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return VARIANT_TRUE;
}


VARIANT_BOOL CDrawShapeCtrl::Pan(LONG ctrlMoveX, LONG ctrlMoveY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return VARIANT_TRUE;
}


void CDrawShapeCtrl::Fit(DOUBLE ratio)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


void CDrawShapeCtrl::ChangePen(LONG style, LONG width, OLE_COLOR color)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


void CDrawShapeCtrl::ChangeBrush(LONG style, OLE_COLOR color, LONG hatch)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


void CDrawShapeCtrl::AddLine(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


void CDrawShapeCtrl::AddInfiniteLine2Point(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


void CDrawShapeCtrl::AddInfiniteLine1PointAngle(DOUBLE x, DOUBLE y, DOUBLE angle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


void CDrawShapeCtrl::AddPoint(DOUBLE x, DOUBLE y, LONG type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


void CDrawShapeCtrl::AddArc(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey, DOUBLE cx, DOUBLE cy, VARIANT_BOOL left)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


void CDrawShapeCtrl::AddCircle(DOUBLE cx, DOUBLE cy, DOUBLE radius, VARIANT_BOOL fill)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


void CDrawShapeCtrl::AddPolygon(DOUBLE* points, LONG pointCount, VARIANT_BOOL fill)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


void CDrawShapeCtrl::AddSector(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey, DOUBLE cx, DOUBLE cy, DOUBLE innerRadius, VARIANT_BOOL left, VARIANT_BOOL fill)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


void CDrawShapeCtrl::AddGrid(DOUBLE ox, DOUBLE oy)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


void CDrawShapeCtrl::AddAxis(DOUBLE ox, DOUBLE oy)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します
}


// 初期化処理
void CDrawShapeCtrl::Initialize()
{
	// ドラッグ状態をクリア
	m_IsDragging = FALSE;
	m_pntDraggingBasePos = POINT{ 0, 0 };

	// コントロールの矩形領域
	CRect rect;
	GetClientRect(&rect);
	// デバイスコンテキスト取得
	CDC* pDC = GetDC();
	// メモリデバイスコンテキストを作成
	m_memDC.CreateCompatibleDC(pDC);
	m_memBmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	m_pOldBmp = m_memDC.SelectObject(&m_memBmp);
	// バックグラウンドを変更しないように背景モードを透明に設定
	//   ※点線を描くときなどに影響
	m_memDC.SetBkMode(TRANSPARENT);
	// デバイスコンテキスト解放
	ReleaseDC(pDC);

	// 通常時カーソル：十字
	SetCursor(::LoadCursor(NULL, IDC_CROSS));

	// 描画管理オブジェクトを作成
	m_pDrawManager = std::make_unique<Drawer::Manager>(&m_memDC, rect);
	// 描画領域初期化
	Redraw();
}

// 終了処理
void CDrawShapeCtrl::Terminate()
{
	// メモリデバイスコンテキストの実態を戻す
	m_memDC.SelectObject(m_pOldBmp);
	// ビットマップを削除
	m_memBmp.DeleteObject();
	// メモリデバイスコンテキストを削除
	m_memDC.DeleteDC();
}
