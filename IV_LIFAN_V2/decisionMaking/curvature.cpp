#include "includes.h"

Curvature::Curvature()
{
    ;
}

/*renew the this->aim_point_sn*/
unsigned int long Curvature::seekPoint(double dist_obj)
{
   double dist_two_point = 0;
   double obj_x_cor = 0, obj_y_cor = 0;
   double cur_x_cor = 0, cur_y_cor = 0;
   unsigned int long point_sn_temp = 0;
    /*caculate the aim distance*/

    /*search the point mathes the distance*/
      cur_x_cor = gps_track[ decisionMake.cp_sn - 1][1] + gps_delta_x(decisionMake.cp_sn); //longitude 388011.84;//
      cur_y_cor = gps_track[ decisionMake.cp_sn - 1][2] + gps_delta_y(decisionMake.cp_sn); //latitude 3123060.50;//

     point_sn_temp = decisionMake.cp_sn;//closest point

     while(dist_two_point < dist_obj) /*to get the aim point on the track*/
     {
        obj_x_cor = gps_track[ point_sn_temp - 1][1] + gps_delta_x(point_sn_temp); //longitude
        obj_y_cor = gps_track[ point_sn_temp - 1][2] + gps_delta_y(point_sn_temp); //latitude

        dist_two_point = decisionMake.distOfPoint(&cur_x_cor, &cur_y_cor, &obj_x_cor, &obj_y_cor);
        point_sn_temp++;

        if(point_sn_temp >= decisionMake.track_row) //maximun num
        {
            point_sn_temp = decisionMake.track_row;
            break;
        }
     }

     if(point_sn_temp <= 0)
         point_sn_temp = 1;
     else if(point_sn_temp >= decisionMake.track_row)
         point_sn_temp = decisionMake.track_row;

     return point_sn_temp;

     /*caculate the real distance between current and aim points*/
//     obj_x_cor = gps_track[ this->aim_point_sn - 1][1]; //longitude
 //    obj_y_cor = gps_track[ this->aim_point_sn - 1][2]; //latitude
}

/*
double p1_x=386501.654;
double p1_y=3124849.284;
double p2_x=386613.6327;
double p2_y=3124874.771;
double p3_x=386625.0663;
double p3_y=3124856.882;
*/
//double k= curvature(p1_x,p1_y,p2_x,p2_y,p3_x, p3_y);
double Curvature::cacuCurvatureSub(double p1_x,double p1_y,double p2_x,double p2_y,double p3_x,double p3_y)
{
    double K = 0;
    double S = 0;
    double L = 0;
    double a = p2_y-p1_y;
    double b = p2_x-p1_x;
    double c = p3_y-p1_y;
    double d = p3_x-p1_x;
    double e = p2_y-p3_y;
    double f = p2_x-p3_x;
     S=((p2_x-p1_x)*(p3_y-p1_y)-(p3_x-p1_x)*(p2_y-p1_y))/2;// triangle area
    L=(sqrt(pow((double) fabs(a),2.0)+pow((double) fabs(b),2.0))*(sqrt(pow((double) fabs(c),2.0)+pow((double) fabs(d),2.0)))*(sqrt(pow((double) fabs(e),2.0)+pow((double) fabs(f),2.0))));//L=A*B*C
    //L=0;
    if(L == 0)
            return 0;
    else
    {
        K=(4*S)/L;//curvature
       return K;
    }
}

/*turn right is negative, turn left is plus*/
double Curvature::cacuCurvature()
{
    double curvature_temp = 0;
    p0_sn = decisionMake.cp_sn;
    p1_sn = this->seekPoint(20);
    p2_sn = this->seekPoint(40);

    p0_x = gps_track[ p0_sn - 1][1] + gps_delta_x(p0_sn); //longitude
    p0_y = gps_track[ p0_sn - 1][2] + gps_delta_y(p0_sn); //latitude

    p1_x = gps_track[ p1_sn - 1][1] + gps_delta_x(p1_sn); //longitude
    p1_y = gps_track[ p1_sn - 1][2] + gps_delta_y(p1_sn); //latitude

    p2_x = gps_track[ p2_sn - 1][1] + gps_delta_x(p2_sn); //longitude
    p2_y = gps_track[ p2_sn - 1][2] + gps_delta_y(p2_sn); //latitude

     curvature_temp = cacuCurvatureSub(p0_x, p0_y, p1_x,  p1_y,   p2_x, p2_y);

     return curvature_temp;
}
