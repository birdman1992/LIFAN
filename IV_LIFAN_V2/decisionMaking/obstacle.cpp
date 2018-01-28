#include "includes.h"

/*id, type, couter(exist), times(disapear), distance*/


double obstacle_bag[OBST_ROW][OBST_COL];

void DecisionMake::checkObstacles()
{
    double obst_point_dist = 0;

    double dist_two_point = 0;
    double obj_x_cor = 0, obj_y_cor = 0, obj_azimth = 0;
    double aim_x_cor = 0, aim_y_cor = 0, aim_azimth = 0;
    double end_x_cor = 0, end_y_cor = 0, end_azimth = 0;

//    double cur_x_cor = 0, cur_y_cor = 0;
    double fst_x_cor = 0,  fst_y_cor = 0;

    unsigned int long obst_point_sn_temp = 0;
    unsigned int times = 0;
    unsigned int i = 0, j = 0;
    bool obst_flag_temp = 0;
    unsigned char find_flag = 0;
    unsigned char num = 0;
    float min_dis_temp = 0;

    unsigned int long end_point_sn;
     /*caculate the maximum distance*/
     obst_point_dist = this->aim_dist_obj + 10 + 20 + HIGH_SPEED_DIST; //13+30 +40= 83m.
     /*search the point mathes the distance*/

    //   cur_x_cor = gps_track[ this->cp_sn - 1][1] +  gps_delta_x(cp_sn); //longitude 388011.84;//
    //   cur_y_cor = gps_track[ this->cp_sn - 1][2] +  gps_delta_y(cp_sn); //latitude 3123060.50;//
      obst_point_sn_temp = this->cp_sn;//closest point
      dist_two_point = 0;

      times = 1;

//--------------------------begin----------------------------------------
      for(j = 0; j < OBST_ROW; j++) //minus every time for each obstacle.
      {
          if(obstacle_bag[j][3] >= 1) //time
          {
               obstacle_bag[j][3]--;
          }
          if(obstacle_bag[j][3] == 0)//clear if time is minused to 0
          {
              obstacle_bag[j][0] = 0;
              obstacle_bag[j][1] =0;
              obstacle_bag[j][2] = 0;
              obstacle_bag[j][3] = 0;
              obstacle_bag[j][4] = 0;
              obstacle_bag[j][5] = 0;
              obstacle_bag[j][6] = 0;
              obstacle_bag[j][7] = 0;
              obstacle_bag[j][8] = 0;
              obstacle_bag[j][9] = 0;
              obstacle_bag[j][10] = 0;
              obstacle_bag[j][11]= 0;
              obstacle_bag[j][12] = 0;
          }
      }

      num = 0; //judge the obstacle whether disappeared.
      for(j = 0; j < OBST_ROW; j++) //
      {
          if(obstacle_bag[j][3] ==  0) //time
                num++;
      }
      if(num >= 10)//all the row
      {
          this->obst_flag = false;
      }
//----------------------------end--------------------------------------
      while(dist_two_point < obst_point_dist) /*to get the obstacle points on the track*/
      {
           fst_x_cor = gps_track[obst_point_sn_temp- 1][1] +  gps_delta_x_m(obst_point_sn_temp); //longitude 388011.84;//
           fst_y_cor = gps_track[ obst_point_sn_temp- 1][2] +  gps_delta_y_m(obst_point_sn_temp); //latitude 3123060.50;//

           if(obst_point_sn_temp >= track_row - 2)  obst_point_sn_temp = track_row - 2;

           obj_x_cor = gps_track[ (obst_point_sn_temp + 1) - 1][1] +  gps_delta_x_m(obst_point_sn_temp + 1); //longitude
            obj_y_cor = gps_track[ (obst_point_sn_temp + 1) - 1][2] +  gps_delta_y_m(obst_point_sn_temp + 1); //latitude
            obj_azimth = gps_track[ (obst_point_sn_temp + 1) - 1][3];

            dist_two_point = dist_two_point + this->distOfPoint(&fst_x_cor, &fst_y_cor, &obj_x_cor, &obj_y_cor);
             if(dist_two_point  >= 4 * times) //4 is 4m, the first time is 4m*1, the second time is 4m*2
             {
                     /*use  the vehicle dynamic  coordinate(obj_x_cor, obj_y_cor) to get the four points of the rectangle*/
                    //in global coordinate
                     getVehFourCrdGlobal(obj_x_cor,  obj_y_cor,  obj_azimth,
                                                      &dyn.p1_x, &dyn.p1_y,   &dyn.p2_x, &dyn.p2_y,   &dyn.p3_x, &dyn.p3_y,   &dyn.p4_x, &dyn.p4_y);
                    /*compare with the rectangle of obstacles.*/
                     for(i = 0; i < this->obstacles_num; i++ ) //seek all obstacles.---long time!!!
                     {
                          obst_flag_temp = this->detectObstOverlay( dyn.p1_x, dyn.p1_y,   dyn.p2_x, dyn.p2_y,   dyn.p3_x, dyn.p3_y,   dyn.p4_x, dyn.p4_y,
                                     obstacles[i].p1_x, obstacles[i].p1_y, obstacles[i].p2_x, obstacles[i].p2_y, obstacles[i].p3_x, obstacles[i].p3_y, obstacles[i].p4_x, obstacles[i].p4_y);
                         //-----------------------begin-----------------------------------------
                          /*
    double p1_x;
    double p1_y;
    double p2_x;
    double p2_y;
    double p3_x;
    double p3_y;
    double p4_x;
    double p4_y;
*/
                          if(obst_flag_temp == true)
                         {
                                find_flag = false;
                                for(j = 0; j < OBST_ROW; j++)
                                {
                                            if(obstacle_bag[j][0] == obstacles[i].id)//if  id exists before
                                            {
                                                    //obstacle_bag[j][0] = obstacles[i].id;
                                                    obstacle_bag[j][1] = obstacles[i].classficiation;
                                                    obstacle_bag[j][2]++;
                                                    if(obstacle_bag[j][2] >= 200)  obstacle_bag[j][2] = 200;
                                                    obstacle_bag[j][3] = OBST_DISP_TIMES;
                                                    obstacle_bag[j][4] = obstacles[i].closest_p_dist;
                                                    obstacle_bag[j][5] = obstacles[i].p1_x;
                                                    obstacle_bag[j][6] = obstacles[i].p1_y;
                                                    obstacle_bag[j][7] = obstacles[i].p2_x;
                                                    obstacle_bag[j][8] = obstacles[i].p2_y;
                                                    obstacle_bag[j][9] = obstacles[i].p3_x;
                                                    obstacle_bag[j][10] = obstacles[i].p3_y;
                                                    obstacle_bag[j][11] = obstacles[i].p4_x;
                                                    obstacle_bag[j][12] = obstacles[i].p4_y;
//                                                    obstacle_bag[j][5] = dyn.p1_x;
//                                                    obstacle_bag[j][6] = dyn.p1_y;
//                                                    obstacle_bag[j][7] = dyn.p2_x;
//                                                    obstacle_bag[j][8] = dyn.p2_y;
//                                                    obstacle_bag[j][9] = dyn.p3_x;
//                                                    obstacle_bag[j][10] = dyn.p3_y;
//                                                    obstacle_bag[j][11] = dyn.p4_x;
//                                                    obstacle_bag[j][12] = dyn.p4_y;
                                                    find_flag = true;
                                            }
                                }

                                if(find_flag == false) //if this id not exists before
                                {
                                        for(j = 0; j < OBST_ROW; j++) //
                                        {
                                                if(obstacle_bag[j][0] == 0) //
                                                {
                                                    obstacle_bag[j][0] = obstacles[i].id;
                                                    obstacle_bag[j][1] = obstacles[i].classficiation;
                                                    obstacle_bag[j][2] = 1;
                                                    obstacle_bag[j][3] = OBST_DISP_TIMES;
                                                    obstacle_bag[j][4] = obstacles[i].closest_p_dist;
                                                    obstacle_bag[j][5] = obstacles[i].p1_x;
                                                    obstacle_bag[j][6] = obstacles[i].p1_y;
                                                    obstacle_bag[j][7] = obstacles[i].p2_x;
                                                    obstacle_bag[j][8] = obstacles[i].p2_y;
                                                    obstacle_bag[j][9] = obstacles[i].p3_x;
                                                    obstacle_bag[j][10] = obstacles[i].p3_y;
                                                    obstacle_bag[j][11] = obstacles[i].p4_x;
                                                    obstacle_bag[j][12] = obstacles[i].p4_y;
//                                                    obstacle_bag[j][5] = dyn.p1_x;
//                                                    obstacle_bag[j][6] = dyn.p1_y;
//                                                    obstacle_bag[j][7] = dyn.p2_x;
//                                                    obstacle_bag[j][8] = dyn.p2_y;
//                                                    obstacle_bag[j][9] = dyn.p3_x;
//                                                    obstacle_bag[j][10] = dyn.p3_y;
//                                                    obstacle_bag[j][11] = dyn.p4_x;
//                                                    obstacle_bag[j][12] = dyn.p4_y;
                                                    break;
                                                }
                                                else
                                                {;}
                                        }
                                }

                                min_dis_temp = 1000;

                                for(j = 0; j < OBST_ROW; j++) //judge the obstacle and get the distance.
                                {
                                     if(obstacle_bag[j][2]  >= OBST_EXIST_TIMES) //10*100ms = 1000ms
                                     {
                                         this->obst_flag = true;
                                         if(obstacle_bag[j][4] <= min_dis_temp)
                                                min_dis_temp = obstacle_bag[j][4] ;
                                     }
                                     else
                                     {
                                         ;//min_dis_temp= 0; //no obstacle.
                                      }
                                }
                                this->obst_dist = min_dis_temp; //the cloest point dist.
                         }
                          //---------------------------end-----------------------------------------------
                     }//for
                     times++;
              }//if
                obst_point_sn_temp++;
                if(obst_point_sn_temp >= this->track_row - 1) //maximun num
                {
                    obst_point_sn_temp = this->track_row - 1;
                    break;
                }
            }//while


       if(obst_point_sn_temp <= 0)
           obst_point_sn_temp = 1;
       else if(obst_point_sn_temp >= track_row - 1)
           obst_point_sn_temp = track_row - 1;

        end_point_sn = obst_point_sn_temp;
        //---------------------------------------------------------------------
       //in global coordinate
        getVehFourCrdGlobal(x_cor,  y_cor,  cur_azimuth,
                                              &cur.p1_x, &cur.p1_y,   &cur.p2_x, &cur.p2_y,   &cur.p3_x, &cur.p3_y,   &cur.p4_x, &cur.p4_y);

        aim_x_cor = gps_track[ aim_point_sn - 1][1] +  gps_delta_x_m(aim_point_sn); //longitude
        aim_y_cor = gps_track[ aim_point_sn - 1][2] +  gps_delta_y_m(aim_point_sn); //latitude
        aim_azimth = gps_track[ aim_point_sn - 1][3];
        getVehFourCrdGlobal(aim_x_cor,  aim_y_cor,  aim_azimth,
                                              &aim.p1_x, &aim.p1_y,   &aim.p2_x, &aim.p2_y,   &aim.p3_x, &aim.p3_y,   &aim.p4_x, &aim.p4_y);

         end_x_cor = gps_track[ end_point_sn - 1][1] +  gps_delta_x_m(end_point_sn); //longitude
         end_y_cor = gps_track[ end_point_sn - 1][2] +  gps_delta_y_m(end_point_sn); //latitude
         end_azimth = gps_track[ end_point_sn - 1][3];
         getVehFourCrdGlobal(end_x_cor,  end_y_cor,  end_azimth,
                                              &end.p1_x, &end.p1_y,   &end.p2_x, &end.p2_y,   &end.p3_x, &end.p3_y,   &end.p4_x, &end.p4_y);
}

/*get four points coordinate from vehicle x, y and azimuth in global coordinate*/
void DecisionMake::getVehFourCrdGlobal(double cor_x, double cor_y, double azimuth,
                                      double *s_p1_x, double *s_p1_y, double *s_p2_x, double *s_p2_y, double *s_p3_x, double *s_p3_y, double *s_p4_x, double *s_p4_y )
{
        double alfa = 0;
        double     len = 6, wid = 1.8; /*the vehicle's length and width*/
        double center_x = 0, center_y = 0;
        double p1_x = 0, p1_y = 0, p2_x = 0, p2_y = 0, p3_x = 0, p3_y = 0, p4_x = 0, p4_y = 0;
      //  double s_p1_x = 0, s_p1_y = 0, s_p2_x = 0, s_p2_y = 0, s_p3_x = 0, s_p3_y = 0, s_p4_x = 0, s_p4_y = 0;

        if(azimuth <= 0)
            azimuth = 0;
        else if(azimuth >= 360)
            azimuth = 360;
        else
        {;}

        alfa = -azimuth * 3.141592627 / 180;
        center_x = cor_x;
        center_y = cor_y;
        /*coordinates without azimuth*/
        p1_x = center_x - wid/2;
        p1_y = center_y + len/2;

        p2_x = center_x + wid/2;
        p2_y = center_y + len/2;

        p3_x = center_x + wid/2;
        p3_y = center_y - len/2;

        p4_x = center_x - wid/2;
        p4_y = center_y - len/2;

        /*coordinates with azimuth*/
        *s_p1_x = (p1_x - center_x) * cos(alfa) - (p1_y - center_y) * sin(alfa) + center_x;
        *s_p1_y = (p1_x - center_x) * sin(alfa) + (p1_y - center_y) * cos(alfa) + center_y;

        *s_p2_x = (p2_x - center_x) * cos(alfa) - (p2_y - center_y) * sin(alfa) + center_x;
        *s_p2_y = (p2_x - center_x) * sin(alfa) + (p2_y - center_y) * cos(alfa) + center_y;

        *s_p3_x = (p3_x - center_x) * cos(alfa) - (p3_y - center_y) * sin(alfa) + center_x;
        *s_p3_y = (p3_x - center_x) * sin(alfa) + (p3_y - center_y) * cos(alfa) + center_y;

        *s_p4_x = (p4_x - center_x) * cos(alfa) - (p4_y - center_y) * sin(alfa) + center_x;
        *s_p4_y = (p4_x - center_x) * sin(alfa) + (p4_y - center_y) * cos(alfa) + center_y;
}

