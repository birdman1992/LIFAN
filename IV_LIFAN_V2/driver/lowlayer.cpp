#include "lowlayer.h"
#include"usbcan.h"

lowlayer::lowlayer()
{
}

void lowlayer::Initial()
{
    if(Initial_flag !=1)
    {
        usbcan can;
        if(can.Initial(4,0,0) == 0)//can initialize success!!
        {
            DWORD dwRel;
            VCI_CAN_OBJ vco;
            vco.ID = 0x0081;
            vco.RemoteFlag = 0;
            vco.ExternFlag = 0;
            vco.DataLen = 8;
            vco.Data[0] = 0b00100000;//请求控制权
            dwRel = VCI_Transmit(4,0,0,&vco,1);
            if(dwRel == 1)
            {
                DWORD dwRel1;
                VCI_CAN_OBJ rco[2500];
                dwRel1 = VCI_Receive(4,0,0,rco,2500,0);
                if(dwRel1 >0)
                {
                    if((rco[0].ID == 0x0082)&&(rco[0].Data[0] == 0b00100000))
                        Initial_flag = 1;
                }
                else
                {
                    Initial_flag = 0;
                }
            }
            else
            {
                Initial_flag = 0;
            }
        }
        else
        {
            Initial_flag = 0;
        }
    }
}

int lowlayer::Mode_control(unsigned char mode)
{
    if(Initial_flag == 1)
    {
        DWORD dwRel;
        VCI_CAN_OBJ vco;
        vco.ID = 0x0081;
        vco.RemoteFlag = 0;
        vco.ExternFlag = 0;
        vco.DataLen = 8;
        vco.Data[0] = 0x02;//智能控制数据发送
        vco.Data[2]= mode;
        dwRel = VCI_Transmit(4,0,0,&vco,1);
        if(dwRel == 1)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    return -1;
}

int lowlayer::Gear_control(unsigned char gear)//0空挡、1前进档、2倒挡
{
    if(Initial_flag == 1)
    {
        DWORD dwRel;
        VCI_CAN_OBJ vco;
        vco.ID = 0x0081;
        vco.RemoteFlag = 0;
        vco.ExternFlag = 0;
        vco.DataLen = 8;
        vco.Data[0] = 0x02;//智能控制数据发送
        vco.Data[2]= (gear&&0x0f)<<4;
        dwRel = VCI_Transmit(4,0,0,&vco,1);
        if(dwRel == 1)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    return -1;
}

int lowlayer::Steering_control(int steering_angle)//-600~600
{
    if(Initial_flag == 1)
    {
        DWORD dwRel;
        VCI_CAN_OBJ vco;
        vco.ID = 0x0081;
        vco.RemoteFlag = 0;
        vco.ExternFlag = 0;
        vco.DataLen = 8;
        vco.Data[0] = 0x02;//智能控制数据发送
        steering_angle = 0x0400 + steering_angle;
        vco.Data[4] = (unsigned char)((steering_angle|0xff00)>>8);
        vco.Data[5] = (unsigned char)(steering_angle|0x00ff);
        dwRel = VCI_Transmit(4,0,0,&vco,1);
        if(dwRel == 1)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    return -1;
}

int lowlayer::Throttle_control(short throttle)//0~4095
{
    if(Initial_flag == 1)
    {
        DWORD dwRel;
        VCI_CAN_OBJ vco;
        vco.ID = 0x0081;
        vco.RemoteFlag = 0;
        vco.ExternFlag = 0;
        vco.DataLen = 8;
        vco.Data[0] = 0x02;//智能控制数据发送
        vco.Data[2] = (unsigned char)((throttle&&0x0f00)>>8);
        vco.Data[3] = (unsigned char)(throttle&&0x00ff);
        dwRel = VCI_Transmit(4,0,0,&vco,1);
        if(dwRel == 1)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    return -1;
}

int lowlayer::brake_control(unsigned char brake_ist)
{
    if(Initial_flag == 1)
    {
        DWORD dwRel;
        VCI_CAN_OBJ vco;
        vco.ID = 0x0081;
        vco.RemoteFlag = 0;
        vco.ExternFlag = 0;
        vco.DataLen = 8;
        vco.Data[0] = 0x02;//智能控制数据发送
        vco.Data[6] = brake_ist;
        dwRel = VCI_Transmit(4,0,0,&vco,1);
        if(dwRel == 1)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    return -1;
}

int lowlayer::Read_status()
{
    DWORD dwRel;
    VCI_CAN_OBJ rco[100];
    dwRel = VCI_Receive(4,0,0,rco,100,0);
    if(dwRel >0)
    {
        if(rco[0].ID == 0x0082)
        {
            this->cur_mode = (rco[0].Data[0]&0b00010000)>>4;
            this->cur_gear = (rco[0].Data[0]&0b00001100)>>2;
            int temp = 0;
            temp =(int) (rco[0].Data[5]<<8|rco[0].Data[6])|0xffff;
            temp = temp - 0x0400;
            this->cur_steering_angle = temp;
        }
    }
}
