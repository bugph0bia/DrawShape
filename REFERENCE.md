リファレンス
===

用語
---
| 用語             | 説明                                                                                                   |
|------------------|--------------------------------------------------------------------------------------------------------|
| コントロール座標 | ウィンドウ（フォーム）のDrawShapeコントロール上の座標のこと。単位はPixel。座標の＋方向はXが右、Yが下。 |
| キャンバス座標   | DrawShpae内部のコンテンツの座標のこと。実数値。座標の＋方向はXが右、Yが上。                            |


機能概要
---
複数の形状を好きな位置に配置して描画することができる。

### 基本機能
- 内部では『コントロール(1)－(N)レイヤー(1)－(n)形状』の関係でデータが管理される。
- コントロールには、背景、グリッド、基準軸、基準原点が存在し、それぞれに色や大きさを設定することができる。
- 描画中のデータはファイルへの保存／読み込みが可能。ファイルはJSONフォーマットを用いる。
- マウスを用いて、描画内容のズーム／パンを行うことが可能。
- 描画内容は、画像ファイルへの保存やクリップボードへのコピーが可能。
- コントロール座標とキャンバス座標の相互変換が可能。マウスカーソル位置から座標値を算出する際などに利用できる。

### レイヤー
- 形状を登録するためのレイヤーの追加／削除が可能。
- レイヤーは 0 から始まる番号で管理され、描画は昇順に行われる。（番号が大きいレイヤーが上に重なって描画される）
- レイヤーは描画可否の設定が可能。
- レイヤーは最低でも１枚存在する。


### ペンとブラシ
- 形状の描画にはペンとブラシが用いられる。
- ペンは輪郭線を描画する際に用いられる。色、太さ、スタイルの情報が存在する。
- ブラシは塗りつぶしを描画する際に用いられる。色の情報が存在する。
- レイヤーに形状を登録する際に設定されているペンとブラシが、その形状の描画に使用される。

### 形状
- 以下の形状を登録可能。
    - 点
    - 直線
    - 無限直線
    - 円弧
    - 円（塗りつぶし可）
    - 多角形（塗りつぶし可）
    - 扇形（塗りつぶし可）
    - 追加軸
    - 追加原点


プロパティ
---
### BackColor
キャンバスの背景色(RGB)を取得／設定する。

- C++
```cpp
COLORREF GetBackColor();
void SetBackColor(COLORREF propVal);
```
- C#
```cs
public Color BackColor { get; set; }
```

### GridColor
グリッドの背景色(RGB)を取得／設定する。

- C++
```cpp
COLORREF GetGridColor();
void SetGridColor(COLORREF propVal);
```
- C#
```cs
public Color GridColor { get; set; }
```

### GridSize
グリッドのサイズ(キャンバス座標)を取得／設定する。

- C++
```cpp
double GetGridSize();
void SetGridSize(double propVal);
```
- C#
```cs
public double GridSize { get; set; }
```

### OriginColor
基準原点の背景色(RGB)を取得／設定する。

- C++
```cpp
COLORREF GetOriginColor();
void SetOriginColor(COLORREF propVal);
```
- C#
```cs
public Color OriginColor { get; set; }
```

### OriginSize
基準原点のサイズ(コントロール座標)を取得／設定する。

- C++
```cpp
double GetOriginSize();
void SetOriginSize(double propVal);
```
- C#
```cs
public double OriginSize { get; set; }
```

### AxisColor
基準軸の背景色(RGB)を取得／設定する。

- C++
```cpp
COLORREF GetAxisColor();
void SetAxisColor(COLORREF propVal);
```
- C#
```cs
public Color AxisColor { get; set; }
```

### AxisScale
基準軸のスケール(キャンバス座標)を取得／設定する。

- C++
```cpp
double GetAxisScale();
void SetAxisScale(double propVal);
```
- C#
```cs
public double AxisScale { get; set; }
```

### IsDrawGrid
グリッドの描画可否を取得／設定する。

- C++
```cpp
BOOL GetIsDrawGrid();
void SetIsDrawGrid(BOOL propVal);
```
- C#
```cs
public bool IsDrawGrid { get; set; }
```

### IsDrawOrigin
原点の描画可否を取得／設定する。

- C++
```cpp
BOOL GetIsDrawOrigin();
void SetIsDrawOrigin(BOOL propVal);
```
- C#
```cs
public bool IsDrawOrigin { get; set; }
```

#### コメント
基準原点と追加原点の両方に適用される。

### IsDrawAxis
軸の描画可否を取得／設定する。

- C++
```cpp
BOOL GetIsDrawAxis();
void SetIsDrawAxis(BOOL propVal);
```
- C#
```cs
public bool IsDrawAxis { get; set; }
```

#### コメント
基準軸と追加軸の両方に適用される。

### IsDrawArrow
形状への矢印の描画可否を取得／設定する。

- C++
```cpp
BOOL GetIsDrawArrow();
void SetIsDrawArrow(BOOL propVal);
```
- C#
```cs
public bool IsDrawArrow { get; set; }
```

#### コメント
矢印の描画は以下の形状にのみ適用される。
- 直線
- 円弧
- 多角形

### IsDrawCenter
円の中心点の描画可否を取得／設定する。

- C++
```cpp
BOOL GetIsDrawCenter();
void SetIsDrawCenter(BOOL propVal);
```
- C#
```cs
public bool IsDrawCenter { get; set; }
```

#### コメント
中心点の描画は以下の形状にのみ適用される。
- 円弧
- 円
- 扇形
中心点は三角点で描画する。

### CurrentLayerNo
カレントレイヤーの番号を取得／設定する。

- C++
```cpp
long GetCurrentLayerNo();
void SetCurrentLayerNo(long propVal);
```
- C#
```cs
public long CurrentLayerNo { get; set; }
```

#### コメント
レイヤー番号の有効範囲は、0 ～ (レイヤー枚数 - 1)。

### LayerCount
現在のレイヤー枚数を取得する。

- C++
```cpp
long GetLayerCount();
```
- C#
```cs
public long LayerCount { get; private set; }
```

### CurrentPenColor
カレントのペンの色(RGB)を取得／設定する。

- C++
```cpp
COLORREF GetCurrentPenColor();
void SetCurrentPenColor(COLORREF propVal);
```
- C#
```cs
public Color CurrentPenColor { get; set; }
```

#### コメント
Win32PIの `LOGPEN` 構造体の仕様に従う。  
デフォルトは白色。  

### CurrentPenWidth
カレントのペンの太さを取得／設定する。

- C++
```cpp
long GetCurrentPenWidth();
void SetCurrentPenWidth(long propVal);
```
- C#
```cs
public long CurrentPenWidth { get; set; }
```

#### コメント
Win32PIの `LOGPEN` 構造体の仕様に従う。  
デフォルトは 1px。  
太さを 2px 以上に設定できるのは、ペンのスタイルが `PS_SOLID` `PS_INSIDERFRAME` のいずれかの場合のみである。

### CurrentPenStyle
カレントのペンのスタイルを取得／設定する。

- C++
```cpp
long GetCurrentPenStyle();
void SetCurrentPenStyle(long propVal);
```
- C#
```cs
public long CurrentPenStyle { get; set; }
```

#### コメント
Win32PIの `LOGPEN` 構造体の仕様に従う。  
以下のいずれかの値を設定可能。  

| 定数             | 値 | 説明       |
|------------------|----|------------|
| `PS_SOLID`       | 0  | 実線       |
| `PS_DASH`        | 1  | 破線       |
| `PS_DOT`         | 2  | 点線       |
| `PS_DASHDOT`     | 3  | 一点鎖線   |
| `PS_DASHDOTDOT`  | 4  | 二点鎖線   |
| `PS_NULL`        | 5  | 描画しない |
| `PS_INSIDEFRAME` | 6  | 実線       |

デフォルトは `PS_SOLID` 。

### CurrentBrushColor
カレントのブラシの色(RGB)を取得／設定する。

- C++
```cpp
COLORREF GetCurrentBrushColor();
void SetCurrentBrushColor(COLORREF propVal);
```
- C#
```cs
public Color CurrentBrushColor { get; set; }
```

#### コメント
Win32PIの `LOGBRUSH` 構造体の仕様に従う。  
デフォルトは白色。  

### CanMouseDragPan
マウスドラッグによるパン（描画内容の上下左右移動）の実行可否を取得／設定する。

- C++
```cpp
BOOL GetCanMouseDragPan();
void SetCanMouseDragPan(BOOL propVal);
```
- C#
```cs
public bool CanMouseDragPan { get; set; }
```

### CanMouseWheelZoom
マウスホイールによるズーム（描画内容の拡大縮小）の実行可否を取得／設定する。

- C++
```cpp
BOOL GetCanMouseWheelZoom();
void SetCanMouseWheelZoom(BOOL propVal);
```
- C#
```cs
public bool CanMouseWheelZoom { get; set; }
```


メソッド
---
### Redraw
全レイヤーの全形状を再描画する。

- C++
```cpp
void Redraw();
```
- C#
```cs
public void Redraw();
```

#### コメント
DrawShapeコントロールでは、プロパティやメソッドにより内部状態の変更しても自動的に再描画が行われない。これは、複数の形状を一度に登録する場合に毎回再描画されてしまうと非効率となるためである。そのような場合は、最後に本メソッドを一度だけ呼び出す必要がある。  

### Clear
DrawShapeコントロールを初期状態に戻す。

- C++
```cpp
void Clear();
```
- C#
```cs
public void Clear();
```

#### コメント
以下を実行する。
- 全レイヤーを削除する。
- カレントのペンとブラシをデフォルト状態に戻す。

### SaveImage
現在の描画内容を画像ファイルに保存する。

- C++
```cpp
BOOL SaveImage(const char* filePath);
```
- C#
```cs
public bool SaveImage(string filePath);
```

#### パラメータ
- filePath
    - ファイルパス。

#### 戻り値
成功した場合は 1 または true 。失敗した場合は 0 または false 。

#### コメント
画像サイズはDrawShapeコントロールの現在のサイズとなる。
ファイルパスに指定した拡張子によって画像フォーマットが自動的に判断される。指定可能な拡張子は BMP/PNG/JPEG/GIF のいずれか。  
既に同名のファイルが存在した場合は上書きする。

### CopyImage
現在の描画内容をクリップボードにコピーする。

- C++
```cpp
BOOL CopyImage();
```
- C#
```cs
public bool CopyImage();
```

#### 戻り値
成功した場合は 1 または true 。失敗した場合は 0 または false 。

#### コメント
画像サイズはDrawShapeコントロールの現在のサイズとなる。

### SaveContents
登録されているレイヤーおよび形状をファイルに保存する。

- C++
```cpp
BOOL SaveContents(const char* filePath);
```
- C#
```cs
public bool SaveContents(string filePath);
```

#### パラメータ
- filePath
    - ファイルパス。

#### 戻り値
成功した場合は 1 または true 。失敗した場合は 0 または false 。

#### コメント
JSONフォーマットを使用する。

### LoadContents
ファイルからレイヤーおよび形状を読み込んで登録する。

- C++
```cpp
BOOL LoadContents(const char* filePath);
```
- C#
```cs
public bool LoadContents(string filePath);
```

#### パラメータ
- filePath
    - ファイルパス。

#### 戻り値
成功した場合は 1 または true 。失敗した場合は 0 または false 。

#### コメント
JSONフォーマットを使用する。  
読み込み前に登録されていたレイヤーおよび形状は全て削除される。

### CanvasToControl
座標値をキャンバス座標からコントロール座標へ変換する。

- C++
```cpp
void CanvasToControl(double canvasX, double canvasY, long* pCtrlX, long* pCtrlY);
```
- C#
```cs
public void CanvasToControl(double canvasX, double canvasY, ref long pCtrlX, ref long pCtrlY);
```

#### パラメータ
- canvasX
    - キャンバス座標のX。
- canvasY
    - キャンバス座標のY。
- pCtrlX
    - （出力）コントロール座標のX。
- pCtrlY
    - （出力）コントロール座標のY。

#### コメント
現在のパン／ズーム状態に応じた変換が行われる。

### ControlToCanvas
座標値をコントロール座標からキャンバス座標へ変換する。

- C++
```cpp
void ControlToCanvas(long ctrlX, long ctrlY, double* pCanvasX, double* pCanvasY);
```
- C#
```cs
public void ControlToCanvas(long ctrlX, long ctrlY, ref double pCanvasX, ref double pCanvasY);
```

#### パラメータ
- ctrlX
    - コントロール座標のX。
- ctrlY
    - コントロール座標のY。
- pCanvasX
    - （出力）キャンバス座標のX。
- pCanvasY
    - （出力）キャンバス座標のY。

#### コメント
現在のパン／ズーム状態に応じた変換が行われる。  
例として、現在のカーソル位置（コントロール座標）をキャンバス座標に変換するといった用途に利用可能。

### InsertLayer
新しいレイヤーを挿入する。

- C++
```cpp
BOOL InsertLayer(long insertNo);
```
- C#
```cs
public bool InsertLayer(long insertNo);
```

#### パラメータ
- insertNo
    - 挿入位置。0 ～ 挿入前のレイヤー枚数、または -1 を指定可能。

#### 戻り値
成功した場合は 1 または true 。失敗した場合は 0 または false 。

#### コメント
挿入位置の番号に新しいレイヤーを挿入する。その位置以降にレイヤーが存在していた場合、それらの番号は 1 加算される。  
挿入位置に -1 を指定した場合は、末尾への挿入となる。（挿入前のレイヤー枚数を指定した場合と同様）  
挿入に成功した場合、挿入したレイヤーがカレントレイヤーとなる。  

### ClearCurrentLayer
カレントレイヤーに登録されている全形状を削除する。

- C++
```cpp
void ClearCurrentLayer();
```
- C#
```cs
public void ClearCurrentLayer();
```

### DeleteCurrentLayer
カレントレイヤーを削除する。

- C++
```cpp
long DeleteCurrentLayer();
```
- C#
```cs
public long DeleteCurrentLayer();
```

#### 戻り値
削除後のレイヤー枚数。

#### コメント
末尾のレイヤーを削除した場合、１つ前のレイヤーがカレントレイヤーとなる。  
末尾以外のレイヤーを削除した場合、レイヤー番号は前詰めされ、１つ次のレイヤーが新たなカレントレイヤーとなる。  
削除前にレイヤーが１枚しか存在しなかった場合は、削除後に自動的に新しいレイヤーが１枚挿入され、それがカレントレイヤーとなる。  


----

ここまで記載。

----


イベント
---
### Event
概要概要概要概要概要概要概要概要。

- C++
```cpp
void OnEvent(int a, double b);
```
- C#
```cs
public class _DDrawShapeEvents_CursorMoveEvent
{
    public int ctrlX;
    public int ctrlY;
    public double canvasX;
    public double canvasY;

    public _DDrawShapeEvents_CursorMoveEvent(int ctrlX, int ctrlY, double canvasX, double canvasY);
}
public delegate void _DDrawShapeEvents_CursorMoveEventHandler(object sender, _DDrawShapeEvents_CursorMoveEvent e);
public event _DDrawShapeEvents_CursorMoveEventHandler CursorMove;
```

#### パラメータ
- xxx
    - 説明説明説明説明説明。
- yyy
    - 説明説明説明説明説明。

