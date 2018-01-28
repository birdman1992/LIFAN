#include "includes.h"

using namespace std;
extern std::vector<ObjectEcuEt> objectEcu;//雷达目标数据

struct Rectangle //左上为第一点，顺时针增大
{
    float x1;
    float x2;
    float x3;
    float x4;
    float y1;
    float y2;
    float y3;
    float y4;
    float closest_p_dist; // distance between cloest point and (center point of vehicle) in vehicle coordiance.
    int Obj_id;
    int Obj_class;
};

vector<Rectangle> vecDetect; //储存所有矩形
vector<ObjectEcuEt> ibeoobject;

Ibeo4::Ibeo4()
{
    ;
}

/*get the four points of obstacle in vehicle coordinate*/
void  Ibeo4:: getOriFourPointObst()
{
 //   if(objectEcu.size == 0)
         //flag = 1;  interrupt
    float min_dist = 0;
    unsigned j = 0;
     ibeoobject.assign(objectEcu.begin(), objectEcu.end());
 //    objectEcu.clear();

    vecDetect.clear();

    for(unsigned int i = 0; i < ibeoobject.size(); i++)//绘制目标数据框
    {
        if((ibeoobject[i].getObjBoxSize().getX()>0.1) || (ibeoobject[i].getObjBoxSize().getY()>0.1))
        {
        Rectangle Rec2;
        float ori;
        ori=ibeoobject[i].getObjBoxOrientation();
        float a_x,a_y,b_x,b_y,c_x,c_y,d_x,d_y;
        //障碍物后面点1
        a_x=ibeoobject[i].getObjBoxCenter().getX()-ibeoobject[i].getObjBoxSize().getX()*cos(ori)*0.5+ibeoobject[i].getObjBoxSize().getY()*sin(ori)*0.5;
        a_y=ibeoobject[i].getObjBoxCenter().getY()-ibeoobject[i].getObjBoxSize().getX()*sin(ori)*0.5-ibeoobject[i].getObjBoxSize().getY()*cos(ori)*0.5;
        //障碍物后面点2
        b_x=ibeoobject[i].getObjBoxCenter().getX()-ibeoobject[i].getObjBoxSize().getX()*cos(ori)*0.5-ibeoobject[i].getObjBoxSize().getY()*sin(ori)*0.5;
        b_y=ibeoobject[i].getObjBoxCenter().getY()-ibeoobject[i].getObjBoxSize().getX()*sin(ori)*0.5+ibeoobject[i].getObjBoxSize().getY()*cos(ori)*0.5;
        //障碍物前面点1
        c_x=ibeoobject[i].getObjBoxCenter().getX()+ibeoobject[i].getObjBoxSize().getX()*cos(ori)*0.5-ibeoobject[i].getObjBoxSize().getY()*sin(ori)*0.5;
        c_y=ibeoobject[i].getObjBoxCenter().getY()+ibeoobject[i].getObjBoxSize().getX()*sin(ori)*0.5+ibeoobject[i].getObjBoxSize().getY()*cos(ori)*0.5;
        //障碍物前面点2
        d_x=ibeoobject[i].getObjBoxCenter().getX()+ibeoobject[i].getObjBoxSize().getX()*cos(ori)*0.5+ibeoobject[i].getObjBoxSize().getY()*sin(ori)*0.5;
        d_y=ibeoobject[i].getObjBoxCenter().getY()+ibeoobject[i].getObjBoxSize().getX()*sin(ori)*0.5-ibeoobject[i].getObjBoxSize().getY()*cos(ori)*0.5;

        min_dist = a_x;
        if(b_x < min_dist)   min_dist = b_x;
        if(c_x < min_dist)   min_dist = c_x;
        if(d_x < min_dist)   min_dist = d_x;

        Rec2.x1=-c_y;
        Rec2.x2=-d_y;
        Rec2.x3=-a_y;
        Rec2.x4=-b_y;
        Rec2.y1=c_x;
        Rec2.y2=d_x;
        Rec2.y3=a_x;
        Rec2.y4=b_x;
        Rec2.closest_p_dist = min_dist;
        Rec2.Obj_id=ibeoobject[i].getObjectId();
        Rec2.Obj_class=ibeoobject[i].getClassification();
        vecDetect.push_back(Rec2);
        }
    }
 }

/*azimuth is the vehicle current azimuth*/
/*rotate the coordinates  of obstale from vehicle into global, for IBEO4 obstacles*/
void Ibeo4::rotateCoordinates(double *azimuth, float *ori_p1_x, float *ori_p1_y, float *ori_p2_x, float *ori_p2_y, float *ori_p3_x, float *ori_p3_y, float *ori_p4_x, float *ori_p4_y,
                                      double *obj_p1_x, double *obj_p1_y, double *obj_p2_x, double *obj_p2_y, double *obj_p3_x, double *obj_p3_y, double *obj_p4_x, double *obj_p4_y )
{
    //*azimuth = 10;
    double beta = 0;
    double temp_p1_x = 0, temp_p1_y = 0, temp_p2_x = 0, temp_p2_y = 0, temp_p3_x = 0, temp_p3_y = 0, temp_p4_x = 0, temp_p4_y = 0;
    beta = -(*azimuth) * 3.141592627 / 180;  //%counter clockwise
    temp_p1_x =  *ori_p1_x * cos(beta) - *ori_p1_y * sin(beta);
    temp_p1_y =  *ori_p1_x * sin(beta) + *ori_p1_y * cos(beta);

    temp_p2_x =  *ori_p2_x * cos(beta) - *ori_p2_y * sin(beta);
    temp_p2_y =  *ori_p2_x * sin(beta) + *ori_p2_y * cos(beta);

    temp_p3_x =  *ori_p3_x * cos(beta) - *ori_p3_y * sin(beta);
    temp_p3_y = * ori_p3_x * sin(beta) + *ori_p3_y * cos(beta);

    temp_p4_x =  *ori_p4_x * cos(beta) - *ori_p4_y * sin(beta);
    temp_p4_y =  *ori_p4_x * sin(beta) + *ori_p4_y * cos(beta);

    *obj_p1_x = temp_p1_x + decisionMake.x_cor;
    *obj_p1_y = temp_p1_y +  decisionMake.y_cor;

    *obj_p2_x = temp_p2_x + decisionMake.x_cor;
    *obj_p2_y = temp_p2_y +  decisionMake.y_cor;

    *obj_p3_x = temp_p3_x + decisionMake.x_cor;
    *obj_p3_y = temp_p3_y + decisionMake.y_cor;

    *obj_p4_x = temp_p4_x + decisionMake.x_cor;
    *obj_p4_y = temp_p4_y +  decisionMake.y_cor;
}



void Ibeo4::transCoordianteVehToGlobal()
{
     struct PointCoordinate global_point;
    double azimuth = 0;

    decisionMake.obstacles_num = vecDetect.size();
    azimuth = decisionMake.cur_azimuth;
    liar4_data_confirm = false;
    obstacles.clear();
 //   obst_color.clear();
    for(unsigned int i = 0; i < decisionMake.obstacles_num; i++) //turn the vehicle coordinate into global coordinate.
    {
       this->rotateCoordinates(&azimuth, &vecDetect[i].x1, &vecDetect[i].y1, &vecDetect[i].x2, &vecDetect[i].y2,
                                                                &vecDetect[i].x3, &vecDetect[i].y3,  &vecDetect[i].x4, &vecDetect[i].y4,
                                                                 &global_point.p1_x, &global_point.p1_y, &global_point.p2_x, &global_point.p2_y,
                                                                &global_point.p3_x, &global_point.p3_y,  &global_point.p4_x, &global_point.p4_y);
       global_point.id=vecDetect[i].Obj_id;
       global_point.classficiation=vecDetect[i].Obj_class;
       global_point.closest_p_dist = vecDetect[i].closest_p_dist;

       obstacles.push_back(global_point);
//       obst_color.push_back(0);
    }
    liar4_data_confirm = true;
}


void Ibeo4::getObstGlobalCoord()
{
     this->getOriFourPointObst();
     this->transCoordianteVehToGlobal();
}
