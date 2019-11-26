#pragma once

// DrawShapePropPage.h : CDrawShapePropPage プロパティ ページ クラスの宣言。


// CDrawShapePropPage : 実装については DrawShapePropPage.cpp を参照してください。

class CDrawShapePropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CDrawShapePropPage)
	DECLARE_OLECREATE_EX(CDrawShapePropPage)

// コンストラクター
public:
	CDrawShapePropPage();

// ダイアログ データ
	enum { IDD = IDD_PROPPAGE_DRAWSHAPE };

// 実装
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// メッセージ マップ
protected:
	DECLARE_MESSAGE_MAP()
};

