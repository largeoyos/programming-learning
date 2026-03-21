#include "SqStack.h"
template <typename T>
void InitStack_Sq(SqStack<T> &S,int msize){
    S.elem=new T[msize];
    S.top=-1;
    S.stacksize=msize;
}//初始化顺序栈

template <typename T>
void DestroyStack_Sq(SqStack<T> &S){
    delete []S.elem;
    S.top=-1;
    S.stacksize=0;
}//销毁顺序栈

template <typename T>
bool GetTop_Sq(SqStack<T> S,T &e){
    if(S.top==-1) return 0;
    e=S.elem[S.top];
    return 1;
}//取栈顶元素

template <typename T>
void Increment(SqStack<T> &S){
    T *newbase=new T[S.stacksize+STACK_INIT_SIZE];
    if(!newbase){
        ErrorMsg("内存分配失败\n");
        return;
    }
    memcpy(newbase,S.elem,S.stacksize*sizeof(T));
    delete []S.elem;
    S.elem=newbase;
    S.stacksize+=STACK_INIT_SIZE;
}//增加顺序栈的存储空间

template <typename T>
void Push_Sq(SqStack<T> &S,T e){
    if(S.top==S.stacksize-1){
        Increment(S);
    }
    S.top++;
    S.elem[S.top]=e;
}//入栈

template <typename T>
bool Pop_Sq(SqStack<T> &S,T &e){
    if(S.top==-1){
        return 0;
    }
    e=S.elem[S.top];
    S.top--;
    return 1;
}//出栈

template <typename T>
void StackTraverse_Sq(SqStack<T> S){
    for(int i=S.top;i>=0;i--){
        std::cout<<S.elem[i]<<" ";
    }
    std::cout<<std::endl;
}//遍历顺序栈

template <typename T>
void ClearStack_Sq(SqStack<T> &S){
    S.top=-1;
}//清空顺序栈

template <typename T>

bool StackEmpty_Sq(SqStack<T> S){
    return S.top==-1;
}//判断顺序栈是否为空

template <typename T>

int StackLength_Sq(SqStack<T> S){
    return S.top+1;
}//求顺序栈长度