using HidSharp;

namespace CaseControl {
    internal static class Program {

        public static object configHidLock = new();
        public static object ledHidLock = new();

        public static HidDevice? configDevice = null;
        public static HidDevice? ledDevice = null;
        public static HidStream? configStream = null;
        public static HidStream? ledStream = null;

        public static event EventHandler? DevicesChanged;

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

        /// <summary>
        ///  The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main() {
            ApplicationConfiguration.Initialize();
            DeviceList.Local.Changed += (_, _) => UpdateDevices();
            Application.Run(new MainForm());
        }
    }
}