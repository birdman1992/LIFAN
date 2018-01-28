#ifndef CURVATURE_H
#define CURVATURE_H

class Curvature
{
public:
    Curvature();
    double cacuCurvatureSub(double p1_x,double p1_y,double p2_x,double p2_y,double p3_x,double p3_y);
    double cacuCurvature();
    unsigned int long  seekPoint(double dist_obj);

public:
    unsigned int long p0_sn;
    unsigned int long p1_sn;
    unsigned int long p2_sn;

    double p0_x;
    double p0_y;
    double p1_x;
    double p1_y;
    double p2_x;
    double p2_y;
};

#endif // CURVATURE_H
