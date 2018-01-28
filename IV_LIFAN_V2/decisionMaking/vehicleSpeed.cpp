#include "includes.h"
#define SPEED_LMT_VAL 2
#define MAXIMUM_THROTTLE 35
enum  STS_FLAG {INIT_S = 0, HIGH_S = 1, MID_S = 2, LOW_S = 3};
enum STS_FLAG speed_sts = INIT_S;
int delay_time = 2; //2 seconds
float  compensate_val = 0; //gas pedal value
float sum_delta_speed = 0;

#define NUM_COL  9
//float c_vehicle_speed[NUM_COL] =  {0, 5, 10, 15, 20, 25, 30, 35, 40};
//float c_alfa[NUM_COL] =                   {0, 10,  12,   13,  14, 15,  20, 25, 30};

float c_vehicle_speed[NUM_COL] =  {0, 5, 10, 15, 20, 25, 30, 35, 40};
float c_alfa[NUM_COL] =                   {0, 20,  28,   28,  30, 30,  31, 32, 35};
/*****11.29******
 * speed       open
 * 10               28
 * 20               30
 * 35               32
 * 40               35
 */

extern float OneDimInterp(float fXValue,  float *  pfXDim, unsigned int  nColMax,  float *  pfOneDim);

void DecisionMake::cacuObjSpeed()
{
    unsigned char vh_temp = 0;
    double too_close_dist = 30;

     //---------5-----------
     vh_temp =  gps_track[ this->cp_sn - 1][4]; //vehicle speed.
      if(vh_temp <= 5)   vh_temp = 5;

     //-----------4--------in zone
      if(flags.in_zone_whole_lane_end == true)
          vh_temp = 0;

      //----------3-------------
   /*   if(flags.gps_err == true)
          vh_temp = 2;
      else
      {;}  */



      //---------------------------------
      if(this->launch_go  == false) //stop when not find green light or LAUNCH BUTTON
      {
           vh_temp = 0;
      }

//&flags.in_zone_county_lane_stop1, &decisionMake.stop_time_elapsed);
      if(decisionMake.stop_time_elapsed >= 1)
      {
          decisionMake.stop_time_elapsed --;
      }
      if(flags.in_zone_county_lane_stop1 == true ||  (decisionMake.stop_time_elapsed != 0))
      {
                vh_temp = 0;
      }

          if(this->current_lane != this->which_lane)
          {
                vh_temp = 15;
          }

      //----------------------------------------------------
      //in county lane, must not detect the obstacles.
      if(flags.in_zone_county_lane_flag == false)
      {
        #ifdef HIGH_SPEED
          //------------------------------------------------------------
          if((this->obst_flag == true || this->is_logo == true) && (this->which_lane == CENTER_LANE) && (obst_dist <= 60))
          {
              vh_temp = 20;
          }
          else if((this->obst_flag_l == true || this->is_logo == true) && (this->which_lane == LEFT_LANE) && (obst_dist_l <= 60))
          {
              vh_temp = 20;
          }
          else   if((this->obst_flag_r== true || this->is_logo == true) && (this->which_lane == RIGHT_LANE) && (obst_dist_r <= 60) )
          {
              vh_temp = 20;
          }


          if((this->obst_flag == true || this->is_logo == true) && (this->which_lane == CENTER_LANE) && (obst_dist <= 40))
          {
              vh_temp = 0;
          }
          else if((this->obst_flag_l == true || this->is_logo == true) && (this->which_lane == LEFT_LANE) && (obst_dist_l <= 40))
          {
              vh_temp = 0;
          }
          else   if((this->obst_flag_r== true || this->is_logo == true) && (this->which_lane == RIGHT_LANE) && (obst_dist_r <= 40) )
          {
              vh_temp = 0;
          }

         #else
              //------------------------------------------------------------
              if((this->obst_flag == true || this->is_logo == true) && (this->which_lane == CENTER_LANE) && (obst_dist <= 30))
              {
                  vh_temp = 10;
              }
              else if((this->obst_flag_l == true || this->is_logo == true) && (this->which_lane == LEFT_LANE) && (obst_dist_l <= 30))
              {
                  vh_temp = 10;
              }
              else   if((this->obst_flag_r== true || this->is_logo == true) && (this->which_lane == RIGHT_LANE) && (obst_dist_l <= 30) )
              {
                  vh_temp = 10;
              }


              if((this->obst_flag == true) && (this->which_lane == CENTER_LANE) && (obst_dist <= 20))
              {
                  vh_temp = 0;
              }
              else if((this->obst_flag_l == true) && (this->which_lane == LEFT_LANE) && (obst_dist_l <= 20))
              {
                  vh_temp = 0;
              }
              else   if((this->obst_flag_r== true) && (this->which_lane == RIGHT_LANE) && (obst_dist_r <= 20) )
              {
                  vh_temp = 0;
              }
        #endif

      }

      //-------0---------key is pressed down
        //0, 1, 2
     if(decisionMake.key_code == KEY_STOP)//red
      {
          vh_temp = 0;
      }
      else if(decisionMake.key_code == KEY_GO) //green
      {
          vh_temp =  gps_track[ this->cp_sn - 1][4]; //vehicle speed.
            if(vh_temp <= 2)   vh_temp = 2;
      }
       else if(decisionMake.key_code ==KEY_NO)//cancel
      {
            ;
      }
      else //cancel
      { ;}
      //---------------------------
     if(vh_temp >= 40) vh_temp = 40;
      this->vehicle_speed_obj = vh_temp;
}


void DecisionMake::controlSpeed()
{
        float kp = 1,  ki = 0.5, delta = 0;
        float interp_val = 0, kp_val = 0, ki_val = 0;
        float  throttle_open_temp = 0;
        float brake_pressure_temp = 0;

        this->cacuObjSpeed();

        if(this->vehicle_speed > this->vehicle_speed_obj +SPEED_LMT_VAL)
        {
            speed_sts = HIGH_S;
        }
        else  if(this->vehicle_speed < this->vehicle_speed_obj - SPEED_LMT_VAL)
        {
            speed_sts = LOW_S;
        }
        else if((speed_sts == HIGH_S) && (this->vehicle_speed < this->vehicle_speed_obj))
        {
            speed_sts = MID_S;
        }
        else if((speed_sts == LOW_S) && (this->vehicle_speed > this->vehicle_speed_obj))
        {
            speed_sts = MID_S;
        }
        else
        {
            speed_sts = MID_S;
        }

        if(speed_sts == HIGH_S)
        {
            brake_pressure_temp  = 1; //1MPa
            /*throttle = 0*/
            throttle_open_temp = 0;

            compensate_val = 0; //clear to 0
        }
        else if((speed_sts = MID_S) || (speed_sts == LOW_S))
        {
                delta = this->vehicle_speed_obj - this->vehicle_speed;
                sum_delta_speed += delta;

                if(sum_delta_speed >= 5)
                    sum_delta_speed = 5;
                else if(sum_delta_speed <= -5)
                    sum_delta_speed = -5;

                 interp_val = OneDimInterp(  this->vehicle_speed_obj,  &c_vehicle_speed[0],   NUM_COL, &c_alfa[0]);
                 kp_val = kp * delta;
                 if(kp_val >= 10) kp_val = 10;
                 ki_val = ki * sum_delta_speed;

                throttle_open_temp =  interp_val +  kp_val + ki_val + compensate_val;//+5;

                if(throttle_open_temp >= MAXIMUM_THROTTLE)//for safety
                   throttle_open_temp= MAXIMUM_THROTTLE;

                /*brake = 0*/
                brake_pressure_temp = 0; //1MPa

                delay_time--;
                if(delay_time <= 0) delay_time = 0;

                 if(speed_sts == LOW_S && delay_time == 0)
                 {
                     delay_time = 2 * 1000 / 50; //2s * 1000 / 50,  the programe period is 50ms
                    compensate_val += 2;
                 }
        }

        if(this->vehicle_speed_obj <= 10)//for safety at low speed
        {
            if(throttle_open_temp >= MAXIMUM_THROTTLE)
                throttle_open_temp = MAXIMUM_THROTTLE;
        }

        if( this->vehicle_speed_obj <= 1)
        {
            this->throttle_open = 0;
            this->braking_pressure = 1;
        }
        else
        {
            this->throttle_open = throttle_open_temp;
            this->braking_pressure = brake_pressure_temp;
        }

        if(((track_row - 5) < cp_sn) && (cp_sn  <= track_row))
        {
           // this->throttle_open = 0;
           // this->braking_pressure = 3;
            this->urg_brake_flag  = true;
        }
}
