#include "pch.h"
#include <atlimage.h>
#include "Drawer.h"

namespace Drawer
{

// �R���X�g���N�^
Canvas::Canvas() :
	m_pDC(nullptr),
	m_rect(0, 0, 0, 0),
	m_ratio(DEFAULT_RATIO),
	m_offset(DEFAULT_OFFSET),
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
	m_isDrawCenter(false)
{
}

// ���W�n�ϊ��F�����L�����o�X���W�n���R���g���[�����W�n
Coord<long> Canvas::CanvasToControl(const Coord<double> &canvasCoord) const
{
	Coord<long> ctrlCoord;
	// �R���g���[�����W�l���v�Z
	ctrlCoord.x = static_cast<long>(CanvasToControl(canvasCoord.x) + m_offset.x);
	ctrlCoord.y = static_cast<long>(-CanvasToControl(canvasCoord.y) + m_offset.y);	// Y�����]
	return ctrlCoord;
}

// ���W�n�ϊ��F�R���g���[�����W�n�������L�����o�X���W�n
Coord<double> Canvas::ControlToCanvas(const Coord<long> &ctrlCoord) const
{
	// �����L�����o�X���W�l���v�Z
	Coord<double> canvasCoord;
	canvasCoord.x = ControlToCanvas(ctrlCoord.x - m_offset.x);
	canvasCoord.y = -ControlToCanvas(ctrlCoord.y - m_offset.y);	// Y�����]
	return canvasCoord;
}

// �R���N�V�����p�̍��W�n�ϊ��F�����L�����o�X���W�n���R���g���[�����W�n
Coords_v<long> Canvas::CanvasToControl(const Coords_v<double>& canvasCoords) const
{
	Coords_v<long> ctrlCoords;
	for (const auto& c : canvasCoords) {
		ctrlCoords.push_back(CanvasToControl(c));
	}
	return ctrlCoords;
}
// �R���N�V�����p�̍��W�n�ϊ��F�R���g���[�����W�n�������L�����o�X���W�n
//   ���e���v���[�g�֐��Ȃ̂Ńw�b�_���Œ�`����K�v����
Coords_v<double> Canvas::ControlToCanvas(const Coords_v<long>& ctrlCoords) const
{
	Coords_v<double> canvasCoords;
	for (const auto& c : ctrlCoords) {
		canvasCoords.push_back(ControlToCanvas(c));
	}
	return canvasCoords;
}

// �`��̈�S�̂̍��W���擾
BoundingBox<double> Canvas::GetCanvasArea() const
{
	// �����L�����o�X���W�l���v�Z
	BoundingBox<double> canvas;
	canvas.min = ControlToCanvas(Coord<long>(0, m_rect.Height()));
	canvas.max = ControlToCanvas(Coord<long>(m_rect.Width(), 0));
	return canvas;
}

// �w�i��h��Ԃ�
void Canvas::FillBackground() const
{
	m_pDC->FillSolidRect(m_rect, m_backColor);
}

// �O���b�h�`��
void Canvas::DrawGrid() const
{
	// �O���b�h�`��OFF
	if (!m_isDrawGrid) return;
	// �O���b�h���ׂ���������`����s��Ȃ�
	if ((CanvasToControl(m_gridSize)) < DRAW_GRID_SIZE_MIN) return;

	// �L�����o�X�S�̂̍��W���擾
	BoundingBox<double>	canvas = GetCanvasArea();

	// �L�����o�X�Ɋ܂܂��O���b�h�̃C���f�b�N�X
	long xstart = static_cast<long>(canvas.min.x / m_gridSize);
	long xend = static_cast<long>(canvas.max.x / m_gridSize);
	long ystart = static_cast<long>(canvas.min.y / m_gridSize);
	long yend = static_cast<long>(canvas.max.y / m_gridSize);

	// �O���b�h�`��
	for (long y = ystart; y <= yend; y++) {
		for (long x = xstart; x <= xend; x++) {
			// �_��`��
			DrawPixel(Coord<double>(x * m_gridSize, y * m_gridSize));
		}
	}
}

// ���_�`��
void Canvas::DrawOrigin(Coord<double> base) const
{
	// ���_�`��OFF
	if (!m_isDrawOrigin) return;

	// ���_�̃R���g���[�����W���Z�o
	Coord<long> origin = CanvasToControl(base);

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
	GetDC()->LineTo(origin.x + m_originSize, origin.y);
	// ����[��`��
	DrawArrowHead(
		Coords<double, 2>{
			base,
			ControlToCanvas(Coord<long>(origin.x + m_originSize, origin.y))
		}
	);

	// Y�������̖��
	GetDC()->MoveTo(origin.x, origin.y);
	GetDC()->LineTo(origin.x, origin.y - m_originSize);
	// ����[��`��
	DrawArrowHead(
		Coords<double, 2>{
			base,
			ControlToCanvas(Coord<long>(origin.x, origin.y - m_originSize))
		}
	);
}

// ���`��
void Canvas::DrawAxis(Coord<double> base) const
{
	// ���`��OFF
	if (!m_isDrawAxis) return;

	// �L�����o�X�S�̂̍��W���擾
	BoundingBox<double>	canvas = GetCanvasArea();

	// ���_�̃R���g���[�����W���Z�o
	Coord<long> origin = CanvasToControl(base);

	// X�����\�������ꍇ
	if (canvas.min.y < base.y && base.y < canvas.max.y) {

		// X����`��
		GetDC()->MoveTo(0, origin.y);
		GetDC()->LineTo(m_rect.Width(), origin.y);
	}

	// Y�����\�������ꍇ
	if (canvas.min.x < base.x && base.x < canvas.max.x) {

		// ���`��
		GetDC()->MoveTo(origin.x, 0);
		GetDC()->LineTo(origin.x, m_rect.Height());
	}

	// �ڐ����ׂ���������`����s��Ȃ�
	if (CanvasToControl(m_axisScale) < DRAW_AXIS_SCALE_MIN) return;

	// X�����\�������ꍇ
	if (canvas.min.y < base.y && base.y < canvas.max.y) {
		// �L�����o�X�Ɋ܂܂��ڐ��̃C���f�b�N�X
		long xstart = static_cast<long>(canvas.min.x / m_axisScale);
		long xend = static_cast<long>(canvas.max.x / m_axisScale);

		// �ڐ��`��
		for (long x = xstart; x <= xend; x++) {
			// �R���g���[�����W�ɕϊ�
			Coord<long> point = CanvasToControl(Coord<double>(x * m_axisScale, base.y));
			// �`��
			GetDC()->MoveTo(point.x, point.y - AXIS_SCALE_LENGTH);
			GetDC()->LineTo(point.x, point.y + AXIS_SCALE_LENGTH + 1);
		}
	}

	// Y�����\�������ꍇ
	if (canvas.min.x < base.x && base.x < canvas.max.x) {
		// �L�����o�X�Ɋ܂܂��ڐ��̃C���f�b�N�X
		long ystart = static_cast<long>(canvas.min.y / m_axisScale);
		long yend = static_cast<long>(canvas.max.y / m_axisScale);

		// �ڐ��`��
		for (long y = ystart; y <= yend; y++) {
			// �R���g���[�����W�ɕϊ�
			Coord<long> point = CanvasToControl(Coord<double>(base.x, y * m_axisScale));
			// �`��
			GetDC()->MoveTo(point.x - AXIS_SCALE_LENGTH, point.y);
			GetDC()->LineTo(point.x + AXIS_SCALE_LENGTH + 1, point.y);
		}
	}
}

// �s�N�Z��(�_)��`��
void Canvas::DrawPixel(const Coord<double>& point) const
{
	// �R���g���[�����W���Z�o
	Coord<long> ctrlPoint = CanvasToControl(point);

	// �J�����g�y���̐F���擾
	LOGPEN pen;
	GetDC()->GetCurrentPen()->GetLogPen(&pen);
	COLORREF color = pen.lopnColor;

	// �s�N�Z���`��
	GetDC()->SetPixel(ctrlPoint.x, ctrlPoint.y, color);
}

// �_���O�p�`�ŕ`��
void Canvas::DrawTrianglePoint(const Coord<double>& point) const
{
	// ���݂̃y���������ɕύX
	LOGPEN logPen;
	GetDC()->GetCurrentPen()->GetLogPen(&logPen);
	logPen.lopnStyle = PS_SOLID;

	// �y����ύX
	PenBrushChanger pc(GetDC(), logPen);

	// �_�̃R���g���[�����W���Z�o
	Coord<long> ctrlPoint = CanvasToControl(point);

	// �`��i�と�E���������j
	GetDC()->MoveTo(ctrlPoint.x, ctrlPoint.y - (TRIANGLE_POINT_SIZE + 1));
	GetDC()->LineTo(ctrlPoint.x - (TRIANGLE_POINT_SIZE + 1), ctrlPoint.y + TRIANGLE_POINT_SIZE);
	GetDC()->LineTo(ctrlPoint.x + (TRIANGLE_POINT_SIZE + 1), ctrlPoint.y + TRIANGLE_POINT_SIZE);
	GetDC()->LineTo(ctrlPoint.x, ctrlPoint.y - (TRIANGLE_POINT_SIZE + 1));
}

// �_���������ĕ`��
void Canvas::DrawLargePoint(const Coord<double>& point) const
{
	// ���݂̃y���������ɕύX
	LOGPEN logPen;
	GetDC()->GetCurrentPen()->GetLogPen(&logPen);
	logPen.lopnStyle = PS_SOLID;

	// ���݂̃u���V���\���b�h�ɕύX
	LOGBRUSH logBrush;
	GetDC()->GetCurrentBrush()->GetLogBrush(&logBrush);
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbStyle = 0;

	// �y���ƃu���V��ύX
	PenBrushChanger pc(GetDC(), logPen);
	PenBrushChanger bc(GetDC(), logBrush);

	// �_�̃R���g���[�����W���Z�o
	Coord<long> ctrlPoint = CanvasToControl(point);

	// �`��
	GetDC()->Ellipse(
		ctrlPoint.x - TRIANGLE_POINT_SIZE,
		ctrlPoint.y - TRIANGLE_POINT_SIZE,
		ctrlPoint.x + TRIANGLE_POINT_SIZE,
		ctrlPoint.y + TRIANGLE_POINT_SIZE
	);
}

// ����[�`��
void Canvas::DrawArrowHead(const Coords<double, 2>& baseSegment) const
{
	// ���݂̃y���������ɕύX
	LOGPEN logPen;
	GetDC()->GetCurrentPen()->GetLogPen(&logPen);
	logPen.lopnStyle = PS_SOLID;

	// �y����ύX
	PenBrushChanger pc(GetDC(), logPen);

	// ��[�̃R���g���[�����W���Z�o
	Coord<long> tip = CanvasToControl(baseSegment[END]);

	// �����̒���
	double length = baseSegment[START].Length(baseSegment[END]);
	// �����̒P�ʃx�N�g�����Z�o
	Coord<double> unit;
	unit = (baseSegment[END] - baseSegment[START]) / length;

	// �������E���̉H
	for (double angle = PI - ARROW_WING_ANGLE; angle <= PI + ARROW_WING_ANGLE; angle += ARROW_WING_ANGLE * 2) {
		// �H�̐�[�̍��W
		Coord<long> wing = CanvasToControl(
			Coord<double>(
				unit.x * cos(angle) - unit.y * sin(angle),
				unit.x * sin(angle) + unit.y * cos(angle)
			)
		);
		// �`��
		GetDC()->MoveTo(tip.x, tip.y);
		GetDC()->LineTo(wing.x, wing.y);
	}
}

// �x�W�G�Ȑ��ɂ��~�ʕ\�����Z�o
// �n�_�ƏI�_����v����~�ʂŌĂяo�����ꍇ�̓���͖���`�i���̂悤�ȃP�[�X�͎��O�ɉ~�ƌ��Ȃ��ׂ��j
Coords_v<double> Canvas::CalcBezierArc(Coords<double, 3> arc, ArcDirectionType direction) const
{
	// �~�ʕ������E���Ȃ獶���ɕϊ�����
	if (direction == ArcDirectionType::Right) std::swap(arc[START], arc[END]);

	// ���S�_�����_(0, 0)�̈ʒu�Ɉړ������~�ʂ��쐬
	Coords<double, 3> oarc = arc;
	oarc[START] -= arc[CENTER];
	oarc[END] -= arc[CENTER];
	oarc[CENTER] -= arc[CENTER];

	// �~�ʂ̔��a���Z�o
	double radius = oarc[START].Length(oarc[CENTER]);

	// �n�_�ƏI�_�̏ی�(��n�ی���n)�𒲂ׂ�
	std::array<int, 2> quadrant;
	for (int se = START; se <= END; se++) {
		// ��1�ی�
		if (oarc[se].x > 0.0 && oarc[se].y >= 0.0) quadrant[se] = 1;
		// ��2�ی�
		else if (oarc[se].x <= 0.0 && oarc[se].y > 0.0) quadrant[se] = 2;
		// ��3�ی�
		else if (oarc[se].x < 0.0 && oarc[se].y <= 0.0) quadrant[se] = 3;
		// ��4�ی�
		else quadrant[se] = 4;
	}

	// �֋X��A��n�ی��Ƒ�(n+4)�ی��𓯂��Ӗ��ƍl����
	// �i���w�I�ɂ́A�ی��̐�n�� `4��@�Ƃ��������Ȑ�` �Œu�������\�ƌ��Ȃ��j
	//   ��F��2�ی��Ƒ�6�ی��͓����ی��ƍl����
	// ���̏�ŁA�I�_�̏ی� >= �n�_�̏ی� �ƂȂ�悤�ɒ��߂��邱�ƂŁA
	// �ی��̐��l�̍��ŉ~�ʂ������̏ی��Ɍׂ邩��\����悤�ɂ���

	// �n�_�ƏI�_������ی��̏ꍇ
	if (quadrant[START] == quadrant[END]) {

		// ���S�_���n�_�ƒ��S�_���I�_�̃x�N�g���O�ς��A�~�ʂ�180�x�ȏォ���肷��
		double outer = oarc[START].x * oarc[END].x - oarc[START].y * oarc[END].y;
		// 180�x�ȏ�̉~�ʂ̏ꍇ
		if (outer < 0.0) {
			// �n�_����I�_�͑S�Ă̏ی����o�R���邽�ߏI�_�̏ی������Z����
			quadrant[END] += 4;
		}
	}
	// �n�_�ƏI�_����4�ی��Ƒ�1�ی����o�R���Ă���召�֌W������ւ���Ă���ꍇ�A
	// �I�_�̏ی������Z����
	else if (quadrant[START] > quadrant[END]) {
		quadrant[END] += 4;
	}

	// �x�W�G�Ȑ���`�����߂̓_�̔z��
	Coords_v<double> bezierPoints;

	// �ŏ��̓_���i�[
	bezierPoints.push_back(arc[START]);

	// �~�ʂ��ׂ�ی������ɒH��A�ی����ƂɃx�W�G�Ȑ��ŕ`��
	for (int quad = quadrant[START]; quad <= quadrant[END]; quad++) {
		// �r���̗v�f�Ȃ�A�~�ʂ̍ő�ŏ��l�����Ƃɗv�f�̏I�_������

		// ���̏ی����̃x�W�G�Ȑ��̊J�n�_
		Coord<double> here = bezierPoints.back();

		// ���̏ی����̃x�W�G�Ȑ��̏I�[�_
		Coord<double> next;

		// �Ō�̏ی��̏ꍇ�A�I�[�_ = �~�ʂ̏I�_
		if (quad == quadrant[END]) {
			// 
			next = arc[END];
		}
		// �r���̏ی��̏ꍇ�A�I�[�_ = ���̏ی��Ƃ̊Ԃ̎���̓_
		else {
			switch (quad % 4) {
				// ��1�ی�
			case 1:
				next.x = arc[CENTER].x;
				next.y = arc[CENTER].y + radius;
				break;
				// ��2�ی�
			case 2:
				next.x = arc[CENTER].x - radius;
				next.y = arc[CENTER].y;
				break;
				// ��3�ی�
			case 3:
				next.x = arc[CENTER].x;
				next.y = arc[CENTER].y - radius;
				break;
				// ��4�ی�
			case 0:
				next.x = arc[CENTER].x + radius;
				next.y = arc[CENTER].y;
				break;
			}
		}

		// ���S�_���x�W�G�Ȑ��̊J�n�_�̒P�ʃx�N�g��
		Coord<double> vecHere;
		vecHere = (here - arc[CENTER]) / radius;
		// ���S�_���x�W�G�Ȑ��̏I�[�_�̒P�ʃx�N�g��
		Coord<double> vecNext;
		vecNext = (next - arc[CENTER]) / radius;

		// �x�N�g���̓��ς���ی����̉~�ʂ̊p�x���Z�o(90�x�ȉ�)
		double inner = vecHere.x * vecNext.x + vecHere.y * vecNext.y;
		double angle = acos(inner);

		// ����_�̒����̌W�����Z�o
		//   ���a * (4 / 3) * tan(�p�x / 4)
		double coef = radius * (4.0 / 3.0) * tan(angle / 4.0);

		// �J�n�_�̒P�ʃx�N�g����90����]
		std::swap(vecHere.x, vecHere.y);
		vecHere.x *= -1;
		// �x�W�G�Ȑ��̊J�n�_���̐���_���Z�o
		Coord<double> ctrl1;
		ctrl1 = vecHere * coef + here;

		// �I�[�_�̒P�ʃx�N�g����-90����]
		std::swap(vecNext.x, vecNext.y);
		vecNext.y *= -1;
		// �x�W�G�Ȑ��̏I�[�_���̐���_���Z�o
		Coord<double> ctrl2;
		ctrl2 = vecNext * coef + next;

		// �x�W�G�Ȑ��̓_��ǉ�
		bezierPoints.push_back(ctrl1);
		bezierPoints.push_back(ctrl2);
		bezierPoints.push_back(next);
	}

	return bezierPoints;
}

// �x�W�G�Ȑ��ɂ��~�ʕ`��
// �n�_�ƏI�_����v����~�ʂŌĂяo�����ꍇ�̓���͖���`�i���̂悤�ȃP�[�X�͎��O�ɉ~�ƌ��Ȃ��ׂ��j
void Canvas::DrawBezierArc(Coords<double, 3> arc, ArcDirectionType direction) const
{
	// �x�W�G�Ȑ��̍��W�l�z����擾
	Coords_v<double> bezierPoints = CalcBezierArc(arc, direction);

	// �x�W�G�Ȑ��̓_���R���g���[�����W�ɕϊ�
	std::vector<POINT> drawPoints;
	for (const auto& pb : bezierPoints) {
		auto pc = CanvasToControl(pb);
		drawPoints.push_back(POINT{ pc.x, pc.y });
	}

	// �`��
	GetDC()->PolyBezier(drawPoints.data(), drawPoints.size());
}

// �`����e���t�@�C���ۑ�(BMP/PNG/JPEG/GIF)
bool Canvas::SaveImage(const std::tstring& filePath) const
{
	// DC����摜�I�u�W�F�N�g���擾���ăA�^�b�`
	ATL::CImage image;
	image.Attach(*(GetDC()->GetCurrentBitmap()));

	// �ۑ�����
	// ��2�����ȗ��Ŋg���q�ɏ]�����t�H�[�}�b�g�ƂȂ�(BMP/PNG/JPEG/GIF)
	HRESULT hr = image.Save(filePath.c_str());

	// �摜�I�u�W�F�N�g���f�^�b�`
	image.Detach();

	return SUCCEEDED(hr);
}

// �`����e���N���b�v�{�[�h�փR�s�[
bool Canvas::CopyImage(CWnd* pOwner) const
{
	// DC�̉摜�I�u�W�F�N�g���擾
	CBitmap* pBitmapMain = GetDC()->GetCurrentBitmap();

	// BITMAP�����擾
	BITMAP bitmapInfo;
	if (!pBitmapMain->GetBitmap(&bitmapInfo)) return false;

	// �r�b�g�}�b�v�̃r�b�g�f�[�^�擾
	std::size_t bitmapBytes = bitmapInfo.bmWidthBytes * bitmapInfo.bmHeight;
	std::vector<BYTE> bmpRaw(bitmapBytes);
	if (!pBitmapMain->GetBitmapBits(bitmapBytes, bmpRaw.data())) return false;

	// �r�b�g�}�b�v�𕡐�����
	// �������o�ϐ���HBITMAP�n���h�������̂܂܃N���b�v�{�[�h�ɐݒ肵�Ă��܂��ƁA
	// �@DC�ɑ΂���̕ύX���������f����Ă��܂�����
	CBitmap bitmapSub;
	bitmapSub.CreateBitmapIndirect(&bitmapInfo);
	bitmapSub.SetBitmapBits(bitmapBytes, bmpRaw.data());

	// �N���b�v�{�[�h���J��
	if (!pOwner->OpenClipboard()) return false;

	bool ret = true;

	// �N���b�v�{�[�h���N���A
	if (::EmptyClipboard()) {
		// �L�����o�X���r�b�g�}�b�v�Ƃ��ē]������
		if (!::SetClipboardData(CF_BITMAP, static_cast<HBITMAP>(bitmapSub))) {
			ret = false;
		}
	}

	// �N���b�v�{�[�h�����
	::CloseClipboard();

	return ret;
}


// �R���X�g���N�^
//   ��Manager�N���X�̃����o���g�p���邽�߃w�b�_�t�@�C���ɒ�`�s��
Node::Node(Manager* pManager) :
	m_canvas(pManager->GetCanvas()),
	m_pen(pManager->GetCurrentPen()),
	m_brush(pManager->GetCurrentBrush())
{
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeGrid::CalcBoundingBox() const
{
	// �O���b�h�̏ꍇ�͍ŏ���ܔ��͖���
	return BoundingBox<double>();
}

// �`�󂪕`��̈�Ɋ܂܂�邩�`�F�b�N
bool NodeGrid::IsIncludeCanvas() const
{
	// �O���b�h�͏�Ɋ܂܂����
	return true;
}

// �`���`��
void NodeGrid::DrawContent()
{
	// �O���b�h�`��
	m_canvas.DrawGrid();
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeOrigin::CalcBoundingBox() const
{
	// ���_�̃T�C�Y
	double size = m_canvas.ControlToCanvas(m_canvas.GetOriginSize());

	// ���_�̋�`���v�Z
	BoundingBox<double> bbox;
	bbox.min.x = m_point.x - size;
	bbox.min.y = m_point.y - size;
	bbox.max.x = m_point.x + size;
	bbox.max.y = m_point.y + size;
	return bbox;
}

// �`���`��
void NodeOrigin::DrawContent()
{
	// ���_�`��
	m_canvas.DrawOrigin(m_point);
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeAxis::CalcBoundingBox() const
{
	// ���̏ꍇ�͍ŏ���ܔ������_�Ƃ���
	return BoundingBox<double>(m_point);
}

// �`���`��
void NodeAxis::DrawContent()
{
	// ���`��
	m_canvas.DrawAxis(m_point);
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodePoint::CalcBoundingBox() const
{
	// �ŏ���ܔ���_�Ƃ���
	return BoundingBox<double>(m_point);
}

// �`���`��
void NodePoint::DrawContent()
{
	// �_�̎�ޕʂɕ`��
	switch (m_pointType) {
	case PointType::Pixel:
		// �s�N�Z��
		m_canvas.DrawPixel(m_point);
		break;
	case PointType::Triangle:
		// �O�p�`
		m_canvas.DrawTrianglePoint(m_point);
		break;
	case PointType::Large:
		// ����
		m_canvas.DrawLargePoint(m_point);
		break;
	}
}


// �������擾
Coords<double, 2> NodeLine::Segment() const
{
	Coords<double, 2> points;

	// �L���̐���
	if (m_lineLimitType == LineLimitType::Finite) {
		points = m_points;
	}
	// ��������
	else {
		// �`��̈���擾
		BoundingBox<double> canvas = m_canvas.GetCanvasArea();

		// ������
		if (m_infiniteInfo.isVertical) {
			// x�ؕЂƕ`��̈�̏㉺Y���W�͈̔͂Ő������쐬
			points[0].x = m_infiniteInfo.interceptX;
			points[0].y = canvas.min.y;
			points[1].x = m_infiniteInfo.interceptX;
			points[1].y = canvas.max.y;
		}
		// �񉔒���
		else {
			// �`��̈�̍��EX���W�͈̔͂Ő������쐬 (y = c * x + i)
			points[0].x = canvas.min.x;
			points[0].y = m_infiniteInfo.coefficientX * canvas.min.x + m_infiniteInfo.interceptY;
			points[1].x = canvas.max.x;
			points[1].y = m_infiniteInfo.coefficientX * canvas.max.x + m_infiniteInfo.interceptY;
		}
	}
	return points;
}

// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeLine::CalcBoundingBox() const
{
	// TODO:
	// ������P���ɍŏ���ܔ��Ƃ��č̗p���Ă��邪
	// ���ۂɂ͕`��̈�Ɋ܂܂�Ȃ��̂ɍŏ���ܔ����d�Ȃ��Ă��܂��ꍇ������

	// �`��p�̐������擾
	Coords<double, 2> points = Segment();
	// �`����\��������W����ŏ���ܔ����Z�o
	BoundingBox<double> bbox;
	for (const auto& p : points) {
		bbox += p;
	}
	return bbox;
}

// �`���`��
void NodeLine::DrawContent()
{
	// �`��p�̐������擾
	auto points = Segment();
	// �R���g���[�����W�ɕϊ�
	auto ctrlPoints = m_canvas.CanvasToControl(Coords_v<double>(points.begin(), points.end()));
	// �`��
	m_canvas.GetDC()->MoveTo(ctrlPoints[0].x, ctrlPoints[0].y);
	m_canvas.GetDC()->LineTo(ctrlPoints[1].x, ctrlPoints[1].y);

	// �����̏ꍇ�A��[�̖��`��
	if (m_lineLimitType == LineLimitType::Finite && m_canvas.GetIsDrawArrow()) {
		// ����`��
		m_canvas.DrawArrowHead(points);
	}
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeArc::CalcBoundingBox() const
{
	// �x�W�G�Ȑ��ɂ����W�l�z����擾
	Coords_v<double> bezierPoints = m_canvas.CalcBezierArc(m_points, m_arcDirectionType);

	// ���W�l�z��� 3n+1 �ō\������Ă���
	// �~�ʂ��ی����������Ƃ��̎n�_/�I�_�̊Ԃɐ���_��2���܂��Ă�����

	// ����_���������~�ʂ̎n�_�ƏI�_�݂̂���ŏ���ܔ����Z�o
	BoundingBox<double> bbox;
	for (auto itr = bezierPoints.cbegin(); itr != bezierPoints.cend(); itr += 3) {
		bbox += *itr;
	}
	return bbox;
}

// �`���`��
void NodeArc::DrawContent()
{
	// �x�W�G�Ȑ��ŉ~�ʂ�`��
	m_canvas.DrawBezierArc(m_points, m_arcDirectionType);

	// �~�ʂ̐�[�̖��`��
	if (m_canvas.GetIsDrawArrow()) {
		// ����`�����߂̎�
		Coords<double, 2> arrowAxis;

		// ��]�p�x������
		//   �~�ʕ��������F+90�x
		//   �~�ʕ������E�F-90�x
		double angle = m_arcDirectionType == ArcDirectionType::Left ? PI : -PI;

		// �I�_����ɁA���S�_����Ō��߂���]�p�x������]����
		// ����`�����߂̎��̎n�_���Z�o
		arrowAxis[START] = m_points[END].Rotate(m_points[CENTER], angle);
		// ����`�����߂̎��̏I�_�͉~�ʂ̏I�_
		arrowAxis[END] = m_points[END];

		// ����`��
		m_canvas.DrawArrowHead(arrowAxis);
	}

	// �~�ʂ̒��S�_��`��
	if (m_canvas.GetIsDrawCenter()) {
		m_canvas.DrawTrianglePoint(m_points[CENTER]);
	}
}

// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeCircle::CalcBoundingBox() const
{
	// �~�̋�`���v�Z
	BoundingBox<double> bbox;
	bbox.min.x = m_point.x - m_radius;
	bbox.min.y = m_point.y - m_radius;
	bbox.max.x = m_point.x + m_radius;
	bbox.max.y = m_point.y + m_radius;
	return bbox;
}
// �`���`��
void NodeCircle::DrawContent()
{
	// �h��Ԃ��Ȃ�
	if (m_fillType == FillType::NoFill) {
		// 0�x����180�x�̉~��
		Coords<double, 3> arc = { m_point, m_point, m_point };
		arc[START].x += m_radius;
		arc[END].x -= m_radius;
		// ���~��`��
		m_canvas.DrawBezierArc(arc, ArcDirectionType::Left);

		// 180�x����360�x�̉~��
		std::swap(arc[START], arc[END]);
		// ���~��`��
		m_canvas.DrawBezierArc(arc, ArcDirectionType::Left);

		// ���S�_��`��
		if (m_canvas.GetIsDrawCenter()) {
			// �O�p�`�̓_��`��
			m_canvas.DrawTrianglePoint(m_point);
		}
	}
	// �h��Ԃ�����
	else {
		// �~�̊O�ڂ����`�̍���ƉE���̓_���Z�o
		Coord<long> ctrlLeftUp = m_canvas.CanvasToControl(Coord<double>(m_point.x - m_radius, m_point.y + m_radius));
		Coord<long> ctrlRightDown = m_canvas.CanvasToControl(Coord<double>(m_point.x + m_radius, m_point.y - m_radius));
		// �~��`��
		m_canvas.GetDC()->Ellipse(ctrlLeftUp.x, ctrlLeftUp.y, ctrlRightDown.x, ctrlRightDown.y);
	}
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodePolygon::CalcBoundingBox() const
{
	// �`����\��������W����ŏ���ܔ����Z�o
	BoundingBox<double> bbox;
	for (const auto& p : m_points) {
		bbox += p;
	}
	return bbox;
}
// �`���`��
void NodePolygon::DrawContent()
{
	// �h��Ԃ��Ȃ�
	if (m_fillType == FillType::NoFill) {
		// �`��̍��W�l���R���g���[�����W�ɕϊ�
		Coords_v<long> ctrlPoints = m_canvas.CanvasToControl(m_points);

		// �Ō�̓_�Ɉړ����Ă���
		m_canvas.GetDC()->MoveTo(ctrlPoints.rbegin()->x, ctrlPoints.rbegin()->y);
		// ���p�`��`��
		for (const auto& here : ctrlPoints) {
			m_canvas.GetDC()->LineTo(here.x, here.y);
		}

		// ����`��
		if (m_canvas.GetIsDrawArrow()) {
			// �Ō�̓_����̎��̎n�_�Ƃ���
			Coord<double> s = *(m_points.rbegin());
			// ���̎��̏I�_�Ƃ��č��W�����Ɏ擾
			for (const auto& e : m_points) {
				// ����`��
				m_canvas.DrawArrowHead(Coords<double, 2>{s, e});
				// ���̎n�_���X�V
				s = e;
			}
		}
	}
	// �h��Ԃ�����
	else {
		// ���W��POINT�^�̔z��֕ϊ�
		std::vector<POINT> ctrlPoints;
		for (const auto& p : m_points) {
			auto cp = m_canvas.CanvasToControl(p);
			ctrlPoints.push_back(POINT{ cp.x, cp.y });
		}
		// ���p�`��`��
		m_canvas.GetDC()->Polygon(ctrlPoints.data(), ctrlPoints.size());
	}
}


// �����̉~�ʍ��W���Z�o
Coords<double, 3> NodeSector::CalcInnerArc() const
{
	// ���S�_����n�_�ƏI�_�ւ̃x�N�g�����Z�o
	Coord<double> vectorCS = m_points[START] - m_points[CENTER];
	Coord<double> vectorCE = m_points[END] - m_points[CENTER];

	// �O���̉~�ʔ��a���Z�o
	double outerRadius = m_points[CENTER].Length(m_points[START]);

	// �����̉~�ʍ��W���Z�o�i���S�_����̃x�N�g��������~�ʂ̒����ɕϊ������̍��W�֖߂��j
	Coords<double, 3> innerArc;
	innerArc[START] = vectorCS / outerRadius * m_innerRadius + m_points[CENTER];
	innerArc[END] = vectorCE / outerRadius * m_innerRadius + m_points[CENTER];
	innerArc[CENTER] = m_points[CENTER];

	return innerArc;
}

// ��`�̃��[�W�����i�R���g���[�����W�j���Z�o
void NodeSector::CalcSectorRgn(CRgn* sectorRgn) const
{
	// �O���̉~�ʁi�R���g���[�����W�j
	Coords_v<long> ctrlOuterArc = m_canvas.CanvasToControl(Coords_v<double>(m_points.begin(), m_points.end()));
	// �������̉~�ʂɂ��Ă���
	if (m_arcDirectionType == ArcDirectionType::Right) {
		std::swap(ctrlOuterArc[START], ctrlOuterArc[END]);
	}

	// DC�𗘗p���ĊO���̐�`�̃p�X���쐬
	m_canvas.GetDC()->AbortPath();
	// �p�X���J�n
	m_canvas.GetDC()->BeginPath();
	// �~�ʕ����̃p�X���~�ʏI�_���璆�S�_�ւ̃p�X�����S�_����~�ʎn�_�ւ̃p�X
	m_canvas.DrawBezierArc(m_points, m_arcDirectionType);
	m_canvas.GetDC()->MoveTo(ctrlOuterArc[END].x, ctrlOuterArc[END].y);
	m_canvas.GetDC()->LineTo(ctrlOuterArc[CENTER].x, ctrlOuterArc[CENTER].y);
	m_canvas.GetDC()->LineTo(ctrlOuterArc[START].x, ctrlOuterArc[START].y);
	// �p�X���I��
	m_canvas.GetDC()->EndPath();

	// �O���̐�`���[�W�������擾
	CRgn outerPieRgn;
	outerPieRgn.CreateFromPath(m_canvas.GetDC());

	// �����~�ʂ̔��a���R���g���[�����W�ɕϊ�
	long ctrlInnerRadius = static_cast<long>(m_canvas.CanvasToControl(m_innerRadius));

	// �����̉~�`���[�W�������擾
	CRgn innerCircleRgn;
	innerCircleRgn.CreateEllipticRgn(
		ctrlOuterArc[CENTER].x - ctrlInnerRadius,
		ctrlOuterArc[CENTER].y - ctrlInnerRadius,
		ctrlOuterArc[CENTER].x + ctrlInnerRadius,
		ctrlOuterArc[CENTER].y + ctrlInnerRadius
	);

	// �O���̐�`���[�W���� - �����̉~�`���[�W����
	sectorRgn->CombineRgn(&outerPieRgn, &innerCircleRgn, RGN_DIFF);
}

// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeSector::CalcBoundingBox() const
{
	// ��`�̃��[�W��������ŏ���ܔ��i�R���g���[�����W�j���擾
	CRgn sectorRgn;
	CalcSectorRgn(&sectorRgn);
	CRect ctrlRect;
	sectorRgn.GetRgnBox(&ctrlRect);

	// �L�����o�X���W�n�ɕϊ�
	BoundingBox<double> bbox;
	bbox.min = m_canvas.ControlToCanvas(Coord<long>(ctrlRect.left, ctrlRect.bottom));
	bbox.max = m_canvas.ControlToCanvas(Coord<long>(ctrlRect.right, ctrlRect.top));
	return bbox;
}
// �`���`��
void NodeSector::DrawContent()
{
	// �h��Ԃ�����
	if (m_fillType == FillType::Fill) {
		// ��`���[�W�������擾���ēh��Ԃ�
		CRgn sectorRgn;
		CalcSectorRgn(&sectorRgn);
		m_canvas.GetDC()->FillRgn(&sectorRgn, m_canvas.GetDC()->GetCurrentBrush());
	}

	// �O���Ɠ����̉~��
	Coords<double, 3> outerArc = m_points;
	Coords<double, 3> innerArc = CalcInnerArc();

	// �~�ʕ�����`��
	m_canvas.DrawBezierArc(outerArc, m_arcDirectionType);
	m_canvas.DrawBezierArc(innerArc, m_arcDirectionType);

	// �R���g���[�����W�ɕϊ����Ē���������`��
	Coords_v<long> ctrlOuterArc = m_canvas.CanvasToControl(Coords_v<double>(outerArc.begin(), outerArc.end()));
	Coords_v<long> ctrlInnerArc = m_canvas.CanvasToControl(Coords_v<double>(innerArc.begin(), innerArc.end()));
	m_canvas.GetDC()->MoveTo(ctrlOuterArc[START].x, ctrlOuterArc[START].y);
	m_canvas.GetDC()->LineTo(ctrlOuterArc[END].x, ctrlOuterArc[END].y);
	m_canvas.GetDC()->MoveTo(ctrlInnerArc[START].x, ctrlInnerArc[START].y);
	m_canvas.GetDC()->LineTo(ctrlInnerArc[END].x, ctrlInnerArc[END].y);
}


// �R���X�g���N�^
Layer::Layer() :
	m_enableDraw(true)
{
}

// ������
void Layer::Clear()
{
	// �m�[�h�R���N�V�������N���A
	m_nodes.clear();
}

// �S�`��̍ŏ���ܔ����Z�o
BoundingBox<double> Layer::CalcBoundingBox() const
{
	BoundingBox<double> bbox;
	// �`��t���OON�Ȃ�
	if (m_enableDraw) {
		// �S�m�[�h�̍ŏ���ܔ�������
		for (const auto& pNode : m_nodes) {
			bbox += pNode->CalcBoundingBox();
		}
	}
	return bbox;
}

// �`��
void Layer::Draw()
{
	// �`��t���OON�Ȃ�
	if (m_enableDraw) {
		// �S�m�[�h��`��
		for (const auto& pNode : m_nodes) {
			pNode->Draw();
		}
	}
}


// �R���X�g���N�^
Manager::Manager() :
	m_canvas(),
	m_baseLayer(),
	m_currentLayerNo(false)
{
	// ������
	Clear();
}

// �S�`��̍ŏ���ܔ����Z�o
BoundingBox<double> Manager::CalcBoundingBox() const
{
	// �x�[�X���C���[�̓��e�͊܂߂Ȃ�

	BoundingBox<double> bbox;
	// �S���C���[�̍ŏ���ܔ�������
	for (const auto& pLayer : m_layers) {
		bbox += pLayer->CalcBoundingBox();
	}
	return bbox;
}

// ���C���[��}��
bool Manager::InsertLayer(std::size_t insertNo)
{
	// ���C���[No�G���[
	if (m_layers.size() <= insertNo) return false;

	// �w��ʒu�ɐV�������C���[��}��
	m_layers.insert(m_layers.begin() + insertNo, std::make_unique<Layer>());
	// �}���������C���[���J�����g�Ƃ���
	m_currentLayerNo = insertNo;

	return true;
}

// �J�����g���C���[���N���A
std::size_t Manager::DeleteCurrentLayer()
{
	// �Ō�̃��C���[�̏ꍇ
	if (m_layers.size() <= 1) {
		m_currentLayerNo = 0;
		m_layers[m_currentLayerNo]->Clear();
	}
	// ���C���[������������ꍇ
	else {
		// �J�����g���C���[���폜
		m_layers.erase(m_layers.begin() + m_currentLayerNo);
		// �ŏI���C���[���폜�����ꍇ�̃J�����g����
		if (m_layers.size() <= m_currentLayerNo) m_currentLayerNo = m_layers.size() - 1;
	}
	// ���C���[������Ԃ�
	return m_layers.size();
}

// ������
void Manager::Clear()
{
	// �y��������
	m_currentPen = DEFAULT_PEN;
	// �u���V������
	m_currentBrush = DEFAULT_BRUSH;

	// �x�[�X���C���[��������
	m_baseLayer.Clear();
	// �O���b�h��o�^
	m_baseLayer.AddNode(new NodeGrid(this));
	// ����o�^
	m_baseLayer.AddNode(new NodeOrigin(this, Coord<double>(0.0, 0.0)));
	// ���_��o�^
	m_baseLayer.AddNode(new NodeAxis(this, Coord<double>(0.0, 0.0)));

	// ���C���[�R���N�V�������N���A
	m_layers.clear();
	// �J�����g���C���[��ǉ�
	m_layers.push_back(std::make_unique<Layer>());
	m_currentLayerNo = 0;
}

// �`��
void Manager::Draw(bool isDesignMode/*=false*/)
{
	// �f�U�C�����[�h�p�̕`��
	if (isDesignMode) {
		// �g��k�����̓R���X�g���N�g���̏����l�Ƃ���
		m_canvas.SetRatio(Canvas::DEFAULT_RATIO);
		// �I�t�Z�b�g�̓R���g���[���̋�`�̒����Ƃ���
		Coord<double> offset{ (double)m_canvas.GetRect()->CenterPoint().x, (double)m_canvas.GetRect()->CenterPoint().y };
		m_canvas.SetOffset(offset);

		// �w�i�F�œh��Ԃ�
		m_canvas.FillBackground();

		LOGPEN logPen = DEFAULT_PEN;
		LOGBRUSH logBrush = DEFAULT_BRUSH;

		// �O���b�h�`��
		{
			// �y���ƃu���V��ύX�i�f�t�H���g����F�̂ݕύX�j
			logPen.lopnColor = m_canvas.GetGridColor();
			logBrush.lbColor = m_canvas.GetGridColor();
			PenBrushChanger pc(m_canvas.GetDC(), logPen);
			PenBrushChanger bc(m_canvas.GetDC(), logBrush);
			m_canvas.DrawGrid();
		}
		// ���`��
		{
			// �y���ƃu���V��ύX�i�f�t�H���g����F�̂ݕύX�j
			logPen.lopnColor = m_canvas.GetAxisColor();
			logBrush.lbColor = m_canvas.GetAxisColor();
			PenBrushChanger pc(m_canvas.GetDC(), logPen);
			PenBrushChanger bc(m_canvas.GetDC(), logBrush);
			m_canvas.DrawAxis(Coord<double>(0.0, 0.0));
		}
		// ���_�`��
		{
			// �y���ƃu���V��ύX�i�f�t�H���g����F�̂ݕύX�j
			logPen.lopnColor = m_canvas.GetOriginColor();
			logBrush.lbColor = m_canvas.GetOriginColor();
			PenBrushChanger pc(m_canvas.GetDC(), logPen);
			PenBrushChanger bc(m_canvas.GetDC(), logBrush);
			m_canvas.DrawOrigin(Coord<double>(0.0, 0.0));
		}
	}
	// �ʏ�̕`��
	else {
		// �w�i��h��Ԃ�
		m_canvas.FillBackground();

		// �x�[�X���C���[��`��
		m_baseLayer.Draw();

		// �S���C���[��`��
		for (const auto& pLayer : m_layers) {
			pLayer->Draw();
		}
	}
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
	offset.x = base.x - ((base.x - offset.x) * ratio);
	offset.y = base.y - ((base.y - offset.y) * ratio);
	m_canvas.SetOffset(offset);

	// �ĕ`��
	Draw();

	return true;
}

// �p��
bool Manager::Pan(const Coord<long>& move)
{
	// ���݂̊g��k����
	double ratio = m_canvas.GetRatio();
	// ���݂̃I�t�Z�b�g
	Coord<double> offset = m_canvas.GetOffset();

	// ����ȏ�I�t�Z�b�g�ł��Ȃ�
	if (OFFSET_MAX < fabs(m_canvas.ControlToCanvas(offset.x + move.x))) return false;
	if (OFFSET_MAX < fabs(m_canvas.ControlToCanvas(offset.y + move.y))) return false;

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
	// �`�悷��S�`��̍ŏ���ܔ����擾
	BoundingBox<double> bbox = CalcBoundingBox();

	// �g��k����
	double ratio;
	// �I�t�Z�b�g
	Coord<double> offset;

	// �ŏ���ܔ��擾���s
	if (!bbox.Verify()) {
		// �����l�Ƃ���
		ratio = Canvas::DEFAULT_RATIO;
		offset = Coord<double>(
			static_cast<double>(m_canvas.GetRect()->CenterPoint().x),
			static_cast<double>(m_canvas.GetRect()->CenterPoint().y)
		);
	}
	else {
		// �`��̏c������Z�o
		double shapeAspect = std::numeric_limits<double>::max();
		if (bbox.GetWidth() != 0.0) {
			shapeAspect = bbox.GetHeight() / bbox.GetWidth();
		}

		// �`��̈�̏c����
		double cnvsAspect = std::numeric_limits<double>::max();
		if (m_canvas.GetRect()->Width() != 0.0) {
			cnvsAspect = static_cast<double>(m_canvas.GetRect()->Height()) / m_canvas.GetRect()->Width();
		}

		// �g��k�������Z�o
		// �`��̈�ɑ΂��Č`�󂪏c���Ȃ�c��������Ƃ���
		if (bbox.GetWidth() == 0.0 || shapeAspect > cnvsAspect) {
			// (�`��̈�T�C�Y / �`��T�C�Y) * �`��̐�L��
			ratio = (m_canvas.GetRect()->Height() / bbox.GetHeight()) * shapeOccupancy;
		}
		// �`��̈�ɑ΂��Č`�󂪉����Ȃ牡��������Ƃ���
		else {
			// (�`��̈�T�C�Y / �`��T�C�Y) * �`��̐�L��
			ratio = (m_canvas.GetRect()->Width() / bbox.GetWidth()) * shapeOccupancy;
		}

		// �I�t�Z�b�g���Z�o
		offset.x = (m_canvas.GetRect()->Width() - bbox.GetWidth()) * ratio / 2.0 - bbox.min.x * ratio;
		double y = (m_canvas.GetRect()->Height() - bbox.GetHeight()) * ratio / 2.0 - bbox.min.y * ratio;
		offset.y = m_canvas.GetRect()->Height() - y;	// Y�����]
	}

	// �g��k�����ƃI�t�Z�b�g���X�V
	m_canvas.SetRatio(ratio);
	m_canvas.SetOffset(offset);

	// �ĕ`��
	Draw();
}


}	// namespace Drawer
