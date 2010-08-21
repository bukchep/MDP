#include <QtGui>
#include <cmath>

#include "plotter.h"

Plotter::Plotter(QWidget *parent)
    : QWidget(parent)
{
   // setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
    //setStyle(new QPlastiqueStyle);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
    rubberBandIsShown = false;

    zoomOutButton = new QToolButton(this);
    zoomOutButton->setCursor(Qt::PointingHandCursor);
    //zoomOutButton->setStyle(new QPlastiqueStyle);
    //pal1 = new QPalette;
    //pal1->setColor(this->backgroundRole(),"#f7f9fe");
    //zoomOutButton->setPalette(*pal1);
    //zoomOutButton->setAutoFillBackground(true);
    zoomOutButton->setMinimumHeight(22);
    zoomOutButton->setMinimumWidth(22);
    zoomOutButton->setMaximumHeight(22);
    zoomOutButton->setMaximumWidth(22);
    zoomOutButton->setIcon(QIcon(":/images/previous.png"));
    zoomOutButton->adjustSize();
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));

    zoomInButton = new QToolButton(this);
    zoomInButton->setCursor(Qt::PointingHandCursor);
    zoomInButton->setMinimumHeight(22);
    zoomInButton->setMinimumWidth(22);
    zoomInButton->setMaximumHeight(22);
    zoomInButton->setMaximumWidth(22);
    zoomInButton->setIcon(QIcon(":/images/next.png"));
    zoomInButton->adjustSize();
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));

    setPlotSettings(PlotSettings());
}

void Plotter::setPlotSettings(const PlotSettings &settings)
{
    zoomStack.clear();
    zoomStack.append(settings);
    curZoom = 0;
    zoomInButton->hide();
    zoomOutButton->hide();
    refreshPixmap();
}

void Plotter::zoomOut()
{
    if (curZoom > 0) {
        --curZoom;
        zoomOutButton->setEnabled(curZoom > 0);
        zoomInButton->setEnabled(true);
        zoomInButton->show();
        refreshPixmap();  
    }
    emit Zoom();
}

void Plotter::zoomIn()
{
    if (curZoom < zoomStack.count() - 1) {
        ++curZoom;
        zoomInButton->setEnabled(curZoom < zoomStack.count() - 1);
        zoomOutButton->setEnabled(true);
        zoomOutButton->show();
        refreshPixmap();
    }
    emit Zoom();
}

void Plotter::setCurveData(int id, const QVector<QPointF> &data)
{

    curveMap[id] = data;
    refreshPixmap();
}

void Plotter::clearCurve(int id)
{
    curveMap.remove(id);
    refreshPixmap();
}

QSize Plotter::minimumSizeHint() const
{
    return QSize(10 * MarginX, 4 * MarginX);
    //return QSize(6 * MarginX, 4 * MarginX);
}

QSize Plotter::sizeHint() const
{
    return QSize(10 * MarginX, 5 * MarginX);
}

void Plotter::paintEvent(QPaintEvent * /* event */)
{
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, pixmap);

    if (rubberBandIsShown) {
        painter.setPen(QPen(Qt::black));
       // painter.setPen(palette().light().color());
        painter.drawRect(rubberBandRect.normalized()
                                       .adjusted(0, 0, -1, -1));
    }

    if (hasFocus()) {
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = palette().dark().color();
        painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
    }
}

void Plotter::resizeEvent(QResizeEvent * /* event */)
{
    int x = width() - (zoomInButton->width()
                       + zoomOutButton->width()+11);
    zoomOutButton->move(x, 19);
    zoomInButton->move(x + zoomInButton->width(), 19);
    refreshPixmap();
}

void Plotter::mousePressEvent(QMouseEvent *event)
{
    QRect rect(MarginX, MarginY,
               width() - 2 * MarginX, height() - 2 * MarginY);

    if (event->button() == Qt::LeftButton) {
        if (rect.contains(event->pos())) {
            rubberBandIsShown = true;
            rubberBandRect.setTopLeft(event->pos());
            rubberBandRect.setBottomRight(event->pos());
            updateRubberBandRegion();
            setCursor(Qt::CrossCursor);
        }
    }
}

void Plotter::mouseMoveEvent(QMouseEvent *event)
{
    if (rubberBandIsShown) {
        updateRubberBandRegion();
        rubberBandRect.setBottomRight(event->pos());
        updateRubberBandRegion();
    }
}

void Plotter::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton) && rubberBandIsShown) {
        rubberBandIsShown = false;
        updateRubberBandRegion();
        unsetCursor();

        QRect rect = rubberBandRect.normalized();
        if (rect.width() < 4 || rect.height() < 4)
            return;
        rect.translate(-MarginX, -MarginY);

        PlotSettings prevSettings = zoomStack[curZoom];
        PlotSettings settings;
        double dx = prevSettings.spanX() / (width() - 2 * MarginX);
        double dy = prevSettings.spanY() / (height() - 2 * MarginY);
        settings.minX = prevSettings.minX + dx * rect.left();
        settings.maxX = prevSettings.minX + dx * rect.right();
        settings.minY = prevSettings.maxY - dy * rect.bottom();
        settings.maxY = prevSettings.maxY - dy * rect.top();
        settings.adjust();

        zoomStack.resize(curZoom + 1);
        zoomStack.append(settings);
        zoomIn();
    }
}

void Plotter::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Left:
        zoomStack[curZoom].scroll(-1, 0);
        refreshPixmap();
        break;
    case Qt::Key_Right:
       zoomStack[curZoom].scroll(+1, 0);
       refreshPixmap();
        break;
    case Qt::Key_Down:
        zoomStack[curZoom].scroll(0, -1);
        refreshPixmap();
        break;
    case Qt::Key_Up:
        zoomStack[curZoom].scroll(0, +1);
        refreshPixmap();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void Plotter::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numTicks = numDegrees / 15;

    if (event->orientation() == Qt::Horizontal) {
        zoomStack[curZoom].scroll(numTicks, 0);
    } else {
        zoomStack[curZoom].scroll(0, numTicks);
    }
    refreshPixmap();
}

void Plotter::updateRubberBandRegion()
{
    QRect rect = rubberBandRect.normalized();
    update(rect.left(), rect.top(), rect.width(), 1);
    update(rect.left(), rect.top(), 1, rect.height());
    update(rect.left(), rect.bottom(), rect.width(), 1);
    update(rect.right(), rect.top(), 1, rect.height());
}

void Plotter::refreshPixmap()
{
    pixmap = QPixmap(size());
    pixmap.fill(this, 0, 0);

    QPainter painter(&pixmap);
    painter.initFrom(this);
    drawGrid(&painter);
    drawCurves(&painter);
    update();
}

void Plotter::drawGrid(QPainter *painter)
{
    painter->setBrush(QBrush("#F8FAFE"));//E0E0E0 f7f9fe
    QRect rect1(0, 0, width(), height());
    painter->drawRect(rect1.adjusted(-1, -1, 0, 0));

    painter->setBrush(QBrush(Qt::white));
    QRect rect(MarginX, MarginY,
               width() - 2*MarginX, height() - 2*MarginY);
    if (!rect.isValid())
        return;
    painter->drawRect(rect.adjusted(0, 0, -1, -1));
    PlotSettings settings = zoomStack[curZoom];
    QPen quiteDark = palette().dark().color().light();
    QPen light = palette().light().color();
    int x;

    for (int i = 0; i <= settings.numXTicks; ++i) {
        x = rect.left() + (i * (rect.width() - 1)
                                 / settings.numXTicks);
        double label = settings.minX + (i * settings.spanX()
                                          / settings.numXTicks);
        //painter->setPen(quiteDark);
        painter->setPen(QPen(QColor("#6a6d71"),0,Qt::DashLine));
        painter->drawLine(x, rect.top(), x, rect.bottom());
        painter->setPen(QPen(Qt::black,0,Qt::DashLine));
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
        painter->drawText(x - 50, rect.bottom()+3, 100, 20,
                          Qt::AlignHCenter | Qt::AlignTop,
                          QString::number(label));
    }


    int y;
    for (int j = 0; j <= settings.numYTicks; ++j) {
        y = rect.bottom() - (j * (rect.height() - 1)
                                   / settings.numYTicks);
        double label = settings.minY + (j * settings.spanY()
                                          / settings.numYTicks);
       // painter->setPen(quiteDark);
        painter->setPen(QPen(QColor("#6a6d71"),0,Qt::DashLine));//A0A6A7
        painter->drawLine(rect.left(), y, rect.right(), y);
        painter->setPen(QPen(Qt::black,0,Qt::DashLine));
        painter->drawLine(rect.left() - 5, y, rect.left(), y);
        painter->drawText(rect.left() - MarginX, y - 10, MarginX - 5, 20,
                          Qt::AlignRight | Qt::AlignVCenter,
                          QString::number(label));
    }
   // painter->setPen(quiteDark);
    //painter->drawRect(rect.adjusted(0, 0, -1, -1));
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::black));
    painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

void Plotter::drawCurves(QPainter *painter)
{
    static const QColor colorForIds[6] = {
        Qt::red, "#000fd1", Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow
    };
    PlotSettings settings = zoomStack[curZoom];
    QRect rect(MarginX, MarginY,
               width() - 2 * MarginX, height() - 2 * MarginY);
    if (!rect.isValid())
        return;

    painter->setClipRect(rect.adjusted(+1, +1, -1, -1));

    QMapIterator<int, QVector<QPointF> > i(curveMap);
    while (i.hasNext()) {
        i.next();

        int id = i.key();
        QVector<QPointF> data = i.value();
        QPolygonF polyline(data.count());
       // painter->setRenderHint(QPainter::Antialiasing,true); // сглаживание
        for (int j = 0; j < data.count(); ++j) {
            double dx = data[j].x() - settings.minX;
            double dy = data[j].y() - settings.minY;
            double x = rect.left() + (dx * (rect.width() - 1)
                                         / settings.spanX());
            double y = rect.bottom() - (dy * (rect.height() - 1)
                                           / settings.spanY());
            polyline[j] = QPointF(x, y);
        }
        painter->setPen(colorForIds[uint(id) % 6]);
        painter->drawPolyline(polyline);
    }
}

void Plotter::Scroll(int n)
{
    double window = zoomStack[curZoom].spanX();
    zoomStack[curZoom].minX=n*StepScroll;
    zoomStack[curZoom].maxX=zoomStack[curZoom].minX+window;
    refreshPixmap();
}
bool Plotter::ExtendX()
{
    double maxX=zoomStack[curZoom].maxX-zoomStack[curZoom].spanX()/zoomStack[curZoom].numXTicks;
    if(maxX>zoomStack[curZoom].RangeXmin)
    {
        zoomStack[curZoom].maxX=maxX;
        refreshPixmap();
        return true;
    }
    else return false;
}
bool Plotter::CompressX()
{
    double maxX=zoomStack[curZoom].maxX*zoomStack[curZoom].numXTicks/(zoomStack[curZoom].numXTicks-1);
    if(maxX<zoomStack[curZoom].RangeXmax)
    {
        zoomStack[curZoom].maxX=maxX;
        refreshPixmap();
        return true;
    }
    else
    {
        zoomStack[curZoom].maxX=zoomStack[curZoom].RangeXmax;
        refreshPixmap();
        return false;
    }
}


PlotSettings::PlotSettings()
{
    RangeXmin=minX = 0.0;
    RangeXmax=maxX = 10.0;
    numXTicks = 10;

    RangeYmin=minY = 0.0;
    RangeYmax=maxY = 10.0;
    numYTicks = 10;
}

void PlotSettings::scroll(int dx, int dy)
{
    double stepX = spanX() / numXTicks;
    minX += dx * stepX;
    maxX += dx * stepX;

    double stepY = spanY() / numYTicks;
    minY += dy * stepY;
    maxY += dy * stepY;
    //if(maxY>RangeYmax)maxY=RangeYmax;
    //if(minY<RangeYmin)minY=RangeYmin;
}

void PlotSettings::adjust()
{
    adjustAxis(minX, maxX, numXTicks);
    adjustAxis(minY, maxY, numYTicks);
}

void PlotSettings::adjustAxis(double &min, double &max, int &numTicks)
{
    const int MinTicks = 4;
    double grossStep = (max - min) / MinTicks;
    double step = std::pow(10.0, std::floor(std::log10(grossStep)));

    if (5 * step < grossStep) {
        step *= 5;
    } else if (2 * step < grossStep) {
        step *= 2;
    }

    numTicks = int(std::ceil(max / step) - std::floor(min / step));
    if (numTicks < MinTicks)
        numTicks = MinTicks;
    min = std::floor(min / step) * step;
    max = std::ceil(max / step) * step;
}
