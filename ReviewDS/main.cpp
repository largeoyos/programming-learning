#include "LinkList.h"  
#include "SqStack.h"
#include "BiTree.h"
#include <iostream>
#include <windows.h>

using namespace std;
int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    BiTree T;
    cout << "按先序输入(# 表示空),例如 AB#D##C##:" << endl;
    CreateBiTree(T);
    
    cout << "递归先序: "; PreOrder(T);  cout << endl;
    cout << "递归中序: "; InOrder(T);   cout << endl;
    cout << "递归后序: "; PostOrder(T); cout << endl;
    cout << "层序遍历: "; LevelOrder(T);cout << endl;
    
    cout << "非递归先序: "; PreOrder_NonRec(T);  cout << endl;
    cout << "非递归中序: "; InOrder_NonRec(T);   cout << endl;
    cout << "非递归后序: "; PostOrder_NonRec(T); cout << endl;
    
    cout << "节点总数: " << NodeCount(T) << endl;
    cout << "叶子数:   " << LeafCount(T) << endl;
    cout << "度2节点: "  << NodeCountDeg2(T) << endl;
    cout << "树深度:   " << TreeDepth(T) << endl;
    
    DestroyTree(T);
    return 0;
}