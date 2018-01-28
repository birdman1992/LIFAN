#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include<QtGui>

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *e);

signals:

public slots:

private:
    QPainterPath globalpath;

};

#endif // PAINTWIDGET_H
