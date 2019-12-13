using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DrawShapeTest
{
    public partial class formMain : Form
    {
        public formMain()
        {
            InitializeComponent();
        }
 
        private void formMain_Load(object sender, EventArgs e)
        {
            // 初期設定
            Initialize();
        }

        private void btFit_Click(object sender, EventArgs e)
        {
            // フィット
            axDrawShape.Fit(0.8);
        }

        private void cbPan_CheckedChanged(object sender, EventArgs e)
        {
            // パン設定
            axDrawShape.CanMouseDragPan = cbPan.Checked;
        }

        private void cbZoom_CheckedChanged(object sender, EventArgs e)
        {
            // ズーム設定
            axDrawShape.CanMouseWheelZoom = cbZoom.Checked;
        }
        private void clbLayer_SelectedIndexChanged(object sender, EventArgs e)
        {
            // カレントレイヤー変更
            axDrawShape.CurrentLayerNo = clbLayer.SelectedIndex;
        }

        private void btSaveImage_Click(object sender, EventArgs e)
        {
            saveFileDlg.Filter = "Image File(*.bmp;*.png;*.jpg;*.gif)|*.bmp;*.png;*.jpg;*.gif|All File(*.*)|*.*";
            saveFileDlg.FilterIndex = 1;
            saveFileDlg.RestoreDirectory = true;

            // ファイル保存ダイアログを表示する
            if (saveFileDlg.ShowDialog() == DialogResult.OK)
            {
                // 画像ファイルを保存
                axDrawShape.SaveImage(saveFileDlg.FileName);
            }
        }

        private void btCopyImage_Click(object sender, EventArgs e)
        {
            // 画像をクリップボードへコピー
            axDrawShape.CopyImage();
        }

        private void clbLayer_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            // 現在のカレントレイヤーNoを退避
            int currentLayerNo = axDrawShape.CurrentLayerNo;
            // カレントレイヤーを一時的に変更してチェック状態に応じて描画設定を切り替える
            axDrawShape.CurrentLayerNo = e.Index;
            axDrawShape.SetEnableCurrentLayer(e.NewValue == CheckState.Checked);
            // カレントレイヤーNoをもとに戻す
            axDrawShape.CurrentLayerNo = currentLayerNo;
        }

        private void btInsertLayer_Click(object sender, EventArgs e)
        {
            // レイヤーを挿入
            if (axDrawShape.InsertLayer(clbLayer.SelectedIndex))
            {
                // レイヤーリストを更新
                UpdateLayerList();
            }
        }

        private void btClearLayer_Click(object sender, EventArgs e)
        {
            // レイヤーをクリア
            axDrawShape.ClearCurrentLayer();
            // 再描画
            axDrawShape.Redraw();
        }

        private void btDeleteLayer_Click(object sender, EventArgs e)
        {
            // レイヤーを削除
            axDrawShape.DeleteCurrentLayer();
            // レイヤーリストを更新
            UpdateLayerList();
        }

        private void formMain_axDrawShapeMouseMoveEvent(object sender, AxDrawShapeLib._DDrawShapeEvents_MouseMoveEvent e)
        {
            // マウスカーソルの描画エリア内のクライアント座標を取得
            Point cursor = axDrawShape.PointToClient(Control.MousePosition);
            // キャンバス座標に変換
            double x = 0, y = 0;
            axDrawShape.ControlToCanvas(cursor.X, cursor.Y, ref x, ref y);
            // 表示
            tbPosition.Text = x.ToString("#####.000") + " , " + y.ToString("#####.000");
        }

        private void cbDrawGrid_CheckedChanged(object sender, EventArgs e)
        {
            // グリッド描画設定
            axDrawShape.IsDrawGrid = cbDrawGrid.Checked;
            // 再描画
            axDrawShape.Redraw();
        }

        private void dbDrawAxis_CheckedChanged(object sender, EventArgs e)
        {
            // 軸描画設定
            axDrawShape.IsDrawAxis = cbDrawAxis.Checked;
            // 再描画
            axDrawShape.Redraw();
        }

        private void cbDrawOrigin_CheckedChanged(object sender, EventArgs e)
        {
            // 原点描画設定
            axDrawShape.IsDrawOrigin = cbDrawOrigin.Checked;
            // 再描画
            axDrawShape.Redraw();
        }

        private void cbDrawArrow_CheckedChanged(object sender, EventArgs e)
        {
            // 矢印描画設定
            axDrawShape.IsDrawArrow = cbDrawArrow.Checked;
            // 再描画
            axDrawShape.Redraw();
        }

        private void dbDrawCenter_CheckedChanged(object sender, EventArgs e)
        {
            // 中心点描画設定
            axDrawShape.IsDrawCenter = cbDrawCenter.Checked;
            // 再描画
            axDrawShape.Redraw();
        }

        private void pbGridColor_Click(object sender, EventArgs e)
        {
            // 現在の色
            colorDlg.Color = axDrawShape.GridColor;
            // 色選択ダイアログ表示
            if (colorDlg.ShowDialog() == DialogResult.OK)
            {
                // 色変更
                axDrawShape.GridColor = colorDlg.Color;
                SetColorPictureBox(pbGridColor, colorDlg.Color);
                // 再描画
                axDrawShape.Redraw();
            }
        }

        private void pbAxisColor_Click(object sender, EventArgs e)
        {
            // 現在の色
            colorDlg.Color = axDrawShape.AxisColor;
            // 色選択ダイアログ表示
            if (colorDlg.ShowDialog() == DialogResult.OK)
            {
                // 色変更
                axDrawShape.AxisColor = colorDlg.Color;
                SetColorPictureBox(pbAxisColor, colorDlg.Color);
                // 再描画
                axDrawShape.Redraw();
            }
        }

        private void pbOriginColor_Click(object sender, EventArgs e)
        {
            // 現在の色
            colorDlg.Color = axDrawShape.OriginColor;
            // 色選択ダイアログ表示
            if (colorDlg.ShowDialog() == DialogResult.OK)
            {
                // 色変更
                axDrawShape.OriginColor = colorDlg.Color;
                SetColorPictureBox(pbOriginColor, colorDlg.Color);
                // 再描画
                axDrawShape.Redraw();
            }
        }

        private void pbBackColor_Click(object sender, EventArgs e)
        {
            // 現在の色
            colorDlg.Color = axDrawShape.CtlBackColor;
            // 色選択ダイアログ表示
            if (colorDlg.ShowDialog() == DialogResult.OK)
            {
                // 色変更
                axDrawShape.CtlBackColor = colorDlg.Color;
                SetColorPictureBox(pbBackColor, colorDlg.Color);
                // 再描画
                axDrawShape.Redraw();
            }
        }

        private Color PenColor { get; set; }
        private void pbPenColor_Click(object sender, EventArgs e)
        {
            // 現在の色
            colorDlg.Color = PenColor;
            // 色選択ダイアログ表示
            if (colorDlg.ShowDialog() == DialogResult.OK)
            {
                // 色変更
                PenColor = colorDlg.Color;
                axDrawShape.ChangePen(0, (int)nudPenWidth.Value, (uint)colorDlg.Color.ToArgb());
                SetColorPictureBox(pbPenColor, colorDlg.Color);
            }
        }

        private void nudPenWidth_ValueChanged(object sender, EventArgs e)
        {
            // ペン幅変更
            axDrawShape.ChangePen(0, (int)nudPenWidth.Value, (uint)PenColor.ToArgb());
        }

        private Color BrushColor { get; set; }
        private void pbBrushColor_Click(object sender, EventArgs e)
        {
            // 現在の色
            colorDlg.Color = BrushColor;
            // 色選択ダイアログ表示
            if (colorDlg.ShowDialog() == DialogResult.OK)
            {
                // 色変更
                BrushColor = colorDlg.Color;
                axDrawShape.ChangeBrush(0, (uint)colorDlg.Color.ToArgb(), 0);
                SetColorPictureBox(pbBrushColor, colorDlg.Color);
            }
        }

        private void cbFunc_SelectedIndexChanged(object sender, EventArgs e)
        {
            // 関数
            int func = cbFunc.SelectedIndex;

            // 有効状態
            lbPointType.Enabled = drawFuncInfos[func].usePointType;
            cbPointType.Enabled = drawFuncInfos[func].usePointType;
            lbArcDirection.Enabled = drawFuncInfos[func].useArcDirection;
            cbArcDirection.Enabled = drawFuncInfos[func].useArcDirection;
            lbFillType.Enabled = drawFuncInfos[func].useFillType;
            cbFillType.Enabled = drawFuncInfos[func].useFillType;
            lbRadius.Enabled = drawFuncInfos[func].useRadius;
            tbRadius.Enabled = drawFuncInfos[func].useRadius;

            // グリッドの行数
            if(drawFuncInfos[func].pointCount != 0)
            {
                nudPointsCount.Minimum = 1;
                nudPointsCount.Value = drawFuncInfos[func].pointCount;
                nudPointsCount.Enabled = false;
            }
            else
            {
                nudPointsCount.Minimum = 3;
                nudPointsCount.Value = 3;
                nudPointsCount.Enabled = true;
            }
            dgvPoints.RowCount = (int)nudPointsCount.Value;
        }

        private void nudPointsCount_ValueChanged(object sender, EventArgs e)
        {
            dgvPoints.RowCount = (int)nudPointsCount.Value;
        }

        private void btRunFunc_Click(object sender, EventArgs e)
        {
            // 種類
            int pointType = cbPointType.SelectedIndex;
            int arcDirection = cbArcDirection.SelectedIndex;
            int fillType = cbFillType.SelectedIndex;
            // 半径
            double radius = 0.0;
            double.TryParse(tbRadius.Text, out radius);
            // 点群
            Point[] points = new Point[(int)nudPointsCount.Value];
            for(int row = 0; row < points.Length; row++)
            {
                int x, y;
                int.TryParse(dgvPoints[0, row].Value.ToString(), out x);
                int.TryParse(dgvPoints[1, row].Value.ToString(), out y);
                points[row].X = x;
                points[row].Y = y;
            }

            // 関数
            int func = cbFunc.SelectedIndex;
            switch (func)
            {
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            default:
                break;
            }
        }

        /// <summary>
        /// 描画関数の情報
        /// </summary>
        public struct DrawFuncInfo
        {
            // 名称
            public string name { get; set; }
            // PointType使用可否
            public Boolean usePointType { get; set; }
            // ArcDirection使用可否
            public Boolean useArcDirection { get; set; }
            // FillType使用可否
            public Boolean useFillType { get; set; }
            // 半径の使用可否
            public Boolean useRadius { get; set; }
            // 点群の使用数
            public int pointCount { get; set; }
        }

        /// <summary>
        /// 関数情報配列
        /// </summary>
        private DrawFuncInfo[] drawFuncInfos = new DrawFuncInfo[]
        {
            new DrawFuncInfo {
                name = "AddLine",
                usePointType = false,
                useArcDirection = false,
                useFillType = false,
                useRadius = false,
                pointCount = 2
            },
            new DrawFuncInfo {
                name = "AddInfiniteLine2Point",
                usePointType = false,
                useArcDirection = false,
                useFillType = false,
                useRadius = false,
                pointCount = 2
            },
            new DrawFuncInfo {
                name = "AddPoint",
                usePointType = true,
                useArcDirection = false,
                useFillType = false,
                useRadius = false,
                pointCount = 1
            },
            new DrawFuncInfo {
                name = "AddArc",
                usePointType = false,
                useArcDirection = true,
                useFillType = false,
                useRadius = false,
                pointCount = 3
            },
            new DrawFuncInfo {
                name = "AddCircle",
                usePointType = false,
                useArcDirection = false,
                useFillType = true,
                useRadius = true,
                pointCount = 1
            },
            new DrawFuncInfo {
                name = "AddPolygon",
                usePointType = false,
                useArcDirection = false,
                useFillType = true,
                useRadius = false,
                pointCount = 0
            },
            new DrawFuncInfo {
                name = "AddSector",
                usePointType = false,
                useArcDirection = true,
                useFillType = true,
                useRadius = true,
                pointCount = 3
            },
            new DrawFuncInfo {
                name = "AddOrigin",
                usePointType = false,
                useArcDirection = false,
                useFillType = false,
                useRadius = false,
                pointCount = 1
            },
            new DrawFuncInfo {
                name = "AddAxis",
                usePointType = false,
                useArcDirection = false,
                useFillType = false,
                useRadius = false,
                pointCount = 1
            },
        };

        /// <summary>
        /// 初期設定
        /// </summary>
        void Initialize()
        {
            // パン・ズームを表示
            cbPan.Checked = axDrawShape.CanMouseDragPan;
            cbZoom.Checked = axDrawShape.CanMouseWheelZoom;
            // レイヤーリストを表示
            UpdateLayerList();
            // 描画エリアのマウス移動イベントハンドラを設定
            axDrawShape.MouseMoveEvent += new AxDrawShapeLib._DDrawShapeEvents_MouseMoveEventHandler(formMain_axDrawShapeMouseMoveEvent);
            // ピクチャーボックスに色を表示
            SetColorPictureBox(pbGridColor, axDrawShape.GridColor);
            SetColorPictureBox(pbAxisColor, axDrawShape.AxisColor);
            SetColorPictureBox(pbOriginColor, axDrawShape.OriginColor);
            SetColorPictureBox(pbBackColor, axDrawShape.CtlBackColor);
            PenColor = Color.White;
            SetColorPictureBox(pbPenColor, PenColor);
            BrushColor = Color.White;
            SetColorPictureBox(pbBrushColor, BrushColor);
            // ペン幅
            nudPenWidth.Value = 1;
            // 関数リスト
            foreach (var f in drawFuncInfos)
            {
                cbFunc.Items.Add(f.name);
            }
            cbFunc.SelectedIndex = 0;

            // 上下選択
            cbPointType.Items.Add("Pixel");
            cbPointType.Items.Add("Large");
            cbPointType.Items.Add("Triangle");
            cbPointType.SelectedIndex = 0;
            cbArcDirection.Items.Add("Left");
            cbArcDirection.Items.Add("Right");
            cbArcDirection.SelectedIndex = 0;
            cbFillType.Items.Add("NoFill");
            cbFillType.Items.Add("Fill");
            cbFillType.SelectedIndex = 0;

            // 点設定グリッド
            dgvPoints.AllowUserToAddRows = false;
            dgvPoints.RowHeadersVisible = false;
            dgvPoints.AllowUserToResizeColumns = false;
            dgvPoints.AllowUserToResizeRows = false;
            dgvPoints.ColumnCount = 2;
            dgvPoints.Columns[0].HeaderText = "X";
            dgvPoints.Columns[1].HeaderText = "Y";
            dgvPoints.Columns[0].Width = dgvPoints.Width / 2 - 10;
            dgvPoints.Columns[1].Width = dgvPoints.Width / 2 - 10;
            dgvPoints.Columns[0].SortMode = DataGridViewColumnSortMode.NotSortable;
            dgvPoints.Columns[1].SortMode = DataGridViewColumnSortMode.NotSortable;
        }
        /// <summary>
        /// レイヤーリストを更新
        /// </summary>
        private void UpdateLayerList()
        {
            // リストをクリア
            clbLayer.Items.Clear();

            // 現在のカレントレイヤーNoを退避
            int currentLayerNo = axDrawShape.CurrentLayerNo;
            // 全レイヤーをループ
            for (int i = 0; i < axDrawShape.LayerCount; i++)
            {
                // リストアイテムを追加
                string name = "Layer[" + i.ToString() + "]";
                clbLayer.Items.Add(name, false);

                // 一時的にカレントレイヤーを変更してチェック状態をリストアイテムに反映
                axDrawShape.CurrentLayerNo = i;
                clbLayer.SetItemChecked(i, axDrawShape.GetEnableCurrentLayer());
            }
            // 元のカレントレイヤーNoに戻してリストアイテムを選択
            axDrawShape.CurrentLayerNo = currentLayerNo;
            clbLayer.SelectedIndex = currentLayerNo;
        }

        /// <summary>
        /// ピクチャーボックスに色を設定
        /// </summary>
        /// <param name="pb"></param>
        /// <param name="color"></param>
        private void SetColorPictureBox(PictureBox pb, Color color)
        {
            Bitmap bitmap = new Bitmap(pb.Width, pb.Height);
            Graphics g = Graphics.FromImage(bitmap);
            Brush brush = new SolidBrush(color);
            g.FillRectangle(brush, 0, 0, pb.Width, pb.Height);
            pb.Image = bitmap;
        }
    }
}
