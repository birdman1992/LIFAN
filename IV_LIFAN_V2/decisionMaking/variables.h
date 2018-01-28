#ifndef VARIABLES_H
#define VARIABLES_H

#include "constant.h"

#ifdef OS_GLOBAL
    #define OS_EXT
#else
    #define OS_EXT extern
#endif


OS_EXT LowerLayer lower_layer;
OS_EXT Ibeo4 ibeo4;
OS_EXT GpsData gps_data;
OS_EXT DecisionMake decisionMake;
OS_EXT Parameters parameters;
OS_EXT Curvature curvature;
OS_EXT  Flags flags;
OS_EXT bool liar4_data_confirm;

//vector < vector<double> > gps_track(4000, vector<double> (7, 0));
OS_EXT vector < vector<double> > gps_track; //don't put into the Class.
OS_EXT vector<PointCoordinate> obstacles; //obstacles four points  value in global coordiante
//OS_EXT vector<bool> obst_color; //obstacles four points  value in global coordiante

#endif // VARIABLES_H
