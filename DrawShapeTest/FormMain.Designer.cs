namespace DrawShapeTest
{
    partial class formMain
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージド リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(formMain));
            this.btInsertLayer = new System.Windows.Forms.Button();
            this.btDeleteLayer = new System.Windows.Forms.Button();
            this.gbLayer = new System.Windows.Forms.GroupBox();
            this.clbLayer = new System.Windows.Forms.CheckedListBox();
            this.btClearLayer = new System.Windows.Forms.Button();
            this.colorDlg = new System.Windows.Forms.ColorDialog();
            this.pbBackColor = new System.Windows.Forms.PictureBox();
            this.pbGridColor = new System.Windows.Forms.PictureBox();
            this.pbAxisColor = new System.Windows.Forms.PictureBox();
            this.pbOriginColor = new System.Windows.Forms.PictureBox();
            this.pbPenColor = new System.Windows.Forms.PictureBox();
            this.pbBrushColor = new System.Windows.Forms.PictureBox();
            this.cbDrawArrow = new System.Windows.Forms.CheckBox();
            this.cbDrawCenter = new System.Windows.Forms.CheckBox();
            this.cbDrawGrid = new System.Windows.Forms.CheckBox();
            this.cbDrawAxis = new System.Windows.Forms.CheckBox();
            this.cbDrawOrigin = new System.Windows.Forms.CheckBox();
            this.gbBasicSetting = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.gbPenBrush = new System.Windows.Forms.GroupBox();
            this.lbBrush = new System.Windows.Forms.Label();
            this.lbPen = new System.Windows.Forms.Label();
            this.lbPenStyle = new System.Windows.Forms.Label();
            this.lbPenWidth = new System.Windows.Forms.Label();
            this.nudPenStyle = new System.Windows.Forms.NumericUpDown();
            this.nudPenWidth = new System.Windows.Forms.NumericUpDown();
            this.cbFunc = new System.Windows.Forms.ComboBox();
            this.gbFunc = new System.Windows.Forms.GroupBox();
            this.tbResult = new System.Windows.Forms.TextBox();
            this.tbRadius = new System.Windows.Forms.TextBox();
            this.lbPoint = new System.Windows.Forms.Label();
            this.nudPointsCount = new System.Windows.Forms.NumericUpDown();
            this.lbResult = new System.Windows.Forms.Label();
            this.btRunFunc = new System.Windows.Forms.Button();
            this.lbRadius = new System.Windows.Forms.Label();
            this.lbFillType = new System.Windows.Forms.Label();
            this.lbArcDirection = new System.Windows.Forms.Label();
            this.dgvPoints = new System.Windows.Forms.DataGridView();
            this.lbPointType = new System.Windows.Forms.Label();
            this.cbFillType = new System.Windows.Forms.ComboBox();
            this.cbArcDirection = new System.Windows.Forms.ComboBox();
            this.cbPointType = new System.Windows.Forms.ComboBox();
            this.tbPosition = new System.Windows.Forms.TextBox();
            this.btFit = new System.Windows.Forms.Button();
            this.btCopyImage = new System.Windows.Forms.Button();
            this.btSaveImage = new System.Windows.Forms.Button();
            this.saveFileDlg = new System.Windows.Forms.SaveFileDialog();
            this.cbPan = new System.Windows.Forms.CheckBox();
            this.cbZoom = new System.Windows.Forms.CheckBox();
            this.axDrawShape = new AxDrawShapeLib.AxDrawShape();
            this.tbLastClick = new System.Windows.Forms.TextBox();
            this.lbLastClick = new System.Windows.Forms.Label();
            this.lbCursor = new System.Windows.Forms.Label();
            this.gbLayer.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbBackColor)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbGridColor)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbAxisColor)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbOriginColor)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbPenColor)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbBrushColor)).BeginInit();
            this.gbBasicSetting.SuspendLayout();
            this.gbPenBrush.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudPenStyle)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudPenWidth)).BeginInit();
            this.gbFunc.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudPointsCount)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvPoints)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.axDrawShape)).BeginInit();
            this.SuspendLayout();
            // 
            // btInsertLayer
            // 
            this.btInsertLayer.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btInsertLayer.Location = new System.Drawing.Point(19, 240);
            this.btInsertLayer.Name = "btInsertLayer";
            this.btInsertLayer.Size = new System.Drawing.Size(131, 23);
            this.btInsertLayer.TabIndex = 1;
            this.btInsertLayer.Text = "insert layer";
            this.btInsertLayer.UseVisualStyleBackColor = true;
            this.btInsertLayer.Click += new System.EventHandler(this.btInsertLayer_Click);
            // 
            // btDeleteLayer
            // 
            this.btDeleteLayer.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btDeleteLayer.Location = new System.Drawing.Point(19, 298);
            this.btDeleteLayer.Name = "btDeleteLayer";
            this.btDeleteLayer.Size = new System.Drawing.Size(131, 23);
            this.btDeleteLayer.TabIndex = 3;
            this.btDeleteLayer.Text = "delete layer";
            this.btDeleteLayer.UseVisualStyleBackColor = true;
            this.btDeleteLayer.Click += new System.EventHandler(this.btDeleteLayer_Click);
            // 
            // gbLayer
            // 
            this.gbLayer.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.gbLayer.Controls.Add(this.clbLayer);
            this.gbLayer.Controls.Add(this.btDeleteLayer);
            this.gbLayer.Controls.Add(this.btClearLayer);
            this.gbLayer.Controls.Add(this.btInsertLayer);
            this.gbLayer.Location = new System.Drawing.Point(711, 12);
            this.gbLayer.Name = "gbLayer";
            this.gbLayer.Size = new System.Drawing.Size(169, 336);
            this.gbLayer.TabIndex = 8;
            this.gbLayer.TabStop = false;
            this.gbLayer.Text = "layers";
            // 
            // clbLayer
            // 
            this.clbLayer.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.clbLayer.FormattingEnabled = true;
            this.clbLayer.Location = new System.Drawing.Point(19, 26);
            this.clbLayer.Name = "clbLayer";
            this.clbLayer.Size = new System.Drawing.Size(131, 200);
            this.clbLayer.TabIndex = 0;
            this.clbLayer.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.clbLayer_ItemCheck);
            this.clbLayer.SelectedIndexChanged += new System.EventHandler(this.clbLayer_SelectedIndexChanged);
            // 
            // btClearLayer
            // 
            this.btClearLayer.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.btClearLayer.Location = new System.Drawing.Point(19, 269);
            this.btClearLayer.Name = "btClearLayer";
            this.btClearLayer.Size = new System.Drawing.Size(131, 23);
            this.btClearLayer.TabIndex = 2;
            this.btClearLayer.Text = "clear layer";
            this.btClearLayer.UseVisualStyleBackColor = true;
            this.btClearLayer.Click += new System.EventHandler(this.btClearLayer_Click);
            // 
            // pbBackColor
            // 
            this.pbBackColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pbBackColor.Location = new System.Drawing.Point(78, 106);
            this.pbBackColor.Name = "pbBackColor";
            this.pbBackColor.Size = new System.Drawing.Size(30, 23);
            this.pbBackColor.TabIndex = 4;
            this.pbBackColor.TabStop = false;
            this.pbBackColor.Click += new System.EventHandler(this.pbBackColor_Click);
            // 
            // pbGridColor
            // 
            this.pbGridColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pbGridColor.Location = new System.Drawing.Point(78, 18);
            this.pbGridColor.Name = "pbGridColor";
            this.pbGridColor.Size = new System.Drawing.Size(30, 23);
            this.pbGridColor.TabIndex = 4;
            this.pbGridColor.TabStop = false;
            this.pbGridColor.Click += new System.EventHandler(this.pbGridColor_Click);
            // 
            // pbAxisColor
            // 
            this.pbAxisColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pbAxisColor.Location = new System.Drawing.Point(78, 47);
            this.pbAxisColor.Name = "pbAxisColor";
            this.pbAxisColor.Size = new System.Drawing.Size(30, 23);
            this.pbAxisColor.TabIndex = 4;
            this.pbAxisColor.TabStop = false;
            this.pbAxisColor.Click += new System.EventHandler(this.pbAxisColor_Click);
            // 
            // pbOriginColor
            // 
            this.pbOriginColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pbOriginColor.Location = new System.Drawing.Point(78, 76);
            this.pbOriginColor.Name = "pbOriginColor";
            this.pbOriginColor.Size = new System.Drawing.Size(30, 23);
            this.pbOriginColor.TabIndex = 4;
            this.pbOriginColor.TabStop = false;
            this.pbOriginColor.Click += new System.EventHandler(this.pbOriginColor_Click);
            // 
            // pbPenColor
            // 
            this.pbPenColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pbPenColor.Location = new System.Drawing.Point(68, 18);
            this.pbPenColor.Name = "pbPenColor";
            this.pbPenColor.Size = new System.Drawing.Size(30, 23);
            this.pbPenColor.TabIndex = 4;
            this.pbPenColor.TabStop = false;
            this.pbPenColor.Click += new System.EventHandler(this.pbPenColor_Click);
            // 
            // pbBrushColor
            // 
            this.pbBrushColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pbBrushColor.Location = new System.Drawing.Point(197, 18);
            this.pbBrushColor.Name = "pbBrushColor";
            this.pbBrushColor.Size = new System.Drawing.Size(30, 23);
            this.pbBrushColor.TabIndex = 4;
            this.pbBrushColor.TabStop = false;
            this.pbBrushColor.Click += new System.EventHandler(this.pbBrushColor_Click);
            // 
            // cbDrawArrow
            // 
            this.cbDrawArrow.AutoSize = true;
            this.cbDrawArrow.Location = new System.Drawing.Point(149, 22);
            this.cbDrawArrow.Name = "cbDrawArrow";
            this.cbDrawArrow.Size = new System.Drawing.Size(52, 16);
            this.cbDrawArrow.TabIndex = 4;
            this.cbDrawArrow.Text = "arrow";
            this.cbDrawArrow.UseVisualStyleBackColor = true;
            this.cbDrawArrow.CheckedChanged += new System.EventHandler(this.cbDrawArrow_CheckedChanged);
            // 
            // cbDrawCenter
            // 
            this.cbDrawCenter.AutoSize = true;
            this.cbDrawCenter.Location = new System.Drawing.Point(149, 50);
            this.cbDrawCenter.Name = "cbDrawCenter";
            this.cbDrawCenter.Size = new System.Drawing.Size(56, 16);
            this.cbDrawCenter.TabIndex = 5;
            this.cbDrawCenter.Text = "center";
            this.cbDrawCenter.UseVisualStyleBackColor = true;
            this.cbDrawCenter.CheckedChanged += new System.EventHandler(this.dbDrawCenter_CheckedChanged);
            // 
            // cbDrawGrid
            // 
            this.cbDrawGrid.AutoSize = true;
            this.cbDrawGrid.Location = new System.Drawing.Point(14, 23);
            this.cbDrawGrid.Name = "cbDrawGrid";
            this.cbDrawGrid.Size = new System.Drawing.Size(43, 16);
            this.cbDrawGrid.TabIndex = 0;
            this.cbDrawGrid.Text = "grid";
            this.cbDrawGrid.UseVisualStyleBackColor = true;
            this.cbDrawGrid.CheckedChanged += new System.EventHandler(this.cbDrawGrid_CheckedChanged);
            // 
            // cbDrawAxis
            // 
            this.cbDrawAxis.AutoSize = true;
            this.cbDrawAxis.Location = new System.Drawing.Point(14, 52);
            this.cbDrawAxis.Name = "cbDrawAxis";
            this.cbDrawAxis.Size = new System.Drawing.Size(45, 16);
            this.cbDrawAxis.TabIndex = 1;
            this.cbDrawAxis.Text = "axis";
            this.cbDrawAxis.UseVisualStyleBackColor = true;
            this.cbDrawAxis.CheckedChanged += new System.EventHandler(this.dbDrawAxis_CheckedChanged);
            // 
            // cbDrawOrigin
            // 
            this.cbDrawOrigin.AutoSize = true;
            this.cbDrawOrigin.Location = new System.Drawing.Point(14, 80);
            this.cbDrawOrigin.Name = "cbDrawOrigin";
            this.cbDrawOrigin.Size = new System.Drawing.Size(52, 16);
            this.cbDrawOrigin.TabIndex = 2;
            this.cbDrawOrigin.Text = "origin";
            this.cbDrawOrigin.UseVisualStyleBackColor = true;
            this.cbDrawOrigin.CheckedChanged += new System.EventHandler(this.cbDrawOrigin_CheckedChanged);
            // 
            // gbBasicSetting
            // 
            this.gbBasicSetting.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.gbBasicSetting.Controls.Add(this.label1);
            this.gbBasicSetting.Controls.Add(this.cbDrawCenter);
            this.gbBasicSetting.Controls.Add(this.cbDrawArrow);
            this.gbBasicSetting.Controls.Add(this.cbDrawOrigin);
            this.gbBasicSetting.Controls.Add(this.cbDrawAxis);
            this.gbBasicSetting.Controls.Add(this.pbBackColor);
            this.gbBasicSetting.Controls.Add(this.cbDrawGrid);
            this.gbBasicSetting.Controls.Add(this.pbGridColor);
            this.gbBasicSetting.Controls.Add(this.pbAxisColor);
            this.gbBasicSetting.Controls.Add(this.pbOriginColor);
            this.gbBasicSetting.Location = new System.Drawing.Point(12, 388);
            this.gbBasicSetting.Name = "gbBasicSetting";
            this.gbBasicSetting.Size = new System.Drawing.Size(247, 140);
            this.gbBasicSetting.TabIndex = 9;
            this.gbBasicSetting.TabStop = false;
            this.gbBasicSetting.Text = "basic setting";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 111);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(63, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "background";
            // 
            // gbPenBrush
            // 
            this.gbPenBrush.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.gbPenBrush.Controls.Add(this.lbBrush);
            this.gbPenBrush.Controls.Add(this.lbPen);
            this.gbPenBrush.Controls.Add(this.lbPenStyle);
            this.gbPenBrush.Controls.Add(this.lbPenWidth);
            this.gbPenBrush.Controls.Add(this.nudPenStyle);
            this.gbPenBrush.Controls.Add(this.nudPenWidth);
            this.gbPenBrush.Controls.Add(this.pbBrushColor);
            this.gbPenBrush.Controls.Add(this.pbPenColor);
            this.gbPenBrush.Location = new System.Drawing.Point(12, 536);
            this.gbPenBrush.Name = "gbPenBrush";
            this.gbPenBrush.Size = new System.Drawing.Size(247, 106);
            this.gbPenBrush.TabIndex = 10;
            this.gbPenBrush.TabStop = false;
            this.gbPenBrush.Text = "pen and brush";
            // 
            // lbBrush
            // 
            this.lbBrush.AutoSize = true;
            this.lbBrush.Location = new System.Drawing.Point(139, 23);
            this.lbBrush.Name = "lbBrush";
            this.lbBrush.Size = new System.Drawing.Size(33, 12);
            this.lbBrush.TabIndex = 5;
            this.lbBrush.Text = "brush";
            // 
            // lbPen
            // 
            this.lbPen.AutoSize = true;
            this.lbPen.Location = new System.Drawing.Point(13, 23);
            this.lbPen.Name = "lbPen";
            this.lbPen.Size = new System.Drawing.Size(23, 12);
            this.lbPen.TabIndex = 0;
            this.lbPen.Text = "pen";
            // 
            // lbPenStyle
            // 
            this.lbPenStyle.AutoSize = true;
            this.lbPenStyle.Location = new System.Drawing.Point(12, 74);
            this.lbPenStyle.Name = "lbPenStyle";
            this.lbPenStyle.Size = new System.Drawing.Size(30, 12);
            this.lbPenStyle.TabIndex = 3;
            this.lbPenStyle.Text = "style";
            // 
            // lbPenWidth
            // 
            this.lbPenWidth.AutoSize = true;
            this.lbPenWidth.Location = new System.Drawing.Point(12, 49);
            this.lbPenWidth.Name = "lbPenWidth";
            this.lbPenWidth.Size = new System.Drawing.Size(32, 12);
            this.lbPenWidth.TabIndex = 1;
            this.lbPenWidth.Text = "width";
            // 
            // nudPenStyle
            // 
            this.nudPenStyle.Location = new System.Drawing.Point(68, 72);
            this.nudPenStyle.Maximum = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.nudPenStyle.Name = "nudPenStyle";
            this.nudPenStyle.Size = new System.Drawing.Size(40, 19);
            this.nudPenStyle.TabIndex = 4;
            this.nudPenStyle.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudPenStyle.ValueChanged += new System.EventHandler(this.nudPenStyle_ValueChanged);
            // 
            // nudPenWidth
            // 
            this.nudPenWidth.Location = new System.Drawing.Point(68, 47);
            this.nudPenWidth.Maximum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.nudPenWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudPenWidth.Name = "nudPenWidth";
            this.nudPenWidth.Size = new System.Drawing.Size(40, 19);
            this.nudPenWidth.TabIndex = 2;
            this.nudPenWidth.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudPenWidth.ValueChanged += new System.EventHandler(this.nudPenWidth_ValueChanged);
            // 
            // cbFunc
            // 
            this.cbFunc.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbFunc.FormattingEnabled = true;
            this.cbFunc.Location = new System.Drawing.Point(19, 29);
            this.cbFunc.Name = "cbFunc";
            this.cbFunc.Size = new System.Drawing.Size(190, 20);
            this.cbFunc.TabIndex = 0;
            this.cbFunc.SelectedIndexChanged += new System.EventHandler(this.cbFunc_SelectedIndexChanged);
            // 
            // gbFunc
            // 
            this.gbFunc.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.gbFunc.Controls.Add(this.tbResult);
            this.gbFunc.Controls.Add(this.tbRadius);
            this.gbFunc.Controls.Add(this.lbPoint);
            this.gbFunc.Controls.Add(this.nudPointsCount);
            this.gbFunc.Controls.Add(this.lbResult);
            this.gbFunc.Controls.Add(this.btRunFunc);
            this.gbFunc.Controls.Add(this.lbRadius);
            this.gbFunc.Controls.Add(this.lbFillType);
            this.gbFunc.Controls.Add(this.lbArcDirection);
            this.gbFunc.Controls.Add(this.dgvPoints);
            this.gbFunc.Controls.Add(this.lbPointType);
            this.gbFunc.Controls.Add(this.cbFillType);
            this.gbFunc.Controls.Add(this.cbArcDirection);
            this.gbFunc.Controls.Add(this.cbPointType);
            this.gbFunc.Controls.Add(this.cbFunc);
            this.gbFunc.Location = new System.Drawing.Point(275, 388);
            this.gbFunc.Name = "gbFunc";
            this.gbFunc.Size = new System.Drawing.Size(417, 254);
            this.gbFunc.TabIndex = 11;
            this.gbFunc.TabStop = false;
            this.gbFunc.Text = "draw functions";
            // 
            // tbResult
            // 
            this.tbResult.Location = new System.Drawing.Point(303, 222);
            this.tbResult.Name = "tbResult";
            this.tbResult.ReadOnly = true;
            this.tbResult.Size = new System.Drawing.Size(94, 19);
            this.tbResult.TabIndex = 14;
            // 
            // tbRadius
            // 
            this.tbRadius.Location = new System.Drawing.Point(303, 169);
            this.tbRadius.Name = "tbRadius";
            this.tbRadius.Size = new System.Drawing.Size(94, 19);
            this.tbRadius.TabIndex = 11;
            // 
            // lbPoint
            // 
            this.lbPoint.AutoSize = true;
            this.lbPoint.Location = new System.Drawing.Point(17, 60);
            this.lbPoint.Name = "lbPoint";
            this.lbPoint.Size = new System.Drawing.Size(36, 12);
            this.lbPoint.TabIndex = 1;
            this.lbPoint.Text = "points";
            // 
            // nudPointsCount
            // 
            this.nudPointsCount.Location = new System.Drawing.Point(169, 59);
            this.nudPointsCount.Minimum = new decimal(new int[] {
            3,
            0,
            0,
            0});
            this.nudPointsCount.Name = "nudPointsCount";
            this.nudPointsCount.Size = new System.Drawing.Size(40, 19);
            this.nudPointsCount.TabIndex = 2;
            this.nudPointsCount.Value = new decimal(new int[] {
            3,
            0,
            0,
            0});
            this.nudPointsCount.ValueChanged += new System.EventHandler(this.nudPointsCount_ValueChanged);
            // 
            // lbResult
            // 
            this.lbResult.AutoSize = true;
            this.lbResult.Location = new System.Drawing.Point(233, 225);
            this.lbResult.Name = "lbResult";
            this.lbResult.Size = new System.Drawing.Size(34, 12);
            this.lbResult.TabIndex = 13;
            this.lbResult.Text = "result";
            // 
            // btRunFunc
            // 
            this.btRunFunc.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btRunFunc.Location = new System.Drawing.Point(19, 222);
            this.btRunFunc.Name = "btRunFunc";
            this.btRunFunc.Size = new System.Drawing.Size(190, 23);
            this.btRunFunc.TabIndex = 12;
            this.btRunFunc.Text = "run";
            this.btRunFunc.UseVisualStyleBackColor = true;
            this.btRunFunc.Click += new System.EventHandler(this.btRunFunc_Click);
            // 
            // lbRadius
            // 
            this.lbRadius.AutoSize = true;
            this.lbRadius.Location = new System.Drawing.Point(233, 172);
            this.lbRadius.Name = "lbRadius";
            this.lbRadius.Size = new System.Drawing.Size(36, 12);
            this.lbRadius.TabIndex = 10;
            this.lbRadius.Text = "radius";
            // 
            // lbFillType
            // 
            this.lbFillType.AutoSize = true;
            this.lbFillType.Location = new System.Drawing.Point(233, 142);
            this.lbFillType.Name = "lbFillType";
            this.lbFillType.Size = new System.Drawing.Size(44, 12);
            this.lbFillType.TabIndex = 8;
            this.lbFillType.Text = "fill type";
            // 
            // lbArcDirection
            // 
            this.lbArcDirection.AutoSize = true;
            this.lbArcDirection.Location = new System.Drawing.Point(233, 115);
            this.lbArcDirection.Name = "lbArcDirection";
            this.lbArcDirection.Size = new System.Drawing.Size(49, 12);
            this.lbArcDirection.TabIndex = 6;
            this.lbArcDirection.Text = "direction";
            // 
            // dgvPoints
            // 
            this.dgvPoints.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvPoints.Location = new System.Drawing.Point(19, 84);
            this.dgvPoints.MultiSelect = false;
            this.dgvPoints.Name = "dgvPoints";
            this.dgvPoints.RowTemplate.Height = 21;
            this.dgvPoints.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.dgvPoints.Size = new System.Drawing.Size(190, 125);
            this.dgvPoints.TabIndex = 3;
            // 
            // lbPointType
            // 
            this.lbPointType.AutoSize = true;
            this.lbPointType.Location = new System.Drawing.Point(233, 87);
            this.lbPointType.Name = "lbPointType";
            this.lbPointType.Size = new System.Drawing.Size(56, 12);
            this.lbPointType.TabIndex = 4;
            this.lbPointType.Text = "point type";
            // 
            // cbFillType
            // 
            this.cbFillType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbFillType.FormattingEnabled = true;
            this.cbFillType.Location = new System.Drawing.Point(303, 139);
            this.cbFillType.Name = "cbFillType";
            this.cbFillType.Size = new System.Drawing.Size(94, 20);
            this.cbFillType.TabIndex = 9;
            // 
            // cbArcDirection
            // 
            this.cbArcDirection.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbArcDirection.FormattingEnabled = true;
            this.cbArcDirection.Location = new System.Drawing.Point(303, 112);
            this.cbArcDirection.Name = "cbArcDirection";
            this.cbArcDirection.Size = new System.Drawing.Size(94, 20);
            this.cbArcDirection.TabIndex = 7;
            // 
            // cbPointType
            // 
            this.cbPointType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbPointType.FormattingEnabled = true;
            this.cbPointType.Location = new System.Drawing.Point(303, 84);
            this.cbPointType.Name = "cbPointType";
            this.cbPointType.Size = new System.Drawing.Size(94, 20);
            this.cbPointType.TabIndex = 5;
            // 
            // tbPosition
            // 
            this.tbPosition.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.tbPosition.Location = new System.Drawing.Point(321, 358);
            this.tbPosition.Name = "tbPosition";
            this.tbPosition.ReadOnly = true;
            this.tbPosition.Size = new System.Drawing.Size(144, 19);
            this.tbPosition.TabIndex = 5;
            this.tbPosition.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // btFit
            // 
            this.btFit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btFit.Location = new System.Drawing.Point(12, 354);
            this.btFit.Name = "btFit";
            this.btFit.Size = new System.Drawing.Size(131, 23);
            this.btFit.TabIndex = 1;
            this.btFit.Text = "Fit";
            this.btFit.UseVisualStyleBackColor = true;
            this.btFit.Click += new System.EventHandler(this.btFit_Click);
            // 
            // btCopyImage
            // 
            this.btCopyImage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btCopyImage.Location = new System.Drawing.Point(730, 604);
            this.btCopyImage.Name = "btCopyImage";
            this.btCopyImage.Size = new System.Drawing.Size(131, 23);
            this.btCopyImage.TabIndex = 13;
            this.btCopyImage.Text = "copy image";
            this.btCopyImage.UseVisualStyleBackColor = true;
            this.btCopyImage.Click += new System.EventHandler(this.btCopyImage_Click);
            // 
            // btSaveImage
            // 
            this.btSaveImage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btSaveImage.Location = new System.Drawing.Point(730, 575);
            this.btSaveImage.Name = "btSaveImage";
            this.btSaveImage.Size = new System.Drawing.Size(131, 23);
            this.btSaveImage.TabIndex = 12;
            this.btSaveImage.Text = "save image";
            this.btSaveImage.UseVisualStyleBackColor = true;
            this.btSaveImage.Click += new System.EventHandler(this.btSaveImage_Click);
            // 
            // cbPan
            // 
            this.cbPan.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cbPan.AutoSize = true;
            this.cbPan.Location = new System.Drawing.Point(161, 358);
            this.cbPan.Name = "cbPan";
            this.cbPan.Size = new System.Drawing.Size(42, 16);
            this.cbPan.TabIndex = 2;
            this.cbPan.Text = "pan";
            this.cbPan.UseVisualStyleBackColor = true;
            this.cbPan.CheckedChanged += new System.EventHandler(this.cbPan_CheckedChanged);
            // 
            // cbZoom
            // 
            this.cbZoom.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cbZoom.AutoSize = true;
            this.cbZoom.Location = new System.Drawing.Point(209, 358);
            this.cbZoom.Name = "cbZoom";
            this.cbZoom.Size = new System.Drawing.Size(50, 16);
            this.cbZoom.TabIndex = 3;
            this.cbZoom.Text = "zoom";
            this.cbZoom.UseVisualStyleBackColor = true;
            this.cbZoom.CheckedChanged += new System.EventHandler(this.cbZoom_CheckedChanged);
            // 
            // axDrawShape
            // 
            this.axDrawShape.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.axDrawShape.Enabled = true;
            this.axDrawShape.Location = new System.Drawing.Point(12, 12);
            this.axDrawShape.Name = "axDrawShape";
            this.axDrawShape.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axDrawShape.OcxState")));
            this.axDrawShape.Size = new System.Drawing.Size(680, 336);
            this.axDrawShape.TabIndex = 0;
            // 
            // tbLastClick
            // 
            this.tbLastClick.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.tbLastClick.Location = new System.Drawing.Point(548, 358);
            this.tbLastClick.Name = "tbLastClick";
            this.tbLastClick.ReadOnly = true;
            this.tbLastClick.Size = new System.Drawing.Size(144, 19);
            this.tbLastClick.TabIndex = 7;
            this.tbLastClick.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // lbLastClick
            // 
            this.lbLastClick.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.lbLastClick.AutoSize = true;
            this.lbLastClick.Location = new System.Drawing.Point(490, 361);
            this.lbLastClick.Name = "lbLastClick";
            this.lbLastClick.Size = new System.Drawing.Size(52, 12);
            this.lbLastClick.TabIndex = 6;
            this.lbLastClick.Text = "last click";
            // 
            // lbCursor
            // 
            this.lbCursor.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.lbCursor.AutoSize = true;
            this.lbCursor.Location = new System.Drawing.Point(278, 361);
            this.lbCursor.Name = "lbCursor";
            this.lbCursor.Size = new System.Drawing.Size(37, 12);
            this.lbCursor.TabIndex = 4;
            this.lbCursor.Text = "cursor";
            // 
            // formMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(896, 651);
            this.Controls.Add(this.axDrawShape);
            this.Controls.Add(this.tbLastClick);
            this.Controls.Add(this.tbPosition);
            this.Controls.Add(this.gbFunc);
            this.Controls.Add(this.btSaveImage);
            this.Controls.Add(this.btCopyImage);
            this.Controls.Add(this.btFit);
            this.Controls.Add(this.gbPenBrush);
            this.Controls.Add(this.lbCursor);
            this.Controls.Add(this.lbLastClick);
            this.Controls.Add(this.gbBasicSetting);
            this.Controls.Add(this.gbLayer);
            this.Controls.Add(this.cbZoom);
            this.Controls.Add(this.cbPan);
            this.Name = "formMain";
            this.Text = "DrawStateTest";
            this.Load += new System.EventHandler(this.formMain_Load);
            this.gbLayer.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pbBackColor)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbGridColor)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbAxisColor)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbOriginColor)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbPenColor)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbBrushColor)).EndInit();
            this.gbBasicSetting.ResumeLayout(false);
            this.gbBasicSetting.PerformLayout();
            this.gbPenBrush.ResumeLayout(false);
            this.gbPenBrush.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudPenStyle)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudPenWidth)).EndInit();
            this.gbFunc.ResumeLayout(false);
            this.gbFunc.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudPointsCount)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dgvPoints)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.axDrawShape)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button btInsertLayer;
        private System.Windows.Forms.Button btDeleteLayer;
        private System.Windows.Forms.GroupBox gbLayer;
        private System.Windows.Forms.ColorDialog colorDlg;
        private System.Windows.Forms.PictureBox pbBackColor;
        private System.Windows.Forms.PictureBox pbGridColor;
        private System.Windows.Forms.PictureBox pbAxisColor;
        private System.Windows.Forms.PictureBox pbOriginColor;
        private System.Windows.Forms.PictureBox pbPenColor;
        private System.Windows.Forms.PictureBox pbBrushColor;
        private System.Windows.Forms.CheckBox cbDrawArrow;
        private System.Windows.Forms.CheckBox cbDrawCenter;
        private System.Windows.Forms.CheckBox cbDrawGrid;
        private System.Windows.Forms.CheckBox cbDrawAxis;
        private System.Windows.Forms.CheckBox cbDrawOrigin;
        private System.Windows.Forms.GroupBox gbBasicSetting;
        private System.Windows.Forms.GroupBox gbPenBrush;
        private System.Windows.Forms.Label lbPenWidth;
        private System.Windows.Forms.NumericUpDown nudPenWidth;
        private System.Windows.Forms.ComboBox cbFunc;
        private System.Windows.Forms.GroupBox gbFunc;
        private System.Windows.Forms.CheckedListBox clbLayer;
        private System.Windows.Forms.TextBox tbRadius;
        private System.Windows.Forms.Label lbRadius;
        private System.Windows.Forms.Label lbFillType;
        private System.Windows.Forms.Label lbArcDirection;
        private System.Windows.Forms.Label lbPointType;
        private System.Windows.Forms.TextBox tbPosition;
        private System.Windows.Forms.Button btFit;
        private System.Windows.Forms.Button btCopyImage;
        private System.Windows.Forms.Button btSaveImage;
        private System.Windows.Forms.SaveFileDialog saveFileDlg;
        private System.Windows.Forms.CheckBox cbPan;
        private System.Windows.Forms.CheckBox cbZoom;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lbBrush;
        private System.Windows.Forms.Label lbPen;
        private System.Windows.Forms.Button btClearLayer;
        private System.Windows.Forms.Button btRunFunc;
        private System.Windows.Forms.ComboBox cbFillType;
        private System.Windows.Forms.ComboBox cbArcDirection;
        private System.Windows.Forms.ComboBox cbPointType;
        private AxDrawShapeLib.AxDrawShape axDrawShape;
        private System.Windows.Forms.Label lbPenStyle;
        private System.Windows.Forms.NumericUpDown nudPenStyle;
        private System.Windows.Forms.TextBox tbLastClick;
        private System.Windows.Forms.Label lbLastClick;
        private System.Windows.Forms.Label lbCursor;
        private System.Windows.Forms.Label lbPoint;
        private System.Windows.Forms.NumericUpDown nudPointsCount;
        private System.Windows.Forms.DataGridView dgvPoints;
        private System.Windows.Forms.TextBox tbResult;
        private System.Windows.Forms.Label lbResult;
    }
}

