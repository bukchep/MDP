#include "buffer.h"

buffer::buffer(QMutex &Mutex)
{
    dataNit = new QQueue<BYTE>;
    dataR =new QQueue<BYTE>;
    dataFsep =new QQueue<BYTE>;

    mutex = new QMutex;
    mutex=&Mutex;
}
void buffer::ConnectDataBuffer(QQueue<BYTE> &Nit, QQueue<BYTE> &R,QQueue<BYTE> &Fsep)
{
    dataNit = &Nit;
    dataR = &R;
    dataFsep = &Fsep;
}
void buffer::run()
{
    exec();
}
void buffer::SlotData(BYTE *rbuf, int count)
{
   /* QMessageBox msgBox;
    QString str;
    str.setNum(rbuf[0]);
    msgBox.setText(str);
    msgBox.exec();*/
    switch(rbuf[0])
    {
        case 11:
            addNit(rbuf,count);
            break;
        case 22:
            addR(rbuf, count);
            break;
        case 33:
            addFsep(rbuf, count);
            break;
        default:
            break;
    }
}
void buffer::SlotNIT(BYTE *rbuf, int count)
{
    mutex->lock();
    for(int i=0;i<count;++i)
    {
        dataNit->enqueue(rbuf[i]);
    }
    mutex->unlock();
}
void buffer::SlotR(BYTE *rbuf, int count)
{
    mutex->lock();
    for(int i=0;i<count;++i)
    {
        dataR->enqueue(rbuf[i]);
    }
    mutex->unlock();
}
void buffer::SlotFsep(BYTE *rbuf, int count)
{
    mutex->lock();
    for(int i=0;i<count;++i)
    {
        dataFsep->enqueue(rbuf[i]);
    }
    mutex->unlock();
}
void buffer::EndBuffer()
{
    emit StopBuffer();
}
void buffer::addNit(BYTE *rbuf, int count)
{
    mutex->lock();
    for(int i=2;i<count;++i)
    {
        dataNit->enqueue(rbuf[i]);
    }
    mutex->unlock();
}
void buffer::addR(BYTE *rbuf, int count)
{
    mutex->lock();
    for(int i=2;i<count;++i)
    {
        dataR->enqueue(rbuf[i]);
    }
    mutex->unlock();
}
void buffer::addFsep(BYTE *rbuf, int count)
{
    mutex->lock();
    for(int i=2;i<count;++i)
    {
        dataFsep->enqueue(rbuf[i]);
    }
    mutex->unlock();
}
