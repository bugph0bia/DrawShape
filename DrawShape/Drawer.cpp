#include "pch.h"
#include "Drawer.h"

namespace Drawer
{

// �R���X�g���N�^
Canvas::Canvas(CDC* pDC) :
	m_pDC(pDC),
	m_ratio(DEFAULT_RATIO),
	m_offset(DEFAULT_OFFSET)
{
	// �f�t�H���g�y�� = �\���b�h, ����1, ���F
	m_pen.CreatePen(PS_SOLID, 1, RGB(0xFF, 0xFF, 0xFF));
	// �f�t�H���g�u���V = �\���b�h, ���F
	LOGBRUSH logBrush{ BS_SOLID, RGB(0xFF, 0xFF, 0xFF), 0 };
	m_brush.CreateBrushIndirect(&logBrush);
}

// ���W�n�ϊ��F�����L�����o�X���W�n���R���g���[�����W�n
Coord<long> Canvas::CanvasToControl(const Coord<double> &canvasCoord) const
{
	Coord<long> ctrlCoord;
	// �R���g���[�����W�l���v�Z
	ctrlCoord.x = (long)(canvasCoord.x * m_ratio + m_offset.x);
	ctrlCoord.y = (long)(-canvasCoord.y * m_ratio + m_offset.y);	// Y�����]
	return ctrlCoord;
}

// ���W�n�ϊ��F�R���g���[�����W�n�������L�����o�X���W�n
Coord<double> Canvas::ControlToCanvas(const Coord<long> &ctrlCoord) const
{
	// �����L�����o�X���W�l���v�Z
	Coord<double> canvasCoord;
	canvasCoord.x = (double)(ctrlCoord.x - m_offset.x) / m_ratio;
	canvasCoord.y = -(double)(ctrlCoord.y - m_offset.y) / m_ratio;	// Y�����]
	return canvasCoord;
}

// �w�i��h��Ԃ�
void Canvas::FillBackground(COLORREF color, const CRect& rect)
{
	m_pDC->FillSolidRect(rect, color);
}

// �O���b�h�`��
void Canvas::DrawGrid(COLORREF color, double size, const CRect& rect)
{
	// TODO:
}

// ���_�`��
void Canvas::DrawOrigin(COLORREF color, double size)
{
	// TODO:
}

// ���`��
void Canvas::DrawAxis(COLORREF color, double scale, const CRect& rect)
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
bool Canvas::SaveBitmap(const std::string& filePath) const
{
	// TODO:
	return true;
}

// �`����e���N���b�v�{�[�h�փR�s�[
bool Canvas::CopyBitmap() const
{
	// TODO:
	return true;
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
	for (auto& pNode : m_nodes) {
		bb += pNode->CalcBoundingBox();
	}
	return bb;
}

// �`��
void Layer::Draw()
{
	// TODO:
}


// �R���X�g���N�^
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
	m_layers.push_back(std::make_unique<Layer>(*this));
	m_currentLayerNo = 0;
}

// �`��
void Manager::Draw()
{
	// TODO:
}

// �`��(�f�U�C�����[�h�p)
// �����̊֐����g�p����ꍇ�͈ȉ��𐄏�
//   �E�{�N���X�̃I�u�W�F�N�g�͈ꎞ�I�ȕϐ��Ƃ��A�R���X�g���N�^�� Manager(nullptr, �f�U�C�����[�h�pDC); �ŌĂяo���Ă���
//   �E���Ȃ��Ƃ��ȉ��̃v���p�e�B��ݒ肷��
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
//   �E���� rect �Ƀf�U�C�����[�h���̃R���g���[���̋�`��n��
void Manager::DrawDesignMode(const CRect& rect)
{
	// �g��k�����̓R���X�g���N�g���̏����l�Ƃ��邽�ߕύX���Ȃ�
	// �I�t�Z�b�g�̓R���g���[���̋�`�̒����Ƃ���
	Coord<double> offset { (double)rect.CenterPoint().x, (double)rect.CenterPoint().y };
	m_canvas.SetOffset(offset);

	// �w�i�F�œh��Ԃ�
	m_canvas.FillBackground(m_backColor, rect);

	// �O���b�h�`��
	if (m_isDrawGrid) m_canvas.DrawGrid(m_gridColor, m_gridSize, rect);
	// ���_�`��
	if (m_isDrawOrigin) m_canvas.DrawOrigin(m_originColor, m_originSize);
	// ���`��
	if (m_isDrawAxis) m_canvas.DrawAxis(m_axisColor, m_axisScale, rect);
}

// �g��k��
void Manager::Zoom(double ratio)
{
	// TODO:
}

// �p��
void Manager::Pan(const Coord<double> &offset)
{
	// TODO:
}
// �t�B�b�g
void Manager::Fit()
{
	// TODO:
}

// �S�`��̍ŏ���ܔ����Z�o
BoundingBox<double> Manager::CalcBoundingBox() const
{
	BoundingBox<double> bb;
	// �S���C���[�̍ŏ���ܔ�������
	for (auto& pLayer : m_layers) {
		bb += pLayer->CalcBoundingBox();
	}
	return bb;
}

// ��ʃR���g���[���̋�`���擾�i�N���C�A���g���W�j
CRect Manager::GetControlRect() const
{
	CRect r;
	m_pCtrl->GetClientRect(&r);
	return r;
}










}	// namespace Drawer
