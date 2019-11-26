// DrawShapeCtrl.cpp : CDrawShapeCtrl ActiveX コントロール クラスの実装。

#include "pch.h"
#include "framework.h"
#include "DrawShape.h"
#include "DrawShapeCtrl.h"
#include "DrawShapePropPage.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CDrawShapeCtrl, COleControl)

// メッセージ マップ

BEGIN_MESSAGE_MAP(CDrawShapeCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

// ディスパッチ マップ

BEGIN_DISPATCH_MAP(CDrawShapeCtrl, COleControl)
END_DISPATCH_MAP()

// イベント マップ

BEGIN_EVENT_MAP(CDrawShapeCtrl, COleControl)
END_EVENT_MAP()

// プロパティ ページ

// TODO: プロパティ ページを追加して、BEGIN 行の最後にあるカウントを増やしてください。
BEGIN_PROPPAGEIDS(CDrawShapeCtrl, 1)
	PROPPAGEID(CDrawShapePropPage::guid)
END_PROPPAGEIDS(CDrawShapeCtrl)

// クラス ファクトリおよび GUID を初期化します。

IMPLEMENT_OLECREATE_EX(CDrawShapeCtrl, "MFCACTIVEXCONTRO.DrawShapeCtrl.1",
	0x92b77edb,0x2984,0x4ca7,0xb1,0x36,0xe3,0x37,0xa6,0x7c,0xc9,0xbb)

// タイプ ライブラリ ID およびバージョン

IMPLEMENT_OLETYPELIB(CDrawShapeCtrl, _tlid, _wVerMajor, _wVerMinor)

// インターフェイス ID

const IID IID_DDrawShape = {0x9987a44e,0x72fb,0x4e9a,{0xa4,0x7f,0x93,0x6f,0xcf,0xe8,0xdb,0x17}};
const IID IID_DDrawShapeEvents = {0x21a2e7b0,0x8b57,0x408c,{0xbd,0x42,0x9a,0x74,0x96,0xd4,0x3d,0x82}};

// コントロールの種類の情報

static const DWORD _dwDrawShapeOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CDrawShapeCtrl, IDS_DRAWSHAPE, _dwDrawShapeOleMisc)

// CDrawShapeCtrl::CDrawShapeCtrlFactory::UpdateRegistry -
// CDrawShapeCtrl のシステム レジストリ エントリを追加または削除します

BOOL CDrawShapeCtrl::CDrawShapeCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: コントロールが apartment モデルのスレッド処理の規則に従っていることを
	// 確認してください。詳細については MFC のテクニカル ノート 64 を参照してください。
	// アパートメント モデルのスレッド処理の規則に従わないコントロールの場合は、6 番目
	// のパラメーターを以下のように変更してください。
	// afxRegApartmentThreading を 0 に設定します。

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_DRAWSHAPE,
			IDB_DRAWSHAPE,
			afxRegApartmentThreading,
			_dwDrawShapeOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


// CDrawShapeCtrl::CDrawShapeCtrl - コンストラクター

CDrawShapeCtrl::CDrawShapeCtrl()
{
	InitializeIIDs(&IID_DDrawShape, &IID_DDrawShapeEvents);
	// TODO: この位置にコントロールのインスタンス データの初期化処理を追加してください
}

// CDrawShapeCtrl::~CDrawShapeCtrl - デストラクタ―

CDrawShapeCtrl::~CDrawShapeCtrl()
{
	// TODO: この位置にコントロールのインスタンス データの後処理用のコードを追加してください
}

// CDrawShapeCtrl::OnDraw - 描画関数

void CDrawShapeCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */)
{
	if (!pdc)
		return;

	// TODO: 以下のコードを描画用のコードに置き換えてください
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// CDrawShapeCtrl::DoPropExchange - 永続性のサポート

void CDrawShapeCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 永続属性を持つ各カスタム プロパティ用の PX_ 関数を呼び出します。
}


// CDrawShapeCtrl::OnResetState - コントロールを既定の状態にリセットします

void CDrawShapeCtrl::OnResetState()
{
	COleControl::OnResetState();  // DoPropExchange を呼び出して既定値にリセット

	// TODO: この位置にコントロールの状態をリセットする処理を追加してください
}


// CDrawShapeCtrl メッセージ ハンドラー
