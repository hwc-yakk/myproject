#include <QApplication>
#include "widget.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");       //UTF-8
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QApplication a(argc, argv);

    Widget w;
    w.show();

    return a.exec();
}
