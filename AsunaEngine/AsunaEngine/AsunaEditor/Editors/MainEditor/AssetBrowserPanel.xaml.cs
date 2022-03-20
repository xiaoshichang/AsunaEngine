using System;
using System.Collections.Generic;
using System.IO;
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

namespace AsunaEditor.Editors.MainEditor
{
    /// <summary>
    /// AssetBrowserPanel.xaml 的交互逻辑
    /// </summary>
    public partial class AssetBrowserPanel : Canvas
    {
        public AssetBrowserPanel()
        {
            InitializeComponent();
        }

        private void InitAssetTree()
        {
            AssetTree = new TreeView();
            AssetTree.SelectedItemChanged += AssetTree_SelectedItemChanged;
            this.Children.Add(AssetTree);
        }


        public void OnProjectLoaded()
        {
            Refresh();
        }

        private static TreeViewItem CreateDirectoryNode(DirectoryInfo directoryInfo)
        {
            var directoryNode = new TreeViewItem { Header = directoryInfo.Name };
            foreach (var directory in directoryInfo.GetDirectories())
            {
                directoryNode.Items.Add(CreateDirectoryNode(directory));
            }

            foreach (var file in directoryInfo.GetFiles())
            {
                directoryNode.Items.Add(new TreeViewItem { Header = file.Name });

            }
            return directoryNode;

        }

        public void Refresh()
        {
            if (AssetTree == null)
            {
                InitAssetTree();
            }

            AssetTree.Items.Clear();
            var rootDirectoryInfo = new DirectoryInfo(AsunaProject.Current.ProjectRootPath);
            var rootNode = CreateDirectoryNode(rootDirectoryInfo);
            AssetTree.Items.Add(rootNode);
        }

        private void AssetTree_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            var item = (TreeViewItem)e.NewValue;
            AsunaEditorMainWindow.Current.InspectorPanel.OnInspectNewObject(item.Header);
        }

        private TreeView AssetTree = null;
    }
}
