#include "braking.h"
#include "gear.h"
#include "steering.h"
#include "throttle.h"
#include "lightHorn.h"
#include "remoteCtrl.h"
#include "transmission.h"
#include "lowerlayer.h"
#include "decisionMake.h"
#include "QDebug"
#include "gps.h"
#include "decisionMake.h"
#include "trafficlight.h"
#include "velodyne.h"
#include "logo.h"

//
#define DEBUG_TEST

extern GpsData gps_data;
extern DecisionMake decisionMake;

Braking brake;
Gear gear;
Steering steering;
Throttle throttle;
LightHorn light_horn;
RemoteCtrl remote_ctrl;
Transmission transmission;
Trafficlight trafficlight;
Velodyne velodyne;
Logo logo;


LowerLayer::LowerLayer()
{
    gear_en = 0;
    gear_value = 0;

    steering_en = 0;
    steering_angle = 0;

    braking_en = 0;
    braking_pressure = 0;
    braking_urg = 0;

    throttle_en = 0;
    throttle_open = 0;

    left_lamp = 0 ;
    right_lamp = 0;
    horn = 0;
}

void LowerLayer::inputDataSensor()
{
    /*pass sensor datum to decision making program*/
    driver_lowlayer.Initial();
    driver_lowlayer.Read_status();

    remote_ctrl.readMode();
    brake.readBrksts();
    trafficlight.read();
    velodyne.read();
    logo.read();
    decisionMake.longtitude = gps_data.m_strLongitude;
    decisionMake.latitude = gps_data.m_strLatitude;
    decisionMake.cur_azimuth = gps_data.m_strHeading;
    decisionMake. vehicle_speed = transmission.readCurSpeed();
    decisionMake.cur_gear = gear.cur_gear;
    decisionMake.auto_manual = remote_ctrl.auto_manual;
    decisionMake.launch_stop = remote_ctrl.launch_stop;
    decisionMake.key_code = remote_ctrl.key_code;
    decisionMake.urg_brake_flag = remote_ctrl.urg_brake_flag | brake.brk_urg_sts;
    decisionMake.lamp_color_l = trafficlight.lamp_color_l;
    decisionMake.lamp_color_r = trafficlight.lamp_color_r;
    decisionMake.lamp_color_m = trafficlight.lamp_color_m;
    decisionMake.obst_cloth = velodyne.obs_exist;
    decisionMake.is_logo = logo.isLogo;

#ifdef DEBUG_TEST
    qDebug()<<"[cur_brake_ist]"<<driver_lowlayer.cur_brake_ist;
    qDebug()<<"[cur_gear]"<<driver_lowlayer.cur_gear<<"||"<<gear.cur_gear;
    qDebug()<<"[cur_mode]"<<driver_lowlayer.cur_mode<<"||"<<remote_ctrl.auto_manual<<remote_ctrl.launch_stop<<remote_ctrl.key_code<<(remote_ctrl.urg_brake_flag | brake.brk_urg_sts);
    qDebug()<<"[cur_steering_angle]"<<driver_lowlayer.cur_steering_angle;
    qDebug()<<"[cur_throttle]"<<driver_lowlayer.cur_throttle;
#endif
}

void LowerLayer::ouputDataActuator()
{
    /*pass results to actuators*/
    this->steering_en = decisionMake.steering_en;
    this->steering_angle=decisionMake.steering_angle;

    this->braking_en = decisionMake.braking_en;
    this->braking_pressure=decisionMake.braking_pressure;
    this->braking_urg = decisionMake.braking_urg;

    this->throttle_en = decisionMake.throttle_en;
    this->throttle_open=decisionMake.throttle_open;

    this->gear_en = decisionMake.gear_en;
    this->gear_value=decisionMake.gear_value;

    this->left_lamp = decisionMake.left_lamp;
    this->right_lamp = decisionMake.right_lamp;
    this->headlight = decisionMake.headlight;

    this->horn = decisionMake.horn;
}

void LowerLayer::runLowerLayer()
{
    gear.controlGear(gear_en, gear_value);
    light_horn.controlLightHorn(left_lamp, right_lamp, horn,headlight);
    brake.brakingControl(braking_en, braking_pressure);
    steering.controlAngle(steering_en,0,steering_angle,300);//干预阈值，目标角度，最大角速度
    throttle.controlThrottle(throttle_en,throttle_open,0);//EN，目标节气门开度，目标节气门速度
}

void LowerLayer::runAppLayer()
{
   //  rt_OneStep();   //先注释
}
