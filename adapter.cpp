#include "adapter.h"

adapter::adapter(QMutex &Mutex,QMutex &MutexG)
{
    Data_Nit = new QQueue<BYTE>; 
    Data_R = new QQueue<BYTE>;
    Data_Fsep = new QQueue<BYTE>;

    DNIT = new QQueue<QPointF>;
    DR = new QQueue<QPointF>;
    DFSEP = new QQueue<QPointF>;

    mutex = new QMutex;
    mutex=&Mutex;

    mutexG = new QMutex;
    mutexG=&MutexG;
}
void adapter::ConnectDataAdapter(QQueue<BYTE> &Nit, QQueue<BYTE> &R,QQueue<BYTE> &Fsep,QQueue<QPointF> &DATANIT, QQueue<QPointF> &DATAR, QQueue<QPointF> &DATAFSEP)
{
    Started=true;

    Data_Nit=&Nit;
    Data_R=&R;
    Data_Fsep=&Fsep;

    DNIT=&DATANIT;
    DR=&DATAR;
    DFSEP=&DATAFSEP;
}
void adapter::SetConfig(int Mode, QTime time, float NIT, int sw)
{
    SwMode=Mode;
    Frequency=1000;
    FrequencyNIT=1000;
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
void adapter::run()
{
    x=y=0.00000;
    switch(SwMode)
    {
        case 0:
            while(Started)
            {
                if (!Data_Nit->isEmpty())
                {
                    /*QMessageBox msgBox;
                    msgBox.setText("Adapter");
                    msgBox.exec();*/
                    mutex->lock();
                    ReceiveNit[0] = Data_Nit->dequeue();
                    ReceiveNit[1] = Data_Nit->dequeue();
                    ReceiveNit[2] = Data_Nit->dequeue();
                    ReceiveNit[3] = Data_Nit->dequeue();
                    mutex->unlock();
                    processingNit();
                }
            }
            break;
        case 1:
            while(Started)
            {
                if(!Data_R->empty())
                {
                    mutex->lock();
                    ReceiveR[0] = Data_R->dequeue();
                    ReceiveR[1] = Data_R->dequeue();
                    mutex->unlock();
                    processingR();
                }
            }
            break;
        case 2:
            while(Started)
            {
                if(!Data_Fsep->empty())
                {
                    mutex->lock();
                    ReceiveY = Data_Fsep->dequeue();
                    mutex->unlock();
                    processingFsep();
                }
            }
            break;
        case 3:
            while(Started)
            {
                if (!Data_Nit->isEmpty())
                {
                    mutex->lock();
                    ReceiveY = Data_Nit->dequeue();
                    mutex->unlock();
                    processingNit();
                }
                for(int i=0;i<SizePacketFsep;++i)
                {
                    if(!Data_Fsep->empty())
                    {
                        mutex->lock();
                        ReceiveY = Data_Fsep->dequeue();
                        mutex->unlock();
                        processingFsep();
                    }
                }
            }
            break;
        case 4:
            while(Started)
            {
                if (!Data_Nit->isEmpty())
                {
                    mutex->lock();
                    ReceiveY = Data_R->dequeue();
                    mutex->unlock();
                    processingNit();
                }
                for(int i=0;i<SizePacketR;++i)
                {
                    if(!Data_R->empty())
                    {

                        mutex->lock();
                        ReceiveY = Data_R->dequeue();
                        mutex->unlock();
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
                    if(!Data_R->empty())
                    {
                         mutex->lock();
                         ReceiveY = Data_R->dequeue();
                         mutex->unlock();
                         processingR();
                    }
                }
                for(int i=0;i<SizePacketFsep;++i)
                {
                    if(!Data_Fsep->empty())
                    {
                        mutex->lock();
                        ReceiveY = Data_Fsep->dequeue();
                        mutex->unlock();
                        processingFsep();
                    }
                }
            }
            break;
        default:
            break;
    }
    emit StopAdapter();
    exec();
}
void adapter::EndAdapter()
{
    Started=false;
}
void adapter::processingNit()
{

    y=(ReceiveNit[3] | ReceiveNit[2] << 8| ReceiveNit[1] << 16 | ReceiveNit[0]<< 24)*scaleNit;
    x+=QuantumNit;
    mutexG->lock();
    DNIT->enqueue(QPointF(x,y));
    mutexG->unlock();
}
void adapter::processingR()
{
    x+=QuantumR;
    y=(ReceiveR[0]|ReceiveR[1]<<8)*scaleR;
    mutexG->lock();
    DR->enqueue(QPointF(x,y));
    mutexG->unlock();
}
void adapter::processingFsep()
{
    x+=QuantumFsep;
    if(ReceiveY>=Ymax)Ymax=ReceiveY;
    else {t[i]=x; Ymax=0; ++i;}
    if(t[1]>0)
    {
        y=Tsector/(t[1]-t[0]);
        mutexG->lock();
        DFSEP->enqueue(QPointF(x,y));
        mutexG->unlock();
        t[1]=0;
        i=0;
    }
}
void adapter::ConfigDisplayNit(float NIT)
{
    QuantumNit=NIT;
    //scaleNit=1.0/(FrequencyNIT*NIT);
    scaleNit=1;
}
void adapter::ConfigDisplayR()
{
    SizePacketR=62;
    QuantumR=0.000071;
    scaleR=5.000/1023;
}
void adapter::ConfigDisplayFsep(int sw)
{
    SizePacketFsep=62;
    QuantumFsep=0.001;
    scaleFsep=5.0/1023;
    i=Ymax=0;
    t[0]=t[1]=0;
    Tsector=(float)1/sw;   
}


