#pragma once
#include "BiTree.h"

typedef struct BiQNode{
    BiTree data;
    struct BiQNode *next;
}BiQNode;

typedef struct {
    BiQNode *front;
    BiQNode *rear;
}BiQueue;

bool InitQueue_Bi(BiQueue &Q);
bool QueueEmpty_Bi(BiQueue Q);
bool EnQueue_Bi(BiQueue &Q, BiTree e);
bool DeQueue_Bi(BiQueue &Q,BiTree &e);
void DestroyQueue_Bi(BiQueue &Q);