#include<canlib.h>
#include <QDebug>
#include"can.h"

CanHandle hnd[10];
bool open;
void CanOpen(int channel)
{
    if(open==false)
    {
        canStatus stat,stat0;

        canInitializeLibrary();

        hnd[channel] =canOpenChannel(channel,canOPEN_EXCLUSIVE);

        stat=canSetBusParams(hnd[channel],canBITRATE_500K,0,0,0,0,0);

        stat0=canBusOn(hnd[channel]);

        if((hnd[channel]<0)&&(stat<0)&&(stat0<0))
        {
            //qDebug()<<"canopen fail !";
            open=false;
        }
        else
        {
            //qDebug()<<"canopen ok !";
            open=true;
        }
    }

}


int CanSendMessage(int channel,int id,unsigned char *msg)
{
    canStatus stat;

    stat=canWrite(hnd[channel],id,msg,8,0);//往id写数据
    if(stat<0)
    {
        //qDebug()<<"send fail !";
        return -1;
    }
    else
    {
        //qDebug()<<"send ok ! ";
        return 0;
    }
}


int CanReceiveMessage(int channel,int id,unsigned char *data)
{
    unsigned int dlc;
    unsigned int flag;
    unsigned long int time;
    canStatus stat;

    stat=canReadSpecific(hnd[channel],id,data,&dlc,&flag,&time);//读id的数据
    switch (stat)
    {
    case canOK:
        //qDebug()<<"Can Receive ok !";
        return 0;
        break;
    case canERR_NOMSG:
        qDebug()<<"Can Receive no msg !!" ;
        return -1;
        break;
    default:
        qDebug()<<"Can Receive error !";
        return -1;
        break;
    }

}
