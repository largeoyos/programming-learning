#ifndef STACK_H
#define STACK_H

#include "LinkList.h"

#define STACK_INIT_SIZE 100
template <typename T>
struct SqStack{
    T *elem;
    int top;
    int stacksize;
};
template <typename T>
void InitStack_Sq(SqStack<T> &S,int msize=STACK_INIT_SIZE);   //初始化顺序栈
template <typename T>
void DestroyStack_Sq(SqStack<T> &S);                         //销毁顺序栈
template <typename T>
void ClearStack_Sq(SqStack<T> &S);                           //清空顺序栈
template <typename T>
int StackLength_Sq(SqStack<T> S);                            //求顺序栈长度
template <typename T>
bool StackEmpty_Sq(SqStack<T> S);                            //判断顺序栈是否为空  
template <typename T>
bool GetTop_Sq(SqStack<T> S,T &e);                      //取栈顶元素
template <typename T>
void Push_Sq(SqStack<T> &S,T e);              //入栈
template <typename T>
bool Pop_Sq(SqStack<T> &S,T &e);             //出栈
template <typename T>
void StackTraverse_Sq(SqStack<T> S);          //遍历顺序栈

#endif // STACK_H

