#ifndef VELODYNE_H
#define VELODYNE_H

#include"can.h"
class Velodyne
{
public:
    Velodyne();
    unsigned char obs_exist;
    void read();
};

#endif // VELODYNE_H
