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
            Title = $"Asuna Editor - {AsunaProject.Current.ProjectRootPath}";
        }

        private void MenuItem_Click_SaveProject(object sender, RoutedEventArgs e)
        {
            AsunaProject.Current.Save();
        }

        private void MenuItem_Click_ExitEditor(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        public static AsunaEditorMainWindow Current;
    }
}
