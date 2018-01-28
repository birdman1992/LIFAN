#ifndef GEAR_H
#define GEAR_H

#include"can.h"

class Gear
{
public:
    Gear();

    unsigned char Cur_Gear;//0,无效值；1-6,手动挡1-6；7-9预留；
    unsigned char Trt_Gear;//A，P档；B，R档；C，N档；D，D档（档位D下的动力模式）；E，E档（档位D下的经济模式）；F，L档

    void Gear_Control(unsigned char Gear_EN,unsigned char desirevalue);
};

#endif // GEAR_H
