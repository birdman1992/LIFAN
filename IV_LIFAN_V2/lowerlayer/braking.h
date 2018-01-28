#ifndef BRAKING_H
#define BRAKING_H

#include"can.h"

class Braking
{
public:
    Braking();

    float ls_prur;//左路压力值，单位为MPA，结果已放大 x100
    float rs_prur; //右路压力值，单位为MPA，结果已放大 x100
    unsigned char brk_sts; //制动状态；0，解除制动；1，主动制动；2，保压；3-F，无
    unsigned char brk_urg;//紧急制动；0，非紧急制动；1，紧急制动
    unsigned char brk_urg_sts; //from the urgent button

    void brakingControl(unsigned char brake_enable,float desire_value);
    void brakingUrgent(unsigned char urg_en);
    void readBrksts();
};

#endif // BRAKING_H
