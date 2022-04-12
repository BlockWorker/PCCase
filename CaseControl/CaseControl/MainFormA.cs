using HidSharp;
using System.Runtime.InteropServices;

namespace CaseControl {
    public partial class MainFormA : Form {

        private DeviceList devList;
        private HidDevice? connectedDevice = null;
        private HidStream? hidStream = null;

        private int fanPoints = 0;
        private int pumpPoints = 0;
        private int wdInt = 0;
        private int wdRun = 0;
        private float accMinFlow = 0;
        private float accMaxFlow = 0;
        private float nomMinFlow = 0;
        private float nomMaxFlow = 0;

        public MainFormA() {
            InitializeComponent();
        }

        private void UpdateDeviceList() {
            deviceBox.Items.Clear();
            var devices = devList.GetHidDevices(0x1209, 0x5001).Where(d => d.GetReportDescriptor().DeviceItems.FirstOrDefault()?.Usages.ContainsValue(0xff200001u) ?? false);
            foreach (var device in devices) {
                deviceBox.Items.Add(device);
            }
        }

        private void MainForm_Load(object sender, EventArgs e) {
            devList = DeviceList.Local;
            UpdateDeviceList();
        }

        private void refreshButton_Click(object sender, EventArgs e) {
            UpdateDeviceList();
        }

        private void connectButton_Click(object sender, EventArgs e) {
            connectedDevice = deviceBox.SelectedItem as HidDevice;
            if (connectedDevice == null) return;
            if (!connectedDevice.TryOpen(out hidStream)) {
                connectedDevice = null;
                hidStream = null;
                MessageBox.Show("Failed to connect.");
                return;
            }
        }

        private void button1_Click(object sender, EventArgs e) {
            if (connectedDevice == null || hidStream == null) return;

            byte[] report = new byte[205];
            report[0] = 0;

            for (int i = 0; i < 10; i++) {
                try {
                    hidStream.GetFeature(report);
                    break;
                } catch {
                    Thread.Sleep(100);
                }
            }
            
            fanPoints = BitConverter.ToInt32(report, 5);
            pumpPoints = BitConverter.ToInt32(report, 69);
            wdInt = BitConverter.ToInt32(report, 133);
            wdRun = BitConverter.ToInt32(report, 137);
            accMinFlow = BitConverter.ToSingle(report, 141);
            accMaxFlow = BitConverter.ToSingle(report, 157);
            nomMinFlow = BitConverter.ToSingle(report, 173);
            nomMaxFlow = BitConverter.ToSingle(report, 189);

            label1.Text = $"Fan: {fanPoints}, Pump: {pumpPoints}, WDInt: {wdInt}, WDRun: {wdRun}, Acc Flow: {accMinFlow} - {accMaxFlow}, Nom Flow: {nomMinFlow} - {nomMaxFlow}";
        }

        private void button2_Click(object sender, EventArgs e) {
            if (connectedDevice == null || hidStream == null) return;

            byte[] report = new byte[205];
            report[0] = 0;

            report[1] = 0x08;

            int runtime = (int)numericUpDown1.Value;

            BitConverter.TryWriteBytes(new Span<byte>(report, 137, 4), runtime);

            hidStream.Write(report);
        }
    }
}