#ifndef PACKET_H
#define PACKET_H
#include "windows.h"

//#define ControlNIT  0x24
//#define ControlR  0x25
//#define ContolFsep 0x26

//union CONTROL_PACKET
//{
    struct fff
    {
        enum CMD
        {
            ControlNIT,
            ControlR,
            ContolFsep
        };
       unsigned  int parameter;
    };
//};
#endif // PACKET_H
