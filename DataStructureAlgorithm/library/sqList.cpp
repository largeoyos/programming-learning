#include "sqList.h"

// Increment 只在本文件内部使用，不对外暴露
template <typename T>
void Increment(SqList<T> &L){
    T *newbase = new T[L.listsize + L.incrementsize];
    if(!newbase){
        ErrorMsg("内存分配失败\n");
        return;
    }
    memcpy(newbase, L.elem, L.listsize * sizeof(T));
    delete [] L.elem;
    L.elem = newbase;
    L.listsize += L.incrementsize;
}//增加顺序表的存储空间

template <typename T>
void InitList_Sq(SqList<T> &L, int msize){
    L.elem = new T[msize];
    L.listsize = msize;
    L.length = 0;
    L.incrementsize = LIST_INC_SIZE;
}//初始化顺序表

template <typename T>
void DestroyList_Sq(SqList<T> &L){
    delete [] L.elem;
    L.elem = nullptr;
    L.length = 0;
    L.listsize = 0;
}//销毁顺序表

template <typename T>
bool ListEmpty_Sq(SqList<T> L){
    return L.length == 0;
}//判断顺序表是否为空

template <typename T>
bool ListFull_Sq(SqList<T> L){
    return L.length == L.listsize;
}//判断顺序表是否已满

template <typename T>
int ListLength_Sq(SqList<T> L){
    return L.length;
}//求顺序表长度

template <typename T>
int LocateItem_Sq(SqList<T> L, T e){
    for(int i = 0; i < L.length; i++){
        if(L.elem[i] == e) return i + 1;
    }
    return 0;
}//按值查找，返回1-based位置，找不到返回0

template <typename T>
int GetItem_Sq(SqList<T> L, int i, T &e){
    if(i < 1 || i > L.length) return 0;
    e = L.elem[i - 1];
    return 1;
}//按位查找（1-based）

void ErrorMsg(const char *msg){
    printf("%s", msg);
}//错误提示

template <typename T>
void ListInsert_Sq(SqList<T> &L, int i, T e){
    if(i < 1 || i > L.length + 1){
        ErrorMsg("插入位置不合法\n");
        return;
    }
    if(L.length == L.listsize){
        Increment(L);
    }
    for(int j = L.length - 1; j >= i - 1; j--){
        L.elem[j + 1] = L.elem[j];
    }
    L.elem[i - 1] = e;
    L.length++;
}//在第i个位置（1-based）插入元素e

template <typename T>
void ListDelete_Sq(SqList<T> &L, int i, T &e){
    if(i < 1 || i > L.length){
        ErrorMsg("删除位置不合法\n");
        return;
    }
    e = L.elem[i - 1];
    for(int j = i; j < L.length; j++){
        L.elem[j - 1] = L.elem[j];
    }
    L.length--;
}//删除第i个位置（1-based）的元素，结果存入e
