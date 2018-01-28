#ifndef IBEO4_H
#define IBEO4_H

/*
#include <ibeosdk/ecu.hpp>
#include <ibeosdk/IpHelper.hpp>
#include <ibeosdk/datablocks/commands/CommandEcuAppBaseStatus.hpp>
#include <ibeosdk/datablocks/commands/ReplyEcuAppBaseStatus.hpp>
#include <ibeosdk/datablocks/commands/CommandEcuAppBaseCtrl.hpp>
#include <ibeosdk/datablocks/commands/EmptyCommandReply.hpp>
#include<ibeosdk/datablocks/snippets/ObjectEcuEt.hpp>
#include <ibeosdk/listener/DataListener.hpp>*/

#include <ibeosdk/ecu.hpp>
#include <ibeosdk/IpHelper.hpp>
#include <ibeosdk/datablocks/commands/CommandEcuAppBaseStatus.hpp>
#include <ibeosdk/datablocks/commands/ReplyEcuAppBaseStatus.hpp>
#include <ibeosdk/datablocks/commands/CommandEcuAppBaseCtrl.hpp>
#include <ibeosdk/datablocks/commands/EmptyCommandReply.hpp>
#include<ibeosdk/datablocks/snippets/ObjectEcuEt.hpp>
#include <ibeosdk/listener/DataListener.hpp>

#include<vector>
#include <iostream>
#include <cstdlib>
#include<QTime>
#include<iostream>
using namespace ibeosdk;


struct Ibeodata
{
    unsigned int unmber;
    unsigned int ID;
    unsigned int classfication;
    float boxsizex;
    float boxsizey;
    float boxcenterx;
    float boxcentery;
    float angle;
    float Avelocityx;
    float Avelocityy;
    float x;
    float y;
    float z;
};

class Ibeo4
{
public:
    Ibeo4();
    void getOriFourPointObst();
    void rotateCoordinates(double *azimuth, float *ori_p1_x, float *ori_p1_y, float *ori_p2_x, float *ori_p2_y, float *ori_p3_x, float *ori_p3_y, float *ori_p4_x, float *ori_p4_y,
                                          double *obj_p1_x, double *obj_p1_y, double *obj_p2_x, double *obj_p2_y, double *obj_p3_x, double *obj_p3_y, double *obj_p4_x, double *obj_p4_y );
    void transCoordianteVehToGlobal();
    void getObstGlobalCoord();
};

#endif // IBEO4_H
