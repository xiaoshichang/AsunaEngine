using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
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

namespace AsunaEditor.Editors.MainEditor
{

    public class HierarcyTreeItem : TreeViewItem
    {
        public HierarcyTreeItem(string Name)
        {
            Header = Name;
            entity = Name;
        }

        public string entity;
    }



    /// <summary>
    /// HierarcyPanel.xaml 的交互逻辑
    /// </summary>
    public partial class HierarcyPanel : Canvas
    {
        public HierarcyPanel()
        {
            InitializeComponent();
            InitHierarcyTreeView();
        }

        private void InitHierarcyTreeView()
        {
            var item1 = new HierarcyTreeItem("Item1");
            var item2 = new HierarcyTreeItem("Item2");
            var item3 = new HierarcyTreeItem("Item3");
            HierarcyTree.Items.Add(item1);
            HierarcyTree.Items.Add(item2);
            item1.Items.Add(item3);
        }

        private void HierarcyTree_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            var item =  (HierarcyTreeItem)e.NewValue;
            AsunaEditorMainWindow.Current.InspectorPanel.OnInspectNewObject(item.entity);
        }
        

    }
}
