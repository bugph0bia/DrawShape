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
	LOGBRUSH logBrush{ BS_SOLID, RGB(0xFF, 0xFF, 0xFF), 0 };
	m_brush.CreateBrushIndirect(&logBrush);
}

// 座標系変換：内部キャンバス座標系→コントロール座標系
Coord<long> Canvas::CanvasToControl(const Coord<double> &canvasCoord) const
{
	Coord<long> ctrlCoord;
	// コントロール座標値を計算
	ctrlCoord.x = (long)(canvasCoord.x * m_ratio + m_offset.x);
	ctrlCoord.y = (long)(-canvasCoord.y * m_ratio + m_offset.y);	// Y軸反転
	return ctrlCoord;
}

// 座標系変換：コントロール座標系→内部キャンバス座標系
Coord<double> Canvas::ControlToCanvas(const Coord<long> &ctrlCoord) const
{
	// 内部キャンバス座標値を計算
	Coord<double> canvasCoord;
	canvasCoord.x = (double)(ctrlCoord.x - m_offset.x) / m_ratio;
	canvasCoord.y = -(double)(ctrlCoord.y - m_offset.y) / m_ratio;	// Y軸反転
	return canvasCoord;
}

// 背景を塗りつぶす
void Canvas::FillBackground(COLORREF color, const CRect& rect)
{
	m_pDC->FillSolidRect(rect, color);
}

// グリッド描画
void Canvas::DrawGrid(COLORREF color, double size, const CRect& rect)
{
	// TODO:
}

// 原点描画
void Canvas::DrawOrigin(COLORREF color, double size)
{
	// TODO:
}

// 軸描画
void Canvas::DrawAxis(COLORREF color, double scale, const CRect& rect)
{
	// TODO:
}

// 点描画
void Canvas::DrawPoint()
{
	// TODO:
}

// 矢印描画
void Canvas::DrawArrow()
{
	// TODO:
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
	BoundingBox<double> bb;
	// 全ノードの最小包含箱を合成
	for (auto& pNode : m_nodes) {
		bb += pNode->CalcBoundingBox();
	}
	return bb;
}

// 描画
void Layer::Draw()
{
	// TODO:
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
	// TODO: 原点を登録
	// TODO: 軸を登録

	// レイヤーコレクションをクリア
	m_layers.clear();
	// カレントレイヤーを追加
	m_layers.push_back(std::make_unique<Layer>(*this));
	m_currentLayerNo = 0;
}

// 描画
void Manager::Draw()
{
	// TODO:
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
	// 原点描画
	if (m_isDrawOrigin) m_canvas.DrawOrigin(m_originColor, m_originSize);
	// 軸描画
	if (m_isDrawAxis) m_canvas.DrawAxis(m_axisColor, m_axisScale, rect);
}

// 拡大縮小
void Manager::Zoom(double ratio)
{
	// TODO:
}

// パン
void Manager::Pan(const Coord<double> &offset)
{
	// TODO:
}
// フィット
void Manager::Fit()
{
	// TODO:
}

// 全形状の最小包含箱を算出
BoundingBox<double> Manager::CalcBoundingBox() const
{
	BoundingBox<double> bb;
	// 全レイヤーの最小包含箱を合成
	for (auto& pLayer : m_layers) {
		bb += pLayer->CalcBoundingBox();
	}
	return bb;
}

// 上位コントロールの矩形を取得（クライアント座標）
CRect Manager::GetControlRect() const
{
	CRect r;
	m_pCtrl->GetClientRect(&r);
	return r;
}










}	// namespace Drawer
