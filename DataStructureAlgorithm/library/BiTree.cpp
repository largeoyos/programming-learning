#include "BiTree.h"

// ─── 初始化与构造 ─────────────────────────────────────────────────────────────

template <typename T>
void InitBiTree(BiTree<T> &root){
    root = nullptr;
}//初始化二叉树（置空）

template <typename T>
void CreateBiTree_Pre(BiTree<T> &root, T nullVal){
    T data;
    std::cin >> data;
    if(data == nullVal){
        root = nullptr;
        return;
    }
    root = new BiTNode<T>;
    root->data   = data;
    root->lchild = nullptr;
    root->rchild = nullptr;
    CreateBiTree_Pre(root->lchild, nullVal);
    CreateBiTree_Pre(root->rchild, nullVal);
}//按先序序列从标准输入建树，nullVal 标记空结点（字符树传'#'，整数树传-1）

template <typename T>
void DestroyBiTree(BiTree<T> &root){
    if(!root) return;
    DestroyBiTree(root->lchild);
    DestroyBiTree(root->rchild);
    delete root;
    root = nullptr;
}//后序递归释放所有结点

template <typename T>
void ClearBiTree(BiTree<T> &root){
    DestroyBiTree(root);
}//清空二叉树（与 DestroyBiTree 等价）

// ─── 基本属性 ────────────────────────────────────────────────────────────────

template <typename T>
bool BiTreeEmpty(BiTree<T> root){
    return root == nullptr;
}

template <typename T>
int BiTreeDepth(BiTree<T> root){
    if(!root) return 0;
    int ld = BiTreeDepth(root->lchild);
    int rd = BiTreeDepth(root->rchild);
    return 1 + (ld > rd ? ld : rd);
}//返回树的高度，空树为0

template <typename T>
int NodeCount(BiTree<T> root){
    if(!root) return 0;
    return 1 + NodeCount(root->lchild) + NodeCount(root->rchild);
}//统计总结点数

template <typename T>
int LeafCount(BiTree<T> root){
    if(!root) return 0;
    if(!root->lchild && !root->rchild) return 1;
    return LeafCount(root->lchild) + LeafCount(root->rchild);
}//统计叶结点数

// ─── 结点导航 ────────────────────────────────────────────────────────────────

template <typename T>
BiTree<T> Root(BiTree<T> root){
    return root;
}

template <typename T>
BiTree<T> LeftChild(BiTree<T> p){
    return p ? p->lchild : nullptr;
}

template <typename T>
BiTree<T> RightChild(BiTree<T> p){
    return p ? p->rchild : nullptr;
}

template <typename T>
BiTree<T> Parent(BiTree<T> root, BiTree<T> p){
    if(!root || root == p) return nullptr;
    if(root->lchild == p || root->rchild == p) return root;
    BiTree<T> found = Parent(root->lchild, p);
    if(found) return found;
    return Parent(root->rchild, p);
}//在 root 树中查找 p 的父结点；p 为根时或不存在时返回 nullptr

template <typename T>
BiTree<T> LocateNode(BiTree<T> root, T e){
    if(!root) return nullptr;
    if(root->data == e) return root;
    BiTree<T> found = LocateNode(root->lchild, e);
    if(found) return found;
    return LocateNode(root->rchild, e);
}//先序方式按值查找，返回第一个匹配结点

// ─── 结构修改 ────────────────────────────────────────────────────────────────

template <typename T>
void InsertLeft(BiTree<T> p, BiTree<T> subtree){
    if(!p || !subtree) return;
    subtree->lchild = p->lchild;   // p 原左子树挂到 subtree 左边
    p->lchild = subtree;
}//将 subtree 插入为 p 的左子树

template <typename T>
void InsertRight(BiTree<T> p, BiTree<T> subtree){
    if(!p || !subtree) return;
    subtree->rchild = p->rchild;   // p 原右子树挂到 subtree 右边
    p->rchild = subtree;
}//将 subtree 插入为 p 的右子树

template <typename T>
void DeleteLeft(BiTree<T> p){
    if(!p) return;
    DestroyBiTree(p->lchild);   // lchild 会被置 nullptr
}//销毁并清除 p 的左子树

template <typename T>
void DeleteRight(BiTree<T> p){
    if(!p) return;
    DestroyBiTree(p->rchild);
}//销毁并清除 p 的右子树

// ─── 递归遍历 ────────────────────────────────────────────────────────────────

template <typename T>
void PreOrderTraverse(BiTree<T> root, void(*visit)(T)){
    if(!root) return;
    visit(root->data);
    PreOrderTraverse(root->lchild, visit);
    PreOrderTraverse(root->rchild, visit);
}//先序遍历（根-左-右）

template <typename T>
void InOrderTraverse(BiTree<T> root, void(*visit)(T)){
    if(!root) return;
    InOrderTraverse(root->lchild, visit);
    visit(root->data);
    InOrderTraverse(root->rchild, visit);
}//中序遍历（左-根-右）

template <typename T>
void PostOrderTraverse(BiTree<T> root, void(*visit)(T)){
    if(!root) return;
    PostOrderTraverse(root->lchild, visit);
    PostOrderTraverse(root->rchild, visit);
    visit(root->data);
}//后序遍历（左-右-根）

// ─── 层序遍历 ────────────────────────────────────────────────────────────────

template <typename T>
void LevelOrderTraverse(BiTree<T> root, void(*visit)(T)){
    if(!root) return;
    LinkQueue<BiTree<T>> Q;
    InitQueue_L(Q);
    EnQueue_L(Q, root);
    while(!QueueEmpty_L(Q)){
        BiTree<T> p;
        DeQueue_L(Q, p);
        visit(p->data);
        if(p->lchild) EnQueue_L(Q, p->lchild);
        if(p->rchild) EnQueue_L(Q, p->rchild);
    }
    DestroyQueue_L(Q);
}//层序遍历（BFS），使用链式队列

// ─── 非递归遍历 ──────────────────────────────────────────────────────────────

template <typename T>
void PreOrderTraverse_NR(BiTree<T> root, void(*visit)(T)){
    SqStack<BiTree<T>> S;
    InitStack_Sq(S);
    BiTree<T> p = root;
    while(p || !StackEmpty_Sq(S)){
        // 一路向左走，沿途访问并压栈
        while(p){
            visit(p->data);
            Push_Sq(S, p);
            p = p->lchild;
        }
        // 回溯，转向右子树
        Pop_Sq(S, p);
        p = p->rchild;
    }
    DestroyStack_Sq(S);
}//非递归先序遍历

template <typename T>
void InOrderTraverse_NR(BiTree<T> root, void(*visit)(T)){
    SqStack<BiTree<T>> S;
    InitStack_Sq(S);
    BiTree<T> p = root;
    while(p || !StackEmpty_Sq(S)){
        // 一路向左走，只压栈不访问
        while(p){
            Push_Sq(S, p);
            p = p->lchild;
        }
        // 弹出时访问，再转向右子树
        Pop_Sq(S, p);
        visit(p->data);
        p = p->rchild;
    }
    DestroyStack_Sq(S);
}//非递归中序遍历

template <typename T>
void PostOrderTraverse_NR(BiTree<T> root, void(*visit)(T)){
    SqStack<BiTree<T>> S;
    InitStack_Sq(S);
    BiTree<T> p    = root;
    BiTree<T> prev = nullptr;  // 最近一次已访问的结点
    while(p || !StackEmpty_Sq(S)){
        // 一路向左压栈
        while(p){
            Push_Sq(S, p);
            p = p->lchild;
        }
        GetTop_Sq(S, p);
        // 若右子树存在且尚未访问，则先处理右子树
        if(p->rchild && p->rchild != prev){
            p = p->rchild;
        } else {
            // 右子树为空或已访问，可以访问当前结点
            Pop_Sq(S, p);
            visit(p->data);
            prev = p;
            p = nullptr;   // 不再向右走，继续弹栈
        }
    }
    DestroyStack_Sq(S);
}//非递归后序遍历（prev 指针法）

// ─── 可视化 ──────────────────────────────────────────────────────────────────

template <typename T>
void PrintBiTree(BiTree<T> root, int depth){
    if(!root) return;
    // 右子树在上（对应视觉上的"右"）
    PrintBiTree(root->rchild, depth + 1);
    // 打印当前结点：depth-1 个缩进单元 + 连接符
    for(int i = 0; i < depth - 1; i++) std::cout << "    ";
    if(depth > 0) std::cout << "|---";
    std::cout << root->data << "\n";
    // 左子树在下（对应视觉上的"左"）
    PrintBiTree(root->lchild, depth + 1);
}
// 示例输出（树旋转90°，根在最左，右子树在上方，左子树在下方）：
//     |---C
// |---A
//     |---B
// 上图表示 A 为根，B 为左孩子，C 为右孩子
