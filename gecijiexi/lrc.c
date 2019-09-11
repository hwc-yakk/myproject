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
	while(pb->next != head){
		if(pb->m_time == m_time)
			return pb;
		pb = pb->next;		
	}	
	if(pb->m_time == m_time)
		return pb;	
	else
		return NULL;
}
// LRC *link_delete(LRC *head,int m_time)
// {
	// LRC *p = head,*pb;
	// if(head == NULL)
		// return NULL;
	// while(p->next != head){
		// if(m_time == p->m_time){//找出要删除的节点
			// if(p == head)//被删除的是头节点
				// head = head->next;		
			// break;
		// }
		// pb = p;
		// p = p->next;		
	// }
	// if(p->m_time == m_time){//头、中间、结尾都需要交换
		// p->next->front = p->front;
		// p->front->next = p->next;	
		// free(p);
		// if(p == head)//只有一个节点的情况
			// return NULL;
		// else
			// return head;
	// }
	// return head;
// }
//带创建的插入接口
#if 0
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
#else
//以学号升序插入
LRC *link_insert(LRC *head,LRC *p_new)
{
	LRC *pb = head;
	if(head == NULL){
		head = p_new;
		head->next = head;
		head->front = head;
	}else{
		while(pb->next != head){
			if(p_new->m_time < pb->m_time)
				break;
			pb = pb->next;		
		}
		if(p_new->m_time >= pb->m_time)
			pb = pb->next;
		p_new->next = pb;
		p_new->front = pb->front;
		pb->front->next = p_new;
		pb->front = p_new;
		if(p_new->m_time < head->m_time)
			head = p_new;
	}
	return head;
}
#endif
LRC *link_order(LRC *head)
{
	LRC temp,*pf=head,*pb;
	if(head == NULL)
		return head;
	while(pf->next != head){
		pb = pf->next;
		while(pb != head){
			if(pf->m_time > pb->m_time){
				//交换
				temp = *pf;
				*pf = *pb;
				*pb = temp;
				//temp.next = pb->next;
				pb->next = pf->next;
				pf->next = temp.next;
				//temp.front = pb->front;
				pb->front = pf->front;
				pf->front = temp.front;				
			}
			pb=pb->next;
		}
		pf = pf->next;
	}
	return head;
}
