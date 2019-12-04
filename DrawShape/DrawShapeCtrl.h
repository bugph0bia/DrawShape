#pragma once

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
	DECLARE_MESSAGE_MAP()

// ディスパッチ マップ
	DECLARE_DISPATCH_MAP()

// イベント マップ
	DECLARE_EVENT_MAP()

// ディスパッチ と イベント ID
public:
	enum {
		dispidAddAxis = 42L,
		dispidAddGrid = 41L,
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
		dispidCopyBitmap = 20L,
		dispidSaveBitmap = 19L,
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
		dispidBaseColor = 1
	};
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
protected:
	OLE_COLOR GetBaseColor();
	void SetBaseColor(OLE_COLOR newVal);
	OLE_COLOR GetGridColor();
	void SetGridColor(OLE_COLOR newVal);
	double GetGridSize();
	void SetGridSize(double newVal);
	OLE_COLOR GetOriginColor();
	void SetOriginColor(OLE_COLOR newVal);
	long GetOriginSize();
	void SetOriginSize(long newVal);
	OLE_COLOR GetAxisColor();
	void SetAxisColor(OLE_COLOR newVal);
	double GetAxisScale();
	void SetAxisScale(double newVal);
	VARIANT_BOOL GetIsDrawGrid();
	void SetIsDrawGrid(VARIANT_BOOL newVal);
	VARIANT_BOOL GetIsDrawOrigin();
	void SetIsDrawOrigin(VARIANT_BOOL newVal);
	VARIANT_BOOL GetIsDrawAxis();
	void SetIsDrawAxis(VARIANT_BOOL newVal);
	VARIANT_BOOL GetIsDrawArrow();
	void SetIsDrawArrow(VARIANT_BOOL newVal);
	VARIANT_BOOL GetIsDrawCenter();
	void SetIsDrawCenter(VARIANT_BOOL newVal);
	LONG GetCurrentLayerNo();
	void SetCurrentLayerNo(LONG newVal);
	LONG GetLayerCount();
	void SetLayerCount(LONG newVal);
	VARIANT_BOOL GetCanMouseDragPan();
	void SetCanMouseDragPan(VARIANT_BOOL newVal);
	VARIANT_BOOL GetCanMouseWheelZoom();
	void SetCanMouseWheelZoom(VARIANT_BOOL newVal);
	void Redraw();
	void Clear();
	VARIANT_BOOL SaveBitmap(BSTR filePath);
	VARIANT_BOOL CopyBitmap();
	void CanvasToControl(DOUBLE canvasX, DOUBLE canvasY, LONG* pCtrlX, LONG* pCtrlY);
	void ControlToCanvas(LONG ctrlX, LONG ctrlY, DOUBLE* pCanvasX, DOUBLE* pCanvasY);
	VARIANT_BOOL InsertLayer(LONG insertNo);
	void ClearCurrentLayer();
	LONG DeleteCurrentLayer();
	void SetEnableCurrentLayer(VARIANT_BOOL enable);
	VARIANT_BOOL GetEnableCurrentLayer();
	VARIANT_BOOL Zoom(DOUBLE ratio, LONG ctrlBaseX, LONG ctrlBaseY);
	VARIANT_BOOL Pan(LONG ctrlMoveX, LONG ctrlMoveY);
	void Fit(DOUBLE ratio);
	void ChangePen(LONG style, LONG width, OLE_COLOR color);
	void ChangeBrush(LONG style, OLE_COLOR color, LONG hatch);
	void AddLine(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey);
	void AddInfiniteLine2Point(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey);
	void AddInfiniteLine1PointAngle(DOUBLE x, DOUBLE y, DOUBLE angle);
	void AddPoint(DOUBLE x, DOUBLE y, LONG type);
	void AddArc(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey, DOUBLE cx, DOUBLE cy, VARIANT_BOOL left);
	void AddCircle(DOUBLE cx, DOUBLE cy, DOUBLE radius, VARIANT_BOOL fill);
	void AddPolygon(DOUBLE* points, LONG pointCount, VARIANT_BOOL fill);
	void AddSector(DOUBLE sx, DOUBLE sy, DOUBLE ex, DOUBLE ey, DOUBLE cx, DOUBLE cy, DOUBLE innerRadius, VARIANT_BOOL left, VARIANT_BOOL fill);
	void AddGrid(DOUBLE ox, DOUBLE oy);
	void AddAxis(DOUBLE ox, DOUBLE oy);
};

