#pragma once
#include "LinkQueue.h"

// ─── 循环链式队列 CQueue ─────────────────────────────────────────────────────
// 结构：带一个哑头结点的循环单链表，rear 始终指向最后一个结点
// 空判断：Q.rear->next == Q.rear（头结点的 next 回指自身）
template <typename T>
struct CQueue{
    LNode<T> *rear;
};

template <typename T>
void InitQueue_C(CQueue<T> &Q){
    Q.rear = new LNode<T>;
    Q.rear->next = Q.rear;   // 头结点自身成环
}//初始化循环链式队列

template <typename T>
void DestroyQueue_C(CQueue<T> &Q){
    LNode<T> *head = Q.rear->next;   // 取出头结点
    LNode<T> *p = head->next;
    while(p != head){
        LNode<T> *q = p->next;
        delete p;
        p = q;
    }
    delete head;
    Q.rear = nullptr;
}//销毁循环链式队列

template <typename T>
bool QueueEmpty_C(CQueue<T> Q){
    return Q.rear->next == Q.rear;
}//判断循环链式队列是否为空

template <typename T>
void EnQueue_C(CQueue<T> &Q, T e){
    LNode<T> *p = new LNode<T>;
    p->data = e;
    p->next = Q.rear->next;  // 新结点 next -> 头结点
    Q.rear->next = p;         // 原末尾 next -> 新结点
    Q.rear = p;               // rear 移到新末尾
}//入队（尾部插入）

template <typename T>
bool DeQueue_C(CQueue<T> &Q, T &e){
    if(Q.rear->next == Q.rear) return false;
    // debug记录:这里生成的队列有一个头结点,出队列的时候需要跳过这个头结点才可以
    LNode<T> *head = Q.rear->next;
    LNode<T> *p = head->next;       // 第一个真实结点
    head->next = p->next;
    e = p->data;
    if(p == Q.rear){                // 队列只有一个元素
        Q.rear = head;
    }
    delete p;
    return true;
}//出队（头部删除）

template <typename T>
bool GetHead_C(CQueue<T> Q, T &e){
    if(Q.rear->next == Q.rear) return false;
    e = Q.rear->next->next->data;   // head->next 是第一个真实结点
    return true;
}//取队头元素

// ─── 循环顺序队列 C_Sq_Queue ─────────────────────────────────────────────────
// 用 length 字段记录当前元素个数，避免浪费一个存储位置
// front 下标 = (rear - length + queuesize) % queuesize
#define CSQ_INIT_SIZE 100

template <typename T>
struct C_Sq_Queue{
    T   *elem;
    int  queuesize;
    int  rear;     // 下一个入队位置
    int  length;   // 当前元素个数
};

template <typename T>
void InitQueue_CSq(C_Sq_Queue<T> &Q, int msize = CSQ_INIT_SIZE){
    Q.elem = new T[msize];
    Q.queuesize = msize;
    Q.rear = 0;
    Q.length = 0;
}//初始化循环顺序队列

template <typename T>
void DestroyQueue_CSq(C_Sq_Queue<T> &Q){
    delete [] Q.elem;
    Q.elem = nullptr;
    Q.queuesize = Q.rear = Q.length = 0;
}//销毁循环顺序队列

template <typename T>
bool QueueEmpty_CSq(C_Sq_Queue<T> Q){
    return Q.length == 0;
}

template <typename T>
bool QueueFull_CSq(C_Sq_Queue<T> Q){
    return Q.length == Q.queuesize;
}

template <typename T>
void Enqueue_CSq(C_Sq_Queue<T> &Q, T e){
    if(Q.length == Q.queuesize) return;
    Q.elem[Q.rear] = e;
    Q.rear = (Q.rear + 1) % Q.queuesize;
    Q.length++;
}//入队

template <typename T>
bool DeQueue_CSq(C_Sq_Queue<T> &Q, T &e){
    if(Q.length == 0) return false;
    int front = (Q.rear + Q.queuesize - Q.length) % Q.queuesize;
    e = Q.elem[front];
    Q.length--;
    return true;
}//出队  作业3.11

template <typename T>
bool GetHead_CSq(C_Sq_Queue<T> Q, T &e){
    if(Q.length == 0) return false;
    int front = (Q.rear + Q.queuesize - Q.length) % Q.queuesize;
    e = Q.elem[front];
    return true;
}//取队头元素
