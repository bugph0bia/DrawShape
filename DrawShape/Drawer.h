#pragma once

#include <vector>
#include <array>
#include <string>
#include <memory>
#include <limits>
#include <stdexcept>
#include <cmath>

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

	// TODO: []���Z�q�g���H�g��Ȃ���΃N���X�ł͂Ȃ��\���̂ł��悢

	// []���Z�q�i���Ӓl�Q�ƔŁj�Fx, y�����o��[0], [1]�ŎQ�Ɖ\�ɂ���
	T& operator[](std::size_t idx) &
	{
		if(idx == 0) return x;
		else if(idx == 1) return y;
		else throw std::out_of_range("Drawer::Coord::operator[] : index is out of range.");
	}
	// []���Z�q�iconst���Ӓl�Q�ƔŁj�Fx, y�����o��[0], [1]�ŎQ�Ɖ\�ɂ���
	const T& operator[](std::size_t idx) const &
	{
		if(idx == 0) return x;
		else if(idx == 1) return y;
		else throw std::out_of_range("Drawer::Coord::operator[] : index is out of range.");
	}
	// []���Z�q�iconst�E�Ӓl�Q�ƔŁj�Fx, y�����o��[0], [1]�ŎQ�Ɖ\�ɂ���
	T operator[](std::size_t idx) const &&
	{
		if(idx == 0) return x;
		else if(idx == 1) return y;
		else throw std::out_of_range("Drawer::Coord::operator[] : index is out of range.");
	}
};


// ���W�f�[�^�R���N�V����
template<typename T, size_t N>
using Coords = std::array<Coord<T>, N>;


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

	// +=���Z�q�F�ŏ���ܔ����m������
	BoundingBox& operator+=(const BoundingBox& rhs)
	{
		// ���͈͂̑傫����`�ɂȂ�悤�ɍ�������
		this->min.x = std::min(this->min.x, rhs.min.x);
		this->min.y = std::min(this->min.y, rhs.min.y);
		this->max.x = std::max(this->max.x, rhs.max.x);
		this->max.y = std::max(this->max.y, rhs.max.y);
		return *this;
	}
	// +=���Z�q�F�ŏ���ܔ��ƍ��W�l������
	BoundingBox& operator+=(const Coord<T>& rhs)
	{
		// ���͈͂̑傫����`�ɂȂ�悤�ɍ�������
		this->min.x = std::min(this->min.x, rhs.x);
		this->min.y = std::min(this->min.y, rhs.y);
		this->max.x = std::max(this->max.x, rhs.x);
		this->max.y = std::max(this->max.y, rhs.y);
		return *this;
	}

	// �����擾
	double GetWidth() const { return (max.x - min.x); }
	// �������擾
	double GetHeight() const { return (max.y - min.y); }

	// �L���m�F
	bool Verify() const
	{
		if (GetWidth() >= 0.0 && GetHeight() > 0.0) return true;
		else return false;
	}
};

// +���Z�q�F�ŏ���ܔ����m������
template<typename T>
BoundingBox<T> operator+(const BoundingBox<T>& lhs, const BoundingBox<T>& rhs) { return BoundingBox<T>(lhs) += rhs; }
// +���Z�q�F�ŏ���ܔ��ƍ��W�l������
template<typename T>
BoundingBox<T> operator+(const BoundingBox<T>& lhs, const Coord<T>& rhs) { return BoundingBox<T>(lhs) += rhs; }
template<typename T>
BoundingBox<T> operator+(const Coord<T>& lhs, const BoundingBox<T>& rhs) { return BoundingBox<T>(rhs) += lhs; }


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
	static constexpr Coord<double> DEFAULT_OFFSET = { 0.0, 0.0 };
	// �~����
	static constexpr double	PI = 3.141592653589793;
	// �����̋��e�덷
	static constexpr double LENGTH_TOLERANCE = 0.00001;
	// �p�x�̋��e�덷(rad)
	static constexpr double ANGLE_TOLERANCE = 0.00001;
	// �ŏ��O���b�h�`��T�C�Y(�R���g���[�����W)
	static constexpr long DRAW_GRID_SIZE_MIN = 5;
	// �ŏ����ڐ��`��T�C�Y(�R���g���[�����W)
	static constexpr long DRAW_AXIS_SCALE_MIN = 10;
	// �ڐ��̒���(���̕Б��̒���)
	static constexpr long AXIS_SCALE_LENGTH = 3;
	// ���_�̒�����`�̑傫��
	static constexpr long ORIGIN_CENTER_SIZE = 2;
	// �_�̋������̃T�C�Y
	static constexpr long LARGE_POINT_SIZE = 3;
	// ���̉H�̎�����̊p�x(20��)
	static constexpr double ARROW_WING_ANGLE = 20.0 * PI / 180.0;

	// �R���X�g���N�^
	Canvas(CDC* pDC, const CRect& rect);

	// �g��k����
	void SetRatio(double val) { m_ratio = val;  }
	double GetRatio() const { return m_ratio; }
	// �I�t�Z�b�g
	void SetOffset(Coord<double> val) { m_offset = val; }
	Coord<double> GetOffset() const { return m_offset; }

	// �`��̈�̍Đݒ�
	void Reset(CDC* pDC, const CRect& rect) { m_pDC = pDC; m_rect = rect; }
	// �f�o�C�X�R���e�L�X�g���擾
	CDC* GetDC() const { return m_pDC; };
	// �`��̈���擾
	const CRect* GetRect() const { return &m_rect; };

	// ���W�n�ϊ��F�����L�����o�X���W�n���R���g���[�����W�n
	Coord<long> CanvasToControl(const Coord<double> &canvasCoord) const;
	// ���W�n�ϊ��F�R���g���[�����W�n�������L�����o�X���W�n
	Coord<double> ControlToCanvas(const Coord<long>& ctrlCoord) const;

	// �`��̈�S�̂̍��W���擾
	BoundingBox<double> GetCanvasBox() const;

	// �w�i��h��Ԃ�
	void FillBackground(COLORREF color);
	// �O���b�h�`��
	void DrawGrid(double size);
	// ���_�`��
	void DrawOrigin(Coord<double> base, long size);
	// ���`��
	void DrawAxis(Coord<double> base, double scale);
	// �_�������`��
	void DrawLargePoint(const Coord<double>& point);
	// ����[�`��
	void DrawArrowHead(const Coord<double>& start, const Coord<double>& end);
	// �x�W�G�Ȑ��ɂ��~�ʕ`��
	void DrawBezierArc(Coord<double> start, Coord<double> end, Coord<double> center, ArcDirectionType direction);

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
private:
	// �L�����o�X�I�u�W�F�N�g
	const Canvas& m_canvas;
	// �y��
	LOGPEN m_pen;
	// �u���V
	LOGBRUSH m_brush;

public:
	// �R���X�g���N�^
	Node(const Canvas& canvas, const LOGPEN& pen, const LOGBRUSH& brush) :
		m_canvas(canvas),
		m_pen(pen),
		m_brush(brush)
	{
	}

	// �f�X�g���N�^
	virtual ~Node() {}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox() const { return BoundingBox<double>(); }
	// �`�󂪕`��G���A�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeDrawArea() const { return false; }
	// �`��
	virtual void Draw() {}
};


// ���C���[�N���X
class Layer
{
private:
	// �m�[�h�R���N�V����
	std::vector<std::unique_ptr<Node>> m_nodes;

	// �`��t���O
	bool m_isDraw;

public:
	// �R���X�g���N�^
	Layer();

	// ������
	void Clear();

	// �m�[�h�ǉ�
	//void AddNode(std::unique_ptr<Node>&& pNode) { m_nodes.push_back(pNode); }

	// �S�`��̍ŏ���ܔ����Z�o
	BoundingBox<double> CalcBoundingBox() const;

	// �`��t���O
	void SetIsDraw(bool val) { m_isDraw = val; }
	bool GetIsDraw() const { return m_isDraw; }

	// �`��
	void Draw();
};


// �`��Ǘ��N���X
class Manager
{
private:
	// �`��L�����o�X
	Canvas m_canvas;

	// �x�[�X���C���[
	Layer m_baseLayer;
	// �`�惌�C���[�R���N�V����
	std::vector<std::unique_ptr<Layer>> m_layers;

	// �J�����g�y��
	LOGPEN m_currentPen;
	// �J�����g�u���V
	LOGBRUSH m_currentBrush;

	// �w�i�F
	COLORREF m_backColor;
	// �O���b�h�F
	COLORREF m_gridColor;
	// �O���b�h�T�C�Y
	double m_gridSize;
	// ���_�F
	COLORREF m_originColor;
	// ���_�T�C�Y
	long m_originSize;
	// ���F
	COLORREF m_axisColor;
	// ���X�P�[��
	double m_axisScale;
	// �O���b�h�`���
	bool m_isDrawGrid;
	// ���_�`���
	bool m_isDrawOrigin;
	// ���`���
	bool m_isDrawAxis;
	// ���`���
	bool m_isDrawArrow;
	// �~���S�_�`���
	bool m_isDrawCenter;

	// �J�����g���C���[�ԍ�
	int m_currentLayerNo;

	// �S�`��̍ŏ���ܔ����Z�o
	BoundingBox<double> CalcBoundingBox() const;

public:
	// �萔
	// �f�t�H���g�y�� = �\���b�h, ����1, ���F
	static constexpr LOGPEN DEFAULT_PEN = LOGPEN{ PS_SOLID, {1, 0}, RGB(0xFF, 0xFF, 0xFF) };
	// �f�t�H���g�u���V = �\���b�h, ���F, �n�b�`����
	static constexpr LOGBRUSH DEFAULT_BRUSH = LOGBRUSH{ BS_SOLID, RGB(0xFF, 0xFF, 0xFF), 0 };
	// �ő�g�嗦
	static constexpr double RATIO_MAX = 50000.0;
	// �ŏ��k����
	static constexpr double RATIO_MIN = 0.0001;
	// �ő�ړ���X,Y
	static constexpr double OFFSET_MAX = 999999.0;

	// �R���X�g���N�^
	Manager(CDC* pDC, const CRect& rect);

	// �R�s�[�R���X�g���N�^
	Manager(const Manager&) = delete;
	// ������Z�q
	Manager& operator=(const Manager&) = delete;

	// �L�����o�X�I�u�W�F�N�g
	const Canvas& GetCanvas() const { return m_canvas; }

	// �J�����g�y��
	void SetCurrentPen(const LOGPEN& val) { m_currentPen = val; };
	LOGPEN GetCurrentPen() const { return m_currentPen; };
	// �J�����g�u���V
	void SetCurrentBrush(const LOGBRUSH& val) { m_currentBrush = val; };
	LOGBRUSH GetCurrentBrush() const { return m_currentBrush; };

	// �w�i�F
	void SetBackColor(COLORREF val) { m_backColor = val; };
	COLORREF GetBackColor() const { return m_backColor; };
	// �O���b�h�F
	void SetGridColor(COLORREF val) { m_gridColor = val; };
	COLORREF GetGridColor() const { return m_gridColor; };
	// �O���b�h�T�C�Y
	void SetGridSize(double val) { m_gridSize = val; };
	double GetGridSize() const { return m_gridSize; };
	// ���_�F
	void SetOriginColor(COLORREF val) { m_originColor = val; };
	COLORREF GetOriginColor() const { return m_originColor; };
	// ���_�T�C�Y
	void SetOriginSize(long val) { m_originSize = val; };
	long GetOriginSize() const { return m_originSize; };
	// ���F
	void SetAxisColor(COLORREF val) { m_axisColor = val; };
	COLORREF GetAxisColor() const { return m_axisColor; };
	// ���X�P�[��
	void SetAxisScale(double val) { m_axisScale = val; };
	double GetAxisScale() const { return m_axisScale; };
	// �O���b�h�`���
	void SetIsDrawGrid(bool val) { m_isDrawGrid = val; };
	bool GetIsDrawGrid() const { return m_isDrawGrid; };
	// ���_�`���
	void SetIsDrawOrigin(bool val) { m_isDrawOrigin = val; };
	bool GetIsDrawOrigin() const { return m_isDrawOrigin; };
	// ���`���
	void SetIsDrawAxis(bool val) { m_isDrawAxis = val; };
	bool GetIsDrawAxis() const { return m_isDrawAxis; };
	// ���`���
	void SetIsDrawArrow(bool val) { m_isDrawArrow = val; };
	bool GetIsDrawArrow() const { return m_isDrawArrow; };
	// �~���S�_�`���
	void SetIsDrawCenter(bool val) { m_isDrawCenter = val; };
	bool GetIsDrawCenter() const { return m_isDrawCenter; };

	// �J�����g���C���[�ԍ�
	void SetCurrentLayerNo(int val) { m_currentLayerNo = val; };
	int GetCurrentLayerNo() const { return m_currentLayerNo; };

	// ���C���[�������擾
	size_t GetLayerCount() const { return m_layers.size(); }

	// �`��̈�̏����Đݒ�
	// OnSize()���̃C�x���g�ŌĂяo���K�v����
	void ResetCanvas(CDC* pDC, const CRect& rect) { m_canvas.Reset(pDC, rect); }

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
};


// �m�[�h�h���N���X�F�O���b�h
class NodeGrid : public Node
{
private:
	// �T�C�Y
	double m_size;

public:
	// �R���X�g���N�^
	NodeGrid(const Canvas& canvas, const LOGPEN& pen, const LOGBRUSH& brush, double size) :
		Node(canvas, pen, brush),
		m_size(size)
	{
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// �`�󂪕`��G���A�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeDrawArea() const override;
	// �`��
	virtual void Draw() override;
};


// �m�[�h�h���N���X�F���_
class NodeOrigin : public Node
{
private:
	// ���W�f�[�^
	Coord<double> m_point;
	// �T�C�Y
	long m_size;

public:
	// �R���X�g���N�^
	NodeOrigin(const Canvas& canvas, const LOGPEN& pen, const LOGBRUSH& brush, const Coord<double>& point, long size) :
		Node(canvas, pen, brush),
		m_point(point),
		m_size(size)
	{
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// �`�󂪕`��G���A�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeDrawArea() const override;
	// �`��
	virtual void Draw() override;
};


// �m�[�h�h���N���X�F��
class NodeAxis : public Node
{
private:
	// ���W�f�[�^
	Coord<double> m_point;
	// �T�C�Y
	double m_scale;

public:
	// �R���X�g���N�^
	NodeAxis(const Canvas& canvas, const LOGPEN& pen, const LOGBRUSH& brush, const Coord<double>& point, double scale) :
		Node(canvas, pen, brush),
		m_point(point),
		m_scale(scale)
	{
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// �`�󂪕`��G���A�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeDrawArea() const override;
	// �`��
	virtual void Draw() override;
};


// �m�[�h�h���N���X�F�_
class NodePoint : public Node
{
private:
	// ���W�f�[�^
	Coord<double> m_point;
	// �_�̎��
	PointType m_pointType;

public:
	// �R���X�g���N�^
	NodePoint(const Canvas& canvas, const LOGPEN& pen, const LOGBRUSH& brush, const Coord<double>& point, PointType pointType) :
		Node(canvas, pen, brush),
		m_point(point),
		m_pointType(pointType)
	{
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// �`�󂪕`��G���A�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeDrawArea() const override;
	// �`��
	virtual void Draw() override;
};


// �m�[�h�h���N���X�F��
class NodeLine : public Node
{
private:
	// ���W�f�[�^
	Coords<double, 2> m_points;
	// ���E�l�̎��
	LineLimitType m_lineLimitType;

public:
	// �R���X�g���N�^
	NodeLine(const Canvas& canvas, const LOGPEN& pen, const LOGBRUSH& brush, const Coords<double, 2>& points, LineLimitType lineLimitType) :
		Node(canvas, pen, brush),
		m_points(points),
		m_lineLimitType(lineLimitType)
	{
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// �`�󂪕`��G���A�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeDrawArea() const override;
	// �`��
	virtual void Draw() override;
};


// �m�[�h�h���N���X�F�~��
class NodeArc : public Node
{
private:
	// ���W�f�[�^
	Coords<double, 3> m_points;
	// �~�ʂ̕����̎��
	ArcDirectionType m_arcDirectionType;

public:
	// �R���X�g���N�^
	NodeArc(const Canvas& canvas, const LOGPEN& pen, const LOGBRUSH& brush, const Coords<double, 3>& points, ArcDirectionType arcDirectionType) :
		Node(canvas, pen, brush),
		m_points(points),
		m_arcDirectionType(arcDirectionType)
	{
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// �`�󂪕`��G���A�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeDrawArea() const override;
	// �`��
	virtual void Draw() override;
};


// �m�[�h�h���N���X�F�~
class NodeCircle : public Node
{
private:
	// ���W�f�[�^
	Coord<double> m_point;
	// ���a
	double m_radius;
	// �h��Ԃ��̎��
	FillType m_fillType;

public:
	// �R���X�g���N�^
	NodeCircle(const Canvas& canvas, const LOGPEN& pen, const LOGBRUSH& brush, const Coord<double>& point, double radius, FillType fillType) :
		Node(canvas, pen, brush),
		m_point(point),
		m_radius(radius),
		m_fillType(fillType)
	{
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// �`�󂪕`��G���A�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeDrawArea() const override;
	// �`��
	virtual void Draw() override;
};


// �m�[�h�h���N���X�F���p�`
class NodePolygon : public Node
{
private:
	// ���W�f�[�^
	std::vector<Coord<double>> m_points;
	// �h��Ԃ��̎��
	FillType m_fillType;

public:
	// �R���X�g���N�^
	NodePolygon(const Canvas& canvas, const LOGPEN& pen, const LOGBRUSH& brush, const std::vector<Coord<double>>& points, FillType fillType) :
		Node(canvas, pen, brush),
		m_points(points),
		m_fillType(fillType)
	{
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// �`�󂪕`��G���A�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeDrawArea() const override;
	// �`��
	virtual void Draw() override;
};


// �m�[�h�h���N���X�F��`
class NodeSector : public Node
{
private:
	// ���W�f�[�^
	Coords<double, 3> m_points;
	// �����̔��a
	double m_innerRadius;
	// �~�ʂ̕����̎��
	ArcDirectionType m_arcDirectionType;
	// �h��Ԃ��̎��
	FillType m_fillType;

public:
	// �R���X�g���N�^
	NodeSector(const Canvas& canvas, const LOGPEN& pen, const LOGBRUSH& brush, const Coords<double, 3>& points, double innerRadius,
		ArcDirectionType arcDirectionType, FillType fillType) :
		Node(canvas, pen, brush),
		m_points(points),
		m_innerRadius(innerRadius),
		m_arcDirectionType(arcDirectionType),
		m_fillType(fillType)
	{
	}

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox() const override;
	// �`�󂪕`��G���A�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeDrawArea() const override;
	// �`��
	virtual void Draw() override;
};

}	// namespace Drawer
