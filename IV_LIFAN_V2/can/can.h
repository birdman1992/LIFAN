#ifndef CAN_H
#define CAN_H

#include<canlib.h>


/*************can初始化**************/
void CanOpen(int channel);
/*************发送数据***************/
int CanSendMessage(int channel,int id,unsigned char *msg);
/**************接收数据**************/
int CanReceiveMessage(int channel,int id,unsigned char *data);

#endif // CAN_H
