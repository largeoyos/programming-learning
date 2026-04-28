#pragma once
#define MaxSize 50

typedef int StackElemType;
typedef struct{
    StackElemType data[MaxSize];
    int top;
} SqStack;

void InitStack(SqStack&S);
bool StackEmpty(SqStack S);
bool Push(SqStack &S,StackElemType x);
bool Pop(SqStack &S, StackElemType &x);
bool GetTop(SqStack S, int &x);

