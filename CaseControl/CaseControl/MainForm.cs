using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Globalization;

namespace CaseControl {
    public partial class MainForm : Form {

        private bool closing = false;

        public MainForm() {
            InitializeComponent();
        }

        private void ReadInputRep() {
            while (!closing) {
                lock (Program.configHidLock) {
                    if (Program.configStream != null) {
                        try {
                            byte[] report = Program.configStream.Read();
                            float fanRPM = BitConverter.ToSingle(report, 1);
                            float pumpRPM = BitConverter.ToSingle(report, 5);
                            float flowLPH = BitConverter.ToSingle(report, 9);
                            float tempC = BitConverter.ToSingle(report, 13);

                            Invoke(() => {
                                fanSpeedLabel.Text = string.Format(CultureInfo.InvariantCulture, "{0:F0} RPM", fanRPM);
                                pumpSpeedLabel.Text = string.Format(CultureInfo.InvariantCulture, "{0:F0} RPM", pumpRPM);
                                flowRateLabel.Text = string.Format(CultureInfo.InvariantCulture, "{0:F1} l/h", flowLPH);
                                tempLabel.Text = string.Format(CultureInfo.InvariantCulture, "{0:F1} °C", tempC);
                            });
                        } catch {

                        }
                    }
                }
                Thread.Sleep(200);
            }
        }

        private void MainForm_Load(object sender, EventArgs e) {
            closing = false;
            Task.Run(ReadInputRep);
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            closing = true;
        }
    }
}
