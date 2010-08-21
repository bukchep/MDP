#ifndef MAINREGION_H
#define MAINREGION_H
#include <QtGui>
#include "plotter.h"

class mainregion: public QWidget
{
    Q_OBJECT

    public:
    QVBoxLayout *pMain;
    Plotter *Graph1;
    Plotter *Graph2;
    PlotSettings *settingsGraph1;
    PlotSettings *settingsGraph2;
    mainregion();

signals:
    //void  pointerDisplay(int index, Plotter &Graph);

public slots:
    void SetDisplayMainRegion(int select);
    //void PointerDisplay(int index);
};
#endif // MAINREGION_H
