#pragma once
#include "LinkQueue.h"

using CQueuePtr=LinkList;
struct CQueue{
    CQueuePtr rear;
};

void InitQueue_C(CQueue &Q){
    Q.rear=new LNode;
    if(!Q.rear){
        return;
    }
    Q.rear->next=Q.rear;
}


void EnQueue_C(CQueue& Q,int e){
    LinkList p=new LNode;
    p->data=e;
    p->next=Q.rear->next;
    Q.rear->next=p;
    Q.rear=p;
}

bool DeQueue_C(CQueue& Q,int &e){
    if(Q.rear->next==Q.rear){
        return 0;
    }
    LinkList p=Q.rear->next->next;
    Q.rear->next->next=p->next;
    e=p->data;
    if(p==Q.rear){
        Q.rear=Q.rear->next;
    }
    delete p;
    return 1;
}

//debug记录:这里生成的队列有一个头结点,出队列的时候需要跳过这个头结点才可以


struct C_Sq_Queue{
    int * elem;
    int queuesize;
    int rear;
    int length;
};
//满的条件:Q.length==Q.queuesize
void Enqueue_CSq(C_Sq_Queue &Q,int e){
    if(Q.length==Q.queuesize){
        return ;
    }
    Q.elem[Q.rear]=e;
    Q.rear=(Q.rear+1)%Q.queuesize;
    Q.length++;
}

bool DeQueue_CSq(C_Sq_Queue &Q,int &e){
    if(Q.length==0){
        return 0;
    }
    e=Q.elem[(Q.rear+(Q.queuesize-Q.length))%Q.queuesize];
    Q.length--;
    return 1;
}
//作业3.11


