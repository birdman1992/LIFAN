#include "camerawidget.h"
#include"QTimer"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QPainter>
#include<QtGui>

extern cv::Mat frame;			//图片读取
CameraWidget::CameraWidget(QWidget *parent) :
    QWidget(parent)
{
    QTimer *timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
}

void CameraWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(frame.cols>0&&frame.rows>0)
    {
        cv::Mat frame_temp;
        cv::cvtColor(frame,frame_temp,CV_BGR2RGB);
        QImage image1 = QImage((uchar*)(frame_temp.data),frame_temp.cols,frame_temp.rows,QImage::Format_RGB888);
        painter.drawImage(0,0,image1);
    }
}

void CameraWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        this->close();
        break;
    }
}
