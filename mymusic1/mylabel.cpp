#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent) :
    QLabel(parent)
{

}

void MyLabel::mousePressEvent(QMouseEvent *ev)
{
    //qDebug() << "鼠标单击啦";
    //qDebug() << "x = " << ev->x() << "y = " << ev->y();
    a = ev->x()/150;
    b = ev->y()/30;
    qDebug() << "a = " << a << "b = " << b;
    emit this->mylabel_signal(a,b);
}
