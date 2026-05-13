#include "BiTreeStack.h"
bool InitStack_Bi(BiStack &S){
    S.top=0;//指向下一个待写入位置
    return true;
}
bool StackEmpty_Bi(BiStack S){
    return S.top==0;

}
bool StackFull_Bi(BiStack S){
    return S.top==STACK_MAXSIZE;
}
bool Push_Bi(BiStack &S,BiTree e){
    if(StackFull_Bi(S))return false;
    S.data[S.top]=e;
    S.top++;
    return true;

}
bool Pop_Bi(BiStack &S, BiTree &e){
    if(StackEmpty_Bi(S))return false;
    S.top--;
    e=S.data[S.top];
    return true;

}
bool GetTop_Bi(BiStack S, BiTree &e){
    if(StackEmpty_Bi(S))return false;
    e=S.data[S.top-1];
    return true;
}
