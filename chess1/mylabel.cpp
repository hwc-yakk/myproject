#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{

}

bool MyLabel::event(QEvent *e)
{

    if(e->type() == QEvent::MouseButtonPress)
    {
        //qDebug()<<"事件分发器";
        //qDebug()<<"鼠标单击了";
        QMouseEvent *ev = static_cast<QMouseEvent *>(e);
        if(ev->button() == Qt::LeftButton)
        {
            //qDebug()<<"鼠标左键点击了";
            //qDebug()<<"局部：x = " << ev->x() << ", y = " << ev->y();
            //qDebug()<<"全局：x = " << ev->globalX() << ", y = " << ev->globalY();
            int a= (ev->x()/50)+1,b=(ev->y()/50)+1;
            //qDebug()<< "***************************";
            qDebug()<< a << b;

            emit this->mylabel_signal(a,b);
        }
    }
     //return QLabel::event(e);
    return true;
}
