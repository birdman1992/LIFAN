#include "mainwindow.h"
#include <QApplication>
#include "thread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    CSimpleThread cst;
    CSimpleThread_1 cst1;

    cst.start();
    cst1.start();

    return a.exec();
}
