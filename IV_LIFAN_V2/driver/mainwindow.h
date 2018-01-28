#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void keyPressEvent(QKeyEvent *e);


    void on_gearEdit_editingFinished();

    void on_mode_switch_clicked();

    void on_throttleEdit_editingFinished();

    void on_brakingEdit_editingFinished();

    void on_steeringEdit_editingFinished();

    void data_update();

private:
    Ui::MainWindow *ui;

    QTimer *timer;
};

#endif // MAINWINDOW_H
