#ifndef LINKSTACK_H
#define LINKSTACK_H
#include "SqStack.h"
template <typename T>
struct LinkStack{
    LinkList<T> list;
};

template <typename T>
void InitStack_L(LinkStack<T> &S);
template <typename T>
void DestroyStack_L(LinkStack<T> &S);
template <typename T>
bool GetTop_L(LinkStack<T> S,T &e);
template <typename T>
void Push_L(LinkStack<T> &S,T e);
template <typename T>
bool Pop_L(LinkStack<T> &S,T &e);

#include "LinkStack.cpp"
#endif // LINKSTACK_H
