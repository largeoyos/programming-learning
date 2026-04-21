#pragma once
#include "Homework.h"
#include "library/BiTree.h"
#include "library/SqStack.h"
#include "library/LinkQueue.h"

// algorithm 3.11 将十进制数N转换为d进制输出
void conversion(int N, int d);
// algorithm 3.12 括号匹配（exp以#结尾）
bool match(char exp[]);
// algorithm 3.13 背包问题（枚举所有可行方案）
void knapsack(int w[], int T, int n);
// algorithm 3.14 / homework 3.9 计算后缀表达式（以#结尾，单位数字）
int calculate(char suffix[]);
// algorithm 3.15 / homework 3.8 中缀表达式转后缀表达式（以#结尾）
void getsuffix(char exp[], char suffix[]);
// algorithm 3.27 打印杨辉三角前n行
void yanghui(int n);

#include "Realize.cpp"
