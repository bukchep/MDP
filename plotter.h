#ifndef PLOTTER_H
#define PLOTTER_H

#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>

class QToolButton;
class PlotSettings;

class Plotter : public QWidget
{
    Q_OBJECT

public:
    Plotter(QWidget *parent = 0);

    QPalette *pal1;

    void setPlotSettings(const PlotSettings &settings);
    void setCurveData(int id, const QVector<QPointF> &data);
    void clearCurve(int id);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void Scroll(int n);
    bool ExtendX();
    bool CompressX();

public slots:
    void zoomIn();
    void zoomOut();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void updateRubberBandRegion();
    void refreshPixmap();
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);

    enum { MarginX = 65 };
    enum { MarginY = 15 };

    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;
    QMap<int, QVector<QPointF> > curveMap;

    bool rubberBandIsShown;
    QRect rubberBandRect;
    QPixmap pixmap;
public:
    QVector<PlotSettings> zoomStack;
    int curZoom;

    double StepScroll;
signals:
    void Zoom();
};

class PlotSettings
{
public:
    PlotSettings();

    void scroll(int dx, int dy);
    void adjust();
    double spanX() const { return maxX - minX; }
    double spanY() const { return maxY - minY; }


    double minX;
    double maxX;
    int numXTicks;
    double minY;
    double maxY;
    int numYTicks;
    double RangeXmin;
    double RangeXmax;
    double RangeYmin;
    double RangeYmax;

private:
    static void adjustAxis(double &min, double &max, int &numTicks);
};

#endif
