
#include "includes.h"

unsigned int long data_sn = 0, closest_point_sn = 0;
double dist_two_point = 0;
double dist_min = 0;
double obj_x_cor = 0, obj_y_cor = 0;
double cur_x_cor = 0, cur_y_cor = 0;
double obj_azimuth = 0, delta_angle_now = 0;

//vector < vector<double> >* pGps_track;

void DecisionMake::seekOriPoint(void)
{
        //pGps_track = &gps_track;
         //m1 = (*pGps_track)[0][1];

        /*re-seek the start point*/
        dist_min = 1000; //the unit is meter
        if(this->re_seek_original_flag == false)
        {
            this->srch_start_sn = 1;
            this->srch_end_sn = this->track_row;
            if((latitude != 0) && (longtitude != 0))
            {
                this->re_seek_original_flag = true;
            }
        }
        else
        {
            this->srch_start_sn = this->cp_sn;
            this->srch_end_sn = this->aim_point_sn;
        }

        for(data_sn = this->srch_start_sn; data_sn <= this->srch_end_sn; data_sn++) /*seek for each point*/
        {
            //void CovertBLToXY(double latitude,double longitude,double centerLongitude,double *cx,double *cy,double *cr)
               //dist_two_point = 0;
              /*transfer the XY coordinate value of track poin*/
               obj_x_cor = gps_track[data_sn - 1][1] +  gps_delta_x(data_sn); //longitude 388011.84;//
               obj_y_cor = gps_track[data_sn - 1][2] +  gps_delta_y(data_sn); //latitude 3123060.50;//
               obj_azimuth = gps_track[data_sn - 1][3]; //the azimuth

               cur_x_cor = this->x_cor;
               cur_y_cor = this->y_cor;

                delta_angle_now = obj_azimuth - this->cur_azimuth;
               if(delta_angle_now >180)
               {
                       delta_angle_now = delta_angle_now - 360;
               }
               else if(delta_angle_now < -180)
               {
                   delta_angle_now = delta_angle_now + 360;
               }
              // delta_angle_now = fabs(delta_angle_now); //keep the positive value

               /*caculate the distance of current point and each track point*/
               dist_two_point = this->distOfPoint(&cur_x_cor,  &cur_y_cor, &obj_x_cor, &obj_y_cor);

               if((dist_two_point <= dist_min) && (delta_angle_now < 45 && delta_angle_now > -45))
               {
                          dist_min = dist_two_point ;
                         closest_point_sn = (unsigned int)gps_track[data_sn - 1][0]; //record the point.
               }
               else{;} //do nothing
        }
        if(closest_point_sn <= 0)
            closest_point_sn = 1;
        else if(closest_point_sn >= track_row)
            closest_point_sn = track_row;
        this->cp_sn = closest_point_sn;
}

/*renew the this->aim_point_sn*/
void DecisionMake::seekAimPoint(void)
{
   double dist_two_point = 0;
   double obj_x_cor = 0, obj_y_cor = 0;
   double cur_x_cor = 0, cur_y_cor = 0;
   double fst_x_cor = 0, fst_y_cor = 0;


   unsigned int long  aim_point_sn_temp = 0;

    /*caculate the aim distance*/
    this->aim_dist_obj = this->cacuAimObjDistance();  //km/h
    /*search the point mathes the distance*/

     cur_x_cor = gps_track[ this->cp_sn - 1][1] +  gps_delta_x(cp_sn); //longitude 388011.84;//
     cur_y_cor = gps_track[ this->cp_sn - 1][2] +  gps_delta_y(cp_sn); //latitude 3123060.50;//

     aim_point_sn_temp = this->cp_sn;//closest point

     while(dist_two_point < this->aim_dist_obj) /*to get the aim point on the track*/
     {
         obj_x_cor = gps_track[ aim_point_sn_temp - 1][1] +  gps_delta_x(aim_point_sn_temp); //longitude
         obj_y_cor = gps_track[ aim_point_sn_temp - 1][2] +  gps_delta_y(aim_point_sn_temp); //latitude

        dist_two_point =   this->distOfPoint(&cur_x_cor, &cur_y_cor, &obj_x_cor, &obj_y_cor);
        aim_point_sn_temp++;

        if(aim_point_sn_temp >= this->track_row) //maximun num
        {
            aim_point_sn_temp = this->track_row;
            break;
        }
     }

     if(aim_point_sn_temp <= 0)
         aim_point_sn_temp = 1;
     else if(aim_point_sn_temp >= track_row)
     {
         aim_point_sn_temp = track_row;
     }

     this->aim_point_sn = aim_point_sn_temp;

     /*caculate the real distance between current and aim points*/
     obj_x_cor = gps_track[ this->aim_point_sn - 1][1]  +  gps_delta_x(aim_point_sn); //longitude
     obj_y_cor = gps_track[ this->aim_point_sn - 1][2]  +  gps_delta_y(aim_point_sn); //latitude
     this->aim_dist_real = this->distOfPoint(&cur_x_cor, &cur_y_cor, &obj_x_cor, &obj_y_cor);//for control
}


void DecisionMake::followTrack(void)
{
    double  delta_angle_pos = 0, delta_angle_direction = 0, delta_angle = 0;
    float front_wheel_angle_temp = 0, steer_angle_temp = 0;
    /*caculate the angle to track*/
    this->aim_azimuth = gps_track[ this->aim_point_sn - 1][3]; //
    this->aim_cur_angle =  this->cacuAimCurAzimuth();
    /*caculate the delta angle between obj angle and current azimuth*/
    delta_angle_pos = delta_azimuth(&this->cur_azimuth,  &this->aim_cur_angle);//PID
    delta_angle_direction = delta_azimuth(&this->cur_azimuth,  &this->aim_azimuth);
    /*decided by the vertical distance*/
    this->verticalDist = verticalLineDist();
     if((verticalDist > 1) || (verticalDist < -1))
       //  delta_angle = delta_angle_pos;
         delta_angle = delta_angle_pos * 0.2+ delta_angle_direction * 0.8;
     else
    //     delta_angle = delta_angle_direction;
         delta_angle = delta_angle_pos * 0.6 + delta_angle_direction * 0.4;

    /*caculate the front wheel*/
    front_wheel_angle_temp = cacuFrontWheelAngle(&delta_angle, &this->aim_dist_real);
   /*caculate the steering angle */
    steer_angle_temp = front_wheel_angle_temp * parameters.coeff_wheel_steering ;

    if(flags.in_zone_whole_lane_end_flag == false)
    {
        if(fabs(delta_angle) < 1 )
            this->steering_angle = 0;
        else
            this->steering_angle = steer_angle_temp;
    }
   else
    {
            this->steering_angle = 0;
    }
}
