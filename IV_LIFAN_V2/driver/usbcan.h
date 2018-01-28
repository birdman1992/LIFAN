#ifndef USBCAN_H
#define USBCAN_H
#include "can/controlcan.h"

class usbcan
{
public:
    usbcan();
    int Initial(int nDeviceType, int nDeviceInd,int nCANInd);
};

#endif // USBCAN_H
