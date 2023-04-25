using HidSharp;
using System.Runtime.InteropServices;

namespace CaseControl {
    public partial class TestFormA : Form {

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
        private int effect = 0;

        public TestFormA() {
            InitializeComponent();
        }

        private void ReadInputRep() {
            while (hidStream != null) {
                try {
                    byte[] report = hidStream.Read();
                    float fanRPM = BitConverter.ToSingle(report, 1);
                    float pumpRPM = BitConverter.ToSingle(report, 5);
                    float flowLPH = BitConverter.ToSingle(report, 9);
                    float tempC = BitConverter.ToSingle(report, 13);

                    Invoke(() => {
                        label4.Text = $"Current Values: Fan {fanRPM} RPM, Pump {pumpRPM} RPM, Flow {flowLPH} l/h, Temp {tempC} °C";
                    });
                } catch {

                }
                Thread.Sleep(200);
            }
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
            Task.Run(() => ReadInputRep());
        }

        private void button1_Click(object sender, EventArgs e) {
            if (connectedDevice == null || hidStream == null) return;

            byte[] report = new byte[237];
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
            effect = BitConverter.ToInt32(report, 205);


            label1.Text = $"Fan: {fanPoints}, Pump: {pumpPoints}, WDInt: {wdInt}, WDRun: {wdRun}, Acc Flow: {accMinFlow} - {accMaxFlow}, Nom Flow: {nomMinFlow} - {nomMaxFlow}, Effect {effect}";
        }

        private void button2_Click(object sender, EventArgs e) {
            if (connectedDevice == null || hidStream == null) return;

            byte[] report = new byte[237];
            report[0] = 0;

            report[1] = 0x08;

            int runtime = (int)numericUpDown1.Value;

            BitConverter.TryWriteBytes(new Span<byte>(report, 137, 4), runtime);

            hidStream.Write(report);
        }

        private void button3_Click(object sender, EventArgs e) {
            if (connectedDevice == null || hidStream == null) return;

            byte[] report = new byte[237];
            report[0] = 0;

            report[1] = 0x00;
            report[2] = 0x01;

            int index = (int)numericUpDown2.Value;
            var boxes = new TextBox[] { textBox1, textBox2, textBox3, textBox4 };
            var values = new uint[4];
            for (int i = 0; i < 4; i++) {
                var text = boxes[i].Text;
                if (text.StartsWith("0x")) {
                    values[i] = uint.Parse(text[2..], System.Globalization.NumberStyles.AllowHexSpecifier);
                } else {
                    values[i] = uint.Parse(text);
                }
            }

            BitConverter.TryWriteBytes(new Span<byte>(report, 205, 4), index);
            for (int i = 0; i < 4; i++) BitConverter.TryWriteBytes(new Span<byte>(report, 209 + (4 * i), 4), values[i]);

            hidStream.Write(report);
        }

        private void button4_Click(object sender, EventArgs e) {
            if (connectedDevice == null || hidStream == null) return;

            byte[] report = new byte[237];
            report[0] = 0;

            report[1] = 0x02;
            report[2] = 0x00;

            Array.Fill(report, (byte)0, 3, 234);

            int value = (int)numericUpDown3.Value;
            if (value > 0) {
                report[69] = 0x01;
                report[121] = (byte)value;
            }

            hidStream.Write(report);
        }

        private void button5_Click(object sender, EventArgs e) {
            if (connectedDevice == null || hidStream == null) return;

            byte[] report = new byte[237];
            report[0] = 0;

            report[1] = 0x01;
            report[2] = 0x00;

            Array.Fill(report, (byte)0, 3, 234);

            int value = (int)numericUpDown4.Value;
            if (value > 0) {
                report[5] = 0x01;
                report[57] = (byte)value;
            }

            hidStream.Write(report);
        }
    }
}