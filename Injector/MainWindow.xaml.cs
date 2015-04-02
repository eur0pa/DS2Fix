using System;
using System.IO.MemoryMappedFiles;
using System.Threading;
using System.Windows;

namespace Injector
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>

    public partial class MainWindow : Window
    {
        private Thread tBackgroundThread;
        private Boolean execute = false;
        private Boolean res = false;

        public MainWindow()
        {
            InitializeComponent();

            tBackgroundThread = new Thread(new ThreadStart(threadEntry));
            tBackgroundThread.Start();
        }

        private void threadEntry()
        {
            do
            {
                Thread.Sleep(500);

                try
                {
                    if (execute)
                    {
                        res = Hooker.InjectDll();
                    }
                }
                catch (Exception ex)
                {
                    Dispatcher.Invoke(delegate()
                    {
                        StatusText.Text = "Error: " + ex.Message;
                    });
                }

                if (res)
                {
                    Dispatcher.Invoke(delegate()
                    {
                        StatusText.Text = "Injected";
                    });

                    execute = false;
                }
            } while (res == false);
        }

        private void Apply_button_Click(object sender, RoutedEventArgs e)
        {
            StatusText.Text = (res ? "Injected" : "...");
            execute = true;
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            tBackgroundThread.Abort();
            Application.Current.Shutdown();
        }

        private void Help_button_Click(object sender, RoutedEventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/eur0pa/DS2Fix");
        }
    }
}