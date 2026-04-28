#pragma once

typedef char BiTreeElemType;
typedef struct BiTNode
{
    BiTreeElemType data;
    struct BiTNode *lchild;
    struct BiTNode *rchild;
} BiTNode, *BiTree;

// Create BiTree
bool InitBiTree(BiTree &T);
BiTree CreateNode(BiTreeElemType e);

void CreateBiTree(BiTree &T);

// recursive traversal
void PreOrder(BiTree T);
void InOrder(BiTree T);
void PostOrder(BiTree T);
void LevelOrder(BiTree T);

// non-recursive traversal
void PreOrder_NonRec(BiTree T);
void InOrder_NonRec(BiTree T);
void PostOrder_NonRec(BiTree T);

// Common Operation
int TreeDepth(BiTree T);
int NodeCount(BiTree T);
int LeafCount(BiTree T);
int NodeCountDeg2(BiTree T);
BiTree FindNode(BiTree T, BiTreeElemType e);
void DestroyTree(BiTree &T);

