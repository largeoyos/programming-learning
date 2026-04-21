#ifndef KMP_STRING_H
#define KMP_STRING_H

// 定义字符串结构体（定长顺序存储，或者您可以根据需要选择动态分配）
// 这里使用一种常见的教学实现方式：SString
#define MAXLEN 255

typedef struct {
    char ch[MAXLEN + 1]; // ch[0] 习惯上可以存放长度，或者不使用 ch[0] 遵循 C 风格
    int length;
} SString;

// 初始化字符串
void StrAssign(SString &S, const char* chars);

// 获取字符串长度
int StrLength(SString S);

// 打印字符串
void PrintString(SString S);

// KMP 算法核心：获取 next 数组
void GetNext(SString T, int next[]);

// KMP 算法核心：获取 nextval 数组 (KMP 优化)
void GetNextVal(SString T, int nextval[]);

// KMP 模式匹配
// 返回子串 T 在主串 S 中第 pos 个字符之后的位置。若未找到返回 0。
int Index_KMP(SString S, SString T, int pos, int next[]);

#endif // KMP_STRING_H
