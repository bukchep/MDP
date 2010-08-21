#ifndef BUFFER_H
#define BUFFER_H
#include <QtGui>
#include "windows.h"

class buffer: public QThread
{
    Q_OBJECT
private:
    QMutex *mutex;
public:
    QQueue<BYTE> *dataNit;
    QQueue<BYTE> *dataR;
    QQueue<BYTE> *dataFsep;
    buffer(QMutex &Mutex);
    void ConnectDataBuffer(QQueue<BYTE> &Nit, QQueue<BYTE> &R,QQueue<BYTE> &Fsep);
    void run();
    void addNit(BYTE *rbuf, int count);
    void addR(BYTE *rbuf, int count);
    void addFsep(BYTE *rbuf, int count);
signals:
    void StopBuffer();
public slots:
    void SlotData(BYTE *rbuf, int count);
    void SlotNIT(BYTE *rbuf, int count);
    void SlotR(BYTE *rbuf, int count);
    void SlotFsep(BYTE *rbuf, int count);
    void EndBuffer();
};
#endif // BUFFER_H
