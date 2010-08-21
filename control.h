#ifndef CONTROL_H
#define CONTROL_H
#include <QtGui>
#include "QObject"
#include "plotter.h"
#include "mainregion.h"
#include "windows.h"
#include "usb.h"
#include "buffer.h"
#include "adapter.h"
#include "threadGraph.h"

class control: public QObject
{
    Q_OBJECT
    void DeleteData();
    void CreateData();
    void DeleteThread();
    void CreateThread();
    void SaveData(QString str,QVector<QPointF> *DATA);
    unsigned int Mode;

public:
    QMutex *mut;
    QMutex *mutG;

    QQueue<BYTE> *dataNit;
    QQueue<BYTE> *dataR;
    QQueue<BYTE> *dataFsep;

    QQueue<QPointF> *dataGNit;
    QQueue<QPointF> *dataGR;
    QQueue<QPointF> *dataGFsep;

    QVector<QPointF> *DATAR;
    QVector<QPointF> *DATANIT;
    QVector<QPointF> *DATAFSEP;

    QVector<QPointF> *DATAGR;
    QVector<QPointF> *DATAGNIT;
    QVector<QPointF> *DATAGFSEP;

    QVector<QPointF> *DATAGR2;
    QVector<QPointF> *DATAGNIT2;
    QVector<QPointF> *DATAGFSEP2;

    mainregion *MainRegion;

    USB * pUSB;
    buffer * pBuffer;
    adapter * pAdapter;
    threadGraph * pThreadGraph;

    Plotter *DisplayR;
    Plotter *DisplayNit;
    Plotter *DisplayFsep;
    PlotSettings *SettingsDisplayR;
    PlotSettings *SettingsDisplayNit;
    PlotSettings *SettingsDisplayFsep;

    control();
    void DrawGraph(Plotter *Graph,PlotSettings *SettingsGraph, QVector<QPointF> &data);

signals:
    void SetFlagSaveData();
    void Error();

public slots:
    void start(int mode, QTime time, int NIT, int sw);
    void save();
    void stop();

    void EndAdapter();
    void EndGraph();
    void StopThreads();

    void receiveError(QString str);

    void DrawingNit(double minX,double minY,double maxX,double maxY,bool sw);
    void DrawingR(double minX,double minY,double maxX,double maxY);
    void DrawingFsep(double minX,double minY,double maxX,double maxY);
};
#endif // CONTROL_H
