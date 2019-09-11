#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QEvent>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *ev); //鼠标单击事件
    int a;
    int b;
    
signals:
     void mylabel_signal(int a,int b);
    
public slots:
    
};

#endif // MYLABEL_H
