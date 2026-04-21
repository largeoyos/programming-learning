#pragma once
#include "library/LinkQueue.h"
#include "library/SqList.h"
#include "library/LinkList.h"
#include "library/SqStack.h"

// 2.3 在有序顺序表L中插入元素x（保持有序）
void Insertx(SqList<int> &L, int x);
// 2.5 逆置顺序表L
void Reverse(SqList<int> &L);
// 2.6 逆置链表L（不带头结点）
void ReverseLinkList(LinkList<int> &L);
// 2.9 两个有序链表La、Lb归并为有序链表Lc（去重）
void Union2_L(LinkList<int> &La, LinkList<int> &Lb, LinkList<int> &Lc);
// 2.10 删除以s为某结点的循环链表的最后一个结点
void DeleteHead_L(LNode<int> *s);
// 2.12 将顺序表中奇数移到偶数前面
void Exchange_Sq(SqList<int> &L);
// 3.7 括号匹配（不需要#结尾，遇到'\0'结束）
bool matchToString(char str[]);
