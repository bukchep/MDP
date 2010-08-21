#include "usb.h"
#include "mpusbapi.h"

USB::USB()
{

}
void USB::SetConfiguration(int Mode, QTime time, float NIT, int sw)
{
    SwMode=Mode;
    START = new BYTE[64];
    STOP = new BYTE[64];
    receive_buf1 = new BYTE[64];
    receive_buf2 = new BYTE[64];

    if(SwMode==3||SwMode==4||SwMode==5)
    {
        unsigned int TotalTime=time.minute()*60+time.second();
        unsigned int T=TotalTime*Frequency/65536;
        float dT=(TotalTime*Frequency)%65536;
        N_T=T;
        offset_T=0;
        if(dT!=0)
        {
            N_T++;
            offset_T=(1-dT/65536)*65536;
        }
    }

    if(SwMode==0||SwMode==3||SwMode==4)
    {
        float dTNIT=(int)(NIT*Frequency)%65536;
        N_NIT=(int)(NIT*Frequency/65536);;
        offset_NIT=0;
        if(dTNIT!=0)
        {
            N_NIT++;
            offset_NIT=(1-dTNIT/65536)*65536;
        }
    }
}
void USB::run()
{
    started=true;
    switch(SwMode)
    {
        case 0:
            //ReadNIT();
            int sizepacketNIT;
            if(N_NIT==1)sizepacketNIT=40;
            if(N_NIT==2)sizepacketNIT=8;
            if(N_NIT>2)sizepacketNIT=4;
            sizepacketNIT+=2;
            START[0]=START_NIT;
            START[2] = (BYTE)(N_NIT/256);
            START[1] = (BYTE)(N_NIT-256*(N_NIT/256));
            START[4] = (BYTE)(offset_NIT/256);
            START[3] = (BYTE)(offset_NIT-256*(offset_NIT/256));
            STOP[0] = STOP_NIT;
            ReadDevice(5,64);
            break;
        case 1:
            START[0]=START_R;
            STOP[0] = STOP_R;
            ReadDevice(1,64);
            break;
        case 2:
            START[0]=START_Fsep;
            STOP[0] = STOP_Fsep;
            ReadDevice(1,64);
            break;
        case 3:
            START[0]=START_NITF;
            START[2] = (BYTE)(N_NIT/256);
            START[1] = (BYTE)(N_NIT-256*(N_NIT/256));
            START[4] = (BYTE)(offset_NIT/256);
            START[3] = (BYTE)(offset_NIT-256*(offset_NIT/256));
            START[6] = (BYTE)(N_T/256);
            START[5] = (BYTE)(N_T-256*(N_T/256));
            START[8] = (BYTE)(offset_T/256);
            START[7] = (BYTE)(offset_T-256*(offset_T/256));
            STOP[0] = STOP_NITF;
            ReadDevice(9,64);
            break;
        case 4:
            START[0]=START_NITR;
            START[2] = (BYTE)(N_NIT/256);
            START[1] = (BYTE)(N_NIT-256*(N_NIT/256));
            START[4] = (BYTE)(offset_NIT/256);
            START[3] = (BYTE)(offset_NIT-256*(offset_NIT/256));
            START[6] = (BYTE)(N_T/256);
            START[5] = (BYTE)(N_T-256*(N_T/256));
            START[8] = (BYTE)(offset_T/256);
            START[7] = (BYTE)(offset_T-256*(offset_T/256));
            STOP[0] = STOP_NITR;
            ReadDevice(9,64);
            break;
        case 5:
            START[0]=START_RF;
            START[6] = (BYTE)(N_T/256);
            START[5] = (BYTE)(N_T-256*(N_T/256));
            START[8] = (BYTE)(offset_T/256);
            START[7] = (BYTE)(offset_T-256*(offset_T/256));
            STOP[0] = STOP_RF;
            ReadDevice(9,64);
            break;
        default:
            break;
    }
    exec();
}
void USB::stop()
{
    started=false;
}

void USB::ReadDevice(DWORD lenghtStartPacket,DWORD lenghtReceivePacket)
{
    char vid_pid[]= "vid_04d8&pid_000c"; // Default Demo Application Firmware
    char out_pipe[]= "\\MCHP_EP1";
    char in_pipe[]= "\\MCHP_EP1";
    DWORD temp;
    HINSTANCE libHandle;
    HANDLE myOutPipe;
    HANDLE myInPipe;
    QLibrary lib("C:/WINDOWS/system32/mpusbapi");
    typedef DWORD (* pF_MPUSBGetDeviceCount) (PCHAR);
    pF_MPUSBGetDeviceCount MPUSBGetDeviceCount = (pF_MPUSBGetDeviceCount)(lib.resolve("_MPUSBGetDeviceCount"));
    typedef HANDLE (* pF_MPUSBOpen) (DWORD instance,PCHAR pVID_PID,PCHAR pEP,DWORD dwDir,DWORD dwReserved);
    pF_MPUSBOpen MPUSBOpen = (pF_MPUSBOpen)(lib.resolve("_MPUSBOpen"));
    typedef DWORD (* pF_MPUSBRead) (HANDLE handle,PVOID pData,DWORD dwLen,PDWORD pLength,DWORD dwMilliseconds);
    pF_MPUSBRead MPUSBRead = (pF_MPUSBRead)(lib.resolve("_MPUSBRead"));
    typedef DWORD (* pF_MPUSBWrite) (HANDLE handle,PVOID pData,DWORD dwLen,PDWORD pLength,DWORD dwMilliseconds);
    pF_MPUSBWrite MPUSBWrite = (pF_MPUSBWrite)(lib.resolve("_MPUSBWrite"));
    typedef DWORD (* pF_MPUSBReadInt) (HANDLE handle,PVOID pData,DWORD dwLen,PDWORD pLength,DWORD dwMilliseconds);
    pF_MPUSBReadInt MPUSBReadInt = (pF_MPUSBReadInt)(lib.resolve("_MPUSBReadInt"));
    typedef BOOL (* pF_MPUSBClose) (HANDLE handle);
    pF_MPUSBClose MPUSBClose = (pF_MPUSBClose)(lib.resolve("_MPUSBClose"));
    if(!MPUSBGetDeviceCount||!MPUSBOpen||!MPUSBRead||!MPUSBWrite||!MPUSBReadInt||!MPUSBClose)
    {
        emit signalError(tr("Ошибка! mpusbapi.dll библиотека не найдена."));
    }
    else
    {
        myOutPipe = MPUSBOpen(0,vid_pid,out_pipe,MP_WRITE,0);
        myInPipe = MPUSBOpen(0,vid_pid,out_pipe,MP_READ,0);
        DWORD RecvLength;
        DWORD SentDataLength;
        BYTE send_buf[64];
        send_buf[0]=0xff;
        if(myOutPipe != INVALID_HANDLE_VALUE && myInPipe != INVALID_HANDLE_VALUE)
        {
            bool t=1;
           // if(MPUSBWrite(myOutPipe,send_buf,1,&SentDataLength,1000))
           // {
            if(MPUSBWrite(myOutPipe,START,lenghtStartPacket,&SentDataLength,1000))
            {
                while(started)
                {
                    if(t)
                    {
                        //if(MPUSBWrite(myOutPipe,send_buf,1,&SentDataLength,1000))
                        //{
                            if(MPUSBRead(myInPipe,receive_buf1, lenghtReceivePacket,
                               &RecvLength,1000))
                            {
                                emit addData(receive_buf1, (int)receive_buf1[1]);
                            }
                            else {started=false;emit signalError(tr("Ошибка чтения 1!"));}
                        //}
                        t=0;
                    }
                    else
                    {
                       // if(MPUSBWrite(myOutPipe,send_buf,1,&SentDataLength,1000))
                        //{
                            if(MPUSBRead(myInPipe,receive_buf2, lenghtReceivePacket,
                               &RecvLength,1000))
                            {
                                emit addData(receive_buf2, (int)receive_buf2[1]);
                            }
                            else {started=false;emit signalError(tr("Ошибка чтения 2!"));}
                        //}
                        t=1;
                    }
                }   
                MPUSBWrite(myOutPipe,STOP,1,&SentDataLength,1000);
            }
        }
        else  emit signalError(tr("Ошибка! Канал данных не открыт."));
        MPUSBClose(myOutPipe);
        MPUSBClose(myInPipe);
        myOutPipe = myInPipe = INVALID_HANDLE_VALUE;
        emit endMeasurement();
    }
}



