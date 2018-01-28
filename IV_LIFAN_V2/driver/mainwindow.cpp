#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QTimer>
#include<QtGui>
#include"lowlayer.h"
#include"thread.h"

extern lowlayer LowLayer;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer;
    timer->start(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(data_update()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{

}

void MainWindow::on_gearEdit_editingFinished()
{
    extern unsigned char gear;
    gear=ui->gearEdit->text().toInt();
}

void MainWindow::on_mode_switch_clicked()
{
    extern unsigned mode;
    if(LowLayer.cur_mode == 0)
    {
        mode=0x03;
        ui->mode_switch->setText("人工模式");
    }
    else if (LowLayer.cur_mode ==1)
    {
        mode = 0x02;
        ui->mode_switch->setText("自动模式");
    }
}

void MainWindow::on_throttleEdit_editingFinished()
{
    extern short throttle;
    throttle=ui->throttleEdit->text().toInt();
}

void MainWindow::on_brakingEdit_editingFinished()
{
    extern unsigned char brake_ist;
    brake_ist=ui->brakingEdit->text().toInt();
}

void MainWindow::on_steeringEdit_editingFinished()
{
    extern int steering_angle;
    steering_angle = ui->steeringEdit->text().toInt();
}

void MainWindow::data_update()
{

    QString gt=QString::number(LowLayer.cur_gear,10);
    QString tt=QString::number(LowLayer.cur_throttle,10);
    QString blt=QString::number(LowLayer.cur_brake_ist,10);
    QString md=QString::number(LowLayer.cur_mode,10);
    QString st=QString::number(LowLayer.cur_steering_angle,10);

    ui->gearlabel->setText(gt);

    ui->throttlelabel->setText(tt);

    ui->leftlabel->setText(blt);

    ui->modelabel->setText(md);

    ui->anglelabel->setText(st);
}

