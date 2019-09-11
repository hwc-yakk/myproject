#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QPushButton>
#include <QPixmap>
#include <QLabel>
#include <QColor>
#include <QPixmap>
#include <QString>
#include <QDebug>
#include "mylabel.h"
#include <QMessageBox>
#include <QWidget>
#include <unistd.h>


int chess = 1;//1轮到黑棋手落子，0轮到白棋手落子
int player_signal = 0;//1为合理，0为不合理
int num_white = 0;//白棋子数量
int num_black = 0;//黑棋子数量
int ret = 0;//1为有合理位置，0为没有合理位置

//棋盘初始化
void GameWindow::initial()
{
    chess = 1;//1轮到黑棋手落子，0轮到白棋手落子
    player_signal = 0;//1为合理，0为不合理
    num_white = 2;//白棋子数量
    num_black = 2;//黑棋子数量
    ret = 0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            Q[i][j] = 0;
            label[i][j]->clear();
        }
    }
    label[3][3]->setPixmap(pix2);
    Q[3][3] = 2;
    label[3][4]->setPixmap(pix1);
    Q[3][4] = 1;
    label[4][3]->setPixmap(pix1);
    Q[4][3] = 1;
    label[4][4]->setPixmap(pix2);
    Q[4][4] = 2;
    ui->lineEdit_black->setText(QString::number(num_black));//显示黑棋子数量
    ui->lineEdit_white->setText(QString::number(num_white));//显示白棋子数量
    ui->button_white->setStyleSheet("");
    ui->button_black->setStyleSheet("");
    //QMessageBox::information(NULL, "选择", "请选择白棋手或者黑棋手", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}
//构造函数
GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    connect(ui->button_return,&QPushButton::clicked,[=](){
        emit this->mysignal();
    });
    //设置游戏界面的背景
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap(":/image/bg2.jpg")));
    setPalette(pal);
    //setStyleSheet("border-image:url(:/image/bg2.jpg)");

    //QPixmap pix1,pix2;
    pix1.load(":/image/white.png");
    pix2.load(":/image/black.png");

    //创建64个label
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            //QLabel *label = new QLabel(this);
            label[i][j] = new QLabel(this);
            Q[i][j] = 0;
            label[i][j]->setFixedSize(50,50);
            label[i][j]->move(50+50*i,50+50*j);
            label[i][j]->setStyleSheet("QLabel{border:2px solid rgb(0, 0, 0);}");
            //label[i][j]->setPixmap(pix);
        }
    }

    //创建一个大label将64个小label框起来
    MyLabel *label1 = new MyLabel(this);
    label1->setFixedSize(400,400);
    label1->move(50,50);

    //设置四个固定的棋子颜色
//    label[3][3]->setPixmap(pix2);
//    Q[3][3] = 2;
//    label[3][4]->setPixmap(pix1);
//    Q[3][4] = 1;
//    label[4][3]->setPixmap(pix1);
//    Q[4][3] = 1;
//    label[4][4]->setPixmap(pix2);
//    Q[4][4] = 2;

    //再玩一次，重新开始
    connect(ui->button_start,&QPushButton::clicked,[=](){
        initial();
        connect(ui->button_people,&QPushButton::clicked,[=](){
            QMessageBox::StandardButton rb1 = QMessageBox::information(NULL, "选择模式", "Yes为人人对战，No为人机对战", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            //选择游戏模式
            if(rb1 == QMessageBox::Yes)
            {
                connect(label1,&MyLabel::mylabel_signal,this,&GameWindow::mylabel_signal_slot);
            }
            else if(rb1 == QMessageBox::No)
            {
                 connect(label1,&MyLabel::mylabel_signal,this,&GameWindow::man_machine_mylabel_signal_slot);
            }
            QMessageBox::StandardButton rb = QMessageBox::information(NULL, "请选择棋手", "Yes为黑棋手，No为白棋手", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            //选择棋手
            if(rb == QMessageBox::Yes)
            {
               chess = 1;
               qDebug()<<chess<<"黑棋手";
               ui->button_white->setStyleSheet("");
               ui->button_black->setStyleSheet(tr("background-image: url(:/image/black.png);"));
            }
            else if(rb == QMessageBox::No)
            {
                chess = 0;
                qDebug()<<chess<<"白棋手";
                ui->button_black->setStyleSheet("");
                ui->button_white->setStyleSheet(tr("background-image: url(:/image/white.png);"));
            }
        });
        });
}

GameWindow::~GameWindow()
{
    delete ui;
}

//黑棋子规则
void GameWindow::rules_black(int a, int b)
{
     int i,j,k,f;
     player_signal = 0;
     //向右黑
     //qDebug()<<"规则判断黑";
     for(i=a;i<8;i++)
     {
         //qDebug()<<"黑1";
         if(Q[i][b-1] == 0 || Q[a][b-1] == 2)
         {
             break;
         }
         else if(Q[i][b-1] != 2)
         {
             continue;
         }
         else if(Q[i][b-1] == 2)
         {

             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix2);
             Q[a-1][b-1] = 2;
             for(j=a;j<i;j++)
             {
                 //qDebug()<<"改棋子"<<j<<b-1;
                 label[j][b-1]->setPixmap(pix2);
                 Q[j][b-1] = 2;
             }
             break;
         }
     }
     //向左黑
     for(i=a-2;i>=0;i--)
     {
         //qDebug()<<"黑2";
         if(Q[i][b-1] == 0 || Q[a-2][b-1] == 2)
         {
             break;
         }
         else if(Q[i][b-1] != 2)
         {
             continue;
         }
         else if(Q[i][b-1] == 2)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix2);
             Q[a-1][b-1] = 2;
             for(j=a-2;j>i;j--)
             {
                 //qDebug()<<"改棋子"<<j<<b-1;
                 label[j][b-1]->setPixmap(pix2);
                 Q[j][b-1] = 2;
             }
             break;
         }
     }
     //向上黑
     for(i=b-2;i>=0;i--)
     {
         //qDebug()<<"黑3";
         if(Q[a-1][i] == 0 || Q[a-1][b-2] == 2)
         {
             break;
         }
         else if(Q[a-1][i] != 2)
         {
             continue;
         }
         else if(Q[a-1][i] == 2)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix2);
             Q[a-1][b-1] = 2;
             for(j=b-2;j>i;j--)
             {
                 //qDebug()<<"改棋子"<<a-1<<j;
                 label[a-1][j]->setPixmap(pix2);
                 Q[a-1][j] = 2;
             }
             break;
         }
     }
     //向下黑
     for(i=b;i<8;i++)
     {
         //qDebug()<<"黑4";
         if(Q[a-1][i] == 0 || Q[a-1][b] == 2)
         {
             break;
         }
         else if(Q[a-1][i] != 2)
         {
             continue;
         }
         else if(Q[a-1][i] == 2)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix2);
             Q[a-1][b-1] = 2;
             for(j=b;j<i;j++)
             {
                 //qDebug()<<"改棋子"<<a-1<<j;
                 label[a-1][j]->setPixmap(pix2);
                 Q[a-1][j] = 2;
             }
             break;
         }
     }
     //左上黑
     for(i=a-2,j=b-2;i>=0 && j>=0;i--,j--)
     {
         //qDebug()<<"黑5";
         if(Q[i][j] == 0 || Q[a-2][b-2] == 2)
         {
             break;
         }
         else if(Q[i][j] !=2)
         {
             continue;
         }
         else if(Q[i][j] == 2)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix2);
             Q[a-1][b-1] = 2;
             for(k=a-2,f=b-2;k>i && f>j;k--,f--)
             {
                 //qDebug()<<"改棋子"<<k<<f;
                 label[k][f]->setPixmap(pix2);
                 Q[k][f] = 2;
             }
             break;
         }
     }
     //右下黑
     for(i=a,j=b;i<8 && j<8;i++,j++)
     {
         //qDebug()<<"黑6";
         if(Q[i][j] == 0 || Q[a][b] == 2)
         {
             break;
         }
         else if(Q[i][j] != 2)
         {
             continue;
         }
         else if(Q[i][j] == 2)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix2);
             Q[a-1][b-1] = 2;
             for(k=a,f=b;k<i && f<j;k++,f++)
             {
                // qDebug()<<"改棋子"<<k<<f;
                 label[k][f]->setPixmap(pix2);
                 Q[k][f] = 2;
             }
             break;
         }
     }
     //右上黑
     for(i=a,j=b-2;i<8 && j>=0;i++,j--)
     {
         //qDebug()<<"黑7";
         if(Q[i][j] == 0 || Q[a][b-2] == 2)
         {
             break;
         }
         else if(Q[i][j] != 2)
         {
             //flag = 1;
             continue;
         }
         else if(Q[i][j] == 2)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix2);
             Q[a-1][b-1] = 2;
             for(k=a,f=b-2;k<i && f>j;k++,f--)
             {
                 //qDebug()<<"改棋子"<<k<<f;
                 label[k][f]->setPixmap(pix2);
                 Q[k][f] = 2;
             }
             break;
         }
     }
     //左下黑
     for(i=a-2,j=b;i>=0 && j<8;i--,j++)
     {
         //qDebug()<<"黑8";
         if(Q[i][j] == 0 || Q[a-2][b] == 2)
         {
             break;
         }
         else if(Q[i][j] != 2)
         {
             continue;
         }
         else if(Q[i][j] == 2)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix2);
             Q[a-1][b-1] = 2;
             for(k=a-2,f=b;k>i && f<j;k--,f++)
             {
                 //qDebug()<<"改棋子"<<k<<f;
                 label[k][f]->setPixmap(pix2);
                 Q[k][f] = 2;
             }
             break;
         }
     }

}
//白棋子规则
void GameWindow::rules_white(int a, int b)
{
     int i,j,k,f;
     player_signal = 0;
     //qDebug()<<"规则判断白";
     //向右白
     //player_signal = 0;
     for(i=a;i<8;i++)
     {
         //qDebug()<<"白1";
         if(Q[i][b-1] == 0 || Q[a][b-1] == 1)
         {
             break;
         }
         else if(Q[i][b-1] != 1)
         {
             continue;
         }
         else if(Q[i][b-1] == 1)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix1);
             Q[a-1][b-1] = 1;
             for(int j=a;j<i;j++)
             {
                 //qDebug()<< "改棋子" <<j<<b-1;
                 label[j][b-1]->setPixmap(pix1);
                 Q[j][b-1] = 1;
             }
              break;
         }
     }
     //向左白
     for(i=a-2;i>=0;i--)
     {
         //qDebug()<<"白2";
         if(Q[i][b-1] == 0 || Q[a-2][b-1]  == 1)
         {
             break;
         }
         else if(Q[i][b-1] != 1)
         {
             continue;
         }
         else if(Q[i][b-1] == 1)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix1);
             Q[a-1][b-1] = 1;
             for(j=a-2;j>i;j--)
             {
                 //qDebug()<< "改棋子" <<j<<b-1;
                 label[j][b-1]->setPixmap(pix1);
                 Q[j][b-1] = 1;
             }
             break;
         }
     }
     //向上白
     for(i=b-2;i>=0;i--)
     {
         //qDebug()<<"白3";
         if(Q[a-1][i] == 0 || Q[a-1][b-2] == 1)
         {
             break;
         }
         else if(Q[a-1][i] != 1)
         {
             continue;
         }
         else if(Q[a-1][i] == 1)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix1);
             Q[a-1][b-1] = 1;
             for(j=b-2;j>i;j--)
             {
                 //qDebug()<< "改棋子" <<a-1<<j;
                 label[a-1][j]->setPixmap(pix1);
                 Q[a-1][j] = 1;
             }
             break;
         }
     }
     //向下白
     for(i=b;i<8;i++)
     {
         //qDebug()<<"白4";
         if(Q[a-1][i] == 0 || Q[a-1][b] == 1)
         {
             break;
         }
         else if(Q[a-1][i] != 1)
         {
             continue;
         }
         else if(Q[a-1][i] == 1)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix1);
             Q[a-1][b-1] = 1;
             for(j=b;j<i;j++)
             {
                 //qDebug()<< "改棋子" <<a-1<<j;
                 label[a-1][j]->setPixmap(pix1);
                 Q[a-1][j] = 1;
             }
             break;
         }
     }
     //左上白
     for(i=a-2,j=b-2;i>=0 && j>=0;i--,j--)
     {
         //qDebug()<<"白5";
         if(Q[i][j] == 0 || Q[a-2][b-2] == 1)
         {
             break;
         }
         else if(Q[i][j] !=1)
         {
             continue;
         }
         else if(Q[i][j] == 1)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix1);
             Q[a-1][b-1] = 1;
             for(k=a-2,f=b-2;k>i && f>j;k--,f--)
             {
                 //qDebug()<< "改棋子" <<k<<f;
                 label[k][f]->setPixmap(pix1);
                 Q[k][f] = 1;
             }
             break;
         }
     }
     //右下白
     for(i=a,j=b;i<8 && j<8;i++,j++)
     {
         //qDebug()<<"白6";
         if(Q[i][j] == 0 || Q[a][b] == 1)
         {
             break;
         }
         else if(Q[i][j] != 1)
         {
             continue;
         }
         else if(Q[i][j] == 1)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix1);
             Q[a-1][b-1] = 1;
             for(k=a,f=b;k<i && f<j;k++,f++)
             {
                 //qDebug()<< "改棋子" <<k<<f;
                 label[k][f]->setPixmap(pix1);
                 Q[k][f] = 1;
             }
             break;
         }
     }
     //右上白
     for(i=a,j=b-2;i<8 && j>=0;i++,j--)
     {
         //qDebug()<<"白7";
         if(Q[i][j] == 0 || Q[a][b-2] == 1)
         {
             break;
         }
         else if(Q[i][j] != 1)
         {
             continue;
         }
         else if(Q[i][j] == 1)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix1);
             Q[a-1][b-1] = 1;
             for(k=a,f=b-2;k<i && f>j;k++,f--)
             {
                 //qDebug()<< "改棋子" <<k<<f;
                 label[k][f]->setPixmap(pix1);
                 Q[k][f] = 1;
             }
             break;
         }
     }
     //左下白
     for(i=a-2,j=b;i>=0 && j<8;i--,j++)
     {
         //qDebug()<<"白8";
         if(Q[i][j] == 0 || Q[a-2][b] == 1)
         {
             break;
         }
         else if(Q[i][j] != 1)
         {
             continue;
         }
         else if(Q[i][j] == 1)
         {
             player_signal = 1;
             label[a-1][b-1]->setPixmap(pix1);
             Q[a-1][b-1] = 1;
             for(k=a-2,f=b;k>i && f<j;k--,f++)
             {
                 //qDebug()<< "改棋子" <<k<<f;
                 label[k][f]->setPixmap(pix1);
                 Q[k][f] = 1;
             }
             break;
         }
     }
}
//判断谁获胜
void GameWindow::declare_winner()
{
    num_black = 0;
    num_white = 0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(Q[i][j] == 1)
            {
                num_white++;
               // qDebug()<<num_white;
            }
            else if(Q[i][j] == 2)
            {
                num_black++;
                //qDebug()<<num_black;
            }
        }
    }
     //qDebug()<<num_white;
     //qDebug()<<num_black;
    if((num_black + num_white == 64 || num_white == 0) && num_black > num_white)
    {
        qDebug()<< num_black << ">" << num_white <<"\t黑棋手获胜";
        QMessageBox::information(NULL, "获胜信息", "黑棋手获胜", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else if((num_black + num_white == 64 || num_black == 0) && num_black < num_white)
    {
        qDebug()<< num_black << "<" << num_white <<"\t白棋手获胜";
        //QMessageBox::information(this,"获胜信息","白棋手获胜");
        QMessageBox::information(NULL, "获胜信息", "白棋手获胜", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else if(num_black + num_white == 64 && num_black == num_white)
    {
        qDebug()<< num_black << "<" << num_white <<"\t白棋手获胜";
         QMessageBox::information(NULL, "获胜信息", "平局", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
}
//判断是否还有合理的格子落子(黑)
int GameWindow::reason_black()
{
    ret = 0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(Q[i][j] == 0)
            {
                rules2(i+1,j+1);
                if(player_signal == 1)
                {
                    ret =1;
                    return ret;
                }
            }
        }
    }
    return ret;
}
//判断是否还有合理的格子落子（白）
int GameWindow::reason_white()
{
    ret = 0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(Q[i][j] == 0)
            {
                rules1(i+1,j+1);
                if(player_signal == 1)
                {
                    ret = 1;
                    return ret;
                }
            }
        }
    }
    return ret;
}
//判断白棋子落子是否合理
void GameWindow::rules1(int a, int b)
{
    int i,j;
    player_signal = 0;
    //向右白
    for(i=a;i<8;i++)
    {
        if(Q[i][b-1] == 0 || Q[a][b-1] == 1)
        {
            break;
        }
        else if(Q[i][b-1] != 1)
        {
            continue;
        }
        else if(Q[i][b-1] == 1)
        {
            player_signal = 1;
        }
    }
    //向左白
    for(i=a-2;i>=0;i--)
    {
        if(Q[i][b-1] == 0 || Q[a-2][b-1] == 1)
        {
            break;
        }
        else if(Q[i][b-1] != 1)
        {
            continue;
        }
        else if(Q[i][b-1] == 1)
        {
            player_signal = 1;
        }
    }
    //向上白
    for(i=b-2;i>=0;i--)
    {
        if(Q[a-1][i] == 0 || Q[a-1][b-2] == 1)
        {
            break;
        }
        else if(Q[a-1][i] != 1)
        {
            continue;
        }
        else if(Q[a-1][i] == 1)
        {
            player_signal = 1;
        }
    }
    //向下白
    for(i=b;i<8;i++)
    {
        if(Q[a-1][i] == 0 || Q[a-1][b] == 1)
        {
            break;
        }
        else if(Q[a-1][i] != 1)
        {
            continue;
        }
        else if(Q[a-1][i] == 1)
        {
            player_signal = 1;
        }
    }
    //左上白
    for(i=a-2,j=b-2;i>=0 && j>=0;i--,j--)
    {
        if(Q[i][j] == 0 || Q[a-2][b-2] == 1)
        {
            break;
        }
        else if(Q[i][j] !=1)
        {
            continue;
        }
        else if(Q[i][j] == 1)
        {
            player_signal = 1;
        }
    }
    //右下白
    for(i=a,j=b;i<8 && j<8;i++,j++)
    {
        if(Q[i][j] == 0 || Q[a][b] == 1)
        {
            break;
        }
        else if(Q[i][j] != 1)
        {
            continue;
        }
        else if(Q[i][j] == 1)
        {
            player_signal = 1;
        }
    }
    //右上白
    for(i=a,j=b-2;i<8 && j>=0;i++,j--)
    {
        if(Q[i][j] == 0 || Q[a][b-2] == 1)
        {
            break;
        }
        else if(Q[i][j] != 1)
        {
            continue;
        }
        else if(Q[i][j] == 1)
        {
            player_signal = 1;
        }
    }
    //左下白
    for(i=a-2,j=b;i>=0 && j<8;i--,j++)
    {
        if(Q[i][j] == 0 || Q[a-2][b] == 1)
        {
            break;
        }
        else if(Q[i][j] != 1)
        {
            continue;
        }
        else if(Q[i][j] == 1)
        {
            player_signal = 1;
        }
    }

}
//判断黑棋子落子是否合理
void GameWindow::rules2(int a, int b)
{
    int i,j;
    player_signal = 0;
    //向右黑
    for(i=a;i<8;i++)
    {
        if(Q[i][b-1] == 0 || Q[a][b-1] == 2)
        {
            break;
        }
        else if(Q[i][b-1] != 2)
        {
            continue;
        }
        else if(Q[i][b-1] == 2)
        {
            player_signal = 1;
        }
    }
    //向左黑
    for(i=a-2;i>=0;i--)
    {
        if(Q[i][b-1] == 0 || Q[a-2][b-1] == 2)
        {
            break;
        }
        else if(Q[i][b-1] != 2)
        {
            continue;
        }
        else if(Q[i][b-1] == 2)
        {
            player_signal = 1;
        }
    }
    //向上黑
    for(i=b-2;i>=0;i--)
    {
        if(Q[a-1][i] == 0 || Q[a-1][b-2] == 2)
        {
            break;
        }
        else if(Q[a-1][i] != 2)
        {
            continue;
        }
        else if(Q[a-1][i] == 2)
        {
            player_signal = 1;
        }
    }
    //向下黑
    for(i=b;i<8;i++)
    {
        if(Q[a-1][i] == 0 || Q[a-1][b] == 2)
        {
            break;
        }
        else if(Q[a-1][i] != 2)
        {
            continue;
        }
        else if(Q[a-1][i] == 2)
        {
            player_signal = 1;
        }
    }
    //左上黑
    for(i=a-2,j=b-2;i>=0 && j>=0;i--,j--)
    {
        if(Q[i][j] == 0 || Q[a-2][b-2] == 2)
        {
            break;
        }
        else if(Q[i][j] !=2)
        {
            continue;
        }
        else if(Q[i][j] == 2)
        {
            player_signal = 1;
        }
    }
    //右下黑
    for(i=a,j=b;i<8 && j<8;i++,j++)
    {
        if(Q[i][j] == 0 || Q[a][b] == 2)
        {
            break;
        }
        else if(Q[i][j] != 2)
        {
            continue;
        }
        else if(Q[i][j] == 2)
        {
            player_signal = 1;
        }
    }
    //右上黑
    for(i=a,j=b-2;i<8 && j>=0;i++,j--)
    {
        if(Q[i][j] == 0 || Q[a][b-2] == 2)
        {
            break;
        }
        else if(Q[i][j] != 2)
        {
            continue;
        }
        else if(Q[i][j] == 2)
        {
            player_signal = 1;
        }
    }
    //左下黑
    for(i=a-2,j=b;i>=0 && j<8;i--,j++)
    {
        if(Q[i][j] == 0 || Q[a-2][b] == 2)
        {
            break;
        }
        else if(Q[i][j] != 2)
        {
            continue;
        }
        else if(Q[i][j] == 2)
        {
            player_signal = 1;
        }
    }
}
//判断是否还有合理的位置落子
void GameWindow::move_chess()
{
    int ret_black,ret_white;

    ret_white = reason_white();
    if(ret_white == 0 && chess == 0)
    {
        QMessageBox::information(NULL, "白棋手提示", "你已经无棋可下，应让对方再次落子", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        chess = 1;
        qDebug()<<"白棋手已无子可落，轮到黑棋手落子";
        qDebug()<< chess << ":黑棋手";
        ui->button_white->setStyleSheet("");
        ui->button_black->setStyleSheet(tr("background-image: url(:/image/black.png);"));
    }

    ret_black = reason_black();
    if(ret_black == 0 && chess == 1)
    {
        QMessageBox::information(NULL, "黑棋手提示", "你已经无棋可下，应让对方再次落子", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        chess = 0;
        qDebug()<<"黑棋手已无子可落，轮到白棋手落子";
        qDebug()<< chess << ":白棋手";
        ui->button_black->setStyleSheet("");
        ui->button_white->setStyleSheet(tr("background-image: url(:/image/white.png);"));
    }
}
//找出机器人的落子位置并落子
void GameWindow::man_machine_rules()
{
    player_signal = 0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(Q[i][j] == 0)
            {
                rules1(i+1,j+1);//判断此位置是否合理
                if(player_signal == 1)
                {
                    qDebug()<<i<<j;
                    rules_white(i+1,j+1);//落子并反转
                    //label[i][j]->setPixmap(pix1);
                    //Q[i][j] = 1;
                    break;
                }
            }

        }
        if(player_signal == 1)
        {
            break;
        }
    }
}
//槽函数：人人对战
void GameWindow::mylabel_signal_slot(int a, int b)
{
    //qDebug()<<Q[a-1][b-1];
    if(chess== 1 && Q[a-1][b-1] == 0)
    {
        //qDebug()<<"666666666";
        move_chess();
        rules_black(a,b);
        if(player_signal == 1)
        {
            chess = 0;
            qDebug()<< chess << ":白棋手";
            ui->button_black->setStyleSheet("");
            ui->button_white->setStyleSheet(tr("background-image: url(:/image/white.png);"));
        }
        else
        {
            qDebug()<< chess <<  ":黑棋手";
        }
    }
    else if(chess == 0 && Q[a-1][b-1] == 0)
    {
        move_chess();
        rules_white(a,b);
         qDebug()<<"**********";
        if(player_signal == 1)
        {
            chess = 1;
            qDebug()<< chess << ":黑棋手";
            ui->button_white->setStyleSheet("");
            ui->button_black->setStyleSheet(tr("background-image: url(:/image/black.png);"));
        }
        else
        {
            qDebug()<< chess <<  ":白棋手";
        }
    }
    declare_winner();
    ui->lineEdit_black->setText(QString::number(num_black));//显示黑棋子数量
    ui->lineEdit_white->setText(QString::number(num_white));//显示白棋子数量

}
//槽函数：人机对战
void GameWindow::man_machine_mylabel_signal_slot(int a, int b)
{
    move_chess();
    if(chess == 1)
    {
        if(chess== 1 && Q[a-1][b-1] == 0)
        {
            //qDebug()<<"666666666";
            //move_chess();
            rules_black(a,b);
            if(player_signal == 1)
            {
                chess = 0;
                qDebug()<< chess << ":白棋手";
                ui->button_black->setStyleSheet("");
                ui->button_white->setStyleSheet(tr("background-image: url(:/image/white.png);"));
            }
            else
            {
                qDebug()<< chess <<  ":黑棋手";
                return;
            }
        }
        else
        {
            return;
        }
    }
    QMainWindow::repaint();
    sleep(1);
    if(chess == 0)
       {
        //move_chess();
        man_machine_rules();
        qDebug()<<"**********";
        if(player_signal == 1)
        {
            chess = 1;
            qDebug()<< chess << ":黑棋手";
            ui->button_white->setStyleSheet("");
            ui->button_black->setStyleSheet(tr("background-image: url(:/image/black.png);"));
        }
        else
        {
            qDebug()<< chess <<  ":白棋手";
        }
    }
    declare_winner();
    ui->lineEdit_black->setText(QString::number(num_black));//显示黑棋子数量
    ui->lineEdit_white->setText(QString::number(num_white));//显示白棋子数量
}
