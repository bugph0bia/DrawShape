#include "pch.h"
#include <atlimage.h>
#include "Drawer.h"

namespace Drawer
{

// コンストラクタ
Canvas::Canvas(CDC* pDC, const CRect& rect) :
	m_pDC(pDC),
	m_rect(rect),
	m_ratio(DEFAULT_RATIO),
	m_offset(DEFAULT_OFFSET)
{
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

// 描画領域全体の座標を取得
BoundingBox<double> Canvas::GetCanvasBox() const
{
	// 内部キャンバス座標値を計算
	BoundingBox<double> canvas;
	canvas.min = ControlToCanvas(Coord<long>{ 0, m_rect.Height() });
	canvas.max = ControlToCanvas(Coord<long>{ m_rect.Width(), 0 });
	return canvas;
}

// 背景を塗りつぶす
void Canvas::FillBackground(COLORREF color)
{
	m_pDC->FillSolidRect(m_rect, color);
}

// グリッド描画
void Canvas::DrawGrid(double size)
{
	// グリッドが細かすぎたら描画を行わない
	if ((size * m_ratio) < DRAW_GRID_SIZE_MIN) return;

	// キャンバス全体の座標を取得
	BoundingBox<double>	canvas = GetCanvasBox();

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
			// カレントペンの色を取得
			LOGPEN pen;
			GetDC()->GetCurrentPen()->GetLogPen(&pen);
			COLORREF color = pen.lopnColor;
			// 点描画
			GetDC()->SetPixel(point.x, point.y, color);
		}
	}
}

// 原点描画
void Canvas::DrawOrigin(Coord<double> base, long size)
{
	// 原点のコントロール座標を算出
	Coord<long> origin = CanvasToControl(base);

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
		base,
		ControlToCanvas(Coord<long>{ origin.x + size, origin.y })
	);

	// Y軸方向の矢印
	GetDC()->MoveTo(origin.x, origin.y);
	GetDC()->LineTo(origin.x, origin.y - size);
	// 矢印先端を描画
	DrawArrowHead(
		base,
		ControlToCanvas(Coord<long>{ origin.x, origin.y - size })
	);
}

// 軸描画
void Canvas::DrawAxis(Coord<double> base, double scale)
{
	// キャンバス全体の座標を取得
	BoundingBox<double>	canvas = GetCanvasBox();

	// 原点のコントロール座標を算出
	Coord<long> origin = CanvasToControl(base);

	// X軸が表示される場合
	if (canvas.min.y < base.y && base.y < canvas.max.y) {

		// X軸を描画
		GetDC()->MoveTo(0, origin.y);
		GetDC()->LineTo(m_rect.Width(), origin.y);
	}

	// Y軸が表示される場合
	if (canvas.min.x < base.x && base.x < canvas.max.x) {

		// 軸描画
		GetDC()->MoveTo(origin.x, 0);
		GetDC()->LineTo(origin.x, m_rect.Height());
	}

	// 目盛が細かすぎたら描画を行わない
	if ((scale * m_ratio) < DRAW_AXIS_SCALE_MIN) return;

	// X軸が表示される場合
	if (canvas.min.y < base.y && base.y < canvas.max.y) {
		// キャンバスに含まれる目盛のインデックス
		long xstart = static_cast<long>(canvas.min.x / scale);
		long xend = static_cast<long>(canvas.max.x / scale);

		// 目盛描画
		for (long x = xstart; x <= xend; x++) {
			// コントロール座標に変換
			Coord<long> point = CanvasToControl(Coord<double>{ x * scale, base.y });
			// 描画
			GetDC()->MoveTo(point.x, point.y - AXIS_SCALE_LENGTH);
			GetDC()->LineTo(point.x, point.y + AXIS_SCALE_LENGTH + 1);
		}
	}

	// Y軸が表示される場合
	if (canvas.min.x < base.x && base.x < canvas.max.x) {
		// キャンバスに含まれる目盛のインデックス
		long ystart = static_cast<long>(canvas.min.y / scale);
		long yend = static_cast<long>(canvas.max.y / scale);

		// 目盛描画
		for (long y = ystart; y <= yend; y++) {
			// コントロール座標に変換
			Coord<long> point = CanvasToControl(Coord<double> { base.x, y * scale });
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
// 始点と終点が一致する円弧で呼び出した場合の動作は未定義（そのようなケースは事前に円と見なすべき）
void Canvas::DrawBezierArc(Coord<double> start, Coord<double> end, Coord<double> center, ArcDirectionType direction)
{
	// 円弧方向が右回りなら左回りに変換する
	if (direction == ArcDirectionType::Right) std::swap(start, end);

	enum { START, END, CENTER };

	// 円弧の始点/終点/中心点を配列にまとめる
	Coords<double, 3> arc {start, end, center};

	// 中心点を原点(0, 0)の位置に移動した円弧を作成
	Coords<double, 3> oarc = arc;
	oarc[START].x -= center.x;
	oarc[START].y -= center.y;
	oarc[END].x -= center.x;
	oarc[END].y -= center.y;
	oarc[CENTER].x -= center.x;
	oarc[CENTER].y -= center.y;

	// 円弧の半径を算出
	double radius = sqrt(pow(oarc[START].x, 2) + pow(oarc[START].y, 2));

	// 始点と終点の象限(第n象限のn)を調べる
	std::array<int, 2> quadrant;
	for (int se = START; se <= END; se++) {
		// 第1象限
		if (oarc[se].x > 0.0 && oarc[se].y >= 0.0) quadrant[se] = 1;
		// 第2象限
		else if (oarc[se].x <= 0.0 && oarc[se].y > 0.0) quadrant[se] = 2;
		// 第3象限
		else if (oarc[se].x < 0.0 && oarc[se].y <= 0.0) quadrant[se] = 3;
		// 第4象限
		else quadrant[se] = 4;
	}

	// 便宜上、第n象限と第(n+4)象限を同じ意味と考える
	// （数学的には、象限の数nを `4を法とした合同な数` で置き換え可能と見なす）
	//   例：第2象限と第6象限は同じ象限と考える
	// その上で、終点の象限 >= 始点の象限 となるように調節することで、
	// 象限の数値の差で円弧がいくつの象限に跨るかを表せるようにする

	// 始点と終点が同一象限の場合
	if (quadrant[START] == quadrant[END]) {

		// 中心点→始点と中心点→終点のベクトル外積より、円弧が180度以上か判定する
		double outer = oarc[START].x * oarc[END].x - oarc[START].y * oarc[END].y;
		// 180度以上の円弧の場合
		if (outer < 0.0) {
			// 始点から終点は全ての象限を経由するため終点の象限を加算する
			quadrant[END] += 4;
		}
	}
	// 始点と終点が第4象限と第1象限を経由しており大小関係が入れ替わっている場合、
	// 終点の象限を加算する
	else if (quadrant[START] > quadrant[END]) {
		quadrant[END] += 4;
	}

	// ベジエ曲線を描くための点の配列
	std::vector<Coord<double>> bezierPoints;

	// 最初の点を格納
	bezierPoints.push_back(arc[START]);

	// 円弧が跨る象限を順に辿り、象限ごとにベジエ曲線で描く
	for (int quad = quadrant[START]; quad <= quadrant[END]; quad++) {
		// 途中の要素なら、円弧の最大最小値をもとに要素の終点を決定

		// その象限内のベジエ曲線の開始点
		Coord<double> here = bezierPoints.back();

		// その象限内のベジエ曲線の終端点
		Coord<double> next {0.0, 0.0};

		// 最後の象限の場合、終端点 = 円弧の終点
		if (quad == quadrant[END]) {
			// 
			next = arc[END];
		}
		// 途中の象限の場合、終端点 = 次の象限との間の軸上の点
		else {
			switch (quad % 4) {
			// 第1象限
			case 1:
				next.x = arc[CENTER].x;
				next.y = arc[CENTER].y + radius;
				break;
			// 第2象限
			case 2:
				next.x = arc[CENTER].x - radius;
				next.y = arc[CENTER].y;
				break;
			// 第3象限
			case 3:
				next.x = arc[CENTER].x;
				next.y = arc[CENTER].y - radius;
				break;
			// 第4象限
			case 0:
				next.x = arc[CENTER].x + radius;
				next.y = arc[CENTER].y;
				break;
			}
		}

		// 中心点→ベジエ曲線の開始点の単位ベクトル
		Coord<double> vecHere;
		vecHere.x = (here.x - arc[CENTER].x) / radius;
		vecHere.y = (here.y - arc[CENTER].y) / radius;
		// 中心点→ベジエ曲線の終端点の単位ベクトル
		Coord<double> vecNext;
		vecNext.x = (next.x - arc[CENTER].x) / radius;
		vecNext.y = (next.y - arc[CENTER].y) / radius;

		// ベクトルの内積から象限内の円弧の角度を算出(90度以下)
		double inner = vecHere.x * vecNext.x + vecHere.y * vecNext.y;
		double angle = acos(inner);

		// 制御点の長さの係数を算出
		//   半径 * (4 / 3) * tan(角度 / 4)
		double coef = radius * (4.0 / 3.0) * tan(angle / 4.0);

		// 開始点の単位ベクトルを90°回転
		std::swap(vecHere.x, vecHere.y);
		vecHere.x *= -1;
		// ベジエ曲線の開始点側の制御点を算出
		Coord<double> ctrl1;
		ctrl1.x = vecHere.x * coef + here.x;
		ctrl1.y = vecHere.y * coef + here.y;

		// 終端点の単位ベクトルを-90°回転
		std::swap(vecNext.x, vecNext.y);
		vecNext.y *= -1;
		// ベジエ曲線の終端点側の制御点を算出
		Coord<double> ctrl2;
		ctrl2.x = vecNext.x * coef + next.x;
		ctrl2.y = vecNext.y * coef + next.y;

		// ベジエ曲線の点を追加
		bezierPoints.push_back(ctrl1);
		bezierPoints.push_back(ctrl2);
		bezierPoints.push_back(next);
	}

	// ベジエ曲線の点をコントロール座標に変換
	std::vector<POINT> drawPoints;
	for (auto& pb : bezierPoints) {
		auto pc = CanvasToControl(pb);
		drawPoints.push_back(POINT{ pc.x, pc.y });
	}

	// 描画
	GetDC()->PolyBezier(drawPoints.data(), drawPoints.size());
}

// 描画内容をファイル保存(BMP/PNG/JPEG/GIF)
bool Canvas::SaveImage(const std::tstring& filePath) const
{
	// DCから画像オブジェクトを取得してアタッチ
	ATL::CImage image;
	image.Attach(*(GetDC()->GetCurrentBitmap()));

	// 保存する
	// 第2引数省略で拡張子に従ったフォーマットとなる(BMP/PNG/JPEG/GIF)
	HRESULT hr = image.Save(filePath.c_str());

	// 画像オブジェクトをデタッチ
	image.Detach();

	return SUCCEEDED(hr);
}

// 描画内容をクリップボードへコピー
bool Canvas::CopyImage(CWnd* pOwner) const
{
	// DCの画像オブジェクトを取得
	CBitmap* pBitmapMain = GetDC()->GetCurrentBitmap();

	// BITMAP情報を取得
	BITMAP bitmapInfo;
	if (!pBitmapMain->GetBitmap(&bitmapInfo)) return false;

	// ビットマップのビットデータ取得
	std::size_t bitmapBytes = bitmapInfo.bmWidthBytes * bitmapInfo.bmHeight;
	std::vector<BYTE> bmpRaw(bitmapBytes);
	if (!pBitmapMain->GetBitmapBits(bitmapBytes, bmpRaw.data())) return false;

	// ビットマップを複製する
	// ※メンバ変数のHBITMAPハンドルをそのままクリップボードに設定してしまうと、
	// 　DCに対するの変更が即時反映されてしまうため
	CBitmap bitmapSub;
	bitmapSub.CreateBitmapIndirect(&bitmapInfo);
	bitmapSub.SetBitmapBits(bitmapBytes, bmpRaw.data());

	// クリップボードを開く
	if (!pOwner->OpenClipboard()) return false;

	bool ret = true;

	// クリップボードをクリア
	if (::EmptyClipboard()) {
		// キャンバスをビットマップとして転送する
		if (!::SetClipboardData(CF_BITMAP, static_cast<HBITMAP>(bitmapSub))) {
			ret = false;
		}
	}

	// クリップボードを閉じる
	::CloseClipboard();

	return ret;
}


// コンストラクタ
Node::Node(Manager* pManager) :
	m_canvas(pManager->GetCanvas()),
	m_pen(pManager->GetCurrentPen()),
	m_brush(pManager->GetCurrentBrush())
{
}

// コンストラクタ
Layer::Layer() :
	m_isDraw(false)
{
}

// 初期化
void Layer::Clear()
{
	// ノードコレクションをクリア
	m_nodes.clear();
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
Manager::Manager(CDC* pDC, const CRect& rect) :
	m_canvas(pDC, rect),
	m_baseLayer(),
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
	// ペン初期化
	m_currentPen = DEFAULT_PEN;
	// ブラシ初期化
	m_currentBrush = DEFAULT_BRUSH;

	// ベースレイヤーを初期化
	m_baseLayer.Clear();
	// グリッドを登録
	m_baseLayer.AddNode(new NodeGrid(this, m_gridSize));
	// 軸を登録
	m_baseLayer.AddNode(new NodeOrigin(this, Coord<double>{ 0.0, 0.0 }, m_originSize));
	// 原点を登録
	m_baseLayer.AddNode(new NodeAxis(this, Coord<double>{ 0.0, 0.0 }, m_axisScale));

	// レイヤーコレクションをクリア
	m_layers.clear();
	// カレントレイヤーを追加
	m_layers.push_back(std::make_unique<Layer>());
	m_currentLayerNo = 0;
}

// 描画
void Manager::Draw(bool isDesignMode/*=false*/)
{
	// デザインモード用の描画
	if (isDesignMode) {
		// ※これを使用する場合は以下を推奨
		//   ・本クラスのオブジェクトは一時的な変数とし、コンストラクタを Manager(デザインモード用のDC, Rect); で呼び出しておく
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

		// 拡大縮小率はコンストラクト時の初期値とする
		m_canvas.SetRatio(Canvas::DEFAULT_RATIO);
		// オフセットはコントロールの矩形の中央とする
		Coord<double> offset{ (double)m_canvas.GetRect()->CenterPoint().x, (double)m_canvas.GetRect()->CenterPoint().y };
		m_canvas.SetOffset(offset);

		// 背景色で塗りつぶす
		m_canvas.FillBackground(m_backColor);

		LOGPEN logPen = DEFAULT_PEN;
		LOGBRUSH logBrush = DEFAULT_BRUSH;

		// グリッド描画
		if (m_isDrawGrid) {
			// ペンとブラシを変更（デフォルトから色のみ変更）
			logPen.lopnColor = m_gridColor;
			logBrush.lbColor = m_gridColor;
			PenBrushChanger pc(m_canvas.GetDC(), logPen);
			PenBrushChanger bc(m_canvas.GetDC(), logBrush);

			m_canvas.DrawGrid(m_gridSize);
		}
		// 軸描画
		if (m_isDrawAxis) {
			// ペンとブラシを変更（デフォルトから色のみ変更）
			logPen.lopnColor = m_axisColor;
			logBrush.lbColor = m_axisColor;
			PenBrushChanger pc(m_canvas.GetDC(), logPen);
			PenBrushChanger bc(m_canvas.GetDC(), logBrush);

			m_canvas.DrawAxis(Coord<double>{0.0, 0.0}, m_axisScale);
		}
		// 原点描画
		if (m_isDrawOrigin) {
			// ペンとブラシを変更（デフォルトから色のみ変更）
			logPen.lopnColor = m_originColor;
			logBrush.lbColor = m_originColor;
			PenBrushChanger pc(m_canvas.GetDC(), logPen);
			PenBrushChanger bc(m_canvas.GetDC(), logBrush);

			m_canvas.DrawOrigin(Coord<double>{0.0, 0.0}, m_originSize);
		}
	}
	// 通常の描画
	else {
		// 背景を塗りつぶす
		m_canvas.FillBackground(m_backColor);

		// ベースレイヤーを描画
		m_baseLayer.Draw();

		// 全レイヤーを描画
		for (auto& pLayer : m_layers) {
			pLayer->Draw();
		}

		/*
		TODO: これはコントロール側に行わせる予定
			// 描画イベント発行
			m_pCtrl->InvalidateRect(nullptr, FALSE);	// 領域無効化
			m_pCtrl->UpdateWindow();					// 再描画命令
		*/
	}
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

/*
TODO: これはコントロール側に行わせる予定

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
*/

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
			static_cast<double>(m_canvas.GetRect()->CenterPoint().x),
			static_cast<double>(m_canvas.GetRect()->CenterPoint().y)
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
		if (m_canvas.GetRect()->Width() != 0.0) {
			cnvsAspect = static_cast<double>(m_canvas.GetRect()->Height()) / m_canvas.GetRect()->Width();
		}

		// 拡大縮小率を算出
		// 描画領域に対して形状が縦長なら縦方向を基準とする
		if (bbox.GetWidth() == 0.0 || shapeAspect > cnvsAspect) {
			// (描画領域サイズ / 形状サイズ) * 形状の占有率
			ratio = (m_canvas.GetRect()->Height() / bbox.GetHeight()) * shapeOccupancy;
		}
		// 描画領域に対して形状が横長なら横方向を基準とする
		else {
			// (描画領域サイズ / 形状サイズ) * 形状の占有率
			ratio = (m_canvas.GetRect()->Width() / bbox.GetWidth()) * shapeOccupancy;
		}

		// オフセットを算出
		offset.x = (m_canvas.GetRect()->Width() - bbox.GetWidth()) * ratio / 2.0 - bbox.min.x * ratio;
		double y = (m_canvas.GetRect()->Height() - bbox.GetHeight()) * ratio / 2.0 - bbox.min.y * ratio;
		offset.y = m_canvas.GetRect()->Height() - y;	// Y軸反転
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


// 形状の最小包含箱を算出
BoundingBox<double> NodeGrid::CalcBoundingBox() const
{
	// TODO:
	return BoundingBox<double>();
}
// 形状が描画エリアに含まれるかチェック
bool NodeGrid::IsIncludeDrawArea() const
{
	// TODO:
	return false;
}
// 描画
void NodeGrid::Draw()
{
	// TODO:
}


// 形状の最小包含箱を算出
BoundingBox<double> NodeOrigin::CalcBoundingBox() const
{
	// TODO:
	return BoundingBox<double>();
}
// 形状が描画エリアに含まれるかチェック
bool NodeOrigin::IsIncludeDrawArea() const
{
	// TODO:
	return false;
}
// 描画
void NodeOrigin::Draw()
{
	// TODO:
}


// 形状の最小包含箱を算出
BoundingBox<double> NodeAxis::CalcBoundingBox() const
{
	// TODO:
	return BoundingBox<double>();
}
// 形状が描画エリアに含まれるかチェック
bool NodeAxis::IsIncludeDrawArea() const
{
	// TODO:
	return false;
}
// 描画
void NodeAxis::Draw()
{
	// TODO:
}


// 形状の最小包含箱を算出
BoundingBox<double> NodePoint::CalcBoundingBox() const
{
	// TODO:
	return BoundingBox<double>();
}
// 形状が描画エリアに含まれるかチェック
bool NodePoint::IsIncludeDrawArea() const
{
	// TODO:
	return false;
}
// 描画
void NodePoint::Draw()
{
	// TODO:
}


// 形状の最小包含箱を算出
BoundingBox<double> NodeLine::CalcBoundingBox() const
{
	// TODO:
	return BoundingBox<double>();
}
// 形状が描画エリアに含まれるかチェック
bool NodeLine::IsIncludeDrawArea() const
{
	// TODO:
	return false;
}
// 描画
void NodeLine::Draw()
{
	// TODO:
}


// 形状の最小包含箱を算出
BoundingBox<double> NodeArc::CalcBoundingBox() const
{
	// TODO:
	return BoundingBox<double>();
}
// 形状が描画エリアに含まれるかチェック
bool NodeArc::IsIncludeDrawArea() const
{
	// TODO:
	return false;
}
// 描画
void NodeArc::Draw()
{
	// TODO:
}


// 形状の最小包含箱を算出
BoundingBox<double> NodeCircle::CalcBoundingBox() const
{
	// TODO:
	return BoundingBox<double>();
}
// 形状が描画エリアに含まれるかチェック
bool NodeCircle::IsIncludeDrawArea() const
{
	// TODO:
	return false;
}
// 描画
void NodeCircle::Draw()
{
	// TODO:
}


// 形状の最小包含箱を算出
BoundingBox<double> NodePolygon::CalcBoundingBox() const
{
	// TODO:
	return BoundingBox<double>();
}
// 形状が描画エリアに含まれるかチェック
bool NodePolygon::IsIncludeDrawArea() const
{
	// TODO:
	return false;
}
// 描画
void NodePolygon::Draw()
{
	// TODO:
}


// 形状の最小包含箱を算出
BoundingBox<double> NodeSector::CalcBoundingBox() const
{
	// TODO:
	return BoundingBox<double>();
}
// 形状が描画エリアに含まれるかチェック
bool NodeSector::IsIncludeDrawArea() const
{
	// TODO:
	return false;
}
// 描画
void NodeSector::Draw()
{
	// TODO:
}


}	// namespace Drawer
