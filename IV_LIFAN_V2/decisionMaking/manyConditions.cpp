
#include "includes.h"

#define  LANE_DIST_LMT 0.5

unsigned int lane_l_time = 0, lane_r_time = 0;

unsigned int p1 = 0, p2 = 0, times = 0, change_lane_finished_flag = 0;
void DecisionMake::changeLane()
{
    double obj_x_cor = 0, obj_y_cor = 0;
    double cur_x_cor = 0, cur_y_cor = 0;
    double dist_two_point = 0;
    //change to left or right lane, the left lane is priority.
    if(this->current_lane == LEFT_LANE && change_lane_finished_flag == 0)
    {
        if((this->obst_flag_l == true || this->is_logo == true) && (this->obst_flag == false)) //change right now
        {
            lane_l_time = 0;
            this->which_lane = CENTER_LANE;
        }
        else if(this->obst_flag == false) //go back automatically.
        {
            lane_l_time++;
            if(lane_l_time >= LANE_L_TIME_LMT)
            {
                lane_l_time = LANE_L_TIME_LMT;
                this->which_lane = CENTER_LANE;
            }
        }
        else
        {
            lane_l_time = 0;
            this->which_lane = LEFT_LANE;
        }
    }
    else if(this->current_lane == RIGHT_LANE && change_lane_finished_flag == 0)
    {
        if((this->obst_flag_r == true || this->is_logo == true ) && (this->obst_flag == false))
        {
            lane_r_time = 0;
            this->which_lane = CENTER_LANE;
        }
        else if(this->obst_flag == false) //go back automatically.
        {
            lane_r_time++;
            if(lane_r_time >= LANE_R_TIME_LMT)
            {
                lane_r_time = LANE_R_TIME_LMT;
                this->which_lane = CENTER_LANE;
            }
        }
        else
        {
            lane_r_time = 0;
            this->which_lane = RIGHT_LANE;
        }
    }
    else if(this->current_lane == CENTER_LANE && change_lane_finished_flag == 0)
    {
        lane_l_time = 0;
        lane_r_time = 0;

        if((this->obst_flag == true || this->is_logo == true) && (this->obst_flag_l == false)) //&& lane is solid type
        {
         //   if(this->obst_dist)
            this->which_lane = LEFT_LANE;
        }
        else if((this->obst_flag == true) && (this->obst_flag_r == false)) //&& lane is solid type
        {
            this->which_lane = RIGHT_LANE;
        }
        else
        {
            this->which_lane = CENTER_LANE;
        }
    }
    else
    {
//         this->which_lane = CENTER_LANE;
    }

    //----------------------------------------
    if(this->current_lane != this->which_lane)
    {
           times++;
           change_lane_finished_flag = 1;
          if(times >= 8*1000/50)
          {
                   times = 0;
                    change_lane_finished_flag = 0;
          }
    }
    else
    {
            change_lane_finished_flag = 0;
    }

    //---------------Judge finish the changing lane------------------
    cur_x_cor = this->x_cor;
    cur_y_cor = this->y_cor;

    if((this->current_lane == CENTER_LANE) && (this->which_lane == LEFT_LANE))
    {
        obj_x_cor = gps_track[this->cp_sn - 1][1] +  gps_delta_x_l(this->cp_sn);
        obj_y_cor = gps_track[this->cp_sn - 1][2] +  gps_delta_y_l(this->cp_sn);
         dist_two_point = this->distOfPoint(&cur_x_cor,  &cur_y_cor, &obj_x_cor, &obj_y_cor);
         if(dist_two_point <= LANE_DIST_LMT)
             this->current_lane = this->which_lane;
    }
    else if((this->current_lane == LEFT_LANE) && (this->which_lane == CENTER_LANE ))
    {
        obj_x_cor = gps_track[this->cp_sn - 1][1] +  gps_delta_x_m(this->cp_sn);
        obj_y_cor = gps_track[this->cp_sn - 1][2] +  gps_delta_x_m(this->cp_sn);
         dist_two_point = this->distOfPoint(&cur_x_cor,  &cur_y_cor, &obj_x_cor, &obj_y_cor);
         if(dist_two_point <= LANE_DIST_LMT)
             this->current_lane = this->which_lane;
    }
    else if((this->current_lane == CENTER_LANE) && (this->which_lane == RIGHT_LANE))
    {
        obj_x_cor = gps_track[this->cp_sn - 1][1] +  gps_delta_x_r(this->cp_sn);
        obj_y_cor = gps_track[this->cp_sn - 1][2] +  gps_delta_x_r(this->cp_sn);
         dist_two_point = this->distOfPoint(&cur_x_cor,  &cur_y_cor, &obj_x_cor, &obj_y_cor);
         if(dist_two_point <= LANE_DIST_LMT)
             this->current_lane = this->which_lane;
    }
    else if((this->current_lane == RIGHT_LANE) && (this->which_lane == CENTER_LANE ))
    {
        obj_x_cor = gps_track[this->cp_sn - 1][1] +  gps_delta_x_m(this->cp_sn);
        obj_y_cor = gps_track[this->cp_sn - 1][2] +  gps_delta_x_m(this->cp_sn);
         dist_two_point = this->distOfPoint(&cur_x_cor,  &cur_y_cor, &obj_x_cor, &obj_y_cor);
         if(dist_two_point <= LANE_DIST_LMT)
             this->current_lane = this->which_lane;
    }

    //-----------------------------------------------

      {
            if(this->which_lane == CENTER_LANE)
            {
                parameters.coeff_lane_x  = 0;
               parameters. coeff_lane_y =  0;
            }
            else if(this->which_lane == LEFT_LANE)   //turn left
            {
                parameters.coeff_lane_x  = 0 - LANE_WIDTH;
                parameters.coeff_lane_y = LANE_WIDTH;
            }
            else if(this->which_lane == RIGHT_LANE)  //turn right
            {
                parameters.coeff_lane_x  = LANE_WIDTH;
                parameters.coeff_lane_y =  0 - LANE_WIDTH;
            }
      }
}

void DecisionMake::JudgeInZone(double *pZone, unsigned int ROW, unsigned int COL, double A_AZIMUTH, double A_DIST, unsigned char *flag_enable, unsigned int *dir)
{
    unsigned int i = 0;
    double cali_point_azimuth = 0, del_azimuth = 0, cur_azimuth_temp = 0;
    double  cali_x_cor = 0, cali_y_cor = 0, cur_x_cor = 0, cur_y_cor = 0;
    double dist_two_point = 0;

    for( i = 0; i < ROW; i++)
    {
        cali_x_cor = *(pZone + i * COL + 0); //x
        cali_y_cor = *(pZone + i * COL + 1); //y

        cali_point_azimuth = cacuCaliCurAzimuth(cali_x_cor,  cali_y_cor); //azimuth between the vehicle and the specified point

        cur_azimuth_temp = *(pZone + i * COL + 2); //obj azimuth

        del_azimuth = fabs(delta_azimuth(&cali_point_azimuth, &cur_azimuth_temp));

        if(del_azimuth <= A_AZIMUTH)
        {
                cur_x_cor = this->x_cor;
                cur_y_cor = this->y_cor;
                dist_two_point = this->distOfPoint(&cur_x_cor, &cur_y_cor, &cali_x_cor, &cali_y_cor);
                if(dist_two_point <= A_DIST)
                {
                      *flag_enable = true;
                      *dir = *(pZone + i * COL + 3); //only for traffic light, turn left or right
                      break;
                }
                else
                {
                    *flag_enable  = false;
                    *dir = NONE_TURN;
                }
        }
        else
        {
            *flag_enable  = false;
            *dir = NONE_TURN;
        }
    }
}

/*longitude latitude, azimuth
   the position for detect the traffic light
*/
#define TRAFFIC_LIGHT_ROW 12
#define TRAFFIC_LIGHT_COL  4
#define DETECT_TRAFFIC_LIGHT_DIST 20 //unit is meter
#define DETECT_TRAFFIC_LIGHT_AZIMUTH 10 //unit is degree

double trafficLightZone[TRAFFIC_LIGHT_ROW][TRAFFIC_LIGHT_COL] =
{
    {1143625.88317436, 3516694.1829042, 63.452, MID_TURN},//1(m,r)
    {1143667.30673718, 3516689.8825675, 324.818, MID_TURN}, //2(l, r)
    {1143669.38127779, 3516733.1802788, 240.836, MID_TURN},//4(m,l)
    {1143803.36103555, 3516798.9145030, 62.259,  MID_TURN},//3(m,r)
    {1143851.53814158, 3516801.1210140, 318.893, LEFT_TURN},//5(l,r)
    {1143848.30216147, 3516836.51759647, 240.245, MID_TURN},//6(m,l)
    {1143922.52807507, 3516702.65487537, 140.453, MID_TURN}, //7(l,m,r)
    {1143971.28977374, 3516662.01968462, 323.407, MID_TURN},//8(l,m,r)
    {1143970.10685475, 3516699.74391636, 237.954, MID_TURN}, //9(l,m,r)
    {1143923.52248221, 3516664.88544099, 56.654, MID_TURN}, //10(l,m,r)
    {1144203.37241059, 3516350.56138055, 144.691, RIGHT_TURN}, //11(r)
    {1143907.61708452, 3516111.10920463, 237.509, RIGHT_TURN}//12(r)
};

void DecisionMake::trafficLight()
{
    unsigned int dir_temp = 0;
    double val = 0;
    JudgeInZone(&trafficLightZone[0][0], TRAFFIC_LIGHT_ROW, TRAFFIC_LIGHT_COL, DETECT_TRAFFIC_LIGHT_AZIMUTH,
           DETECT_TRAFFIC_LIGHT_DIST,  &flags.traffic_light_detect_enable, &dir_temp);

    val = curvature.cacuCurvature();
    if(val > 0.005)
       flags.traffic_light_dir =  LEFT_TURN;
    else if(val < -0.005)
         flags.traffic_light_dir = RIGHT_TURN;
    else
        flags.traffic_light_dir =  MID_TURN;

   if(flags.traffic_light_dir == LEFT_TURN) //for the turn light
   {
      decisionMake.left_lamp = true;
   }
   else if(flags.traffic_light_dir == RIGHT_TURN)
 {
       decisionMake.right_lamp = true;
   }
   else
   {
        decisionMake.left_lamp = false;
        decisionMake.right_lamp = false;
   }

     if(flags.traffic_light_detect_enable == true)
    {
            // turn according to the light.
            if( flags.traffic_light_dir /*dir_temp*/ == LEFT_TURN)
            {
                    if(this->lamp_color_l == RED_LIGHT)
                        flags.traffic_light_no_go = true;
                    else if(this->lamp_color_l == GREEN_LIGHT)
                        flags.traffic_light_no_go = false;
            }
            else if( flags.traffic_light_dir  == RIGHT_TURN)
            {
                    if(this->lamp_color_r == RED_LIGHT)
                        flags.traffic_light_no_go = true;
                    else if(this->lamp_color_r == GREEN_LIGHT)
                        flags.traffic_light_no_go = false;
            }
            else if( flags.traffic_light_dir  == MID_TURN)
            {
                    if(this->lamp_color_m == RED_LIGHT)
                        flags.traffic_light_no_go = true;
                    else if(this->lamp_color_m == GREEN_LIGHT)
                        flags.traffic_light_no_go = false;
            }
            else
            {
                    flags.traffic_light_no_go = false;
            }
    }
    else
    {
        flags.traffic_light_no_go = false;
        decisionMake.right_lamp = false;
        decisionMake.left_lamp = false;
    }
}

#define STOP_TIME_ELAPSE 5*1000/50

double specifiedZone[ZONE_ROW][ZONE_COL] =
{
    {0, 0,0,0}, //whole lane end  0
    {1144102.17216993, 3516537.77111035, 237.59, 0}, //end point of county lane 1
    {1144327.64930227, 3516672.60221602, 169.48, 0}, //start point of county lane 2
    {1143877.00959332, 3516410.86031396 , 143.347, STOP_TIME_ELAPSE},//  stop1. --3
    {1143909.8874881,   3516379.4194598,    324.717, STOP_TIME_ELAPSE}, //  stop2. --4
    {1143782.35801868, 3516572.36154209, 239.609,  STOP_TIME_ELAPSE}  //tunnel stop3 --5
};

void DecisionMake::detectZone()
{
    unsigned int  dir_temp = 0;
    JudgeInZone(&specifiedZone[0][0], 1, ZONE_COL, ZONE_AZIMUTH,
           3/*ZONE_DIST*/,  &flags.in_zone_whole_lane_end, &dir_temp);
if(flags.in_zone_county_lane_end == true) flags.in_zone_whole_lane_end_flag = true;

    JudgeInZone(&specifiedZone[1][0], 1, ZONE_COL, ZONE_AZIMUTH,
           10/*ZONE_DIST*/,  &flags.in_zone_county_lane_end, &dir_temp);

    JudgeInZone(&specifiedZone[2][0], 1, ZONE_COL, ZONE_AZIMUTH,
           10/*ZONE_DIST*/,  &flags.in_zone_county_lane_start, &dir_temp);

    JudgeInZone(&specifiedZone[3][0], 1, ZONE_COL, ZONE_AZIMUTH,
           10/*ZONE_DIST*/,  &flags.in_zone_county_lane_stop1, &decisionMake.stop_time_elapsed);

    JudgeInZone(&specifiedZone[4][0], 1, ZONE_COL, ZONE_AZIMUTH,
           10/*ZONE_DIST*/,  &flags.in_zone_county_lane_stop2, &decisionMake.stop_time_elapsed);

    JudgeInZone(&specifiedZone[5][0], 1, ZONE_COL, ZONE_AZIMUTH,
           10/*ZONE_DIST*/,  &flags.in_zone_county_lane_stop3, &decisionMake.stop_time_elapsed);

   if(flags.in_zone_county_lane_start == true)
   {
       flags.in_zone_county_lane_end = false;
      flags.in_zone_county_lane_flag = true;
   }

   if(flags.in_zone_county_lane_end == true)
   {
       flags.in_zone_county_lane_start = false;
       flags.in_zone_county_lane_flag  = false;
   }

}

//if(this->current_lane == LEFT_LANE)
//{
   // if((this->obst_flag_l == true)

void DecisionMake::manyConditions()
{
    //in county lane, must not change lane
//    if(flags.in_zone_county_lane_flag == false)
    {
      if(this->vehicle_speed <= CHANG_LANE_SPEED)
        {
                this->changeLane();
        }
    }

      this->trafficLight();
     this->detectZone();

 }
