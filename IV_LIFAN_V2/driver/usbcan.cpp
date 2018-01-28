#include "usbcan.h"

usbcan::usbcan()
{
}

/*nDeviceType   4  /USBCAN-2A或USBCAN-2C或CANalyst-II/
 * nDeviceInd    0  /第一个设置/
 * nCANInd        0  /第一个通道/
 */
int usbcan::Initial(int nDeviceType, int nDeviceInd,int nCANInd)
{
    DWORD dwRel;
    VCI_INIT_CONFIG vic;
    dwRel = VCI_OpenDevice(nDeviceType,nDeviceInd,0);
    if(dwRel != 1)
    {
        return -1;
    }
    vic.AccCode=0x80000008;
    vic.AccMask=0xFFFFFFFF;
    vic.Filter=1;
    vic.Timing0=0x00;
    vic.Timing1=0x1C;
    vic.Mode=0;
    dwRel = VCI_InitCAN(nDeviceType, nDeviceInd, nCANInd, &vic);
    if(dwRel != 1)
    {
        VCI_CloseDevice(nDeviceType,nDeviceInd);
        return -1;
    }
    dwRel = VCI_StartCAN(nDeviceType,nDeviceInd,nCANInd);
    if(dwRel != 1)
    {
        VCI_CloseDevice(nDeviceType,nDeviceInd);
        return -1;
    }
    return 0;
}
