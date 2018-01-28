#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include<QtGui>

class CameraWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *e);

signals:

public slots:

};

#endif // CAMERAWIDGET_H
