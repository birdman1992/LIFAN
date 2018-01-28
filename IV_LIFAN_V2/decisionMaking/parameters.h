#ifndef PARAMETERS_H
#define PARAMETERS_H


class Parameters
{
public:
    Parameters();
    void renewData();

public:
    float coeff_wheel_steering; //20

    double coeff_lane_x;
    double coeff_lane_y;
};

class Flags
{
public:
    Flags();
public:
    unsigned char traffic_light_detect_enable;
    unsigned char traffic_light_dir;
    unsigned char traffic_light_no_go;
    unsigned char dist_flag;


   unsigned char  in_zone_whole_lane_end;
    unsigned char  in_zone_whole_lane_end_flag;
   unsigned char  in_zone_county_lane_start;
   unsigned char  in_zone_county_lane_end;
   unsigned char  in_zone_county_lane_flag;

      unsigned char  in_zone_county_lane_stop1;
      unsigned char  in_zone_county_lane_stop2;
      unsigned char  in_zone_county_lane_stop3;



    bool gps_err;

};



#endif // PARAMETERS_H
