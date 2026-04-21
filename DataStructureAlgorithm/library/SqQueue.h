#pragma once
#include "sqList.h"

#define QUEUE_INIT_SIZE  100
#define QUEUE_INCREMENT   10

// 循环顺序队列（标准做法：牺牲一个存储位置区分空/满）
// 空：front == rear
// 满：(rear + 1) % queuesize == front
// 长度：(rear - front + queuesize) % queuesize
template <typename T>
struct SqQueue{
    T   *base;
    int  front;
    int  rear;
    int  queuesize;
};

template <typename T>
void InitQueue_Sq(SqQueue<T> &Q, int msize = QUEUE_INIT_SIZE){
    Q.base = new T[msize];
    Q.front = Q.rear = 0;
    Q.queuesize = msize;
}//初始化循环顺序队列

template <typename T>
void DestroyQueue_Sq(SqQueue<T> &Q){
    delete [] Q.base;
    Q.base = nullptr;
    Q.front = Q.rear = Q.queuesize = 0;
}//销毁循环顺序队列

template <typename T>
bool QueueEmpty_Sq(SqQueue<T> Q){
    return Q.front == Q.rear;
}//判断循环顺序队列是否为空

template <typename T>
bool QueueFull_Sq(SqQueue<T> Q){
    return (Q.rear + 1) % Q.queuesize == Q.front;
}//判断循环顺序队列是否已满

template <typename T>
int QueueLength_Sq(SqQueue<T> Q){
    return (Q.rear - Q.front + Q.queuesize) % Q.queuesize;
}//求循环顺序队列的长度

template <typename T>
bool GetHead_Sq(SqQueue<T> Q, T &e){
    if(Q.front == Q.rear) return false;
    e = Q.base[Q.front];
    return true;
}//取队头元素（不出队）

template <typename T>
bool EnQueue_Sq(SqQueue<T> &Q, T e){
    if((Q.rear + 1) % Q.queuesize == Q.front) return false;
    Q.base[Q.rear] = e;
    Q.rear = (Q.rear + 1) % Q.queuesize;
    return true;
}//入队，满时返回false

template <typename T>
bool DeQueue_Sq(SqQueue<T> &Q, T &e){
    if(Q.front == Q.rear) return false;
    e = Q.base[Q.front];
    Q.front = (Q.front + 1) % Q.queuesize;
    return true;
}//出队，空时返回false

template <typename T>
void QueueTraverse_Sq(SqQueue<T> Q){
    int i = Q.front;
    while(i != Q.rear){
        std::cout << Q.base[i] << " ";
        i = (i + 1) % Q.queuesize;
    }
    std::cout << std::endl;
}//遍历循环顺序队列
