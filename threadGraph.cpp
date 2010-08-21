#include "threadGraph.h"

threadGraph::threadGraph(QVector<QPointF> &DATANITG, QVector<QPointF> &DATARG, QVector<QPointF> &DATAFSEPG,QVector<QPointF> &DATANITG2, QVector<QPointF> &DATARG2, QVector<QPointF> &DATAFSEPG2,QMutex &Mutex)//Plotter &DisGNit,Plotter &DisGR,Plotter &DisGFsep,
{
    Data_GNit = new QQueue<QPointF>;
    Data_GR = new QQueue<QPointF>;
    Data_GFsep = new QQueue<QPointF>;

    DNIT = new QVector<QPointF>;
    DR = new QVector<QPointF>;
    DFSEP = new QVector<QPointF>;

    GraphNIT = new QVector<QPointF>;

    DGNIT = new QVector<QPointF>;
    DGNIT = &DATANITG;
    GraphNIT=&DATANITG;
    DGR = new QVector<QPointF>;
    DGR = &DATARG;
    DGFSEP = new QVector<QPointF>;
    DGFSEP = &DATAFSEPG;
    DGNIT2 = new QVector<QPointF>;
    DGNIT2 = &DATANITG2;
    DGR2 = new QVector<QPointF>;
    DGR2 = &DATARG2;
    DGFSEP2 = new QVector<QPointF>;
    DGFSEP2 = &DATAFSEPG2;

    mutexG = new QMutex;
    mutexG=&Mutex;

    flagDraw=swNIT=swR=swFsep=false;
}
void threadGraph::ConnectDataGraph(QQueue<QPointF> &Nit, QQueue<QPointF> &R,QQueue<QPointF> &Fsep,QVector<QPointF> &DATANIT, QVector<QPointF> &DATAR, QVector<QPointF> &DATAFSEP)
{
    Started=true;

    Data_GNit=&Nit;
    Data_GR=&R;
    Data_GFsep=&Fsep;

    DNIT=&DATANIT;
    DR=&DATAR;
    DFSEP=&DATAFSEP;
}
void threadGraph::SetConf(int Mode, QTime time, float NIT, int sw)
{
    SwMode=Mode;
    switch(SwMode)
    {
        case 0:
            ConfigDisplayNit(NIT);
            break;
        case 1:
            ConfigDisplayR();
            break;
        case 2:
            ConfigDisplayFsep(sw);
            break;
        case 3:
            ConfigDisplayNit(NIT);
            ConfigDisplayFsep(sw);
            break;
        case 4:
            ConfigDisplayNit(NIT);
            ConfigDisplayR();
            break;
        case 5:
            ConfigDisplayR();
            ConfigDisplayFsep(sw);
            break;
        default:
            break;
    }
}
void threadGraph::run()
{
    switch(SwMode)
    {
        case 0:
            while(Started)
            {
                if (!Data_GNit->isEmpty())
                {
                    mutexG->lock();
                    point=Data_GNit->dequeue();
                    mutexG->unlock();
                    processingNit();
                }
            }
            break;
        case 1:
            while(Started)
            {
                if(!Data_GR->empty())
                {
                    mutexG->lock();
                    point=Data_GR->dequeue();
                    mutexG->unlock();
                    processingR();
                }
            }
            break;
        case 2:
            while(Started)
            {
                if(!Data_GFsep->empty())
                {
                    mutexG->lock();
                    point=Data_GFsep->dequeue();
                    mutexG->unlock();
                    processingFsep();
                }
            }
            break;
        case 3:
            while(Started)
            {
                if (!Data_GNit->isEmpty())
                {
                    mutexG->lock();
                    point=Data_GNit->dequeue();
                    mutexG->unlock();
                    processingNit();
                }
                for(int i=0;i<SizePacketFsep;++i)
                {
                    if(!Data_GFsep->empty())
                    {
                        mutexG->lock();
                        point=Data_GFsep->dequeue();
                        mutexG->unlock();
                        processingFsep();
                    }
                }
            }
            break;
        case 4:
            while(Started)
            {
                if (!Data_GNit->isEmpty())
                {
                    mutexG->lock();
                    point=Data_GNit->dequeue();
                    mutexG->unlock();
                    processingNit();
                }
                for(int i=0;i<SizePacketR;++i)
                {
                    if(!Data_GR->empty())
                    {
                        mutexG->lock();
                        point=Data_GR->dequeue();
                        mutexG->unlock();
                        processingR();
                    }
                }
            }
            break;
        case 5:
            while(Started)
            {
                for(int i=0;i<SizePacketR;++i)
                {
                    if(!Data_GR->empty())
                    {
                         mutexG->lock();
                         point=Data_GR->dequeue();
                         mutexG->unlock();
                         processingR();
                    }
                }
                for(int i=0;i<SizePacketFsep;++i)
                {
                    if(!Data_GFsep->empty())
                    {
                        mutexG->lock();
                        point=Data_GFsep->dequeue();
                        mutexG->unlock();
                        processingFsep();
                    }
                }
            }
            break;
        default:
            break;
    }
    emit StopGraph();
    exec();
}
void threadGraph::EndThreadGraph()
{
    Started=false;
}
void threadGraph::processingNit()
{
    DNIT->append(point);
    //if(swNIT)DGNIT2->append(point);
    //else DGNIT->append(point);
    DGNIT->append(point);
    if(point.x()>=SetDisplayNit.maxX)
    {
        emit DrawNit(SetDisplayNit.minX,SetDisplayNit.minY,SetDisplayNit.maxX,SetDisplayNit.maxY,swNIT);
        SetDisplayNit.minX+=ShiftX_Nit;
        SetDisplayNit.maxX+=ShiftX_Nit;
        //if(swNIT){DGNIT->clear();GraphNIT=DGNIT; swNIT=false;}
        //else {DGNIT2->clear(); GraphNIT=DGNIT2; swNIT=true;}
    }
    else
    {
        if(flagDraw)
        {
            if(point.x()>=29)flagDraw=0;
            emit DrawNit(SetDisplayNit.minX,SetDisplayNit.minY,SetDisplayNit.maxX,SetDisplayNit.maxY,swNIT);
        }
    }
}
void threadGraph::processingR()
{
    DR->append(point);
    DGR->append(point);
    //DR->insert(pos++,QPointF(x,y));
    //D->insert(pos++,QPointF(x,y));
    //if(y>Yend_R)Yend_R=y;
    if(point.x()>=SetDisplayR.maxX)
    {
        emit DrawR(SetDisplayR.minX,SetDisplayR.minY,SetDisplayR.maxX,SetDisplayR.maxY);
        SetDisplayR.minX+=ShiftX_R;
        SetDisplayR.maxX+=ShiftX_R;
        //pos=0;
    }
}
void threadGraph::processingFsep()
{
    DFSEP->append(point);
    if(point.y()>SetDisplayFsep.maxY)SetDisplayFsep.maxY=point.y()+point.y()*0.01;
    if(point.x()>=SetDisplayFsep.maxX)
    {
        emit DrawFsep(SetDisplayFsep.minX,SetDisplayFsep.minY,SetDisplayFsep.maxX,SetDisplayFsep.maxY);
        SetDisplayFsep.minX+=ShiftX_Fsep;
        SetDisplayFsep.maxX+=ShiftX_Fsep;
    }
}
void threadGraph::ConfigDisplayNit(float NIT)
{
    SetDisplayNit.minX=NIT;
    SetDisplayNit.minY=0;
    SetDisplayNit.maxX=60;
    ShiftX_Nit=1;
    SetDisplayNit.maxY=1;
    flagDraw=1;
}
void threadGraph::ConfigDisplayR()
{
    SizePacketR=62;
    SetDisplayR.minX=0;
    SetDisplayR.minY=0;
    ShiftX_R=SetDisplayR.maxX=1;
    SetDisplayR.maxY=5;
}
void threadGraph::ConfigDisplayFsep(int sw)
{
    SizePacketFsep=62;
    SetDisplayFsep.minX=0;
    SetDisplayFsep.minY=0;
    ShiftX_Fsep=SetDisplayFsep.maxX=1;
    SetDisplayFsep.maxY=5;
}
