namespace LuaPathConverter;

public partial class MainForm : Form
{
	public MainForm()
	{
		InitializeComponent();
	}

    private void Convert_Button_Click(object sender, EventArgs e)
    {
		String InputPath = OriginPath_TextBlock.Text;
		if (InputPath != "")
        {
			// Get the index of first '.' character in InputPath
			var Index = InputPath.LastIndexOf('.');

			// Remove extension from file path
			if (Index != -1)
            {
				InputPath = InputPath.Substring(0, Index);
            }

			// Replace '\' character with '.'
			InputPath = InputPath.Replace('\\', '.');

			ConvertPath_TextBlock.Text = InputPath;
        }
    }
}