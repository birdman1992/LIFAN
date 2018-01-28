#include "mainwindow.h"
#include <QApplication>
#include "thread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   //MainWindow w;
  // w.show();
   ThreadLowlayer threadLowerL;
   Lidar4 lidar4Line;
   Lidar16 lidar16Line;


    threadLowerL.start();
    lidar4Line.start();
    lidar16Line.start();

   return a.exec();
}
