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
	m_offset(DEFAULT_OFFSET)
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
	canvas.min = ControlToCanvas(Coord<long>(0, GetRect()->Height()));
	canvas.max = ControlToCanvas(Coord<long>(GetRect()->Width(), 0));
	return canvas;
}

// �w�i��h��Ԃ�
void Canvas::FillBackground() const
{
	// �J�����g�u���V�̐F���擾
	LOGBRUSH brush;
	GetDC()->GetCurrentBrush()->GetLogBrush(&brush);
	COLORREF color = brush.lbColor;

	// �S�̂�h��Ԃ�
	m_pDC->FillSolidRect(GetRect(), color);
}

// �O���b�h�`��
void Canvas::DrawGrid(double size) const
{
	// �O���b�h���ׂ���������`����s��Ȃ�
	if ((CanvasToControl(size)) < DRAW_GRID_SIZE_MIN) return;

	// �L�����o�X�S�̂̍��W���擾
	BoundingBox<double>	canvas = GetCanvasArea();

	// �L�����o�X�Ɋ܂܂��O���b�h�̃C���f�b�N�X
	long xstart = static_cast<long>(canvas.min.x / size);
	long xend = static_cast<long>(canvas.max.x / size);
	long ystart = static_cast<long>(canvas.min.y / size);
	long yend = static_cast<long>(canvas.max.y / size);

	// �O���b�h�`��
	for (long y = ystart; y <= yend; y++) {
		for (long x = xstart; x <= xend; x++) {
			// �_��`��
			DrawPixel(Coord<double>(x * size, y * size));
		}
	}
}

// ���_�`��
void Canvas::DrawOrigin(const Coord<double>& base, long size) const
{
	// ���_�̃R���g���[�����W���Z�o
	Coord<long> ctrlBase = CanvasToControl(base);

	// �����̋�`��`��
	std::array<POINT, 4> points = {
		// ����, �E��, �E��, ���� (���Ő��̑�����2pixel�ɂ��邽�߁A1pixel����)
		POINT{ctrlBase.x - ORIGIN_CENTER_SIZE - 1, ctrlBase.y - ORIGIN_CENTER_SIZE},
		POINT{ctrlBase.x + ORIGIN_CENTER_SIZE, ctrlBase.y - ORIGIN_CENTER_SIZE},
		POINT{ctrlBase.x + ORIGIN_CENTER_SIZE, ctrlBase.y + ORIGIN_CENTER_SIZE + 1},
		POINT{ctrlBase.x - ORIGIN_CENTER_SIZE - 1, ctrlBase.y + ORIGIN_CENTER_SIZE + 1}
	};
	GetDC()->Polygon(points.data(), points.size());

	// X�������̖��(2pixel)
	GetDC()->MoveTo(ctrlBase.x, ctrlBase.y);
	GetDC()->LineTo(ctrlBase.x + size, ctrlBase.y);
	GetDC()->MoveTo(ctrlBase.x, ctrlBase.y + 1);
	GetDC()->LineTo(ctrlBase.x + size, ctrlBase.y + 1);
	// ����[��`��
	DrawArrowHead(
		Coords<double, 2>{
			base,
			ControlToCanvas(Coord<long>(ctrlBase.x + size, ctrlBase.y))
		},
		FillType::Fill
	);

	// Y�������̖��(2pixel)
	GetDC()->MoveTo(ctrlBase.x - 1, ctrlBase.y);
	GetDC()->LineTo(ctrlBase.x - 1, ctrlBase.y - size);
	GetDC()->MoveTo(ctrlBase.x, ctrlBase.y);
	GetDC()->LineTo(ctrlBase.x, ctrlBase.y - size);
	// ����[��`��
	DrawArrowHead(
		Coords<double, 2>{
			base,
			ControlToCanvas(Coord<long>(ctrlBase.x, ctrlBase.y - size))
		},
		FillType::Fill
	);
}

// ���`��
void Canvas::DrawAxis(const Coord<double>& base, double scale) const
{
	// �L�����o�X�S�̂̍��W���擾
	BoundingBox<double>	canvas = GetCanvasArea();

	// ���_�̃R���g���[�����W���Z�o
	Coord<long> origin = CanvasToControl(base);

	// X�����\�������ꍇ
	if (canvas.min.y < base.y && base.y < canvas.max.y) {

		// X����`��
		GetDC()->MoveTo(0, origin.y);
		GetDC()->LineTo(GetRect()->Width(), origin.y);
	}

	// Y�����\�������ꍇ
	if (canvas.min.x < base.x && base.x < canvas.max.x) {

		// ���`��
		GetDC()->MoveTo(origin.x, 0);
		GetDC()->LineTo(origin.x, GetRect()->Height());
	}

	// �ڐ����ׂ���������`����s��Ȃ�
	if (CanvasToControl(scale) < DRAW_AXIS_SCALE_MIN) return;

	// X�����\�������ꍇ
	if (canvas.min.y < base.y && base.y < canvas.max.y) {
		// �L�����o�X�Ɋ܂܂��ڐ��̃C���f�b�N�X
		long xstart = static_cast<long>(canvas.min.x / scale);
		long xend = static_cast<long>(canvas.max.x / scale);

		// �ڐ��`��
		for (long x = xstart; x <= xend; x++) {
			// �R���g���[�����W�ɕϊ�
			Coord<long> point = CanvasToControl(Coord<double>(x * scale, base.y));
			// �`��
			GetDC()->MoveTo(point.x, point.y - AXIS_SCALE_LENGTH);
			GetDC()->LineTo(point.x, point.y + AXIS_SCALE_LENGTH + 1);
		}
	}

	// Y�����\�������ꍇ
	if (canvas.min.x < base.x && base.x < canvas.max.x) {
		// �L�����o�X�Ɋ܂܂��ڐ��̃C���f�b�N�X
		long ystart = static_cast<long>(canvas.min.y / scale);
		long yend = static_cast<long>(canvas.max.y / scale);

		// �ڐ��`��
		for (long y = ystart; y <= yend; y++) {
			// �R���g���[�����W�ɕϊ�
			Coord<long> point = CanvasToControl(Coord<double>(base.x, y * scale));
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
void Canvas::DrawArrowHead(const Coords<double, 2>& baseSegment, FillType fillType/*=FillType::NoFill*/) const
{
	// ���݂̃y���������ɕύX
	LOGPEN logPen;
	GetDC()->GetCurrentPen()->GetLogPen(&logPen);
	logPen.lopnStyle = PS_SOLID;

	// �y����ύX
	PenBrushChanger pc(GetDC(), logPen);

	// �����̃R���g���[�����W���Z�o
	Coords_v<long> ctrlBaseSegment = CanvasToControl(Coords_v<double>(baseSegment.begin(), baseSegment.end()));

	// ����[�̃R���g���[�����W(���_���H1���H2)
	std::array<POINT, 3> ctrlArrowHead;
	// ���_���i�[
	ctrlArrowHead[0].x = ctrlBaseSegment[END].x;
	ctrlArrowHead[0].y = ctrlBaseSegment[END].y;

	// �����̒���
	double length = ctrlBaseSegment[START].Length(ctrlBaseSegment[END]);

	// �����̒P�ʃx�N�g�����Z�o(�v�Z�덷�̂��߂�double�^�Ƃ���)
	Coord<double> vec;
	vec.x = (static_cast<double>(ctrlBaseSegment[END].x) - ctrlBaseSegment[START].x) / length;
	vec.y = (static_cast<double>(ctrlBaseSegment[END].y) - ctrlBaseSegment[START].y) / length;

	// �����̉H
	double angle = PI - ARROW_WING_ANGLE;
	for (int i = 1; i <= 2; i++) {
		// ��]
		Coord<double> wing;
		wing.x = vec.x * cos(angle) - vec.y * sin(angle);
		wing.y = vec.x * sin(angle) + vec.y * cos(angle);
		// ����[�Ɉړ����Ē�����ݒ�
		wing *= ARROW_WING_LENGTH;
		wing.x += ctrlBaseSegment[END].x;
		wing.y += ctrlBaseSegment[END].y;

		// �H���i�[
		ctrlArrowHead[i].x = static_cast<long>(wing.x);
		ctrlArrowHead[i].y = static_cast<long>(wing.y);

		angle += ARROW_WING_ANGLE * 2;
	}

	// �h��Ԃ������̕`��
	if(fillType == FillType::NoFill) {
		GetDC()->MoveTo(ctrlArrowHead[0].x, ctrlArrowHead[0].y);
		GetDC()->LineTo(ctrlArrowHead[1].x, ctrlArrowHead[1].y);
		GetDC()->MoveTo(ctrlArrowHead[0].x, ctrlArrowHead[0].y);
		GetDC()->LineTo(ctrlArrowHead[2].x, ctrlArrowHead[2].y);
	}
	// �h��Ԃ�����̕`��
	else {
		GetDC()->Polygon(ctrlArrowHead.data(), ctrlArrowHead.size());
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

	// �~�ʕ������E���Ȃ�A�����Ōv�Z���ꂽ���W���t�]����
	if (direction == ArcDirectionType::Right) std::reverse(bezierPoints.begin(), bezierPoints.end());

	return bezierPoints;
}

// �x�W�G�Ȑ��ɂ��~�ʕ`��
// �n�_�ƏI�_����v����~�ʂŌĂяo�����ꍇ�̓���͖���`�i���̂悤�ȃP�[�X�͎��O�ɉ~�ƌ��Ȃ��ׂ��j
void Canvas::DrawBezierArc(Coords<double, 3> arc, ArcDirectionType direction) const
{
	// �~�ʂ��s�����Ȃ�`�悵�Ȃ�
	if (!Util::VerifyArc(arc)) return;

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
	m_info(pManager->m_info),
	m_canvas(pManager->GetCanvas()),
	m_pen(pManager->GetCurrentPen()),
	m_brush(pManager->GetCurrentBrush())
{
}

// JSON�f�[�^�擾
picojson::object Node::GetContents() const
{
	// JSON: �m�[�h
	picojson::object jNode;

	// JSON: �^�C�v
	jNode.insert(std::make_pair(JSON_KEY_TYPE, picojson::value(m_typeName)));

	// JSON: �y��
	picojson::object jPen;
	jPen.insert(std::make_pair(JSON_KEY_COLOR, picojson::value(static_cast<double>(m_pen.lopnColor))));
	jPen.insert(std::make_pair(JSON_KEY_WIDTH, picojson::value(static_cast<double>(m_pen.lopnWidth.x))));
	jPen.insert(std::make_pair(JSON_KEY_STYLE, picojson::value(static_cast<double>(m_pen.lopnStyle))));
	jNode.insert(std::make_pair(JSON_KEY_PEN, picojson::value(jPen)));

	// JSON: �u���V
	picojson::object jBrush;
	jBrush.insert(std::make_pair(JSON_KEY_COLOR, picojson::value(static_cast<double>(m_brush.lbColor))));
	jNode.insert(std::make_pair(JSON_KEY_BRUSH, picojson::value(jBrush)));

	// JSON: �_�Q
	if (m_data.points.size() > 0) {
		picojson::array jPoints;
		for (const auto& p : m_data.points) {
			picojson::array jCoords;
			jCoords.push_back(picojson::value(p.x));
			jCoords.push_back(picojson::value(p.y));
			jPoints.push_back(picojson::value(jCoords));
		}
		jNode.insert(std::make_pair(JSON_KEY_POINTS, picojson::value(jPoints)));
	}

	// JSON: ���a
	if (m_data.use_radius) {
		jNode.insert(std::make_pair(JSON_KEY_RADIUS, picojson::value(m_data.radius)));
	}

	// JSON: �_���
	if (m_data.use_pointType) {
		jNode.insert(std::make_pair(JSON_KEY_POINT_TYPE, picojson::value(static_cast<double>(m_data.pointType))));
	}
	// JSON: �����
	if (m_data.use_lineLimitType) {
		jNode.insert(std::make_pair(JSON_KEY_LINE_LIMIT_TYPE, picojson::value(static_cast<double>(m_data.lineLimitType))));
	}
	// JSON: �~�ʕ���
	if (m_data.use_arcDirectionType) {
		jNode.insert(std::make_pair(JSON_KEY_ARC_DIRECTION_TYPE, picojson::value(static_cast<double>(m_data.arcDirectionType))));
	}
	// JSON: �h��Ԃ����
	if (m_data.use_fillType) {
		jNode.insert(std::make_pair(JSON_KEY_FILL_TYPE, picojson::value(static_cast<double>(m_data.fillType))));
	}
	return jNode;
}

// JSON�f�[�^�ݒ�
bool Node::SetContents(picojson::object& jNode)
{
	// JSON: �y��
	if (jNode.count(JSON_KEY_PEN) == 0) return false;
	picojson::object& jPen = jNode[JSON_KEY_PEN].get<picojson::object>();
	if (jPen.count(JSON_KEY_COLOR) == 0) return false;
	if (jPen.count(JSON_KEY_WIDTH) == 0) return false;
	if (jPen.count(JSON_KEY_STYLE) == 0) return false;
	m_pen.lopnColor = static_cast<COLORREF>(jPen[JSON_KEY_COLOR].get<double>());
	m_pen.lopnWidth.x = static_cast<LONG>(jPen[JSON_KEY_WIDTH].get<double>());
	m_pen.lopnStyle = static_cast<UINT>(jPen[JSON_KEY_STYLE].get<double>());

	// JSON: �u���V
	if (jNode.count(JSON_KEY_BRUSH) == 0) return false;
	picojson::object& jBrush = jNode[JSON_KEY_BRUSH].get<picojson::object>();
	if (jBrush.count(JSON_KEY_COLOR) == 0) return false;
	m_brush.lbColor = static_cast<COLORREF>(jBrush[JSON_KEY_COLOR].get<double>());

	// JSON: �_�Q
	if (m_data.points.size() > 0) {
		if (jNode.count(JSON_KEY_POINTS) == 0) return false;
		picojson::array& jPoints = jNode[JSON_KEY_POINTS].get<picojson::array>();
		if (jPoints.size() != m_data.points.size()) return false;
		int i = 0;
		for (auto& jPoint : jPoints) {
			picojson::array& jCoords = jPoint.get< picojson::array>();
			if (jCoords.size() != 2) return false;
			m_data.points[i].x = jCoords[0].get<double>();
			m_data.points[i].y = jCoords[1].get<double>();
			i++;
		}
	}

	// JSON: ���a
	if (m_data.use_radius) {
		if (jNode.count(JSON_KEY_RADIUS) == 0) return false;
		m_data.radius = jNode[JSON_KEY_RADIUS].get<double>();
	}

	// JSON: �_���
	if (m_data.use_pointType) {
		if (jNode.count(JSON_KEY_POINT_TYPE) == 0) return false;
		m_data.pointType = static_cast<PointType>(static_cast<int>(jNode[JSON_KEY_POINT_TYPE].get<double>()));
	}
	// JSON: �����
	if (m_data.use_lineLimitType) {
		if (jNode.count(JSON_KEY_LINE_LIMIT_TYPE) == 0) return false;
		m_data.lineLimitType = static_cast<LineLimitType>(static_cast<int>(jNode[JSON_KEY_LINE_LIMIT_TYPE].get<double>()));
	}
	// JSON: �~�ʕ���
	if (m_data.use_arcDirectionType) {
		if (jNode.count(JSON_KEY_ARC_DIRECTION_TYPE) == 0) return false;
		m_data.arcDirectionType = static_cast<ArcDirectionType>(static_cast<int>(jNode[JSON_KEY_ARC_DIRECTION_TYPE].get<double>()));
	}
	// JSON: �h��Ԃ����
	if (m_data.use_fillType) {
		if (jNode.count(JSON_KEY_FILL_TYPE) == 0) return false;
		m_data.fillType = static_cast<FillType>(static_cast<int>(jNode[JSON_KEY_FILL_TYPE].get<double>()));
	}

	return true;
}

// �^�C�v���ɉ������m�[�h���쐬
Node* Node::FromTypeName(const std::string& typeName, Manager* pManager)
{
	Node* pNode = nullptr;
	if (typeName == JSON_VALUE_TYPE_GRID) pNode = new NodeGrid(pManager);
	else if (typeName == JSON_VALUE_TYPE_ORIGIN) pNode = new NodeOrigin(pManager);
	else if (typeName == JSON_VALUE_TYPE_AXIS) pNode = new NodeAxis(pManager);
	else if (typeName == JSON_VALUE_TYPE_POINT) pNode = new NodePoint(pManager);
	else if (typeName == JSON_VALUE_TYPE_LINE) pNode = new NodeLine(pManager);
	else if (typeName == JSON_VALUE_TYPE_ARC) pNode = new NodeArc(pManager);
	else if (typeName == JSON_VALUE_TYPE_CIRCLE) pNode = new NodeCircle(pManager);
	else if (typeName == JSON_VALUE_TYPE_POLYGON) pNode = new NodePolygon(pManager);
	else if (typeName == JSON_VALUE_TYPE_SECTOR) pNode = new NodeSector(pManager);
	return pNode;
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeGrid::CalcBoundingBox(bool forFit/*=false*/) const
{
	// �O���b�h�̏ꍇ�͍ŏ���ܔ��͖���
	return BoundingBox<double>();
}

// �`�󂪕`��̈�Ɋ܂܂�邩�`�F�b�N
bool NodeGrid::IsIncludeCanvas() const
{
	// �O���b�h�͏�Ɋ܂܂���ԂƂ���
	return true;
}

// �`���`��
void NodeGrid::DrawContent()
{
	// �O���b�h�`��
	if(m_info.isDrawGrid) m_canvas.DrawGrid(m_info.gridSize);
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeOrigin::CalcBoundingBox(bool forFit/*=false*/) const
{
	// ���_�̃T�C�Y
	double size = m_canvas.ControlToCanvas(m_info.gridSize);

	// ���_�̋�`���v�Z
	BoundingBox<double> bbox;
	bbox.min.x = m_data.points[BASE].x - size;
	bbox.min.y = m_data.points[BASE].y - size;
	bbox.max.x = m_data.points[BASE].x + size;
	bbox.max.y = m_data.points[BASE].y + size;
	return bbox;
}

// �`�󂪕`��̈�Ɋ܂܂�邩�`�F�b�N
bool NodeOrigin::IsIncludeCanvas() const
{
	// ���_�͏�Ɋ܂܂���ԂƂ���
	// TODO: �����ɂ͌��_�̖��̈ꕔ���܂܂�邩�̔��肪�K�v
	return true;
}

// �`���`��
void NodeOrigin::DrawContent()
{
	// ���_�`��
	if (m_info.isDrawOrigin) m_canvas.DrawOrigin(m_data.points[BASE], m_info.originSize);
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeAxis::CalcBoundingBox(bool forFit/*=false*/) const
{
	// ���̏ꍇ�͍ŏ���ܔ������_�Ƃ���
	return BoundingBox<double>(m_data.points[BASE]);
}

// �`�󂪕`��̈�Ɋ܂܂�邩�`�F�b�N
bool NodeAxis::IsIncludeCanvas() const
{
	// ���͏�Ɋ܂܂���ԂƂ���
	// TODO: �����ɂ͎��̈ꕔ���܂܂�邩�̔��肪�K�v
	return true;
}

// �`���`��
void NodeAxis::DrawContent()
{
	// ���`��
	if (m_info.isDrawAxis) m_canvas.DrawAxis(m_data.points[BASE], m_info.axisScale);
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodePoint::CalcBoundingBox(bool forFit/*=false*/) const
{
	// �ŏ���ܔ���_�Ƃ���
	return BoundingBox<double>(m_data.points[BASE]);
}

// �`���`��
void NodePoint::DrawContent()
{
	// �_�̎�ޕʂɕ`��
	switch (m_data.pointType) {
	case PointType::Pixel:
		// �s�N�Z��
		m_canvas.DrawPixel(m_data.points[BASE]);
		break;
	case PointType::Triangle:
		// �O�p�`
		m_canvas.DrawTrianglePoint(m_data.points[BASE]);
		break;
	case PointType::Large:
		// ����
		m_canvas.DrawLargePoint(m_data.points[BASE]);
		break;
	}
}


// �������擾
Coords<double, 2> NodeLine::Segment() const
{
	Coords<double, 2> points;

	// �L���̐���
	if (m_data.lineLimitType == LineLimitType::Finite) {
		points[START] = m_data.points[START];
		points[END] = m_data.points[END];
	}
	// ��������
	else {
		// �`��̈���擾
		BoundingBox<double> canvas = m_canvas.GetCanvasArea();

		// ������
		if (m_infiniteInfo.isVertical) {
			// x�ؕЂƕ`��̈�̏㉺Y���W�͈̔͂Ő������쐬
			points[START].x = m_infiniteInfo.interceptX;
			points[START].y = canvas.min.y;
			points[END].x = m_infiniteInfo.interceptX;
			points[END].y = canvas.max.y;
		}
		// �񉔒���
		else {
			// �`��̈�̍��EX���W�͈̔͂Ő������쐬 (y = c * x + i)
			points[START].x = canvas.min.x;
			points[START].y = m_infiniteInfo.coefficientX * canvas.min.x + m_infiniteInfo.interceptY;
			points[END].x = canvas.max.x;
			points[END].y = m_infiniteInfo.coefficientX * canvas.max.x + m_infiniteInfo.interceptY;
		}
	}
	return points;
}

// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeLine::CalcBoundingBox(bool forFit/*=false*/) const
{
	// Fit�̂��߂ɌĂяo���ꂽ�ꍇ�͖������������O����
	if (forFit && m_data.lineLimitType == LineLimitType::Infinite) return BoundingBox<double>();

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
	m_canvas.GetDC()->MoveTo(ctrlPoints[START].x, ctrlPoints[START].y);
	m_canvas.GetDC()->LineTo(ctrlPoints[END].x, ctrlPoints[END].y);

	// �����̏ꍇ�A��[�̖��`��
	if (m_data.lineLimitType == LineLimitType::Finite && m_info.isDrawArrow) {
		// ���������̉H��80%�̒������Z���Ȃ�قǏk������Ă���ꍇ�͕`�悵�Ȃ�
		double length = points[START].Length(points[END]);
		if (m_canvas.CanvasToControl(length) >= m_canvas.ARROW_WING_LENGTH * 0.8) {
			// ����`��
			m_canvas.DrawArrowHead(points);
		}
	}
}

// JSON�f�[�^�擾
picojson::object NodeLine::GetContents() const
{
	// ���N���X�̏��������{
	picojson::object jNode = Node::GetContents();

	// ���������̏ꍇ
	if (m_data.lineLimitType == LineLimitType::Infinite) {
		// JSON: ���������̏��
		picojson::object jInfiniteInfo;
		jInfiniteInfo.insert(std::make_pair(JSON_KEY_IS_VERTICAL, picojson::value(m_infiniteInfo.isVertical)));
		jInfiniteInfo.insert(std::make_pair(JSON_KEY_COEFFICIENT_X, picojson::value(m_infiniteInfo.coefficientX)));
		jInfiniteInfo.insert(std::make_pair(JSON_KEY_INTERCEPT_Y, picojson::value(m_infiniteInfo.interceptY)));
		jInfiniteInfo.insert(std::make_pair(JSON_KEY_INTERCEPT_X, picojson::value(m_infiniteInfo.interceptX)));
		jNode.insert(std::make_pair(JSON_KEY_INFINITE_INFO, picojson::value(jInfiniteInfo)));
	}

	return jNode;
}

// JSON�f�[�^�ݒ�
bool NodeLine::SetContents(picojson::object& jNode)
{
	// ���N���X�̏��������{
	if (!Node::SetContents(jNode)) return false;

	// ���������̏ꍇ
	if (m_data.lineLimitType == LineLimitType::Infinite) {
		// JSON: ���������̏��
		if (jNode.count(JSON_KEY_INFINITE_INFO) == 0) return false;
		picojson::object& jInfiniteInfo = jNode[JSON_KEY_INFINITE_INFO].get<picojson::object>();

		if (jInfiniteInfo.count(JSON_KEY_IS_VERTICAL) == 0) return false;
		if (jInfiniteInfo.count(JSON_KEY_COEFFICIENT_X) == 0) return false;
		if (jInfiniteInfo.count(JSON_KEY_INTERCEPT_Y) == 0) return false;
		if (jInfiniteInfo.count(JSON_KEY_INTERCEPT_X) == 0) return false;
		m_infiniteInfo.isVertical = jInfiniteInfo[JSON_KEY_IS_VERTICAL].get<bool>();
		m_infiniteInfo.coefficientX = jInfiniteInfo[JSON_KEY_COEFFICIENT_X].get<double>();
		m_infiniteInfo.interceptY = jInfiniteInfo[JSON_KEY_INTERCEPT_Y].get<double>();
		m_infiniteInfo.interceptX = jInfiniteInfo[JSON_KEY_INTERCEPT_X].get<double>();
	}

	return true;
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeArc::CalcBoundingBox(bool forFit/*=false*/) const
{
	// �x�W�G�Ȑ��ɂ����W�l�z����擾
	Coords_v<double> bezierPoints = m_canvas.CalcBezierArc(
		Coords<double, 3> { m_data.points[START], m_data.points[END], m_data.points[CENTER] },
		m_data.arcDirectionType
	);

	// ���W�l�z��� 3n+1 �ō\������Ă���
	// �~�ʂ��ی����������Ƃ��̎n�_/�I�_�̊Ԃɐ���_��2���܂��Ă�����

	// ����_���������~�ʂ̎n�_�ƏI�_�݂̂���ŏ���ܔ����Z�o
	BoundingBox<double> bbox;
	auto itr = bezierPoints.cbegin();
	for (int i = 0; i != bezierPoints.size(); i++) {
		if (i % 3 == 0) bbox += *itr;;
		itr++;
	}
	return bbox;
}

// �`���`��
void NodeArc::DrawContent()
{
	// �x�W�G�Ȑ��ŉ~�ʂ�`��
	m_canvas.DrawBezierArc(
		Coords<double, 3> { m_data.points[START], m_data.points[END], m_data.points[CENTER] },
		m_data.arcDirectionType
	);

	// ���a�����̉H�̒������Z���Ȃ�قǏk������Ă���ꍇ�͕`�悵�Ȃ�
	// �~�ʒ��S�_�̕`������������Ƃ���
	double radius = m_data.points[CENTER].Length(m_data.points[START]);
	if (m_canvas.CanvasToControl(radius) >= m_canvas.ARROW_WING_LENGTH) {
		// �~�ʂ̐�[�̖��`��
		if (m_info.isDrawArrow) {
			// ����`�����߂̎�
			Coords<double, 2> arrowAxis;

			// ��]�p�x������
			//   �~�ʕ��������F+90�x
			//   �~�ʕ������E�F-90�x
			double angle = m_data.arcDirectionType == ArcDirectionType::Left ? PI / 2 : -PI / 2;

			// �I�_����ɁA���S�_����Ō��߂���]�p�x������]����
			// ����`�����߂̎��̎n�_���Z�o
			arrowAxis[START] = m_data.points[END].Rotate(m_data.points[CENTER], angle);
			// ����`�����߂̎��̏I�_�͉~�ʂ̏I�_
			arrowAxis[END] = m_data.points[END];

			// ����`��
			m_canvas.DrawArrowHead(arrowAxis);
		}

		// �~�ʂ̒��S�_��`��
		if (m_info.isDrawCenter) {
			m_canvas.DrawTrianglePoint(m_data.points[CENTER]);
		}
	}
}

// �`��̐������`�F�b�N
bool NodeArc::Verify() const
{
	return Util::VerifyArc(Coords<double, 3> { m_data.points[START], m_data.points[END], m_data.points[CENTER] });
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeCircle::CalcBoundingBox(bool forFit/*=false*/) const
{
	// �~�̋�`���v�Z
	BoundingBox<double> bbox;
	bbox.min.x = m_data.points[BASE].x - m_data.radius;
	bbox.min.y = m_data.points[BASE].y - m_data.radius;
	bbox.max.x = m_data.points[BASE].x + m_data.radius;
	bbox.max.y = m_data.points[BASE].y + m_data.radius;
	return bbox;
}
// �`���`��
void NodeCircle::DrawContent()
{
	// �h��Ԃ��Ȃ�
	if (m_data.fillType == FillType::NoFill) {
		// 0�x����180�x�̉~��
		Coords<double, 3> arc;
		arc.fill(m_data.points[BASE]);
		arc[START].x += m_data.radius;
		arc[END].x -= m_data.radius;
		// ���~��`��
		m_canvas.DrawBezierArc(arc, ArcDirectionType::Left);
		// ���~��`��
		m_canvas.DrawBezierArc(arc, ArcDirectionType::Right);

		// ���a�����̉H�̒������Z���Ȃ�قǏk������Ă���ꍇ�͕`�悵�Ȃ�
		// �~�ʒ��S�_�̕`������������Ƃ���
		double radius = arc[CENTER].Length(arc[START]);
		if (m_canvas.CanvasToControl(radius) >= m_canvas.ARROW_WING_LENGTH) {
			// ���S�_��`��
			if (m_info.isDrawCenter) {
				// �O�p�`�̓_��`��
				m_canvas.DrawTrianglePoint(m_data.points[BASE]);
			}
		}
	}
	// �h��Ԃ�����
	else {
		// �~�̊O�ڂ����`�̍���ƉE���̓_���Z�o
		Coord<long> ctrlLeftUp = m_canvas.CanvasToControl(Coord<double>(m_data.points[BASE].x - m_data.radius, m_data.points[BASE].y + m_data.radius));
		Coord<long> ctrlRightDown = m_canvas.CanvasToControl(Coord<double>(m_data.points[BASE].x + m_data.radius, m_data.points[BASE].y - m_data.radius));
		// �~��`��
		m_canvas.GetDC()->Ellipse(ctrlLeftUp.x, ctrlLeftUp.y, ctrlRightDown.x, ctrlRightDown.y);
	}
}


// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodePolygon::CalcBoundingBox(bool forFit/*=false*/) const
{
	// �`����\��������W����ŏ���ܔ����Z�o
	BoundingBox<double> bbox;
	for (const auto& p : m_data.points) {
		bbox += p;
	}
	return bbox;
}
// �`���`��
void NodePolygon::DrawContent()
{
	// �h��Ԃ��Ȃ�
	if (m_data.fillType == FillType::NoFill) {
		// �`��̍��W�l���R���g���[�����W�ɕϊ�
		Coords_v<long> ctrlPoints = m_canvas.CanvasToControl(m_data.points);

		// �Ō�̓_�Ɉړ����Ă���
		m_canvas.GetDC()->MoveTo(ctrlPoints.rbegin()->x, ctrlPoints.rbegin()->y);
		// ���p�`��`��
		for (const auto& here : ctrlPoints) {
			m_canvas.GetDC()->LineTo(here.x, here.y);
		}

		// ����`��
		if (m_info.isDrawArrow) {
			// �Ō�̓_����̎��̎n�_�Ƃ���
			Coord<double> s = *(m_data.points.rbegin());
			// ���̎��̏I�_�Ƃ��č��W�����Ɏ擾
			for (const auto& e : m_data.points) {
				// ���������̉H��80%�̒������Z���Ȃ�قǏk������Ă���ꍇ�͕`�悵�Ȃ�
				double length = s.Length(e);
				if (m_canvas.CanvasToControl(length) >= m_canvas.ARROW_WING_LENGTH * 0.8) {
					// ����`��
					m_canvas.DrawArrowHead(Coords<double, 2>{s, e});
				}
				// ���̎n�_���X�V
				s = e;
			}
		}
	}
	// �h��Ԃ�����
	else {
		// ���W��POINT�^�̔z��֕ϊ�
		std::vector<POINT> ctrlPoints;
		for (const auto& p : m_data.points) {
			auto cp = m_canvas.CanvasToControl(p);
			ctrlPoints.push_back(POINT{ cp.x, cp.y });
		}
		// ���p�`��`��
		m_canvas.GetDC()->Polygon(ctrlPoints.data(), ctrlPoints.size());
	}
}

// JSON�f�[�^�ݒ�
bool NodePolygon::SetContents(picojson::object& jNode)
{
	// ���N���X�̏��������{
	if (!Node::SetContents(jNode)) return false;

	// NodePolygon�̓_�Q�͉ς̂��ߊ��N���X�ł͐ݒ�s�\�̂��߂����Őݒ肷��
	if (jNode.count(JSON_KEY_POINTS) == 0) return false;
	picojson::array& jPoints = jNode[JSON_KEY_POINTS].get<picojson::array>();
	for (auto& jPoint : jPoints) {
		picojson::array& jCoords = jPoint.get< picojson::array>();
		if (jCoords.size() != 2) return false;
		m_data.points.push_back(Coord<double>(jCoords[0].get<double>(), jCoords[1].get<double>()));
	}

	return true;
}


// �����̉~�ʍ��W���Z�o
Coords<double, 3> NodeSector::CalcInnerArc() const
{
	// ���S�_����n�_�ƏI�_�ւ̃x�N�g�����Z�o
	Coord<double> vectorCS = m_data.points[START] - m_data.points[CENTER];
	Coord<double> vectorCE = m_data.points[END] - m_data.points[CENTER];

	// �O���̉~�ʔ��a���Z�o
	double outerRadius = m_data.points[CENTER].Length(m_data.points[START]);

	// �����̉~�ʍ��W���Z�o�i���S�_����̃x�N�g��������~�ʂ̒����ɕϊ������̍��W�֖߂��j
	Coords<double, 3> innerArc;
	innerArc[START] = vectorCS / outerRadius * m_data.radius + m_data.points[CENTER];
	innerArc[END] = vectorCE / outerRadius * m_data.radius + m_data.points[CENTER];
	innerArc[CENTER] = m_data.points[CENTER];

	// �����̉~�ʂ������ȏꍇ�͒��S�_�̍��W�Ƃ���
	if (Util::IsSamePoint(innerArc[START], innerArc[CENTER]) ||
		Util::IsSamePoint(innerArc[END], innerArc[CENTER])) {
		innerArc[START] = innerArc[END] = innerArc[CENTER];
	}

	return innerArc;
}

// ��`�̃p�X��DC�ɐݒ�
void NodeSector::CreateSectorPath() const
{
	// �O���Ɠ����̉~�ʂ��擾
	Coords<double, 3> outerArc { m_data.points[START], m_data.points[END], m_data.points[CENTER] };
	Coords<double, 3> innerArc = CalcInnerArc();
	// �����̉~�ʂ͕������t�]���Ă���
	std::swap(innerArc[START], innerArc[END]);
	// �R���g���[�����W�ɕϊ�
	Coords_v<long> ctrlOuterArc = m_canvas.CanvasToControl(Coords_v<double>(outerArc.begin(), outerArc.end()));
	Coords_v<long> ctrlInnerArc = m_canvas.CanvasToControl(Coords_v<double>(innerArc.begin(), innerArc.end()));
	// �~�ʕ���
	ArcDirectionType outerDir = m_data.arcDirectionType;
	ArcDirectionType innerDir = (outerDir == ArcDirectionType::Left ? ArcDirectionType::Right : ArcDirectionType::Left);

	// �p�X���N���A
	m_canvas.GetDC()->AbortPath();
	// �p�X���J�n
	m_canvas.GetDC()->BeginPath();

	// �O���~�ʁ������������~�ʁ������̏��ɐ�`�̃p�X���쐬
	m_canvas.DrawBezierArc(outerArc, outerDir);
	m_canvas.GetDC()->MoveTo(ctrlOuterArc[END].x, ctrlOuterArc[END].y);
	m_canvas.GetDC()->LineTo(ctrlInnerArc[START].x, ctrlInnerArc[START].y);
	m_canvas.DrawBezierArc(innerArc, innerDir);
	m_canvas.GetDC()->MoveTo(ctrlInnerArc[END].x, ctrlInnerArc[END].y);
	m_canvas.GetDC()->LineTo(ctrlOuterArc[START].x, ctrlOuterArc[START].y);

	// �p�X���I��
	m_canvas.GetDC()->EndPath();
}

// ��`�̃��[�W�����i�R���g���[�����W�j���Z�o
void NodeSector::CreateSectorRgn(CRgn* pSectorRgn) const
{
	// ��CreateSectorPath(), CreateFromPath() �ł́A
	//   �x�W�G�Ȑ������ƒ����������ʁX�̕̈�ƔF������Ă��܂����܂������Ȃ�

	// �O���̉~�ʁi�R���g���[�����W�j
	Coords_v<long> ctrlOuterArc = m_canvas.CanvasToControl(Coords_v<double>(m_data.points.begin(), m_data.points.end()));
	// �������̉~�ʂɂ��Ă���
	if (m_data.arcDirectionType == ArcDirectionType::Right) {
		std::swap(ctrlOuterArc[START], ctrlOuterArc[END]);
	}

	// DC�𗘗p���ĊO���̐�`�̃p�X���쐬

	// �p�X���N���A
	m_canvas.GetDC()->AbortPath();
	// �p�X���J�n
	m_canvas.GetDC()->BeginPath();
	// �~�ʕ����̃p�X���~�ʏI�_���璆�S�_�ւ̃p�X�����S�_����~�ʎn�_�ւ̃p�X
	m_canvas.DrawBezierArc(
		Coords<double, 3>{ m_data.points[START], m_data.points[END], m_data.points[CENTER] },
		m_data.arcDirectionType
	);
	m_canvas.GetDC()->MoveTo(ctrlOuterArc[END].x, ctrlOuterArc[END].y);
	m_canvas.GetDC()->LineTo(ctrlOuterArc[CENTER].x, ctrlOuterArc[CENTER].y);
	m_canvas.GetDC()->LineTo(ctrlOuterArc[START].x, ctrlOuterArc[START].y);
	// �p�X���I��
	m_canvas.GetDC()->EndPath();

	// �O���̐�`���[�W�������擾
	pSectorRgn->CreateFromPath(m_canvas.GetDC());

	// �����~�ʂ̔��a���R���g���[�����W�ɕϊ�
	long ctrlInnerRadius = static_cast<long>(m_canvas.CanvasToControl(m_data.radius));

	// �����̉~�`���[�W�������擾
	CRgn innerCircleRgn;
	innerCircleRgn.CreateEllipticRgn(
		ctrlOuterArc[CENTER].x - ctrlInnerRadius,
		ctrlOuterArc[CENTER].y - ctrlInnerRadius,
		ctrlOuterArc[CENTER].x + ctrlInnerRadius,
		ctrlOuterArc[CENTER].y + ctrlInnerRadius
	);

	// �O���̐�`���[�W���� - �����̉~�`���[�W����
	pSectorRgn->CombineRgn(pSectorRgn, &innerCircleRgn, RGN_DIFF);
}

// �`��̍ŏ���ܔ����Z�o
BoundingBox<double> NodeSector::CalcBoundingBox(bool forFit/*=false*/) const
{
	// ��`�̃��[�W��������ŏ���ܔ��i�R���g���[�����W�j���擾
	CRgn sectorRgn;
	CreateSectorRgn(&sectorRgn);
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
	if (m_data.fillType == FillType::Fill) {
		// ��`���[�W�������擾���ēh��Ԃ�
		CRgn sectorRgn;
		CreateSectorRgn(&sectorRgn);
		m_canvas.GetDC()->FillRgn(&sectorRgn, m_canvas.GetDC()->GetCurrentBrush());
	}

	// ��`�̃p�X���쐬���ė֊s����`��
	CreateSectorPath();
	m_canvas.GetDC()->StrokePath();
}

// �`��̐������`�F�b�N
bool NodeSector::Verify() const
{
	return Util::VerifyArc(
		Coords<double, 3>{ m_data.points[START], m_data.points[END], m_data.points[CENTER] }
	);
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
BoundingBox<double> Layer::CalcBoundingBox(bool forFit/*=false*/) const
{
	BoundingBox<double> bbox;
	// �`��t���OON�Ȃ�
	if (m_enableDraw) {
		// �S�m�[�h�̍ŏ���ܔ�������
		for (const auto& pNode : m_nodes) {
			bbox += pNode->CalcBoundingBox(forFit);
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

// JSON�f�[�^�擾
picojson::object Layer::GetContents() const
{
	// JSON: ���C���[
	picojson::object jLayer;
	// JSON: EnableDraw
	jLayer.insert(std::make_pair(JSON_KEY_ENABLE_DRAW, picojson::value(m_enableDraw)));

	// JSON: �m�[�h�z��
	picojson::array jNodes;
	// �S�m�[�h��JSON�f�[�^���쐬
	for (const auto& pNode : m_nodes) {
		picojson::object jNode = pNode->GetContents();
		jNodes.push_back(picojson::value(jNode));
	}
	// �Z�b�g
	jLayer.insert(std::make_pair(JSON_KEY_NODES, picojson::value(jNodes)));

	return jLayer;
}

// JSON�f�[�^�ݒ�
bool Layer::SetContents(picojson::object& jLayer, Manager* pManager)
{
	// JSON: EnableDraw
	if (jLayer.count(JSON_KEY_ENABLE_DRAW) == 0) return false;
	m_enableDraw = jLayer[JSON_KEY_ENABLE_DRAW].get<bool>();

	// JSON: �m�[�h�R���N�V�������擾
	if (jLayer.count(JSON_KEY_NODES) == 0) return false;
	picojson::array& jNodes = jLayer[JSON_KEY_NODES].get<picojson::array>();

	// �S�m�[�h��JSON�f�[�^���쐬
	m_nodes.clear();
	for (auto& v : jNodes) {
		picojson::object& jNode = v.get<picojson::object>();

		// �^�C�v�ɉ������m�[�h���쐬
		if (jNode.count(JSON_KEY_TYPE) == 0) return false;
		Node *p = Node::FromTypeName(jNode[JSON_KEY_TYPE].get<std::string>(), pManager);
		if (!p) return false;
		std::unique_ptr<Node> pNode(p);

		// �m�[�h��JSON�f�[�^��ݒ�
		if (!pNode->SetContents(jNode)) return false;
		// �R���N�V�����ɓo�^
		m_nodes.push_back(std::move(pNode));
	}

	return true;
}


// �R���X�g���N�^
Manager::Manager() :
	m_canvas(),
	m_currentLayerNo(0),
	m_penColor(0),
	m_penWidth(0),
	m_penStyle(0),
	m_brushColor(0),
	m_info({ 0 })
{
	// ������
	Clear();
}

// �S�`��̍ŏ���ܔ����Z�o
BoundingBox<double> Manager::CalcBoundingBox(bool forFit/*=false*/) const
{
	// �x�[�X���C���[�̓��e�͊܂߂Ȃ�

	BoundingBox<double> bbox;
	// �S���C���[�̍ŏ���ܔ�������
	for (const auto& pLayer : m_layers) {
		bbox += pLayer->CalcBoundingBox(forFit);
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
	if (m_layers.size() == 0) {
		// ���C���[��1���ǉ�
		m_layers.push_back(std::make_unique<Layer>());
		m_currentLayerNo = 0;
	}
	// �Ō�̃��C���[�̏ꍇ
	else if (m_layers.size() == 1) {
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

// �`����e��Json�G�N�X�|�[�g
bool Manager::SaveContents(const std::tstring& filePath) const
{
	// JSON: ���[�g
	picojson::object jRoot;

	// JSON: ���C���[�z��
	picojson::array jLayers;

	// �S���C���[��JSON�f�[�^���쐬
	for (const auto& pLayer : m_layers) {
		picojson::object jLayer = pLayer->GetContents();
		jLayers.push_back(picojson::value(jLayer));
	}
	// ���[�g�ɃZ�b�g
	jRoot.insert(std::make_pair(JSON_KEY_LAYERS, picojson::value(jLayers)));

	// �t�@�C���I�[�v��
	std::ofstream ofs(filePath);
	if (!ofs) return false;

	// JSON�o��
	ofs << picojson::value(jRoot).serialize(true);
	ofs.close();
	return true;
}

// �`����e��Json�C���|�[�g
bool Manager::LoadContents(const std::tstring& filePath)
{
	// �t�@�C���I�[�v��
	std::ifstream ifs(filePath, std::ios::in);
	if (!ifs) return false;

	// JSON�������ǂݍ���
	const std::string json((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();

	// JSON���p�[�X
	picojson::value jData;
	const std::string err = picojson::parse(jData, json);
	if (!err.empty()) return false;

	// JSON: ���[�g���擾
	picojson::object& jRoot = jData.get<picojson::object>();
	// JSON: ���C���[�R���N�V�������擾
	if (jRoot.count(JSON_KEY_LAYERS) == 0) return false;
	picojson::array& jLayers = jRoot[JSON_KEY_LAYERS].get<picojson::array>();

	// �ꎞ�I�ȃ��C���[�R���N�V�������쐬���ēǂݍ���
	std::vector<std::unique_ptr<Layer>> tmplayers;

	// �S���C���[��JSON�f�[�^���쐬
	for (auto& v : jLayers) {
		picojson::object& jLayer = v.get<picojson::object>();

		// ���C���[���쐬����JSON�f�[�^��ݒ�
		std::unique_ptr<Layer> pLayer = std::make_unique<Layer>();
		if (!pLayer->SetContents(jLayer, this)) return false;
		// �R���N�V�����ɓo�^
		tmplayers.push_back(std::move(pLayer));
	}

	// �쐬�������e���m�肷��
	m_layers.clear();
	m_layers = std::move(tmplayers);
	return true;
}

// ������
void Manager::Clear()
{
	// �y���ƃu���V��������
	SetCurrentPen(DEFAULT_PEN);
	SetCurrentBrush(DEFAULT_BRUSH);

	// ���C���[�R���N�V�������N���A
	m_layers.clear();
	// ���C���[��1���ǉ�
	m_layers.push_back(std::make_unique<Layer>());
	m_currentLayerNo = 0;
}

// �`��
void Manager::Draw(bool isDesignMode/*=false*/)
{
	// �f�U�C�����[�h�p�ɕ`������o�b�N�A�b�v
	double bkRatio = m_canvas.GetRatio();
	Coord<double> bkOffset = m_canvas.GetOffset();
	LOGPEN bkPen = GetCurrentPen();
	LOGBRUSH bkBrush = GetCurrentBrush();

	// �f�U�C�����[�h�p�̕`��
	if (isDesignMode) {
		// �g��k�����̓R���X�g���N�g���̏����l�Ƃ���
		m_canvas.SetRatio(Canvas::DEFAULT_RATIO);
		// �I�t�Z�b�g�̓R���g���[���̋�`�̒����Ƃ���
		m_canvas.SetOffset(
			Coord<double> { (double)m_canvas.GetRect()->CenterPoint().x, (double)m_canvas.GetRect()->CenterPoint().y }
		);
	}

	// �y���ƃu���V�̏����l
	LOGPEN logPen = DEFAULT_PEN;
	LOGBRUSH logBrush = DEFAULT_BRUSH;

	if(true){
		// �y���ƃu���V��ύX�i�f�t�H���g����F�̂ݕύX�j
		logPen.lopnColor = m_info.backColor;
		logBrush.lbColor = m_info.backColor;
		PenBrushChanger pc(m_canvas.GetDC(), logPen);
		PenBrushChanger bc(m_canvas.GetDC(), logBrush);
		// �w�i�F�œh��Ԃ�
		m_canvas.FillBackground();
	}

	if (m_info.isDrawGrid) {
		// �y���ƃu���V��ύX�i�f�t�H���g����F�̂ݕύX�j
		logPen.lopnColor = m_info.gridColor;
		logBrush.lbColor = m_info.gridColor;
		PenBrushChanger pc(m_canvas.GetDC(), logPen);
		PenBrushChanger bc(m_canvas.GetDC(), logBrush);
		// �O���b�h�`��
		m_canvas.DrawGrid(m_info.gridSize);
	}

	if (m_info.isDrawAxis) {
		// �y���ƃu���V��ύX�i�f�t�H���g����F�̂ݕύX�j
		logPen.lopnColor = m_info.axisColor;
		logBrush.lbColor = m_info.axisColor;
		PenBrushChanger pc(m_canvas.GetDC(), logPen);
		PenBrushChanger bc(m_canvas.GetDC(), logBrush);
		// ���`��
		m_canvas.DrawAxis(Coord<double>(0.0, 0.0), m_info.axisScale);
	}

	if (m_info.isDrawOrigin) {
		// �y���ƃu���V��ύX�i�f�t�H���g����F�̂ݕύX�j
		logPen.lopnColor = m_info.originColor;
		logBrush.lbColor = m_info.originColor;
		PenBrushChanger pc(m_canvas.GetDC(), logPen);
		PenBrushChanger bc(m_canvas.GetDC(), logBrush);
		// ���_�`��
		m_canvas.DrawOrigin(Coord<double>(0.0, 0.0), m_info.originSize);
	}

	// �ʏ�̕`��
	if (!isDesignMode) {
		// �S���C���[��`��
		for (const auto& pLayer : m_layers) {
			pLayer->Draw();
		}
	}

	// �f�U�C�����[�h�p�ɕ`��������X�g�A
	m_canvas.SetRatio(bkRatio);
	m_canvas.SetOffset(bkOffset);
	SetCurrentPen(bkPen);
	SetCurrentBrush(bkBrush);
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
	// �`�悷��S�`��̍ŏ���ܔ����擾(Fit�p)
	BoundingBox<double> shapeBox = CalcBoundingBox(true);

	// �g��k����(�����l)
	double ratio = Canvas::DEFAULT_RATIO;
	// �I�t�Z�b�g(�����l)
	Coord<double> offset = Coord<double>(
		m_canvas.GetRect()->CenterPoint().x,
		m_canvas.GetRect()->CenterPoint().y
	);

	// �ŏ���ܔ��擾����
	if (shapeBox.Verify()) {
		// �`��̏c������Z�o
		double shapeAspect = std::numeric_limits<double>::max();
		if (shapeBox.GetWidth() != 0.0) {
			shapeAspect = shapeBox.GetHeight() / shapeBox.GetWidth();
		}

		// �`��̈�̏c����
		double cnvsAspect = std::numeric_limits<double>::max();
		if (m_canvas.GetRect()->Width() != 0.0) {
			cnvsAspect = static_cast<double>(m_canvas.GetRect()->Height()) / m_canvas.GetRect()->Width();
		}

		// �g��k�������Z�o
		// �`��̈�ɑ΂��Č`�󂪏c���Ȃ�c��������Ƃ���
		if (shapeAspect > cnvsAspect) {
			// (�`��̈�T�C�Y / �`��T�C�Y) * �`��̐�L��
			if(shapeBox.GetHeight() != 0.0)
				ratio = (m_canvas.GetRect()->Height() / shapeBox.GetHeight()) * shapeOccupancy;
		}
		// �`��̈�ɑ΂��Č`�󂪉����Ȃ牡��������Ƃ���
		else {
			// (�`��̈�T�C�Y / �`��T�C�Y) * �`��̐�L��
			if (shapeBox.GetWidth() != 0.0)
				ratio = (m_canvas.GetRect()->Width() / shapeBox.GetWidth()) * shapeOccupancy;
		}

		// �I�t�Z�b�g���Z�o
		offset.x = ((m_canvas.GetRect()->Width() - shapeBox.GetWidth() * ratio) / 2.0) - shapeBox.min.x * ratio;
		double y = ((m_canvas.GetRect()->Height() - shapeBox.GetHeight() * ratio) / 2.0) - shapeBox.min.y * ratio;
		offset.y = m_canvas.GetRect()->Height() - y;	// Y�����]
	}

	// �g��k�����ƃI�t�Z�b�g���X�V
	m_canvas.SetRatio(ratio);
	m_canvas.SetOffset(offset);

	// �ĕ`��
	Draw();
}

}	// namespace Drawer
