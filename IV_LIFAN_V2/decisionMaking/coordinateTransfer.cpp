#include "includes.h"

/*cy is for latitude(zong zuo biao), cx is for longtitude(heng zuo biao )
*/
void DecisionMake::CovertBLToXY(double latitude,double longitude,double centerLongitude,double *cx,double *cy,double *cr)
{
    double PI=3.14159265358979;
    double p =206264.8062470963;     //p表示206265
    double B;  //纬度
    double L;  //经度
    double L0;  //中央经度
    double c,e2Power;
  //  a=6378137;
    c=6399593.625803977;
    e2Power=0.006739496742227;

    double  c0,c2,c4,c6,c8;
    c0=c*(1-0.75*e2Power+45*e2Power*e2Power/64-175*pow(e2Power,3)/256+11025*pow(e2Power,4)/16384);
    c2=c0-c;
    c4=c*(15*e2Power*e2Power/32-175*pow(e2Power,3)/384+3675*pow(e2Power,4)/8192);
    c6=c*(-35*pow(e2Power,3)/96+735*pow(e2Power,4)/2048);
    c8=c*315*pow(e2Power,4)/1024;

    double N,l,t_s,n2,l0;   //n2是η的平方
    double a1,a2,b1,b2,dx,dy,x,y,r; //r为该点的平面子午线收敛角
    double X;   //X是子午线弧长

    B=latitude;
    L=longitude;
    L0=centerLongitude;
    l=(L-L0)*3600.0;         //经度秒差
    B=B*PI/180.0;            //纬度纯度转换为弧度

    N=c/sqrt(1+e2Power*cos(B)*cos(B));
    X=c0*B+(c2*cos(B)+c4*pow(cos(B),3)+c6*pow(cos(B),5)+c8*pow(cos(B),7))*sin(B);

    t_s=tan(B)*tan(B);    //t的平方
    n2=e2Power*cos(B)*cos(B);
    l0=l*l*pow(10.0,-8);

    a1=N*sin(B)*cos(B)*pow(10.0,8)/(2*pow(p,2));
    a2=N*sin(B)*pow(cos(B),3)*(5-t_s+9*n2+4*n2*n2)*pow(10.0,16)/(24*pow(p,4));

    b1=N*cos(B)/p;
    b2=N*pow(cos(B),3)*(1-t_s+n2)*pow(10.0,8)/(6*pow(p,3));

    dx=N*sin(B)*pow(cos(B),5)*(61-58*t_s+t_s*t_s)*pow(l,6)/(720*pow(p,6));

    dy=N*pow(cos(B),5)*(5-18*t_s+t_s*t_s+14*n2-58*n2*t_s)*pow(l,5)/(120*pow(p,5));

    x=X+l0*(a1+a2*l0)+dx;
    y=l*(b1+b2*l0)+dy;
    *cy=x;
    *cx=y+500000;

    if(latitude==90)
        *cx=500000;
    r=sin(B)*l+sin(B)*cos(B)*cos(B)*pow(l,3)*(1+3*n2+2*n2*n2)/3/p/p+sin(B)*pow(cos(B),4)*pow(l,5)*(2-t_s)/15/p/p/p/p;
    //this->R=r/3600.0;  //r为秒值,转换为纯度
    *cr=r/3600.0;

}

