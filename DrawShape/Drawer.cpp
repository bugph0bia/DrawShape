#include "pch.h"
#include "Drawer.h"

namespace Drawer
{

// コンストラクタ
Canvas::Canvas(CDC& dc) :
	m_dc(dc),
	m_ratio(DEFAULT_RATIO),
	m_offset(DEFAULT_OFFSET)
{
	// デフォルトペン = ソリッド, 太さ1, 白色
	m_pen.CreatePen(PS_SOLID, 1, RGB(0xFF, 0xFF, 0xFF));
	// デフォルトブラシ = ソリッド, 白色
	LOGBRUSH logBrush{ BS_SOLID, RGB(0xFF, 0xFF, 0xFF), 0 };
	m_brush.CreateBrushIndirect(&logBrush);
}

// 座標系変換：コントロール座標系→内部キャンバス座標系
void Canvas::CanvasToControl(Coord<long> ctrlCoord, Coord<double>& canvasCoord) const
{
	// TODO:
}

// 座標系変換：内部キャンバス座標系→コントロール座標系
void Canvas::ControlToCanvas(Coord<double>& canvasCoord, Coord<long> ctrlCoord) const
{
	// TODO:
}

// 背景を塗りつぶす
void Canvas::FillBackground(COLORREF color)
{
	// TODO:
}

// グリッド描画
void Canvas::DrawGrid(COLORREF color, double size)
{
	// TODO:
}

// 原点描画
void Canvas::DrawOrigin(COLORREF color, double size)
{
	// TODO:
}

// 軸描画
void Canvas::DrawAxis(COLORREF color, double scale)
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
bool Canvas::SaveBitmap(const std::string filePath) const
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
Manager::Manager(COleControl& ctrl, CDC& dc) :
	m_ctrl(ctrl),
	m_canvas(dc),
	m_baseLayer(*this),
	m_backgroundColor(0),
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

// 拡大縮小
void Manager::Zoom(double ratio)
{
	// TODO:
}

// パン
void Manager::Pan(Coord<double> offset)
{
	// TODO:
}
// フィット
void Manager::Fit()
{
	// TODO:
}

// 上位コントロールの矩形を取得（クライアント座標）
CRect Manager::GetControlRect() const
{
	CRect r;
	m_ctrl.GetClientRect(&r);
	return r;
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










}	// namespace Drawer
