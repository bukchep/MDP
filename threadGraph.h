#ifndef THREADGRAPH_H
#define THREADGRAPH_H
#include <QtGui>
#include "plotter.h"

class threadGraph: public QThread
{
    Q_OBJECT
private:
    int SwMode;
    unsigned int SizePacketR,SizePacketFsep;
    bool swNIT,swR,swFsep,flagDraw;
    QMutex *mutexG;
public:
    PlotSettings SetDisplayNit;
    PlotSettings SetDisplayR;
    PlotSettings SetDisplayFsep;
    float ShiftX_Nit,ShiftX_R,ShiftX_Fsep;

    volatile bool Started;

    QPointF point;

    QQueue<QPointF> *Data_GNit;
    QQueue<QPointF> *Data_GR;
    QQueue<QPointF> *Data_GFsep;

    QVector<QPointF> *DR;
    QVector<QPointF> *DNIT;
    QVector<QPointF> *DFSEP;

    QVector<QPointF> *GraphNIT;
    QVector<QPointF> *DGR;
    QVector<QPointF> *DGNIT;
    QVector<QPointF> *DGFSEP;
    QVector<QPointF> *DGR2;
    QVector<QPointF> *DGNIT2;
    QVector<QPointF> *DGFSEP2;

    threadGraph(QVector<QPointF> &DATANITG, QVector<QPointF> &DATARG, QVector<QPointF> &DATAFSEPG,QVector<QPointF> &DATANITG2, QVector<QPointF> &DATARG2, QVector<QPointF> &DATAFSEPG2,QMutex &Mutex);//Plotter &DisGNit,Plotter &DisGR,Plotter &DisGFsep,
    void SetConf(int Mode, QTime time, float NIT, int sw);
    void ConnectDataGraph(QQueue<QPointF> &Nit, QQueue<QPointF> &R,QQueue<QPointF> &Fsep,QVector<QPointF> &DATANIT, QVector<QPointF> &DATAR, QVector<QPointF> &DATAFSEP);

    void ConfigDisplayNit(float NIT);
    void ConfigDisplayR();
    void ConfigDisplayFsep(int sw);

    void processingNit();
    void processingR();
    void processingFsep();

    void DrawGraph(Plotter *Graph,PlotSettings *SettingsGraph, QVector<QPointF> &data);
    void EndThreadGraph();
protected:
    void run();
signals:
    void StopGraph();
    void DrawNit(double minX,double minY,double maxX,double maxY,bool sw);
    void DrawR(double minX,double minY,double maxX,double maxY);
    void DrawFsep(double minX,double minY,double maxX,double maxY);
};
#endif // THREADGRAPH_H
