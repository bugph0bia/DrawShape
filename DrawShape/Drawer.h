#pragma once

#include "pch.h"
#include <vector>
#include <array>
#include <string>

namespace Drawer
{

// クラス前方宣言
class Manager;
class Canvas;
class Layer;
class Node;

// 座標データ型
template<typename T>
using coord_t = std::array<T, 2>;
// 座標データ配列型
template<typename T, size_t N>
using coords_t = std::array<coord_t<T>, N>;


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
	std::vector<Layer> m_layers;

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
	void CalcBoundingBox(coord_t<double> &min, coord_t<double> &max) const;

public:
	// コンストラクタ
	Manager(COleControl &ctrl, CDC &dc);
	
	// コピーコンストラクタ
	Manager(const Manager&) = delete;
	// 代入演算子
	Manager& operator=(const Manager&) = delete;

	// 初期化（レイヤー１枚、描画データなし）
	void init();

	// 描画
	void Draw();

	// 拡大縮小
	void Zoom(double ratio);
	// パン
	void Pan(coord_t<double> offset);
	// フィット
	void Fit();

	// レイヤー枚数を取得
	size_t GetLayerCount() const { return m_layers.size(); }

	// 上位コントロールの矩形を取得（クライアント座標）
	CRect GetControlRect() const;
};


// 描画キャンバスクラス
class Canvas
{
private:
	// 描画対象のデバイスコンテキスト
	CDC &m_dc;

	// 拡大縮小率
	double m_ratio;
	// オフセット
	coord_t<double>& m_offset;

public:
	// コンストラクタ
	Canvas(CDC& dc);

	// 拡大縮小率取得
	double GetRatio() const { return m_ratio; }
	// オフセットX取得
	double GetOffsetX() const { return m_offsetX; }
	// オフセットY取得
	double GetOffsetY() const { return m_offsetY; }

	// 座標系変換：コントロール座標系→内部キャンバス座標系
	void CanvasToControl(coord_t<long> ctrlCoord, coord_t<double> &canvasCoord) const;
	// 座標系変換：内部キャンバス座標系→コントロール座標系
	void ControlToCanvas(coord_t<double>& canvasCoord, coord_t<long> ctrlCoord) const;

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


// レイヤークラス
class Layer
{
private:
	// 描画管理オブジェクト
	Manager& m_mng;
	// ノードコレクション
	std::vector<Node> m_nodes;

	// 描画フラグ
	bool m_isDraw;

public:
	// コンストラクタ
	Layer(Manager& mng);

	// 全形状の最小包含箱を算出
	void CalcBoundingBox(coord_t<double>& min, coord_t<double>& max) const;

	// 描画フラグ設定
	void SetIsDraw(bool val) { m_isDraw = val; }
	// 描画フラグ取得
	bool GetIsDraw() const { return m_isDraw; }

	// 描画
	void Draw();
};


// ノードクラス
class Node
{
private:
	// 描画管理オブジェクト
	Manager& m_mng;

public:
	// コンストラクタ
	Node(Manager& mng) : m_mng(mng) {};
	// デストラクタ
	virtual ~Node() {};

	// 形状の最小包含箱を算出
	virtual void CalcBoundingBox(coord_t<double>& min, coord_t<double>& max) const {}
	// 形状が描画エリアに含まれるかチェック
	virtual bool IsIncludeDrawArea() const {}
	// 描画
	virtual void Draw() {}

};

// 線分ノードクラス
class LineNode : public Node
{
private:
	// 座標データ配列
	coords_t<double, 2> m_datas;

public:
	// コンストラクタ
	LineNode(Manager& mng);

	// 形状の最小包含箱を算出
	void CalcBoundingBox(coord_t<double>& min, coord_t<double>& max) const override;
	// 形状が描画エリアに含まれるかチェック
	bool IsIncludeDrawArea() const override;

	// 描画
	void Draw() override;
};

}	// namespace DrawShapeLib
