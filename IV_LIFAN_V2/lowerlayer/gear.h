#ifndef GEAR_H
#define GEAR_H

#include"can.h"

/*desire_value
1. P-->10
2. R--> 11
3. N-->12
4. D-->14
5. S-->15
*/

#define GEAR_P 10
#define GEAR_R 11
#define GEAR_N 12
#define GEAR_D 13
#define GEAR_E  14
#define GEAR_S  15

class Gear
{
public:
    Gear();

    unsigned char cur_gear;//0,无效值；1-6,手动挡1-6；7-9预留；
    //unsigned char Trt_Gear;//A，P档；B，R档；C，N档；D，D档（档位D下的动力模式）；E，E档（档位D下的经济模式）；F，L档

    void controlGear(unsigned char gear_en,unsigned char desire_value);
};

#endif // GEAR_H
