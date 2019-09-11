#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();
     QLabel *label[8][8];
     int Q[8][8];
     QPixmap pix1,pix2;

     void rules_black(int a,int b);//白棋子规则
     void rules_white(int a,int b);//黑棋子规则
     void declare_winner();//计算白棋子和黑棋子数量并宣布获胜者
     int reason_black();//判断是否还有合理的格子落子(黑)
     int reason_white();//判断是否还有合理的格子落子(白)
     void rules1(int a,int b);//判断白棋子落子是否有合理
     void rules2(int a,int b);//判断黑棋子落子是否有合理
     void move_chess();//判断还能否有合理的位置落子
     void man_machine_rules();//找出机器人的落子位置并落子
     void initial();//棋盘初始化

    signals:
        void mysignal(void);
        void mylabel_signal(int a,int b);
private slots:
        void mylabel_signal_slot(int a,int b);//人人对战
        void man_machine_mylabel_signal_slot(int a,int b);//人机对战
private:
    Ui::GameWindow *ui;
};

#endif // GAMEWINDOW_H
