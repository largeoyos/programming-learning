#pragma once
typedef int LinkListElemType;
//1.单链表
typedef struct LNode{
    LinkListElemType data;
    struct LNode *next;
}LNode, *LinkList;
//2.循环链表
typedef LNode CLNode;
typedef LNode *CLinkList;

//带头结点单链表
bool InitList_H(LinkList &L);
bool ListInsert_H(LinkList &L,int i,LinkListElemType e);
bool ListDelete_H(LinkList &L, int i, LinkListElemType &e);
LNode *GetElem_H(LinkList L,int i);
LNode *LocateElem_H(LinkList L, LinkListElemType e);
void PrintList_H(LinkList L);
void DestroyList_H(LinkList &L);


//无头结点单链表
bool InitList(LinkList &L);
//循环链表
bool InitCircularList(LinkList &L);
