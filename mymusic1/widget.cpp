#include "widget.h"
#include "ui_widget.h"

int i = 0;//存歌曲名的顺序
int j = 0;//播放音乐的顺序
int k = 0;//存歌词顺序
int pause_signal = 0;//暂停信号
int volume_signal = 0;//音量信号

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //设置窗口标题
    this->setWindowTitle("QQ音乐");

    open_read_dir();
    //read_text();
    label_creat();
    start_player();

}

Widget::~Widget()
{
    delete ui;
}
//播放音乐
void Widget::start_player()
{


    if((pid = fork()) < 0)
    {
        perror("fork");
    }
    else if(pid == 0)//子进程
    {
        execlp("mplayer",
                     " mplayer ",
                     "-slave", "-quiet","-idle",
                     "-input", "file=./myfifo",
                     "./song/简单爱.mp3", NULL);
    }
    else //父进程
    {
        //给按钮设置图片
        QPixmap pix1;
        pix1.load("./button_style/front.png");
        ui->button_front->setAutoFillBackground(true);
        ui->button_front->setIcon(QIcon(pix1));
        ui->button_front->setIconSize(QSize(50,50));
        QPixmap pix2;
        pix2.load("./button_style/play.png");
        ui->button_pause->setAutoFillBackground(true);
        ui->button_pause->setIcon(QIcon(pix2));
        ui->button_pause->setIconSize(QSize(50,50));
        QPixmap pix3;
        pix3.load("./button_style/back.png");
        ui->button_back->setAutoFillBackground(true);
        ui->button_back->setIcon(QIcon(pix3));
        ui->button_back->setIconSize(QSize(50,50));
        QPixmap pix4;
        pix4.load("./button_style/backward.png");
        ui->button_backward->setAutoFillBackground(true);
        ui->button_backward->setIcon(QIcon(pix4));
        ui->button_backward->setIconSize(QSize(50,50));
        QPixmap pix5;
        pix5.load("./button_style/forward.png");
        ui->button_forward->setAutoFillBackground(true);
        ui->button_forward->setIcon(QIcon(pix5));
        ui->button_forward->setIconSize(QSize(50,50));
        QPixmap pix6;
        pix6.load("./button_style/mute.png");
        ui->button_volume->setAutoFillBackground(true);
        ui->button_volume->setIcon(QIcon(pix6));
        ui->button_volume->setIconSize(QSize(50,50));

        //设置界面背景图
        QPixmap pix7;
        QPalette pal = this->palette();
        pix7.load("./button_style/music.jpeg");
        pix7 = pix7.scaled(800, 450, Qt::KeepAspectRatio);
        pal.setBrush(QPalette::Background, QBrush(pix7));
        setPalette(pal);

        temp = 0;
        my_time=new QTimer();
        my_time->start(1000);
        connect(my_time,SIGNAL(timeout()),this,SLOT(lyrics_swith()));
        connect(label1,SIGNAL(mylabel_signal(int,int)),this,SLOT(mylabel_signal_slot(int,int)));
        initial();

   }
}
//将歌名+路径写入管道文件（上一曲）
void Widget::write_myfifo_back(char *buf[128])
{
    int fd;
    if(j == 0)
    {
        j = 8;
    }
    j--;
    //qDebug() << "buf =" << buf[j];
    sprintf(song_path_name, "loadfile ./song/%s\n", buf[j]);
    //qDebug() << "j = " << j << song_path_name ;
    if((fd = open("myfifo",O_RDWR)) == -1)
    {
        perror("fail to open");
        exit(1);
    }
    write(fd,song_path_name,strlen(song_path_name));
}
//将歌名+路径写入管道文件（下一曲）
void Widget::write_myfifo_front(char *buf[128])
{
    int fd;
    j++;
    if(j > 7)
    {
        j = 0;
    }
    //qDebug() << "buf =" << buf[j];
    sprintf(song_path_name, "loadfile ./song/%s\n", buf[j]);
    //qDebug() << "j = " << j << song_path_name ;
    if((fd = open("myfifo",O_RDWR)) == -1)
    {
        perror("fail to open");
        exit(1);
    }
    write(fd,song_path_name,strlen(song_path_name));
}
//创建label并将歌名放上去,并且设置一个大label将小label框起来
void Widget::label_creat()
{
    for(int i;i<8;i++)
    {
        //qDebug() << "222";
        label[i] = new QLabel(this);
        label[i]->setFixedSize(150,30);
        label[i]->move(100,50+30*i);
        label[i]->setText(buf[i]);
        label[i]->setStyleSheet("color:white;");
    }
    //设置大label
    label1 = new MyLabel(this);
    label1->setFixedSize(150,240);
    label1->move(100,50);
}
//读取文本里面的内容
char *Widget::read_text()
{   
    FILE *fp;
    char *pnew;       
    int len,num;
    char str[32];

    //qDebug() << "buf1 = " << buf1[1];
    //qDebug() << "歌曲" << j;
    sprintf(str,"./lyrics/%s",buf1[j]);
    //qDebug() << str;
    fp = fopen(str,"r");
    if(fp == NULL)
    {
        printf("打开错误\n");
        exit(0);
    }

    fseek(fp,0,SEEK_END);
    len=ftell(fp);
    //printf("len=%d\n",len);
    rewind(fp);
    pnew=(char *)malloc((len+1));
    num=fread(pnew,1,len,fp);
    pnew[num]='\0';
    //printf("%s\n",pnew);

    fclose(fp);
    return pnew;
}
//打开并读取指定目录
void Widget::open_read_dir()
{
    //打开指定目录(歌名)
    dirp = opendir("./song");
    //读取目录中的文件
    while((mydir = readdir(dirp)) != NULL)
    {
        if(mydir->d_name[0] == '.')
        {
            continue;
        }
        //qDebug()<< "inode = " << mydir->d_ino << "filename = " << mydir->d_name ;
        buf[i] = (char *)malloc(strlen(mydir->d_name));
        strcpy(buf[i],mydir->d_name);
        //qDebug() << "buf =" << buf[i];
        i++;
    }
#if 0
    //打开指定目录(歌词)
    dirp1 = opendir("./lyrics");
    //读取目录中的文件
    while((mydir1 = readdir(dirp1)) != NULL)
    {

        if(mydir1->d_name[0] == '.')
        {
            continue;
        }
        //qDebug()<< "inode = " << mydir1->d_ino << "filename = " << mydir1->d_name ;
        buf1[k] = (char *)malloc(strlen(mydir1->d_name));
        strcpy(buf1[k],mydir1->d_name);
        //qDebug() << "buf1 =" << buf1[k];
        k++;
    }
#endif
    buf1[0] = "简单爱.lrc";
    buf1[1] = "董小姐.lrc";
    buf1[2] = "Diamonds.lrc";
    buf1[3] = "StopLove.lrc";
    buf1[4] = "后来的我们.lrc";
    buf1[5] = "爱上你很快乐.lrc";
    buf1[6] = "安和桥.lrc";
    buf1[7] = "爱转角.lrc";
}
//歌词切割
int Widget::msg_deal(char *msg_src, char *msg_done[], char *str)
{
    int i=0;
    msg_done[i]=strtok(msg_src,str);
    while(msg_done[i]!=NULL)
    {
        i++;
        msg_done[i]=strtok(NULL,str);
    }
    return i;
}
//创建链表并给成员赋值
LRC *Widget::link_creat(int len, char *q[])
{

    int k,j,i=0,min,sec;
    int time_buf[5];
    char *str;
    //char *q[100];
    LRC *head=NULL,*p_new;
    //printf("%d\n",len);
    for(k=3;k<len;k++)//处理第4行到最后一行
    {
        str=q[k];
        i=0;
            //printf("%d\n",len);
        while(*str=='[')
        {
            min=atoi(str+1);
            sec=atoi(str+4);
            time_buf[i++]=min*60+sec;
            str=str+10;
        }
        for(j=0;j<i;j++)
        {
            p_new = (LRC *)malloc(sizeof(LRC));
            //申请节点
            p_new->m_time=time_buf[j];
            //节点的时间成员赋值
            strcpy(p_new->lrc_buf,str);
            //节点的歌词成员赋值
            head = link_insert(head,p_new);
            //插入链表
        }
    }
    return head;
}
//歌曲信息
void Widget::song_information(char *q[])
{
    int k;
    char *str;
    char *p = "]";

    lrc_buf = read_text();
    len = msg_deal(lrc_buf,q,"\r\n");

    for(k=0;k<3;k++)
    {
        //qDebug() << "***" << q[k] ;
        str=q[k];
        while(*str=='[')
        {
            str=str+4;
            //qDebug() << str ;
            p1[k] = strtok(str,p);
            //qDebug() << p1[k];
        }
    }
}
//初始化label、temp、进度条
void Widget::initial()
{
    temp = 0;
    //初始化歌词
    ui->label_1->clear();
    ui->label_2->clear();
    ui->label_3->clear();
    //初始化进度条
    lrc_buf = read_text();
    len = msg_deal(lrc_buf,q,"\r\n");
    head=link_creat(len,q);
    total_time = head->next->m_time + 15;
    //qDebug() << "head = " << head->next->m_time;
    ui->slider_progress->setMaximum(total_time);

}
//歌词切换槽函数
void Widget::lyrics_swith()
{
    lrc_buf = read_text();
    len = msg_deal(lrc_buf,q,"\r\n");
    head=link_creat(len,q);
    pb = link_find(head,temp);
    char time_bar[128];

    //qDebug() << "head = " << head->m_time;
    song_information(q);
    string str1=string("歌曲名:")+string(p1[0]);
    ui->label_ti->setText(str1.c_str());
    ui->label_ti->setStyleSheet("color:blue;");
    string str2=string("歌手名:")+string(p1[1]);
    ui->label_ar->setText(str2.c_str());
    ui->label_ar->setStyleSheet("color:blue;");
    string str3=string("专辑名:")+string(p1[2]);
    ui->label_al->setText(str3.c_str());
    ui->label_al->setStyleSheet("color:blue;");

    sprintf(time_bar,"%02d:%02d",temp/60,temp%60);
    ui->label_time->setText(time_bar);
    ui->label_time->setStyleSheet("color:white;");
    if(pb != NULL)
    {
        ui->label_1->clear();
        ui->label_2->clear();
        ui->label_3->clear();
        ui->label_3->setText(pb->front->lrc_buf);
        ui->label_3->setStyleSheet("color:white;");
        ui->label_2->setText(pb->lrc_buf);
        ui->label_2->setStyleSheet("color:red;");
        ui->label_1->setText(pb->next->lrc_buf);
        ui->label_1->setStyleSheet("color:white;");
    }

    ui->slider_progress->setValue(temp);
    temp ++;
    if(temp == total_time)
    {
        on_button_front_clicked();
    }

}
//上一曲槽函数
void Widget::on_button_back_clicked()
{
    write_myfifo_back(buf);
    initial();
}
//下一曲槽函数
void Widget::on_button_front_clicked()
{
    write_myfifo_front(buf);
    initial();
}
//暂停槽函数
void Widget::on_button_pause_clicked()
{
    int fd;
    if((fd = open("myfifo",O_RDWR)) == -1)
    {
        perror("fail to open");
    }
    if(pause_signal == 0)
    {
        QPixmap pix1;
        pix1.load("./button_style/pause.png");
        ui->button_pause->setAutoFillBackground(true);
        ui->button_pause->setIcon(QIcon(pix1));
        ui->button_pause->setIconSize(QSize(50,50));

        my_time->stop();
        write(fd,"pause\n",6);
        pause_signal = 1;
    }
    else if(pause_signal == 1)
    {
        QPixmap pix2;
        pix2.load("./button_style/play.png");
        ui->button_pause->setAutoFillBackground(true);
        ui->button_pause->setIcon(QIcon(pix2));
        ui->button_pause->setIconSize(QSize(50,50));

        my_time->start();
        write(fd,"pause\n",6);
        pause_signal = 0;
    }
}
//退出槽函数
void Widget::on_pushButton_clicked()
{
    char str[128];
    //qDebug() << "pid = " << pid;
    sprintf(str,"kill -9 %d\n",pid);
    system(str);
    exit(0);
}
//歌曲快进槽函数
void Widget::on_button_forward_clicked()
{
    int fd;
    if((fd = open("myfifo",O_RDWR)) == -1)
    {
        perror("fail to open");
        exit(1);
    }
    write(fd,"seek 15\n",8);
    temp = temp + 15;
}
//歌曲快退槽函数
void Widget::on_button_backward_clicked()
{
    int fd;
    if((fd = open("myfifo",O_RDWR)) == -1)
    {
        perror("fail to open");
        exit(1);
    }
    write(fd,"seek -15\n",9);
    temp = temp - 15;
}
//音量开关槽函数
void Widget::on_button_volume_clicked()
{

    int fd;
    if((fd = open("myfifo",O_RDWR)) == -1)
    {
        perror("fail to open");
        exit(1);
    }
    if(volume_signal == 0)
    {
        QPixmap pix1;
        pix1.load("./button_style/muteoff.png");
        ui->button_volume->setAutoFillBackground(true);
        ui->button_volume->setIcon(QIcon(pix1));
        ui->button_volume->setIconSize(QSize(50,50));
        write(fd,"mute 1\n",7);
        volume_signal = 1;
    }
    else if(volume_signal == 1)
    {
        QPixmap pix2;
        pix2.load("./button_style/mute.png");
        ui->button_volume->setAutoFillBackground(true);
        ui->button_volume->setIcon(QIcon(pix2));
        ui->button_volume->setIconSize(QSize(50,50));
        write(fd,"mute 0\n",7);
        volume_signal = 0;
    }
}
//点击放歌槽函数
void Widget::mylabel_signal_slot(int a, int b)
{
    temp = 0;
    j=b;
    int fd;
    //qDebug() << "buf =" << buf[j];
    sprintf(song_path_name, "loadfile ./song/%s\n", buf[j]);
    //qDebug() << "j = " << j << song_path_name ;
    if((fd = open("myfifo",O_RDWR)) == -1)
    {
        perror("fail to open");
        exit(1);
    }
    write(fd,song_path_name,strlen(song_path_name));
    initial();
}
//进度条快进、快退
void Widget::on_slider_progress_sliderReleased()
{
    int t_time;
    int fd;
    char str[128];
    int value = ui->slider_progress->value();
    t_time = value - temp;

    if((fd = open("myfifo",O_RDWR)) == -1)
    {
        perror("fail to open");
        exit(1);
    }
    sprintf(str,"seek %d\n",t_time);
    qDebug() << "t_time = " << t_time << "str = " << str;
    write(fd,str,strlen(str));

    temp = value;
}
