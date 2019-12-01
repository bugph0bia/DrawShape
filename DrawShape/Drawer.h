#pragma once

#include <vector>
#include <array>
#include <string>
#include <memory>
#include <limits>
#include <stdexcept>
#include <cmath>

// std::string/wstringのサンク対応
#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif

// std::numeric_limits<T>::max, min を使用するため
// このヘッダファイルをインクルードした後はマクロ関数のmax,minは使用できないので注意
#undef max
#undef min


namespace Drawer
{

//////////////// クラス前方宣言 ////////////////

class Manager;
class Canvas;
class Layer;
class Node;


//////////////// 列挙型 ////////////////

// 点の種類
enum class PointType {
	Pixel,		// ・
	Large,		// ●
	Triangle,	// △
};

// 線の限界値の種類
enum class LineLimitType {
	Finite,		// 有限
	Infinite,	// 無限
};

// 円弧の方向の種類
enum class ArcDirectionType {
	Left,	// 左
	Right,	// 右
};

// 塗りつぶしの種類
enum class FillType {
	NoFill,	// 塗りつぶさない
	Fill,	// 塗りつぶす
};


//////////////// クラス、構造体、型 ////////////////

// 座標データ構造体
template<typename T>
class Coord
{
public:
	// x座標
	T x;
	// y座標
	T y;

	// 2点間の距離
	double Length(const Coord& rhs) const
	{
		return sqrt(pow((rhs.x - this->x), 2) + pow((rhs.y - this->y), 2));
	}

	// TODO: []演算子使う？使わなければクラスではなく構造体でもよい

	// []演算子（左辺値参照版）：x, yメンバを[0], [1]で参照可能にする
	T& operator[](std::size_t idx) &
	{
		if(idx == 0) return x;
		else if(idx == 1) return y;
		else throw std::out_of_range("Drawer::Coord::operator[] : index is out of range.");
	}
	// []演算子（const左辺値参照版）：x, yメンバを[0], [1]で参照可能にする
	const T& operator[](std::size_t idx) const &
	{
		if(idx == 0) return x;
		else if(idx == 1) return y;
		else throw std::out_of_range("Drawer::Coord::operator[] : index is out of range.");
	}
	// []演算子（const右辺値参照版）：x, yメンバを[0], [1]で参照可能にする
	T operator[](std::size_t idx) const &&
	{
		if(idx == 0) return x;
		else if(idx == 1) return y;
		else throw std::out_of_range("Drawer::Coord::operator[] : index is out of range.");
	}
	// +=演算子
	Coord& operator+=(const Coord& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
	// -=演算子
	Coord& operator-=(const Coord& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}
	// *=演算子
	Coord& operator*=(double rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}
	// /=演算子
	Coord& operator/=(double rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		return *this;
	}
};

// +演算子
template<typename T>
Coord<T> operator+(const Coord<T>& lhs, const Coord<T>& rhs) { return Coord<T>(lhs) += rhs; }
// -演算子
template<typename T>
Coord<T> operator-(const Coord<T>& lhs, const Coord<T>& rhs) { return Coord<T>(lhs) -= rhs; }
// *演算子
template<typename T>
Coord<T> operator*(const Coord<T>& lhs, double rhs) { return Coord<T>(lhs) *= rhs; }
template<typename T>
Coord<T> operator*(double lhs, const Coord<T>& rhs) { return Coord<T>(rhs) *= lhs; }
// /演算子
template<typename T>
Coord<T> operator/(const Coord<T>& lhs, double rhs) { return Coord<T>(lhs) /= rhs; }

// 座標データコレクション
template<typename T, size_t N>
using Coords = std::array<Coord<T>, N>;


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

	// 幅を取得
	double GetWidth() const { return (max.x - min.x); }
	// 高さを取得
	double GetHeight() const { return (max.y - min.y); }

	// 相手の領域との交差領域を求める
	BoundingBox Intersect(const BoundingBox& rhs)
	{
		BoundingBox bbox;
		bbox.min.x = std::max(this->min.x, rhs.min.x);
		bbox.min.y = std::max(this->min.y, rhs.min.y);
		bbox.max.x = std::min(this->max.x, rhs.max.x);
		bbox.max.y = std::min(this->max.y, rhs.max.y);
		return bbox;
	}

	// 有効確認
	bool Verify() const
	{
		if (GetWidth() >= 0.0 && GetHeight() >= 0.0) return true;
		else return false;
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
	// 描画対象のデバイスコンテキスト
	CDC* m_pDC;
	// 描画対象のコントロールの矩形
	CRect m_rect;

	// 拡大縮小率
	double m_ratio;
	// オフセット
	// ※コントロール座標だが計算制度のためにdouble型とする
	Coord<double> m_offset;

	// 背景色
	COLORREF m_backColor;
	// グリッド色
	COLORREF m_gridColor;
	// グリッドサイズ
	double m_gridSize;
	// 原点色
	COLORREF m_originColor;
	// 原点サイズ
	long m_originSize;
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

public:
	// 定数
	// 拡大縮小率の初期値
	static constexpr double DEFAULT_RATIO = 1.0;
	// オフセットの初期値
	static constexpr Coord<double> DEFAULT_OFFSET = { 0.0, 0.0 };
	// 円周率
	static constexpr double	PI = 3.141592653589793;
	// 距離の許容誤差
	static constexpr double LENGTH_TOLERANCE = 0.00001;
	// 角度の許容誤差(rad)
	static constexpr double ANGLE_TOLERANCE = 0.00001;
	// 最小グリッド描画サイズ(コントロール座標)
	static constexpr long DRAW_GRID_SIZE_MIN = 5;
	// 最小軸目盛描画サイズ(コントロール座標)
	static constexpr long DRAW_AXIS_SCALE_MIN = 10;
	// 目盛の長さ(軸の片側の長さ)
	static constexpr long AXIS_SCALE_LENGTH = 3;
	// 原点の中央矩形の大きさ
	static constexpr long ORIGIN_CENTER_SIZE = 2;
	// 点の三角形描画時のサイズ
	static constexpr long TRIANGLE_POINT_SIZE = 3;
	// 点の強調時のサイズ
	static constexpr long LARGE_POINT_SIZE = 3;
	// 矢印の羽の軸からの角度(20°)
	static constexpr double ARROW_WING_ANGLE = 20.0 * PI / 180.0;

	// コンストラクタ
	Canvas(CDC* pDC, const CRect& rect);

	// 拡大縮小率
	void SetRatio(double val) { m_ratio = val;  }
	double GetRatio() const { return m_ratio; }
	// オフセット
	void SetOffset(Coord<double> val) { m_offset = val; }
	Coord<double> GetOffset() const { return m_offset; }

	// 背景色
	void SetBackColor(COLORREF val) { m_backColor = val; };
	COLORREF GetBackColor() const { return m_backColor; };
	// グリッド色
	void SetGridColor(COLORREF val) { m_gridColor = val; };
	COLORREF GetGridColor() const { return m_gridColor; };
	// グリッドサイズ
	void SetGridSize(double val) { m_gridSize = val; };
	double GetGridSize() const { return m_gridSize; };
	// 原点色
	void SetOriginColor(COLORREF val) { m_originColor = val; };
	COLORREF GetOriginColor() const { return m_originColor; };
	// 原点サイズ
	void SetOriginSize(long val) { m_originSize = val; };
	long GetOriginSize() const { return m_originSize; };
	// 軸色
	void SetAxisColor(COLORREF val) { m_axisColor = val; };
	COLORREF GetAxisColor() const { return m_axisColor; };
	// 軸スケール
	void SetAxisScale(double val) { m_axisScale = val; };
	double GetAxisScale() const { return m_axisScale; };
	// グリッド描画可否
	void SetIsDrawGrid(bool val) { m_isDrawGrid = val; };
	bool GetIsDrawGrid() const { return m_isDrawGrid; };
	// 原点描画可否
	void SetIsDrawOrigin(bool val) { m_isDrawOrigin = val; };
	bool GetIsDrawOrigin() const { return m_isDrawOrigin; };
	// 軸描画可否
	void SetIsDrawAxis(bool val) { m_isDrawAxis = val; };
	bool GetIsDrawAxis() const { return m_isDrawAxis; };
	// 矢印描画可否
	void SetIsDrawArrow(bool val) { m_isDrawArrow = val; };
	bool GetIsDrawArrow() const { return m_isDrawArrow; };
	// 円中心点描画可否
	void SetIsDrawCenter(bool val) { m_isDrawCenter = val; };
	bool GetIsDrawCenter() const { return m_isDrawCenter; };

	// 描画領域の再設定
	void Reset(CDC* pDC, const CRect& rect) { m_pDC = pDC; m_rect = rect; }
	// デバイスコンテキストを取得
	CDC* GetDC() const { return m_pDC; };
	// 描画領域を取得
	const CRect* GetRect() const { return &m_rect; };

	// 座標系変換：内部キャンバス座標系→コントロール座標系
	Coord<long> CanvasToControl(const Coord<double> &canvasCoord) const;
	// 座標系変換：コントロール座標系→内部キャンバス座標系
	Coord<double> ControlToCanvas(const Coord<long>& ctrlCoord) const;

	// 描画領域全体の座標を取得
	BoundingBox<double> GetCanvasArea() const;

	// 背景を塗りつぶす
	void FillBackground() const;
	// グリッド描画
	void DrawGrid() const;
	// 原点描画
	void DrawOrigin(Coord<double> base) const;
	// 軸描画
	void DrawAxis(Coord<double> base) const;
	// ピクセル(点)を描画
	void DrawPixel(const Coord<double>& point) const;
	// 点を三角形で描画
	void DrawTrianglePoint(const Coord<double>& point) const;
	// 点を強調して描画
	void DrawLargePoint(const Coord<double>& point) const;
	// 矢印先端描画
	void DrawArrowHead(const Coord<double>& start, const Coord<double>& end) const;
	// ベジエ曲線による円弧描画
	void DrawBezierArc(Coord<double> start, Coord<double> end, Coord<double> center, ArcDirectionType direction) const;

	// 描画内容をファイル保存(BMP/PNG/JPEG/GIF)
	bool SaveImage(const std::tstring& filePath) const;
	// 描画内容をクリップボードへコピー
	bool CopyImage(CWnd* pOwner) const;
};


// ペン/ブラシ一時変更クラス
class PenBrushChanger
{
private:
	// 対象のデバイスコンテキスト
	CDC* m_pDC;

	// ペン
	CPen m_pen;
	// 変更前ペン
	CPen* m_pOldPen;

	// ブラシ
	CBrush m_brush;
	// 変更前ブラシ
	CBrush* m_pOldBrush;

public:
	// コンストラクタ：ペン変更
	PenBrushChanger(CDC* pDC, LOGPEN logPen) :
		m_pDC(pDC),
		m_pOldPen(nullptr),
		m_pOldBrush(nullptr)
	{
		if (m_pDC) {
			// ペン変更
			m_pen.CreatePenIndirect(&logPen);
			m_pOldPen = m_pDC->SelectObject(&m_pen);
		}
	}

	// コンストラクタ：ブラシ変更
	PenBrushChanger(CDC* pDC, LOGBRUSH logBrush) :
		m_pDC(pDC),
		m_pOldPen(nullptr),
		m_pOldBrush(nullptr)
	{
		if (m_pDC) {
			// ペン変更
			m_brush.CreateBrushIndirect(&logBrush);
			m_pOldBrush = m_pDC->SelectObject(&m_brush);
		}
	}

	// コピーコンストラクタ
	PenBrushChanger(const PenBrushChanger&) = delete;
	// コピー代入演算子
	PenBrushChanger& operator=(const PenBrushChanger&) = delete;

	// デストラクタ
	~PenBrushChanger()
	{
		if (m_pDC) {
			// ペンを元に戻す
			if (m_pOldPen) m_pDC->SelectObject(m_pOldPen);
			// ブラシを元に戻す
			if (m_pOldBrush) m_pDC->SelectObject(m_pOldBrush);
		}
	}
};

// ノードクラス
class Node
{
protected:
	// キャンバスオブジェクト
	const Canvas& m_canvas;
	// ペン
	LOGPEN m_pen;
	// ブラシ
	LOGBRUSH m_brush;

	// 形状が描画領域に含まれるかチェック
	bool IsIncludeCanvas() const;

public:
	// コンストラクタ
	Node(Manager* pManager);
	// コンストラクタ
	Node(const Canvas& canvas, const LOGPEN& pen, const LOGBRUSH& brush) :
		m_canvas(canvas),
		m_pen(pen),
		m_brush(brush)
	{
	}

	// デストラクタ
	virtual ~Node() {}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox() const { return BoundingBox<double>(); }
	// 描画
	virtual void Draw() {}
};


// ノード派生クラス：グリッド
class NodeGrid : public Node
{
public:
	// コンストラクタ
	NodeGrid(Manager* pManager) :
		Node(pManager)
	{
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// 描画
	virtual void Draw() override;
};


// ノード派生クラス：原点
class NodeOrigin : public Node
{
private:
	// 座標データ
	Coord<double> m_point;

public:
	// コンストラクタ
	NodeOrigin(Manager* pManager, const Coord<double>& point) :
		Node(pManager),
		m_point(point)
	{
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// 描画
	virtual void Draw() override;
};


// ノード派生クラス：軸
class NodeAxis : public Node
{
private:
	// 座標データ
	Coord<double> m_point;

public:
	// コンストラクタ
	NodeAxis(Manager* pManager, const Coord<double>& point) :
		Node(pManager),
		m_point(point)
	{
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// 描画
	virtual void Draw() override;
};


// ノード派生クラス：点
class NodePoint : public Node
{
private:
	// 座標データ
	Coord<double> m_point;
	// 点の種類
	PointType m_pointType;

public:
	// コンストラクタ
	NodePoint(Manager* pManager, const Coord<double>& point, PointType pointType) :
		Node(pManager),
		m_point(point),
		m_pointType(pointType)
	{
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// 描画
	virtual void Draw() override;
};


// ノード派生クラス：線
class NodeLine : public Node
{
private:
	// 座標データ
	Coords<double, 2> m_points;
	// 限界値の種類
	LineLimitType m_lineLimitType;

public:
	// コンストラクタ
	NodeLine(Manager* pManager, const Coords<double, 2>& points, LineLimitType lineLimitType) :
		Node(pManager),
		m_points(points),
		m_lineLimitType(lineLimitType)
	{
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// 描画
	virtual void Draw() override;
};


// ノード派生クラス：円弧
class NodeArc : public Node
{
private:
	// 座標データ
	Coords<double, 3> m_points;
	// 円弧の方向の種類
	ArcDirectionType m_arcDirectionType;

public:
	// コンストラクタ
	NodeArc(Manager* pManager, const Coords<double, 3>& points, ArcDirectionType arcDirectionType) :
		Node(pManager),
		m_points(points),
		m_arcDirectionType(arcDirectionType)
	{
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// 描画
	virtual void Draw() override;
};


// ノード派生クラス：円
class NodeCircle : public Node
{
private:
	// 座標データ
	Coord<double> m_point;
	// 半径
	double m_radius;
	// 塗りつぶしの種類
	FillType m_fillType;

public:
	// コンストラクタ
	NodeCircle(Manager* pManager, const Coord<double>& point, double radius, FillType fillType) :
		Node(pManager),
		m_point(point),
		m_radius(radius),
		m_fillType(fillType)
	{
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// 描画
	virtual void Draw() override;
};


// ノード派生クラス：多角形
class NodePolygon : public Node
{
private:
	// 座標データ
	std::vector<Coord<double>> m_points;
	// 塗りつぶしの種類
	FillType m_fillType;

public:
	// コンストラクタ
	NodePolygon(Manager* pManager, const std::vector<Coord<double>>& points, FillType fillType) :
		Node(pManager),
		m_points(points),
		m_fillType(fillType)
	{
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// 描画
	virtual void Draw() override;
};


// ノード派生クラス：扇形
class NodeSector : public Node
{
private:
	// 座標データ
	Coords<double, 3> m_points;
	// 内側の半径
	double m_innerRadius;
	// 円弧の方向の種類
	ArcDirectionType m_arcDirectionType;
	// 塗りつぶしの種類
	FillType m_fillType;

public:
	// コンストラクタ
	NodeSector(Manager* pManager, const Coords<double, 3>& points, double innerRadius, ArcDirectionType arcDirectionType, FillType fillType) :
		Node(pManager),
		m_points(points),
		m_innerRadius(innerRadius),
		m_arcDirectionType(arcDirectionType),
		m_fillType(fillType)
	{
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// 描画
	virtual void Draw() override;
};


// レイヤークラス
class Layer
{
private:
	// ノードコレクション
	std::vector<std::unique_ptr<Node>> m_nodes;

	// 描画フラグ
	bool m_isDraw;

public:
	// コンストラクタ
	Layer();

	// 初期化
	void Clear();

	// ノード追加
	void AddNode(Node* pNode) { m_nodes.push_back(std::unique_ptr<Node>(pNode)); }

	// 全形状の最小包含箱を算出
	BoundingBox<double> CalcBoundingBox() const;

	// 描画フラグ
	void SetIsDraw(bool val) { m_isDraw = val; }
	bool GetIsDraw() const { return m_isDraw; }

	// 描画
	void Draw();
};


// 描画管理クラス
class Manager
{
private:
	// 描画キャンバス
	Canvas m_canvas;

	// ベースレイヤー
	Layer m_baseLayer;
	// 描画レイヤーコレクション
	std::vector<std::unique_ptr<Layer>> m_layers;

	// カレントペン
	LOGPEN m_currentPen;
	// カレントブラシ
	LOGBRUSH m_currentBrush;

	// カレントレイヤー番号
	int m_currentLayerNo;

	// 全形状の最小包含箱を算出
	BoundingBox<double> CalcBoundingBox() const;

public:
	// 定数
	// デフォルトペン = ソリッド, 太さ1, 白色
	static constexpr LOGPEN DEFAULT_PEN = LOGPEN{ PS_SOLID, {1, 0}, RGB(0xFF, 0xFF, 0xFF) };
	// デフォルトブラシ = ソリッド, 白色, ハッチ無し
	static constexpr LOGBRUSH DEFAULT_BRUSH = LOGBRUSH{ BS_SOLID, RGB(0xFF, 0xFF, 0xFF), 0 };
	// 最大拡大率
	static constexpr double RATIO_MAX = 50000.0;
	// 最小縮小率
	static constexpr double RATIO_MIN = 0.0001;
	// 最大移動量X,Y
	static constexpr double OFFSET_MAX = 999999.0;

	// コンストラクタ
	Manager(CDC* pDC, const CRect& rect);

	// コピーコンストラクタ
	Manager(const Manager&) = delete;
	// 代入演算子
	Manager& operator=(const Manager&) = delete;

	// キャンバスオブジェクト
	const Canvas& GetCanvas() const { return m_canvas; }

	// カレントペン
	void SetCurrentPen(const LOGPEN& val) { m_currentPen = val; };
	LOGPEN GetCurrentPen() const { return m_currentPen; };
	// カレントブラシ
	void SetCurrentBrush(const LOGBRUSH& val) { m_currentBrush = val; };
	LOGBRUSH GetCurrentBrush() const { return m_currentBrush; };

	// 背景色
	void SetBackColor(COLORREF val) { m_canvas.SetBackColor(val); };
	COLORREF GetBackColor() const { return m_canvas.GetBackColor(); };
	// グリッド色
	void SetGridColor(COLORREF val) { m_canvas.SetGridColor(val); };
	COLORREF GetGridColor() const { return m_canvas.GetGridColor(); };
	// グリッドサイズ
	void SetGridSize(double val) { m_canvas.SetGridSize(val); };
	double GetGridSize() const { return m_canvas.GetGridSize(); };
	// 原点色
	void SetOriginColor(COLORREF val) { m_canvas.SetOriginColor(val); };
	COLORREF GetOriginColor() const { return m_canvas.GetOriginColor(); };
	// 原点サイズ
	void SetOriginSize(long val) { m_canvas.SetOriginSize(val); };
	long GetOriginSize() const { return m_canvas.GetOriginSize(); };
	// 軸色
	void SetAxisColor(COLORREF val) { m_canvas.SetAxisColor(val); };
	COLORREF GetAxisColor() const { return m_canvas.GetAxisColor(); };
	// 軸スケール
	void SetAxisScale(double val) { m_canvas.SetAxisScale(val); };
	double GetAxisScale() const { return m_canvas.GetAxisScale(); };
	// グリッド描画可否
	void SetIsDrawGrid(bool val) { m_canvas.SetIsDrawGrid(val); };
	bool GetIsDrawGrid() const { return m_canvas.GetIsDrawGrid(); };
	// 原点描画可否
	void SetIsDrawOrigin(bool val) { m_canvas.SetIsDrawOrigin(val); };
	bool GetIsDrawOrigin() const { return m_canvas.GetIsDrawOrigin(); };
	// 軸描画可否
	void SetIsDrawAxis(bool val) { m_canvas.SetIsDrawAxis(val); };
	bool GetIsDrawAxis() const { return m_canvas.GetIsDrawAxis(); };
	// 矢印描画可否
	void SetIsDrawArrow(bool val) { m_canvas.SetIsDrawArrow(val); };
	bool GetIsDrawArrow() const { return m_canvas.GetIsDrawArrow(); };
	// 円中心点描画可否
	void SetIsDrawCenter(bool val) { m_canvas.SetIsDrawCenter(val); };
	bool GetIsDrawCenter() const { return m_canvas.GetIsDrawCenter(); };

	// カレントレイヤー番号
	void SetCurrentLayerNo(int val) { m_currentLayerNo = val; };
	int GetCurrentLayerNo() const { return m_currentLayerNo; };

	// レイヤー枚数を取得
	size_t GetLayerCount() const { return m_layers.size(); }

	// 描画領域の情報を再設定
	// OnSize()等のイベントで呼び出す必要あり
	void ResetCanvas(CDC* pDC, const CRect& rect) { m_canvas.Reset(pDC, rect); }

	// 初期化
	void Clear();

	// 描画
	void Draw(bool isDesignMode=false);

	// 拡大縮小
	bool Zoom(double coef, const Coord<long>& base);
	// パン
	bool Pan(const Coord<long>& move);
	// フィット
	void Fit(double shapeOccupancy);

	// グリッド追加
	void AddGrid()
	{
		m_layers[m_currentLayerNo]->AddNode(new NodeGrid(this));
	}
	// 原点追加
	void AddOrigin(const Coord<double>& point)
	{
		m_layers[m_currentLayerNo]->AddNode(new NodeOrigin(this, point));
	}
	// 軸追加
	void AddAxis(const Coord<double>& point)
	{
		m_layers[m_currentLayerNo]->AddNode(new NodeAxis(this, point));
	}
	// 点追加
	void AddPoint(const Coord<double>& point, PointType pointType)
	{
		m_layers[m_currentLayerNo]->AddNode(new NodePoint(this, point, pointType));
	}
	// 線追加
	void AddLine(const Coords<double, 2>& points, LineLimitType lineLimitType)
	{
		m_layers[m_currentLayerNo]->AddNode(new NodeLine(this, points, lineLimitType));
	}
	// 円弧追加
	void AddArc(const Coords<double, 3>& points, ArcDirectionType arcDirectionType)
	{
		m_layers[m_currentLayerNo]->AddNode(new NodeArc(this, points, arcDirectionType));
	}
	// 円追加
	void AddCircle(const Coord<double>& point, double radius, FillType fillType)
	{
		m_layers[m_currentLayerNo]->AddNode(new NodeCircle(this, point, radius, fillType));
	}
	// 多角形追加
	void AddPlygon(const std::vector<Coord<double>>& points, FillType fillType)
	{
		m_layers[m_currentLayerNo]->AddNode(new NodePolygon(this, points, fillType));
	}
	// 扇形追加
	void AddSector(const Coords<double, 3>& points, double innerRadius, ArcDirectionType arcDirectionType, FillType fillType)
	{
		m_layers[m_currentLayerNo]->AddNode(new NodeSector(this, points, innerRadius, arcDirectionType, fillType));
	}
};

}	// namespace Drawer
