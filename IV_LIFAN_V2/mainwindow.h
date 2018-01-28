#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void paintEvent(QPaintEvent *);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *e);

private slots:
    void on_lineEdit_editingFinished();

    void on_savepath_clicked();

    void savepathdata();

private:
    Ui::MainWindow *ui;
    float scale;
    QPainterPath globalpath;
    bool save_flag;
    QTimer *savetimer;
};

#endif // MAINWINDOW_H
