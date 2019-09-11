#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <unistd.h>
#include <QPixmap>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include "link.h"
#include <QTimer>
#include <QPalette>
#include <QSlider>
#include "mylabel.h"

using namespace std;


extern LRC *link_insert(LRC *head,LRC *p_new);
extern void link_print(LRC *head);
extern LRC *link_find(LRC *head,int m_time);

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    char *buf[128],*buf1[128];
    DIR *dirp;
    struct dirent *mydir;
    char song_path_name[128];
    QLabel *label[8];
    QTimer *my_time;
    MyLabel *label1;
    pid_t pid;

    int len,temp;//歌词行数
    char *lrc_buf;//保存歌词
    char *q[100];//切割后的歌词
    LRC *head,*pb;
    char *p1[32];//歌曲信息
    int total_time;//歌曲总时间

    void start_player();//播放音乐
    void write_myfifo_back(char *buf[128]);//将歌名+路径写入管道文件（上一曲）
    void write_myfifo_front(char *buf[128]);//将歌名+路径写入管道文件（下一曲）
    void label_creat();//创建label并将歌名放上去,并且设置一个大label将小label框起来
    char *read_text();//读取文本里面的内容
    void open_read_dir();//打开并读取指定目录
    int msg_deal(char *msg_src,char *msg_done[],char *str);//歌词切割
    LRC  *link_creat(int len,char *q[]);//创建链表并给成员赋值
    void song_information(char *q[]);//歌曲信息
    void initial();////初始化label、temp、进度条


    signals:
        void mylabel_signal(int a,int b);
private slots:
    void on_button_back_clicked();//上一曲槽函数
    void on_button_front_clicked();//下一曲槽函数
    void on_button_pause_clicked();//暂停槽函数
    void on_pushButton_clicked();//退出槽函数
    void lyrics_swith();//歌词切换槽函数
    void on_button_forward_clicked();//歌曲快进槽函数
    void on_button_backward_clicked();//歌曲快退槽函数
    void on_button_volume_clicked();//音量开关槽函数
    void mylabel_signal_slot(int a,int b);//点击放歌的槽函数
    void on_slider_progress_sliderReleased();//进度条快进、快退

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
