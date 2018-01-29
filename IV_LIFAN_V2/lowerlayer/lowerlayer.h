#ifndef LOWERLAYER_H
#define LOWERLAYER_H

#include "gear.h"

class LowerLayer
{
public:
    LowerLayer();
    void runAppLayer();
    void runLowerLayer();
    void inputDataSensor();
    void ouputDataActuator();
private: /*this is output variables*/
    unsigned char gear_en;
    unsigned char gear_value;

    unsigned char steering_en;
    float steering_angle;

    unsigned char braking_en;
    float braking_pressure;
    unsigned char braking_urg;

    unsigned char throttle_en;
    float throttle_open;

    unsigned char left_lamp;
    unsigned char right_lamp;
    unsigned char horn;
    unsigned char headlight;
};

#endif // LOWERLAYER_H


