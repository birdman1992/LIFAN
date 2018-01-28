#ifndef THROTTLE_H
#define THROTTLE_H

#include"can.h"

class throttle
{
public:
    throttle();

    double CUR_THRT_OPN;//当前节气门开度，放大100倍传送，范围【0，10】，单位（%）
    double CUR_PEDL_OPN;//当前油门踏板开度，放大100倍传送，范围【0，10】，单位（%）
    double CUR_PEDL_OPN_VEL;//当前油门踏板速度，放大100倍传送，范围【0，655.35】，单位（%/s）

    void throttle_Control(unsigned char TAR_THRT_ENABLE,double TGT_THRT_OPN,double TGT_PEDL_OPN_VEL);


};

#endif // THROTTLE_H
