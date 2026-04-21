#include "LinkStack.h"
template <typename T>
void InitStack_L(LinkStack<T> &S){
    S.list=NULL;
}//初始化链栈

template <typename T>
void DestroyStack_L(LinkStack<T> &S){
    while(S.list){
        LinkList<T> p=S.list;
        S.list=S.list->next;
        delete p;
    }
}//销毁链栈

template <typename T>
bool GetTop_L(LinkStack<T> S,T &e){
    if(S.list==NULL) return 0;
    e=S.list->data;
    return 1;
}//取栈顶元素

template <typename T>
void Push_L(LinkStack<T> &S,T e){
    LinkList<T> p=new LNode<T>;
    p->data=e;
    p->next=S.list;
    S.list=p;
}//入栈

template <typename T>
bool Pop_L(LinkStack<T> &S,T &e){
    if(S.list==NULL) return 0;
    LinkList<T> p=S.list;
    e=p->data;
    S.list=S.list->next;
    delete p;
    return 1;
}//出栈


