#include "BiTreeQueue.h"

bool InitQueue_Bi(BiQueue &Q){
    Q.front=Q.rear=new BiQNode;
    Q.front->next=nullptr;
    return true;
}
bool QueueEmpty_Bi(BiQueue Q){
    return Q.front==Q.rear;
}
bool EnQueue_Bi(BiQueue &Q, BiTree e){
    BiQNode * p=new BiQNode;
    p->data=e;
    p->next=nullptr;
    Q.rear->next=p;
    Q.rear=p;
    return true;
}
bool DeQueue_Bi(BiQueue &Q,BiTree &e){
    if(Q.front==Q.rear)return false;
    BiQNode *p=Q.front->next;
    e=p->data;
    Q.front->next=p->next;
    if(Q.rear==p)Q.rear=Q.front;
    delete p;
    return true;
}
void DestroyQueue_Bi(BiQueue &Q){
    while (Q.front!=nullptr)
    {
        BiQNode *p=Q.front;
        Q.front=Q.front->next;
        delete p;
    }
    Q.rear=nullptr;
} 