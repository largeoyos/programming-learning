#ifndef IOSINCLUDE

#include<iostream>
#endif


#include"CyList.h"

int main(){
    CyList Joseph=new LNode;
    InitCyList(Joseph);
    int n;
    std::cin>>n;
    for(int i=0;i<n;i++){
        int password;
        std::cin>>password;
        InsertCyList(Joseph,i,password);
    }
    int m;
    std::cin>>m;
    LNode* Now=Joseph;
    while(!isCyListEmpty){
        for(int i=0;i<m;i++)Now=Now->next;
        int order=DeleteCyList(Joseph,m);
        std::cout<<order<<" ";
    }
}
