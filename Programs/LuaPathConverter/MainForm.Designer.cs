namespace LuaPathConverter;

partial class MainForm
{
	/// <summary>
	///  Required designer variable.
	/// </summary>
	private System.ComponentModel.IContainer components = null;

	/// <summary>
	///  Clean up any resources being used.
	/// </summary>
	/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
	protected override void Dispose(bool disposing)
	{
		if (disposing && (components != null))
		{
			components.Dispose();
		}

		base.Dispose(disposing);
	}

	#region Windows Form Designer generated code

	/// <summary>
	///  Required method for Designer support - do not modify
	///  the contents of this method with the code editor.
	/// </summary>
	private void InitializeComponent()
	{
            this.label1 = new System.Windows.Forms.Label();
            this.OriginPath_TextBlock = new System.Windows.Forms.TextBox();
            this.Convert_Button = new System.Windows.Forms.Button();
            this.ConvertPath_TextBlock = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(19, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(69, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "OriginPath";
            // 
            // OriginPath_TextBlock
            // 
            this.OriginPath_TextBlock.Location = new System.Drawing.Point(94, 20);
            this.OriginPath_TextBlock.Name = "OriginPath_TextBlock";
            this.OriginPath_TextBlock.Size = new System.Drawing.Size(167, 23);
            this.OriginPath_TextBlock.TabIndex = 1;
            // 
            // Convert_Button
            // 
            this.Convert_Button.Location = new System.Drawing.Point(276, 20);
            this.Convert_Button.Name = "Convert_Button";
            this.Convert_Button.Size = new System.Drawing.Size(75, 23);
            this.Convert_Button.TabIndex = 2;
            this.Convert_Button.Text = "Convert";
            this.Convert_Button.UseVisualStyleBackColor = true;
            this.Convert_Button.Click += new System.EventHandler(this.Convert_Button_Click);
            // 
            // ConvertPath_TextBlock
            // 
            this.ConvertPath_TextBlock.Location = new System.Drawing.Point(367, 20);
            this.ConvertPath_TextBlock.Name = "ConvertPath_TextBlock";
            this.ConvertPath_TextBlock.ReadOnly = true;
            this.ConvertPath_TextBlock.Size = new System.Drawing.Size(167, 23);
            this.ConvertPath_TextBlock.TabIndex = 3;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 17F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.ConvertPath_TextBlock);
            this.Controls.Add(this.Convert_Button);
            this.Controls.Add(this.OriginPath_TextBlock);
            this.Controls.Add(this.label1);
            this.Name = "MainForm";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

	}

    #endregion

    private Label label1;
    private TextBox OriginPath_TextBlock;
    private Button Convert_Button;
    private TextBox ConvertPath_TextBlock;
}