#pragma once
#include "LinkQueue.h"
// LinkQueue.h -> LinkStack.h -> SqStack.h -> LinkList.h -> sqList.h
// 层序遍历用 LinkQueue<T>，非递归遍历用 SqStack<T>，上面的链已全部覆盖

// ─── 结点与类型别名 ──────────────────────────────────────────────────────────
template <typename T>
struct BiTNode {
    T data;
    BiTNode<T> *lchild;
    BiTNode<T> *rchild;
};

template <typename T>
using BiTree = BiTNode<T>*;   // BiTree<T> p; 等价于 BiTNode<T> *p;

// ─── 初始化与构造 ─────────────────────────────────────────────────────────────
template <typename T>
void InitBiTree(BiTree<T> &root);
// 按先序序列建树（nullVal 表示空结点，如字符树用'#'，整数树用-1）
template <typename T>
void CreateBiTree_Pre(BiTree<T> &root, T nullVal);
template <typename T>
void DestroyBiTree(BiTree<T> &root);
template <typename T>
void ClearBiTree(BiTree<T> &root);         // 等价于 DestroyBiTree

// ─── 基本属性 ────────────────────────────────────────────────────────────────
template <typename T>
bool BiTreeEmpty(BiTree<T> root);
template <typename T>
int  BiTreeDepth(BiTree<T> root);          // 树的高度（空树为0）
template <typename T>
int  NodeCount(BiTree<T> root);            // 总结点数
template <typename T>
int  LeafCount(BiTree<T> root);            // 叶结点数

// ─── 结点导航 ────────────────────────────────────────────────────────────────
template <typename T>
BiTree<T> Root(BiTree<T> root);
template <typename T>
BiTree<T> LeftChild(BiTree<T> p);
template <typename T>
BiTree<T> RightChild(BiTree<T> p);
// 在以 root 为根的树中查找 p 的父结点，找不到返回 nullptr
template <typename T>
BiTree<T> Parent(BiTree<T> root, BiTree<T> p);
// 在以 root 为根的树中按值查找，返回第一个匹配结点，找不到返回 nullptr
template <typename T>
BiTree<T> LocateNode(BiTree<T> root, T e);

// ─── 结构修改 ────────────────────────────────────────────────────────────────
// 将 subtree 插入为 p 的左子树，p 原左子树成为 subtree 的左子树
template <typename T>
void InsertLeft(BiTree<T> p, BiTree<T> subtree);
// 将 subtree 插入为 p 的右子树，p 原右子树成为 subtree 的右子树
template <typename T>
void InsertRight(BiTree<T> p, BiTree<T> subtree);
// 销毁并清除 p 的左/右子树
template <typename T>
void DeleteLeft(BiTree<T> p);
template <typename T>
void DeleteRight(BiTree<T> p);

// ─── 递归遍历 ────────────────────────────────────────────────────────────────
template <typename T>
void PreOrderTraverse(BiTree<T> root, void(*visit)(T));
template <typename T>
void InOrderTraverse(BiTree<T> root, void(*visit)(T));
template <typename T>
void PostOrderTraverse(BiTree<T> root, void(*visit)(T));

// ─── 层序遍历（BFS，借助链式队列）────────────────────────────────────────────
template <typename T>
void LevelOrderTraverse(BiTree<T> root, void(*visit)(T));

// ─── 非递归遍历（借助顺序栈）─────────────────────────────────────────────────
template <typename T>
void PreOrderTraverse_NR(BiTree<T> root, void(*visit)(T));
template <typename T>
void InOrderTraverse_NR(BiTree<T> root, void(*visit)(T));
// 后序非递归：用 prev 指针标记最近访问的结点，判断右子树是否已访问
template <typename T>
void PostOrderTraverse_NR(BiTree<T> root, void(*visit)(T));

// ─── 可视化 ──────────────────────────────────────────────────────────────────
// 将树旋转90°横向打印（右子树在上，左子树在下）；depth 内部使用，调用时省略
template <typename T>
void PrintBiTree(BiTree<T> root, int depth = 0);

#include "BiTree.cpp"
