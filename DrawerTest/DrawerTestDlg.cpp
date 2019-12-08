
// DrawerTestDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "DrawerTest.h"
#include "DrawerTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrawerTestDlg ダイアログ



CDrawerTestDlg::CDrawerTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRAWERTEST_DIALOG, pParent)
	, m_pDrawManager(std::make_unique<Drawer::Manager>())
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDrawerTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_PICTURE, m_StPicture);
}

BEGIN_MESSAGE_MAP(CDrawerTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDrawerTestDlg メッセージ ハンドラー

BOOL CDrawerTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	// 初期化処理
	Initialize();

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}


void CDrawerTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。

	// 終了処理
	Terminate();
}


// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CDrawerTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
		m_StPicture.GetClientRect(&rect);
		m_StPicture.GetDC()->BitBlt(0, 0, rect.Width(), rect.Height(), &m_memDC, 0, 0, SRCCOPY);

		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CDrawerTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 初期化処理
void CDrawerTestDlg::Initialize()
{
	// ドラッグ状態をクリア
	m_isDragging = FALSE;
	m_pntDraggingBasePos = POINT{ 0, 0 };

	// コントロールの矩形領域
	CRect rect;
	m_StPicture.GetClientRect(&rect);
	// デバイスコンテキスト取得
	CDC* pDC = m_StPicture.GetDC();
	// メモリデバイスコンテキストを作成
	m_memDC.CreateCompatibleDC(pDC);
	m_memBmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	m_pOldBmp = m_memDC.SelectObject(&m_memBmp);
	// バックグラウンドを変更しないように背景モードを透明に設定
	//   ※点線を描くときなどに影響
	m_memDC.SetBkMode(TRANSPARENT);
	// デバイスコンテキスト解放
	m_StPicture.ReleaseDC(pDC);

	// 通常時カーソル：十字
	SetCursor(::LoadCursor(NULL, IDC_CROSS));

	// 描画管理オブジェクトを作成
	m_pDrawManager->ResetCanvas(&m_memDC, rect);

	m_pDrawManager->SetGridColor(RGB(0, 255, 0));
	m_pDrawManager->SetGridSize(5.0);
	m_pDrawManager->SetIsDrawGrid(true);
	m_pDrawManager->SetOriginSize(20);
	m_pDrawManager->SetOriginColor(RGB(255, 255, 0));
	m_pDrawManager->SetIsDrawOrigin(true);
	m_pDrawManager->SetAxisColor(RGB(255, 255, 255));
	m_pDrawManager->SetAxisScale(10.0);
	m_pDrawManager->SetIsDrawAxis(false);

	// 描画領域初期化
	m_pDrawManager->Clear();
	Redraw();
}

// 終了処理
void CDrawerTestDlg::Terminate()
{
	// メモリデバイスコンテキストの実態を戻す
	m_memDC.SelectObject(m_pOldBmp);
	// ビットマップを削除
	m_memBmp.DeleteObject();
	// メモリデバイスコンテキストを削除
	m_memDC.DeleteDC();
}


void CDrawerTestDlg::Redraw()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ここにディスパッチ ハンドラー コードを追加します

	m_pDrawManager->Draw();

	// 再描画
	InvalidateRect(nullptr, FALSE);
	UpdateWindow();
}
