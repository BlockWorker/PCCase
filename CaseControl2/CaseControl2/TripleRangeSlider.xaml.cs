using System;
using System.Collections.Generic;
using System.Globalization;
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

namespace CaseControl2 {
    /// <summary>
    /// Interaction logic for TripleRangeSlider.xaml
    /// </summary>
    public partial class TripleRangeSlider : UserControl {

        private readonly Brush backColorBrush;
        private readonly Brush borderColorBrush;
        private readonly Brush labelColorBrush;
        private readonly Brush handleColorBrush;
        private readonly Brush goodColorBrush;
        private readonly Brush warnColorBrush;
        private readonly Brush softOffColorBrush;
        private readonly Brush hardOffColorBrush;
        private readonly Brush hintColorBrush;

        private const double HANDLE_SIZE = 6; //size ("radius") of curve point handles
        private const double SIDE_OFFSET = 10;
        private const double TOP_OFFSET = 7;

        public Point TotalRange { get; set; } = new Point(0d, 100d);
        public double TickStep { get; set; } = 10d;
        public Point GoodRange { get; set; } = new Point(30d, 70d);
        public Point WarnRange { get; set; } = new Point(20d, 80d);
        public Point SoftOffRange { get; set; } = new Point(10d, 90d);

        public TripleRangeSlider() {
            InitializeComponent();

            App.MeasurementNewValues += (_, _) => Dispatcher.Invoke(InvalidateVisual);

            backColorBrush = new SolidColorBrush((Color)Resources["BackColor"]);
            borderColorBrush = new SolidColorBrush((Color)Resources["BorderColor"]);
            labelColorBrush = new SolidColorBrush((Color)Resources["LabelColor"]);
            handleColorBrush = new SolidColorBrush((Color)Resources["HandleColor"]);
            goodColorBrush = new SolidColorBrush((Color)Resources["GoodColor"]);
            warnColorBrush = new SolidColorBrush((Color)Resources["WarnColor"]);
            softOffColorBrush = new SolidColorBrush((Color)Resources["SoftOffColor"]);
            hardOffColorBrush = new SolidColorBrush((Color)Resources["HardOffColor"]);
            hintColorBrush = new SolidColorBrush((Color)Resources["HintColor"]);
        }

        protected override void OnRender(DrawingContext drawingContext) {
            base.OnRender(drawingContext);

            var width = ActualWidth;
            var height = ActualHeight;

            var typeface = new Typeface(FontFamily, FontStyle, FontWeight, FontStretch);

            var borderPen = new Pen(borderColorBrush, 7);
            var backPen = new Pen(backColorBrush, 5);
            borderPen.StartLineCap = borderPen.EndLineCap = backPen.StartLineCap = backPen.EndLineCap = PenLineCap.Round;
            var labelPen = new Pen(labelColorBrush, 1);

            //draw slider tick markings and labels once every TickStep
            var steps = (int)((TotalRange.Y - TotalRange.X) / TickStep);
            for (int i = 0; i < steps; i++) {
                var value = TotalRange.X + i * TickStep;
                var pos = GetValuePosition(value);
                var labelText = new FormattedText($"{value:F0}", CultureInfo.InvariantCulture, FlowDirection.LeftToRight, typeface, 10, labelColorBrush, 1);

                drawingContext.DrawLine(labelPen, new Point(pos, TOP_OFFSET), new Point(pos, TOP_OFFSET + 9));
                drawingContext.DrawText(labelText, new Point(pos - labelText.Width / 2d, TOP_OFFSET + 8));
            }
            //last tick/label at end of range
            var lastLabelText = new FormattedText($"{TotalRange.Y:F0}", CultureInfo.InvariantCulture, FlowDirection.LeftToRight, typeface, 10, labelColorBrush, 1);
            drawingContext.DrawLine(labelPen, new Point(width - SIDE_OFFSET, TOP_OFFSET), new Point(width - SIDE_OFFSET, TOP_OFFSET + 9));
            drawingContext.DrawText(lastLabelText, new Point(width - SIDE_OFFSET - lastLabelText.Width / 2d, TOP_OFFSET + 8));

            //draw border and background
            drawingContext.DrawLine(borderPen, new Point(SIDE_OFFSET, TOP_OFFSET), new Point(width - SIDE_OFFSET, TOP_OFFSET));
            drawingContext.DrawLine(backPen, new Point(SIDE_OFFSET, TOP_OFFSET), new Point(width - SIDE_OFFSET, TOP_OFFSET));

            //draw ranges
            var penThickness = 3;
            var goodPen = new Pen(goodColorBrush, penThickness);
            var warnPen = new Pen(warnColorBrush, penThickness);
            var softOffPen = new Pen(softOffColorBrush, penThickness);
            var hardOffPen = new Pen(hardOffColorBrush, penThickness);
            hardOffPen.StartLineCap = hardOffPen.EndLineCap = PenLineCap.Round;

            drawingContext.DrawLine(hardOffPen, new Point(SIDE_OFFSET, TOP_OFFSET), new Point(GetValuePosition(SoftOffRange.X), TOP_OFFSET));
            drawingContext.DrawLine(hardOffPen, new Point(width - SIDE_OFFSET, TOP_OFFSET), new Point(GetValuePosition(SoftOffRange.Y), TOP_OFFSET));
            drawingContext.DrawLine(softOffPen, new Point(GetValuePosition(SoftOffRange.X), TOP_OFFSET), new Point(GetValuePosition(WarnRange.X), TOP_OFFSET));
            drawingContext.DrawLine(softOffPen, new Point(GetValuePosition(SoftOffRange.Y), TOP_OFFSET), new Point(GetValuePosition(WarnRange.Y), TOP_OFFSET));
            drawingContext.DrawLine(warnPen, new Point(GetValuePosition(WarnRange.X), TOP_OFFSET), new Point(GetValuePosition(GoodRange.X), TOP_OFFSET));
            drawingContext.DrawLine(warnPen, new Point(GetValuePosition(WarnRange.Y), TOP_OFFSET), new Point(GetValuePosition(GoodRange.Y), TOP_OFFSET));
            drawingContext.DrawLine(goodPen, new Point(GetValuePosition(GoodRange.X), TOP_OFFSET), new Point(GetValuePosition(GoodRange.Y), TOP_OFFSET));

            //draw handles
            var handlePen = new Pen(borderColorBrush, 1);
            drawingContext.DrawEllipse(softOffColorBrush, handlePen, new Point(GetValuePosition(SoftOffRange.X), TOP_OFFSET), HANDLE_SIZE, HANDLE_SIZE);
            drawingContext.DrawEllipse(softOffColorBrush, handlePen, new Point(GetValuePosition(SoftOffRange.Y), TOP_OFFSET), HANDLE_SIZE, HANDLE_SIZE);
            drawingContext.DrawEllipse(warnColorBrush, handlePen, new Point(GetValuePosition(WarnRange.X), TOP_OFFSET), HANDLE_SIZE, HANDLE_SIZE);
            drawingContext.DrawEllipse(warnColorBrush, handlePen, new Point(GetValuePosition(WarnRange.Y), TOP_OFFSET), HANDLE_SIZE, HANDLE_SIZE);
            drawingContext.DrawEllipse(goodColorBrush, handlePen, new Point(GetValuePosition(GoodRange.X), TOP_OFFSET), HANDLE_SIZE, HANDLE_SIZE);
            drawingContext.DrawEllipse(goodColorBrush, handlePen, new Point(GetValuePosition(GoodRange.Y), TOP_OFFSET), HANDLE_SIZE, HANDLE_SIZE);
        }

        private void OnMouseDown(object sender, MouseButtonEventArgs e) {

        }

        private void OnMouseUp(object sender, MouseButtonEventArgs e) {

        }

        private void OnMouseMove(object sender, MouseEventArgs e) {

        }

        /// <returns>The drawing position for the given data value</returns>
        private double GetValuePosition(double value) {
            return (ActualWidth - 2d * SIDE_OFFSET) * (value - TotalRange.X) / (TotalRange.Y - TotalRange.X) + SIDE_OFFSET;
        }

        /// <returns>The data value for the given drawing position</returns>
        private double GetPositionValue(double position) {
            return (TotalRange.Y - TotalRange.X) * (position - SIDE_OFFSET) / (ActualWidth - 2d * SIDE_OFFSET) + TotalRange.X;
        }
    }
}
