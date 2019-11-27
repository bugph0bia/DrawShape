#include "pch.h"
#include "Drawer.h"

namespace Drawer
{

// �R���X�g���N�^
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
	// ������
	Clear();
}

// ������
void Manager::Clear()
{
	// �x�[�X���C���[��������
	m_baseLayer.Clear();
	// TODO: �O���b�h��o�^
	// TODO: ���_��o�^
	// TODO: ����o�^

	// ���C���[�R���N�V�������N���A
	m_layers.clear();
	// �J�����g���C���[��ǉ�
	m_layers.push_back(Layer(*this));
	m_currentLayerNo = 0;
}

// �`��
void Manager::Draw()
{
	// TODO:
}

// �g��k��
void Manager::Zoom(double ratio)
{
	// TODO:
}

// �p��
void Manager::Pan(Coord<double> offset)
{
	// TODO:
}
// �t�B�b�g
void Manager::Fit()
{
	// TODO:
}

// ��ʃR���g���[���̋�`���擾�i�N���C�A���g���W�j
CRect Manager::GetControlRect() const
{
	CRect r;
	m_ctrl.GetClientRect(&r);
	return r;
}

// �S�`��̍ŏ���ܔ����Z�o
BoundingBox<double> Manager::CalcBoundingBox() const
{
	BoundingBox<double> bb;
	// �S���C���[�̍ŏ���ܔ�������
	for (auto layer : m_layers) {
		bb += layer.CalcBoundingBox();
	}
	return bb;
}


// �R���X�g���N�^
Canvas::Canvas(CDC& dc) :
	m_dc(dc),
	m_ratio(DEFAULT_RATIO),
	m_offset(DEFAULT_OFFSET)
{
	// �f�t�H���g�y�� = �\���b�h, ����1, ���F
	m_pen.CreatePen(PS_SOLID, 1, RGB(0xFF, 0xFF, 0xFF));
	// �f�t�H���g�u���V = �\���b�h, ���F
	LOGBRUSH logBrush { BS_SOLID, RGB(0xFF, 0xFF, 0xFF), 0};
	m_brush.CreateBrushIndirect(&logBrush);
}

// ���W�n�ϊ��F�R���g���[�����W�n�������L�����o�X���W�n
void Canvas::CanvasToControl(Coord<long> ctrlCoord, Coord<double>& canvasCoord) const
{
	// TODO:
}

// ���W�n�ϊ��F�����L�����o�X���W�n���R���g���[�����W�n
void Canvas::ControlToCanvas(Coord<double>& canvasCoord, Coord<long> ctrlCoord) const
{
	// TODO:
}

// �w�i��h��Ԃ�
void Canvas::FillBackground(COLORREF color)
{
	// TODO:
}

// �O���b�h�`��
void Canvas::DrawGrid(COLORREF color, double size)
{
	// TODO:
}

// ���_�`��
void Canvas::DrawOrigin(COLORREF color, double size)
{
	// TODO:
}

// ���`��
void Canvas::DrawAxis(COLORREF color, double scale)
{
	// TODO:
}

// �_�`��
void Canvas::DrawPoint()
{
	// TODO:
}

// ���`��
void Canvas::DrawArrow()
{
	// TODO:
}

// �x�W�G�Ȑ��ɂ��~�ʕ`��
void Canvas::DrawBezierArc()
{
	// TODO:
}


// �`����e���t�@�C���ۑ�
bool Canvas::SaveBitmap(const std::string filePath) const
{
	// TODO:
}

// �`����e���N���b�v�{�[�h�փR�s�[
bool Canvas::CopyBitmap() const
{
	// TODO:
}


// �R���X�g���N�^
Layer::Layer(Manager& mng) :
	m_mng(mng),
	m_isDraw(false)
{
}

// ������
void Layer::Clear()
{
	// �m�[�h�R���N�V�������N���A
	m_nodes.clear();

	// TODO: �J�����g�̃y���ݒ�m�[�h��ǉ�
	// TODO: �J�����g�̃u���V�ݒ�m�[�h��ǉ�
}

// �S�`��̍ŏ���ܔ����Z�o
BoundingBox<double> Layer::CalcBoundingBox() const
{
	BoundingBox<double> bb;
	// �S�m�[�h�̍ŏ���ܔ�������
	for (auto layer : m_nodes) {
		bb += layer.CalcBoundingBox();
	}
	return bb;
}

// �`��
void Layer::Draw()
{
	// TODO:
}











}	// namespace Drawer
