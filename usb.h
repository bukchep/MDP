#ifndef USB_H
#define USB_H
#include <QLibrary>
#include <QtGui>
#include "windows.h"

#define START_NIT 0x34
#define STOP_NIT 0x32

#define START_R 0x35
#define STOP_R 0x37

#define START_Fsep 0x38
#define STOP_Fsep 0x39

#define START_NITF 0x41
#define STOP_NITF 0x42

#define START_NITR 0x44
#define STOP_NITR 0x45

#define START_RF 0x47
#define STOP_RF 0x48

class USB: public QThread
{
    Q_OBJECT
public:
    BYTE *START;
    BYTE *STOP;
    BYTE *receive_buf1;
    BYTE *receive_buf2;
    volatile bool started;
    QMutex mutex;
public: //Frequency = 250000 дл€ 8ћ√ц 625000 дл€ 20ћ√ц 1250000
    enum { Frequency =  625000}; // частота в герцах счетных импульсов дл€ счетчиков задающего интервал усреднени€ и измерени€
    enum { FrequencyNIT = 100000 };// в герцах дл€ счетчика считающего NIT
    int TotalTime;
    float T_NIT;
    unsigned int N_T;
    unsigned int offset_T;
    unsigned int N_NIT;
    unsigned int offset_NIT;
    int SwMode;
    USB();

    void ReadDevice(DWORD lenghtStartPacket,DWORD lenghtReceivePacket);

    void run();
    void stop();
    void SetConfiguration(int Mode, QTime time, float NIT, int sw);
signals:
    void addData(BYTE *rbuf, int count);
    void addDataNit(BYTE *rbuf, int count);
    void addDataR(BYTE *rbuf, int count);
    void addDataFsep(BYTE *rbuf, int count);
    void signalError(QString str);
    void endMeasurement();
};

#endif // USB_H
