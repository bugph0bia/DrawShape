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
// �N���X�O���錾
class Manager;
class Canvas;
class Layer;
class Node;


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

	// �J�����g�y���I�u�W�F�N�g
	CPen m_pen;
	// �J�����g�u���V�I�u�W�F�N�g
	CBrush m_brush;

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
	Canvas(CDC* pDC);

	// �g��k����
	void SetRatio(double val) { m_ratio = val;  }
	double GetRatio() const { return m_ratio; }
	// �I�t�Z�b�g
	void SetOffset(Coord<double> val) { m_offset = val; }
	Coord<double> GetOffset() const { return m_offset; }

	// ���W�n�ϊ��F�����L�����o�X���W�n���R���g���[�����W�n
	Coord<long> CanvasToControl(const Coord<double> &canvasCoord) const;
	// ���W�n�ϊ��F�R���g���[�����W�n�������L�����o�X���W�n
	Coord<double> ControlToCanvas(const Coord<long>& ctrlCoord) const;
	BoundingBox<double> ControlToCanvas(const CRect& rect) const;

	// �w�i��h��Ԃ�
	void FillBackground(COLORREF color, const CRect& rect);
	// �O���b�h�`��
	void DrawGrid(COLORREF color, double size, const CRect& rect);
	// ���_�`��
	void DrawOrigin(COLORREF color, long size);
	// ���`��
	void DrawAxis(COLORREF color, double scale, const CRect& rect);
	// �_�������`��
	void DrawLargePoint(const Coord<double>& point);
	// ����[�`��
	void DrawArrowHead(const Coord<double>& start, const Coord<double>& end);
	// �x�W�G�Ȑ��ɂ��~�ʕ`��
	void DrawBezierArc(Coord<double> start, Coord<double> end, Coord<double> center, bool rht);

	// �`����e���t�@�C���ۑ�(BMP/PNG/JPEG/GIF)
	bool SaveImage(const std::tstring& filePath) const;
	// �`����e���N���b�v�{�[�h�փR�s�[
	bool CopyImage(CWnd* pOwner) const;

	// �f�o�C�X�R���e�L�X�g���擾
	CDC* GetDC() const { return m_pDC; };
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
	// ���W�f�[�^�z��^
	template<typename T, size_t N>
	using coords_t = std::array<Coord<T>, N>;

private:
	// �`��Ǘ��I�u�W�F�N�g
	Manager& m_mng;

public:
	// �R���X�g���N�^
	Node(Manager& mng) : m_mng(mng) {};
	// �f�X�g���N�^
	virtual ~Node() {};

	// �`��̍ŏ���ܔ����Z�o
	virtual BoundingBox<double> CalcBoundingBox() const = 0;
	// �`�󂪕`��G���A�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeDrawArea() const = 0;
	// �`��
	virtual void Draw() = 0;

};


// ���C���[�N���X
class Layer
{
private:
	// �`��Ǘ��I�u�W�F�N�g
	Manager& m_mng;
	// �m�[�h�R���N�V����
	std::vector<std::unique_ptr<Node>> m_nodes;

	// �`��t���O
	bool m_isDraw;

public:
	// �R���X�g���N�^
	Layer(Manager& mng);

	// ������
	void Clear();

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
	// ��ʃR���g���[��
	COleControl* m_pCtrl;
	// �`��L�����o�X
	Canvas m_canvas;

	// �x�[�X���C���[
	Layer m_baseLayer;
	// �`�惌�C���[�R���N�V����
	std::vector<std::unique_ptr<Layer>> m_layers;

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
	// �ő�g�嗦
	static constexpr double RATIO_MAX = 50000.0;
	// �ŏ��k����
	static constexpr double RATIO_MIN = 0.0001;
	// �ő�ړ���X,Y
	static constexpr double OFFSET_MAX = 999999.0;

	// �R���X�g���N�^
	Manager(COleControl* pCtrl, CDC* pDC);

	// �R�s�[�R���X�g���N�^
	Manager(const Manager&) = delete;
	// ������Z�q
	Manager& operator=(const Manager&) = delete;

	// �w�i�F
	void SetBackColor(COLORREF val) { m_backColor = val; };
	COLORREF GetBackColor() { return m_backColor; };
	// �O���b�h�F
	void SetGridColor(COLORREF val) { m_gridColor = val; };
	COLORREF GetGridColor() { return m_gridColor; };
	// �O���b�h�T�C�Y
	void SetGridSize(double val) { m_gridSize = val; };
	double GetGridSize() { return m_gridSize; };
	// ���_�F
	void SetOriginColor(COLORREF val) { m_originColor = val; };
	COLORREF GetOriginColor() { return m_originColor; };
	// ���_�T�C�Y
	void SetOriginSize(long val) { m_originSize = val; };
	long GetOriginSize() { return m_originSize; };
	// ���F
	void SetAxisColor(COLORREF val) { m_axisColor = val; };
	COLORREF GetAxisColor() { return m_axisColor; };
	// ���X�P�[��
	void SetAxisScale(double val) { m_axisScale = val; };
	double GetAxisScale() { return m_axisScale; };
	// �O���b�h�`���
	void SetIsDrawGrid(bool val) { m_isDrawGrid = val; };
	bool GetIsDrawGrid() { return m_isDrawGrid; };
	// ���_�`���
	void SetIsDrawOrigin(bool val) { m_isDrawOrigin = val; };
	bool GetIsDrawOrigin() { return m_isDrawOrigin; };
	// ���`���
	void SetIsDrawAxis(bool val) { m_isDrawAxis = val; };
	bool GetIsDrawAxis() { return m_isDrawAxis; };
	// ���`���
	void SetIsDrawArrow(bool val) { m_isDrawArrow = val; };
	bool GetIsDrawArrow() { return m_isDrawArrow; };
	// �~���S�_�`���
	void SetIsDrawCenter(bool val) { m_isDrawCenter = val; };
	bool GetIsDrawCenter() { return m_isDrawCenter; };

	// �J�����g���C���[�ԍ�
	void SetCurrentLayerNo(int val) { m_currentLayerNo = val; };
	int GetCurrentLayerNo() { return m_currentLayerNo; };

	// ������
	void Clear();

	// �`��
	void Draw();
	// �`��(�f�U�C�����[�h�p)
	void DrawDesignMode(const CRect& rect);

	// �g��k��
	bool Zoom(double coef, const Coord<long>& base);
	// �g��k���i�J�[�\���ʒu��j
	bool Zoom(double coef);
	// �p��
	bool Pan(const Coord<long>& move);
	// �t�B�b�g
	void Fit(double shapeOccupancy);

	// ���C���[�������擾
	size_t GetLayerCount() const { return m_layers.size(); }

	// ��ʃR���g���[���̋�`���擾�i�N���C�A���g���W�j
	CRect GetControlRect() const;
};


//// �����m�[�h�N���X
//class LineNode : public Node
//{
//private:
//	// ���W�f�[�^�z��
//	coords_t<double, 2> m_datas;
//
//public:
//	// �R���X�g���N�^
//	LineNode(Manager& mng);
//
//	// �`��̍ŏ���ܔ����Z�o
//	BoundingBox<double> CalcBoundingBox() const override;
//	// �`�󂪕`��G���A�Ɋ܂܂�邩�`�F�b�N
//	bool IsIncludeDrawArea() const override;
//
//	// �`��
//	void Draw() override;
//};

}	// namespace DrawShapeLib