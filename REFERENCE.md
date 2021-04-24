Reference
===

[English Page](./REFERENCE.md)

Overview
---

### basic functionality
- The controller has a background, a grid, a reference axis, and a reference origin, and the color and size of each can be set.
- The data being drawn can be saved/loaded to a file. The file uses the JSON format.
- Zooming and panning of the drawing contents is possible using the mouse.
- The drawing contents can be saved to an image file or copied to the clipboard.
- Mutual conversion between the control coordinate system and the canvas coordinate system is possible. This can be used when calculating coordinate values from the mouse cursor position.
- Internally, data is managed in the relationship of "Control (1) : (N) Layer (1) : (n) Shape".

### Layer
- Layers can be added and deleted to register shapes.
- Layers are managed by numbers starting from 0, and drawing is done in ascending order. (Layers with higher numbers are drawn on top of each other.)
- Layers can be set to be drawable or not.
- At least one layer must exist.


### Pen and brush
- Pen and brush are used to draw shapes.
- Pen is used to draw outlines. It has information about color, thickness, and style.
- Brush is used to draw fills. It has color information.
- Pen and brush that are set when registering a shape to a layer will be used to draw the shape.

### Shape
- The following shapes can be registered.
    - Point
    - Line
    - Infinite line
    - Arc
    - Circle (fillable)
    - Polygon (fillable)
    - Sector (fillable)
    - Additional axis
    - Additional origin


Terminology
---
| Terminology               | Description                                                                                                                                                        |
| ------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| Control coordinate system | The coordinate system on the DrawShape control of a window (form). The unit is Pixel. X coordinate is + for right direction, Y coordinate is + for down direction. |
| Canvas coordinate system  | The coordinate system of the content inside DrawShpae. Real values. X coordinate is + for right direction, Y coordinate is + for up direction.                     |


Properties
---
### BackColor
Get/Set the background color (RGB) of the canvas.

- C++
```cpp
COLORREF GetBackColor();
void SetBackColor(COLORREF propVal);
```
- C#
```cs
public Color CtlBackColor { get; set; }
```

---

### GridColor
Get/Set the background color (RGB) of the grid.

- C++
```cpp
COLORREF GetGridColor();
void SetGridColor(COLORREF propVal);
```
- C#
```cs
public Color GridColor { get; set; }
```

---

### GridSize
Get/Set the size of the grid (canvas coordinate system).

- C++
```cpp
double GetGridSize();
void SetGridSize(double propVal);
```
- C#
```cs
public double GridSize { get; set; }
```

---

### OriginColor
Get/Set the background color (RGB) of the reference origin.

- C++
```cpp
COLORREF GetOriginColor();
void SetOriginColor(COLORREF propVal);
```
- C#
```cs
public Color OriginColor { get; set; }
```

---

### OriginSize
Get/Set the size of the reference origin (control coordinate system).

- C++
```cpp
double GetOriginSize();
void SetOriginSize(double propVal);
```
- C#
```cs
public double OriginSize { get; set; }
```

---

### AxisColor
Get/Set the background color (RGB) of the reference axis.

- C++
```cpp
COLORREF GetAxisColor();
void SetAxisColor(COLORREF propVal);
```
- C#
```cs
public Color AxisColor { get; set; }
```

---

### AxisScale
Get/Set the scale of the reference axis (canvas coordinate system).

- C++
```cpp
double GetAxisScale();
void SetAxisScale(double propVal);
```
- C#
```cs
public double AxisScale { get; set; }
```

---

### IsDrawGrid
Get/Set the grid drawing availability.

- C++
```cpp
BOOL GetIsDrawGrid();
void SetIsDrawGrid(BOOL propVal);
```
- C#
```cs
public bool IsDrawGrid { get; set; }
```

---

### IsDrawOrigin
Get/Set the drawability of the origin point.

- C++
```cpp
BOOL GetIsDrawOrigin();
void SetIsDrawOrigin(BOOL propVal);
```
- C#
```cs
public bool IsDrawOrigin { get; set; }
```

#### Comment
Applies to both the standard origin and the additional origin.

---

### IsDrawAxis
Get/Set the drawability of the axis.

- C++
```cpp
BOOL GetIsDrawAxis();
void SetIsDrawAxis(BOOL propVal);
```
- C#
```cs
public bool IsDrawAxis { get; set; }
```

#### Comment
Applies to both the standard axis and the additional axis.

---

### IsDrawArrow
Get/Set whether arrows can be drawn on the shape.

- C++
```cpp
BOOL GetIsDrawArrow();
void SetIsDrawArrow(BOOL propVal);
```
- C#
```cs
public bool IsDrawArrow { get; set; }
```

#### Comment
Arrow drawing is only applicable to the following shapes
- Line
- Arc
- Polygon

---

### IsDrawCenter
Get/Set the possibility of drawing the center point of the circle.

- C++
```cpp
BOOL GetIsDrawCenter();
void SetIsDrawCenter(BOOL propVal);
```
- C#
```cs
public bool IsDrawCenter { get; set; }
```

#### Comment
Center point drawing is only applicable to the following shapes
- Arc
- Circle
- Sector

The center point should be drawn as a triangle.

---

### CurrentLayerNo
Get/Set the number of the current layer.

- C++
```cpp
long GetCurrentLayerNo();
void SetCurrentLayerNo(long propVal);
```
- C#
```cs
public long CurrentLayerNo { get; set; }
```

#### Comment
The valid range of the layer number is from 0 to (number of layers - 1).

---

### LayerCount
Get the current number of layers.

- C++
```cpp
long GetLayerCount();
```
- C#
```cs
public long LayerCount { get; private set; }
```

---

### CurrentPenColor
Get/Set the color (RGB) of the current pen.

- C++
```cpp
COLORREF GetCurrentPenColor();
void SetCurrentPenColor(COLORREF propVal);
```
- C#
```cs
public Color CurrentPenColor { get; set; }
```

#### Comment
Follows the specification of the `LOGPEN` structure of Win32PI.  
Default is white.  

---

### CurrentPenWidth
Get/Set the thickness of the current pen.

- C++
```cpp
long GetCurrentPenWidth();
void SetCurrentPenWidth(long propVal);
```
- C#
```cs
public long CurrentPenWidth { get; set; }
```

#### Comment
Follows the specification of the `LOGPEN` structure of Win32PI.  
Default is 1px.  
The thickness can be set to more than 2px only if the pen style is one of `PS_SOLID` or `PS_INSIDERFRAME`.

---

### CurrentPenStyle
Get/Set the style of the current pen.

- C++
```cpp
long GetCurrentPenStyle();
void SetCurrentPenStyle(long propVal);
```
- C#
```cs
public long CurrentPenStyle { get; set; }
```

#### Comment
Follows the specification of the `LOGPEN` structure of Win32PI.  
One of the following values can be set.  

| Constant         | Value | Description        |
| ---------------- | ----- | ------------------ |
| `PS_SOLID`       | 0     | Solid line         |
| `PS_DASH`        | 1     | Dashed line        |
| `PS_DOT`         | 2     | Dotted line        |
| `PS_DASHDOT`     | 3     | Single-dotted line |
| `PS_DASHDOTDOT`  | 4     | Double-dotted line |
| `PS_NULL`        | 5     | Not drawn          |
| `PS_INSIDEFRAME` | 6     | Solid line         |

Default is `PS_SOLID`.

---

### CurrentBrushColor
Get/Set the color (RGB) of the current brush.

- C++
```cpp
COLORREF GetCurrentBrushColor();
void SetCurrentBrushColor(COLORREF propVal);
```
- C#
```cs
public Color CurrentBrushColor { get; set; }
```

#### Comment
Follows the specification of the `LOGBRUSH` structure of Win32PI.  
Default is white.  

---

### CanMouseDragPan
Get/Set whether or not to execute panning (up/down/left/right movement of drawing contents) by mouse dragging.

- C++
```cpp
BOOL GetCanMouseDragPan();
void SetCanMouseDragPan(BOOL propVal);
```
- C#
```cs
public bool CanMouseDragPan { get; set; }
```

---

### CanMouseWheelZoom
Gets/set whether or not zooming (zooming in/out of the drawing contents) can be performed using the mouse wheel.

- C++
```cpp
BOOL GetCanMouseWheelZoom();
void SetCanMouseWheelZoom(BOOL propVal);
```
- C#
```cs
public bool CanMouseWheelZoom { get; set; }
```

---


Methods
---
### Redraw
Redraw all shapes on all layers.

- C++
```cpp
void Redraw();
```
- C#
```cs
public void Redraw();
```

#### Comment
In the DrawShape control, even if the internal state is changed by a property or method, it is not automatically redrawn. This is because it is inefficient to redraw every time when registering multiple shapes at once. In such a case, it is necessary to call this method only once at the end.  

---

### Clear
Return the DrawShape control to its initial state.

- C++
```cpp
void Clear();
```
- C#
```cs
public void Clear();
```

#### Comment
Do the following
- Delete all layers.
- Restore the current pen and brush to their default state.

---

### SaveImage
Save the current drawing to an image file.

- C++
```cpp
BOOL SaveImage(const char* filePath);
```
- C#
```cs
public bool SaveImage(string filePath);
```

#### Parameters
- filePath
    - File path.

#### Return
If successful, 1 or true, otherwise 0 or false.

#### Comment
The image size will be the current size of the DrawShape control.
The image format is automatically determined by the extension specified in the file path. The extension can be one of BMP/PNG/JPEG/GIF.  
If a file with the same name already exists, it will be overwritten.

---

### CopyImage
Copies the current drawing to the clipboard.

- C++
```cpp
BOOL CopyImage();
```
- C#
```cs
public bool CopyImage();
```

#### Return
If successful, 1 or true, otherwise 0 or false.

#### Comment
The image size will be the current size of the DrawShape control.

---

### SaveContents
Save the registered layers and shapes to a file.

- C++
```cpp
BOOL SaveContents(const char* filePath);
```
- C#
```cs
public bool SaveContents(string filePath);
```

#### Parameters
- filePath
    - ファイルパス。

#### Return
If successful, 1 or true, otherwise 0 or false.

#### Comment
Use the JSON format.

---

### LoadContents
Import and register layers and shapes from files.

- C++
```cpp
BOOL LoadContents(const char* filePath);
```
- C#
```cs
public bool LoadContents(string filePath);
```

#### Parameters
- filePath
    - File path.

#### Return
If successful, 1 or true, otherwise 0 or false.

#### Comment
JSON format is used.  
All layers and shapes that were registered before loading will be deleted.

---

### CanvasToControl
Convert coordinate values from the canvas coordinate system to the control coordinate system.

- C++
```cpp
void CanvasToControl(double canvasX, double canvasY, long* pCtrlX, long* pCtrlY);
```
- C#
```cs
public void CanvasToControl(double canvasX, double canvasY, ref long pCtrlX, ref long pCtrlY);
```

#### Parameters
- canvasX
    - X coordinate (canvas coordinate system).
- canvasY
    - Y coordinate (canvas coordinate system).
- pCtrlX (Output)
    - X coordinate (control coordinate system).
- pCtrlY (Output)
    - Y coordinate (control coordinate system).

#### Comment
The conversion is performed according to the current pan/zoom state.

---

### ControlToCanvas
Convert coordinate values from the control coordinate system to the canvas coordinate system.

- C++
```cpp
void ControlToCanvas(long ctrlX, long ctrlY, double* pCanvasX, double* pCanvasY);
```
- C#
```cs
public void ControlToCanvas(long ctrlX, long ctrlY, ref double pCanvasX, ref double pCanvasY);
```

#### Parameters
- pCtrlX
    - X coordinate (control coordinate system).
- pCtrlY
    - Y coordinate (control coordinate system).
- canvasX (Output)
    - X coordinate (canvas coordinate system).
- canvasY (Output)
    - Y coordinate (canvas coordinate system).

#### Comment
The transformation is performed according to the current pan/zoom state.  
For example, it can be used to convert the current cursor position (control coordinate system) to the canvas coordinate system.

---

### InsertLayer
Insert a new layer.

- C++
```cpp
BOOL InsertLayer(long insertNo);
```
- C#
```cs
public bool InsertLayer(long insertNo);
```

#### Parameters
- insertNo
    - Insertion position. 0 to the number of layers before insertion, or -1 can be specified.

#### Return
If successful, 1 or true, otherwise 0 or false.

#### Comment
Inserts a new layer at the insertion position number. If layers exist after the insertion position, their numbers will be added by 1.  
If -1 is specified for the insertion position, the layer will be inserted at the end. (It is the same as when the number of layers before insertion is specified.  
If the insertion is successful, the inserted layer becomes the current layer.  

---

### ClearCurrentLayer
Delete all shapes registered in the current layer.

- C++
```cpp
void ClearCurrentLayer();
```
- C#
```cs
public void ClearCurrentLayer();
```

---

### DeleteCurrentLayer
Delete the current layer.

- C++
```cpp
long DeleteCurrentLayer();
```
- C#
```cs
public long DeleteCurrentLayer();
```

#### Return
Number of layers after deletion.

#### Comment
If the last layer is deleted, the previous layer becomes the current layer.  
If a layer other than the last layer is deleted, the layer number will be prepended and the next layer will become the new current layer.  
If there was only one layer before deleting, a new layer will be automatically inserted after deleting, and it will become the current layer.  

---

### SetEnableCurrentLayer
Set whether the current layer can be drawn or not.

- C++
```cpp
void SetEnableCurrentLayer(BOOL enable);
```
- C#
```cs
public void SetEnableCurrentLayer(bool enable);
```

#### Parameters
- enable
    - Allows or disallows drawing of the current layer. True if drawing is allowed, false if drawing is prohibited.

---

### GetEnableCurrentLayer
Get whether the current layer is drawable or not.

- C++
```cpp
BOOL GetEnableCurrentLayer();
```
- C#
```cs
public bool GetEnableCurrentLayer();
```

#### Return
Whether or not the current layer can be drawn. True if drawing is allowed, false if drawing is not allowed.

---

### Zoom
Zoom in/out on the drawing.

- C++
```cpp
BOOL Zoom(double coef, long ctrlBaseX, long ctrlBaseY);
```
- C#
```cs
public bool Zoom(double coef, long ctrlBaseX, long ctrlBaseY);
```

#### Parameters
- coef
    - Scaling factor, greater than 1.0 means zoom in, less than 1.0 means zoom out.
- ctrlBaseX
    - X coordinate (control coordinate system) to use for scaling.
- ctrlBaseY
    - Y coordinate (control coordinate system) to use for scaling.

#### Return
If successful, 1 or true, otherwise 0 or false.

---

### Pan
Pan (move up, down, left, right) the drawing contents.

- C++
```cpp
BOOL Pan(long ctrlMoveX, long ctrlMoveY);
```
- C#
```cs
public bool Pan(long ctrlMoveX, long ctrlMoveY);
```

#### Parameters
- ctrlMoveX
    - Amount of movement in the X direction (control coordinate system).
- ctrlMoveY
    - Amount of movement in the Y direction (control coordinate system).

#### Return
If successful, 1 or true, otherwise 0 or false.

---

### Fit
Automatically zooms and pans so that all the shapes to be drawn fit into the drawing area.

- C++
```cpp
void Fit(double shapeOccupancy);
```
- C#
```cs
public void Fit(double shapeOccupancy);
```

#### Parameters
- shapeOccupancy
    - The percentage of the entire drawing area that the shape occupies when fitted.

#### Comment
The percentage specified by `shapeOccupancy` is applied to the one of the up/down and left/right directions where the margins are smaller when fitted.  
For the other direction, the shape may occupy a smaller percentage of the drawing area.  

---

### AddLine
Add a line to the current layer.

- C++
```cpp
BOOL AddLine(double sx, double sy, double ex, double ey);
```
- C#
```cs
public bool AddLine(double sx, double sy, double ex, double ey);
```

#### Parameters
- sx
    - Start point X (canvas coordinate system).
- sy
    - Start point Y (canvas coordinate system).
- ex
    - End point X (canvas coordinate system).
- ey
    - End point Y (canvas coordinate system).

#### Return
If successful, 1 or true, otherwise 0 or false.

---

### AddInfiniteLine2Point
Add an infinite line to the current layer in a way that allows you to specify any two points.

- C++
```cpp
BOOL AddInfiniteLine2Point(double sx, double sy, double ex, double ey);
```
- C#
```cs
public bool AddInfiniteLine2Point(double sx, double sy, double ex, double ey);
```

#### Parameters
- sx
    - Start point X (canvas coordinate system).
- sy
    - Start point Y (canvas coordinate system).
- ex
    - End point X (canvas coordinate system).
- ey
    - End point Y (canvas coordinate system).

#### Return
If successful, 1 or true, otherwise 0 or false.

---

### AddInfiniteLine1PointAngle
Add an infinite line to the current layer in a way that specifies any one point and angle.

- C++
```cpp
BOOL AddInfiniteLine1PointAngle(double x, double y, double angle);
```
- C#
```cs
public bool AddInfiniteLine1PointAngle(double x, double y, double angle);
```

#### Parameters
- x
    - Point X (canvas coordinate system).
- y
    - Point Y (canvas coordinate system).
- angle
    - Angle (in radians)

#### Return
If successful, 1 or true, otherwise 0 or false.

---

### AddPoint
Add a point to the current layer.

- C++
```cpp
BOOL AddPoint(double x, double y, long type);
```
- C#
```cs
public bool AddPoint(double x, double y, long type);
```

#### Parameters
- x
    - Point X (canvas coordinate system).
- y
    - Point Y (canvas coordinate system).
- type
    - Point species. It shall be one of the following integer values. If a value out of the range is given, it will not fail, but the behavior will be undefined.
        | Value | Point species      |
        | ----- | ------------------ |
        | 0     | Normal dots (.)    |
        | 1     | Large dots (●)     |
        | 2     | Triangle point (△) |

#### Return
If successful, 1 or true, otherwise 0 or false.

---

### AddArc
Add an arc to the current layer.

- C++
```cpp
BOOL AddArc(double sx, double sy, double ex, double ey, double cx, double cy, BOOL left);
```
- C#
```cs
public bool AddArc(double sx, double sy, double ex, double ey, double cx, double cy, bool left);
```

#### Parameters
- sx
    - Start point X (canvas coordinate system).
- sy
    - Start point Y (canvas coordinate system).
- ex
    - End point X (canvas coordinate system).
- ey
    - End point Y (canvas coordinate system).
- cx
    - Center point X (canvas coordinate system).
- cy
    - Center point Y (canvas coordinate system).
- left
    - Arc direction. If true, the arc will be leftward (counterclockwise), otherwise it will be rightward (clockwise).

#### Return
If successful, 1 or true, otherwise 0 or false.

#### Comment
If the specified coordinates are geometrically inconsistent as the constituent points of the arc, it will fail.

---

### AddCircle
Add a circle to the current layer.

- C++
```cpp
BOOL AddCircle(double cx, double cy, double radius, BOOL fill);
```
- C#
```cs
public bool AddCircle(double cx, double cy, double radius, bool fill);
```

#### Parameters
- cx
    - Center point X (canvas coordinate system).
- cy
    - Center point Y (canvas coordinate system).
- radius
    - Radius (canvas coordinate system).
- fill
    - Fill/no fill.

#### Return
If successful, 1 or true, otherwise 0 or false.

---

### AddPolygon
Add a circle to the current layer.

- C++
```cpp
BOOL AddPolygon(SAFEARRAY* pointCoords, long pointsCount, BOOL fill);
```
- C#
```cs
public bool AddPolygon(Array pointCoords, long pointsCount, bool fill);
```

#### Parameters
- pointCoords
    - Point cloud array (canvas coordinate system). Stores values of type double in the order of point 1-X -> point 1-Y -> point 2-X -> ... -> point n-Y.
- pointsCount
    - The number of points to be entered in `pointCoords`; note that each pair of XY coordinates counts as one.
- fill
    - Fill/no fill.

#### Return
If successful, 1 or true, otherwise 0 or false.

#### Comment
If a value other than a double type is stored in `pointCoords`, it will fail.  
Fails if a value less than or equal to 0 is passed to `pointsCount`.  

---

### AddSector
Add a Sector shape to the current layer.

- C++
```cpp
BOOL AddSector(double sx, double sy, double ex, double ey, double cx, double cy, double innerRadius, BOOL left, BOOL fill);
```
- C#
```cs
public bool AddSector(double sx, double sy, double ex, double ey, double cx, double cy, double innerRadius, bool left, bool fill);
```

#### Parameters
- sx
    - Start point X of the outer arc of the sector (canvas coordinate system).
- sy
    - Start point Y of the outer arc of the sector (canvas coordinate system).
- ex
    - End point X of the outer arc of the sector (canvas coordinate system).
- ey
    - End point Y of the outer arc of the sector (canvas coordinate system).
- cx
    - Center point X of the sector (canvas coordinate system).
- cy
    - Center point Y of the sector (canvas coordinate system).
- innerRadius
    - Radius of the inner arc of the sector (canvas coordinate system).
- left
    - Arc direction. If true, the arc will be leftward (counterclockwise), otherwise it will be rightward (clockwise).
- fill
    - Fill/no fill.

#### Return
If successful, 1 or true, otherwise 0 or false.

#### Comment
Fails if the specified coordinates are geometrically inconsistent as the constituent points of a fan.  
Passing 0 for `innnerRadius` will result in a fan with the inside filled.  

---

### AddOrigin
Add an origin to the current layer.

- C++
```cpp
BOOL AddOrigin(double ox, double oy);
```
- C#
```cs
public bool AddOrigin(double ox, double oy);
```

#### Parameters
- ox
    - The coordinate X (canvas coordinate system) to add the origin.
- oy
    - The coordinate Y (canvas coordinate system) to add the origin.

#### Return
If successful, 1 or true, otherwise 0 or false.

#### Comment
This method is used when you want to add an origin other than the reference origin.  
`IsDrawOrigin` property controls whether or not the origin added by this method can be drawn.  
Only the pen can be used for drawing, not the brush.  

---

### AddAxis
Add an axis to the current layer.

- C++
```cpp
BOOL AddAxis(double ox, double oy);
```
- C#
```cs
public bool AddAxis(double ox, double oy);
```

#### Parameters
- ox
    - The coordinate X (canvas coordinate system) to add the axis to.
- oy
    - The coordinate Y (canvas coordinate system) to add the axis to.

#### Return
If successful, 1 or true, otherwise 0 or false.

#### Comment
Use this method when you want to add an axis other than the reference axis.  
`IsDrawAxis` property controls whether or not the axis added by this method can be drawn.  
Only the pen can be used for drawing, not the brush.  

---


Events
---
### CursorMove
Mouse cursor movement event on the DrawShape control.

- C++
```cpp
void OnCursorMove(long ctrlX, long ctrlY, double canvasX, double canvasY);
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

#### Parameters
- ctrlX
    - Mouse cursor position X (control coordinate system).
- ctrlY
    - Mouse cursor position Y (control coordinate system).
- canvasX
    - Mouse cursor position X (canvas coordinate system).
- canvasY
    - Mouse cursor position Y (canvas coordinate system).

#### Comment
(`ctrlX`, `ctrlY`) matches the value obtained from the mouse cursor position using Win32API and converted to the client coordinate system of the DrawShape control.  
(`canvasX`, `canvasY`) will match the value obtained by transforming (`ctrlX`, `ctrlY`) with the ControlToCanvas method.  

---

### LeftClick
A left mouse click event on a DrawShape control.

- C++
```cpp
void OnLeftClick(long ctrlX, long ctrlY, double canvasX, double canvasY);
```
- C#
```cs
public class _DDrawShapeEvents_LeftClickEvent
{
    public int ctrlX;
    public int ctrlY;
    public double canvasX;
    public double canvasY;

    public _DDrawShapeEvents_LeftClickEvent(int ctrlX, int ctrlY, double canvasX, double canvasY);
}
public delegate void _DDrawShapeEvents_LeftClickEventHandler(object sender, _DDrawShapeEvents_LeftClickEvent e);
public event _DDrawShapeEvents_LeftClickEventHandler LeftClick;
```

#### Parameters
- ctrlX
    - Mouse cursor position X (control coordinate system).
- ctrlY
    - Mouse cursor position Y (control coordinate system).
- canvasX
    - Mouse cursor position X (canvas coordinate system).
- canvasY
    - Mouse cursor position Y (canvas coordinate system).

#### Comment
This event will be fired when a left mouse click is made from Down to Up. Note that the event is fired even when dragging is performed.  
(`ctrlX`, `ctrlY`) matches the value obtained from the mouse cursor position using Win32API and converted to the client coordinate system of the DrawShape control.  
(`canvasX`, `canvasY`) will match the value obtained by transforming (`ctrlX`, `ctrlY`) with the ControlToCanvas method.  
