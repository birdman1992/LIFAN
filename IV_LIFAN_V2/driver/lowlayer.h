#ifndef LOWLAYER_H
#define LOWLAYER_H


class lowlayer
{
public:
    lowlayer();
    void Initial();
    int Mode_control(unsigned char mode);
    int Gear_control(unsigned char gear);
    int Steering_control(int steering_angle);
    int Throttle_control(short throttle);
    int brake_control(unsigned char brake_ist);
    int Read_status();

    //三个运行接口
    void inputDataSensor();
    void outputDataActuator();
    void runLowerLayer();

    int Initial_flag;
    unsigned char cur_mode;
    unsigned char cur_gear;
    int cur_steering_angle;
    short cur_throttle;
    unsigned char cur_brake_ist;
};

#endif // LOWLAYER_H
