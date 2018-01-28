#ifndef REMOTECTRL_H
#define REMOTECTRL_H

#include"can.h"

class RemoteCtrl
{
public:
    RemoteCtrl();
    void readMode();

    unsigned char auto_manual;
    unsigned char launch_stop;
    unsigned char urg_brake_flag;
    unsigned char key_code;
};

#endif // REMOTECTRL_H
