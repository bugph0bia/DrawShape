#pragma once

#include "pch.h"
#include <vector>
#include <array>
#include <string>

namespace Drawer
{

// �N���X�O���錾
class Manager;
class Canvas;
class Layer;
class Node;

// ���W�f�[�^�^
template<typename T>
using coord_t = std::array<T, 2>;
// ���W�f�[�^�z��^
template<typename T, size_t N>
using coords_t = std::array<coord_t<T>, N>;


// �`��Ǘ��N���X
class Manager
{
private:
	// ��ʃR���g���[��
	COleControl& m_ctrl;
	// �`��L�����o�X
	Canvas m_canvas;

	// �x�[�X���C���[
	Layer m_baseLayer;
	// �`�惌�C���[�R���N�V����
	std::vector<Layer> m_layers;

	// �w�i�F
	COLORREF m_backgroundColor;
	// �O���b�h�F
	COLORREF m_gridColor;
	// �O���b�h�T�C�Y
	double m_gridSize;
	// ���_�F
	COLORREF m_originColor;
	// ���_�T�C�Y
	double m_originSize;
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
	void CalcBoundingBox(coord_t<double> &min, coord_t<double> &max) const;

public:
	// �R���X�g���N�^
	Manager(COleControl &ctrl, CDC &dc);
	
	// �R�s�[�R���X�g���N�^
	Manager(const Manager&) = delete;
	// ������Z�q
	Manager& operator=(const Manager&) = delete;

	// �������i���C���[�P���A�`��f�[�^�Ȃ��j
	void init();

	// �`��
	void Draw();

	// �g��k��
	void Zoom(double ratio);
	// �p��
	void Pan(coord_t<double> offset);
	// �t�B�b�g
	void Fit();

	// ���C���[�������擾
	size_t GetLayerCount() const { return m_layers.size(); }

	// ��ʃR���g���[���̋�`���擾�i�N���C�A���g���W�j
	CRect GetControlRect() const;
};


// �`��L�����o�X�N���X
class Canvas
{
private:
	// �`��Ώۂ̃f�o�C�X�R���e�L�X�g
	CDC &m_dc;

	// �g��k����
	double m_ratio;
	// �I�t�Z�b�g
	coord_t<double>& m_offset;

public:
	// �R���X�g���N�^
	Canvas(CDC& dc);

	// �g��k�����擾
	double GetRatio() const { return m_ratio; }
	// �I�t�Z�b�gX�擾
	double GetOffsetX() const { return m_offsetX; }
	// �I�t�Z�b�gY�擾
	double GetOffsetY() const { return m_offsetY; }

	// ���W�n�ϊ��F�R���g���[�����W�n�������L�����o�X���W�n
	void CanvasToControl(coord_t<long> ctrlCoord, coord_t<double> &canvasCoord) const;
	// ���W�n�ϊ��F�����L�����o�X���W�n���R���g���[�����W�n
	void ControlToCanvas(coord_t<double>& canvasCoord, coord_t<long> ctrlCoord) const;

	// �w�i��h��Ԃ�
	void FillBackground(COLORREF color);
	// �O���b�h�`��
	void DrawGrid(COLORREF color, double size);
	// ���_�`��
	void DrawOrigin(COLORREF color, double size);
	// ���`��
	void DrawAxis(COLORREF color, double scale);
	// �_�`��
	void DrawPoint();
	// ���`��
	void DrawArrow();
	// �x�W�G�Ȑ��ɂ��~�ʕ`��
	void DrawBezierArc();

	// �`����e���t�@�C���ۑ�
	bool SaveBitmap(const std::string filePath) const;
	// �`����e���N���b�v�{�[�h�փR�s�[
	bool CopyBitmap() const;
};


// ���C���[�N���X
class Layer
{
private:
	// �`��Ǘ��I�u�W�F�N�g
	Manager& m_mng;
	// �m�[�h�R���N�V����
	std::vector<Node> m_nodes;

	// �`��t���O
	bool m_isDraw;

public:
	// �R���X�g���N�^
	Layer(Manager& mng);

	// �S�`��̍ŏ���ܔ����Z�o
	void CalcBoundingBox(coord_t<double>& min, coord_t<double>& max) const;

	// �`��t���O�ݒ�
	void SetIsDraw(bool val) { m_isDraw = val; }
	// �`��t���O�擾
	bool GetIsDraw() const { return m_isDraw; }

	// �`��
	void Draw();
};


// �m�[�h�N���X
class Node
{
private:
	// �`��Ǘ��I�u�W�F�N�g
	Manager& m_mng;

public:
	// �R���X�g���N�^
	Node(Manager& mng) : m_mng(mng) {};
	// �f�X�g���N�^
	virtual ~Node() {};

	// �`��̍ŏ���ܔ����Z�o
	virtual void CalcBoundingBox(coord_t<double>& min, coord_t<double>& max) const {}
	// �`�󂪕`��G���A�Ɋ܂܂�邩�`�F�b�N
	virtual bool IsIncludeDrawArea() const {}
	// �`��
	virtual void Draw() {}

};

// �����m�[�h�N���X
class LineNode : public Node
{
private:
	// ���W�f�[�^�z��
	coords_t<double, 2> m_datas;

public:
	// �R���X�g���N�^
	LineNode(Manager& mng);

	// �`��̍ŏ���ܔ����Z�o
	void CalcBoundingBox(coord_t<double>& min, coord_t<double>& max) const override;
	// �`�󂪕`��G���A�Ɋ܂܂�邩�`�F�b�N
	bool IsIncludeDrawArea() const override;

	// �`��
	void Draw() override;
};

}	// namespace DrawShapeLib
