#pragma once

#include <vector>
#include <array>
#include <string>
#include <memory>
#include <limits>

// std::numeric_limits<T>::max, min を使用するため
#undef max
#undef min

namespace Drawer
{
// クラス前方宣言
class Manager;
class Canvas;
class Layer;
class Node;


// 座標データ構造体
template<typename T>
struct Coord
{
	T x;
	T y;
};


// 最小包含箱クラス
template<typename T>
class BoundingBox
{
public:
	// 最小座標
	Coord<T> min;
	// 最大座標
	Coord<T> max;

	// コンストラクタ
	BoundingBox()
	{
		// 最小座標を型の最大値で初期化
		min.x = std::numeric_limits<T>::max();
		min.y = std::numeric_limits<T>::max();
		// 最大座標を型の最小値で初期化
		max.x = std::numeric_limits<T>::min();
		max.y = std::numeric_limits<T>::min();
	}

	// +=演算子：最小包含箱同士を合成
	BoundingBox& operator+=(const BoundingBox& rhs)
	{
		// より範囲の大きい矩形になるように合成する
		this->min.x = std::min(this->min.x, rhs.min.x);
		this->min.y = std::min(this->min.y, rhs.min.y);
		this->max.x = std::max(this->max.x, rhs.max.x);
		this->max.y = std::max(this->max.y, rhs.max.y);
		return *this;
	}
	// +=演算子：最小包含箱と座標値を合成
	BoundingBox& operator+=(const Coord<T>& rhs)
	{
		// より範囲の大きい矩形になるように合成する
		this->min.x = std::min(this->min.x, rhs.x);
		this->min.y = std::min(this->min.y, rhs.y);
		this->max.x = std::max(this->max.x, rhs.x);
		this->max.y = std::max(this->max.y, rhs.y);
		return *this;
	}
};

// +演算子：最小包含箱同士を合成
template<typename T>
BoundingBox<T> operator+(const BoundingBox<T>& lhs, const BoundingBox<T>& rhs) { return BoundingBox<T>(lhs) += rhs; }
// +演算子：最小包含箱と座標値を合成
template<typename T>
BoundingBox<T> operator+(const BoundingBox<T>& lhs, const Coord<T>& rhs) { return BoundingBox<T>(lhs) += rhs; }
template<typename T>
BoundingBox<T> operator+(const Coord<T>& lhs, const BoundingBox<T>& rhs) { return BoundingBox<T>(rhs) += lhs; }


// 描画キャンバスクラス
class Canvas
{
private:
	// 定数
	// 拡大縮小率の初期値
	static constexpr double DEFAULT_RATIO = 0.0;
	// オフセットの初期値
	static constexpr Coord<double> DEFAULT_OFFSET = { 0.0, 0.0 };

	// 描画対象のデバイスコンテキスト
	CDC& m_dc;

	// カレントペンオブジェクト
	CPen m_pen;
	// カレントブラシオブジェクト
	CBrush m_brush;

	// 拡大縮小率
	double m_ratio;
	// オフセット
	Coord<double> m_offset;

public:
	// コンストラクタ
	Canvas(CDC& dc);

	// 拡大縮小率取得
	double GetRatio() const { return m_ratio; }
	// オフセットX取得
	double GetOffsetX() const { return m_offset.x; }
	// オフセットY取得
	double GetOffsetY() const { return m_offset.y; }

	// 座標系変換：コントロール座標系→内部キャンバス座標系
	void CanvasToControl(Coord<long> ctrlCoord, Coord<double>& canvasCoord) const;
	// 座標系変換：内部キャンバス座標系→コントロール座標系
	void ControlToCanvas(Coord<double>& canvasCoord, Coord<long> ctrlCoord) const;

	// 背景を塗りつぶす
	void FillBackground(COLORREF color);
	// グリッド描画
	void DrawGrid(COLORREF color, double size);
	// 原点描画
	void DrawOrigin(COLORREF color, double size);
	// 軸描画
	void DrawAxis(COLORREF color, double scale);
	// 点描画
	void DrawPoint();
	// 矢印描画
	void DrawArrow();
	// ベジエ曲線による円弧描画
	void DrawBezierArc();

	// 描画内容をファイル保存
	bool SaveBitmap(const std::string filePath) const;
	// 描画内容をクリップボードへコピー
	bool CopyBitmap() const;
};


// ノードクラス
class Node
{
	// 座標データ配列型
	template<typename T, size_t N>
	using coords_t = std::array<Coord<T>, N>;

private:
	// 描画管理オブジェクト
	Manager& m_mng;

public:
	// コンストラクタ
	Node(Manager& mng) : m_mng(mng) {};
	// デストラクタ
	virtual ~Node() {};

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox() const = 0;
	// 形状が描画エリアに含まれるかチェック
	virtual bool IsIncludeDrawArea() const = 0;
	// 描画
	virtual void Draw() = 0;

};


//// 線分ノードクラス
//class LineNode : public Node
//{
//private:
//	// 座標データ配列
//	coords_t<double, 2> m_datas;
//
//public:
//	// コンストラクタ
//	LineNode(Manager& mng);
//
//	// 形状の最小包含箱を算出
//	BoundingBox<double> CalcBoundingBox() const override;
//	// 形状が描画エリアに含まれるかチェック
//	bool IsIncludeDrawArea() const override;
//
//	// 描画
//	void Draw() override;
//};


// レイヤークラス
class Layer
{
private:
	// 描画管理オブジェクト
	Manager& m_mng;
	// ノードコレクション
	std::vector<std::unique_ptr<Node>> m_nodes;

	// 描画フラグ
	bool m_isDraw;

public:
	// コンストラクタ
	Layer(Manager& mng);

	// 初期化
	void Clear();

	// 全形状の最小包含箱を算出
	BoundingBox<double> CalcBoundingBox() const;

	// 描画フラグ設定
	void SetIsDraw(bool val) { m_isDraw = val; }
	// 描画フラグ取得
	bool GetIsDraw() const { return m_isDraw; }

	// 描画
	void Draw();
};


// 描画管理クラス
class Manager
{
private:
	// 上位コントロール
	COleControl& m_ctrl;
	// 描画キャンバス
	Canvas m_canvas;

	// ベースレイヤー
	Layer m_baseLayer;
	// 描画レイヤーコレクション
	std::vector<std::unique_ptr<Layer>> m_layers;

	// 背景色
	COLORREF m_backgroundColor;
	// グリッド色
	COLORREF m_gridColor;
	// グリッドサイズ
	double m_gridSize;
	// 原点色
	COLORREF m_originColor;
	// 原点サイズ
	double m_originSize;
	// 軸色
	COLORREF m_axisColor;
	// 軸スケール
	double m_axisScale;
	// グリッド描画可否
	bool m_isDrawGrid;
	// 原点描画可否
	bool m_isDrawOrigin;
	// 軸描画可否
	bool m_isDrawAxis;
	// 矢印描画可否
	bool m_isDrawArrow;
	// 円中心点描画可否
	bool m_isDrawCenter;

	// カレントレイヤー番号
	int m_currentLayerNo;

	// 全形状の最小包含箱を算出
	BoundingBox<double> CalcBoundingBox() const;

public:
	// コンストラクタ
	Manager(COleControl& ctrl, CDC& dc);

	// コピーコンストラクタ
	Manager(const Manager&) = delete;
	// 代入演算子
	Manager& operator=(const Manager&) = delete;

	// 初期化
	void Clear();

	// 描画
	void Draw();

	// 拡大縮小
	void Zoom(double ratio);
	// パン
	void Pan(Coord<double> offset);
	// フィット
	void Fit();

	// レイヤー枚数を取得
	size_t GetLayerCount() const { return m_layers.size(); }

	// 上位コントロールの矩形を取得（クライアント座標）
	CRect GetControlRect() const;
};

}	// namespace DrawShapeLib