#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"decisionMake.h"
#include"gps.h"
//#include "braking.h"
//#include "gear.h"
//#include "steering.h"
//#include "throttle.h"
//#include "lightHorn.h"
//#include "remoteCtrl.h"
//#include "transmission.h"
#include<QtGui>
#include<QWheelEvent>
#include<qmath.h>
#include "includes.h"
#include"paintwidget.h"
#include"camerawidget.h"
#include"QFile"
#include<QTime>

//extern Braking brake;
//extern Gear gear;
//extern Steering steering;
//extern Throttle throttle;
//extern LightHorn light_horn;
//extern RemoteCtrl remote_ctrl;
//extern Transmission transmission;

unsigned long cnt=0;      //模拟跟踪
double x_start;//起始点x
double y_start;//起始点y
double x_min;
double x_max;
double y_min;
double y_max;

QFile file("1real_track.txt");
QTextStream txtOutput(&file);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scale=0.825795;
    save_flag=false;

    PaintWidget *w1= new PaintWidget();
    w1->setWindowTitle("全局路径");
  //  w1->setWindowFlags(Qt::WindowStaysOnTopHint);
//    w1->setParent(this);
//    connect(this, SIGNAL(closed()), w1, SLOT(close()));
//    w1->move(0,0);
    w1->show();

    CameraWidget *w2 = new CameraWidget();
    w2->setWindowTitle("Video");
    w2->move(0,20);
    w2->show();

    QTimer *timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);//刷新频率

    savetimer=new QTimer;
    connect(savetimer, SIGNAL(timeout()), this, SLOT(savepathdata()));

    connect(ui->savepath,SIGNAL(clicked()),this,SLOT(savepathdata()));

    //判断文件是否存在
    if(file.exists()){
        qDebug()<<"文件已存在";
    }else{
        qDebug()<<"文件不存在";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

QPainterPath fourpoint_drawRect(double objx,double objy,double x1,double y1,double x2,double y2,
                                                                                                     double x3,double y3,double x4,double y4)
{
    QPainterPath rect;
    int magnify_coefficient=5;
    rect.moveTo((x1-objx)*magnify_coefficient,-(y1-objy)*magnify_coefficient);
    rect.lineTo((x2-objx)*magnify_coefficient,-(y2-objy)*magnify_coefficient);
    rect.lineTo((x3-objx)*magnify_coefficient,-(y3-objy)*magnify_coefficient);
    rect.lineTo((x4-objx)*magnify_coefficient,-(y4-objy)*magnify_coefficient);
    rect.lineTo((x1-objx)*magnify_coefficient,-(y1-objy)*magnify_coefficient);
    return rect;
}

void MainWindow::paintEvent(QPaintEvent *)
{

    /**************************************************************************************************/
    ui->groupBox->setGeometry(20,20,width()-4*width()/5,height()-ui->frame_status->height()-10);
    ui->frame_direction->setGeometry(width()-240,ui->frame_status->height()+5,240,331);
    ui->frame_status->setGeometry(width()-240,0,240,281);
    ui->lineEdit->setGeometry(width()-113,height()-60,113,27);
    ui->savepath->setGeometry(width()-240-99,10+5,99,27);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//抗锯齿

    cnt=decisionMake.aim_point_sn-1;//预瞄点

    ui->keycode->setText(QString::number(decisionMake.key_code));
    //当前偏航角
    ui->cur_direction->setText(QString::number(gps_data.m_strHeading));
    //目标偏航角
    ui->obj_direction->setText(QString::number(decisionMake.aim_azimuth));
    //目标速度
    ui->obj_speed->setText(QString::number(decisionMake.vehicle_speed_obj));
    //前轮转角
//    ui->wheelangle->setText(QString::number(steering.EPS_SteeringWheelAngle/15.2));
    //制动压力
//    ui->braking_pressure->setText(QString::number(brake.ls_prur));
    //档位
//    switch (gear.cur_gear) {
//    case 10:
//        ui->gear_value->setText("P");
//        break;
//    case 11:
//        ui->gear_value->setText("R");
//        break;
//    case 12:
//        ui->gear_value->setText("N");
//        break;
//    case 13:
//        ui->gear_value->setText("D");
//        break;
//    case 14:
//        ui->gear_value->setText("E");
//        break;
//    case 15:
//        ui->gear_value->setText("L");
//    default:
//        ui->gear_value->setText("数据错误");
//        break;
//    }
    //速度
    ui->vehicle_speed->setText(QString::number(decisionMake.vehicle_speed ));
    //油门开度
    ui->throttle_open->setText(QString::number(decisionMake.throttle_open ));
    //目标开度
//    ui->desire_throttle_open->setText(QString::number(throttle.CUR_THRT_OPN));
    //遥控
//    if(remote_ctrl.auto_manual) ui->auto_manual->setText("自动模式");
//    else
//    {
//        ui->auto_manual->setText("手动模式");
//        ui->launch_stop->setText("");
//    }
//    if(remote_ctrl.auto_manual&&remote_ctrl.launch_stop) ui->launch_stop->setText("启动");
//    else if(remote_ctrl.auto_manual&&(!remote_ctrl.launch_stop)) ui->launch_stop->setText("停止");
    if(decisionMake.urg_brake_flag)
    {
        ui->urg_brake_falg->setText("紧急制动状态");
        ui->auto_manual->setText("");
        ui->launch_stop->setText("");
    }
    else ui->urg_brake_falg->setText("");
    //垂直距离
    ui->verticalDist->setText(QString::number(decisionMake.verticalDist));
    //GPS状态
    switch(gps_data.m_strStatus)
    {
    case 0:
        ui->GPS_status->setText("初始化");
        break;
    case 1:
        ui->GPS_status->setText("粗对准");
        break;
    case 2:
        ui->GPS_status->setText("精对准");
        break;
    case 3:
        ui->GPS_status->setText("GPS定位");
        break;
    case 4:
        ui->GPS_status->setText("GPS定向");
        break;
    case 5:
        ui->GPS_status->setText("RTK");
        break;
    case 6:
        ui->GPS_status->setText("DMI组合");
        break;
    case 7:
        ui->GPS_status->setText("DMI标定");
        break;
    case 8:
        ui->GPS_status->setText("纯惯性");
        break;
    case 9:
        ui->GPS_status->setText("零速校正");
        break;
    case 10:
        ui->GPS_status->setText("VG模式");
        break;
    case 11:
        ui->GPS_status->setText("差分定向");
        break;
    case 12:
        ui->GPS_status->setText("动态对准");
        break;
    default:
        ui->GPS_status->setText("数据错误");
        break;
    }
    //交通灯
    if(decisionMake.lamp_color_l)  ui->lamp_color_l->setStyleSheet("background-color:red");
    else   ui->lamp_color_l->setStyleSheet("background-color:green");
    if(decisionMake.lamp_color_m)  ui->lamp_color_m->setStyleSheet("background-color:red");
    else   ui->lamp_color_m->setStyleSheet("background-color:green");
    if(decisionMake.lamp_color_r)  ui->lamp_color_r->setStyleSheet("background-color:red");
    else   ui->lamp_color_r->setStyleSheet("background-color:green");
    //车道线
    if(decisionMake.line_type_l_flag==1)
    {
        if(decisionMake.line_type_l) ui->lane_l_style->setText("实线");
        else ui->lane_l_style->setText("虚线");
    }
    else if(decisionMake.line_type_l_flag==0) ui->lane_l_style->setText("");

    if(decisionMake.line_type_r_flag==1)
    {
        if(decisionMake.line_type_r) ui->lane_r_style->setText("实线");
        else ui->lane_r_style->setText("虚线");
    }
    else if(decisionMake.line_type_r_flag==0) ui->lane_r_style->setText("");

    if(decisionMake.line_dist_flag)
    ui->lane_difference->setText(QString::number(decisionMake.line_dist));
    else if(decisionMake.line_dist_flag==0) ui->lane_difference->setText("");

    if(decisionMake.is_logo)  ui->isLogo->setStyleSheet("background-color:red");
    else ui->isLogo->setStyleSheet("background-color:green");

    /**********************************************局部路径绘制**************************************************/
    //坐标变换，绘制局部路径
    painter.translate(0.4*width(),height()/2);//坐标变换
    painter.scale(scale, scale);//放大倍数
    painter.save();//保存状态，之后有旋转操作

    int magnify_coefficient=5;//放大系数

    //绘制当前点在局部路径的位置
    painter.setPen(Qt::darkMagenta);
    QPainterPath cur_rect;
    cur_rect=fourpoint_drawRect(decisionMake.x_cor,decisionMake.y_cor,decisionMake.cur.p1_x,decisionMake.cur.p1_y,decisionMake.cur.p2_x,decisionMake.cur.p2_y,
                                                                          decisionMake.cur.p3_x,decisionMake.cur.p3_y,decisionMake.cur.p4_x,decisionMake.cur.p4_y);
    painter.drawPath(cur_rect);

    //绘制障碍物
    for(unsigned int i=0;i<decisionMake.obstacles_num;i++)
    {
        painter.setBrush(Qt::darkGray);
        painter.setPen(Qt::darkGray);

        QPainterPath obs_rect;
        obs_rect=fourpoint_drawRect(decisionMake.x_cor,decisionMake.y_cor,obstacles[i].p1_x,obstacles[i].p1_y,obstacles[i].p2_x,obstacles[i].p2_y,
                                                                               obstacles[i].p3_x,obstacles[i].p3_y,obstacles[i].p4_x,obstacles[i].p4_y);
        painter.drawPath(obs_rect);
    }

    extern double obstacle_bag[OBST_ROW][OBST_COL];
    for(unsigned int j=0;j<OBST_ROW;j++)
    {
        if(obstacle_bag[j][2]>=OBST_EXIST_TIMES)
        {
            painter.setBrush(QColor(255,140,0));
            painter.setPen(QColor(255,140,0));
            QPainterPath obs_rect;
            obs_rect=fourpoint_drawRect(decisionMake.x_cor,decisionMake.y_cor,obstacle_bag[j][5],obstacle_bag[j][6],obstacle_bag[j][7],obstacle_bag[j][8],
                                                                                   obstacle_bag[j][9],obstacle_bag[j][10],obstacle_bag[j][11],obstacle_bag[j][12]);
            painter.drawPath(obs_rect);
            painter.setFont(QFont("Arial", 15/scale));
            painter.drawText((obstacle_bag[j][7]-decisionMake.x_cor)*magnify_coefficient+5,-(obstacle_bag[j][8]-decisionMake.y_cor)*magnify_coefficient+5,QString::number(obstacle_bag[j][0]));
        }
    }

    extern double obstacle_bag_l[OBST_ROW_L][OBST_COL_L];
    for(unsigned int j=0;j<OBST_ROW_L;j++)
    {
        if(obstacle_bag_l[j][2]>=OBST_EXIST_TIMES_L)
        {
            painter.setBrush(QColor(220,20,60));
            painter.setPen(QColor(220,20,60));
            QPainterPath obs_rect;
            obs_rect=fourpoint_drawRect(decisionMake.x_cor,decisionMake.y_cor,obstacle_bag_l[j][5],obstacle_bag_l[j][6],obstacle_bag_l[j][7],obstacle_bag_l[j][8],
                                                                                   obstacle_bag_l[j][9],obstacle_bag_l[j][10],obstacle_bag_l[j][11],obstacle_bag_l[j][12]);
            painter.drawPath(obs_rect);
            painter.setFont(QFont("Arial", 15/scale));
            painter.drawText((obstacle_bag_l[j][7]-decisionMake.x_cor)*magnify_coefficient+5,-(obstacle_bag_l[j][8]-decisionMake.y_cor)*magnify_coefficient+5,QString::number(obstacle_bag_l[j][0]));
        }
    }

    extern double obstacle_bag_r[OBST_ROW_R][OBST_COL_R];
    for(unsigned int j=0;j<OBST_ROW_R;j++)
    {
        if(obstacle_bag_r[j][2]>=OBST_EXIST_TIMES)
        {
            painter.setBrush(QColor(178,34,34));
            painter.setPen(QColor(178,34,34));
            QPainterPath obs_rect;
            obs_rect=fourpoint_drawRect(decisionMake.x_cor,decisionMake.y_cor,obstacle_bag_r[j][5],obstacle_bag_r[j][6],obstacle_bag_r[j][7],obstacle_bag_r[j][8],
                                                                                   obstacle_bag_r[j][9],obstacle_bag_r[j][10],obstacle_bag_r[j][11],obstacle_bag_r[j][12]);
            painter.drawPath(obs_rect);
            painter.setFont(QFont("Arial", 15/scale));
            painter.drawText((obstacle_bag_r[j][7]-decisionMake.x_cor)*magnify_coefficient+5,-(obstacle_bag_r[j][8]-decisionMake.y_cor)*magnify_coefficient+5,QString::number(obstacle_bag_r[j][0]));
        }
    }

    //绘制预瞄点左侧的检测框
    painter.setPen(Qt::green);
    painter.setBrush(Qt::NoBrush);
    QPainterPath aim_rect_l;
    aim_rect_l=fourpoint_drawRect(decisionMake.x_cor,decisionMake.y_cor,decisionMake.aim_l.p1_x,decisionMake.aim_l.p1_y,decisionMake.aim_l.p2_x,decisionMake.aim_l.p2_y,
                                                  decisionMake.aim_l.p3_x,decisionMake.aim_l.p3_y,decisionMake.aim_l.p4_x,decisionMake.aim_l.p4_y);
    painter.drawPath(aim_rect_l);

    //绘制预瞄点右侧的检测框
    painter.setPen(Qt::green);
    painter.setBrush(Qt::NoBrush);
    QPainterPath aim_rect_r;
    aim_rect_r=fourpoint_drawRect(decisionMake.x_cor,decisionMake.y_cor,decisionMake.aim_r.p1_x,decisionMake.aim_r.p1_y,decisionMake.aim_r.p2_x,decisionMake.aim_r.p2_y,
                                                  decisionMake.aim_r.p3_x,decisionMake.aim_r.p3_y,decisionMake.aim_r.p4_x,decisionMake.aim_r.p4_y);
    painter.drawPath(aim_rect_r);

    //绘制预瞄点后20m的左侧检测框
    painter.setPen(Qt::green);
    painter.setBrush(Qt::NoBrush);
    if(decisionMake.which_lane==LEFT_LANE)
    {
        painter.setPen(Qt::darkBlue);
        painter.setBrush(Qt::NoBrush);
    }
    QPainterPath end_rect_l;
    end_rect_l=fourpoint_drawRect(decisionMake.x_cor,decisionMake.y_cor,decisionMake.end_l.p1_x,decisionMake.end_l.p1_y,decisionMake.end_l.p2_x,decisionMake.end_l.p2_y,
                                                  decisionMake.end_l.p3_x,decisionMake.end_l.p3_y,decisionMake.end_l.p4_x,decisionMake.end_l.p4_y);
    painter.drawPath(end_rect_l);

    //绘制预瞄点后20m的右侧检测框
    painter.setPen(Qt::green);
    painter.setBrush(Qt::NoBrush);
    if(decisionMake.which_lane==RIGHT_LANE)
    {
        painter.setPen(Qt::darkBlue);
        painter.setBrush(Qt::NoBrush);
    }
    QPainterPath end_rect_r;
    end_rect_r=fourpoint_drawRect(decisionMake.x_cor,decisionMake.y_cor,decisionMake.end_r.p1_x,decisionMake.end_r.p1_y,decisionMake.end_r.p2_x,decisionMake.end_r.p2_y,
                                                  decisionMake.end_r.p3_x,decisionMake.end_r.p3_y,decisionMake.end_r.p4_x,decisionMake.end_r.p4_y);
    painter.drawPath(end_rect_r);

    //绘制预瞄点后20m的检测框
    painter.setPen(Qt::green);
    painter.setBrush(Qt::NoBrush);
    if(decisionMake.which_lane==CENTER_LANE)
    {
        painter.setPen(Qt::darkBlue);
        painter.setBrush(Qt::NoBrush);
    }
    QPainterPath end_rect;
    end_rect=fourpoint_drawRect(decisionMake.x_cor,decisionMake.y_cor,decisionMake.end.p1_x,decisionMake.end.p1_y,decisionMake.end.p2_x,decisionMake.end.p2_y,
                                                  decisionMake.end.p3_x,decisionMake.end.p3_y,decisionMake.end.p4_x,decisionMake.end.p4_y);
    painter.drawPath(end_rect);

//    switch(decisionMake.which_lane)
//    {
//    case CENTER_LANE:
//    {

//    }
//        break;
//    case LEFT_LANE:
//    {

//    }
//        break;
//    case RIGHT_LANE:
//    {

//    }
//        break;
//    }

    //实际偏航角方向
    painter.setPen(Qt::red);
    painter.rotate(gps_data.m_strHeading);//y方向顺时针旋转
    painter.drawLine(0,0,0,-30);
    //前轮转角方向（偏航角+前轮转角）
    painter.setPen(Qt::green);
//    painter.rotate(-steering.EPS_SteeringWheelAngle/15.2);//y方向顺时针旋转,方向盘转角左正右负
    painter.drawLine(0,0,0,-30);

    painter.restore();//恢复到旋转前的状态

    //绘制预瞄点在局部路径的位置
    painter.setPen(Qt::red);
    painter.drawEllipse((gps_track[cnt][1]-decisionMake.x_cor)*magnify_coefficient-4,-(gps_track[cnt][2]-decisionMake.y_cor)*magnify_coefficient-4,8,8);
    QPainterPath part_path;
    //绘制局部路径
    painter.setPen(Qt::black);
    painter.setPen(Qt::DotLine);
    unsigned long show_num=150;
    if(cnt<show_num)
    {
        //设置路径的起始点
        part_path.moveTo((gps_track[0][1]-decisionMake.x_cor)*magnify_coefficient,-(gps_track[0][2]-decisionMake.y_cor)*magnify_coefficient);
        for(unsigned long i=0;i<cnt+show_num;i++)
        {
            double x,y;
            x=gps_track[i][1];
            y=gps_track[i][2];

            part_path.lineTo((x-decisionMake.x_cor)*magnify_coefficient,-(y-decisionMake.y_cor)*magnify_coefficient);
        }

        painter.drawPath(part_path);
    }
    else if((cnt<decisionMake.track_row-show_num)&&(cnt>show_num))
    {
        //设置路径的起始点
        part_path.moveTo((gps_track[cnt-show_num][1]-decisionMake.x_cor)*magnify_coefficient,-(gps_track[cnt-show_num][2]-decisionMake.y_cor)*magnify_coefficient);
        for(unsigned long i=cnt-show_num;i<cnt;i++)
        {
            double x,y;
            x=gps_track[i][1];
            y=gps_track[i][2];

            part_path.lineTo((x-decisionMake.x_cor)*magnify_coefficient,-(y-decisionMake.y_cor)*magnify_coefficient);
        }

        for(unsigned long i=cnt;i<cnt+show_num;i++)
        {
            double x,y;
            x=gps_track[i][1];
            y=gps_track[i][2];

            part_path.lineTo((x-decisionMake.x_cor)*magnify_coefficient,-(y-decisionMake.y_cor)*magnify_coefficient);
        }

        painter.drawPath(part_path);
    }
    else if(cnt>decisionMake.track_row-show_num)
    {
        //设置路径的起始点
        part_path.moveTo((gps_track[decisionMake.track_row-1][1]-decisionMake.x_cor)*magnify_coefficient,-(gps_track[decisionMake.track_row-1][2]-decisionMake.y_cor)*magnify_coefficient);
        for(unsigned long i=decisionMake.track_row-1;i>cnt-show_num;i--)
        {
            double x,y;
            x=gps_track[i][1];
            y=gps_track[i][2];

            part_path.lineTo((x-decisionMake.x_cor)*magnify_coefficient,-(y-decisionMake.y_cor)*magnify_coefficient);
        }

        painter.drawPath(part_path);
    }
    /**************************************************左边***********************************************************/
    QPainterPath part_path_l;
    //绘制局部路径
    painter.setPen(Qt::black);
    painter.setPen(Qt::DotLine);
    if(cnt<show_num)
    {
        //设置路径的起始点
        part_path_l.moveTo((gps_track[0][1]+gps_delta_x_l(1)-decisionMake.x_cor)*magnify_coefficient,-(gps_track[0][2]+gps_delta_y_l(1)-decisionMake.y_cor)*magnify_coefficient);
        for(unsigned long i=0;i<cnt+show_num;i++)
        {
            double x,y;
            x=gps_track[i][1]+gps_delta_x_l(i+1);
            y=gps_track[i][2]+gps_delta_y_l(i+1);

            part_path_l.lineTo((x-decisionMake.x_cor)*magnify_coefficient,-(y-decisionMake.y_cor)*magnify_coefficient);
        }

        painter.drawPath(part_path_l);
    }
    else if((cnt<decisionMake.track_row-show_num)&&(cnt>show_num))
    {
        //设置路径的起始点
        part_path_l.moveTo((gps_track[cnt-show_num][1]+gps_delta_x_l(decisionMake.aim_point_sn-show_num)-decisionMake.x_cor)*magnify_coefficient,-(gps_track[cnt-show_num][2]+gps_delta_y_l(decisionMake.aim_point_sn-show_num)-decisionMake.y_cor)*magnify_coefficient);
        for(unsigned long i=cnt-show_num;i<cnt;i++)
        {
            double x,y;
            x=gps_track[i][1]+gps_delta_x_l(i+1);
            y=gps_track[i][2]+gps_delta_y_l(i+1);

            part_path_l.lineTo((x-decisionMake.x_cor)*magnify_coefficient,-(y-decisionMake.y_cor)*magnify_coefficient);
        }

        for(unsigned long i=cnt;i<cnt+show_num;i++)
        {
            double x,y;
            x=gps_track[i][1]+gps_delta_x_l(i+1);
            y=gps_track[i][2]+gps_delta_y_l(i+1);

            part_path_l.lineTo((x-decisionMake.x_cor)*magnify_coefficient,-(y-decisionMake.y_cor)*magnify_coefficient);
        }

        painter.drawPath(part_path_l);
    }
    else if(cnt>decisionMake.track_row-show_num)
    {
        //设置路径的起始点
        part_path_l.moveTo((gps_track[decisionMake.track_row-1][1]+gps_delta_x_l(decisionMake.track_row-1)-decisionMake.x_cor)*magnify_coefficient,-(gps_track[decisionMake.track_row-1][2]+gps_delta_y_l(decisionMake.track_row-1)-decisionMake.y_cor)*magnify_coefficient);
        for(unsigned long i=decisionMake.track_row-1;i>cnt-show_num;i--)
        {
            double x,y;
            x=gps_track[i][1]+gps_delta_x_l(i+1);
            y=gps_track[i][2]+gps_delta_y_l(i+1);

            part_path_l.lineTo((x-decisionMake.x_cor)*magnify_coefficient,-(y-decisionMake.y_cor)*magnify_coefficient);
        }

        painter.drawPath(part_path_l);
    }
    /*************************************************************************************************************/
    /*************************************************************************************************************/
    QPainterPath part_path_r;
    //绘制局部路径
    painter.setPen(Qt::black);
    painter.setPen(Qt::DotLine);
    if(cnt<show_num)
    {
        //设置路径的起始点
        part_path_r.moveTo((gps_track[0][1]+gps_delta_x_r(1)-decisionMake.x_cor)*magnify_coefficient,-(gps_track[0][2]+gps_delta_y_r(1)-decisionMake.y_cor)*magnify_coefficient);
        for(unsigned long i=0;i<cnt+show_num;i++)
        {
            double x,y;
            x=gps_track[i][1]+gps_delta_x_r(i+1);
            y=gps_track[i][2]+gps_delta_y_r(i+1);

            part_path_r.lineTo((x-decisionMake.x_cor)*magnify_coefficient,-(y-decisionMake.y_cor)*magnify_coefficient);
        }

        painter.drawPath(part_path_r);
    }
    else if((cnt<decisionMake.track_row-show_num)&&(cnt>show_num))
    {
        //设置路径的起始点
        part_path_r.moveTo((gps_track[cnt-show_num][1]+gps_delta_x_r(decisionMake.aim_point_sn-show_num)-decisionMake.x_cor)*magnify_coefficient,-(gps_track[cnt-show_num][2]+gps_delta_y_r(decisionMake.aim_point_sn-show_num)-decisionMake.y_cor)*magnify_coefficient);
        for(unsigned long i=cnt-show_num;i<cnt;i++)
        {
            double x,y;
            x=gps_track[i][1]+gps_delta_x_r(i+1);
            y=gps_track[i][2]+gps_delta_y_r(i+1);

            part_path_r.lineTo((x-decisionMake.x_cor)*magnify_coefficient,-(y-decisionMake.y_cor)*magnify_coefficient);
        }

        for(unsigned long i=cnt;i<cnt+show_num;i++)
        {
            double x,y;
            x=gps_track[i][1]+gps_delta_x_r(i+1);
            y=gps_track[i][2]+gps_delta_y_r(i+1);

            part_path_r.lineTo((x-decisionMake.x_cor)*magnify_coefficient,-(y-decisionMake.y_cor)*magnify_coefficient);
        }

        painter.drawPath(part_path_r);
    }
    else if(cnt>decisionMake.track_row-show_num)
    {
        //设置路径的起始点
        part_path_r.moveTo((gps_track[decisionMake.track_row-1][1]+gps_delta_x_r(decisionMake.track_row-1)-decisionMake.x_cor)*magnify_coefficient,-(gps_track[decisionMake.track_row-1][2]+gps_delta_y_r(decisionMake.track_row-1)-decisionMake.y_cor)*magnify_coefficient);
        for(unsigned long i=decisionMake.track_row-1;i>cnt-show_num;i--)
        {
            double x,y;
            x=gps_track[i][1]+gps_delta_x_r(i+1);
            y=gps_track[i][2]+gps_delta_y_r(i+1);

            part_path_r.lineTo((x-decisionMake.x_cor)*magnify_coefficient,-(y-decisionMake.y_cor)*magnify_coefficient);
        }

        painter.drawPath(part_path_r);
    }
    /*************************************************************************************************************/
}

void MainWindow::wheelEvent(QWheelEvent *event)
{

    if(event->delta()>0)
    {
        scale*=1.05;
        ui->groupBox->update();//刷新绘图
    }
    else if(event->delta()<0)
    {
        scale/=1.05;
        ui->groupBox->update();//刷新绘图
    }
    event->accept();
}

void MainWindow::on_lineEdit_editingFinished()
{
    //extern unsigned  int direction;
  //  direction=ui->lineEdit->text().toUInt();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        this->close();
        break;
    }
}

void MainWindow::on_savepath_clicked()
{
    save_flag=!save_flag;
}

void MainWindow::savepathdata()
{
    if(save_flag==true)
    {
        savetimer->start(100);
        ui->savepath->setText("保存中");
        //已读写方式打开文件，
        //如果文件不存在会自动创建文件
        if(!file.open(QIODevice::ReadWrite)){
            qDebug()<<"打开失败";
        }
         //获得文件大小
         qint64 pos;
         pos = file.size();
         //重新定位文件输入位置，这里是定位到文件尾端
         file.seek(pos);

        QTime qctime = QTime::currentTime();
        QString qstrtime =qctime.toString("h:m:s.z");

//        txtOutput <<qstrtime<<"   "<< QString::number(gps_data.m_strLatitude,'f',7) <<"   "<<QString::number(gps_data.m_strLongitude,'f',7)<<"   "<<"   "<<QString::number(gps_track[decisionMake.cp_sn][3],'f',7)<<"   "<<QString::number(gps_data.m_strHeading,'f',7)<<"   "<<QString::number(gps_track[decisionMake.cp_sn][4],'f',2)<<"   "<<QString::number(gps_data.m_strVe,'f',7)<<"   "<<QString::number(gps_data.m_strVn,'f',7)<<"   "<<QString::number(gps_data.m_strVu,'f',7)<<"   "<<QString::number(decisionMake.steering_angle,'f',2) <<"   "<<QString::number(steering.EPS_SteeringWheelAngle,'f',2)<<"\r\n";

        file.close();
    }
    else
    {
        savetimer->stop();
        ui->savepath->setText("未保存");
    }
}
