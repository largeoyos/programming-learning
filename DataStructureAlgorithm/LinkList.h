#ifndef LINKLIST_H
#define LINKLIST_H

#include <stdio.h>
#include <string.h>
#include "sqList.h"

template <typename T>
struct LNode{
    T data;
    LNode<T> *next;
};//链表结点类型定义

template <typename T>
using LinkList = LNode<T>*;
//写LNode<T> *p;等价于LNode<T> *p;
//写LinkList<T> p;等价于LNode<T> *p;


template <typename T>
void InitList_HL(LinkList<T> &L);
template <typename T>
void InitList_L(LinkList<T> &L);
template <typename T>
int ListLength_L(LinkList<T> L);
template <typename T>
LNode<T> * LocateItem_L(LinkList<T> L,T e);
template <typename T>
void ListInsertHead_L(LinkList<T> &L,LNode<T> *p,LNode<T> *s);
template <typename T>
void ListInsertTail_L(LinkList<T> &L,LNode<T> *p,LNode<T> *s);
template <typename T>
void ListDelete_L(LinkList<T> &L,LNode<T> *p,T &e);
template <typename T>
void Union_L(LinkList<T> &La,LinkList<T> Lb);
template <typename T>
void Union1_L(LinkList<T> &La,LinkList<T> Lb);




#endif // LINKLIST_H

