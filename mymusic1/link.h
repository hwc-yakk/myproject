#ifndef LINK_H
#define LINK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

typedef  struct lrc{
    int m_time;
    char lrc_buf[200];
    struct  lrc *front;
    struct  lrc *next;
}LRC;

void link_print(LRC *head);//遍历
LRC *link_find(LRC *head,int m_time);//查找
LRC *link_insert(LRC *head,LRC *p_new);//插入

#endif // LINK_H
