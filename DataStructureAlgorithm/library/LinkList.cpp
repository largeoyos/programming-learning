#include "LinkList.h"
template <typename T>

void InitList_HL(LinkList<T> &L){
    L=new LNode<T>;
    L->next=NULL;
}//初始化带头结点的链表

template <typename T>
void InitList_L(LinkList<T> &L){
    L=NULL;
}//初始化不带头结点的链表
template <typename T>
int ListLength_L(LinkList<T> L){
    int count=0;
    LinkList<T> p=L;
    while(p){
        count++;
        p=p->next;
    }
    return count;
}//求链表长度
template <typename T>
LNode<T> * LocateItem_L(LinkList<T> L,T e){
    LinkList<T> p=L;
    while(p&&p->data!=e){
        p=p->next;
    }
    return p;
}//按值查找
template <typename T>
void ListInsertHead_L(LinkList<T> &L,LNode<T> *p,LNode<T> *s){
    if(p==NULL){
        s->next=L;
        L=s;
    }
    else {
        LinkList<T> q=L;
        while(q&&q->next!=p){
            q=q->next;
        }
        if(q){
            q->next=s;
            s->next=p;
        }
        else{
            ErrorMsg("p不是链表中的结点\n");
        }
    }
}//在p结点之前插入s结点

template <typename T>
void ListInsertTail_L(LinkList<T> &L,LNode<T> *p,LNode<T> *s){
    s->next=p->next;
    p->next=s;
}//在p结点之后插入s结点

template <typename T>
void ListDelete_L(LinkList<T> &L,LNode<T> *p,T &e){
    if(p==L){
        L=p->next;
    }
    else {
        LinkList<T> q=L;
        while(q&&q->next!=p){
            q=q->next;
        }
        if(q){
            q->next=p->next;
        }
        else{
            ErrorMsg("p不是链表中的结点\n");
        }
        e=p->data;
        delete p;
    }
}//删除p结点

template <typename T>
void Union_L(LinkList<T> &La,LinkList<T> Lb){
    if(La==NULL){
        La=Lb;
        return;
    }
    while(Lb){
        LinkList<T> s=Lb;
        Lb=Lb->next;
        LinkList<T> p=La;
        LinkList<T> pre=NULL;
        while(p&&p->data!=s->data){
            pre=p;
            p=p->next;
        }
        if(p){
            delete s;
        }
        else{
            pre->next=s;
            s->next=NULL;
        }
    }
}//求La和Lb的并集，结果存储在La中，Lb不变

template <typename T>
void Union1_L(LinkList<T> &La,LinkList<T> Lb){
    LinkList<T> pa=La;
    while(Lb){
        LinkList<T> s=Lb;
        Lb=Lb->next;
        LinkList<T> p=pa;
        while(p&&p->data!=s->data){
            p=p->next;
        }
        if(p){
            delete s;
        }
        else{
            s->next=La;
            La=s;
        }
    }
}//求La和Lb的并集，结果存储在La中，Lb不变，算法2

