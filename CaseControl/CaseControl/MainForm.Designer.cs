namespace CaseControl {
    partial class MainForm {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.sensorPanel = new System.Windows.Forms.TableLayoutPanel();
            this.tempLabel = new System.Windows.Forms.Label();
            this.flowRateLabel = new System.Windows.Forms.Label();
            this.pumpSpeedLabel = new System.Windows.Forms.Label();
            this.fanSpeedLabel = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.tabPanel = new System.Windows.Forms.TableLayoutPanel();
            this.ledTabLabel = new System.Windows.Forms.Label();
            this.pumpTabLabel = new System.Windows.Forms.Label();
            this.fanTabLabel = new System.Windows.Forms.Label();
            this.configTabLabel = new System.Windows.Forms.Label();
            this.configPanel = new System.Windows.Forms.Panel();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.sensorPanel.SuspendLayout();
            this.tabPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // sensorPanel
            // 
            this.sensorPanel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.sensorPanel.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this.sensorPanel.ColumnCount = 4;
            this.sensorPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.sensorPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.sensorPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.sensorPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.sensorPanel.Controls.Add(this.tempLabel, 3, 1);
            this.sensorPanel.Controls.Add(this.flowRateLabel, 2, 1);
            this.sensorPanel.Controls.Add(this.pumpSpeedLabel, 1, 1);
            this.sensorPanel.Controls.Add(this.fanSpeedLabel, 0, 1);
            this.sensorPanel.Controls.Add(this.label4, 3, 0);
            this.sensorPanel.Controls.Add(this.label3, 2, 0);
            this.sensorPanel.Controls.Add(this.label2, 1, 0);
            this.sensorPanel.Controls.Add(this.label1, 0, 0);
            this.sensorPanel.Location = new System.Drawing.Point(12, 13);
            this.sensorPanel.Name = "sensorPanel";
            this.sensorPanel.RowCount = 2;
            this.sensorPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 27F));
            this.sensorPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.sensorPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 21F));
            this.sensorPanel.Size = new System.Drawing.Size(785, 86);
            this.sensorPanel.TabIndex = 0;
            // 
            // tempLabel
            // 
            this.tempLabel.AutoSize = true;
            this.tempLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tempLabel.Font = new System.Drawing.Font("Segoe UI", 27.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.tempLabel.ForeColor = System.Drawing.Color.White;
            this.tempLabel.Location = new System.Drawing.Point(592, 29);
            this.tempLabel.Name = "tempLabel";
            this.tempLabel.Size = new System.Drawing.Size(189, 56);
            this.tempLabel.TabIndex = 7;
            this.tempLabel.Text = "N/A";
            this.tempLabel.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // flowRateLabel
            // 
            this.flowRateLabel.AutoSize = true;
            this.flowRateLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowRateLabel.Font = new System.Drawing.Font("Segoe UI", 27.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.flowRateLabel.ForeColor = System.Drawing.Color.White;
            this.flowRateLabel.Location = new System.Drawing.Point(396, 29);
            this.flowRateLabel.Name = "flowRateLabel";
            this.flowRateLabel.Size = new System.Drawing.Size(189, 56);
            this.flowRateLabel.TabIndex = 6;
            this.flowRateLabel.Text = "N/A";
            this.flowRateLabel.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // pumpSpeedLabel
            // 
            this.pumpSpeedLabel.AutoSize = true;
            this.pumpSpeedLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pumpSpeedLabel.Font = new System.Drawing.Font("Segoe UI", 27.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.pumpSpeedLabel.ForeColor = System.Drawing.Color.White;
            this.pumpSpeedLabel.Location = new System.Drawing.Point(200, 29);
            this.pumpSpeedLabel.Name = "pumpSpeedLabel";
            this.pumpSpeedLabel.Size = new System.Drawing.Size(189, 56);
            this.pumpSpeedLabel.TabIndex = 5;
            this.pumpSpeedLabel.Text = "N/A";
            this.pumpSpeedLabel.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // fanSpeedLabel
            // 
            this.fanSpeedLabel.AutoSize = true;
            this.fanSpeedLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fanSpeedLabel.Font = new System.Drawing.Font("Segoe UI", 27.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.fanSpeedLabel.ForeColor = System.Drawing.Color.White;
            this.fanSpeedLabel.Location = new System.Drawing.Point(4, 29);
            this.fanSpeedLabel.Name = "fanSpeedLabel";
            this.fanSpeedLabel.Size = new System.Drawing.Size(189, 56);
            this.fanSpeedLabel.TabIndex = 1;
            this.fanSpeedLabel.Text = "N/A";
            this.fanSpeedLabel.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label4.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.label4.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.label4.Location = new System.Drawing.Point(592, 1);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(189, 27);
            this.label4.TabIndex = 4;
            this.label4.Text = "Coolant Temperature";
            this.label4.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label3.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.label3.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.label3.Location = new System.Drawing.Point(396, 1);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(189, 27);
            this.label3.TabIndex = 3;
            this.label3.Text = "Flow Rate";
            this.label3.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.label2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.label2.Location = new System.Drawing.Point(200, 1);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(189, 27);
            this.label2.TabIndex = 2;
            this.label2.Text = "Pump Speed";
            this.label2.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.label1.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.label1.Location = new System.Drawing.Point(4, 1);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(189, 27);
            this.label1.TabIndex = 1;
            this.label1.Text = "Fan Speed";
            this.label1.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // tabPanel
            // 
            this.tabPanel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.tabPanel.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this.tabPanel.ColumnCount = 1;
            this.tabPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tabPanel.Controls.Add(this.ledTabLabel, 0, 3);
            this.tabPanel.Controls.Add(this.pumpTabLabel, 0, 2);
            this.tabPanel.Controls.Add(this.fanTabLabel, 0, 1);
            this.tabPanel.Controls.Add(this.configTabLabel, 0, 0);
            this.tabPanel.Location = new System.Drawing.Point(12, 106);
            this.tabPanel.Name = "tabPanel";
            this.tabPanel.RowCount = 4;
            this.tabPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tabPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tabPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tabPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tabPanel.Size = new System.Drawing.Size(125, 343);
            this.tabPanel.TabIndex = 1;
            // 
            // ledTabLabel
            // 
            this.ledTabLabel.AutoSize = true;
            this.ledTabLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ledTabLabel.Font = new System.Drawing.Font("Segoe UI", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.ledTabLabel.ForeColor = System.Drawing.Color.White;
            this.ledTabLabel.Location = new System.Drawing.Point(4, 256);
            this.ledTabLabel.Name = "ledTabLabel";
            this.ledTabLabel.Size = new System.Drawing.Size(117, 86);
            this.ledTabLabel.TabIndex = 3;
            this.ledTabLabel.Text = "LEDs";
            this.ledTabLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // pumpTabLabel
            // 
            this.pumpTabLabel.AutoSize = true;
            this.pumpTabLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pumpTabLabel.Font = new System.Drawing.Font("Segoe UI", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.pumpTabLabel.ForeColor = System.Drawing.Color.White;
            this.pumpTabLabel.Location = new System.Drawing.Point(4, 171);
            this.pumpTabLabel.Name = "pumpTabLabel";
            this.pumpTabLabel.Size = new System.Drawing.Size(117, 84);
            this.pumpTabLabel.TabIndex = 2;
            this.pumpTabLabel.Text = "Pump";
            this.pumpTabLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // fanTabLabel
            // 
            this.fanTabLabel.AutoSize = true;
            this.fanTabLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fanTabLabel.Font = new System.Drawing.Font("Segoe UI", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.fanTabLabel.ForeColor = System.Drawing.Color.White;
            this.fanTabLabel.Location = new System.Drawing.Point(4, 86);
            this.fanTabLabel.Name = "fanTabLabel";
            this.fanTabLabel.Size = new System.Drawing.Size(117, 84);
            this.fanTabLabel.TabIndex = 1;
            this.fanTabLabel.Text = "Fan";
            this.fanTabLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // configTabLabel
            // 
            this.configTabLabel.AutoSize = true;
            this.configTabLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.configTabLabel.Font = new System.Drawing.Font("Segoe UI", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.configTabLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.configTabLabel.Location = new System.Drawing.Point(4, 1);
            this.configTabLabel.Name = "configTabLabel";
            this.configTabLabel.Size = new System.Drawing.Size(117, 84);
            this.configTabLabel.TabIndex = 0;
            this.configTabLabel.Text = "Config";
            this.configTabLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // configPanel
            // 
            this.configPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.configPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.configPanel.Location = new System.Drawing.Point(135, 106);
            this.configPanel.Name = "configPanel";
            this.configPanel.Size = new System.Drawing.Size(662, 343);
            this.configPanel.TabIndex = 2;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this.tableLayoutPanel1.ColumnCount = 4;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel1.Location = new System.Drawing.Point(12, 455);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(785, 31);
            this.tableLayoutPanel1.TabIndex = 3;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(32)))), ((int)(((byte)(32)))), ((int)(((byte)(32)))));
            this.ClientSize = new System.Drawing.Size(809, 498);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(this.configPanel);
            this.Controls.Add(this.tabPanel);
            this.Controls.Add(this.sensorPanel);
            this.MinimumSize = new System.Drawing.Size(825, 531);
            this.Name = "MainForm";
            this.Text = "CaseControl";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.sensorPanel.ResumeLayout(false);
            this.sensorPanel.PerformLayout();
            this.tabPanel.ResumeLayout(false);
            this.tabPanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private TableLayoutPanel sensorPanel;
        private Label label4;
        private Label label3;
        private Label label2;
        private Label label1;
        private Label tempLabel;
        private Label flowRateLabel;
        private Label pumpSpeedLabel;
        private Label fanSpeedLabel;
        private TableLayoutPanel tabPanel;
        private Label ledTabLabel;
        private Label pumpTabLabel;
        private Label fanTabLabel;
        private Label configTabLabel;
        private Panel configPanel;
        private TableLayoutPanel tableLayoutPanel1;
    }
}