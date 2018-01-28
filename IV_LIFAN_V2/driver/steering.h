#ifndef STEERING_H
#define STEERING_H

#include"canlib.h"
#include"can.h"

class Steering
{
public:
    Steering();

//    unsigned char ADAS_status;  //ADAS状态，0x0:Off;0x1:Ready;   0x2:Active;  0x3:Reserved;
//    unsigned char ADAS_ControlMode;//ADAS控制模式，0x0:AngleMode;0x1:TorqueMode
//    unsigned char TakeOverThreshold;//主动干预阈值，0x0:3.5Nm;0x1:5.5Nm
//    double ADAS_DesiredAngle;//ADAS期望角度,顺时针方向为“-”，逆时针方向为“+”
//    int ADAS_LimitAngleSpeed;//ADAS最大角速度限制,顺时针方向为“-”，逆时针方向为“+”
//    double ADAS_DesiredSteeringTorque;//ADAS期望扭矩,顺时针方向为“-”，逆时针方向为“+”
//    unsigned char HWA_RollingCounter_201;//计数器
//    unsigned char HWA_CheckSum_201;//校验和,Checksum  = (Byte1  +Byte2  …+  Byte7)  XOR 0xFF

    unsigned char EPS_ControlAvailable;//EPS控制有效状态,0x0：Invalid  0x1：Valid
    unsigned char EPS_DriverTakeOver;//驾驶员主动干预,0x0=No TakeOver,   0x1=TakeOver
    //EPS当前控制状态,0、Temp  inhibit（暂时不可用）；1、Available  for  control（EPS可接受控制）；2、Active（转向干预激活，EPS接受控制）;3、Permanently  failed（故障，EPS不能接受控制）
    unsigned char EPS_AvailabilityStatus;
    unsigned char EPS_EpsFailed;//助力转向故障,0x0=No Fail，  0x1=Fail
    double EPS_SteeringWheelAngle;//方向盘角度,顺时针方向为“-”，逆时针方向为“+”
    int EPS_SteeringWheelAngularSpeed;//方向盘角速度,顺时针方向为“-”，逆时针方向为“+”
    double EPS_SteeringTorque;//方向盘角扭矩,顺时针方向为“-”，逆时针方向为“+”
    unsigned char EPS_RollingCounter_221;//计数器
    unsigned char EPS_Checksum_221;//校验和,Checksum  = (Byte1  +Byte2  …+  Byte7)  XOR 0xFF

    void Angle_Contol(unsigned char ADAS_EN,unsigned char Threshold,double DesireValue,int LimitAngleSpeed);
    void Torque_Contol(unsigned char Threshold,double DesireValue,int LimitAngleSpeed);
    void DataToValid(unsigned char *data);
};

#endif // STEERING_H
