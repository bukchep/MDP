#ifndef ADAPTER_H
#define ADAPTER_H
#include <QtGui>
#include "windows.h"
#include "plotter.h"

class adapter: public QThread
{
    Q_OBJECT
private:
    QMutex *mutex; 
    QMutex *mutexG;

    double scaleNit,scaleR,scaleFsep;
    unsigned int ReceiveY;
    unsigned int ReceiveNit[4];
    unsigned int ReceiveR[2];
    unsigned int SwMode;
    unsigned int SizePacketR,SizePacketFsep;
    int Frequency;
    int FrequencyNIT;
    float QuantumNit,QuantumR,QuantumFsep;
    float  Tsector;
    float t[2];
    unsigned int Ymax,i;

public:
    volatile bool Started;
    float x,y;
    QQueue<BYTE> *Data_Nit;
    QQueue<BYTE> *Data_R;
    QQueue<BYTE> *Data_Fsep;

    QQueue<QPointF> *DR;
    QQueue<QPointF> *DNIT;
    QQueue<QPointF> *DFSEP;

    adapter(QMutex &Mutex,QMutex &MutexG);
    void ConnectDataAdapter(QQueue<BYTE> &Nit, QQueue<BYTE> &R,QQueue<BYTE> &Fsep,QQueue<QPointF> &DATANIT, QQueue<QPointF> &DATAR, QQueue<QPointF> &DATAFSEP);
    void SetConfig(int Mode, QTime time, float NIT, int sw);

    void ConfigDisplayNit(float NIT);
    void ConfigDisplayR();
    void ConfigDisplayFsep(int sw);

    void processingNit();
    void processingR();
    void processingFsep();

    void EndAdapter();  
protected:
    void run();
signals:
    void StopAdapter();
};
#endif // ADAPTER_H
