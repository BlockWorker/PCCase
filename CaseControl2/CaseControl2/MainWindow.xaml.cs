using HidSharp;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
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

namespace CaseControl2
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void UpdateMeasurements() {
            Dispatcher.Invoke(() => {
                fanSpeedLabel.Content = float.IsNaN(App.fanRPM) ? "N/A" : string.Format(CultureInfo.InvariantCulture, "{0:F0} RPM", App.fanRPM);
                pumpSpeedLabel.Content = float.IsNaN(App.pumpRPM) ? "N/A" : string.Format(CultureInfo.InvariantCulture, "{0:F0} RPM", App.pumpRPM);
                flowRateLabel.Content = float.IsNaN(App.flowLPH) ? "N/A" : string.Format(CultureInfo.InvariantCulture, "{0:F1} l/h", App.flowLPH);
                coolantTempLabel.Content = float.IsNaN(App.tempC) ? "N/A" : string.Format(CultureInfo.InvariantCulture, "{0:F1} °C", App.tempC);
            });
        }

        private void UpdateConfigFromDevice() {
            App.ReadConfig();
            Dispatcher.Invoke(() => {
                fanCurveEditor.CurvePoints.Clear();
                fanCurveEditor.CurvePoints.AddRange(App.fanPoints);
                fanCurveEditor.InvalidateVisual();
                pumpCurveEditor.CurvePoints.Clear();
                pumpCurveEditor.CurvePoints.AddRange(App.pumpPoints);
                pumpCurveEditor.InvalidateVisual();
            });
        }

        private void UpdateDevices() {
            if (App.configStream is null) {
                Dispatcher.Invoke(() => {
                    controlStatusLabel.Content = "Control: Disconnected";
                    controlStatusLabel.Foreground = Brushes.Red;
                });
            } else {
                Dispatcher.Invoke(() => {
                    controlStatusLabel.Content = "Control: Connected";
                    controlStatusLabel.Foreground = Brushes.Lime;
                });

                UpdateConfigFromDevice();
            }

            if (App.ledStream is null) {
                Dispatcher.Invoke(() => {
                    rgbStatusLabel.Content = "RGB: Disconnected";
                    rgbStatusLabel.Foreground = Brushes.Red;
                });
            } else {
                Dispatcher.Invoke(() => {
                    rgbStatusLabel.Content = "RGB: Connected";
                    rgbStatusLabel.Foreground = Brushes.Lime;
                });
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e) {
            App.MeasurementNewValues += (_, _) => UpdateMeasurements();
            App.DevicesChanged += (_, _) => UpdateDevices();
            UpdateDevices();
        }

        private void OnTabMouseUp(object sender, MouseButtonEventArgs e) {
            configTabLabel.Foreground = fanTabLabel.Foreground = pumpTabLabel.Foreground = ledTabLabel.Foreground
                = new SolidColorBrush((Color)Application.Current.Resources["TextColorInactive"]);
            configTabGrid.Visibility = fanTabGrid.Visibility = pumpTabGrid.Visibility = ledTabGrid.Visibility = Visibility.Hidden;

            ((Label)sender).Foreground = new SolidColorBrush((Color)Application.Current.Resources["TextColorActive"]);
            ((Grid)((Label)sender).Tag).Visibility = Visibility.Visible;
        }

        private void FanApplyPress(object sender, RoutedEventArgs e) {
            if (App.configStream is null) return;

            byte[] report = new byte[App.REPORT_SIZE];
            Array.Fill<byte>(report, 0);

            BitConverter.TryWriteBytes(new Span<byte>(report, 1, 4), App.REPORT_TAG_FAN);

            var pointCount = fanCurveEditor.CurvePoints.Count;
            BitConverter.TryWriteBytes(new Span<byte>(report, App.REPORT_FAN_OFFSET, 4), pointCount);
            for (int i = 0; i < pointCount; i++) {
                var point = fanCurveEditor.CurvePoints[i];
                BitConverter.TryWriteBytes(new Span<byte>(report, App.REPORT_FAN_OFFSET + 4 + i * 4, 4), (float)point.X);
                report[App.REPORT_FAN_OFFSET + 52 + i] = (byte)(point.Y * 2.55f);
            }

            for (int i = 0; i < 10; i++) {
                try {
                    lock (App.configHidLock) {
                        App.configStream.Write(report);
                        break;
                    }
                } catch {
                    Thread.Sleep(10);
                }
            }

            UpdateConfigFromDevice();
        }

        private void PumpApplyPress(object sender, RoutedEventArgs e) {
            if (App.configStream is null) return;

            byte[] report = new byte[App.REPORT_SIZE];
            Array.Fill<byte>(report, 0);

            BitConverter.TryWriteBytes(new Span<byte>(report, 1, 4), App.REPORT_TAG_PUMP);

            var pointCount = pumpCurveEditor.CurvePoints.Count;
            BitConverter.TryWriteBytes(new Span<byte>(report, App.REPORT_PUMP_OFFSET, 4), pointCount);
            for (int i = 0; i < pointCount; i++) {
                var point = pumpCurveEditor.CurvePoints[i];
                BitConverter.TryWriteBytes(new Span<byte>(report, App.REPORT_PUMP_OFFSET + 4 + i * 4, 4), (float)point.X);
                report[App.REPORT_PUMP_OFFSET + 52 + i] = (byte)(point.Y * 2.55f);
            }

            for (int i = 0; i < 10; i++) {
                try {
                    lock (App.configHidLock) {
                        App.configStream.Write(report);
                        break;
                    }
                } catch {
                    Thread.Sleep(10);
                }
            }

            UpdateConfigFromDevice();
        }
    }
}
