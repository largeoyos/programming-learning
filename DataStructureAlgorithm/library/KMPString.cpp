#include "KMPString.h"
#include <iostream>
#include <cstring>

void StrAssign(SString &S, const char* chars) {
    int i = 0;
    while (chars[i] != '\0' && i < MAXLEN) {
        S.ch[i + 1] = chars[i]; // 我们从 ch[1] 开始存储字符，方便 KMP 逻辑
        i++;
    }
    S.length = i;
}

int StrLength(SString S) {
    return S.length;
}

void PrintString(SString S) {
    for (int i = 1; i <= S.length; i++) {
        std::cout << S.ch[i];
    }
    std::cout << std::endl;
}

void GetNext(SString T, int next[]) {
    int i = 1, j = 0;
    next[1] = 0;
    while (i < T.length) {
        if (j == 0 || T.ch[i] == T.ch[j]) {
            ++i;
            ++j;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
}

void GetNextVal(SString T, int nextval[]) {
    int i = 1, j = 0;
    nextval[1] = 0;
    while (i < T.length) {
        if (j == 0 || T.ch[i] == T.ch[j]) {
            ++i;
            ++j;
            if (T.ch[i] != T.ch[j])
                nextval[i] = j;
            else
                nextval[i] = nextval[j];
        } else {
            j = nextval[j];
        }
    }
}

int Index_KMP(SString S, SString T, int pos, int next[]) {
    int i = pos; // 主串指针
    int j = 1;   // 模式串指针
    while (i <= S.length && j <= T.length) {
        if (j == 0 || S.ch[i] == T.ch[j]) {
            ++i;
            ++j;
        } else {
            j = next[j];
        }
    }
    if (j > T.length) {
        return i - T.length; // 匹配成功
    } else {
        return 0; // 匹配失败
    }
}
