#pragma once
#include "BiTree.h"
#define STACK_MAXSIZE 100

typedef struct{
    BiTree data[STACK_MAXSIZE];
    int top;
}BiStack;

bool InitStack_Bi(BiStack &S);
bool StackEmpty_Bi(BiStack S);
bool StackFull_Bi(BiStack S);
bool Push_Bi(BiStack &S,BiTree e);
bool Pop_Bi(BiStack &S, BiTree &e);
bool GetTop_Bi(BiStack S, BiTree &e);
