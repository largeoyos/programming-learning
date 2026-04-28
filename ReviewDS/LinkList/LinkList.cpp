#include "LinkList.h"


bool InitList_H(LinkList &L){
    L=new LNode;
    if(L=nullptr)
        return false;
    L->next=nullptr;
    return true;
} 

bool InitList(LinkList &L){
    L=nullptr;
    return true;
}

bool InitCircularList(LinkList&L){
    L=new LNode;
    if(L=nullptr)
        return false;
    L->next=L;
    return true;
}

