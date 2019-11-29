#include "pch.h"
#include "Drawer.h"

namespace Drawer
{

// コンストラクタ
Canvas::Canvas(CDC* pDC) :
	m_pDC(pDC),
	m_ratio(DEFAULT_RATIO),
	m_offset(DEFAULT_OFFSET)
{
	// デフォルトペン = ソリッド, 太さ1, 白色
	m_pen.CreatePen(PS_SOLID, 1, RGB(0xFF, 0xFF, 0xFF));
	// デフォルトブラシ = ソリッド, 白色
	m_brush.CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));
}

// 座標系変換：内部キャンバス座標系→コントロール座標系
Coord<long> Canvas::CanvasToControl(const Coord<double> &canvasCoord) const
{
	Coord<long> ctrlCoord;
	// コントロール座標値を計算
	ctrlCoord.x = static_cast<long>(canvasCoord.x * m_ratio + m_offset.x);
	ctrlCoord.y = static_cast<long>(-canvasCoord.y * m_ratio + m_offset.y);	// Y軸反転
	return ctrlCoord;
}

// 座標系変換：コントロール座標系→内部キャンバス座標系
Coord<double> Canvas::ControlToCanvas(const Coord<long> &ctrlCoord) const
{
	// 内部キャンバス座標値を計算
	Coord<double> canvasCoord;
	canvasCoord.x = static_cast<double>((ctrlCoord.x - m_offset.x) / m_ratio);
	canvasCoord.y = -static_cast<double>((ctrlCoord.y - m_offset.y) / m_ratio);	// Y軸反転
	return canvasCoord;
}

// 座標系変換：コントロール座標系→内部キャンバス座標系
// コントロール全体をキャンバス座標に変換
BoundingBox<double> Canvas::ControlToCanvas(const CRect& rect) const
{
	// 内部キャンバス座標値を計算
	BoundingBox<double> canvas;
	canvas.min = ControlToCanvas(Coord<long>{ 0, rect.Height() });
	canvas.max = ControlToCanvas(Coord<long>{ rect.Width(), 0 });
	return canvas;
}

// 背景を塗りつぶす
void Canvas::FillBackground(COLORREF color, const CRect& rect)
{
	m_pDC->FillSolidRect(rect, color);
}

// グリッド描画
void Canvas::DrawGrid(COLORREF color, double size, const CRect& rect)
{
	// グリッドが細かすぎたら描画を行わない
	if ((size * m_ratio) < DRAW_GRID_SIZE_MIN) return;

	// ペンを変更
	PenBrushChanger pc(GetDC(), LOGPEN{ PS_SOLID, POINT{1, 0}, color });

	// キャンバス全体の座標を取得
	BoundingBox<double>	canvas = ControlToCanvas(rect);

	// キャンバスに含まれるグリッドのインデックス
	long xstart = static_cast<long>(canvas.min.x / size);
	long xend = static_cast<long>(canvas.max.x / size);
	long ystart = static_cast<long>(canvas.min.y / size);
	long yend = static_cast<long>(canvas.max.y / size);

	// グリッド描画
	for (long y = ystart; y <= yend; y++) {
		for (long x = xstart; x <= xend; x++) {
			// コントロール座標を算出
			Coord<long> point = CanvasToControl(Coord<double>{ x * size, y * size });
			// 点描画
			GetDC()->SetPixel(point.x, point.y, color);
		}
	}
}

// 原点描画
void Canvas::DrawOrigin(COLORREF color, long size)
{
	// ペンを変更
	PenBrushChanger pc(GetDC(), LOGPEN{ PS_SOLID, POINT{1, 0}, color });
	// ブラシを変更
	PenBrushChanger bc(GetDC(), LOGBRUSH{ BS_SOLID, color, 0 });

	// 原点のコントロール座標を算出
	Coord<long> origin = CanvasToControl(Coord<double>{ 0.0, 0.0 });

	// 中央の矩形を描画
	std::array<POINT, 4> points = {
		// 左上, 右上, 右下, 左下
		POINT{origin.x - ORIGIN_CENTER_SIZE, origin.y - ORIGIN_CENTER_SIZE},
		POINT{origin.x + ORIGIN_CENTER_SIZE, origin.y - ORIGIN_CENTER_SIZE},
		POINT{origin.x + ORIGIN_CENTER_SIZE, origin.y + ORIGIN_CENTER_SIZE},
		POINT{origin.x - ORIGIN_CENTER_SIZE, origin.y + ORIGIN_CENTER_SIZE}
	};
	GetDC()->Polygon(points.data(), points.size());

	// X軸方向の矢印
	GetDC()->MoveTo(origin.x, origin.y);
	GetDC()->LineTo(origin.x + size, origin.y);
	// 矢印先端を描画
	DrawArrowHead(
		Coord<double>{ 0.0, 0.0 },
		ControlToCanvas(Coord<long>{ origin.x + size, origin.y })
	);

	// Y軸方向の矢印
	GetDC()->MoveTo(origin.x, origin.y);
	GetDC()->LineTo(origin.x, origin.y - size);
	// 矢印先端を描画
	DrawArrowHead(
		Coord<double>{ 0.0, 0.0 },
		ControlToCanvas(Coord<long>{ origin.x, origin.y - size })
	);
}

// 軸描画
void Canvas::DrawAxis(COLORREF color, double scale, const CRect& rect)
{
	// ペンを変更
	PenBrushChanger pc(GetDC(), LOGPEN{ PS_SOLID, POINT{1, 0}, color });

	// キャンバス全体の座標を取得
	BoundingBox<double>	canvas = ControlToCanvas(rect);

	// 原点のコントロール座標を算出
	Coord<long> origin = CanvasToControl(Coord<double>{ 0.0, 0.0 });

	// X軸が表示される場合
	if (canvas.min.y < 0.0 && 0.0 < canvas.max.y) {

		// X軸を描画
		GetDC()->MoveTo(0, origin.y);
		GetDC()->LineTo(rect.Width(), origin.y);
	}

	// Y軸が表示される場合
	if (canvas.min.y < 0.0 && 0.0 < canvas.max.x) {

		// 軸描画
		GetDC()->MoveTo(origin.x, 0);
		GetDC()->LineTo(origin.x, rect.Height());
	}

	// 目盛が細かすぎたら描画を行わない
	if ((scale * m_ratio) < DRAW_AXIS_SCALE_MIN) return;

	// X軸が表示される場合
	if (canvas.min.y < 0.0 && 0.0 < canvas.max.y) {
		// キャンバスに含まれる目盛のインデックス
		long xstart = static_cast<long>(canvas.min.x / scale);
		long xend = static_cast<long>(canvas.max.x / scale);

		// 目盛描画
		for (long x = xstart; x <= xend; x++) {
			// コントロール座標に変換
			Coord<long> point = CanvasToControl(Coord<double>{ x * scale, 0.0 });
			// 描画
			GetDC()->MoveTo(point.x, point.y - AXIS_SCALE_LENGTH);
			GetDC()->LineTo(point.x, point.y + AXIS_SCALE_LENGTH + 1);
		}
	}

	// Y軸が表示される場合
	if (canvas.min.y < 0.0 && 0.0 < canvas.max.x) {
		// キャンバスに含まれる目盛のインデックス
		long ystart = static_cast<long>(canvas.min.y / scale);
		long yend = static_cast<long>(canvas.max.y / scale);

		// 目盛描画
		for (long y = ystart; y <= yend; y++) {
			// コントロール座標に変換
			Coord<long> point = CanvasToControl(Coord<double> { 0.0, y * scale });
			// 描画
			GetDC()->MoveTo(point.x - AXIS_SCALE_LENGTH, point.y);
			GetDC()->LineTo(point.x + AXIS_SCALE_LENGTH + 1, point.y);
		}
	}
}

// 点を強調描画
void Canvas::DrawLargePoint(const Coord<double>& point)
{
	// 現在のペンを実線に変更
	LOGPEN logPen;
	GetDC()->GetCurrentPen()->GetLogPen(&logPen);
	logPen.lopnStyle = PS_SOLID;

	// ペンを変更
	PenBrushChanger pc(GetDC(), logPen);

	// 点のコントロール座標を算出
	Coord<long> point_ctrl = CanvasToControl(point);

	// 描画（上→右下→左下）
	GetDC()->MoveTo(point_ctrl.x, point_ctrl.y - (LARGE_POINT_SIZE + 1));
	GetDC()->LineTo(point_ctrl.x - (LARGE_POINT_SIZE + 1), point_ctrl.y + LARGE_POINT_SIZE);
	GetDC()->LineTo(point_ctrl.x + (LARGE_POINT_SIZE + 1), point_ctrl.y + LARGE_POINT_SIZE);
	GetDC()->LineTo(point_ctrl.x, point_ctrl.y - (LARGE_POINT_SIZE + 1));
}

// 矢印先端描画
void Canvas::DrawArrowHead(const Coord<double>& start, const Coord<double>& end)
{
	// 現在のペンを実線に変更
	LOGPEN logPen;
	GetDC()->GetCurrentPen()->GetLogPen(&logPen);
	logPen.lopnStyle = PS_SOLID;

	// ペンを変更
	PenBrushChanger pc(GetDC(), logPen);

	// 先端のコントロール座標を算出
	Coord<long> tip = CanvasToControl(end);

	// 線分の長さ
	double length = sqrt(pow((end.x - start.x), 2) + pow((end.y - start.y), 2));
	// 線分の単位ベクトルを算出
	Coord<double> unit;
	unit.x = (end.x - start.x) / length;
	unit.y = (end.y - start.y) / length;

	// 左側→右側の羽
	for (double angle = PI - ARROW_WING_ANGLE; angle <= PI + ARROW_WING_ANGLE; angle += ARROW_WING_ANGLE * 2) {
		// 羽の先端の座標
		Coord<long> wing = CanvasToControl(
			Coord<double>{
				unit.x* cos(angle) - unit.y * sin(angle),
				unit.x* sin(angle) + unit.y * cos(angle)
			}
		);
		// 描画
		GetDC()->MoveTo(tip.x, tip.y);
		GetDC()->LineTo(wing.x, wing.y);
	}
}

// ベジエ曲線による円弧描画
void Canvas::DrawBezierArc()
{
	// TODO:
}


// 描画内容をファイル保存
bool Canvas::SaveBitmap(const std::string& filePath) const
{
	// TODO:
	return true;
}

// 描画内容をクリップボードへコピー
bool Canvas::CopyBitmap() const
{
	// TODO:
	return true;
}


// コンストラクタ
Layer::Layer(Manager& mng) :
	m_mng(mng),
	m_isDraw(false)
{
}

// 初期化
void Layer::Clear()
{
	// ノードコレクションをクリア
	m_nodes.clear();

	// TODO: カレントのペン設定ノードを追加
	// TODO: カレントのブラシ設定ノードを追加
}

// 全形状の最小包含箱を算出
BoundingBox<double> Layer::CalcBoundingBox() const
{
	BoundingBox<double> bbox;
	// 描画フラグONなら
	if (m_isDraw) {
		// 全ノードの最小包含箱を合成
		for (auto& pNode : m_nodes) {
			bbox += pNode->CalcBoundingBox();
		}
	}
	return bbox;
}

// 描画
void Layer::Draw()
{
	// 描画フラグONなら
	if (m_isDraw) {
		// 全ノードを描画
		for (auto& pNode : m_nodes) {
			pNode->Draw();
		}
	}
}


// コンストラクタ
Manager::Manager(COleControl* pCtrl, CDC* pDC) :
	m_pCtrl(pCtrl),
	m_canvas(pDC),
	m_baseLayer(*this),
	m_backColor(0),
	m_gridColor(0),
	m_gridSize(0.0),
	m_originColor(0),
	m_originSize(0),
	m_axisColor(0),
	m_axisScale(0),
	m_isDrawGrid(false),
	m_isDrawOrigin(false),
	m_isDrawAxis(false),
	m_isDrawArrow(false),
	m_isDrawCenter(false),
	m_currentLayerNo(false)
{
	// 初期化
	Clear();
}

// 初期化
void Manager::Clear()
{
	// ベースレイヤーを初期化
	m_baseLayer.Clear();

	// TODO: グリッドを登録
	// TODO: 軸を登録
	// TODO: 原点を登録

	// レイヤーコレクションをクリア
	m_layers.clear();
	// カレントレイヤーを追加
	m_layers.push_back(std::make_unique<Layer>(*this));
	m_currentLayerNo = 0;
}

// 描画
void Manager::Draw()
{
	// 背景を塗りつぶす
	m_canvas.FillBackground(m_backColor, GetControlRect());

	// ベースレイヤーを描画
	m_baseLayer.Draw();

	// 全レイヤーを描画
	for (auto& pLayer : m_layers) {
		pLayer->Draw();
	}

	// 描画イベント発行
	m_pCtrl->InvalidateRect(nullptr, FALSE);	// 領域無効化
	m_pCtrl->UpdateWindow();					// 再描画命令
}

// 描画(デザインモード用)
// ※この関数を使用する場合は以下を推奨
//   ・本クラスのオブジェクトは一時的な変数とし、コンストラクタを Manager(nullptr, デザインモード用DC); で呼び出しておく
//   ・少なくとも以下のプロパティを設定する
//       SetBackColor
//       SetGridColor
//       SetGridSize
//       SetOriginColor
//       SetOriginSize
//       SetAxisColor
//       SetAxisScale
//       SetIsDrawGrid
//       SetIsDrawOrigin
//       SetIsDrawAxis
//   ・引数 rect にデザインモード時のコントロールの矩形を渡す
void Manager::DrawDesignMode(const CRect& rect)
{
	// 拡大縮小率はコンストラクト時の初期値とするため変更しない
	// オフセットはコントロールの矩形の中央とする
	Coord<double> offset { (double)rect.CenterPoint().x, (double)rect.CenterPoint().y };
	m_canvas.SetOffset(offset);

	// 背景色で塗りつぶす
	m_canvas.FillBackground(m_backColor, rect);

	// グリッド描画
	if (m_isDrawGrid) m_canvas.DrawGrid(m_gridColor, m_gridSize, rect);
	// 軸描画
	if (m_isDrawAxis) m_canvas.DrawAxis(m_axisColor, m_axisScale, rect);
	// 原点描画
	if (m_isDrawOrigin) m_canvas.DrawOrigin(m_originColor, m_originSize);
}

// 拡大縮小
bool Manager::Zoom(double coef, const Coord<long>& base)
{
	// 現在の拡大縮小率に係数を掛ける
	double ratio = m_canvas.GetRatio() * coef;

	// これ以上拡大縮小できない
	if (ratio < RATIO_MIN || RATIO_MAX < ratio) return false;

	// 拡大縮小率を更新
	m_canvas.SetRatio(ratio);

	// 基準座標から描画オフセット量を算出して更新
	Coord<double> offset = m_canvas.GetOffset();
	offset.x = base.x - ((base.x - offset.x) * coef);
	offset.y = base.y - ((base.y - offset.y) * coef);
	m_canvas.SetOffset(offset);

	// 再描画
	Draw();

	return true;
}

// 拡大縮小（カーソル位置基準）
bool Manager::Zoom(double coef)
{
	// マウスカーソル位置
	CPoint cursor;
	::GetCursorPos(&cursor);

	// コントロールの矩形（スクリーン座標系）
	CRect rect;
	m_pCtrl->GetWindowRect(&rect);

	// マウスカーソルがコントロール内に無い
	if (!rect.PtInRect(cursor)) return false;

	// マウスカーソル位置をコントロールのクライアント座標に変換
	m_pCtrl->ScreenToClient(&cursor);

	// 拡大縮小
	return Zoom(coef, Coord<long>{ cursor.x, cursor.y });
}

// パン
bool Manager::Pan(const Coord<long>& move)
{
	// 現在の拡大縮小率
	double ratio = m_canvas.GetRatio();
	// 現在のオフセット
	Coord<double> offset = m_canvas.GetOffset();

	// これ以上オフセットできない
	if (OFFSET_MAX < fabs((offset.x + move.x) / ratio)) return false;
	if (OFFSET_MAX < fabs((offset.y + move.y) / ratio)) return false;

	// オフセット量を更新
	offset.x += move.x;
	offset.y += move.y;
	m_canvas.SetOffset(offset);

	// 再描画
	Draw();

	return true;
}

// フィット
void Manager::Fit(double shapeOccupancy)
{
	// コントロールの矩形を取得
	CRect rect;
	m_pCtrl->GetClientRect(&rect);

	// 最小包含箱取得
	BoundingBox<double> bbox = CalcBoundingBox();

	// 拡大縮小率
	double ratio;
	// オフセット
	Coord<double> offset;

	// 最小包含箱取得失敗
	if (!bbox.Verify()) {
		// 初期値とする
		ratio = Canvas::DEFAULT_RATIO;
		offset = Coord<double>{
			static_cast<double>(rect.CenterPoint().x),
			static_cast<double>(rect.CenterPoint().y)
		};
	}
	else {
		// 形状の縦横比を算出
		double shapeAspect = std::numeric_limits<double>::max();
		if (bbox.GetWidth() != 0.0) {
			shapeAspect = bbox.GetHeight() / bbox.GetWidth();
		}

		// 描画領域の縦横比
		double cnvsAspect = std::numeric_limits<double>::max();
		if (rect.Width() != 0.0) {
			cnvsAspect = static_cast<double>(rect.Height()) / rect.Width();
		}

		// 拡大縮小率を算出
		// 描画領域に対して形状が縦長なら縦方向を基準とする
		if (bbox.GetWidth() == 0.0 || shapeAspect > cnvsAspect) {
			// (描画領域サイズ / 形状サイズ) * 形状の占有率
			ratio = (rect.Height() / bbox.GetHeight()) * shapeOccupancy;
		}
		// 描画領域に対して形状が横長なら横方向を基準とする
		else {
			// (描画領域サイズ / 形状サイズ) * 形状の占有率
			ratio = (rect.Width() / bbox.GetWidth()) * shapeOccupancy;
		}

		// オフセットを算出
		offset.x = (rect.Width() - bbox.GetWidth()) * ratio / 2.0 - bbox.min.x * ratio;
		double y = (rect.Height() - bbox.GetHeight()) * ratio / 2.0 - bbox.min.y * ratio;
		offset.y = rect.Height() - y;	// Y軸反転
	}

	// 拡大縮小率とオフセットを更新
	m_canvas.SetRatio(ratio);
	m_canvas.SetOffset(offset);

	// 再描画
	Draw();
}

// 全形状の最小包含箱を算出
BoundingBox<double> Manager::CalcBoundingBox() const
{
	BoundingBox<double> bbox;
	// 全レイヤーの最小包含箱を合成
	for (auto& pLayer : m_layers) {
		bbox += pLayer->CalcBoundingBox();
	}
	return bbox;
}

// 上位コントロールの矩形を取得（クライアント座標）
CRect Manager::GetControlRect() const
{
	CRect r;
	m_pCtrl->GetClientRect(&r);
	return r;
}










}	// namespace Drawer
