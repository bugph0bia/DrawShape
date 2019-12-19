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
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// ディスパッチ マップ

BEGIN_DISPATCH_MAP(CDrawShapeCtrl, COleControl)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "BackColor", dispidBackColor, GetBackColor, SetBackColor, VT_COLOR)
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
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "LayerCount", dispidLayerCount, GetLayerCount, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "CurrentPenColor", dispidCurrentPenColor, GetCurrentPenColor, SetCurrentPenColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "CurrentPenWidth", dispidCurrentPenWidth, GetCurrentPenWidth, SetCurrentPenWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "CurrentPenStyle", dispidCurrentPenStyle, GetCurrentPenStyle, SetCurrentPenStyle, VT_I4)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "CurrentBrushColor", dispidCurrentBrushColor, GetCurrentBrushColor, SetCurrentBrushColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "CanMouseDragPan", dispidCanMouseDragPan, GetCanMouseDragPan, SetCanMouseDragPan, VT_BOOL)
	DISP_PROPERTY_EX_ID(CDrawShapeCtrl, "CanMouseWheelZoom", dispidCanMouseWheelZoom, GetCanMouseWheelZoom, SetCanMouseWheelZoom, VT_BOOL)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "Redraw", dispidRedraw, Redraw, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "Clear", dispidClear, Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "SaveImage", dispidSaveImage, SaveImage, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "CopyImage", dispidCopyImage, CopyImage, VT_BOOL, VTS_NONE)
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
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddLine", dispidAddLine, AddLine, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8 VTS_R8)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddInfiniteLine2Point", dispidAddInfiniteLine2Point, AddInfiniteLine2Point, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8 VTS_R8)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddInfiniteLine1PointAngle", dispidAddInfiniteLine1PointAngle, AddInfiniteLine1PointAngle, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddPoint", dispidAddPoint, AddPoint, VT_EMPTY, VTS_R8 VTS_R8 VTS_I4)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddArc", dispidAddArc, AddArc, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_BOOL)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddCircle", dispidAddCircle, AddCircle, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8 VTS_BOOL)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddPolygon", dispidAddPolygon, AddPolygon, VT_BOOL, VTS_VARIANT VTS_I4 VTS_BOOL)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddSector", dispidAddSector, AddSector, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_BOOL VTS_BOOL)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddOrigin", dispidAddOrigin, AddOrigin, VT_EMPTY, VTS_R8 VTS_R8)
	DISP_FUNCTION_ID(CDrawShapeCtrl, "AddAxis", dispidAddAxis, AddAxis, VT_EMPTY, VTS_R8 VTS_R8)
END_DISPATCH_MAP()

// イベント マップ

BEGIN_EVENT_MAP(CDrawShapeCtrl, COleControl)
	EVENT_CUSTOM("CursorMove", FireCursorMove, VTS_I4 VTS_I4 VTS_R8 VTS_R8)
	EVENT_CUSTOM("LeftClick", FireLeftClick, VTS_I4 VTS_I4 VTS_R8 VTS_R8)
END_EVENT_MAP()

// プロパティ ページ

// TODO: プロパティ ページを追加して、BEGIN 行の最後にあるカウントを増やしてください。
BEGIN_PROPPAGEIDS(CDrawShapeCtrl, 2)
	PROPPAGEID(CLSID_CColorPropPage)
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
	m_CanMouseDragPan(0),
	m_CanMouseWheelZoom(0),
	m_pOldBmp(nullptr),
	m_isDragging(0),
	m_pDrawManager(std::make_unique<Drawer::Manager>())
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

	// 実行モード時
	if (AmbientUserMode()) {
		// メモリデバイスコンテキストの内容をコントロールデバイスコンテキストに転送
		pdc->BitBlt(0, 0, rcBounds.Width(), rcBounds.Height(), &m_memDC, 0, 0, SRCCOPY);
	}
	// デザインモード時
	else {
		// デザインモード用描画処理
		m_pDrawManager->ResetCanvas(pdc, rcBounds);
		m_pDrawManager->Draw(true);
	}
}

// CDrawShapeCtrl::DoPropExchange - 永続性のサポート

void CDrawShapeCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 永続属性を持つ各カスタム プロパティ用の PX_ 関数を呼び出します。

	PX_Color(pPX, _T("BackColor"), m_pDrawManager->m_info.backColor, DEFAULT_BACK_COLOR);
	PX_Color(pPX, _T("GridColor"), m_pDrawManager->m_info.gridColor, DEFAULT_GRID_COLOR);
	PX_Double(pPX, _T("GridSize"), m_pDrawManager->m_info.gridSize, DEFAULT_GRID_SIZE);
	PX_Color(pPX, _T("OriginColor"), m_pDrawManager->m_info.originColor, DEFAULT_ORIGIN_COLOR);
	PX_Long(pPX, _T("OriginSize"), m_pDrawManager->m_info.originSize, DEFAULT_ORIGIN_SIZE);
	PX_Color(pPX, _T("AxisColor"), m_pDrawManager->m_info.axisColor, DEFAULT_AXIS_COLOR);
	PX_Double(pPX, _T("AxisScale"), m_pDrawManager->m_info.axisScale, DEFAULT_AXIS_SCALE);
	PX_Bool(pPX, _T("IsDrawGrid"), m_pDrawManager->m_info.isDrawGrid, DEFAULT_IS_DRAW_GRID);
	PX_Bool(pPX, _T("IsDrawOrigin"), m_pDrawManager->m_info.isDrawOrigin, DEFAULT_IS_DRAW_ORIGIN);
	PX_Bool(pPX, _T("IsDrawAxis"), m_pDrawManager->m_info.isDrawAxis, DEFAULT_IS_DRAW_AXIS);
	PX_Bool(pPX, _T("IsDrawArrow"), m_pDrawManager->m_info.isDrawArrow, DEFAULT_IS_DRAW_ARROW);
	PX_Bool(pPX, _T("IsDrawCenter"), m_pDrawManager->m_info.isDrawCenter, DEFAULT_IS_DRAW_CENTER);
	PX_Color(pPX, _T("CurrentPenColor"), m_pDrawManager->m_penColor, Drawer::Manager::DEFAULT_PEN.lopnColor);
	PX_Long(pPX, _T("CurrentPenWidth"), m_pDrawManager->m_penWidth, Drawer::Manager::DEFAULT_PEN.lopnWidth.x);
	PX_Long(pPX, _T("CurrentPenStyle"), m_pDrawManager->m_penStyle, Drawer::Manager::DEFAULT_PEN.lopnStyle);
	PX_Color(pPX, _T("CurrentBrushColor"), m_pDrawManager->m_brushColor, Drawer::Manager::DEFAULT_BRUSH.lbColor);
	PX_Bool(pPX, _T("CanMouseDragPan"), m_CanMouseDragPan, DEFAULT_IS_MOUSE_DRAG_PAN);
	PX_Bool(pPX, _T("CanMouseWheelZoom"), m_CanMouseWheelZoom, DEFAULT_IS_MOUSE_WHEEL_ZOOM);
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
		m_pDrawManager->ResetCanvas(&m_memDC, rect, false);

		// 再描画
		Redraw();
	}
}


void CDrawShapeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	// 実行モード
	if (AmbientUserMode()) {
		// ドラッグによるパンを許可している場合
		if (m_CanMouseDragPan) {
			// ドラッグ中であることを覚える
			m_isDragging = TRUE;
			m_pntDraggingBasePos = point;

			// ドラッグ中カーソル：全方向矢印
			SetCursor(::LoadCursor(NULL, IDC_SIZEALL));

			// マウスカーソルの移動範囲をコントロール内に制限
			CRect rect;
			GetWindowRect(&rect);
			::ClipCursor(&rect);
		}
	}

	COleControl::OnLButtonDown(nFlags, point);
}


void CDrawShapeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	// 実行モード
	if (AmbientUserMode()) {
		// ドラッグによるパンを許可している場合
		if (m_CanMouseDragPan) {
			// ドラッグ終了とする
			m_isDragging = FALSE;
			m_pntDraggingBasePos = POINT{ 0, 0 };

			// カーソルを戻す：十字
			SetCursor(::LoadCursor(NULL, IDC_CROSS));

			// マウスカーソルの移動範囲の制限を解除
			::ClipCursor(NULL);
		}

		// LeftClickイベント発生
		auto canvasCoord = m_pDrawManager->ControlToCanvas(Drawer::Coord<long>(point.x, point.y));
		FireLeftClick(point.x, point.y, canvasCoord.x, canvasCoord.y);
	}

	COleControl::OnLButtonUp(nFlags, point);
}


void CDrawShapeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	// 実行モード
	if (AmbientUserMode()) {
		// ドラッグによるパンを許可 かつ ドラッグ中
		if (m_CanMouseDragPan && m_isDragging) {
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

		// CursorMoveイベント発生
		auto canvasCoord = m_pDrawManager->ControlToCanvas(Drawer::Coord<long>(point.x, point.y));
		FireCursorMove(point.x, point.y, canvasCoord.x, canvasCoord.y);
	}

	COleControl::OnMouseMove(nFlags, point);
}


BOOL CDrawShapeCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	// 実行モード
	if (AmbientUserMode()) {
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
	}

	return COleControl::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL CDrawShapeCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	//return COleControl::OnSetCursor(pWnd, nHitTest, message);
	return TRUE;
}


OLE_COLOR CDrawShapeCtrl::GetBackColor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_info.backColor;
}


void CDrawShapeCtrl::SetBackColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_info.backColor = newVal;
	SetModifiedFlag();
}


OLE_COLOR CDrawShapeCtrl::GetGridColor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_info.gridColor;
}


void CDrawShapeCtrl::SetGridColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_info.gridColor = newVal;
	SetModifiedFlag();
}


DOUBLE CDrawShapeCtrl::GetGridSize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_info.gridSize;
}


void CDrawShapeCtrl::SetGridSize(DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_info.gridSize = newVal;
	SetModifiedFlag();
}


OLE_COLOR CDrawShapeCtrl::GetOriginColor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_info.originColor;
}


void CDrawShapeCtrl::SetOriginColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_info.originColor = newVal;
	SetModifiedFlag();
}


LONG CDrawShapeCtrl::GetOriginSize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_info.originSize;
}


void CDrawShapeCtrl::SetOriginSize(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_info.originSize = newVal;
	SetModifiedFlag();
}


OLE_COLOR CDrawShapeCtrl::GetAxisColor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_info.axisColor;
}


void CDrawShapeCtrl::SetAxisColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_info.axisColor = newVal;
	SetModifiedFlag();
}


DOUBLE CDrawShapeCtrl::GetAxisScale()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_info.axisScale;
}


void CDrawShapeCtrl::SetAxisScale(DOUBLE newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_info.axisScale = newVal;
	SetModifiedFlag();
}


VARIANT_BOOL CDrawShapeCtrl::GetIsDrawGrid()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_info.isDrawGrid ? VARIANT_TRUE : VARIANT_FALSE;
}


void CDrawShapeCtrl::SetIsDrawGrid(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_info.isDrawGrid = (newVal != VARIANT_FALSE);
	SetModifiedFlag();
}


VARIANT_BOOL CDrawShapeCtrl::GetIsDrawOrigin()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_info.isDrawOrigin ? VARIANT_TRUE : VARIANT_FALSE;
}


void CDrawShapeCtrl::SetIsDrawOrigin(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_info.isDrawOrigin = (newVal != VARIANT_FALSE);
	SetModifiedFlag();
}


VARIANT_BOOL CDrawShapeCtrl::GetIsDrawAxis()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_info.isDrawAxis ? VARIANT_TRUE : VARIANT_FALSE;
}


void CDrawShapeCtrl::SetIsDrawAxis(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_info.isDrawAxis = (newVal != VARIANT_FALSE);
	SetModifiedFlag();
}


VARIANT_BOOL CDrawShapeCtrl::GetIsDrawArrow()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_info.isDrawArrow ? VARIANT_TRUE : VARIANT_FALSE;
}


void CDrawShapeCtrl::SetIsDrawArrow(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_info.isDrawArrow = (newVal != VARIANT_FALSE);
	SetModifiedFlag();
}


VARIANT_BOOL CDrawShapeCtrl::GetIsDrawCenter()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_info.isDrawCenter ? VARIANT_TRUE : VARIANT_FALSE;
}


void CDrawShapeCtrl::SetIsDrawCenter(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_info.isDrawCenter = (newVal != VARIANT_FALSE);
	SetModifiedFlag();
}


LONG CDrawShapeCtrl::GetCurrentLayerNo()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->GetCurrentLayerNo();
}


void CDrawShapeCtrl::SetCurrentLayerNo(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->SetCurrentLayerNo(newVal);
	SetModifiedFlag();
}


LONG CDrawShapeCtrl::GetLayerCount()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->GetLayerCount();
}


OLE_COLOR CDrawShapeCtrl::GetCurrentPenColor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_penColor;
}


void CDrawShapeCtrl::SetCurrentPenColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_penColor = newVal;
	SetModifiedFlag();
}


LONG CDrawShapeCtrl::GetCurrentPenWidth()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_penWidth;
}


void CDrawShapeCtrl::SetCurrentPenWidth(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_penWidth = newVal;
	SetModifiedFlag();
}


LONG CDrawShapeCtrl::GetCurrentPenStyle()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_penStyle;
}


void CDrawShapeCtrl::SetCurrentPenStyle(LONG newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_penStyle = newVal;
	SetModifiedFlag();
}


OLE_COLOR CDrawShapeCtrl::GetCurrentBrushColor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->m_brushColor;
}


void CDrawShapeCtrl::SetCurrentBrushColor(OLE_COLOR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_pDrawManager->m_brushColor = newVal;
	SetModifiedFlag();
}


VARIANT_BOOL CDrawShapeCtrl::GetCanMouseDragPan()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_CanMouseDragPan ? VARIANT_TRUE : VARIANT_FALSE;
}


void CDrawShapeCtrl::SetCanMouseDragPan(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_CanMouseDragPan = (newVal != VARIANT_FALSE);
	SetModifiedFlag();
}


VARIANT_BOOL CDrawShapeCtrl::GetCanMouseWheelZoom()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_CanMouseWheelZoom ? VARIANT_TRUE : VARIANT_FALSE;
}


void CDrawShapeCtrl::SetCanMouseWheelZoom(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにプロパティ ハンドラー コードを追加します

	m_CanMouseWheelZoom = (newVal != VARIANT_FALSE);
	SetModifiedFlag();
}


void CDrawShapeCtrl::Redraw()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	m_pDrawManager->Draw();

	// 再描画
	InvalidateRect(nullptr, FALSE);
	UpdateWindow();
}


void CDrawShapeCtrl::Clear()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	m_pDrawManager->Clear();
	Redraw();
}


VARIANT_BOOL CDrawShapeCtrl::SaveImage(LPCTSTR filePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->SaveImage(filePath) ? VARIANT_TRUE : VARIANT_FALSE;
}


VARIANT_BOOL CDrawShapeCtrl::CopyImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->CopyImage(this) ? VARIANT_TRUE : VARIANT_FALSE;
}


void CDrawShapeCtrl::CanvasToControl(DOUBLE canvasX, DOUBLE canvasY, LONG* pCtrlX, LONG* pCtrlY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	auto ctrlCoord = m_pDrawManager->CanvasToControl(Drawer::Coord<double>(canvasX, canvasY));
	*pCtrlX = ctrlCoord.x;
	*pCtrlY = ctrlCoord.y;
}


void CDrawShapeCtrl::ControlToCanvas(LONG ctrlX, LONG ctrlY, DOUBLE* pCanvasX, DOUBLE* pCanvasY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	auto canvasCoord = m_pDrawManager->ControlToCanvas(Drawer::Coord<long>(ctrlX, ctrlY));
	*pCanvasX = canvasCoord.x;
	*pCanvasY = canvasCoord.y;
}


VARIANT_BOOL CDrawShapeCtrl::InsertLayer(LONG insertNo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->InsertLayer(insertNo) ? VARIANT_TRUE : VARIANT_FALSE;
}


void CDrawShapeCtrl::ClearCurrentLayer()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	m_pDrawManager->ClearCurrentLayer();
}


LONG CDrawShapeCtrl::DeleteCurrentLayer()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->DeleteCurrentLayer();
}


void CDrawShapeCtrl::SetEnableCurrentLayer(VARIANT_BOOL enable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	m_pDrawManager->SetEnableCurrentLayer(enable != VARIANT_FALSE);
}


VARIANT_BOOL CDrawShapeCtrl::GetEnableCurrentLayer()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	return m_pDrawManager->GetEnableCurrentLayer() ? VARIANT_TRUE : VARIANT_FALSE;
}


VARIANT_BOOL CDrawShapeCtrl::Zoom(DOUBLE coef, LONG ctrlBaseX, LONG ctrlBaseY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	VARIANT_BOOL ret = m_pDrawManager->Zoom(coef, Drawer::Coord<long>(ctrlBaseX, ctrlBaseY)) ? VARIANT_TRUE : VARIANT_FALSE;
	Redraw();
	return ret;
}


VARIANT_BOOL CDrawShapeCtrl::Pan(LONG ctrlMoveX, LONG ctrlMoveY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	VARIANT_BOOL ret = m_pDrawManager->Pan(Drawer::Coord<long>(ctrlMoveX, ctrlMoveY)) ? VARIANT_TRUE : VARIANT_FALSE;
	Redraw();
	return ret;
}


void CDrawShapeCtrl::Fit(DOUBLE shapeOccupancy)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	m_pDrawManager->Fit(shapeOccupancy);
	Redraw();
}


void CDrawShapeCtrl::AddLine(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	m_pDrawManager->AddLine(
		Drawer::Coords<double, 2>{ Drawer::Coord<double>(sx, sy), Drawer::Coord<double>(ex, ey) },
		Drawer::LineLimitType::Finite
	);
}


void CDrawShapeCtrl::AddInfiniteLine2Point(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	m_pDrawManager->AddLine(
		Drawer::Coords<double, 2>{ Drawer::Coord<double>(sx, sy), Drawer::Coord<double>(ex, ey) },
		Drawer::LineLimitType::Infinite
	);
}


void CDrawShapeCtrl::AddInfiniteLine1PointAngle(DOUBLE x, DOUBLE y, DOUBLE angle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	// 直線上の仮想終点を決める
	double ex = x + cos(angle);
	double ey = x + sin(angle);

	m_pDrawManager->AddLine(
		Drawer::Coords<double, 2>{ Drawer::Coord<double>(x, y), Drawer::Coord<double>(ex, ey) },
		Drawer::LineLimitType::Infinite
	);
}


void CDrawShapeCtrl::AddPoint(DOUBLE x, DOUBLE y, LONG type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	m_pDrawManager->AddPoint(
		Drawer::Coord<double>(x, y),
		static_cast<Drawer::PointType>(type)
	);
}


void CDrawShapeCtrl::AddArc(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey, DOUBLE cx, DOUBLE cy, VARIANT_BOOL left)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	m_pDrawManager->AddArc(
		Drawer::Coords<double, 3>{ Drawer::Coord<double>(sx, sy), Drawer::Coord<double>(ex, ey), Drawer::Coord<double>(cx, cy) },
		(left ? Drawer::ArcDirectionType::Left : Drawer::ArcDirectionType::Right)
	);
}


void CDrawShapeCtrl::AddCircle(DOUBLE cx, DOUBLE cy, DOUBLE radius, VARIANT_BOOL fill)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	m_pDrawManager->AddCircle(
		Drawer::Coord<double>(cx, cy),
		radius,
		(fill ? Drawer::FillType::Fill : Drawer::FillType::NoFill)
	);
}


VARIANT_BOOL CDrawShapeCtrl::AddPolygon(VARIANT& pointCoords, LONG pointsCount, VARIANT_BOOL fill)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	// 座標値の数が0なら終了
	if (pointsCount <= 0) return VARIANT_FALSE;
	// 配列の型がアンマッチなら終了
	if ((V_VT(&pointCoords) & (VT_ARRAY | VT_R8)) != (VT_ARRAY | VT_R8)) return VARIANT_FALSE;

	// 配列をSAFEARRAYに変換
	ATL::CComSafeArray<double> saPointCoords;
	saPointCoords.CopyFrom(pointCoords.parray);

	// 座標値配列を変換
	Drawer::Coords_v<double> points;
	for (int i = 0; i < pointsCount; i++) {
		points.push_back(Drawer::Coord<double>(saPointCoords[i * 2 + 0], saPointCoords[i * 2 + 1]));
	}

	m_pDrawManager->AddPolygon(
		points,
		(fill ? Drawer::FillType::Fill : Drawer::FillType::NoFill)
	);

	return VARIANT_TRUE;
}


void CDrawShapeCtrl::AddSector(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey, DOUBLE cx, DOUBLE cy, DOUBLE innerRadius, VARIANT_BOOL left, VARIANT_BOOL fill)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	m_pDrawManager->AddSector(
		Drawer::Coords<double, 3>{ Drawer::Coord<double>(sx, sy), Drawer::Coord<double>(ex, ey), Drawer::Coord<double>(cx, cy) },
		innerRadius,
		(left ? Drawer::ArcDirectionType::Left : Drawer::ArcDirectionType::Right),
		(fill ? Drawer::FillType::Fill : Drawer::FillType::NoFill)
	);
}


void CDrawShapeCtrl::AddOrigin(DOUBLE ox, DOUBLE oy)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	m_pDrawManager->AddOrigin(
		Drawer::Coord<double>(ox, oy)
	);
}


void CDrawShapeCtrl::AddAxis(DOUBLE ox, DOUBLE oy)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	m_pDrawManager->AddAxis(
		Drawer::Coord<double>(ox, oy)
	);
}


// 初期化処理
void CDrawShapeCtrl::Initialize()
{
	// ドラッグ状態をクリア
	m_isDragging = FALSE;
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
	m_pDrawManager->ResetCanvas(&m_memDC, rect);
	// 描画領域初期化
	m_pDrawManager->Clear();
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
