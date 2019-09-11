#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <mainwindow.h>
#include "gamewindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GameWindow *w1 = new GameWindow;

    connect(ui->button_login,&QPushButton::clicked,[=](){
    QString username = ui->lineEdit1->text();
    QString passworld = ui->lineEdit2->text();

    if(username == "yakking" && passworld == "123456")
    {
        this->hide();
        w1->show();
        QMessageBox::information(this,"Welcome","欢迎来到黑白棋");
    }
    else
    {
        QMessageBox::critical(this,"错误","用户名或密码错误");
        ui->lineEdit2->setText("");
    }
    connect(w1,&GameWindow::mysignal,[=](){
        w1->hide();
        this->show();
        ui->lineEdit2->setText("");
    });
    });
    connect(ui->button_exit,&QPushButton::clicked,this,&MainWindow::close);

    //设置登录界面的背景
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/bg1.jpg")));
    setPalette(pal);
    //setStyleSheet("border-image:url(:/image/bg1.jpg)");

}

MainWindow::~MainWindow()
{
    delete ui;
}
