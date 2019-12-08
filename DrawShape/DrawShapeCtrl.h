#pragma once

#include "Drawer.h"

// DrawShapeCtrl.h : CDrawShapeCtrl ActiveX コントロール クラスの宣言。


// CDrawShapeCtrl : 実装については DrawShapeCtrl.cpp を参照してください。

class CDrawShapeCtrl : public COleControl
{
	DECLARE_DYNCREATE(CDrawShapeCtrl)

// コンストラクター
public:
	CDrawShapeCtrl();

// オーバーライド
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 実装
protected:
	~CDrawShapeCtrl();

	DECLARE_OLECREATE_EX(CDrawShapeCtrl)    // クラス ファクトリ と guid
	DECLARE_OLETYPELIB(CDrawShapeCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CDrawShapeCtrl)     // プロパティ ページ ID
	DECLARE_OLECTLTYPE(CDrawShapeCtrl)		// タイプ名とその他のステータス

// メッセージ マップ
	// イベントハンドラ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()

// ディスパッチ マップ
	// IDLインタフェース - プロパティ
	afx_msg OLE_COLOR GetBackColor();
	afx_msg void SetBackColor(OLE_COLOR newVal);
	afx_msg OLE_COLOR GetGridColor();
	afx_msg void SetGridColor(OLE_COLOR newVal);
	afx_msg DOUBLE GetGridSize();
	afx_msg void SetGridSize(DOUBLE newVal);
	afx_msg OLE_COLOR GetOriginColor();
	afx_msg void SetOriginColor(OLE_COLOR newVal);
	afx_msg LONG GetOriginSize();
	afx_msg void SetOriginSize(LONG newVal);
	afx_msg OLE_COLOR GetAxisColor();
	afx_msg void SetAxisColor(OLE_COLOR newVal);
	afx_msg DOUBLE GetAxisScale();
	afx_msg void SetAxisScale(DOUBLE newVal);
	afx_msg VARIANT_BOOL GetIsDrawGrid();
	afx_msg void SetIsDrawGrid(VARIANT_BOOL newVal);
	afx_msg VARIANT_BOOL GetIsDrawOrigin();
	afx_msg void SetIsDrawOrigin(VARIANT_BOOL newVal);
	afx_msg VARIANT_BOOL GetIsDrawAxis();
	afx_msg void SetIsDrawAxis(VARIANT_BOOL newVal);
	afx_msg VARIANT_BOOL GetIsDrawArrow();
	afx_msg void SetIsDrawArrow(VARIANT_BOOL newVal);
	afx_msg VARIANT_BOOL GetIsDrawCenter();
	afx_msg void SetIsDrawCenter(VARIANT_BOOL newVal);
	afx_msg LONG GetCurrentLayerNo();
	afx_msg void SetCurrentLayerNo(LONG newVal);
	afx_msg LONG GetLayerCount();
	afx_msg VARIANT_BOOL GetCanMouseDragPan();
	afx_msg void SetCanMouseDragPan(VARIANT_BOOL newVal);
	afx_msg VARIANT_BOOL GetCanMouseWheelZoom();
	afx_msg void SetCanMouseWheelZoom(VARIANT_BOOL newVal);
	// IDLインタフェース - メソッド
	afx_msg void Redraw();
	afx_msg void Clear();
	afx_msg VARIANT_BOOL SaveImage(LPCTSTR filePath);
	afx_msg VARIANT_BOOL CopyImage();
	afx_msg void CanvasToControl(DOUBLE canvasX, DOUBLE canvasY, LONG* pCtrlX, LONG* pCtrlY);
	afx_msg void ControlToCanvas(LONG ctrlX, LONG ctrlY, DOUBLE* pCanvasX, DOUBLE* pCanvasY);
	afx_msg VARIANT_BOOL InsertLayer(LONG insertNo);
	afx_msg void ClearCurrentLayer();
	afx_msg LONG DeleteCurrentLayer();
	afx_msg void SetEnableCurrentLayer(VARIANT_BOOL enable);
	afx_msg VARIANT_BOOL GetEnableCurrentLayer();
	afx_msg VARIANT_BOOL Zoom(DOUBLE ratio, LONG ctrlBaseX, LONG ctrlBaseY);
	afx_msg VARIANT_BOOL Pan(LONG ctrlMoveX, LONG ctrlMoveY);
	afx_msg void Fit(DOUBLE shapeOccupancy);
	afx_msg void ChangePen(LONG style, LONG width, OLE_COLOR color);
	afx_msg void ChangeBrush(LONG style, OLE_COLOR color, LONG hatch);
	afx_msg void AddLine(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey);
	afx_msg void AddInfiniteLine2Point(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey);
	afx_msg void AddInfiniteLine1PointAngle(DOUBLE x, DOUBLE y, DOUBLE angle);
	afx_msg void AddPoint(DOUBLE x, DOUBLE y, LONG type);
	afx_msg void AddArc(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey, DOUBLE cx, DOUBLE cy, VARIANT_BOOL left);
	afx_msg void AddCircle(DOUBLE cx, DOUBLE cy, DOUBLE radius, VARIANT_BOOL fill);
	afx_msg VARIANT_BOOL AddPolygon(DOUBLE* pointCoords, LONG pointCoordsCount, VARIANT_BOOL fill);
	afx_msg void AddSector(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey, DOUBLE cx, DOUBLE cy, DOUBLE innerRadius, VARIANT_BOOL left, VARIANT_BOOL fill);
	afx_msg void AddOrigin(DOUBLE ox, DOUBLE oy);
	afx_msg void AddAxis(DOUBLE ox, DOUBLE oy);
	DECLARE_DISPATCH_MAP()

// イベント マップ
	DECLARE_EVENT_MAP()

// ディスパッチ と イベント ID
public:
	enum {
		dispidAddAxis = 42L,
		dispidAddOrigin = 41L,
		dispidAddSector = 40L,
		dispidAddPolygon = 39L,
		dispidAddCircle = 38L,
		dispidAddArc = 37L,
		dispidAddPoint = 36L,
		dispidAddInfiniteLine1PointAngle = 35L,
		dispidAddInfiniteLine2Point = 34L,
		dispidAddLine = 33L,
		dispidChangeBrush = 32L,
		dispidChangePen = 31L,
		dispidFit = 30L,
		dispidPan = 29L,
		dispidZoom = 28L,
		dispidGetEnableCurrentLayer = 27L,
		dispidSetEnableCurrentLayer = 26L,
		dispidDeleteCurrentLayer = 25L,
		dispidClearCurrentLayer = 24L,
		dispidInsertLayer = 23L,
		dispidControlToCanvas = 22L,
		dispidCanvasToControl = 21L,
		dispidCopyImage = 20L,
		dispidSaveImage = 19L,
		dispidClear = 18L,
		dispidRedraw = 17L,
		dispidCanMouseWheelZoom = 16,
		dispidCanMouseDragPan = 15,
		dispidLayerCount = 14,
		dispidCurrentLayerNo = 13,
		dispidIsDrawCenter = 12,
		dispidIsDrawArrow = 11,
		dispidIsDrawAxis = 10,
		dispidIsDrawOrigin = 9,
		dispidIsDrawGrid = 8,
		dispidAxisScale = 7,
		dispidAxisColor = 6,
		dispidOriginSize = 5,
		dispidOriginColor = 4,
		dispidGridSize = 3,
		dispidGridColor = 2,
		dispidBackColor = 1
	};

protected:
	// IDLインタフェース - プロパティの実体
	// マウスドラッグによるパンの許可
	BOOL m_CanMouseDragPan;
	// マウスホイールによるズームの許可
	BOOL m_CanMouseWheelZoom;

private:
	// ズームの拡大縮小率
	static constexpr double ZOOM_UP_RATIO = (5.0 / 4.0);
	static constexpr double ZOOM_DOWN_RATIO = (4.0 / 5.0);
	// プロパティの初期値
	static constexpr COLORREF DEFAULT_BACK_COLOR = RGB(0, 0, 0);
	static constexpr COLORREF DEFAULT_GRID_COLOR = RGB(0, 255, 0);
	static constexpr double DEFAULT_GRID_SIZE = 10.0;
	static constexpr COLORREF DEFAULT_ORIGIN_COLOR = RGB(255, 255, 0);
	static constexpr long DEFAULT_ORIGIN_SIZE = 20;
	static constexpr COLORREF DEFAULT_AXIS_COLOR = RGB(255, 255, 255);
	static constexpr double DEFAULT_AXIS_SIZE = 50.0;
	static constexpr bool DEFAULT_IS_DRAW_GRID = true;
	static constexpr bool DEFAULT_IS_DRAW_ORIGIN = true;
	static constexpr bool DEFAULT_IS_DRAW_AXIS = false;
	static constexpr bool DEFAULT_IS_DRAW_ARROW = false;
	static constexpr bool DEFAULT_IS_DRAW_CENTER = false;
	static constexpr bool DEFAULT_IS_MOUSE_DRAG_PAN = true;
	static constexpr bool DEFAULT_IS_MOUSE_WHEEL_ZOOM = true;

	// メモリDC
	CDC m_memDC;
	// メモリDCの実体のビットマップ
	CBitmap m_memBmp;
	CBitmap* m_pOldBmp;

	// 描画管理オブジェクト
	std::unique_ptr<Drawer::Manager> m_pDrawManager;

	// ドラッグフラグ
	BOOL m_isDragging;
	// ドラッグ中基準点
	CPoint m_pntDraggingBasePos;

	// 初期化処理
	void Initialize();
	// 終了処理
	void Terminate();
};

