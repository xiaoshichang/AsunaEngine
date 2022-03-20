using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using AsunaEditor.Structures;
using AsunaEditor.Utils;
using Microsoft.Win32;

namespace AsunaEditor
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class AsunaEditorMainWindow : Window
    {
        public AsunaEditorMainWindow()
        {
            Current = this;
            InitializeComponent();
        }

        private void MenuItem_Click_NewProject(object sender, RoutedEventArgs e)
        {
            var result = AsunaProject.Create("C:\\Users\\xiao\\Desktop\\AsunaClientProjectDemo", "0.0.1");
            if (result != AsunaProjectCreateResult.Success)
            {
                MessageBox.Show($"{result}", ErrorMsg.ErrorMsg_CreateProject_Caption, MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            AssetBrowserPanel.OnProjectLoaded();
        }

        private void MenuItem_Click_OpenProject(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = $"AsunaProject File|{AsunaProject.ProjectFileName}";
            openFileDialog.Multiselect = false;
            if (openFileDialog.ShowDialog() == true)
            {
                var projectFile = openFileDialog.FileName;
                var result = AsunaProject.Load(projectFile);
                if (result != AsunaProjectLoadResult.Success)
                {
                    MessageBox.Show($"{result}", ErrorMsg.ErrorMsg_LoadProject_Caption, MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }
                AssetBrowserPanel.OnProjectLoaded();
                ActionResult.Text = "Open Project OK!";
                Title = $"AsunaEditor - {projectFile}";
            }

        }

        private void MenuItem_Click_SaveProject(object sender, RoutedEventArgs e)
        {
            if (AsunaProject.Current == null)
            {
                MessageBox.Show(ErrorMsg.ErrorMsg_SaveProject_Content_Empty, ErrorMsg.ErrorMsg_SaveProject_Caption, MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
        }

        private void MenuItem_Click_ExitEditor(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        public static AsunaEditorMainWindow Current;
    }
}
