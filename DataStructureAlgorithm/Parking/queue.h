#pragma once

struct LNode {
    int data;
    LNode* next;
};

using LinkList = LNode*;

struct Queue {
    LinkList front;
    LinkList rear;
};

void InitQueue(Queue& Q);
void DestroyQueue(Queue& Q);
void EnQueue(Queue& Q, int num);
bool DeQueue(Queue& Q, int& num);
bool isQueueEmpty(Queue Q);
void TraverseQueue(Queue Q);
