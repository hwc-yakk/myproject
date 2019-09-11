#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "console.h"
#include "start_mplayer.h"

int key = 0;

typedef  struct lrc{
	int m_time;
	char lrc_buf[200];
	struct lrc *front;
	struct lrc *next;
}LRC;

extern LRC *link_insert(LRC *head,LRC *p_new);
extern void link_print(LRC *head);
extern LRC *link_find(LRC *head,int m_time);

//读取文本里面的内容
char *haha(void)
{
	FILE *fp;
	char *pnew;
	int len,num;
	
	fp=fopen("简单爱.lrc","r");
	if(fp == NULL)
	{
		printf("打开错误\n");
		exit(0);
	}
	
	fseek(fp,0,SEEK_END);
	len=ftell(fp);
	//printf("len=%d\n",len);
	rewind(fp);
	pnew=malloc((len+1));
	num=fread(pnew,1,len,fp);
	pnew[num]='\0';
	//printf("%s\n",pnew);
	
	fclose(fp);
	return pnew;
}
//链表的创建和成员的赋值
LRC  *link_creat(int len,char *q[])
{
	int k,j,i=0,min,sec;
	int time_buf[5];
	char *str;
	//char *q[100];
	LRC *head=NULL,*p_new;
	//printf("%d\n",len);
	for(k=4;k<len;k++)//处理第5行到最后一行
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
			p_new = malloc(sizeof(LRC));
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
//歌词切割
int msg_deal(char *msg_src,char *msg_done[],char *str)
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
//背景颜色和前景颜色
void background_color()
{
	int i;
	set_fg_color(COLOR_BLUE);//设置字体颜色为蓝色
	set_bg_color(COLOR_WHITE);//设置背景颜色为白色
	for(i=1;i<=12;i++)
	{
		cusor_moveto(7, i);//光标移到 第1行，第5列
		printf("%70s","");
	}
}
//播放菜单
void play_menu()
{
	mplayer_play("lrc.mp3");
	clear_screen();//清屏幕内容
	background_color();
	cusor_moveto(35, 1);
	printf("歌名\t简单爱\n");
	cusor_moveto(35, 2);
	printf("歌手\t周杰伦\n");
	cusor_moveto(35, 3);
	printf("专辑\t范特西\n");
	cusor_moveto(35, 4);
	printf("制作\t大脸猫\n");
	cusor_moveto(30, 12);
	printf("1.下一曲\t2.暂停\t3.上一曲");

}
//播放歌词
void play_lyrics(LRC *pb)
{
	cusor_moveto(7, 8);
	printf("%20d\t%-45s\n",pb->front->m_time,pb->front->lrc_buf);
	set_fg_color(COLOR_RED);
		
	cusor_moveto(7, 9);
	printf("%20d\t%-45s\n",pb->m_time,pb->lrc_buf);
	set_fg_color(COLOR_BLUE);
	
	cusor_moveto(7, 10);
	printf("%20d\t%-45s\n",pb->next->m_time,pb->next->lrc_buf);

}
//添加线程
void *my_fun(void *data)
{
	while(1){
		//printf("key = %d\n",key++);
		//sleep(1);
	}
	return NULL;
}

int main()
{
	int temp=0;
	char *lrc_buf;
	LRC *head=NULL,*pb;
	char *q[100];
	int len=0;
	
	lrc_buf=haha();
	len=msg_deal(lrc_buf,q,"\r\n");
	head=link_creat(len,q);
	play_menu();
	pthread_t tid;
	pthread_create(&tid,NULL,my_fun,NULL);
	while(1)
	{
		sleep(1);
		temp++;
		cusor_hide();//隐藏光标
		cusor_moveto(39, 6);
		printf("%02d:%02d\n",temp/60,temp%60);
		pb=link_find(head,temp);
		// 按temp查找节点 调link_search_time 
		if(pb != NULL)
		play_lyrics(pb);
		//printf("%20d\t%-50s\n",pb->m_time,pb->lrc_buf);
		// 找到打印
		if(temp == 260)
		{
			cusor_show();//显示光标
			exit(0);
		}
	}
	// link_print(head);
	return 0;
}