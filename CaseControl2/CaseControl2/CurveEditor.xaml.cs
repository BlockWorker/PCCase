using CaseControl2.Properties;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.CompilerServices;

namespace CaseControl2 {
    /// <summary>
    /// Interaction logic for CurveEditor.xaml
    /// </summary>
    public partial class CurveEditor : UserControl {

        private readonly Brush backColorBrush;
        private readonly Brush borderColorBrush;
        private readonly Brush ruleColorBrush;
        private readonly Brush pointColorBrush;
        private readonly Brush lineColorBrush;
        private readonly Brush shadeColorBrush;
        private readonly Brush hintColorBrush;

        private const double HANDLE_SIZE = 5; //size ("radius") of curve point handles
        private const int MAX_POINTS = 12; //maximum number of curve points

        public List<Point> CurvePoints { get; private set; } = new List<Point>();

        private int draggedPoint = -1;
        private int hoveredPoint = -1;


        public CurveEditor() {
            InitializeComponent();

            App.MeasurementNewValues += (_, _) => Dispatcher.Invoke(InvalidateVisual);

            backColorBrush = new SolidColorBrush((Color)Resources["BackColor"]);
            borderColorBrush = new SolidColorBrush((Color)Resources["BorderColor"]);
            ruleColorBrush = new SolidColorBrush((Color)Resources["RuleColor"]);
            pointColorBrush = new SolidColorBrush((Color)Resources["PointColor"]);
            lineColorBrush = new SolidColorBrush((Color)Resources["LineColor"]);
            shadeColorBrush = new SolidColorBrush((Color)Resources["ShadeColor"]);
            hintColorBrush = new SolidColorBrush((Color)Resources["HintColor"]);
        }

        protected override void OnRender(DrawingContext drawingContext) {
            base.OnRender(drawingContext);

            var width = ActualWidth;
            var height = ActualHeight;

            drawingContext.DrawRectangle(backColorBrush, new Pen(borderColorBrush, 1), new Rect(0, 0, width, height)); //background and border

            var typeface = new Typeface(FontFamily, FontStyle, FontWeight, FontStretch);
            var rulePen = new Pen(ruleColorBrush, 1);

            //edge labels
            drawingContext.DrawText(new FormattedText("100%", CultureInfo.InvariantCulture, FlowDirection.LeftToRight, typeface, 10, ruleColorBrush, 1),
                new Point(4, 2));
            drawingContext.DrawText(new FormattedText("0%".ToString(), CultureInfo.InvariantCulture, FlowDirection.LeftToRight, typeface, 10, ruleColorBrush, 1),
                new Point(4, height - 25));
            drawingContext.DrawText(new FormattedText("20°C", CultureInfo.InvariantCulture, FlowDirection.LeftToRight, typeface, 10, ruleColorBrush, 1),
                new Point(15, height - 15));
            drawingContext.DrawText(new FormattedText("60°C", CultureInfo.InvariantCulture, FlowDirection.LeftToRight, typeface, 10, ruleColorBrush, 1),
                new Point(width - 25, height - 15));

            //temp labels and rules
            for (int i = 3; i <= 5; i++) {
                var x = (i - 2) * width / 4f;
                var text = $"{i * 10}°C";

                drawingContext.DrawText(new FormattedText(text, CultureInfo.InvariantCulture, FlowDirection.LeftToRight, typeface, 10, ruleColorBrush, 1),
                    new Point(x - 10, height - 15));
                drawingContext.DrawLine(rulePen, new Point(x, 0), new Point(x, height - 14));
                drawingContext.DrawLine(rulePen, new Point(x, height - 2), new Point(x, height));
            }

            //speed labels and rules
            for (int i = 1; i <= 4; i++) {
                var y = i * height / 5f;
                var text = $"{100 - i * 20}%";

                drawingContext.DrawText(new FormattedText(text, CultureInfo.InvariantCulture, FlowDirection.LeftToRight, typeface, 10, ruleColorBrush, 1),
                    new Point(4, y - 7));
                drawingContext.DrawLine(rulePen, new Point(0, y), new Point(2, y));
                drawingContext.DrawLine(rulePen, new Point(25, y), new Point(width, y));
            }

            //hint lines for current value
            if (!float.IsNaN(App.tempC)) {
                var temp = Math.Clamp(App.tempC, 20d, 60d);
                var hintPen = new Pen(hintColorBrush, 1);
                hintPen.DashStyle = new DashStyle(new double[] {4, 4}, 0);
                var intersection = DataToScreenPoint(new Point(temp, GetCurveValue(temp)));
                drawingContext.DrawLine(hintPen, new Point(intersection.X, 0), new Point(intersection.X, height));
                drawingContext.DrawLine(hintPen, new Point(0, intersection.Y), new Point(width, intersection.Y));
            }

            //actual curve - shading, line, handles
            var linePen = new Pen(lineColorBrush, 2);

            if (CurvePoints.Count == 0) { //no points: equivalent to always 100% speed - so draw that
                drawingContext.DrawRectangle(shadeColorBrush, null, new Rect(0, 1, width, height - 1));
                drawingContext.DrawLine(linePen, new Point(0, 1), new Point(width, 1));
            } else if (CurvePoints.Count == 1) { //one point: equivalent to always that speed - so draw that, along with point
                var screenPoint = DataToScreenPoint(CurvePoints[0]);

                drawingContext.DrawRectangle(shadeColorBrush, null, new Rect(0, screenPoint.Y, width, height - screenPoint.Y));
                drawingContext.DrawLine(linePen, new Point(0, screenPoint.Y), new Point(width, screenPoint.Y));
                drawingContext.DrawRectangle(pointColorBrush, null, new Rect(screenPoint.X - HANDLE_SIZE, screenPoint.Y - HANDLE_SIZE, 2 * HANDLE_SIZE, 2 * HANDLE_SIZE));
            } else { //more than one point: constant below first and above last point, piecewise linear between points
                var poly = new StreamGeometry(); //create and draw polygon for shading
                using (var gc = poly.Open()) {
                    gc.BeginFigure(new Point(width, height), true, true);
                    gc.LineTo(new Point(width, DataToScreenPoint(CurvePoints.Last()).Y), false, true);
                    for (int i = CurvePoints.Count - 1; i >= 0; i--) {
                        gc.LineTo(DataToScreenPoint(CurvePoints[i]), false, true);
                    }
                    gc.LineTo(new Point(0, DataToScreenPoint(CurvePoints[0]).Y), false, true);
                    gc.LineTo(new Point(0, height), false, true);
                }
                drawingContext.DrawGeometry(shadeColorBrush, null, poly);

                for (int i = 1; i < CurvePoints.Count; i++) { //draw actual value lines
                    var screenPoint1 = DataToScreenPoint(CurvePoints[i - 1]);
                    var screenPoint2 = DataToScreenPoint(CurvePoints[i]);
                    if (i == 1) { //constant line below first point
                        drawingContext.DrawLine(linePen, new Point(0, screenPoint1.Y), screenPoint1);
                    }
                    if (i == CurvePoints.Count - 1) { //constant line above last point
                        drawingContext.DrawLine(linePen, screenPoint2, new Point(width, screenPoint2.Y));
                    }
                    drawingContext.DrawLine(linePen, screenPoint1, screenPoint2);
                }

                foreach (var point in CurvePoints) { //draw point handles
                    var screenPoint = DataToScreenPoint(point);
                    drawingContext.DrawRectangle(pointColorBrush, null, new Rect(screenPoint.X - HANDLE_SIZE, screenPoint.Y - HANDLE_SIZE, 2 * HANDLE_SIZE, 2 * HANDLE_SIZE));
                }
            }

            //value text for hovered point
            if (hoveredPoint >= 0) {
                var point = CurvePoints[hoveredPoint];
                var coordText = new FormattedText($"{point.X:F1}°C: {point.Y:F0}%", CultureInfo.InvariantCulture, FlowDirection.LeftToRight, typeface, 10, Brushes.White, 1);

                var screenPoint = DataToScreenPoint(point);
                screenPoint.Offset(-coordText.Width / 2d, -coordText.Height - HANDLE_SIZE); //offset to be centered above handle, clamp to fit within curve area
                if (screenPoint.X < 0) screenPoint.X = 0;
                if (screenPoint.X > width - coordText.Width) screenPoint.X = width - coordText.Width;
                if (screenPoint.Y < 0) screenPoint.Y = 0;
                if (screenPoint.Y > height - coordText.Height) screenPoint.Y = height - coordText.Height;

                drawingContext.DrawText(coordText, screenPoint);
            }
        }

        private void MouseLeftDown(object sender, MouseButtonEventArgs e) {
            if (!IsEnabled) return;

            var pos = e.GetPosition(this);
            if (PointHandleHit(pos, out var index)) { //clicked on handle: start dragging that point
                draggedPoint = index;
                OnMouseMove(sender, e);
            } else if (CurvePoints.Count < MAX_POINTS && !PointHandleHit(pos, out _, HANDLE_SIZE * 4)) { //clicked "far" from existing handles: create point there and start dragging it
                CurvePoints.Add(new Point());
                draggedPoint = CurvePoints.Count - 1;
                OnMouseMove(sender, e);
            }
        }

        private void MouseLeftUp(object sender, MouseEventArgs e) { //released left button: stop dragging point
            draggedPoint = -1;
            CurvePoints.Sort((p1, p2) => (int)((p1.X - p2.X) * 1000));
            InvalidateVisual();
        }

        private void MouseRightUp(object sender, MouseButtonEventArgs e) {
            if (!IsEnabled) return;

            var pos = e.GetPosition(this);
            if (PointHandleHit(pos, out var index)) { //right clicked handle: delete that point
                draggedPoint = -1;
                hoveredPoint = -1;
                CurvePoints.RemoveAt(index);
                InvalidateVisual();
            }
        }

        private void OnMouseMove(object sender, MouseEventArgs e) {
            var pos = e.GetPosition(this);

            if (draggedPoint >= 0 && IsEnabled) { //drag point if necessary
                if (e.LeftButton != MouseButtonState.Pressed) { //missed mouse release? (if that is even possible)
                    MouseLeftUp(sender, e);
                } else { //mouse held - drag point
                    //new point position at mouse position, clamped to within curve area
                    var newPoint = ScreenToDataPoint(pos);
                    if (newPoint.X < 20d) newPoint.X = 20d;
                    if (newPoint.X > 60d) newPoint.X = 60d;
                    if (newPoint.Y < 0d) newPoint.Y = 0d;
                    if (newPoint.Y > 100d) newPoint.Y = 100d;

                    if (CurvePoints[draggedPoint] != newPoint) { //update point and visual if moved
                        CurvePoints[draggedPoint] = newPoint;

                        //sort curve points by X position - in two steps: sort indices, then apply new indices - allows us to update dragged point index along with the sort
                        var indices = Enumerable.Range(0, CurvePoints.Count).OrderBy(i => CurvePoints[i].X).ToList();
                        CurvePoints = indices.Select(i => CurvePoints[i]).ToList();
                        draggedPoint = indices.IndexOf(draggedPoint);
                        InvalidateVisual();
                    }
                }
            }

            int newHoverPoint = -1;
            if (PointHandleHit(pos, out var index)) { //check for hovered point
                newHoverPoint = index;
            } else {
                newHoverPoint = -1;
            }

            if (hoveredPoint != newHoverPoint) { //update hovered point if necessary
                hoveredPoint = newHoverPoint;
                InvalidateVisual();
            }
        }

        private void OnMouseLeave(object sender, MouseEventArgs e) {
            hoveredPoint = -1;
            InvalidateVisual();
        }

        /// <summary>
        /// Basic hit check for a point handle
        /// </summary>
        private bool PointHandleHit(Point testPoint, out int hitIndex, double range = HANDLE_SIZE) {
            for (int i = 0; i < CurvePoints.Count; i++) {
                var p = DataToScreenPoint(CurvePoints[i]);
                if (Math.Abs(p.X - testPoint.X) < range && Math.Abs(p.Y - testPoint.Y) < range) {
                    hitIndex = i;
                    return true;
                }
            }
            hitIndex = -1;
            return false;
        }

        private Point ScreenToDataPoint(Point point) {
            var x = (point.X / ActualWidth) * 40d + 20d;
            var y = (1d - point.Y / ActualHeight) * 100d;
            return new Point(x, y);
        }

        private Point DataToScreenPoint(Point point) {
            var x = (point.X - 20d) / 40d * ActualWidth;
            var y = (1d - point.Y / 100d) * ActualHeight;
            return new Point(x, y);
        }

        private double GetCurveValue(double temp) {
            if (CurvePoints.Count == 0) return 100d;
            else if (CurvePoints.Count == 1) return CurvePoints[0].Y;
            else {
                if (temp <= CurvePoints[0].X) return CurvePoints[0].Y;
                else if (temp >= CurvePoints.Last().X) return CurvePoints.Last().Y;
                else {
                    int i;
                    for (i = 1; i < CurvePoints.Count; i++) {
                        if (CurvePoints[i].X >= temp) break;
                    }
                    var t = (temp - CurvePoints[i - 1].X) / (CurvePoints[i].X - CurvePoints[i - 1].X);
                    return t * CurvePoints[i].Y + (1d - t) * CurvePoints[i - 1].Y;
                }
            }
        }
    }
}
