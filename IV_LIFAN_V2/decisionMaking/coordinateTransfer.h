#ifndef COORDINATETRANSFER_H
#define COORDINATETRANSFER_H

class CCovert
{
public:
    CCovert();
    virtual ~CCovert();
public:
    double B;  //纬度
    double L;  //经度
    double L0;  //中央经度
    double X;  //平面坐标X
    double Y;  //平面坐标Y
    double R;  //子午线收敛角
public:
    void CovertBLToXY(int coor);
    void CovertBLToXY(double latitude,double longitude,double centerLongitude,int coor);

//	void CovertXYToBL(double x,double y,int coor);


};

#endif // COORDINATETRANSFER_H
