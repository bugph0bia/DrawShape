
// DrawerTestDlg.h : ヘッダー ファイル
//

#pragma once

#include "../DrawShape/Drawer.h"

// CDrawerTestDlg ダイアログ
class CDrawerTestDlg : public CDialogEx
{
// コンストラクション
public:
	CDrawerTestDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAWERTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_StPicture;
	afx_msg void OnDestroy();

	// メモリDC
	CDC m_memDC;
	// メモリDCの実体のビットマップ
	CBitmap m_memBmp;
	CBitmap* m_pOldBmp;

	// 描画管理オブジェクト
	std::unique_ptr<Drawer::Manager> m_pDrawManager;

	// ドラッグフラグ
	BOOL m_isDragging;
	// ドラッグ中基準点
	CPoint m_pntDraggingBasePos;

	// 初期化処理
	void Initialize();
	// 終了処理
	void Terminate();

	void Redraw();
};

