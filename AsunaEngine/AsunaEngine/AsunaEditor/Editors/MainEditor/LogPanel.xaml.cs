using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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

    public class LogRecordItem : ListViewItem
    {
        public LogRecordItem(LogRecord record)
        {
            var text = new TextBlock();
            if (record.Level == LogLevel.Info)
            {
                text.Foreground = System.Windows.Media.Brushes.DarkSeaGreen;
            }
            else if (record.Level == LogLevel.Warning)
            {
                text.Foreground = System.Windows.Media.Brushes.DarkGoldenrod;
            }
            else
            {
                text.Foreground = System.Windows.Media.Brushes.DarkRed;
            }
            text.Text = record.FullMessage;
            AddChild(text);
        }
    }



    /// <summary>
    /// LogPanel.xaml 的交互逻辑
    /// </summary>
    public partial class LogPanel : UserControl
    {
        public LogPanel()
        {
            InitializeComponent();
            // https://wpf-tutorial.com/data-binding/using-the-datacontext/
            // we assign the "this" reference to the DataContext, which basically just tells the Window that we want itself to be the data context.
            this.DataContext = this;
            AddLogRecord(new LogRecord(LogLevel.Info, "a simple message"));
            AddLogRecord(new LogRecord(LogLevel.Warning, "a warning!!"));
            for(int i = 0; i < 100; i++)
            {
                AddLogRecord(new LogRecord(LogLevel.Error, "code red! code red!"));
            }
        }

        public void AddLogRecord(LogRecord record)
        {
            var item = new LogRecordItem(record);
            LogList.Items.Add(item);

            if (LogList.Items.Count > 101)
            {
                LogList.Items.RemoveAt(0);
            }
        }

        
    }
}
