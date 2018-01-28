#include "includes.h"

using namespace std;

int linedetect(double &x1,double &y1,double &x2,double &y2,double &x3,double &y3,double &x4,double &y4,vector<std::pair<double, double> >& Line);
bool judge(vector<std::pair<double, double> >& Line,double &x1,double &y1,double &x2,double &y2,double &x3,double &y3,double &x4,double &y4,double &obj_x1,double &obj_y1,double &obj_x2,double &obj_y2,double &obj_x3,double &obj_y3,double &obj_x4,double &obj_y4);
bool judge1(double &x1,double &y1,double &x2,double &y2,double &x3,double &y3,double &x4,double &y4,double &obj_x1,double &obj_y1,double &obj_x2,double &obj_y2,double &obj_x3,double &obj_y3,double &obj_x4,double &obj_y4);
bool detectObstOverlay(double &x1,double &y1,double &x2,double &y2,double &x3,double &y3,double &x4,double &y4,double &obj_x1,double &obj_y1,double &obj_x2,double &obj_y2,double &obj_x3,double &obj_y3,double &obj_x4,double &obj_y4);

/*
    double aim_x1=-3 ,aim_y1=5;
    double aim_x2=-2,aim_y2=6;
    double aim_x3=2 ,aim_y3=2;
    double aim_x4=1,aim_y4=1;
    double orb_x1=1,orb_y1=0.5;
    double orb_x2=5,orb_y2=0.5;
    double orb_x3=5,orb_y3=-2;
    double orb_x4=1,orb_y4=-2;
    int flag_Orb=obstacleDetect(aim_x1,aim_y1,aim_x2,aim_y2,aim_x3,aim_y3,aim_x4,aim_y4,orb_x1,orb_y1,orb_x2,orb_y2,orb_x3,orb_y3,orb_x4,orb_y4);
*/
std::vector<std::pair<double, double> > Line;//储存直线的A和B

bool DecisionMake::detectObstOverlay(double &aim_x1,double &aim_y1,double &aim_x2,double &aim_y2,double &aim_x3,double &aim_y3,double &aim_x4,double &aim_y4,double &orb_x1,double &orb_y1,double &orb_x2,double &orb_y2,double &orb_x3,double &orb_y3,double &orb_x4,double &orb_y4)
{
    double x1,x2,x3,x4,y1,y2,y3,y4;
    double obj_x1,obj_x2,obj_x3,obj_x4,obj_y1,obj_y2,obj_y3,obj_y4;

    Line.clear();

    x1=aim_x1;y1=aim_y1;
    x2=aim_x2;y2=aim_y2;
    x3=aim_x3;y3=aim_y3;
    x4=aim_x4;y4=aim_y4;
    obj_x1=orb_x1;obj_y1=orb_y1;
    obj_x2=orb_x2;obj_y2=orb_y2;
    obj_x3=orb_x3;obj_y3=orb_y3;
    obj_x4=orb_x4;obj_y4=orb_y4;
    bool m = false;
    if((fabs(x1-x4))>0.001)
    {
        linedetect( x1,y1,x2,y2,x3,y3,x4,y4,Line);
        m=  judge( Line, x1,y1,x2,y2,x3,y3,x4,y4,obj_x1,obj_y1,obj_x2,obj_y2,obj_x3,obj_y3,obj_x4,obj_y4);
     //   cout<<"m="<<m<<endl;//m=1表明是前方障碍物，需要换道
        return m;
    }
    else
    {
        m=judge1(x1,y1,x2,y2,x3,y3,x4,y4,obj_x1,obj_y1,obj_x2,obj_y2,obj_x3,obj_y3,obj_x4,obj_y4);
     //    cout<<"M="<<m<<endl;//m=1表明是前方障碍物，需要换道
         return m;
    }
    return m;
}
//求出四条直线的值
int linedetect(double &x1,double &y1,double &x2,double &y2,double &x3,double &y3,double &x4,double &y4,vector<std::pair<double, double> >& Line)
{
    float A,C;
     A=-1*(y1-y2)/(x1-x2);
    C=(y1-y2)/(x1-x2)*x1-y1;
    Line.push_back(std::pair<double, double>(A,C));
    A=-1*(y2-y3)/(x2-x3);
    C=(y2-y3)/(x2-x3)*x2-y2;
    Line.push_back(std::pair<double, double>(A,C));
    A=-1*(y3-y4)/(x3-x4);
    C=(y3-y4)/(x3-x4)*x3-y3;
    Line.push_back(std::pair<double, double>(A,C));
    A=-1*(y4-y1)/(x4-x1);
    C=(y4-y1)/(x4-x1)*x4-y4;
    Line.push_back(std::pair<double, double>(A,C));
return 0;
}

bool judge(vector<std::pair<double, double> >& Line,double &x1,double &y1,double &x2,double &y2,double &x3,double &y3,double &x4,double &y4,double &obj_x1,double &obj_y1,double &obj_x2,double &obj_y2,double &obj_x3,double &obj_y3,double &obj_x4,double &obj_y4)
//--------------------------new----------------------------
{
    float Width=sqrt(pow((x1-x2),2)+pow((y1-y2),2));
    float Long=sqrt(pow((x2-x3),2)+pow((y2-y3),2));
  //  cout<<"Width="<<Width<<endl;
  //  cout<<"Long="<<Long<<endl;
    double  denominator=sqrt(pow(Line[0].first,2)+1);//分母
    double numerator=fabs(Line[0].first*obj_x1+obj_y1+Line[0].second);//分子
    double d=numerator/denominator;
    double add_x,add_y,newX_1,newX_2,newX_3,newX_4,newY_1,newY_2,newY_3,newY_4;
    add_x=(obj_x2-obj_x1)/5;
    add_y=(obj_y2-obj_y1)/5;
    newX_1=obj_x1+add_x;
    newX_2=newX_1+add_x;
    newX_3=newX_2+add_x;
    newX_4=newX_3+add_x;
    newY_1=obj_y1+add_y;
    newY_2=newY_1+add_y;
    newY_3=newY_2+add_y;
    newY_4=newY_3+add_y;
    //cout<<"d1="<<d<<endl;
    bool   bool_1=0,bool_2=0,bool_3=0,bool_4=0;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    denominator=sqrt(pow(Line[1].first,2)+1);//分母
    numerator=fabs(Line[1].first*obj_x1+obj_y1+Line[1].second);//分子
    d=numerator/denominator;
   // cout<<"d2="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[2].first*obj_x1+obj_y1+Line[2].second);//分子
    d=numerator/denominator;
    //cout<<"d3="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    denominator=sqrt(pow(Line[3].first,2)+1);//分母
    numerator=fabs(Line[3].first*obj_x1+obj_y1+Line[3].second);//分子
    d=numerator/denominator;
    //cout<<"d4="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }

    //第二点判断
        bool_1=0, bool_2=0, bool_3=0,bool_4=0;
    denominator=sqrt(pow(Line[0].first,2)+1);//分母
    numerator=fabs(Line[0].first*obj_x2+obj_y2+Line[0].second);//分子
    d=numerator/denominator;
    // cout<<"d5="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    denominator=sqrt(pow(Line[1].first,2)+1);//分母
    numerator=fabs(Line[1].first*obj_x2+obj_y2+Line[1].second);//分子
    d=numerator/denominator;
    //cout<<"d6="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[2].first*obj_x2+obj_y2+Line[2].second);//分子
    d=numerator/denominator;
    //cout<<"d7="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    denominator=sqrt(pow(Line[3].first,2)+1);//分母
    numerator=fabs(Line[3].first*obj_x2+obj_y2+Line[3].second);//分子
    d=numerator/denominator;
    //cout<<"d8="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }
    //第三点
    bool_1=0, bool_2=0, bool_3=0,bool_4=0;
    denominator=sqrt(pow(Line[0].first,2)+1);//分母
    numerator=fabs(Line[0].first*obj_x3+obj_y3+Line[0].second);//分子
    d=numerator/denominator;
    //cout<<"d9="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    denominator=sqrt(pow(Line[1].first,2)+1);//分母
    numerator=fabs(Line[1].first*obj_x3+obj_y3+Line[1].second);//分子
    d=numerator/denominator;
    //cout<<"d10="<<d<<endl;
    //cout<<d-Width<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[2].first*obj_x3+obj_y3+Line[2].second);//分子
    d=numerator/denominator;
    //cout<<"d11="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=true;
    }
    denominator=sqrt(pow(Line[3].first,2)+1);//分母
    numerator=fabs(Line[3].first*obj_x3+obj_y3+Line[3].second);//分子
    d=numerator/denominator;
    //cout<<"d12="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return true;
    }
    //第四点
    bool_1=0, bool_2=0, bool_3=0,bool_4=0;
    denominator=sqrt(pow(Line[0].first,2)+1);//分母
    numerator=fabs(Line[0].first*obj_x4+obj_y4+Line[0].second);//分子
    d=numerator/denominator;
    //cout<<"d13="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    denominator=sqrt(pow(Line[1].first,2)+1);//分母
    numerator=fabs(Line[1].first*obj_x4+obj_y4+Line[1].second);//分子
    d=numerator/denominator;
       //      cout<<"d14="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[2].first*obj_x4+obj_y4+Line[2].second);//分子
    d=numerator/denominator;
    //cout<<"d15="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    denominator=sqrt(pow(Line[3].first,2)+1);//分母
    numerator=fabs(Line[3].first*obj_x4+obj_y4+Line[3].second);//分子
    d=numerator/denominator;
    //cout<<"d16="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }
    //第五点
    bool_1=0, bool_2=0, bool_3=0,bool_4=0;
    denominator=sqrt(pow(Line[0].first,2)+1);//分母
    numerator=fabs(Line[0].first*newX_1+newY_1+Line[0].second);//分子
    d=numerator/denominator;
    //cout<<"d13="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    denominator=sqrt(pow(Line[1].first,2)+1);//分母
    numerator=fabs(Line[1].first*newX_1+newY_1+Line[1].second);//分子
    d=numerator/denominator;
       //      cout<<"d14="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[2].first*newX_1+newY_1+Line[2].second);//分子
    d=numerator/denominator;
    //cout<<"d15="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[3].first*newX_1+newY_1+Line[3].second);//分子
    d=numerator/denominator;
    //cout<<"d15="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }
    //第六点
    bool_1=0, bool_2=0, bool_3=0,bool_4=0;
    denominator=sqrt(pow(Line[0].first,2)+1);//分母
    numerator=fabs(Line[0].first*newX_2+newY_2+Line[0].second);//分子
    d=numerator/denominator;
    //cout<<"d13="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    denominator=sqrt(pow(Line[1].first,2)+1);//分母
    numerator=fabs(Line[1].first*newX_2+newY_2+Line[1].second);//分子
    d=numerator/denominator;
       //      cout<<"d14="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[2].first*newX_2+newY_2+Line[2].second);//分子
    d=numerator/denominator;
    //cout<<"d15="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[3].first*newX_2+newY_2+Line[3].second);//分子
    d=numerator/denominator;
    //cout<<"d15="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }

    //第七点
    bool_1=0, bool_2=0, bool_3=0,bool_4=0;
    denominator=sqrt(pow(Line[0].first,2)+1);//分母
    numerator=fabs(Line[0].first*newX_3+newY_3+Line[0].second);//分子
    d=numerator/denominator;
    //cout<<"d13="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    denominator=sqrt(pow(Line[1].first,2)+1);//分母
    numerator=fabs(Line[1].first*newX_3+newY_3+Line[1].second);//分子
    d=numerator/denominator;
       //      cout<<"d14="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[2].first*newX_3+newY_3+Line[2].second);//分子
    d=numerator/denominator;
    //cout<<"d15="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[3].first*newX_3+newY_3+Line[3].second);//分子
    d=numerator/denominator;
    //cout<<"d15="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }

    //第八点
    bool_1=0, bool_2=0, bool_3=0,bool_4=0;
    denominator=sqrt(pow(Line[0].first,2)+1);//分母
    numerator=fabs(Line[0].first*newX_4+newY_4+Line[0].second);//分子
    d=numerator/denominator;
    //cout<<"d13="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    denominator=sqrt(pow(Line[1].first,2)+1);//分母
    numerator=fabs(Line[1].first*newX_4+newY_4+Line[1].second);//分子
    d=numerator/denominator;
       //      cout<<"d14="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[2].first*newX_4+newY_4+Line[2].second);//分子
    d=numerator/denominator;
    //cout<<"d15="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[3].first*newX_4+newY_4+Line[3].second);//分子
    d=numerator/denominator;
    //cout<<"d15="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }

    return 0;
}
//------------------------before-----------------------

/*{
    float Width=sqrt(pow((x1-x2),2)+pow((y1-y2),2));
    float Long=sqrt(pow((x2-x3),2)+pow((y2-y3),2));
    x4 = x4;
  //  cout<<"Width="<<Width<<endl;
  //  cout<<"Long="<<Long<<endl;
    double  denominator=sqrt(pow(Line[0].first,2)+1);//分母
    double numerator=fabs(Line[0].first*obj_x1+obj_y1+Line[0].second);//分子
    double d=numerator/denominator;
   // cout<<"d1="<<d<<endl;
    bool   bool_1=0,bool_2=0,bool_3=0,bool_4=0;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    denominator=sqrt(pow(Line[1].first,2)+1);//分母
    numerator=fabs(Line[1].first*obj_x1+obj_y1+Line[1].second);//分子
    d=numerator/denominator;
  //  cout<<"d2="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[2].first*obj_x1+obj_y1+Line[2].second);//分子
    d=numerator/denominator;
   // cout<<"d3="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    denominator=sqrt(pow(Line[3].first,2)+1);//分母
    numerator=fabs(Line[3].first*obj_x1+obj_y1+Line[3].second);//分子
    d=numerator/denominator;
 //   cout<<"d4="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }

    //第二点判断
    bool_1=0,bool_2=0,bool_3=0,bool_4=0;
    denominator=sqrt(pow(Line[0].first,2)+1);//分母
    numerator=fabs(Line[0].first*obj_x2+obj_y2+Line[0].second);//分子
    d=numerator/denominator;
    // cout<<"d5="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    denominator=sqrt(pow(Line[1].first,2)+1);//分母
    numerator=fabs(Line[1].first*obj_x2+obj_y2+Line[1].second);//分子
    d=numerator/denominator;
  //  cout<<"d6="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[2].first*obj_x2+obj_y2+Line[2].second);//分子
    d=numerator/denominator;
 //   cout<<"d7="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    denominator=sqrt(pow(Line[3].first,2)+1);//分母
    numerator=fabs(Line[3].first*obj_x2+obj_y2+Line[3].second);//分子
    d=numerator/denominator;
 //   cout<<"d8="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }
    //第三点
    bool_1=0,bool_2=0,bool_3=0,bool_4=0;
    denominator=sqrt(pow(Line[0].first,2)+1);//分母
    numerator=fabs(Line[0].first*obj_x3+obj_y3+Line[0].second);//分子
    d=numerator/denominator;
 //   cout<<"d9="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    denominator=sqrt(pow(Line[1].first,2)+1);//分母
    numerator=fabs(Line[1].first*obj_x3+obj_y3+Line[1].second);//分子
    d=numerator/denominator;
 //   cout<<"d10="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[2].first*obj_x3+obj_y3+Line[2].second);//分子
    d=numerator/denominator;
  //  cout<<"d11="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    denominator=sqrt(pow(Line[3].first,2)+1);//分母
    numerator=fabs(Line[3].first*obj_x3+obj_y3+Line[3].second);//分子
    d=numerator/denominator;
 //   cout<<"d12="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }
    //第四点
    bool_1=0,bool_2=0,bool_3=0,bool_4=0;
    denominator=sqrt(pow(Line[0].first,2)+1);//分母
    numerator=fabs(Line[0].first*obj_x4+obj_y4+Line[0].second);//分子
    d=numerator/denominator;
  //  cout<<"d13="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    denominator=sqrt(pow(Line[1].first,2)+1);//分母
    numerator=fabs(Line[1].first*obj_x3+obj_y3+Line[1].second);//分子
    d=numerator/denominator;
    //         cout<<"d14="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    denominator=sqrt(pow(Line[2].first,2)+1);//分母
    numerator=fabs(Line[2].first*obj_x3+obj_y3+Line[2].second);//分子
    d=numerator/denominator;
 //   cout<<"d15="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    denominator=sqrt(pow(Line[3].first,2)+1);//分母
    numerator=fabs(Line[3].first*obj_x3+obj_y3+Line[3].second);//分子
    d=numerator/denominator;
//    cout<<"d16="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }

    return 0;
}*/

bool judge1(double &x1,double &y1,double &x2,double &y2,double &x3,double &y3,double &x4,double &y4,double &obj_x1,double &obj_y1,double &obj_x2,double &obj_y2,double &obj_x3,double &obj_y3,double &obj_x4,double &obj_y4)
//------------------------new---------------------------
{
    float Width=fabs(x1-x2);
    float Long=fabs(y2-y3);
//    cout<<"Width="<<Width<<endl;
//    cout<<"Long="<<Long<<endl;
    double add_x,add_y,newX_1,newX_2,newX_3,newX_4,newY_1,newY_2,newY_3,newY_4;
    add_x=(obj_x2-obj_x1)/5;
    add_y=(obj_y2-obj_y1)/5;
    newX_1=obj_x1+add_x;
    newX_2=newX_1+add_x;
    newX_3=newX_2+add_x;
    newX_4=newX_3+add_x;
    newY_1=obj_y1+add_y;
    newY_2=newY_1+add_y;
    newY_3=newY_2+add_y;
    newY_4=newY_3+add_y;
    bool   bool_1=0,bool_2=0,bool_3=0,bool_4=0;
    //第一点
    double d=fabs(obj_y1-y1);
//    cout<<"1d0="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    d=fabs(obj_x1-x2);
 //   cout<<"1d1="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    d=fabs(obj_y1-y3);
//    cout<<"1d2="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    d=fabs(obj_x1-x4);
 //   cout<<"1d3="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }

    //第二点检测
    bool_1=0, bool_2=0, bool_3=0,bool_4=0;
    d=fabs(obj_y2-y1);
//    cout<<"2d0="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    d=fabs(obj_x2-x2);
//    cout<<"2d1="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    d=fabs(obj_y2-y3);
   // cout<<"2d2="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    d=fabs(obj_x2-x4);
 //   cout<<"2d3="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }
    //第三点
    bool_1=0, bool_2=0, bool_3=0,bool_4=0;
    d=fabs(obj_y3-y1);
  //  cout<<"3d0="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    d=fabs(obj_x3-x2);
   // cout<<"3d1="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    d=fabs(obj_y3-y3);
   // cout<<"3d2="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    d=fabs(obj_x3-x4);
   // cout<<"3d3="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }
    //第四点
    bool_1=0, bool_2=0, bool_3=0,bool_4=0;
    d=fabs(obj_y4-y1);
   // cout<<"4d0="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    d=fabs(obj_x4-x2);
    //cout<<"4d1="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    d=fabs(obj_y4-y3);
   // cout<<"4d2="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    d=fabs(obj_x4-x4);
   // cout<<"4d3="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }
    //第五点
    bool_1=0, bool_2=0, bool_3=0,bool_4=0;
    d=fabs(newY_1-y1);
    //cout<<"5d0="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    d=fabs(newX_1-x2);
    //cout<<"5d1="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    d=fabs(newY_1-y3);
  //  cout<<"5d2="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    d=fabs(newX_1-x4);
 //   cout<<"5d3="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }

    //第六点
    bool_1=0, bool_2=0, bool_3=0,bool_4=0;
    d=fabs(newY_2-y1);
//    cout<<"6d0="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    d=fabs(newX_2-x2);
 //   cout<<"6d1="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    d=fabs(newY_2-y3);
 //   cout<<"6d2="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    d=fabs(newX_2-x4);
 //   cout<<"6d3="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }
    //第七点
    bool_1=0, bool_2=0, bool_3=0,bool_4=0;
    d=fabs(newY_3-y1);
//    cout<<"7d0="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    d=fabs(newX_3-x2);
//    cout<<"7d1="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    d=fabs(newY_3-y3);
//    cout<<"7d2="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    d=fabs(newX_3-x4);
  //  cout<<"7d3="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }
    //第八点
    bool_1=0, bool_2=0, bool_3=0,bool_4=0;
    d=fabs(newY_4-y1);
//    cout<<"8d0="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    d=fabs(newX_4-x2);
//    cout<<"8d1="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    d=fabs(newY_4-y3);
////    cout<<"8d2="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    d=fabs(newX_4-x4);
//    cout<<"8d3="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }
    return 0;
}
//-------------------before--------------------
/*
{
    float Width=fabs(x1-x2);
    float Long=fabs(y2-y3);
 //   cout<<"Width="<<Width<<endl;
 //   cout<<"Long="<<Long<<endl;
    bool   bool_1=0,bool_2=0,bool_3=0,bool_4=0;
    //第一点
    double d=fabs(obj_y1-y1);
 //   cout<<"1d0="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    d=fabs(obj_x1-x2);
//    cout<<"1d1="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    d=fabs(obj_y1-y3);
 //   cout<<"1d2="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    d=fabs(obj_x1-x4);
  //  cout<<"1d3="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }

    //第二点检测
    bool_1=0,bool_2=0,bool_3=0,bool_4=0;
    d=fabs(obj_y2-y1);
 //   cout<<"2d0="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    d=fabs(obj_x2-x2);
//    cout<<"2d1="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    d=fabs(obj_y2-y3);
//    cout<<"2d2="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    d=fabs(obj_x2-x4);
//    cout<<"2d3="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }
    //第三点
    bool_1=0,bool_2=0,bool_3=0,bool_4=0;
    d=fabs(obj_y3-y1);
//    cout<<"3d0="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    d=fabs(obj_x3-x2);
//    cout<<"3d1="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    d=fabs(obj_y3-y3);
//    cout<<"3d2="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    d=fabs(obj_x3-x4);
//    cout<<"3d3="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }
    //第四点
    bool_1=0,bool_2=0,bool_3=0,bool_4=0;
    d=fabs(obj_y4-y1);
//    cout<<"4d0="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_1=1;
    }
    d=fabs(obj_x4-x2);
//    cout<<"4d1="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_2=1;
    }
    d=fabs(obj_y4-y3);
//    cout<<"4d2="<<d<<endl;
    if(d<Long)//距离阈值
    {
        bool_3=1;
    }
    d=fabs(obj_x4-x4);
//    cout<<"4d3="<<d<<endl;
    if(d<Width)//距离阈值
    {
        bool_4=1;
    }
    if(bool_1&&bool_2&&bool_3&&bool_4)
    {
        return 1;
    }

    return 0;
}*/
