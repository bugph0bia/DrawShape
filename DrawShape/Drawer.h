#pragma once

#include <vector>
#include <array>
#include <string>
#include <memory>
#include <limits>
#include <stdexcept>
#include <cmath>
#include <fstream>

#include "picojson.h"

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


//////////////// 定数 ////////////////
// 距離の許容誤差
static constexpr double LENGTH_TOLERANCE = 0.00001;
// 角度の許容誤差(rad)
static constexpr double ANGLE_TOLERANCE = 0.00001;
// 円周率
static constexpr double	PI = 3.141592653589793;
// 線分、円弧の座標コレクションのインデックス値
enum { START = 0, END = 1, CENTER = 2, BASE = 0 };

// JSONキー：レイヤー配列
static const std::tstring JSON_KEY_LAYERS = _T("Layers");
// JSONキー：描画フラグ
static const std::tstring JSON_KEY_ENABLE_DRAW = _T("EnableDraw");
// JSONキー：ノード配列
static const std::tstring JSON_KEY_NODES = _T("Nodes");
// JSONキー：ノードタイプ
static const std::tstring JSON_KEY_TYPE = _T("Type");
// JSONキー：ペン・ブラシ
static const std::tstring JSON_KEY_PEN = _T("Pen");
static const std::tstring JSON_KEY_BRUSH = _T("Brush");
static const std::tstring JSON_KEY_COLOR = _T("Color");
static const std::tstring JSON_KEY_WIDTH = _T("Width");
static const std::tstring JSON_KEY_STYLE = _T("Style");
// JSONキー: ノード情報
static const std::tstring JSON_KEY_POINTS = _T("Points");
static const std::tstring JSON_KEY_POINT_TYPE = _T("PointType");
static const std::tstring JSON_KEY_RADIUS = _T("Radius");
static const std::tstring JSON_KEY_LINE_LIMIT_TYPE = _T("LineLimitType");
static const std::tstring JSON_KEY_ARC_DIRECTION_TYPE = _T("ArcDirectionType");
static const std::tstring JSON_KEY_FILL_TYPE = _T("FillType");
// JSONキー: ノード情報（無限直線）
static const std::tstring JSON_KEY_INFINITE_INFO = _T("InfiniteInfo");
static const std::tstring JSON_KEY_IS_VERTICAL = _T("IsVertical");
static const std::tstring JSON_KEY_COEFFICIENT_X = _T("CoefficientX");
static const std::tstring JSON_KEY_INTERCEPT_Y = _T("InterceptY");
static const std::tstring JSON_KEY_INTERCEPT_X = _T("InterceptX");
// JSON値：ノードタイプ
static const std::tstring JSON_VALUE_TYPE_GRID = _T("Grid");
static const std::tstring JSON_VALUE_TYPE_ORIGIN = _T("Origin");
static const std::tstring JSON_VALUE_TYPE_AXIS = _T("Axis");
static const std::tstring JSON_VALUE_TYPE_POINT = _T("Point");
static const std::tstring JSON_VALUE_TYPE_LINE = _T("Line");
static const std::tstring JSON_VALUE_TYPE_ARC = _T("Arc");
static const std::tstring JSON_VALUE_TYPE_CIRCLE = _T("Circle");
static const std::tstring JSON_VALUE_TYPE_POLYGON = _T("Polygon");
static const std::tstring JSON_VALUE_TYPE_SECTOR = _T("Sector");


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

	// コンストラクタ
	Coord() : x(0), y(0) {}
	// コンストラクタ
	Coord(T x, T y) : x(x), y(y) {}

	// 2点間の距離
	double Length(const Coord& rhs) const
	{
		return sqrt(pow((rhs.x - this->x), 2) + pow((rhs.y - this->y), 2));
	}

	// 自身を中心点として点を回転
	Coord Rotate(const Coord& point, double angle)
	{
		// 自身から点へのベクトル
		Coord vec = point - *this;
		// ベクトルを回転
		Coord newPoint;
		newPoint.x = vec.x * cos(angle) - vec.y * sin(angle);
		newPoint.y = vec.x * sin(angle) + vec.y * cos(angle);
		// ベクトルと基準点から回転後の座標を算出
		newPoint += *this;
		return newPoint;
	}

	//// []演算子（左辺値参照版）：x, yメンバを[0], [1]で参照可能にする
	//T& operator[](std::size_t idx) &
	//{
	//	if(idx == 0) return x;
	//	else if(idx == 1) return y;
	//	else throw std::out_of_range("Drawer::Coord::operator[] : index is out of range.");
	//}
	//// []演算子（const左辺値参照版）：x, yメンバを[0], [1]で参照可能にする
	//const T& operator[](std::size_t idx) const &
	//{
	//	if(idx == 0) return x;
	//	else if(idx == 1) return y;
	//	else throw std::out_of_range("Drawer::Coord::operator[] : index is out of range.");
	//}
	//// []演算子（const右辺値参照版）：x, yメンバを[0], [1]で参照可能にする
	//T operator[](std::size_t idx) const &&
	//{
	//	if(idx == 0) return x;
	//	else if(idx == 1) return y;
	//	else throw std::out_of_range("Drawer::Coord::operator[] : index is out of range.");
	//}

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
template<typename T>
using Coords_v = std::vector<Coord<T>>;


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
	// コンストラクタ
	BoundingBox(Coord<T> min, Coord<T> max) : min(min), max(max) {}
	// コンストラクタ
	BoundingBox(Coord<T> point) : min(point), max(point) {}

	// 最小包含箱同士の和集合
	void Union(const BoundingBox& rhs)
	{
		// 相手が有効オブジェクト
		if (rhs.Verify()) {
			// 自身も有効オブジェクト
			if (this->Verify()) {
				// より範囲の大きい矩形になるように合成する
				this->min.x = std::min(this->min.x, rhs.min.x);
				this->min.y = std::min(this->min.y, rhs.min.y);
				this->max.x = std::max(this->max.x, rhs.max.x);
				this->max.y = std::max(this->max.y, rhs.max.y);
			}
			// 自身が無効オブジェクト
			else {
				// 相手をそのまま採用
				*this = rhs;
			}
		}
	}

	// 最小包含箱同士の積集合
	void Intersect(const BoundingBox& rhs)
	{
		// 自身と相手の両方が有効オブジェクト
		if (rhs.Verify() && this->Verify()) {
			// より範囲の小さい矩形になるように合成する
			this->min.x = std::max(this->min.x, rhs.min.x);
			this->min.y = std::max(this->min.y, rhs.min.y);
			this->max.x = std::min(this->max.x, rhs.max.x);
			this->max.y = std::min(this->max.y, rhs.max.y);
		}
		// 一方でも有効オブジェクトでなければ無効
		else {
			*this = BoundingBox();
		}
	}

	// +=演算子：最小包含箱同士の和集合
	BoundingBox& operator+=(const BoundingBox& rhs)
	{
		this->Union(rhs);
		return *this;
	}
	// +=演算子：最小包含箱と座標値の和集合
	BoundingBox& operator+=(const Coord<T>& rhs)
	{
		this->Union(BoundingBox(rhs, rhs));
		return *this;
	}

	// 幅を取得
	double GetWidth() const { return (max.x - min.x); }
	// 高さを取得
	double GetHeight() const { return (max.y - min.y); }

	// +=演算子：最小包含箱同士の積集合
	BoundingBox& operator*=(const BoundingBox& rhs)
	{
		this->Intersect(rhs);
		return *this;
	}

	// 有効確認
	bool Verify() const
	{
		if (GetWidth() >= 0.0 && GetHeight() >= 0.0) return true;
		else return false;
	}
};

// +演算子：最小包含箱同士の和集合
template<typename T>
BoundingBox<T> operator+(const BoundingBox<T>& lhs, const BoundingBox<T>& rhs) { return BoundingBox<T>(lhs) += rhs; }
// +演算子：最小包含箱と座標値の和集合
template<typename T>
BoundingBox<T> operator+(const BoundingBox<T>& lhs, const Coord<T>& rhs) { return BoundingBox<T>(lhs) += rhs; }
template<typename T>
BoundingBox<T> operator+(const Coord<T>& lhs, const BoundingBox<T>& rhs) { return BoundingBox<T>(rhs) += lhs; }
// *演算子：最小包含箱同士の積集合
template<typename T>
BoundingBox<T> operator*(const BoundingBox<T>& lhs, const BoundingBox<T>& rhs) { return BoundingBox<T>(lhs) *= rhs; }


// ユーティリティクラス
class Util
{
public:
	// 長さ0チェック
	static bool IsZeroLength(double value) { return std::abs(value) < LENGTH_TOLERANCE; }
	// 角度0チェック
	static bool IsZeroAngle(double value) { return std::abs(value) < ANGLE_TOLERANCE; }

	// 2点の同一チェック
	static bool IsSamePoint(const Coord<double>& p1, const Coord<double>& p2) { return IsZeroLength(p1.Length(p2)); }
	// 2直線の平行チェック
	static bool IsParallel(const Coords<double, 2>& l1, const Coords<double, 2>& l2)
	{
		// 単位ベクトル算出
		Coord<double> v1 = (l1[END] - l1[START]) / l1[START].Length(l1[END]);
		Coord<double> v2 = (l2[END] - l2[START]) / l2[START].Length(l2[END]);
		// 内積と外積を算出
		double inner = v1.x * v2.x + v1.y * v2.y;
		double outer = v1.x * v2.y - v2.x * v1.y;
		// 角度が0なら平行
		return IsZeroAngle(std::atan2(outer, inner));
	}

	// 円弧の整合性チェック
	static bool VerifyArc(const Coords<double, 3>& arc)
	{
		// 始点と終点が一致
		if (Util::IsSamePoint(arc[START], arc[END])) return false;
		// 始点と中心点が一致
		if (Util::IsSamePoint(arc[START], arc[CENTER])) return false;
		// 終点と中心点が一致
		if (Util::IsSamePoint(arc[END], arc[CENTER])) return false;
		// 半径が不一致
		if (!Util::IsZeroLength(
			arc[START].Length(arc[CENTER]) - arc[END].Length(arc[CENTER])
		)) return false;

		return true;
	}
};


// 設定情報構造体
struct SettingInfo
{
	// 背景色
	COLORREF backColor;
	// グリッド色
	COLORREF gridColor;
	// グリッドサイズ
	double gridSize;
	// 原点色
	COLORREF originColor;
	// 原点サイズ
	long originSize;
	// 軸色
	COLORREF axisColor;
	// 軸スケール
	double axisScale;
	// グリッド描画可否
	BOOL isDrawGrid;
	// 原点描画可否
	BOOL isDrawOrigin;
	// 軸描画可否
	BOOL isDrawAxis;
	// 矢印描画可否
	BOOL isDrawArrow;
	// 円中心点描画可否
	BOOL isDrawCenter;
};

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

public:
	// 定数
	// 拡大縮小率の初期値
	static constexpr double DEFAULT_RATIO = 1.0;
	// オフセットの初期値
	static inline const Coord<double> DEFAULT_OFFSET = Coord<double>();
	// 最小グリッド描画サイズ(コントロール座標)
	static constexpr long DRAW_GRID_SIZE_MIN = 10;
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
	// 矢印の羽の軸からの角度
	static constexpr double ARROW_WING_ANGLE = 26.0 * PI / 180.0;
	// 矢印の羽の長さ(コントロール座標)
	static constexpr long ARROW_WING_LENGTH = 10;

	// コンストラクタ
	Canvas();

	// 拡大縮小率
	void SetRatio(double val) { m_ratio = val;  }
	double GetRatio() const { return m_ratio; }
	// オフセット
	void SetOffset(Coord<double> val) { m_offset = val; }
	Coord<double> GetOffset() const { return m_offset; }

	// 描画領域の再設定
	void Reset(CDC* pDC, const CRect& rect, bool isResetRatioOffset)
	{
		m_pDC = pDC;
		m_rect = rect;
		if (isResetRatioOffset) {
			m_ratio = DEFAULT_RATIO;
			m_offset = Coord<double>(rect.CenterPoint().x, rect.CenterPoint().y);
		}
	}
	// デバイスコンテキストを取得
	CDC* GetDC() const { return m_pDC; };
	// 描画領域を取得
	const CRect* GetRect() const { return &m_rect; };

	// 長さ変換：内部キャンバス座標系→コントロール座標系
	//   ※コントロール座標はlong型だが計算途中で精度を落とさないためにdouble型で戻す
	double CanvasToControl(double canvasVal) const { return canvasVal * m_ratio; }
	// 長さ変換：コントロール座標系→内部キャンバス座標系
	//   ※コントロール座標はlong型だが計算途中で精度を落とさないためにdouble型で渡す
	double ControlToCanvas(double ctrlVal) const { return ctrlVal / m_ratio; }
	// 座標系変換：内部キャンバス座標系→コントロール座標系
	Coord<long> CanvasToControl(const Coord<double>& canvasCoord) const;
	// 座標系変換：コントロール座標系→内部キャンバス座標系
	Coord<double> ControlToCanvas(const Coord<long>& ctrlCoord) const;
	// コレクション用の座標系変換：内部キャンバス座標系→コントロール座標系
	Coords_v<long> CanvasToControl(const Coords_v<double>& canvasCoords) const;
	// コレクション用の座標系変換：コントロール座標系→内部キャンバス座標系
	Coords_v<double> ControlToCanvas(const Coords_v<long>& ctrlCoords) const;

	// 描画領域全体の座標を取得
	BoundingBox<double> GetCanvasArea() const;

	// 背景を塗りつぶす
	void FillBackground() const;
	// グリッド描画
	void DrawGrid(double size) const;
	// 原点描画
	void DrawOrigin(const Coord<double>& base, long size) const;
	// 軸描画
	void DrawAxis(const Coord<double>& base, double scale) const;
	// ピクセル(点)を描画
	void DrawPixel(const Coord<double>& point) const;
	// 点を三角形で描画
	void DrawTrianglePoint(const Coord<double>& point) const;
	// 点を強調して描画
	void DrawLargePoint(const Coord<double>& point) const;
	// 矢印先端描画
	void DrawArrowHead(const Coords<double, 2>& baseSegment, FillType fillType=FillType::NoFill) const;
	// ベジエ曲線による円弧表現を算出
	Coords_v<double> CalcBezierArc(Coords<double, 3> arc, ArcDirectionType direction) const;
	// ベジエ曲線による円弧描画
	void DrawBezierArc(Coords<double, 3> arc, ArcDirectionType direction) const;

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
	// 設定情報
	const SettingInfo& m_info;
	// キャンバスオブジェクト
	const Canvas& m_canvas;
	// ペン
	LOGPEN m_pen;
	// ブラシ
	LOGBRUSH m_brush;
	// JSON: ノードタイプ名称
	std::string m_typeName;

	// コンテンツデータ
	struct ContentsData {
		// 座標データ
		Coords_v<double> points;
		// 半径
		double radius;
		bool use_radius;
		// 点の種類
		PointType pointType;
		bool use_pointType;
		// 限界値の種類
		LineLimitType lineLimitType;
		bool use_lineLimitType;
		// 円弧の方向の種類
		ArcDirectionType arcDirectionType;
		bool use_arcDirectionType;
		// 塗りつぶしの種類
		FillType fillType;
		bool use_fillType;

		// コンストラクタ
		ContentsData() :
			radius(0),
			use_radius(0),
			pointType(PointType::Pixel),
			use_pointType(0),
			lineLimitType(LineLimitType::Finite),
			use_lineLimitType(0),
			arcDirectionType(ArcDirectionType::Left),
			use_arcDirectionType(0),
			fillType(FillType::NoFill),
			use_fillType(0) {}
	} m_data;

	// 形状が描画領域に含まれるかチェック
	//   ※形状ごとに必要に応じてオーバーライドする
	virtual bool IsIncludeCanvas() const
	{
		// 描画エリア
		BoundingBox<double> bboxCanvas = m_canvas.GetCanvasArea();
		// 形状の最小包含箱
		BoundingBox<double> bboxShape = CalcBoundingBox();
		// 規定の判定方法は、2つの領域が共通領域を持つかどうか
		return (bboxCanvas * bboxShape).Verify();
	}

	// 形状を描画
	//   ※形状ごとにオーバーライドする
	virtual void DrawContent() {}

public:
	// コンストラクタ
	Node(Manager* pManager);
	// コンストラクタ
	Node(const SettingInfo& info, const Canvas& canvas, const LOGPEN& pen, const LOGBRUSH& brush) :
		m_info(info),
		m_canvas(canvas),
		m_pen(pen),
		m_brush(brush) {}

	// デストラクタ
	virtual ~Node() {}

	// 形状の最小包含箱を算出
	//   ※形状ごとにオーバーライドする
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const { return BoundingBox<double>(); }

	// 形状の整合性チェック
	//   ※形状ごとにオーバーライドする
	virtual bool Verify() const { return true; }

	// 描画
	void Draw() {
		// 描画領域に含まれなければ描画しない
		if (!IsIncludeCanvas()) return;

		// ペンとブラシを変更
		PenBrushChanger pc(m_canvas.GetDC(), m_pen);
		PenBrushChanger bc(m_canvas.GetDC(), m_brush);
		// 形状を描画
		DrawContent();
	}

	// JSONデータ取得
	//   ※必要に応じて形状ごとにオーバーライドする
	virtual picojson::object GetContents() const;
	// JSONデータ設定
	//   ※必要に応じて形状ごとにオーバーライドする
	virtual bool SetContents(picojson::object& jNode);

	// タイプ名に応じたノードを作成
	static Node* FromTypeName(const std::string& typeName, Manager* pManager);
};


// ノード派生クラス：グリッド
class NodeGrid : public Node
{
protected:
	// 形状が描画領域に含まれるかチェック
	virtual bool IsIncludeCanvas() const override;
	// 形状を描画
	virtual void DrawContent() override;

public:
	// コンストラクタ
	NodeGrid(Manager* pManager) :
		Node(pManager)
	{
		// JSON: ノードタイプ名
		m_typeName = JSON_VALUE_TYPE_GRID;
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;
};


// ノード派生クラス：原点
class NodeOrigin : public Node
{
protected:
	// 形状が描画領域に含まれるかチェック
	virtual bool IsIncludeCanvas() const override;
	// 形状を描画
	virtual void DrawContent() override;

public:
	// コンストラクタ
	NodeOrigin(Manager* pManager) :
		Node(pManager)
	{
		// JSON: ノードタイプ名
		m_typeName = JSON_VALUE_TYPE_ORIGIN;
		// 原点はブラシの色をペンの色に合わせる
		m_brush.lbColor = m_pen.lopnColor;

		// コンテンツデータを準備
		m_data.points.resize(1);
	}

	// コンストラクタ
	NodeOrigin(Manager* pManager, const Coord<double>& point) :
		NodeOrigin(pManager)
	{
		// 内部データ登録
		m_data.points[BASE] = point;
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;
};


// ノード派生クラス：軸
class NodeAxis : public Node
{
protected:
	// 形状が描画領域に含まれるかチェック
	virtual bool IsIncludeCanvas() const override;
	// 形状を描画
	virtual void DrawContent() override;

public:
	// コンストラクタ
	NodeAxis(Manager* pManager) :
		Node(pManager)
	{
		// JSON: ノードタイプ名
		m_typeName = JSON_VALUE_TYPE_AXIS;

		// コンテンツデータを準備
		m_data.points.resize(1);
	}

	// コンストラクタ
	NodeAxis(Manager* pManager, const Coord<double>& point) :
		NodeAxis(pManager)
	{
		// 内部データ登録
		m_data.points[BASE] = point;
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;
};


// ノード派生クラス：点
class NodePoint : public Node
{
protected:
	// 形状を描画
	virtual void DrawContent() override;

public:
	// コンストラクタ
	NodePoint(Manager* pManager) :
		Node(pManager)
	{
		// JSON: ノードタイプ名
		m_typeName = JSON_VALUE_TYPE_POINT;

		// コンテンツデータを準備
		m_data.points.resize(1);
		m_data.use_pointType = true;
	}

	// コンストラクタ
	NodePoint(Manager* pManager, const Coord<double>& point, PointType pointType) :
		NodePoint(pManager)
	{
		// 内部データ登録
		m_data.points[BASE] = point;
		m_data.pointType = pointType;
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;
};


// ノード派生クラス：線
class NodeLine : public Node
{
private:
	// 線分を取得
	Coords<double, 2> Segment() const;
		
	// 無限直線情報
	struct InfiniteInfo{
		// 鉛直フラグ
		bool isVertical;
		// xの係数 (非鉛直の場合に使用する)
		double coefficientX;
		// y切片 (非鉛直の場合に使用する)
		double interceptY;
		// x切片 (鉛直の場合に使用する)
		double interceptX;

		// コンストラクタ
		InfiniteInfo() :
			isVertical(0),
			coefficientX(0),
			interceptY(0),
			interceptX(0) {}
	} m_infiniteInfo;

protected:
	// 形状を描画
	virtual void DrawContent() override;

public:
	// コンストラクタ
	NodeLine(Manager* pManager) :
		Node(pManager)
	{
		// JSON: ノードタイプ名
		m_typeName = JSON_VALUE_TYPE_LINE;

		// コンテンツデータを準備
		m_data.points.resize(2);
		m_data.use_lineLimitType = true;
	}

	// コンストラクタ
	NodeLine(Manager* pManager, const Coords<double, 2>& points, LineLimitType lineLimitType) :
		NodeLine(pManager)
	{
		// 内部データ登録
		m_data.lineLimitType = lineLimitType;

		// 有限の線分
		if (lineLimitType == LineLimitType::Finite) {
			// 座標値データをコピー
			m_data.points.assign(points.begin(), points.end());
		}
		// 無限直線
		else {
			// x方向が(ほぼ)0であれば鉛直線とする
			if (Util::IsZeroLength(points[END].x - points[START].x))
			{
				// 無限直線を x切片 で保持する
				m_infiniteInfo.isVertical = true;
				m_infiniteInfo.interceptX = points[0].x;
			}
			else {
				// 無限直線を x係数 と y切片 で保持する
				m_infiniteInfo.isVertical = false;
				m_infiniteInfo.coefficientX = (points[1].y - points[0].y) / (points[1].x - points[0].x);
				m_infiniteInfo.interceptY = m_infiniteInfo.coefficientX * -points[0].x + points[0].y;
			}
		}
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;

	// JSONデータ取得
	virtual picojson::object GetContents() const override;
	// JSONデータ設定
	virtual bool SetContents(picojson::object& jNode) override;
};


// ノード派生クラス：円弧
class NodeArc : public Node
{
protected:
	// 形状を描画
	virtual void DrawContent() override;

public:
	// コンストラクタ
	NodeArc(Manager* pManager) :
		Node(pManager)
	{
		// JSON: ノードタイプ名
		m_typeName = JSON_VALUE_TYPE_ARC;

		// コンテンツデータを準備
		m_data.points.resize(3);
		m_data.use_arcDirectionType = true;
	}

	// コンストラクタ
	NodeArc(Manager* pManager, const Coords<double, 3>& points, ArcDirectionType arcDirectionType) :
		NodeArc(pManager)
	{
		// 内部データ登録
		m_data.points.assign(points.begin(), points.end());
		m_data.arcDirectionType = arcDirectionType;
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;
	// 形状の整合性チェック
	virtual bool Verify() const override;
};


// ノード派生クラス：円
class NodeCircle : public Node
{
protected:
	// 形状を描画
	virtual void DrawContent() override;

public:
	// コンストラクタ
	NodeCircle(Manager* pManager) :
		Node(pManager)
	{
		// JSON: ノードタイプ名
		m_typeName = JSON_VALUE_TYPE_CIRCLE;

		// コンテンツデータを準備
		m_data.points.resize(1);
		m_data.use_radius = true;
		m_data.use_fillType = true;
	}

	// コンストラクタ
	NodeCircle(Manager* pManager, const Coord<double>& point, double radius, FillType fillType) :
		NodeCircle(pManager)
	{
		// 内部データ登録
		m_data.points[BASE] = point;
		m_data.radius = radius;
		m_data.fillType = fillType;
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;
};


// ノード派生クラス：多角形
class NodePolygon : public Node
{
protected:
	// 形状を描画
	virtual void DrawContent() override;

public:
	// コンストラクタ
	NodePolygon(Manager* pManager) :
		Node(pManager)
	{
		// JSON: ノードタイプ名
		m_typeName = JSON_VALUE_TYPE_POLYGON;

		// コンテンツデータを準備
		m_data.use_fillType = true;
	}

	// コンストラクタ
	NodePolygon(Manager* pManager, const Coords_v<double>& points, FillType fillType) :
		NodePolygon(pManager)
	{
		// 内部データ登録
		m_data.points.assign(points.begin(), points.end());
		m_data.fillType = fillType;
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;

	// JSONデータ設定
	virtual bool SetContents(picojson::object& jNode) override;
};


// ノード派生クラス：扇形
class NodeSector : public Node
{
private:
	// 内側の円弧座標を算出
	Coords<double, 3> CalcInnerArc() const;
	// 扇形のパスをDCに設定
	void CreateSectorPath() const;
	// 扇形のリージョン（コントロール座標）を算出
	void CreateSectorRgn(CRgn* sectorRgn) const;

protected:
	// 形状を描画
	virtual void DrawContent() override;

public:
	// コンストラクタ
	NodeSector(Manager* pManager) :
		Node(pManager)
	{
		// JSON: ノードタイプ名
		m_typeName = JSON_VALUE_TYPE_SECTOR;

		// コンテンツデータを準備
		m_data.points.resize(3);
		m_data.use_radius = true;
		m_data.use_arcDirectionType = true;
		m_data.use_fillType = true;
	}

	// コンストラクタ
	NodeSector(Manager* pManager, const Coords<double, 3>& points, double innerRadius, ArcDirectionType arcDirectionType, FillType fillType) :
		NodeSector(pManager)
	{
		// 内部データ登録
		m_data.points.assign(points.begin(), points.end());
		m_data.radius = innerRadius;
		m_data.arcDirectionType = arcDirectionType;
		m_data.fillType = fillType;
	}

	// 形状の最小包含箱を算出
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;
	// 形状の整合性チェック
	virtual bool Verify() const override;
};


// レイヤークラス
class Layer
{
private:
	// ノードコレクション
	std::vector<std::unique_ptr<Node>> m_nodes;

	// 描画フラグ
	bool m_enableDraw;

public:
	// コンストラクタ
	Layer();

	// 初期化
	void Clear();

	// ノード追加
	bool AddNode(Node* pNode) {
		if (!pNode->Verify()) return false;
		m_nodes.push_back(std::unique_ptr<Node>(pNode));
		return true;
	}

	// 全形状の最小包含箱を算出
	BoundingBox<double> CalcBoundingBox(bool forFit = false) const;

	// 描画フラグ
	void SetEnableDraw(bool val) { m_enableDraw = val; }
	bool GetEnableDraw() const { return m_enableDraw; }

	// 描画
	void Draw();

	// JSONデータ取得
	picojson::object GetContents() const;
	// JSONデータ設定
	bool SetContents(picojson::object& jLayer, Manager* pManager);
};


// 描画管理クラス
class Manager
{
private:
	// 描画キャンバス
	Canvas m_canvas;

	// 描画レイヤーコレクション
	std::vector<std::unique_ptr<Layer>> m_layers;

	// カレントレイヤー番号
	std::size_t m_currentLayerNo;

	// 全形状の最小包含箱を算出
	BoundingBox<double> CalcBoundingBox(bool forFit = false) const;

public:
	// 定数
	// デフォルトペン = ソリッド, 太さ1, 白色
	static constexpr LOGPEN DEFAULT_PEN = LOGPEN{ PS_SOLID, {1, 0}, RGB(0xFF, 0xFF, 0xFF) };
	// デフォルトブラシ = ソリッド, 白色, ハッチ無し
	static constexpr LOGBRUSH DEFAULT_BRUSH = LOGBRUSH{ BS_SOLID, RGB(0xFF, 0xFF, 0xFF), 0 };
	// 最大拡大率
	static constexpr double RATIO_MAX = 10000.0;
	// 最小縮小率
	static constexpr double RATIO_MIN = 0.001;
	// 最大移動量X,Y
	static constexpr double OFFSET_MAX = 99999.0;

	// 設定情報
	SettingInfo m_info;

	// カレントペン・ブラシ情報
	COLORREF m_penColor;
	long m_penWidth;
	long m_penStyle;
	COLORREF m_brushColor;

	// コンストラクタ
	Manager();

	// コピーコンストラクタ
	Manager(const Manager&) = delete;
	// 代入演算子
	Manager& operator=(const Manager&) = delete;

	// キャンバスオブジェクト
	const Canvas& GetCanvas() const { return m_canvas; }

	// カレントペン
	void SetCurrentPen(LOGPEN lp)
	{
		m_penColor = lp.lopnColor;
		m_penWidth = lp.lopnWidth.x;
		m_penStyle = lp.lopnStyle;
	}
	LOGPEN GetCurrentPen() { return LOGPEN{ (UINT)m_penStyle, POINT { m_penWidth, 0 }, m_penColor }; }
	// カレントブラシ
	void SetCurrentBrush(LOGBRUSH lb) { m_brushColor = lb.lbColor; }
	LOGBRUSH GetCurrentBrush() { return LOGBRUSH{ (UINT)0, m_brushColor, (ULONG_PTR)0 }; }

	// カレントレイヤー番号
	void SetCurrentLayerNo(int val) { m_currentLayerNo = val; }
	int GetCurrentLayerNo() const { return m_currentLayerNo; }

	// レイヤーを挿入
	bool InsertLayer(std::size_t noinsertNo);
	// レイヤー枚数を取得
	std::size_t GetLayerCount() const { return m_layers.size(); }
	// カレントレイヤーをクリア
	void ClearCurrentLayer() { if (m_layers.size() > m_currentLayerNo) m_layers[m_currentLayerNo]->Clear(); }
	// カレントレイヤーをクリア
	std::size_t DeleteCurrentLayer();
	// カレントレイヤーの描画可否
	void SetEnableCurrentLayer(bool val) { if (m_layers.size() > m_currentLayerNo) m_layers[m_currentLayerNo]->SetEnableDraw(val); }
	bool GetEnableCurrentLayer() const
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->GetEnableDraw();
	}

	// 描画内容をファイル保存(BMP/PNG/JPEG/GIF)
	bool SaveImage(const std::tstring& filePath) const { return m_canvas.SaveImage(filePath); }
	// 描画内容をクリップボードへコピー
	bool CopyImage(CWnd* pOwner) const { return m_canvas.CopyImage(pOwner); }

	// 描画内容をJSONファイルへエクスポート
	bool SaveContents(const std::tstring& filePath) const;
	// 描画内容をJSONファイルからインポート
	bool LoadContents(const std::tstring& filePath);

	// 座標系変換：内部キャンバス座標系→コントロール座標系
	Coord<long> CanvasToControl(Coord<double> canvasCoord) const
	{
		return m_canvas.CanvasToControl(canvasCoord);
	}
	// 座標系変換：コントロール座標系→内部キャンバス座標系
	Coord<double> ControlToCanvas(Coord<long> canvasCoord) const
	{
		return m_canvas.ControlToCanvas(canvasCoord);
	}

	// 描画領域の情報を再設定
	// OnSize()等のイベントで呼び出す必要あり
	void ResetCanvas(CDC* pDC, const CRect& rect, bool isResetRatioOffset = true) { m_canvas.Reset(pDC, rect, isResetRatioOffset); }

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
	bool AddGrid()
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodeGrid(this));
	}
	// 原点追加
	bool AddOrigin(const Coord<double>& point)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodeOrigin(this, point));
	}
	// 軸追加
	bool AddAxis(const Coord<double>& point)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodeAxis(this, point));
	}
	// 点追加
	bool AddPoint(const Coord<double>& point, PointType pointType)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodePoint(this, point, pointType));
	}
	// 線追加
	bool AddLine(const Coords<double, 2>& points, LineLimitType lineLimitType)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodeLine(this, points, lineLimitType));
	}
	// 円弧追加
	bool AddArc(const Coords<double, 3>& points, ArcDirectionType arcDirectionType)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodeArc(this, points, arcDirectionType));
	}
	// 円追加
	bool AddCircle(const Coord<double>& point, double radius, FillType fillType)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodeCircle(this, point, radius, fillType));
	}
	// 多角形追加
	bool AddPolygon(const Coords_v<double>& points, FillType fillType)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodePolygon(this, points, fillType));
	}
	// 扇形追加
	bool AddSector(const Coords<double, 3>& points, double innerRadius, ArcDirectionType arcDirectionType, FillType fillType)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodeSector(this, points, innerRadius, arcDirectionType, fillType));
	}
};

}	// namespace Drawer
