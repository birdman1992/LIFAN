#ifndef CONSTANT_H
#define CONSTANT_H
#define LANE_WIDTH 3.5

#define gps_delta_x(sn)   parameters.coeff_lane_x*cos(gps_track[sn - 1][3] * 3.141592627 / 180)
#define gps_delta_y(sn)   parameters.coeff_lane_y*sin(gps_track[sn - 1][3] * 3.141592627 / 180)

#define gps_delta_x_m(sn)   0*cos(gps_track[sn - 1][3] * 3.141592627 / 180)
#define gps_delta_y_m(sn)   0*sin(gps_track[sn - 1][3] * 3.141592627 / 180)

#define gps_delta_x_l(sn)   (-LANE_WIDTH)*cos(gps_track[sn - 1][3] * 3.141592627 / 180)
#define gps_delta_y_l(sn)   (LANE_WIDTH)*sin(gps_track[sn - 1][3] * 3.141592627 / 180)

#define gps_delta_x_r(sn)   (LANE_WIDTH)*cos(gps_track[sn - 1][3] * 3.141592627 / 180)
#define gps_delta_y_r(sn)   (-LANE_WIDTH)*sin(gps_track[sn - 1][3] * 3.141592627 / 180)

#define RED_LIGHT 1
#define GREEN_LIGHT 0

#define NONE_TURN 0
#define LEFT_TURN 1
#define MID_TURN 2
#define RIGHT_TURN 3

#define CENTER_LANE 0
#define LEFT_LANE 1
#define RIGHT_LANE 2

/*id, type, couter(exist), times(disapear), distance*/
#define OBST_ROW  10
#define OBST_COL  13 //5+8
#define OBST_DISP_TIMES  6
#define OBST_EXIST_TIMES  6 //10*100ms

#define OBST_ROW_L  10
#define OBST_COL_L  13
#define OBST_DISP_TIMES_L  6
#define OBST_EXIST_TIMES_L  4 //10*100ms

#define OBST_ROW_R  10
#define OBST_COL_R  13
#define OBST_DISP_TIMES_R  6
#define OBST_EXIST_TIMES_R  4 //10*100ms

#define KEY_NO    0
#define KEY_STOP 1
#define KEY_GO    2

#define ZONE_ROW 6
#define ZONE_COL  4
#define ZONE_DIST 10 //unit is meter
#define ZONE_AZIMUTH 45 //unit is


#define HIGH_SPEED

#ifdef HIGH_SPEED
#define HIGH_SPEED_DIST  20
#define CHANG_LANE_DIST 10
#define CHANG_LANE_SPEED  18

#define  LANE_L_TIME_LMT   15*1000/50
#define  LANE_R_TIME_LMT   15*1000/50

#else
#define HIGH_SPEED_DIST 0
#define CHANG_LANE_DIST 0
#define CHANG_LANE_SPEED 15

#define  LANE_L_TIME_LMT   5*1000/50
#define  LANE_R_TIME_LMT   5*1000/50
#endif

struct PointCoordinate
{
    double p1_x; /*rectangle of the vehicle*/
    double p1_y;
    double p2_x;
    double p2_y;
    double p3_x;
    double p3_y;
    double p4_x;
    double p4_y;
    float closest_p_dist; // distance between cloest point and (center point of vehicle) in vehicle coordiance.
    unsigned int id;
    unsigned int classficiation;
};

#endif // CONSTANT_H
