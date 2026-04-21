#include "queue.h"
#include <iostream>
using namespace std;

void InitQueue(Queue& Q) {
    Q.front = Q.rear = new LNode;
    Q.front->next = NULL;
}

void DestroyQueue(Queue& Q) {
    while (Q.front) {
        Q.rear = Q.front->next;
        delete Q.front;
        Q.front = Q.rear;
    }
}

void EnQueue(Queue& Q, int num) {
    LinkList p = new LNode;
    p->data = num;
    p->next = NULL;
    Q.rear->next = p;
    Q.rear = p;
}

bool DeQueue(Queue& Q, int& num) {
    if (Q.front == Q.rear)
        return false;
    LinkList p = Q.front->next;
    Q.front->next = p->next;
    num = p->data;
    if (Q.rear == p)
        Q.rear = Q.front;
    delete p;
    return true;
}

bool isQueueEmpty(Queue Q) {
    return Q.front == Q.rear;
}

void TraverseQueue(Queue Q) {
    LinkList p = Q.front;
    while (p->next) {
        cout << p->next->data;
        p = p->next;
    }
}
