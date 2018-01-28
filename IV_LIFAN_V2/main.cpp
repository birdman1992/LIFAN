#include "mainwindow.h"
#include <QApplication>
#include "thread.h"
#include "decisionMake.h"
#include "gps.h"

void InitSystem(void);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DecisionMakeThrd decision_make_thrd;
    InitSystem();
   LowlayerThrd  lowerlayer_thrd;
//   Lidar4Thrd lidar4_thrd;
//   Lidar16Thrd lidar16_thrd;
   GpsThrd gps_thrd;

    MainWindow w;
    w.show();

    lowerlayer_thrd.start();
//    lidar16_thrd.start();
    gps_thrd.start();
    decision_make_thrd.start();

//    lidar4_thrd.start();

   return a.exec();
}
