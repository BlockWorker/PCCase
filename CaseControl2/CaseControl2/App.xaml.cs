using HidSharp;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Globalization;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace CaseControl2 {
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application {

        public const int REPORT_SIZE = 237;
        public const int REPORT_FAN_OFFSET = 5;
        public const int REPORT_PUMP_OFFSET = 69;
        public const int REPORT_TAG_FAN = 0x001;
        public const int REPORT_TAG_PUMP = 0x002;


        private bool running = true;

        public static object configHidLock = new();
        public static object ledHidLock = new();

        public static HidDevice? configDevice = null;
        public static HidDevice? ledDevice = null;
        public static HidStream? configStream = null;
        public static HidStream? ledStream = null;

        public static float fanRPM = float.NaN;
        public static float pumpRPM = float.NaN;
        public static float flowLPH = float.NaN;
        public static float tempC = float.NaN;

        public static List<Point> fanPoints = new();
        public static List<Point> pumpPoints = new();

        public static event EventHandler? DevicesChanged;
        public static event EventHandler? MeasurementNewValues;

        public static void UpdateDevices() {
            var changed = false;

            var devices = DeviceList.Local.GetHidDevices(0x1209, 0x5001);

            var configDeviceOptions = devices.Where(d => d.GetReportDescriptor().DeviceItems.FirstOrDefault()?.Usages.ContainsValue(0xff200001u) ?? false).ToList();
            var ledDeviceOptions = devices.Where(d => d.GetReportDescriptor().DeviceItems.FirstOrDefault()?.Usages.ContainsValue(0xff590001u) ?? false).ToList();

            lock (configHidLock) {
                lock (ledHidLock) {
                    if (configDevice != null && configDeviceOptions.Count == 0) {
                        configStream?.Close();
                        configStream = null;
                        configDevice = null;
                        changed = true;
                    } else if (configDevice == null && configDeviceOptions.Count > 0) {
                        configDevice = configDeviceOptions[0];
                        if (!configDevice.TryOpen(out configStream)) {
                            configDevice = null;
                            configStream = null;
                        } else {
                            changed = true;
                        }
                    }

                    if (ledDevice != null && ledDeviceOptions.Count == 0) {
                        ledStream?.Close();
                        ledStream = null;
                        ledDevice = null;
                        changed = true;
                    } else if (ledDevice == null && ledDeviceOptions.Count > 0) {
                        ledDevice = ledDeviceOptions[0];
                        if (!ledDevice.TryOpen(out ledStream)) {
                            ledDevice = null;
                            ledStream = null;
                        } else {
                            changed = true;
                        }
                    }

                    if (changed) DevicesChanged?.Invoke(null, EventArgs.Empty);
                }
            }
        }

        private void ReadInputRep() {
            while (running) {
                lock (configHidLock) {
                    if (configStream != null) {
                        try {
                            byte[] report = configStream.Read();
                            fanRPM = BitConverter.ToSingle(report, 1);
                            pumpRPM = BitConverter.ToSingle(report, 5);
                            flowLPH = BitConverter.ToSingle(report, 9);
                            tempC = BitConverter.ToSingle(report, 13);
                        } catch {
                            fanRPM = pumpRPM = flowLPH = tempC = float.NaN;
                        }
                    } else {
                        fanRPM = pumpRPM = flowLPH = tempC = float.NaN;
                    }
                }
                MeasurementNewValues?.Invoke(null, EventArgs.Empty);
                Thread.Sleep(200);
            }
        }

        public static void ReadConfig() {
            byte[] report = new byte[REPORT_SIZE];
            report[0] = 0;

            for (int i = 0; i < 10; i++) {
                try {
                    lock (configHidLock) {
                        if (configStream is null) return;

                        configStream.GetFeature(report);
                        break;
                    }
                } catch {
                    Thread.Sleep(10);
                }
            }

            fanPoints.Clear();
            var fanPointCount = BitConverter.ToInt32(report, REPORT_FAN_OFFSET);
            fanPoints.EnsureCapacity(fanPointCount);
            for (int i = 0; i < fanPointCount; i++) {
                var temp = Math.Clamp((double)BitConverter.ToSingle(report, REPORT_FAN_OFFSET + 4 + i * 4), 20d, 60d);
                var speed = Math.Clamp(report[REPORT_FAN_OFFSET + 52 + i] / 2.55d, 0d, 100d);
                fanPoints.Add(new Point(temp, speed));
            }

            pumpPoints.Clear();
            var pumpPointCount = BitConverter.ToInt32(report, REPORT_PUMP_OFFSET);
            pumpPoints.EnsureCapacity(pumpPointCount);
            for (int i = 0; i < pumpPointCount; i++) {
                var temp = Math.Clamp((double)BitConverter.ToSingle(report, REPORT_PUMP_OFFSET + 4 + i * 4), 20d, 60d);
                var speed = Math.Clamp(report[REPORT_PUMP_OFFSET + 52 + i] / 2.55d, 0d, 100d);
                pumpPoints.Add(new Point(temp, speed));
            }
        }

        protected override void OnStartup(StartupEventArgs e) {
            base.OnStartup(e);

            DeviceList.Local.Changed += (_, _) => UpdateDevices();
            UpdateDevices();

            running = true;
            Task.Run(ReadInputRep);
        }

        protected override void OnExit(ExitEventArgs e) {
            base.OnExit(e);
            running = false;
        }
    }
}
