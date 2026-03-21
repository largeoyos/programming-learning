#ifndef SQLIST_H
#define SQLIST_H

#include <stdio.h>
#include <string.h>
#include <iostream>


#define LIST_INIT_SIZE 100
#define LIST_INC_SIZE 20

template <typename T>
struct SqList{
    T *elem;
    int listsize;
    int length;
    int incrementsize;
};//顺序表

template <typename T>
void InitList_Sq(SqList<T> &L,int msize=LIST_INIT_SIZE);   //初始化顺序表
template <typename T>
void DestroyList_Sq(SqList<T> &L);                         //销毁顺序表
template <typename T>
bool ListEmpty_Sq(SqList<T> L);                            //判断顺序表是否为空
template <typename T>
bool ListFull_Sq(SqList<T> L);                             //判断顺序表是否已满
template <typename T>
int ListLength_Sq(SqList<T> L);                            //求顺序表长度
template <typename T>
int LocateItem_Sq(SqList<T> L,T e);                        //按值查找
template <typename T>
int GetItem_Sq(SqList<T> L,int i,T &e);                    //按位查找
void ErrorMsg(const char *msg);                            //错误信息提示
template <typename T>
void ListInsert_Sq(SqList<T> &L,int i,T e);                //插入元素
template <typename T>
void ListDelete_Sq(SqList<T> &L,int i,T &e);               //删除元素

#include "sqList.cpp"
#endif // SQLIST_H

