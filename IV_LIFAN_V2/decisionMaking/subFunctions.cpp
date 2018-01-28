#include "includes.h"

double DecisionMake::distOfPoint(double *x1, double *y1, double *x2, double *y2)
{
   return sqrt((*x2 - *x1) * (*x2 - *x1)  + (*y2 - *y1) *  (*y2 - *y1));
}

/*the unit is m*/
double DecisionMake::cacuAimObjDistance(void)
{
    double dist = 0;
    double changelane_dist = 30;
    this->curvatureVal =  curvature.cacuCurvature();

  //  if(this->curvatureVal >= 0.019) //0.0218
    {
  //      dist = 3;
    }
  //  else
/*
    if(this->changing_lane_flag == true && (obst_dist_l <= changelane_dist || obst_dist <= changelane_dist || obst_dist_r <= changelane_dist)) //less than
    {
        dist = this-> vehicle_speed * 0.35;
        if(dist <= 3)
             dist = 3;
        else if(dist >= 30)
            dist = 30;
    }*/
   // else
    {
        dist = this-> vehicle_speed * 0.25;
        if(dist <= 3)
             dist = 3;
        else if(dist >= 7)
            dist = 7;
    }
    return dist;
}


/*amimuth constructed by current and aim point */
double DecisionMake::cacuAimCurAzimuth(void)
{
    double aim_x_cor = 0, aim_y_cor = 0;
    double cur_x_cor = 0, cur_y_cor = 0;
    double result = 0, alfa = 0;

    aim_x_cor = gps_track[this->aim_point_sn - 1][1] +  gps_delta_x(aim_point_sn); //longitude 388011.84;//
    aim_y_cor = gps_track[this->aim_point_sn - 1][2] +  gps_delta_y(aim_point_sn); //latitude 3123060.50;//

    cur_x_cor = this->x_cor;
    cur_y_cor = this->y_cor;
/*
    aim_x_cor = 1;
    aim_y_cor = 1;

    cur_x_cor = 2;
    cur_y_cor = 2;
*/
    //cos(alfa) = a.b/(|a|*|b|)   a(cur, aim), b(0,1)north
    result  = (aim_y_cor - cur_y_cor) * 1 / sqrt((aim_x_cor - cur_x_cor) * (aim_x_cor - cur_x_cor) + (aim_y_cor - cur_y_cor) * (aim_y_cor - cur_y_cor) );
     alfa  = acos(result) * 180 / 3.1411592627;

    if(aim_x_cor >= cur_x_cor) //right side
    { ;}
    else
    {
        alfa = 360 - alfa;
    }
    return alfa;
}

/*amimuth constructed by current point and calibration point */
double DecisionMake::cacuCaliCurAzimuth(double cali_x_cor, double cali_y_cor)
{
  //  double cali_x_cor = 0, cali_y_cor = 0, cali_c_cor = 0;
    double cur_x_cor = 0, cur_y_cor = 0;
    double result = 0, alfa = 0;
    //for gps original data.
   //  this->CovertBLToXY(cali_latitude, cali_longitude,114, &cali_x_cor, &cali_y_cor, &cali_c_cor);

    cur_x_cor = this->x_cor;
    cur_y_cor = this->y_cor;

  //  cur_x_cor = 386212.86 ;//for test
  //  cur_y_cor = 3124060.16   + 20;//for test
/*
    cali_x_cor = 1;
    cali_y_cor = 1;

    cur_x_cor = 2;
    cur_y_cor = 2;
*/
    //cos(alfa) = a.b/(|a|*|b|)   a(cur, aim), b(0,1)north
    result  = (cali_y_cor - cur_y_cor) * 1 / sqrt((cali_x_cor - cur_x_cor) * (cali_x_cor - cur_x_cor) + (cali_y_cor - cur_y_cor) * (cali_y_cor - cur_y_cor) );
     alfa  = acos(result) * 180 / 3.1411592627;

    if(cali_x_cor >= cur_x_cor) //right side
    { ;}
    else
    {
        alfa = 360 - alfa;
    }
    return alfa;
}


/*cur_azimuth is amimuth of the vehicle in current status.
  aim_azimuth is amimuth of aim point  */
double DecisionMake::delta_azimuth(double* cur_azimuth, double* aim_azimuth)
{
    double delta_angle;
    delta_angle = *cur_azimuth - *aim_azimuth; /*>0, steering wheel turns left hand, counter-clockwise; < 0, turn right hand, clockwise*/
    if(delta_angle >180)
    {
            delta_angle = delta_angle - 360;
    }
    else if(delta_angle < -180)
    {
        delta_angle = delta_angle + 360;
    }
    else{; }//do nothing

    return delta_angle;
}

/*aim_dist, the distance between aim point and  current point*/
float DecisionMake::cacuFrontWheelAngle(double* delta_angle, double* aim_dist)
{
    float L = 2.605;
    float temp = 0, temp1 = 0;
    temp = (*delta_angle) * 3.141592627 / 180;
    temp1  = atan(2 * L * sin(temp) / (*aim_dist));
    front_wheel_angle = temp1 * 180 / 3.141592627;
    return front_wheel_angle;
}

double DecisionMake::verticalLineDist()
{
    double start_x_cor = 0, start_y_cor = 0;
    double end_x_cor = 0, end_y_cor = 0;
    double cur_x_cor = 0, cur_y_cor = 0;
    double delta_dist = 0;

    double k = 0, b = 0;   // y = kx + b;

    start_x_cor = gps_track[ this->cp_sn - 1][1] +  gps_delta_x(cp_sn); //longitude 388011.84;//
    start_y_cor = gps_track[ this->cp_sn - 1][2] +  gps_delta_y(cp_sn); //latitude 3123060.50;//

    end_x_cor = gps_track[ this->aim_point_sn - 1][1] +  gps_delta_x(aim_point_sn); //longitude 388011.84;//
    end_y_cor = gps_track[ this->aim_point_sn - 1][2] +  gps_delta_y(aim_point_sn); //latitude 3123060.50;//

    cur_x_cor = this->x_cor; //longitude 388011.84;//
    cur_y_cor = this->y_cor; //latitude 3123060.50;//

    //for test only
    /*
    start_x_cor = 2;
    start_y_cor = 0;

    end_x_cor = 2;
    end_y_cor = 2;

    cur_x_cor = 0;
    cur_y_cor = 1;*/

    if(end_x_cor == start_x_cor)
        delta_dist = fabs(cur_x_cor - start_x_cor);
    else
    {
        k = (end_y_cor - start_y_cor) / (end_x_cor - start_x_cor);
        b = start_y_cor - k * start_x_cor; //b = y1- k*x1
        delta_dist = fabs(k * cur_x_cor - cur_y_cor + b) / sqrt(k * k + 1 * 1) ;//kx-y+b = 0; (Ax0+By0+c)/sqrt(A^2+B^2)
    }

    if(this->includedAngle() < 0)  /*Judge the direction*/
        delta_dist = 0 - delta_dist; //the distance has the + or -.

    return delta_dist;
}

/*caculated the included angle for judge the vehicle in which side*/
/*>0, steering wheel turns left hand, counter-clockwise; < 0, turn right hand, clockwise*/
double DecisionMake::includedAngle()
{
        double start_x_cor = 0, start_y_cor = 0;
        double end_x_cor = 0, end_y_cor = 0;
        double cur_x_cor = 0, cur_y_cor = 0;
        double delta_angle_aim = 0; //(cloest point, aim point)
        double delta_angle_cur = 0; //(cloest point, cur point)
        double k = 0;

        start_x_cor = gps_track[ this->cp_sn - 1][1] +   gps_delta_x(cp_sn); //longitude 388011.84;//
        start_y_cor = gps_track[ this->cp_sn - 1][2] +   gps_delta_y(cp_sn);

        end_x_cor = gps_track[ this->aim_point_sn - 1][1] +   gps_delta_x(aim_point_sn);
        end_y_cor = gps_track[ this->aim_point_sn - 1][2] +   gps_delta_y(aim_point_sn);

        cur_x_cor = this->x_cor;
        cur_y_cor = this->y_cor;

/*
        start_x_cor = 2;
        start_y_cor = 0;

        end_x_cor = 2;
        end_y_cor = 2;

        cur_x_cor = 0;
        cur_y_cor = 1;
*/
        //y = kx + b
        /*caculate the aim included angle*/
        if(end_x_cor == start_x_cor)
        {
            if(end_y_cor > start_y_cor) //upward
                    delta_angle_aim = 90;
            else if(end_y_cor < start_y_cor) //downward
                   delta_angle_aim = -90;
            else
                   delta_angle_aim = 0;
        }
        else
        {
            k = (end_y_cor - start_y_cor) / (end_x_cor - start_x_cor);
            delta_angle_aim = atan(k) * 180 / 3.141592627;
        }

        /*caculate the cur included  angle*/
        if(cur_x_cor == start_x_cor)
        {
            if(cur_y_cor > start_y_cor) //upward
                    delta_angle_cur = 90;
            else if(cur_y_cor < start_y_cor) //downward
                   delta_angle_cur = -90;
            else
                   delta_angle_cur = 0;
        }
        else
        {
            k = (cur_y_cor - start_y_cor) / (cur_x_cor - start_x_cor);
            delta_angle_cur = atan(k) * 180 / 3.141592627;
        }

        /*>0, steering wheel turns left hand, counter-clockwise; < 0, turn right hand, clockwise*/
       return  delta_angle_aim - delta_angle_cur;
}

/******************************************************************************
 * 函数名称  : float OneDimInterp(...)
 *
 * 功能描述  : 计算一维数组中的值
 *
 * 全局变量  :
 *
 * 形式参数  :
 *			   float fXValue: 输入X的值
 *			   const float *pfXDim: 指向一维X的值
 *			   INT16U nColMax: 一维数组的列数
                 float* pfOneDim: 指向一维数组的Y值
 * 返回值    : 查值结果
 *
 * 备注	     : 无
 *
 %float OneDimInterp(float fXValue, const float * far pfXDim, INT16U nColMax, const float * far pfOneDim)

******************************************************************************/
float OneDimInterp(float fXValue,  float *  pfXDim, unsigned int nColMax,  float *  pfOneDim)
{
    unsigned int  i = 0, nCol = 0, nColMin = 0;
    float fInterp3 = 0;
    if(fXValue <= (*pfXDim))
        fXValue = *pfXDim;
    else if(fXValue >= *(pfXDim + nColMax - 1))
        fXValue = *(pfXDim + nColMax - 1);

    for(i = 0; i < nColMax; i++)
    {
        if(*(pfXDim+i) >= fXValue)
        {
            nCol = i;
            break;
        }
    }

    nColMin = nCol;
    if(nColMin == 0)
    {
        nColMin = 1;
    }

    fInterp3 = (fXValue - *(pfXDim + nColMin- 1)) * (*(pfOneDim+nColMin) - *(pfOneDim + nColMin - 1)) / (*(pfXDim+nColMin) - *(pfXDim + nColMin - 1))
      + *(pfOneDim + nColMin - 1);

    return fInterp3;
}






