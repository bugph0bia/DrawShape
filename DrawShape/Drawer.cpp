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
	m_offset(DEFAULT_OFFSET),
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
	m_isDrawCenter(false)
{
}

// 座標系変換：内部キャンバス座標系→コントロール座標系
Coord<long> Canvas::CanvasToControl(const Coord<double> &canvasCoord) const
{
	Coord<long> ctrlCoord;
	// コントロール座標値を計算
	ctrlCoord.x = static_cast<long>(CanvasToControl(canvasCoord.x) + m_offset.x);
	ctrlCoord.y = static_cast<long>(-CanvasToControl(canvasCoord.y) + m_offset.y);	// Y軸反転
	return ctrlCoord;
}

// 座標系変換：コントロール座標系→内部キャンバス座標系
Coord<double> Canvas::ControlToCanvas(const Coord<long> &ctrlCoord) const
{
	// 内部キャンバス座標値を計算
	Coord<double> canvasCoord;
	canvasCoord.x = ControlToCanvas(ctrlCoord.x - m_offset.x);
	canvasCoord.y = -ControlToCanvas(ctrlCoord.y - m_offset.y);	// Y軸反転
	return canvasCoord;
}

// コレクション用の座標系変換：内部キャンバス座標系→コントロール座標系
Coords_v<long> Canvas::CanvasToControl(const Coords_v<double>& canvasCoords) const
{
	Coords_v<long> ctrlCoords;
	for (const auto& c : canvasCoords) {
		ctrlCoords.push_back(CanvasToControl(c));
	}
	return ctrlCoords;
}
// コレクション用の座標系変換：コントロール座標系→内部キャンバス座標系
//   ※テンプレート関数なのでヘッダ内で定義する必要あり
Coords_v<double> Canvas::ControlToCanvas(const Coords_v<long>& ctrlCoords) const
{
	Coords_v<double> canvasCoords;
	for (const auto& c : ctrlCoords) {
		canvasCoords.push_back(ControlToCanvas(c));
	}
	return canvasCoords;
}

// 描画領域全体の座標を取得
BoundingBox<double> Canvas::GetCanvasArea() const
{
	// 内部キャンバス座標値を計算
	BoundingBox<double> canvas;
	canvas.min = ControlToCanvas(Coord<long>(0, m_rect.Height()));
	canvas.max = ControlToCanvas(Coord<long>(m_rect.Width(), 0));
	return canvas;
}

// 背景を塗りつぶす
void Canvas::FillBackground() const
{
	m_pDC->FillSolidRect(m_rect, m_backColor);
}

// グリッド描画
void Canvas::DrawGrid() const
{
	// グリッド描画OFF
	if (m_isDrawGrid) return;
	// グリッドが細かすぎたら描画を行わない
	if ((CanvasToControl(m_gridSize)) < DRAW_GRID_SIZE_MIN) return;

	// キャンバス全体の座標を取得
	BoundingBox<double>	canvas = GetCanvasArea();

	// キャンバスに含まれるグリッドのインデックス
	long xstart = static_cast<long>(canvas.min.x / m_gridSize);
	long xend = static_cast<long>(canvas.max.x / m_gridSize);
	long ystart = static_cast<long>(canvas.min.y / m_gridSize);
	long yend = static_cast<long>(canvas.max.y / m_gridSize);

	// グリッド描画
	for (long y = ystart; y <= yend; y++) {
		for (long x = xstart; x <= xend; x++) {
			// 点を描画
			DrawPixel(Coord<double>(x * m_gridSize, y * m_gridSize));
		}
	}
}

// 原点描画
void Canvas::DrawOrigin(Coord<double> base) const
{
	// 原点描画OFF
	if (m_isDrawOrigin) return;

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
	GetDC()->LineTo(origin.x + m_originSize, origin.y);
	// 矢印先端を描画
	DrawArrowHead(
		Coords<double, 2>{
			base,
			ControlToCanvas(Coord<long>(origin.x + m_originSize, origin.y))
		}
	);

	// Y軸方向の矢印
	GetDC()->MoveTo(origin.x, origin.y);
	GetDC()->LineTo(origin.x, origin.y - m_originSize);
	// 矢印先端を描画
	DrawArrowHead(
		Coords<double, 2>{
			base,
			ControlToCanvas(Coord<long>(origin.x, origin.y - m_originSize))
		}
	);
}

// 軸描画
void Canvas::DrawAxis(Coord<double> base) const
{
	// 軸描画OFF
	if (m_isDrawAxis) return;

	// キャンバス全体の座標を取得
	BoundingBox<double>	canvas = GetCanvasArea();

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
	if (CanvasToControl(m_axisScale) < DRAW_AXIS_SCALE_MIN) return;

	// X軸が表示される場合
	if (canvas.min.y < base.y && base.y < canvas.max.y) {
		// キャンバスに含まれる目盛のインデックス
		long xstart = static_cast<long>(canvas.min.x / m_axisScale);
		long xend = static_cast<long>(canvas.max.x / m_axisScale);

		// 目盛描画
		for (long x = xstart; x <= xend; x++) {
			// コントロール座標に変換
			Coord<long> point = CanvasToControl(Coord<double>(x * m_axisScale, base.y));
			// 描画
			GetDC()->MoveTo(point.x, point.y - AXIS_SCALE_LENGTH);
			GetDC()->LineTo(point.x, point.y + AXIS_SCALE_LENGTH + 1);
		}
	}

	// Y軸が表示される場合
	if (canvas.min.x < base.x && base.x < canvas.max.x) {
		// キャンバスに含まれる目盛のインデックス
		long ystart = static_cast<long>(canvas.min.y / m_axisScale);
		long yend = static_cast<long>(canvas.max.y / m_axisScale);

		// 目盛描画
		for (long y = ystart; y <= yend; y++) {
			// コントロール座標に変換
			Coord<long> point = CanvasToControl(Coord<double>(base.x, y * m_axisScale));
			// 描画
			GetDC()->MoveTo(point.x - AXIS_SCALE_LENGTH, point.y);
			GetDC()->LineTo(point.x + AXIS_SCALE_LENGTH + 1, point.y);
		}
	}
}

// ピクセル(点)を描画
void Canvas::DrawPixel(const Coord<double>& point) const
{
	// コントロール座標を算出
	Coord<long> ctrlPoint = CanvasToControl(point);

	// カレントペンの色を取得
	LOGPEN pen;
	GetDC()->GetCurrentPen()->GetLogPen(&pen);
	COLORREF color = pen.lopnColor;

	// ピクセル描画
	GetDC()->SetPixel(ctrlPoint.x, ctrlPoint.y, color);
}

// 点を三角形で描画
void Canvas::DrawTrianglePoint(const Coord<double>& point) const
{
	// 現在のペンを実線に変更
	LOGPEN logPen;
	GetDC()->GetCurrentPen()->GetLogPen(&logPen);
	logPen.lopnStyle = PS_SOLID;

	// ペンを変更
	PenBrushChanger pc(GetDC(), logPen);

	// 点のコントロール座標を算出
	Coord<long> ctrlPoint = CanvasToControl(point);

	// 描画（上→右下→左下）
	GetDC()->MoveTo(ctrlPoint.x, ctrlPoint.y - (TRIANGLE_POINT_SIZE + 1));
	GetDC()->LineTo(ctrlPoint.x - (TRIANGLE_POINT_SIZE + 1), ctrlPoint.y + TRIANGLE_POINT_SIZE);
	GetDC()->LineTo(ctrlPoint.x + (TRIANGLE_POINT_SIZE + 1), ctrlPoint.y + TRIANGLE_POINT_SIZE);
	GetDC()->LineTo(ctrlPoint.x, ctrlPoint.y - (TRIANGLE_POINT_SIZE + 1));
}

// 点を強調して描画
void Canvas::DrawLargePoint(const Coord<double>& point) const
{
	// 現在のペンを実線に変更
	LOGPEN logPen;
	GetDC()->GetCurrentPen()->GetLogPen(&logPen);
	logPen.lopnStyle = PS_SOLID;

	// 現在のブラシをソリッドに変更
	LOGBRUSH logBrush;
	GetDC()->GetCurrentBrush()->GetLogBrush(&logBrush);
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbStyle = 0;

	// ペンとブラシを変更
	PenBrushChanger pc(GetDC(), logPen);
	PenBrushChanger bc(GetDC(), logBrush);

	// 点のコントロール座標を算出
	Coord<long> ctrlPoint = CanvasToControl(point);

	// 描画
	GetDC()->Ellipse(
		ctrlPoint.x - TRIANGLE_POINT_SIZE,
		ctrlPoint.y - TRIANGLE_POINT_SIZE,
		ctrlPoint.x + TRIANGLE_POINT_SIZE,
		ctrlPoint.y + TRIANGLE_POINT_SIZE
	);
}

// 矢印先端描画
void Canvas::DrawArrowHead(const Coords<double, 2>& baseSegment) const
{
	// 現在のペンを実線に変更
	LOGPEN logPen;
	GetDC()->GetCurrentPen()->GetLogPen(&logPen);
	logPen.lopnStyle = PS_SOLID;

	// ペンを変更
	PenBrushChanger pc(GetDC(), logPen);

	// 先端のコントロール座標を算出
	Coord<long> tip = CanvasToControl(baseSegment[END]);

	// 線分の長さ
	double length = baseSegment[START].Length(baseSegment[END]);
	// 線分の単位ベクトルを算出
	Coord<double> unit;
	unit = (baseSegment[END] - baseSegment[START]) / length;

	// 左側→右側の羽
	for (double angle = PI - ARROW_WING_ANGLE; angle <= PI + ARROW_WING_ANGLE; angle += ARROW_WING_ANGLE * 2) {
		// 羽の先端の座標
		Coord<long> wing = CanvasToControl(
			Coord<double>(
				unit.x * cos(angle) - unit.y * sin(angle),
				unit.x * sin(angle) + unit.y * cos(angle)
			)
		);
		// 描画
		GetDC()->MoveTo(tip.x, tip.y);
		GetDC()->LineTo(wing.x, wing.y);
	}
}

// ベジエ曲線による円弧表現を算出
// 始点と終点が一致する円弧で呼び出した場合の動作は未定義（そのようなケースは事前に円と見なすべき）
Coords_v<double> Canvas::CalcBezierArc(Coords<double, 3> arc, ArcDirectionType direction) const
{
	// 円弧方向が右回りなら左回りに変換する
	if (direction == ArcDirectionType::Right) std::swap(arc[START], arc[END]);

	// 中心点を原点(0, 0)の位置に移動した円弧を作成
	Coords<double, 3> oarc = arc;
	oarc[START] -= arc[CENTER];
	oarc[END] -= arc[CENTER];
	oarc[CENTER] -= arc[CENTER];

	// 円弧の半径を算出
	double radius = oarc[START].Length(oarc[CENTER]);

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
	Coords_v<double> bezierPoints;

	// 最初の点を格納
	bezierPoints.push_back(arc[START]);

	// 円弧が跨る象限を順に辿り、象限ごとにベジエ曲線で描く
	for (int quad = quadrant[START]; quad <= quadrant[END]; quad++) {
		// 途中の要素なら、円弧の最大最小値をもとに要素の終点を決定

		// その象限内のベジエ曲線の開始点
		Coord<double> here = bezierPoints.back();

		// その象限内のベジエ曲線の終端点
		Coord<double> next;

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
		vecHere = (here - arc[CENTER]) / radius;
		// 中心点→ベジエ曲線の終端点の単位ベクトル
		Coord<double> vecNext;
		vecNext = (next - arc[CENTER]) / radius;

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
		ctrl1 = vecHere * coef + here;

		// 終端点の単位ベクトルを-90°回転
		std::swap(vecNext.x, vecNext.y);
		vecNext.y *= -1;
		// ベジエ曲線の終端点側の制御点を算出
		Coord<double> ctrl2;
		ctrl2 = vecNext * coef + next;

		// ベジエ曲線の点を追加
		bezierPoints.push_back(ctrl1);
		bezierPoints.push_back(ctrl2);
		bezierPoints.push_back(next);
	}

	return bezierPoints;
}

// ベジエ曲線による円弧描画
// 始点と終点が一致する円弧で呼び出した場合の動作は未定義（そのようなケースは事前に円と見なすべき）
void Canvas::DrawBezierArc(Coords<double, 3> arc, ArcDirectionType direction) const
{
	// ベジエ曲線の座標値配列を取得
	Coords_v<double> bezierPoints = CalcBezierArc(arc, direction);

	// ベジエ曲線の点をコントロール座標に変換
	std::vector<POINT> drawPoints;
	for (const auto& pb : bezierPoints) {
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
//   ※Managerクラスのメンバを使用するためヘッダファイルに定義不可
Node::Node(Manager* pManager) :
	m_canvas(pManager->GetCanvas()),
	m_pen(pManager->GetCurrentPen()),
	m_brush(pManager->GetCurrentBrush())
{
}


// 形状の最小包含箱を算出
BoundingBox<double> NodeGrid::CalcBoundingBox() const
{
	// グリッドの場合は最小包含箱は無い
	return BoundingBox<double>();
}

// 形状が描画領域に含まれるかチェック
bool NodeGrid::IsIncludeCanvas() const
{
	// グリッドは常に含まれる状態
	return true;
}

// 形状を描画
void NodeGrid::DrawContent()
{
	// グリッド描画
	m_canvas.DrawGrid();
}


// 形状の最小包含箱を算出
BoundingBox<double> NodeOrigin::CalcBoundingBox() const
{
	// 原点のサイズ
	double size = m_canvas.ControlToCanvas(m_canvas.GetOriginSize());

	// 原点の矩形を計算
	BoundingBox<double> bbox;
	bbox.min.x = m_point.x - size;
	bbox.min.y = m_point.y - size;
	bbox.max.x = m_point.x + size;
	bbox.max.y = m_point.y + size;
	return bbox;
}

// 形状を描画
void NodeOrigin::DrawContent()
{
	// 原点描画
	m_canvas.DrawOrigin(m_point);
}


// 形状の最小包含箱を算出
BoundingBox<double> NodeAxis::CalcBoundingBox() const
{
	// 軸の場合は最小包含箱を原点とする
	return BoundingBox<double>(m_point);
}

// 形状を描画
void NodeAxis::DrawContent()
{
	// 軸描画
	m_canvas.DrawAxis(m_point);
}


// 形状の最小包含箱を算出
BoundingBox<double> NodePoint::CalcBoundingBox() const
{
	// 最小包含箱を点とする
	return BoundingBox<double>(m_point);
}

// 形状を描画
void NodePoint::DrawContent()
{
	// 点の種類別に描画
	switch (m_pointType) {
	case PointType::Pixel:
		// ピクセル
		m_canvas.DrawPixel(m_point);
		break;
	case PointType::Triangle:
		// 三角形
		m_canvas.DrawTrianglePoint(m_point);
		break;
	case PointType::Large:
		// 強調
		m_canvas.DrawLargePoint(m_point);
		break;
	}
}


// 線分を取得
Coords<double, 2> NodeLine::Segment() const
{
	Coords<double, 2> points;

	// 有限の線分
	if (m_lineLimitType == LineLimitType::Finite) {
		points = m_points;
	}
	// 無限直線
	else {
		// 描画領域を取得
		BoundingBox<double> canvas = m_canvas.GetCanvasArea();

		// 鉛直線
		if (m_infiniteInfo.isVertical) {
			// x切片と描画領域の上下Y座標の範囲で線分を作成
			points[0].x = m_infiniteInfo.interceptX;
			points[0].y = canvas.min.y;
			points[1].x = m_infiniteInfo.interceptX;
			points[1].y = canvas.max.y;
		}
		// 非鉛直線
		else {
			// 描画領域の左右X座標の範囲で線分を作成 (y = c * x + i)
			points[0].x = canvas.min.x;
			points[0].y = m_infiniteInfo.coefficientX * canvas.min.x + m_infiniteInfo.interceptY;
			points[1].x = canvas.max.x;
			points[1].y = m_infiniteInfo.coefficientX * canvas.max.x + m_infiniteInfo.interceptY;
		}
	}
	return points;
}

// 形状の最小包含箱を算出
BoundingBox<double> NodeLine::CalcBoundingBox() const
{
	// TODO:
	// 線分を単純に最小包含箱として採用しているが
	// 実際には描画領域に含まれないのに最小包含箱が重なってしまう場合がある

	// 描画用の線分を取得
	Coords<double, 2> points = Segment();
	// 形状を構成する座標から最小包含箱を算出
	BoundingBox<double> bbox;
	for (const auto& p : points) {
		bbox += p;
	}
	return bbox;
}

// 形状を描画
void NodeLine::DrawContent()
{
	// 描画用の線分を取得
	auto points = Segment();
	// コントロール座標に変換
	auto ctrlPoints = m_canvas.CanvasToControl(Coords_v<double>(points.begin(), points.end()));
	// 描画
	m_canvas.GetDC()->MoveTo(ctrlPoints[0].x, ctrlPoints[0].y);
	m_canvas.GetDC()->LineTo(ctrlPoints[1].x, ctrlPoints[1].y);

	// 線分の場合、先端の矢印描画
	if (m_lineLimitType == LineLimitType::Finite && m_canvas.GetIsDrawArrow()) {
		// 矢印を描画
		m_canvas.DrawArrowHead(points);
	}
}


// 形状の最小包含箱を算出
BoundingBox<double> NodeArc::CalcBoundingBox() const
{
	// ベジエ曲線による座標値配列を取得
	Coords_v<double> bezierPoints = m_canvas.CalcBezierArc(m_points, m_arcDirectionType);

	// 座標値配列は 3n+1 個で構成されている
	// 円弧を象限分割したときの始点/終点の間に制御点が2つ挟まっている状態

	// 制御点を除いた円弧の始点と終点のみから最小包含箱を算出
	BoundingBox<double> bbox;
	for (auto itr = bezierPoints.cbegin(); itr != bezierPoints.cend(); itr += 3) {
		bbox += *itr;
	}
	return bbox;
}

// 形状を描画
void NodeArc::DrawContent()
{
	// ベジエ曲線で円弧を描画
	m_canvas.DrawBezierArc(m_points, m_arcDirectionType);

	// 円弧の先端の矢印描画
	if (m_canvas.GetIsDrawArrow()) {
		// 矢印を描くための軸
		Coords<double, 2> arrowAxis;

		// 回転角度を決定
		//   円弧方向が左：+90度
		//   円弧方向が右：-90度
		double angle = m_arcDirectionType == ArcDirectionType::Left ? PI : -PI;

		// 終点を基準に、中心点を上で決めた回転角度だけ回転して
		// 矢印を描くための軸の始点を算出
		arrowAxis[START] = m_points[END].Rotate(m_points[CENTER], angle);
		// 矢印を描くための軸の終点は円弧の終点
		arrowAxis[END] = m_points[END];

		// 矢印を描画
		m_canvas.DrawArrowHead(arrowAxis);
	}

	// 円弧の中心点を描画
	if (m_canvas.GetIsDrawCenter()) {
		m_canvas.DrawTrianglePoint(m_points[CENTER]);
	}
}

// 形状の最小包含箱を算出
BoundingBox<double> NodeCircle::CalcBoundingBox() const
{
	// 円の矩形を計算
	BoundingBox<double> bbox;
	bbox.min.x = m_point.x - m_radius;
	bbox.min.y = m_point.y - m_radius;
	bbox.max.x = m_point.x + m_radius;
	bbox.max.y = m_point.y + m_radius;
	return bbox;
}
// 形状を描画
void NodeCircle::DrawContent()
{
	// 塗りつぶしなし
	if (m_fillType == FillType::NoFill) {
		// 0度から180度の円弧
		Coords<double, 3> arc = { m_point, m_point, m_point };
		arc[START].x += m_radius;
		arc[END].x -= m_radius;
		// 半円を描画
		m_canvas.DrawBezierArc(arc, ArcDirectionType::Left);

		// 180度から360度の円弧
		std::swap(arc[START], arc[END]);
		// 半円を描画
		m_canvas.DrawBezierArc(arc, ArcDirectionType::Left);

		// 中心点を描画
		if (m_canvas.GetIsDrawCenter()) {
			// 三角形の点を描画
			m_canvas.DrawTrianglePoint(m_point);
		}
	}
	// 塗りつぶしあり
	else {
		// 円の外接する矩形の左上と右下の点を算出
		Coord<long> ctrlLeftUp = m_canvas.CanvasToControl(Coord<double>(m_point.x - m_radius, m_point.y + m_radius));
		Coord<long> ctrlRightDown = m_canvas.CanvasToControl(Coord<double>(m_point.x + m_radius, m_point.y - m_radius));
		// 円を描画
		m_canvas.GetDC()->Ellipse(ctrlLeftUp.x, ctrlLeftUp.y, ctrlRightDown.x, ctrlRightDown.y);
	}
}


// 形状の最小包含箱を算出
BoundingBox<double> NodePolygon::CalcBoundingBox() const
{
	// 形状を構成する座標から最小包含箱を算出
	BoundingBox<double> bbox;
	for (const auto& p : m_points) {
		bbox += p;
	}
	return bbox;
}
// 形状を描画
void NodePolygon::DrawContent()
{
	// 塗りつぶしなし
	if (m_fillType == FillType::NoFill) {
		// 形状の座標値をコントロール座標に変換
		Coords_v<long> ctrlPoints = m_canvas.CanvasToControl(m_points);

		// 最後の点に移動しておく
		m_canvas.GetDC()->MoveTo(ctrlPoints.rbegin()->x, ctrlPoints.rbegin()->y);
		// 多角形を描画
		for (const auto& here : ctrlPoints) {
			m_canvas.GetDC()->LineTo(here.x, here.y);
		}

		// 矢印を描画
		if (m_canvas.GetIsDrawArrow()) {
			// 最後の点を矢印の軸の始点とする
			Coord<double> s = *(m_points.rbegin());
			// 矢印の軸の終点として座標を順に取得
			for (const auto& e : m_points) {
				// 矢印を描画
				m_canvas.DrawArrowHead(Coords<double, 2>{s, e});
				// 次の始点を更新
				s = e;
			}
		}
	}
	// 塗りつぶしあり
	else {
		// 座標をPOINT型の配列へ変換
		std::vector<POINT> ctrlPoints;
		for (const auto& p : m_points) {
			auto cp = m_canvas.CanvasToControl(p);
			ctrlPoints.push_back(POINT{ cp.x, cp.y });
		}
		// 多角形を描画
		m_canvas.GetDC()->Polygon(ctrlPoints.data(), ctrlPoints.size());
	}
}


// 内側の円弧座標を算出
Coords<double, 3> NodeSector::CalcInnerArc() const
{
	// 中心点から始点と終点へのベクトルを算出
	Coord<double> vectorCS = m_points[START] - m_points[CENTER];
	Coord<double> vectorCE = m_points[END] - m_points[CENTER];

	// 外側の円弧半径を算出
	double outerRadius = m_points[CENTER].Length(m_points[START]);

	// 内側の円弧座標を算出（中心点からのベクトルを内側円弧の長さに変換し元の座標へ戻す）
	Coords<double, 3> innerArc;
	innerArc[START] = vectorCS / outerRadius * m_innerRadius + m_points[CENTER];
	innerArc[END] = vectorCE / outerRadius * m_innerRadius + m_points[CENTER];
	innerArc[CENTER] = m_points[CENTER];

	return innerArc;
}

// 扇形のリージョン（コントロール座標）を算出
void NodeSector::CalcSectorRgn(CRgn* sectorRgn) const
{
	// 外側の円弧（コントロール座標）
	Coords_v<long> ctrlOuterArc = m_canvas.CanvasToControl(Coords_v<double>(m_points.begin(), m_points.end()));
	// 左向きの円弧にしておく
	if (m_arcDirectionType == ArcDirectionType::Right) {
		std::swap(ctrlOuterArc[START], ctrlOuterArc[END]);
	}

	// DCを利用して外側の扇形のパスを作成
	m_canvas.GetDC()->AbortPath();
	// パスを開始
	m_canvas.GetDC()->BeginPath();
	// 円弧部分のパス→円弧終点から中心点へのパス→中心点から円弧始点へのパス
	m_canvas.DrawBezierArc(m_points, m_arcDirectionType);
	m_canvas.GetDC()->MoveTo(ctrlOuterArc[END].x, ctrlOuterArc[END].y);
	m_canvas.GetDC()->LineTo(ctrlOuterArc[CENTER].x, ctrlOuterArc[CENTER].y);
	m_canvas.GetDC()->LineTo(ctrlOuterArc[START].x, ctrlOuterArc[START].y);
	// パスを終了
	m_canvas.GetDC()->EndPath();

	// 外側の扇形リージョンを取得
	CRgn outerPieRgn;
	outerPieRgn.CreateFromPath(m_canvas.GetDC());

	// 内側円弧の半径をコントロール座標に変換
	long ctrlInnerRadius = static_cast<long>(m_canvas.CanvasToControl(m_innerRadius));

	// 内側の円形リージョンを取得
	CRgn innerCircleRgn;
	innerCircleRgn.CreateEllipticRgn(
		ctrlOuterArc[CENTER].x - ctrlInnerRadius,
		ctrlOuterArc[CENTER].y - ctrlInnerRadius,
		ctrlOuterArc[CENTER].x + ctrlInnerRadius,
		ctrlOuterArc[CENTER].y + ctrlInnerRadius
	);

	// 外側の扇形リージョン - 内側の円形リージョン
	sectorRgn->CombineRgn(&outerPieRgn, &innerCircleRgn, RGN_DIFF);
}

// 形状の最小包含箱を算出
BoundingBox<double> NodeSector::CalcBoundingBox() const
{
	// 扇形のリージョンから最小包含箱（コントロール座標）を取得
	CRgn sectorRgn;
	CalcSectorRgn(&sectorRgn);
	CRect ctrlRect;
	sectorRgn.GetRgnBox(&ctrlRect);

	// キャンバス座標系に変換
	BoundingBox<double> bbox;
	bbox.min = m_canvas.ControlToCanvas(Coord<long>(ctrlRect.left, ctrlRect.bottom));
	bbox.max = m_canvas.ControlToCanvas(Coord<long>(ctrlRect.right, ctrlRect.top));
	return bbox;
}
// 形状を描画
void NodeSector::DrawContent()
{
	// 塗りつぶしあり
	if (m_fillType == FillType::Fill) {
		// 扇形リージョンを取得して塗りつぶす
		CRgn sectorRgn;
		CalcSectorRgn(&sectorRgn);
		m_canvas.GetDC()->FillRgn(&sectorRgn, m_canvas.GetDC()->GetCurrentBrush());
	}

	// 外側と内側の円弧
	Coords<double, 3> outerArc = m_points;
	Coords<double, 3> innerArc = CalcInnerArc();

	// 円弧部分を描画
	m_canvas.DrawBezierArc(outerArc, m_arcDirectionType);
	m_canvas.DrawBezierArc(innerArc, m_arcDirectionType);

	// コントロール座標に変換して直線部分を描画
	Coords_v<long> ctrlOuterArc = m_canvas.CanvasToControl(Coords_v<double>(outerArc.begin(), outerArc.end()));
	Coords_v<long> ctrlInnerArc = m_canvas.CanvasToControl(Coords_v<double>(innerArc.begin(), innerArc.end()));
	m_canvas.GetDC()->MoveTo(ctrlOuterArc[START].x, ctrlOuterArc[START].y);
	m_canvas.GetDC()->LineTo(ctrlOuterArc[END].x, ctrlOuterArc[END].y);
	m_canvas.GetDC()->MoveTo(ctrlInnerArc[START].x, ctrlInnerArc[START].y);
	m_canvas.GetDC()->LineTo(ctrlInnerArc[END].x, ctrlInnerArc[END].y);
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
		for (const auto& pNode : m_nodes) {
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
		for (const auto& pNode : m_nodes) {
			pNode->Draw();
		}
	}
}


// コンストラクタ
Manager::Manager(CDC* pDC, const CRect& rect) :
	m_canvas(pDC, rect),
	m_baseLayer(),
	m_currentLayerNo(false)
{
	// 初期化
	Clear();
}

// 全形状の最小包含箱を算出
BoundingBox<double> Manager::CalcBoundingBox() const
{
	// ベースレイヤーの内容は含めない

	BoundingBox<double> bbox;
	// 全レイヤーの最小包含箱を合成
	for (const auto& pLayer : m_layers) {
		bbox += pLayer->CalcBoundingBox();
	}
	return bbox;
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
	m_baseLayer.AddNode(new NodeGrid(this));
	// 軸を登録
	m_baseLayer.AddNode(new NodeOrigin(this, Coord<double>(0.0, 0.0)));
	// 原点を登録
	m_baseLayer.AddNode(new NodeAxis(this, Coord<double>(0.0, 0.0)));

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
		m_canvas.FillBackground();

		LOGPEN logPen = DEFAULT_PEN;
		LOGBRUSH logBrush = DEFAULT_BRUSH;

		// グリッド描画
		{
			// ペンとブラシを変更（デフォルトから色のみ変更）
			logPen.lopnColor = m_canvas.GetGridColor();
			logBrush.lbColor = m_canvas.GetGridColor();
			PenBrushChanger pc(m_canvas.GetDC(), logPen);
			PenBrushChanger bc(m_canvas.GetDC(), logBrush);
			m_canvas.DrawGrid();
		}
		// 軸描画
		{
			// ペンとブラシを変更（デフォルトから色のみ変更）
			logPen.lopnColor = m_canvas.GetAxisColor();
			logBrush.lbColor = m_canvas.GetAxisColor();
			PenBrushChanger pc(m_canvas.GetDC(), logPen);
			PenBrushChanger bc(m_canvas.GetDC(), logBrush);
			m_canvas.DrawAxis(Coord<double>(0.0, 0.0));
		}
		// 原点描画
		{
			// ペンとブラシを変更（デフォルトから色のみ変更）
			logPen.lopnColor = m_canvas.GetOriginColor();
			logBrush.lbColor = m_canvas.GetOriginColor();
			PenBrushChanger pc(m_canvas.GetDC(), logPen);
			PenBrushChanger bc(m_canvas.GetDC(), logBrush);
			m_canvas.DrawOrigin(Coord<double>(0.0, 0.0));
		}
	}
	// 通常の描画
	else {
		// 背景を塗りつぶす
		m_canvas.FillBackground();

		// ベースレイヤーを描画
		m_baseLayer.Draw();

		// 全レイヤーを描画
		for (const auto& pLayer : m_layers) {
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
	if (OFFSET_MAX < fabs(m_canvas.ControlToCanvas(offset.x + move.x))) return false;
	if (OFFSET_MAX < fabs(m_canvas.ControlToCanvas(offset.y + move.y))) return false;

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
	// 描画する全形状の最小包含箱を取得
	BoundingBox<double> bbox = CalcBoundingBox();

	// 拡大縮小率
	double ratio;
	// オフセット
	Coord<double> offset;

	// 最小包含箱取得失敗
	if (!bbox.Verify()) {
		// 初期値とする
		ratio = Canvas::DEFAULT_RATIO;
		offset = Coord<double>(
			static_cast<double>(m_canvas.GetRect()->CenterPoint().x),
			static_cast<double>(m_canvas.GetRect()->CenterPoint().y)
		);
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


}	// namespace Drawer
