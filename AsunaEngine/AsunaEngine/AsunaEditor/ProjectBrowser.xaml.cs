using AsunaEditor.Structures;
using AsunaEditor.Utils;
using System.Windows;
using System.Windows.Forms;
using System.IO;

namespace AsunaEditor
{
    /// <summary>
    /// ProjectBrowser.xaml 的交互逻辑
    /// </summary>
    public partial class ProjectBrowser : Window
    {
        public ProjectBrowser()
        {
            InitializeComponent();
            this.DataContext = this;
        }

        private void Button_Click_SelectProjectRoot(object sender, RoutedEventArgs e)
        {
            using (var dialog = new FolderBrowserDialog())
            {
                DialogResult result = dialog.ShowDialog();
                if (result == System.Windows.Forms.DialogResult.OK)
                {
                    ProjectRootText.Text = dialog.SelectedPath;
                }
            }
        }

        private void Button_Click_OpenProject(object sender, RoutedEventArgs e)
        {
            using (var dialog = new FolderBrowserDialog())
            {
                DialogResult result = dialog.ShowDialog();
                if (result == System.Windows.Forms.DialogResult.OK)
                {
                    var projectRoot = dialog.SelectedPath;
                    var projectFile = Path.Combine(projectRoot, AsunaProject.ProjectFileName);
                    if (!File.Exists(projectFile))
                    {
                        System.Windows.MessageBox.Show(ErrorMsg.ErrorMsg_NotAProject);
                        return;
                    }
                    var loadRet = AsunaProject.Load(projectFile);
                    if (loadRet != AsunaProjectLoadResult.Success)
                    {
                        System.Windows.MessageBox.Show($"{loadRet}", ErrorMsg.ErrorMsg_LoadProject_Caption, MessageBoxButton.OK, MessageBoxImage.Error);
                        return;
                    }
                    var window = new AsunaEditorMainWindow();
                    window.Show();
                    Close();
                }
            }
        }

        private void Button_Click_CreateProject(object sender, RoutedEventArgs e)
        {
            var projectRoot = ProjectRootText.Text;
            if (!Directory.Exists(projectRoot))
            {
                System.Windows.MessageBox.Show(ErrorMsg.ErrorMsg_DirectoryNotExist);
                return;
            }

            if (string.IsNullOrEmpty(ProjectNameInput.Text))
            {
                System.Windows.MessageBox.Show(ErrorMsg.ErrorMsg_ProjectNameEmpty);
                return;
            }

            var result = AsunaProject.Create(projectRoot, "0.0.1");
            if (result != AsunaProjectCreateResult.Success)
            {
                System.Windows.MessageBox.Show($"{result}", ErrorMsg.ErrorMsg_CreateProject_Caption, MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            var window = new AsunaEditorMainWindow();
            window.Show();
            Close();
        }

    }
}
