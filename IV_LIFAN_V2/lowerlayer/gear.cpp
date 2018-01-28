#include "gear.h"
#include"QDebug"

/************构造函数************/
Gear::Gear()
{
    this->cur_gear=0;
}

/************档位控制************/
/*desire_value
1. P-->10
2. R--> 11
3. N-->12
4. D-->14
5. S-->15
*/
void Gear::controlGear(unsigned char gear_en, unsigned char desire_value)
{

    CanOpen(0);

    unsigned char data[8]={};
    CanReceiveMessage(0,0x0521,data);
    this->cur_gear=data[7];

    unsigned char msg[8]={};
    msg[7]=gear_en;
    msg[6]=desire_value;

    CanSendMessage(0,0x0501,msg);

}
