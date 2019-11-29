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
	m_brush.CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));
}

// ���W�n�ϊ��F�����L�����o�X���W�n���R���g���[�����W�n
Coord<long> Canvas::CanvasToControl(const Coord<double> &canvasCoord) const
{
	Coord<long> ctrlCoord;
	// �R���g���[�����W�l���v�Z
	ctrlCoord.x = static_cast<long>(canvasCoord.x * m_ratio + m_offset.x);
	ctrlCoord.y = static_cast<long>(-canvasCoord.y * m_ratio + m_offset.y);	// Y�����]
	return ctrlCoord;
}

// ���W�n�ϊ��F�R���g���[�����W�n�������L�����o�X���W�n
Coord<double> Canvas::ControlToCanvas(const Coord<long> &ctrlCoord) const
{
	// �����L�����o�X���W�l���v�Z
	Coord<double> canvasCoord;
	canvasCoord.x = static_cast<double>((ctrlCoord.x - m_offset.x) / m_ratio);
	canvasCoord.y = -static_cast<double>((ctrlCoord.y - m_offset.y) / m_ratio);	// Y�����]
	return canvasCoord;
}

// ���W�n�ϊ��F�R���g���[�����W�n�������L�����o�X���W�n
// �R���g���[���S�̂��L�����o�X���W�ɕϊ�
BoundingBox<double> Canvas::ControlToCanvas(const CRect& rect) const
{
	// �����L�����o�X���W�l���v�Z
	BoundingBox<double> canvas;
	canvas.min = ControlToCanvas(Coord<long>{ 0, rect.Height() });
	canvas.max = ControlToCanvas(Coord<long>{ rect.Width(), 0 });
	return canvas;
}

// �w�i��h��Ԃ�
void Canvas::FillBackground(COLORREF color, const CRect& rect)
{
	m_pDC->FillSolidRect(rect, color);
}

// �O���b�h�`��
void Canvas::DrawGrid(COLORREF color, double size, const CRect& rect)
{
	// �O���b�h���ׂ���������`����s��Ȃ�
	if ((size * m_ratio) < DRAW_GRID_SIZE_MIN) return;

	// �y����ύX
	PenBrushChanger pc(GetDC(), LOGPEN{ PS_SOLID, POINT{1, 0}, color });

	// �L�����o�X�S�̂̍��W���擾
	BoundingBox<double>	canvas = ControlToCanvas(rect);

	// �L�����o�X�Ɋ܂܂��O���b�h�̃C���f�b�N�X
	long xstart = static_cast<long>(canvas.min.x / size);
	long xend = static_cast<long>(canvas.max.x / size);
	long ystart = static_cast<long>(canvas.min.y / size);
	long yend = static_cast<long>(canvas.max.y / size);

	// �O���b�h�`��
	for (long y = ystart; y <= yend; y++) {
		for (long x = xstart; x <= xend; x++) {
			// �R���g���[�����W���Z�o
			Coord<long> point = CanvasToControl(Coord<double>{ x * size, y * size });
			// �_�`��
			GetDC()->SetPixel(point.x, point.y, color);
		}
	}
}

// ���_�`��
void Canvas::DrawOrigin(COLORREF color, long size)
{
	// �y����ύX
	PenBrushChanger pc(GetDC(), LOGPEN{ PS_SOLID, POINT{1, 0}, color });
	// �u���V��ύX
	PenBrushChanger bc(GetDC(), LOGBRUSH{ BS_SOLID, color, 0 });

	// ���_�̃R���g���[�����W���Z�o
	Coord<long> origin = CanvasToControl(Coord<double>{ 0.0, 0.0 });

	// �����̋�`��`��
	std::array<POINT, 4> points = {
		// ����, �E��, �E��, ����
		POINT{origin.x - ORIGIN_CENTER_SIZE, origin.y - ORIGIN_CENTER_SIZE},
		POINT{origin.x + ORIGIN_CENTER_SIZE, origin.y - ORIGIN_CENTER_SIZE},
		POINT{origin.x + ORIGIN_CENTER_SIZE, origin.y + ORIGIN_CENTER_SIZE},
		POINT{origin.x - ORIGIN_CENTER_SIZE, origin.y + ORIGIN_CENTER_SIZE}
	};
	GetDC()->Polygon(points.data(), points.size());

	// X�������̖��
	GetDC()->MoveTo(origin.x, origin.y);
	GetDC()->LineTo(origin.x + size, origin.y);
	// ����[��`��
	DrawArrowHead(
		Coord<double>{ 0.0, 0.0 },
		ControlToCanvas(Coord<long>{ origin.x + size, origin.y })
	);

	// Y�������̖��
	GetDC()->MoveTo(origin.x, origin.y);
	GetDC()->LineTo(origin.x, origin.y - size);
	// ����[��`��
	DrawArrowHead(
		Coord<double>{ 0.0, 0.0 },
		ControlToCanvas(Coord<long>{ origin.x, origin.y - size })
	);
}

// ���`��
void Canvas::DrawAxis(COLORREF color, double scale, const CRect& rect)
{
	// �y����ύX
	PenBrushChanger pc(GetDC(), LOGPEN{ PS_SOLID, POINT{1, 0}, color });

	// �L�����o�X�S�̂̍��W���擾
	BoundingBox<double>	canvas = ControlToCanvas(rect);

	// ���_�̃R���g���[�����W���Z�o
	Coord<long> origin = CanvasToControl(Coord<double>{ 0.0, 0.0 });

	// X�����\�������ꍇ
	if (canvas.min.y < 0.0 && 0.0 < canvas.max.y) {

		// X����`��
		GetDC()->MoveTo(0, origin.y);
		GetDC()->LineTo(rect.Width(), origin.y);
	}

	// Y�����\�������ꍇ
	if (canvas.min.y < 0.0 && 0.0 < canvas.max.x) {

		// ���`��
		GetDC()->MoveTo(origin.x, 0);
		GetDC()->LineTo(origin.x, rect.Height());
	}

	// �ڐ����ׂ���������`����s��Ȃ�
	if ((scale * m_ratio) < DRAW_AXIS_SCALE_MIN) return;

	// X�����\�������ꍇ
	if (canvas.min.y < 0.0 && 0.0 < canvas.max.y) {
		// �L�����o�X�Ɋ܂܂��ڐ��̃C���f�b�N�X
		long xstart = static_cast<long>(canvas.min.x / scale);
		long xend = static_cast<long>(canvas.max.x / scale);

		// �ڐ��`��
		for (long x = xstart; x <= xend; x++) {
			// �R���g���[�����W�ɕϊ�
			Coord<long> point = CanvasToControl(Coord<double>{ x * scale, 0.0 });
			// �`��
			GetDC()->MoveTo(point.x, point.y - AXIS_SCALE_LENGTH);
			GetDC()->LineTo(point.x, point.y + AXIS_SCALE_LENGTH + 1);
		}
	}

	// Y�����\�������ꍇ
	if (canvas.min.y < 0.0 && 0.0 < canvas.max.x) {
		// �L�����o�X�Ɋ܂܂��ڐ��̃C���f�b�N�X
		long ystart = static_cast<long>(canvas.min.y / scale);
		long yend = static_cast<long>(canvas.max.y / scale);

		// �ڐ��`��
		for (long y = ystart; y <= yend; y++) {
			// �R���g���[�����W�ɕϊ�
			Coord<long> point = CanvasToControl(Coord<double> { 0.0, y * scale });
			// �`��
			GetDC()->MoveTo(point.x - AXIS_SCALE_LENGTH, point.y);
			GetDC()->LineTo(point.x + AXIS_SCALE_LENGTH + 1, point.y);
		}
	}
}

// �_�������`��
void Canvas::DrawLargePoint(const Coord<double>& point)
{
	// ���݂̃y���������ɕύX
	LOGPEN logPen;
	GetDC()->GetCurrentPen()->GetLogPen(&logPen);
	logPen.lopnStyle = PS_SOLID;

	// �y����ύX
	PenBrushChanger pc(GetDC(), logPen);

	// �_�̃R���g���[�����W���Z�o
	Coord<long> point_ctrl = CanvasToControl(point);

	// �`��i�と�E���������j
	GetDC()->MoveTo(point_ctrl.x, point_ctrl.y - (LARGE_POINT_SIZE + 1));
	GetDC()->LineTo(point_ctrl.x - (LARGE_POINT_SIZE + 1), point_ctrl.y + LARGE_POINT_SIZE);
	GetDC()->LineTo(point_ctrl.x + (LARGE_POINT_SIZE + 1), point_ctrl.y + LARGE_POINT_SIZE);
	GetDC()->LineTo(point_ctrl.x, point_ctrl.y - (LARGE_POINT_SIZE + 1));
}

// ����[�`��
void Canvas::DrawArrowHead(const Coord<double>& start, const Coord<double>& end)
{
	// ���݂̃y���������ɕύX
	LOGPEN logPen;
	GetDC()->GetCurrentPen()->GetLogPen(&logPen);
	logPen.lopnStyle = PS_SOLID;

	// �y����ύX
	PenBrushChanger pc(GetDC(), logPen);

	// ��[�̃R���g���[�����W���Z�o
	Coord<long> tip = CanvasToControl(end);

	// �����̒���
	double length = sqrt(pow((end.x - start.x), 2) + pow((end.y - start.y), 2));
	// �����̒P�ʃx�N�g�����Z�o
	Coord<double> unit;
	unit.x = (end.x - start.x) / length;
	unit.y = (end.y - start.y) / length;

	// �������E���̉H
	for (double angle = PI - ARROW_WING_ANGLE; angle <= PI + ARROW_WING_ANGLE; angle += ARROW_WING_ANGLE * 2) {
		// �H�̐�[�̍��W
		Coord<long> wing = CanvasToControl(
			Coord<double>{
				unit.x* cos(angle) - unit.y * sin(angle),
				unit.x* sin(angle) + unit.y * cos(angle)
			}
		);
		// �`��
		GetDC()->MoveTo(tip.x, tip.y);
		GetDC()->LineTo(wing.x, wing.y);
	}
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
	BoundingBox<double> bbox;
	// �`��t���OON�Ȃ�
	if (m_isDraw) {
		// �S�m�[�h�̍ŏ���ܔ�������
		for (auto& pNode : m_nodes) {
			bbox += pNode->CalcBoundingBox();
		}
	}
	return bbox;
}

// �`��
void Layer::Draw()
{
	// �`��t���OON�Ȃ�
	if (m_isDraw) {
		// �S�m�[�h��`��
		for (auto& pNode : m_nodes) {
			pNode->Draw();
		}
	}
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
	// TODO: ����o�^
	// TODO: ���_��o�^

	// ���C���[�R���N�V�������N���A
	m_layers.clear();
	// �J�����g���C���[��ǉ�
	m_layers.push_back(std::make_unique<Layer>(*this));
	m_currentLayerNo = 0;
}

// �`��
void Manager::Draw()
{
	// �w�i��h��Ԃ�
	m_canvas.FillBackground(m_backColor, GetControlRect());

	// �x�[�X���C���[��`��
	m_baseLayer.Draw();

	// �S���C���[��`��
	for (auto& pLayer : m_layers) {
		pLayer->Draw();
	}

	// �`��C�x���g���s
	m_pCtrl->InvalidateRect(nullptr, FALSE);	// �̈斳����
	m_pCtrl->UpdateWindow();					// �ĕ`�施��
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
	// ���`��
	if (m_isDrawAxis) m_canvas.DrawAxis(m_axisColor, m_axisScale, rect);
	// ���_�`��
	if (m_isDrawOrigin) m_canvas.DrawOrigin(m_originColor, m_originSize);
}

// �g��k��
bool Manager::Zoom(double coef, const Coord<long>& base)
{
	// ���݂̊g��k�����ɌW�����|����
	double ratio = m_canvas.GetRatio() * coef;

	// ����ȏ�g��k���ł��Ȃ�
	if (ratio < RATIO_MIN || RATIO_MAX < ratio) return false;

	// �g��k�������X�V
	m_canvas.SetRatio(ratio);

	// ����W����`��I�t�Z�b�g�ʂ��Z�o���čX�V
	Coord<double> offset = m_canvas.GetOffset();
	offset.x = base.x - ((base.x - offset.x) * coef);
	offset.y = base.y - ((base.y - offset.y) * coef);
	m_canvas.SetOffset(offset);

	// �ĕ`��
	Draw();

	return true;
}

// �g��k���i�J�[�\���ʒu��j
bool Manager::Zoom(double coef)
{
	// �}�E�X�J�[�\���ʒu
	CPoint cursor;
	::GetCursorPos(&cursor);

	// �R���g���[���̋�`�i�X�N���[�����W�n�j
	CRect rect;
	m_pCtrl->GetWindowRect(&rect);

	// �}�E�X�J�[�\�����R���g���[�����ɖ���
	if (!rect.PtInRect(cursor)) return false;

	// �}�E�X�J�[�\���ʒu���R���g���[���̃N���C�A���g���W�ɕϊ�
	m_pCtrl->ScreenToClient(&cursor);

	// �g��k��
	return Zoom(coef, Coord<long>{ cursor.x, cursor.y });
}

// �p��
bool Manager::Pan(const Coord<long>& move)
{
	// ���݂̊g��k����
	double ratio = m_canvas.GetRatio();
	// ���݂̃I�t�Z�b�g
	Coord<double> offset = m_canvas.GetOffset();

	// ����ȏ�I�t�Z�b�g�ł��Ȃ�
	if (OFFSET_MAX < fabs((offset.x + move.x) / ratio)) return false;
	if (OFFSET_MAX < fabs((offset.y + move.y) / ratio)) return false;

	// �I�t�Z�b�g�ʂ��X�V
	offset.x += move.x;
	offset.y += move.y;
	m_canvas.SetOffset(offset);

	// �ĕ`��
	Draw();

	return true;
}

// �t�B�b�g
void Manager::Fit(double shapeOccupancy)
{
	// �R���g���[���̋�`���擾
	CRect rect;
	m_pCtrl->GetClientRect(&rect);

	// �ŏ���ܔ��擾
	BoundingBox<double> bbox = CalcBoundingBox();

	// �g��k����
	double ratio;
	// �I�t�Z�b�g
	Coord<double> offset;

	// �ŏ���ܔ��擾���s
	if (!bbox.Verify()) {
		// �����l�Ƃ���
		ratio = Canvas::DEFAULT_RATIO;
		offset = Coord<double>{
			static_cast<double>(rect.CenterPoint().x),
			static_cast<double>(rect.CenterPoint().y)
		};
	}
	else {
		// �`��̏c������Z�o
		double shapeAspect = std::numeric_limits<double>::max();
		if (bbox.GetWidth() != 0.0) {
			shapeAspect = bbox.GetHeight() / bbox.GetWidth();
		}

		// �`��̈�̏c����
		double cnvsAspect = std::numeric_limits<double>::max();
		if (rect.Width() != 0.0) {
			cnvsAspect = static_cast<double>(rect.Height()) / rect.Width();
		}

		// �g��k�������Z�o
		// �`��̈�ɑ΂��Č`�󂪏c���Ȃ�c��������Ƃ���
		if (bbox.GetWidth() == 0.0 || shapeAspect > cnvsAspect) {
			// (�`��̈�T�C�Y / �`��T�C�Y) * �`��̐�L��
			ratio = (rect.Height() / bbox.GetHeight()) * shapeOccupancy;
		}
		// �`��̈�ɑ΂��Č`�󂪉����Ȃ牡��������Ƃ���
		else {
			// (�`��̈�T�C�Y / �`��T�C�Y) * �`��̐�L��
			ratio = (rect.Width() / bbox.GetWidth()) * shapeOccupancy;
		}

		// �I�t�Z�b�g���Z�o
		offset.x = (rect.Width() - bbox.GetWidth()) * ratio / 2.0 - bbox.min.x * ratio;
		double y = (rect.Height() - bbox.GetHeight()) * ratio / 2.0 - bbox.min.y * ratio;
		offset.y = rect.Height() - y;	// Y�����]
	}

	// �g��k�����ƃI�t�Z�b�g���X�V
	m_canvas.SetRatio(ratio);
	m_canvas.SetOffset(offset);

	// �ĕ`��
	Draw();
}

// �S�`��̍ŏ���ܔ����Z�o
BoundingBox<double> Manager::CalcBoundingBox() const
{
	BoundingBox<double> bbox;
	// �S���C���[�̍ŏ���ܔ�������
	for (auto& pLayer : m_layers) {
		bbox += pLayer->CalcBoundingBox();
	}
	return bbox;
}

// ��ʃR���g���[���̋�`���擾�i�N���C�A���g���W�j
CRect Manager::GetControlRect() const
{
	CRect r;
	m_pCtrl->GetClientRect(&r);
	return r;
}










}	// namespace Drawer
