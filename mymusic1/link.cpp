#include "link.h"


void link_print(LRC *head)
{
    LRC *pb = head;
    if(head == NULL)
        return;
    printf("时间\t歌词\n");
    while(pb->next != head){
        printf("%d\t%s\n",pb->m_time,pb->lrc_buf);
        pb = pb->next;
    }
    printf("%d\t%s\n",pb->m_time,pb->lrc_buf);
}

LRC *link_find(LRC *head,int m_time)
{
    LRC *pb = head;
    if(head == NULL)
        return NULL;
    while(pb->next != head)
    {
        if(pb->m_time == m_time)
        return pb;
        pb = pb->next;
    }
    if(pb->m_time == m_time)
        return pb;
    else
        return NULL;
}

LRC *link_insert(LRC *head,LRC *p_new)
{
    if(head == NULL){
        head = p_new;
        head->next = head;
        head->front = head;
    }else{
        head->next->front = p_new;
        p_new->next = head->next;
        head->next = p_new;
        p_new->front = head;
    }
    return head;
}
