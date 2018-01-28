#ifndef DECISIONMAKE_H
#define DECISIONMAKE_H
#include <vector>
#include "parameters.h"
#include "constant.h"

using namespace std;
extern vector < vector<double> > gps_track;

class DecisionMake
{
public:
    DecisionMake();
    void initiate();
    void  initiateTrack();
    void inputData();
    void  followTrack();
    void seekOriPoint();
    void seekAimPoint();
    void CovertBLToXY(double latitude,double longitude,double centerLongitude,double *cy,double *cx,double *cr);
    void arrangeProg();
    double distOfPoint(double *x1, double *y1, double *x2, double *y2);
    void interp_1table();
    double cacuAimObjDistance();
    double delta_azimuth(double* cur_azimuth, double* aim_azimuth);
    double verticalLineDist();
    double includedAngle();
    double cacuAimCurAzimuth();
    double cacuCaliCurAzimuth(double, double);
    void JudgeInZone(double *pTrafficLightZone, unsigned int ROW, unsigned int COL, double A_AZIMUTH, double A_DIST, unsigned char *flag_enable, unsigned int *dir);
    float cacuFrontWheelAngle(double* delta_angle, double* aim_dist);
    void manyConditions();
    void checkObstacles();
    void checkObstacles_l();
    void checkObstacles_r();
    void getVehFourCrdGlobal(double cor_x, double cor_y, double azimuth,
                            double *s_p1_x, double *s_p1_y, double *s_p2_x, double *s_p2_y, double *s_p3_x, double *s_p3_y, double *s_p4_x, double *s_p4_y );
    bool detectObstOverlay(double &aim_x1,double &aim_y1,double &aim_x2,double &aim_y2,double &aim_x3,double &aim_y3,double &aim_x4,double &aim_y4,double &orb_x1,double &orb_y1,double &orb_x2,double &orb_y2,double &orb_x3,double &orb_y3,double &orb_x4,double &orb_y4);
    void controlSpeed();
    void selectMode();
    void trafficLight();
    void changeLane();
    void cacuObjSpeed();
    void detectZone();
    void initVariables();
public: /*middle variables*/
    // vector <vector<double> > gps_track;  /*0:number, 1:x_longitude 2:y_latitude, 3:direction, 4:vehicle speed, 5:track segment, 6. speical point*/
     unsigned int long track_row;
     unsigned int long track_col;
      bool re_seek_original_flag; //whether to re-seek start point
      unsigned int long cp_sn; // closest point sequence number
      unsigned int long srch_start_sn;
      unsigned int long srch_end_sn;
      unsigned int long aim_point_sn;


      double x_cor; //current x coordinate value
      double y_cor; //current y corrdinate value
      double c_cor; //current c value
      double aim_dist_obj; //the aim distance.
      double aim_dist_real; //the aim distance.

      double aim_cur_angle;
      float front_wheel_angle;
      float verticalDist;
      double curvatureVal;

      struct PointCoordinate cur;
      struct PointCoordinate aim;
      struct PointCoordinate aim_l;
      struct PointCoordinate aim_r;
      struct PointCoordinate end;
      struct PointCoordinate end_l;
      struct PointCoordinate end_r;
      struct PointCoordinate dyn;
       struct PointCoordinate dyn_l;
        struct PointCoordinate dyn_r;

      unsigned int obstacles_num;
      bool obst_flag;
      bool obst_flag_l ;
      bool obst_flag_r;
      float obst_dist;
      float obst_dist_l;
      float obst_dist_r;
      unsigned char  vehicle_speed_obj;
      unsigned char  gear_obj_in_auto;
      unsigned char key_code;
      unsigned char which_lane;
      unsigned char current_lane;
      bool changing_lane_flag;
      bool launch_go;
     bool obst_cloth;
     bool is_logo;
     unsigned int stop_time_elapsed;


public: /*this is output variables*/
    unsigned char gear_en;
    unsigned char gear_value;

    unsigned char steering_en;
    float steering_angle;

    unsigned char throttle_en;
    float throttle_open;

    unsigned char braking_en;
    float braking_pressure;
    unsigned char  braking_urg;

    unsigned char left_lamp;
    unsigned char right_lamp;
    unsigned char headlight;
    unsigned char horn;

public: /*this is for input variable*/
    double longtitude;
    double latitude;
    double cur_azimuth;
    double aim_azimuth;

    unsigned char cur_gear;
    unsigned char  vehicle_speed;

    unsigned char auto_manual;
    unsigned char launch_stop;
    unsigned char urg_brake_flag; //from the remote control
    unsigned char brk_urg_sts; //from the urgent button

    unsigned char  line_type_l; //solid(true)and dash(false)
    unsigned char  line_type_r;
    double line_dist; //+,vehicle is left to the centerline of two lanes.  -: right

    unsigned char  line_type_l_flag; //
    unsigned char  line_type_r_flag;//
    double line_dist_flag; //

    unsigned char lamp_color_l; //1:red, 0:green
    unsigned char lamp_color_r; //1:red, 0:green
    unsigned char lamp_color_m; //1:red, 0:green



};


#endif // GPSTRACK_H
