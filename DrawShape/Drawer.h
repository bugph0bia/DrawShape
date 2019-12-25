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

// std::string/wstring�̃T���N�Ή�
#ifdef UNICODE
#define tstring wstring
#else
#define tstring string
#endif

// std::numeric_limits<T>::max, min ���g�p���邽��
// ���̃w�b�_�t�@�C�����C���N���[�h������̓}�N���֐���max,min�͎g�p�ł��Ȃ��̂Œ���
#undef max
#undef min


namespace Drawer
{

//////////////// �N���X�O���錾 ////////////////

class Manager;
class Canvas;
class Layer;
class Node;


//////////////// �萔 ////////////////
// �����̋��e�덷
static constexpr double LENGTH_TOLERANCE = 0.00001;
// �p�x�̋��e�덷(rad)
static constexpr double ANGLE_TOLERANCE = 0.00001;
// �~����
static constexpr double	PI = 3.141592653589793;
// �����A�~�ʂ̍��W�R���N�V�����̃C���f�b�N�X�l
enum { START = 0, END = 1, CENTER = 2, BASE = 0 };

// JSON�L�[�F���C���[�z��
static const std::tstring JSON_KEY_LAYERS = _T("Layers");
// JSON�L�[�F�`��t���O
static const std::tstring JSON_KEY_ENABLE_DRAW = _T("EnableDraw");
// JSON�L�[�F�m�[�h�z��
static const std::tstring JSON_KEY_NODES = _T("Nodes");
// JSON�L�[�F�m�[�h�^�C�v
static const std::tstring JSON_KEY_TYPE = _T("Type");
// JSON�L�[�F�y���E�u���V
static const std::tstring JSON_KEY_PEN = _T("Pen");
static const std::tstring JSON_KEY_BRUSH = _T("Brush");
static const std::tstring JSON_KEY_COLOR = _T("Color");
static const std::tstring JSON_KEY_WIDTH = _T("Width");
static const std::tstring JSON_KEY_STYLE = _T("Style");
// JSON�L�[: �m�[�h���
static const std::tstring JSON_KEY_POINTS = _T("Points");
static const std::tstring JSON_KEY_POINT_TYPE = _T("PointType");
static const std::tstring JSON_KEY_RADIUS = _T("Radius");
static const std::tstring JSON_KEY_LINE_LIMIT_TYPE = _T("LineLimitType");
static const std::tstring JSON_KEY_ARC_DIRECTION_TYPE = _T("ArcDirectionType");
static const std::tstring JSON_KEY_FILL_TYPE = _T("FillType");
// JSON�L�[: �m�[�h���i���������j
static const std::tstring JSON_KEY_INFINITE_INFO = _T("InfiniteInfo");
static const std::tstring JSON_KEY_IS_VERTICAL = _T("IsVertical");
static const std::tstring JSON_KEY_COEFFICIENT_X = _T("CoefficientX");
static const std::tstring JSON_KEY_INTERCEPT_Y = _T("InterceptY");
static const std::tstring JSON_KEY_INTERCEPT_X = _T("InterceptX");
// JSON�l�F�m�[�h�^�C�v
static const std::tstring JSON_VALUE_TYPE_GRID = _T("Grid");
static const std::tstring JSON_VALUE_TYPE_ORIGIN = _T("Origin");
static const std::tstring JSON_VALUE_TYPE_AXIS = _T("Axis");
static const std::tstring JSON_VALUE_TYPE_POINT = _T("Point");
static const std::tstring JSON_VALUE_TYPE_LINE = _T("Line");
static const std::tstring JSON_VALUE_TYPE_ARC = _T("Arc");
static const std::tstring JSON_VALUE_TYPE_CIRCLE = _T("Circle");
static const std::tstring JSON_VALUE_TYPE_POLYGON = _T("Polygon");
static const std::tstring JSON_VALUE_TYPE_SECTOR = _T("Sector");


//////////////// �񋓌^ ////////////////

// �_�̎��
enum class PointType {
	Pixel,		// �E
	Large,		// ��
	Triangle,	// ��
};

// ���̌��E�l�̎��
enum class LineLimitType {
	Finite,		// �L��
	Infinite,	// ����
};

// �~�ʂ̕����̎��
enum class ArcDirectionType {
	Left,	// ��
	Right,	// �E
};

// �h��Ԃ��̎��
enum class FillType {
	NoFill,	// �h��Ԃ��Ȃ�
	Fill,	// �h��Ԃ�
};


//////////////// �N���X�A�\���́A�^ ////////////////

// ���W�f�[�^�\����
template<typename T>
class Coord
{
public:
	// x���W
	T x;
	// y���W
	T y;

	// �R���X�g���N�^
	Coord() : x(0), y(0) {}
	// �R���X�g���N�^
	Coord(T x, T y) : x(x), y(y) {}

	// 2�_�Ԃ̋���
	double Length(const Coord& rhs) const
	{
		return sqrt(pow((rhs.x - this->x), 2) + pow((rhs.y - this->y), 2));
	}

	// ���g�𒆐S�_�Ƃ��ē_����]
	Coord Rotate(const Coord& point, double angle)
	{
		// ���g����_�ւ̃x�N�g��
		Coord vec = point - *this;
		// �x�N�g������]
		Coord newPoint;
		newPoint.x = vec.x * cos(angle) - vec.y * sin(angle);
		newPoint.y = vec.x * sin(angle) + vec.y * cos(angle);
		// �x�N�g���Ɗ�_�����]��̍��W���Z�o
		newPoint += *this;
		return newPoint;
	}

	//// []���Z�q�i���Ӓl�Q�ƔŁj�Fx, y�����o��[0], [1]�ŎQ�Ɖ\�ɂ���
	//T& operator[](std::size_t idx) &
	//{
	//	if(idx == 0) return x;
	//	else if(idx == 1) return y;
	//	else throw std::out_of_range("Drawer::Coord::operator[] : index is out of range.");
	//}
	//// []���Z�q�iconst���Ӓl�Q�ƔŁj�Fx, y�����o��[0], [1]�ŎQ�Ɖ\�ɂ���
	//const T& operator[](std::size_t idx) const &
	//{
	//	if(idx == 0) return x;
	//	else if(idx == 1) return y;
	//	else throw std::out_of_range("Drawer::Coord::operator[] : index is out of range.");
	//}
	//// []���Z�q�iconst�E�Ӓl�Q�ƔŁj�Fx, y�����o��[0], [1]�ŎQ�Ɖ\�ɂ���
	//T operator[](std::size_t idx) const &&
	//{
	//	if(idx == 0) return x;
	//	else if(idx == 1) return y;
	//	else throw std::out_of_range("Drawer::Coord::operator[] : index is out of range.");
	//}

	// +=���Z�q
	Coord& operator+=(const Coord& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
	// -=���Z�q
	Coord& operator-=(const Coord& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}
	// *=���Z�q
	Coord& operator*=(double rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}
	// /=���Z�q
	Coord& operator/=(double rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		return *this;
	}
};

// +���Z�q
template<typename T>
Coord<T> operator+(const Coord<T>& lhs, const Coord<T>& rhs) { return Coord<T>(lhs) += rhs; }
// -���Z�q
template<typename T>
Coord<T> operator-(const Coord<T>& lhs, const Coord<T>& rhs) { return Coord<T>(lhs) -= rhs; }
// *���Z�q
template<typename T>
Coord<T> operator*(const Coord<T>& lhs, double rhs) { return Coord<T>(lhs) *= rhs; }
template<typename T>
Coord<T> operator*(double lhs, const Coord<T>& rhs) { return Coord<T>(rhs) *= lhs; }
// /���Z�q
template<typename T>
Coord<T> operator/(const Coord<T>& lhs, double rhs) { return Coord<T>(lhs) /= rhs; }

// ���W�f�[�^�R���N�V����
template<typename T, size_t N>
using Coords = std::array<Coord<T>, N>;
template<typename T>
using Coords_v = std::vector<Coord<T>>;


// �ŏ���ܔ��N���X
template<typename T>
class BoundingBox
{
public:
	// �ŏ����W
	Coord<T> min;
	// �ő���W
	Coord<T> max;

	// �R���X�g���N�^
	BoundingBox()
	{
		// �ŏ����W���^�̍ő�l�ŏ�����
		min.x = std::numeric_limits<T>::max();
		min.y = std::numeric_limits<T>::max();
		// �ő���W���^�̍ŏ��l�ŏ�����
		max.x = std::numeric_limits<T>::min();
		max.y = std::numeric_limits<T>::min();
	}
	// �R���X�g���N�^
	BoundingBox(Coord<T> min, Coord<T> max) : min(min), max(max) {}
	// �R���X�g���N�^
	BoundingBox(Coord<T> point) : min(point), max(point) {}

	// �ŏ���ܔ����m�̘a�W��
	void Union(const BoundingBox& rhs)
	{
		// ���肪�L���I�u�W�F�N�g
		if (rhs.Verify()) {
			// ���g���L���I�u�W�F�N�g
			if (this->Verify()) {
				// ���͈͂̑傫����`�ɂȂ�悤�ɍ�������
				this->min.x = std::min(this->min.x, rhs.min.x);
				this->min.y = std::min(this->min.y, rhs.min.y);
				this->max.x = std::max(this->max.x, rhs.max.x);
				this->max.y = std::max(this->max.y, rhs.max.y);
			}
			// ���g�������I�u�W�F�N�g
			else {
				// ��������̂܂܍̗p
				*this = rhs;
			}
		}
	}

	// �ŏ���ܔ����m�̐ϏW��
	void Intersect(const BoundingBox& rhs)
	{
		// ���g�Ƒ���̗������L���I�u�W�F�N�g
		if (rhs.Verify() && this->Verify()) {
			// ���͈͂̏�������`�ɂȂ�悤�ɍ�������
			this->min.x = std::max(this->min.x, rhs.min.x);
			this->min.y = std::max(this->min.y, rhs.min.y);
			this->max.x = std::min(this->max.x, rhs.max.x);
			this->max.y = std::min(this->max.y, rhs.max.y);
		}
		// ����ł��L���I�u�W�F�N�g�łȂ���Ζ���
		else {
			*this = BoundingBox();
		}
	}

	// +=���Z�q�F�ŏ���ܔ����m�̘a�W��
	BoundingBox& operator+=(const BoundingBox& rhs)
	{
		this->Union(rhs);
		return *this;
	}
	// +=���Z�q�F�ŏ���ܔ��ƍ��W�l�̘a�W��
	BoundingBox& operator+=(const Coord<T>& rhs)
	{
		this->Union(BoundingBox(rhs, rhs));
		return *this;
	}

	// �����擾
	double GetWidth() const { return (max.x - min.x); }
	// �������擾
	double GetHeight() const { return (max.y - min.y); }

	// +=���Z�q�F�ŏ���ܔ����m�̐ϏW��
	BoundingBox& operator*=(const BoundingBox& rhs)
	{
		this->Intersect(rhs);
		return *this;
	}

	// �L���m�F
	bool Verify() const
	{
		if (GetWidth() >= 0.0 && GetHeight() >= 0.0) return true;
		else return false;
	}
};

// +���Z�q�F�ŏ���ܔ����m�̘a�W��
template<typename T>
BoundingBox<T> operator+(const BoundingBox<T>& lhs, const BoundingBox<T>& rhs) { return BoundingBox<T>(lhs) += rhs; }
// +���Z�q�F�ŏ���ܔ��ƍ��W�l�̘a�W��
template<typename T>
BoundingBox<T> operator+(const BoundingBox<T>& lhs, const Coord<T>& rhs) { return BoundingBox<T>(lhs) += rhs; }
template<typename T>
BoundingBox<T> operator+(const Coord<T>& lhs, const BoundingBox<T>& rhs) { return BoundingBox<T>(rhs) += lhs; }
// *���Z�q�F�ŏ���ܔ����m�̐ϏW��
template<typename T>
BoundingBox<T> operator*(const BoundingBox<T>& lhs, const BoundingBox<T>& rhs) { return BoundingBox<T>(lhs) *= rhs; }


// ���[�e�B���e�B�N���X
class Util
{
public:
	// ����0�`�F�b�N
	static bool IsZeroLength(double value) { return std::abs(value) < LENGTH_TOLERANCE; }
	// �p�x0�`�F�b�N
	static bool IsZeroAngle(double value) { return std::abs(value) < ANGLE_TOLERANCE; }

	// 2�_�̓���`�F�b�N
	static bool IsSamePoint(const Coord<double>& p1, const Coord<double>& p2) { return IsZeroLength(p1.Length(p2)); }
	// 2�����̕��s�`�F�b�N
	static bool IsParallel(const Coords<double, 2>& l1, const Coords<double, 2>& l2)
	{
		// �P�ʃx�N�g���Z�o
		Coord<double> v1 = (l1[END] - l1[START]) / l1[START].Length(l1[END]);
		Coord<double> v2 = (l2[END] - l2[START]) / l2[START].Length(l2[END]);
		// ���ςƊO�ς��Z�o
		double inner = v1.x * v2.x + v1.y * v2.y;
		double outer = v1.x * v2.y - v2.x * v1.y;
		// �p�x��0�Ȃ畽�s
		return IsZeroAngle(std::atan2(outer, inner));
	}

	// �~�ʂ̐������`�F�b�N
	static bool VerifyArc(const Coords<double, 3>& arc)
	{
		// �n�_�ƏI�_����v
		if (Util::IsSamePoint(arc[START], arc[END])) return false;
		// �n�_�ƒ��S�_����v
		if (Util::IsSamePoint(arc[START], arc[CENTER])) return false;
		// �I�_�ƒ��S�_����v
		if (Util::IsSamePoint(arc[END], arc[CENTER])) return false;
		// ���a���s��v
		if (!Util::IsZeroLength(
			arc[START].Length(arc[CENTER]) - arc[END].Length(arc[CENTER])
		)) return false;

		return true;
	}
};


// �ݒ���\����
struct SettingInfo
{
	// �w�i�F
	COLORREF backColor;
	// �O���b�h�F
	COLORREF gridColor;
	// �O���b�h�T�C�Y
	double gridSize;
	// ���_�F
	COLORREF originColor;
	// ���_�T�C�Y
	long originSize;
	// ���F
	COLORREF axisColor;
	// ���X�P�[��
	double axisScale;
	// �O���b�h�`���
	BOOL isDrawGrid;
	// ���_�`���
	BOOL isDrawOrigin;
	// ���`���
	BOOL isDrawAxis;
	// ���`���
	BOOL isDrawArrow;
	// �~���S�_�`���
	BOOL isDrawCenter;
};

// �`��L�����o�X�N���X
class Canvas
{
private:
	// �`��Ώۂ̃f�o�C�X�R���e�L�X�g
	CDC* m_pDC;
	// �`��Ώۂ̃R���g���[���̋�`
	CRect m_rect;

	// �g��k����
	double m_ratio;
	// �I�t�Z�b�g
	// ���R���g���[�����W�����v�Z���x�̂��߂�double�^�Ƃ���
	Coord<double> m_offset;

public:
	// �萔
	// �g��k�����̏����l
	static constexpr double DEFAULT_RATIO = 1.0;
	// �I�t�Z�b�g�̏����l
	static inline const Coord<double> DEFAULT_OFFSET = Coord<double>();
	// �ŏ��O���b�h�`��T�C�Y(�R���g���[�����W)
	static constexpr long DRAW_GRID_SIZE_MIN = 10;
	// �ŏ����ڐ��`��T�C�Y(�R���g���[�����W)
	static constexpr long DRAW_AXIS_SCALE_MIN = 10;
	// �ڐ��̒���(���̕Б��̒���)
	static constexpr long AXIS_SCALE_LENGTH = 3;
	// ���_�̒�����`�̑傫��
	static constexpr long ORIGIN_CENTER_SIZE = 2;
	// �_�̎O�p�`�`�掞�̃T�C�Y
	static constexpr long TRIANGLE_POINT_SIZE = 3;
	// �_�̋������̃T�C�Y
	static constexpr long LARGE_POINT_SIZE = 3;
	// ���̉H�̎�����̊p�x
	static constexpr double ARROW_WING_ANGLE = 26.0 * PI / 180.0;
	// ���̉H�̒���(�R���g���[�����W)
	static constexpr long ARROW_WING_LENGTH = 10;

	// �R���X�g���N�^
	Canvas();

	// �g��k����
	void SetRatio(double val) { m_ratio = val;  }
	double GetRatio() const { return m_ratio; }
	// �I�t�Z�b�g
	void SetOffset(Coord<double> val) { m_offset = val; }
	Coord<double> GetOffset() const { return m_offset; }

	// �`��̈�̍Đݒ�
	void Reset(CDC* pDC, const CRect& rect, bool isResetRatioOffset)
	{
		m_pDC = pDC;
		m_rect = rect;
		if (isResetRatioOffset) {
			m_ratio = DEFAULT_RATIO;
			m_offset = Coord<double>(rect.CenterPoint().x, rect.CenterPoint().y);
		}
	}
	// �f�o�C�X�R���e�L�X�g���擾
	CDC* GetDC() const { return m_pDC; };
	// �`��̈���擾
	const CRect* GetRect() const { return &m_rect; };

	// �����ϊ��F�����L�����o�X���W�n���R���g���[�����W�n
	//   ���R���g���[�����W��long�^�����v�Z�r���Ő��x�𗎂Ƃ��Ȃ����߂�double�^�Ŗ߂�
	double CanvasToControl(double canvasVal) const { return canvasVal * m_ratio; }
	// �����ϊ��F�R���g���[�����W�n�������L�����o�X���W�n
	//   ���R���g���[�����W��long�^�����v�Z�r���Ő��x�𗎂Ƃ��Ȃ����߂�double�^�œn��
	double ControlToCanvas(double ctrlVal) const { return ctrlVal / m_ratio; }
	// ���W�n�ϊ��F�����L�����o�X���W�n���R���g���[�����W�n
	Coord<long> CanvasToControl(const Coord<double>& canvasCoord) const;
	// ���W�n�ϊ��F�R���g���[�����W�n�������L�����o�X���W�n
	Coord<double> ControlToCanvas(const Coord<long>& ctrlCoord) const;
	// �R���N�V�����p�̍��W�n�ϊ��F�����L�����o�X���W�n���R���g���[�����W�n
	Coords_v<long> CanvasToControl(const Coords_v<double>& canvasCoords) const;
	// �R���N�V�����p�̍��W�n�ϊ��F�R���g���[�����W�n�������L�����o�X���W�n
	Coords_v<double> ControlToCanvas(const Coords_v<long>& ctrlCoords) const;

	// �`��̈�S�̂̍��W���擾
	BoundingBox<double> GetCanvasArea() const;

	// �w�i��h��Ԃ�
	void FillBackground() const;
	// �O���b�h�`��
	void DrawGrid(double size) const;
	// ���_�`��
	void DrawOrigin(const Coord<double>& base, long size) const;
	// ���`��
	void DrawAxis(const Coord<double>& base, double scale) const;
	// �s�N�Z��(�_)��`��
	void DrawPixel(const Coord<double>& point) const;
	// �_���O�p�`�ŕ`��
	void DrawTrianglePoint(const Coord<double>& point) const;
	// �_���������ĕ`��
	void DrawLargePoint(const Coord<double>& point) const;
	// ����[�`��
	void DrawArrowHead(const Coords<double, 2>& baseSegment, FillType fillType=FillType::NoFill) const;
	// �x�W�G�Ȑ��ɂ��~�ʕ\�����Z�o
	Coords_v<double> CalcBezierArc(Coords<double, 3> arc, ArcDirectionType direction) const;
	// �x�W�G�Ȑ��ɂ��~�ʕ`��
	void DrawBezierArc(Coords<double, 3> arc, ArcDirectionType direction) const;

	// �`����e���t�@�C���ۑ�(BMP/PNG/JPEG/GIF)
	bool SaveImage(const std::tstring& filePath) const;
	// �`����e���N���b�v�{�[�h�փR�s�[
	bool CopyImage(CWnd* pOwner) const;
};


// �y��/�u���V�ꎞ�ύX�N���X
class PenBrushChanger
{
private:
	// �Ώۂ̃f�o�C�X�R���e�L�X�g
	CDC* m_pDC;

	// �y��
	CPen m_pen;
	// �ύX�O�y��
	CPen* m_pOldPen;

	// �u���V
	CBrush m_brush;
	// �ύX�O�u���V
	CBrush* m_pOldBrush;

public:
	// �R���X�g���N�^�F�y���ύX
	PenBrushChanger(CDC* pDC, LOGPEN logPen) :
		m_pDC(pDC),
		m_pOldPen(nullptr),
		m_pOldBrush(nullptr)
	{
		if (m_pDC) {
			// �y���ύX
			m_pen.CreatePenIndirect(&logPen);
			m_pOldPen = m_pDC->SelectObject(&m_pen);
		}
	}

	// �R���X�g���N�^�F�u���V�ύX
	PenBrushChanger(CDC* pDC, LOGBRUSH logBrush) :
		m_pDC(pDC),
		m_pOldPen(nullptr),
		m_pOldBrush(nullptr)
	{
		if (m_pDC) {
			// �y���ύX
			m_brush.CreateBrushIndirect(&logBrush);
			m_pOldBrush = m_pDC->SelectObject(&m_brush);
		}
	}

	// �R�s�[�R���X�g���N�^
	PenBrushChanger(const PenBrushChanger&) = delete;
	// �R�s�[������Z�q
	PenBrushChanger& operator=(const PenBrushChanger&) = delete;

	// �f�X�g���N�^
	~PenBrushChanger()
	{
		if (m_pDC) {
			// �y�������ɖ߂�
			if (m_pOldPen) m_pDC->SelectObject(m_pOldPen);
			// �u���V�����ɖ߂�
			if (m_pOldBrush) m_pDC->SelectObject(m_pOldBrush);
		}
	}
};

// �m�[�h�N���X
class Node
{
protected:
	// �ݒ���
	const SettingInfo& m_info;
	// �L�����o�X�I�u�W�F�N�g
	const Canvas& m_canvas;
	// �y��
	LOGPEN m_pen;
	// �u���V
	LOGBRUSH m_brush;
	// JSON: �m�[�h�^�C�v����
	std::string m_typeName;

	// �R���e���c�f�[�^
	struct ContentsData {
		// ���W�f�[�^
		Coords_v<double> points;
		// ���a
		double radius;
		bool use_radius;
		// �_�̎��
		PointType pointType;
		bool use_pointType;
		// ���E�l�̎��
		LineLimitType lineLimitType;
		bool use_lineLimitType;
		// �~�ʂ̕����̎��
		ArcDirectionType arcDirectionType;
		bool use_arcDirectionType;
		// �h��Ԃ��̎��
		FillType fillType;
		bool use_fillType;

		// �R���X�g���N�^
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

	// �`�󂪕`��̈�Ɋ܂܂�邩�`�F�b�N
	//   ���`�󂲂ƂɕK�v�ɉ����ăI�[�o�[���C�h����
	virtual bool IsIncludeCanvas() const
	{
		// �`��G���A
		BoundingBox<double> bboxCanvas = m_canvas.GetCanvasArea();
		// �`��̍ŏ���ܔ�
		BoundingBox<double> bboxShape = CalcBoundingBox();
		// �K��̔�����@�́A2�̗̈悪���ʗ̈�������ǂ���
		return (bboxCanvas * bboxShape).Verify();
	}

	// �`���`��
	//   ���`�󂲂ƂɃI�[�o�[���C�h����
	virtual void DrawContent() {}

public:
	// �R���X�g���N�^
	Node(Manager* pManager);
	// �R���X�g���N�^
	Node(const SettingInfo& info, const Canvas& canvas, const LOGPEN& pen, const LOGBRUSH& brush) :
		m_info(info),
		m_canvas(canvas),
		m_pen(pen),
		m_brush(brush) {}

	// �f�X�g���N�^
	virtual ~Node() {}

	// �`��̍ŏ���ܔ����Z�o
	//   ���`�󂲂ƂɃI�[�o�[���C�h����
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const { return BoundingBox<double>(); }

	// �`��̐������`�F�b�N
	//   ���`�󂲂ƂɃI�[�o�[���C�h����
	virtual bool Verify() const { return true; }

	// �`��
	void Draw() {
		// �`��̈�Ɋ܂܂�Ȃ���Ε`�悵�Ȃ�
		if (!IsIncludeCanvas()) return;

		// �y���ƃu���V��ύX
		PenBrushChanger pc(m_canvas.GetDC(), m_pen);
		PenBrushChanger bc(m_canvas.GetDC(), m_brush);
		// �`���`��
		DrawContent();
	}

	// JSON�f�[�^�擾
	//   ���K�v�ɉ����Č`�󂲂ƂɃI�[�o�[���C�h����
	virtual picojson::object GetContents() const;
	// JSON�f�[�^�ݒ�
	//   ���K�v�ɉ����Č`�󂲂ƂɃI�[�o�[���C�h����
	virtual bool SetContents(picojson::object& jNode);

	// �^�C�v���ɉ������m�[�h���쐬
	static Node* FromTypeName(const std::string& typeName, Manager* pManager);
};


// �m�[�h�h���N���X�F�O���b�h
class NodeGrid : public Node
{
protected:
	// �`�󂪕`��̈�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeCanvas() const override;
	// �`���`��
	virtual void DrawContent() override;

public:
	// �R���X�g���N�^
	NodeGrid(Manager* pManager) :
		Node(pManager)
	{
		// JSON: �m�[�h�^�C�v��
		m_typeName = JSON_VALUE_TYPE_GRID;
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;
};


// �m�[�h�h���N���X�F���_
class NodeOrigin : public Node
{
protected:
	// �`�󂪕`��̈�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeCanvas() const override;
	// �`���`��
	virtual void DrawContent() override;

public:
	// �R���X�g���N�^
	NodeOrigin(Manager* pManager) :
		Node(pManager)
	{
		// JSON: �m�[�h�^�C�v��
		m_typeName = JSON_VALUE_TYPE_ORIGIN;
		// ���_�̓u���V�̐F���y���̐F�ɍ��킹��
		m_brush.lbColor = m_pen.lopnColor;

		// �R���e���c�f�[�^������
		m_data.points.resize(1);
	}

	// �R���X�g���N�^
	NodeOrigin(Manager* pManager, const Coord<double>& point) :
		NodeOrigin(pManager)
	{
		// �����f�[�^�o�^
		m_data.points[BASE] = point;
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;
};


// �m�[�h�h���N���X�F��
class NodeAxis : public Node
{
protected:
	// �`�󂪕`��̈�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeCanvas() const override;
	// �`���`��
	virtual void DrawContent() override;

public:
	// �R���X�g���N�^
	NodeAxis(Manager* pManager) :
		Node(pManager)
	{
		// JSON: �m�[�h�^�C�v��
		m_typeName = JSON_VALUE_TYPE_AXIS;

		// �R���e���c�f�[�^������
		m_data.points.resize(1);
	}

	// �R���X�g���N�^
	NodeAxis(Manager* pManager, const Coord<double>& point) :
		NodeAxis(pManager)
	{
		// �����f�[�^�o�^
		m_data.points[BASE] = point;
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;
};


// �m�[�h�h���N���X�F�_
class NodePoint : public Node
{
protected:
	// �`���`��
	virtual void DrawContent() override;

public:
	// �R���X�g���N�^
	NodePoint(Manager* pManager) :
		Node(pManager)
	{
		// JSON: �m�[�h�^�C�v��
		m_typeName = JSON_VALUE_TYPE_POINT;

		// �R���e���c�f�[�^������
		m_data.points.resize(1);
		m_data.use_pointType = true;
	}

	// �R���X�g���N�^
	NodePoint(Manager* pManager, const Coord<double>& point, PointType pointType) :
		NodePoint(pManager)
	{
		// �����f�[�^�o�^
		m_data.points[BASE] = point;
		m_data.pointType = pointType;
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;
};


// �m�[�h�h���N���X�F��
class NodeLine : public Node
{
private:
	// �������擾
	Coords<double, 2> Segment() const;
		
	// �����������
	struct InfiniteInfo{
		// �����t���O
		bool isVertical;
		// x�̌W�� (�񉔒��̏ꍇ�Ɏg�p����)
		double coefficientX;
		// y�ؕ� (�񉔒��̏ꍇ�Ɏg�p����)
		double interceptY;
		// x�ؕ� (�����̏ꍇ�Ɏg�p����)
		double interceptX;

		// �R���X�g���N�^
		InfiniteInfo() :
			isVertical(0),
			coefficientX(0),
			interceptY(0),
			interceptX(0) {}
	} m_infiniteInfo;

protected:
	// �`���`��
	virtual void DrawContent() override;

public:
	// �R���X�g���N�^
	NodeLine(Manager* pManager) :
		Node(pManager)
	{
		// JSON: �m�[�h�^�C�v��
		m_typeName = JSON_VALUE_TYPE_LINE;

		// �R���e���c�f�[�^������
		m_data.points.resize(2);
		m_data.use_lineLimitType = true;
	}

	// �R���X�g���N�^
	NodeLine(Manager* pManager, const Coords<double, 2>& points, LineLimitType lineLimitType) :
		NodeLine(pManager)
	{
		// �����f�[�^�o�^
		m_data.lineLimitType = lineLimitType;

		// �L���̐���
		if (lineLimitType == LineLimitType::Finite) {
			// ���W�l�f�[�^���R�s�[
			m_data.points.assign(points.begin(), points.end());
		}
		// ��������
		else {
			// x������(�ق�)0�ł���Ή������Ƃ���
			if (Util::IsZeroLength(points[END].x - points[START].x))
			{
				// ���������� x�ؕ� �ŕێ�����
				m_infiniteInfo.isVertical = true;
				m_infiniteInfo.interceptX = points[0].x;
			}
			else {
				// ���������� x�W�� �� y�ؕ� �ŕێ�����
				m_infiniteInfo.isVertical = false;
				m_infiniteInfo.coefficientX = (points[1].y - points[0].y) / (points[1].x - points[0].x);
				m_infiniteInfo.interceptY = m_infiniteInfo.coefficientX * -points[0].x + points[0].y;
			}
		}
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;

	// JSON�f�[�^�擾
	virtual picojson::object GetContents() const override;
	// JSON�f�[�^�ݒ�
	virtual bool SetContents(picojson::object& jNode) override;
};


// �m�[�h�h���N���X�F�~��
class NodeArc : public Node
{
protected:
	// �`���`��
	virtual void DrawContent() override;

public:
	// �R���X�g���N�^
	NodeArc(Manager* pManager) :
		Node(pManager)
	{
		// JSON: �m�[�h�^�C�v��
		m_typeName = JSON_VALUE_TYPE_ARC;

		// �R���e���c�f�[�^������
		m_data.points.resize(3);
		m_data.use_arcDirectionType = true;
	}

	// �R���X�g���N�^
	NodeArc(Manager* pManager, const Coords<double, 3>& points, ArcDirectionType arcDirectionType) :
		NodeArc(pManager)
	{
		// �����f�[�^�o�^
		m_data.points.assign(points.begin(), points.end());
		m_data.arcDirectionType = arcDirectionType;
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;
	// �`��̐������`�F�b�N
	virtual bool Verify() const override;
};


// �m�[�h�h���N���X�F�~
class NodeCircle : public Node
{
protected:
	// �`���`��
	virtual void DrawContent() override;

public:
	// �R���X�g���N�^
	NodeCircle(Manager* pManager) :
		Node(pManager)
	{
		// JSON: �m�[�h�^�C�v��
		m_typeName = JSON_VALUE_TYPE_CIRCLE;

		// �R���e���c�f�[�^������
		m_data.points.resize(1);
		m_data.use_radius = true;
		m_data.use_fillType = true;
	}

	// �R���X�g���N�^
	NodeCircle(Manager* pManager, const Coord<double>& point, double radius, FillType fillType) :
		NodeCircle(pManager)
	{
		// �����f�[�^�o�^
		m_data.points[BASE] = point;
		m_data.radius = radius;
		m_data.fillType = fillType;
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;
};


// �m�[�h�h���N���X�F���p�`
class NodePolygon : public Node
{
protected:
	// �`���`��
	virtual void DrawContent() override;

public:
	// �R���X�g���N�^
	NodePolygon(Manager* pManager) :
		Node(pManager)
	{
		// JSON: �m�[�h�^�C�v��
		m_typeName = JSON_VALUE_TYPE_POLYGON;

		// �R���e���c�f�[�^������
		m_data.use_fillType = true;
	}

	// �R���X�g���N�^
	NodePolygon(Manager* pManager, const Coords_v<double>& points, FillType fillType) :
		NodePolygon(pManager)
	{
		// �����f�[�^�o�^
		m_data.points.assign(points.begin(), points.end());
		m_data.fillType = fillType;
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;

	// JSON�f�[�^�ݒ�
	virtual bool SetContents(picojson::object& jNode) override;
};


// �m�[�h�h���N���X�F��`
class NodeSector : public Node
{
private:
	// �����̉~�ʍ��W���Z�o
	Coords<double, 3> CalcInnerArc() const;
	// ��`�̃p�X��DC�ɐݒ�
	void CreateSectorPath() const;
	// ��`�̃��[�W�����i�R���g���[�����W�j���Z�o
	void CreateSectorRgn(CRgn* sectorRgn) const;

protected:
	// �`���`��
	virtual void DrawContent() override;

public:
	// �R���X�g���N�^
	NodeSector(Manager* pManager) :
		Node(pManager)
	{
		// JSON: �m�[�h�^�C�v��
		m_typeName = JSON_VALUE_TYPE_SECTOR;

		// �R���e���c�f�[�^������
		m_data.points.resize(3);
		m_data.use_radius = true;
		m_data.use_arcDirectionType = true;
		m_data.use_fillType = true;
	}

	// �R���X�g���N�^
	NodeSector(Manager* pManager, const Coords<double, 3>& points, double innerRadius, ArcDirectionType arcDirectionType, FillType fillType) :
		NodeSector(pManager)
	{
		// �����f�[�^�o�^
		m_data.points.assign(points.begin(), points.end());
		m_data.radius = innerRadius;
		m_data.arcDirectionType = arcDirectionType;
		m_data.fillType = fillType;
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox(bool forFit = false) const override;
	// �`��̐������`�F�b�N
	virtual bool Verify() const override;
};


// ���C���[�N���X
class Layer
{
private:
	// �m�[�h�R���N�V����
	std::vector<std::unique_ptr<Node>> m_nodes;

	// �`��t���O
	bool m_enableDraw;

public:
	// �R���X�g���N�^
	Layer();

	// ������
	void Clear();

	// �m�[�h�ǉ�
	bool AddNode(Node* pNode) {
		if (!pNode->Verify()) return false;
		m_nodes.push_back(std::unique_ptr<Node>(pNode));
		return true;
	}

	// �S�`��̍ŏ���ܔ����Z�o
	BoundingBox<double> CalcBoundingBox(bool forFit = false) const;

	// �`��t���O
	void SetEnableDraw(bool val) { m_enableDraw = val; }
	bool GetEnableDraw() const { return m_enableDraw; }

	// �`��
	void Draw();

	// JSON�f�[�^�擾
	picojson::object GetContents() const;
	// JSON�f�[�^�ݒ�
	bool SetContents(picojson::object& jLayer, Manager* pManager);
};


// �`��Ǘ��N���X
class Manager
{
private:
	// �`��L�����o�X
	Canvas m_canvas;

	// �`�惌�C���[�R���N�V����
	std::vector<std::unique_ptr<Layer>> m_layers;

	// �J�����g���C���[�ԍ�
	std::size_t m_currentLayerNo;

	// �S�`��̍ŏ���ܔ����Z�o
	BoundingBox<double> CalcBoundingBox(bool forFit = false) const;

public:
	// �萔
	// �f�t�H���g�y�� = �\���b�h, ����1, ���F
	static constexpr LOGPEN DEFAULT_PEN = LOGPEN{ PS_SOLID, {1, 0}, RGB(0xFF, 0xFF, 0xFF) };
	// �f�t�H���g�u���V = �\���b�h, ���F, �n�b�`����
	static constexpr LOGBRUSH DEFAULT_BRUSH = LOGBRUSH{ BS_SOLID, RGB(0xFF, 0xFF, 0xFF), 0 };
	// �ő�g�嗦
	static constexpr double RATIO_MAX = 10000.0;
	// �ŏ��k����
	static constexpr double RATIO_MIN = 0.001;
	// �ő�ړ���X,Y
	static constexpr double OFFSET_MAX = 99999.0;

	// �ݒ���
	SettingInfo m_info;

	// �J�����g�y���E�u���V���
	COLORREF m_penColor;
	long m_penWidth;
	long m_penStyle;
	COLORREF m_brushColor;

	// �R���X�g���N�^
	Manager();

	// �R�s�[�R���X�g���N�^
	Manager(const Manager&) = delete;
	// ������Z�q
	Manager& operator=(const Manager&) = delete;

	// �L�����o�X�I�u�W�F�N�g
	const Canvas& GetCanvas() const { return m_canvas; }

	// �J�����g�y��
	void SetCurrentPen(LOGPEN lp)
	{
		m_penColor = lp.lopnColor;
		m_penWidth = lp.lopnWidth.x;
		m_penStyle = lp.lopnStyle;
	}
	LOGPEN GetCurrentPen() { return LOGPEN{ (UINT)m_penStyle, POINT { m_penWidth, 0 }, m_penColor }; }
	// �J�����g�u���V
	void SetCurrentBrush(LOGBRUSH lb) { m_brushColor = lb.lbColor; }
	LOGBRUSH GetCurrentBrush() { return LOGBRUSH{ (UINT)0, m_brushColor, (ULONG_PTR)0 }; }

	// �J�����g���C���[�ԍ�
	void SetCurrentLayerNo(int val) { m_currentLayerNo = val; }
	int GetCurrentLayerNo() const { return m_currentLayerNo; }

	// ���C���[��}��
	bool InsertLayer(std::size_t noinsertNo);
	// ���C���[�������擾
	std::size_t GetLayerCount() const { return m_layers.size(); }
	// �J�����g���C���[���N���A
	void ClearCurrentLayer() { if (m_layers.size() > m_currentLayerNo) m_layers[m_currentLayerNo]->Clear(); }
	// �J�����g���C���[���N���A
	std::size_t DeleteCurrentLayer();
	// �J�����g���C���[�̕`���
	void SetEnableCurrentLayer(bool val) { if (m_layers.size() > m_currentLayerNo) m_layers[m_currentLayerNo]->SetEnableDraw(val); }
	bool GetEnableCurrentLayer() const
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->GetEnableDraw();
	}

	// �`����e���t�@�C���ۑ�(BMP/PNG/JPEG/GIF)
	bool SaveImage(const std::tstring& filePath) const { return m_canvas.SaveImage(filePath); }
	// �`����e���N���b�v�{�[�h�փR�s�[
	bool CopyImage(CWnd* pOwner) const { return m_canvas.CopyImage(pOwner); }

	// �`����e��JSON�t�@�C���փG�N�X�|�[�g
	bool SaveContents(const std::tstring& filePath) const;
	// �`����e��JSON�t�@�C������C���|�[�g
	bool LoadContents(const std::tstring& filePath);

	// ���W�n�ϊ��F�����L�����o�X���W�n���R���g���[�����W�n
	Coord<long> CanvasToControl(Coord<double> canvasCoord) const
	{
		return m_canvas.CanvasToControl(canvasCoord);
	}
	// ���W�n�ϊ��F�R���g���[�����W�n�������L�����o�X���W�n
	Coord<double> ControlToCanvas(Coord<long> canvasCoord) const
	{
		return m_canvas.ControlToCanvas(canvasCoord);
	}

	// �`��̈�̏����Đݒ�
	// OnSize()���̃C�x���g�ŌĂяo���K�v����
	void ResetCanvas(CDC* pDC, const CRect& rect, bool isResetRatioOffset = true) { m_canvas.Reset(pDC, rect, isResetRatioOffset); }

	// ������
	void Clear();

	// �`��
	void Draw(bool isDesignMode=false);

	// �g��k��
	bool Zoom(double coef, const Coord<long>& base);
	// �p��
	bool Pan(const Coord<long>& move);
	// �t�B�b�g
	void Fit(double shapeOccupancy);

	// �O���b�h�ǉ�
	bool AddGrid()
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodeGrid(this));
	}
	// ���_�ǉ�
	bool AddOrigin(const Coord<double>& point)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodeOrigin(this, point));
	}
	// ���ǉ�
	bool AddAxis(const Coord<double>& point)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodeAxis(this, point));
	}
	// �_�ǉ�
	bool AddPoint(const Coord<double>& point, PointType pointType)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodePoint(this, point, pointType));
	}
	// ���ǉ�
	bool AddLine(const Coords<double, 2>& points, LineLimitType lineLimitType)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodeLine(this, points, lineLimitType));
	}
	// �~�ʒǉ�
	bool AddArc(const Coords<double, 3>& points, ArcDirectionType arcDirectionType)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodeArc(this, points, arcDirectionType));
	}
	// �~�ǉ�
	bool AddCircle(const Coord<double>& point, double radius, FillType fillType)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodeCircle(this, point, radius, fillType));
	}
	// ���p�`�ǉ�
	bool AddPolygon(const Coords_v<double>& points, FillType fillType)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodePolygon(this, points, fillType));
	}
	// ��`�ǉ�
	bool AddSector(const Coords<double, 3>& points, double innerRadius, ArcDirectionType arcDirectionType, FillType fillType)
	{
		if (m_layers.size() <= m_currentLayerNo) return false;
		return m_layers[m_currentLayerNo]->AddNode(new NodeSector(this, points, innerRadius, arcDirectionType, fillType));
	}
};

}	// namespace Drawer
