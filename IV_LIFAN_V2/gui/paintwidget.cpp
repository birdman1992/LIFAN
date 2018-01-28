#include "paintwidget.h"
#include <QPainter>
#include <QWidget>
#include "includes.h"
#include<QTimer>
#include<QtGui>

double start_x;//起始点x
double start_y;//起始点y
double min_x;
double max_x;
double min_y;
double max_y;

PaintWidget::PaintWidget(QWidget *parent) :
    QWidget(parent)
{
    start_x=gps_track[0][1];
    start_y=gps_track[0][2];
    min_x=gps_track[0][1];
    min_y=gps_track[0][2];
    max_x=gps_track[0][1];
    max_y=gps_track[0][2];

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//抗锯齿

    for(unsigned int i=0;i<decisionMake.track_row;i++)
    {
        double x,y;
        x=gps_track[i][1];
        y=gps_track[i][2];

        if(x>max_x) max_x=x;
        if(x<min_x) min_x=x;
        if(y>max_y) max_y=y;
        if(y<min_y) min_y=y;

        globalpath.lineTo((x-start_x),-(y-start_y));
    }
    this->setGeometry(10,10,max_x-min_x+40,max_y-min_y+40);
    painter.drawPath(globalpath);

    QTimer *timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(500);
}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);

    painter.setPen(Qt::black);
    painter.drawRect(0,0,width(),height());
    painter.translate((start_x-min_x)+20,(max_y-start_y)+20);//坐标变换

    painter.setPen(Qt::blue);
    painter.drawPath(globalpath);
    painter.setPen(Qt::red);

    /********************************全局路径绘制***************************************************************/
    //绘制预瞄点在全局地图中的位置
    double obj_x,obj_y;
    obj_x=gps_track[decisionMake.aim_point_sn-1][1];
    obj_y=gps_track[decisionMake.aim_point_sn-1][2];
    QPoint aim_position((obj_x-start_x),-(obj_y-start_y));
    painter.drawEllipse(aim_position,5,5);

    //绘制当前车的坐标
    painter.setPen(Qt::darkMagenta);
    QPoint cur_position((decisionMake.x_cor-start_x),-(decisionMake.y_cor-start_y));
    painter.drawEllipse(cur_position,5,5);

    /***********************************************************************************************************/

}

void PaintWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        this->close();
        break;
    }
}


