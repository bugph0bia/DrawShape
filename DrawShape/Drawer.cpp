#include "pch.h"
#include <atlimage.h>
#include "Drawer.h"

namespace Drawer
{

// �R���X�g���N�^
Canvas::Canvas(CDC* pDC, const CRect& rect) :
	m_pDC(pDC),
	m_rect(rect),
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
	if (m_isDrawGrid) return;
	// �O���b�h���ׂ���������`����s��Ȃ�
	if ((m_gridSize * m_ratio) < DRAW_GRID_SIZE_MIN) return;

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
	if (m_isDrawOrigin) return;

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
	if (m_isDrawAxis) return;

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
	if ((m_axisScale * m_ratio) < DRAW_AXIS_SCALE_MIN) return;

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

// �x�W�G�Ȑ��ɂ��~�ʕ`��
// �n�_�ƏI�_����v����~�ʂŌĂяo�����ꍇ�̓���͖���`�i���̂悤�ȃP�[�X�͎��O�ɉ~�ƌ��Ȃ��ׂ��j
void Canvas::DrawBezierArc(Coords<double, 3> arc, ArcDirectionType direction) const
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
	std::vector<Coord<double>> bezierPoints;

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

	// �x�W�G�Ȑ��̓_���R���g���[�����W�ɕϊ�
	std::vector<POINT> drawPoints;
	for (auto& pb : bezierPoints) {
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

// �`��
void NodeGrid::Draw()
{
	// �O���b�h�`��
	m_canvas.DrawGrid();
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeOrigin::CalcBoundingBox() const
{
	// ���_�̃T�C�Y
	// �L�����o�X���W�n�ł͑��ΓI�Ɋg��k�����t�]���ꂽ�傫���ƂȂ�
	double size = m_canvas.GetOriginSize() / m_canvas.GetRatio();

	// ���_�̋�`���v�Z
	BoundingBox<double> bbox;
	bbox.min.x = m_point.x - size;
	bbox.min.y = m_point.y - size;
	bbox.max.x = m_point.x + size;
	bbox.max.y = m_point.y + size;
	return bbox;
}

// �`��
void NodeOrigin::Draw()
{
	// �`��̈�Ɋ܂܂�Ȃ���Ε`�悵�Ȃ�
	if (!IsIncludeCanvas()) return;
	// ���_�`��
	m_canvas.DrawOrigin(m_point);
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeAxis::CalcBoundingBox() const
{
	// ���̏ꍇ�͍ŏ���ܔ������_�Ƃ���
	BoundingBox<double> bbox;
	bbox.min = m_point;
	bbox.max = m_point;
	return bbox;
}

// �`��
void NodeAxis::Draw()
{
	// �`��̈�Ɋ܂܂�Ȃ���Ε`�悵�Ȃ�
	if (!IsIncludeCanvas()) return;
	// ���`��
	m_canvas.DrawAxis(m_point);
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodePoint::CalcBoundingBox() const
{
	BoundingBox<double> bbox;
	bbox.min = m_point;
	bbox.max = m_point;
	return bbox;
}

// �`��
void NodePoint::Draw()
{
	// �`��̈�Ɋ܂܂�Ȃ���Ε`�悵�Ȃ�
	if (!IsIncludeCanvas()) return;

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
	// �`��p�̐������擾
	Coords<double, 2> points = Segment();
	// �`����\��������W����ŏ���ܔ����Z�o
	BoundingBox<double> bbox;
	for (auto& p : points) {
		bbox += p;
	}
	return bbox;
}

// �`��
void NodeLine::Draw()
{
	// �`��̈�Ɋ܂܂�Ȃ���Ε`�悵�Ȃ�
	if (!IsIncludeCanvas()) return;

	// �`��p�̐������擾
	Coords<double, 2> points = Segment();
	// �R���g���[�����W�ɕϊ�
	auto ctrlPoints = m_canvas.CanvasToControl(points);
	// �`��
	m_canvas.GetDC()->MoveTo(ctrlPoints[0].x, ctrlPoints[0].y);
	m_canvas.GetDC()->LineTo(ctrlPoints[1].x, ctrlPoints[1].y);
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeArc::CalcBoundingBox() const
{
	// �`����\��������W����ŏ���ܔ����Z�o
	BoundingBox<double> bbox;
	for (auto& p : m_points) {
		bbox += p;
	}
	return bbox;
}

// �`��
void NodeArc::Draw()
{
	// �`��̈�Ɋ܂܂�Ȃ���Ε`�悵�Ȃ�
	if (!IsIncludeCanvas()) return;

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
	BoundingBox<double> bbox;
	// TODO:
	return bbox;
}
// �`��
void NodeCircle::Draw()
{
	// �`��̈�Ɋ܂܂�Ȃ���Ε`�悵�Ȃ�
	if (!IsIncludeCanvas()) return;
	// TODO:
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodePolygon::CalcBoundingBox() const
{
	BoundingBox<double> bbox;
	// TODO:
	return bbox;
}
// �`��
void NodePolygon::Draw()
{
	// �`��̈�Ɋ܂܂�Ȃ���Ε`�悵�Ȃ�
	if (!IsIncludeCanvas()) return;
	// TODO:
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeSector::CalcBoundingBox() const
{
	BoundingBox<double> bbox;
	// TODO:
	return bbox;
}
// �`��
void NodeSector::Draw()
{
	// �`��̈�Ɋ܂܂�Ȃ���Ε`�悵�Ȃ�
	if (!IsIncludeCanvas()) return;
	// TODO:
}


// �R���X�g���N�^
Layer::Layer() :
	m_isDraw(false)
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
Manager::Manager(CDC* pDC, const CRect& rect) :
	m_canvas(pDC, rect),
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
	for (auto& pLayer : m_layers) {
		bbox += pLayer->CalcBoundingBox();
	}
	return bbox;
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
		// ��������g�p����ꍇ�͈ȉ��𐄏�
		//   �E�{�N���X�̃I�u�W�F�N�g�͈ꎞ�I�ȕϐ��Ƃ��A�R���X�g���N�^�� Manager(�f�U�C�����[�h�p��DC, Rect); �ŌĂяo���Ă���
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
		for (auto& pLayer : m_layers) {
			pLayer->Draw();
		}

		/*
		TODO: ����̓R���g���[�����ɍs�킹��\��
			// �`��C�x���g���s
			m_pCtrl->InvalidateRect(nullptr, FALSE);	// �̈斳����
			m_pCtrl->UpdateWindow();					// �ĕ`�施��
		*/
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
	offset.x = base.x - ((base.x - offset.x) * coef);
	offset.y = base.y - ((base.y - offset.y) * coef);
	m_canvas.SetOffset(offset);

	// �ĕ`��
	Draw();

	return true;
}

/*
TODO: ����̓R���g���[�����ɍs�킹��\��

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
*/

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
