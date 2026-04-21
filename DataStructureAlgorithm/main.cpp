#include "Realize.h"
#include "library/LinkStack.h"
#include "library/KMPString.h"
#include "library/CyList.h"
// 包含链：Realize.h -> Homework.h -> LinkQueue.h -> LinkStack.h
//        -> SqStack.h -> LinkList.h -> sqList.h
// 所有数据结构库函数（SqList、LinkList、SqStack、LinkStack、LinkQueue）
// 以及 Homework 作业函数和 Realize 算法函数均可在此直接调用。
//
// 可调用的函数示例：
//   数据结构：InitList_Sq / InitStack_Sq / InitQueue_L / ...
//   作业函数：Insertx / Reverse / ReverseLinkList / Union2_L /
//             DeleteHead_L / Exchange_Sq / matchToString
//   算法函数：conversion / match / knapsack / calculate /
//             getsuffix / yanghui

int main(){
    int n;
    std::cin >> n;
    yanghui(n);
    return 0;
}
