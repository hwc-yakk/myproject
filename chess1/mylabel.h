#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QEvent>
#include <QObject>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);

    //重写事件分发器
    bool event(QEvent *e);
    int a;
    int b;

signals:
    void mylabel_signal(int a,int b);

public slots:
};

#endif // MYLABEL_H
