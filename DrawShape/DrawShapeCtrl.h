#pragma once

// DrawShapeCtrl.h : CDrawShapeCtrl ActiveX コントロール クラスの宣言。


// CDrawShapeCtrl : 実装については DrawShapeCtrl.cpp を参照してください。

class CDrawShapeCtrl : public COleControl
{
	DECLARE_DYNCREATE(CDrawShapeCtrl)

// コンストラクター
public:
	CDrawShapeCtrl();

// オーバーライド
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 実装
protected:
	~CDrawShapeCtrl();

	DECLARE_OLECREATE_EX(CDrawShapeCtrl)    // クラス ファクトリ と guid
	DECLARE_OLETYPELIB(CDrawShapeCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CDrawShapeCtrl)     // プロパティ ページ ID
	DECLARE_OLECTLTYPE(CDrawShapeCtrl)		// タイプ名とその他のステータス

// メッセージ マップ
	DECLARE_MESSAGE_MAP()

// ディスパッチ マップ
	DECLARE_DISPATCH_MAP()

// イベント マップ
	DECLARE_EVENT_MAP()

// ディスパッチ と イベント ID
public:
	enum {
	};
};

