using HidSharp;

namespace CaseControl {
    public partial class TestFormB : Form {

        private DeviceList devList;
        private HidDevice? connectedDevice = null;
        private HidStream? hidStream = null;
        private int timer = 1000;
        private DateTime start;

        private byte[] colors = new byte[769 * 4];
        private int counter = 0;
        private bool running = false;

        public TestFormB() {
            InitializeComponent();
        }

        private void UpdateDeviceList() {
            deviceBox.Items.Clear();
            var devices = devList.GetHidDevices(0x1209, 0x5001).Where(d => d.GetReportDescriptor().DeviceItems.FirstOrDefault()?.Usages.ContainsValue(0xff590001u) ?? false);
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

            byte[] report = new byte[2];
            report[0] = 0x06;

            hidStream.GetFeature(report);

            report[1] = (byte)(0x1 - report[1]);

            hidStream.SetFeature(report);
        }

        private async void SendColorReport(uint startIndex, uint count, bool finish) {
            if (connectedDevice == null || hidStream == null) return;

            byte[] report = new byte[69];
            report[0] = 0x07;
            report[1] = (byte)(startIndex & 0xff);
            report[2] = (byte)((startIndex >> 8) & 0xff);
            report[3] = (byte)count;

            Array.Copy(colors, startIndex * 4, report, 4, count * 4);

            report[68] = finish ? (byte)1 : (byte)0;

            await hidStream.WriteAsync(report);
        }

        private void SendColors() {
            if (connectedDevice == null || hidStream == null) return;

            for (uint i = 0; i < 48; i++) {
                SendColorReport(16 * i, 16, false);
            }
            SendColorReport(768, 1, true);
        }

        private void button2_Click(object sender, EventArgs e) {
            if (running) {
                running = false;
            } else {
                running = true;
                Task.Run(() => RunTimer());
                start = DateTime.Now;
                timer = 0;
            }
        }

        private void DoUpdate() {
            for (int i = 0; i < 10; i++) {
                int index = 4 * i;
                colors[index] = 40;
                colors[index + 1] = 40;
                colors[index + 2] = 40;
                colors[index + 3] = (byte)((counter >> i) & 0x01);
            }
            counter++;

            for (int i = 10; i < 769; i++) {
                int index = 4 * i;
                colors[index] = (byte)Random.Shared.Next(30);
                colors[index + 1] = (byte)Random.Shared.Next(30);
                colors[index + 2] = (byte)Random.Shared.Next(30);
                colors[index + 3] = 1;
            }

            SendColors();
        }

        private void RunTimer() {
            while (running) {
                Task.Run(() => DoUpdate());
                if (++timer >= 1000) {
                    var dt = DateTime.Now;

                    label1.Invoke(() => { label1.Text = (dt - start).TotalSeconds.ToString() + "s"; });

                    start = dt;
                    timer = 0;
                }
                Thread.Sleep(15);
            }
        }
    }
}