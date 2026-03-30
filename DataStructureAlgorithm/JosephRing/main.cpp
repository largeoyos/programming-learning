#ifndef IOSINCLUDE

#include<iostream>
#endif


#include"CyList.h"

int main(){
    CyList Joseph=new LNode;
    InitCyList(Joseph);    
    LNode* Now=Joseph;

    int n;
    std::cin>>n;
    for(int i=0;i<n;i++){
        int password;
        std::cin>>password;
        InsertCyList(Joseph,Now,i,password);
    }
    int m;
    std::cin>>m;
    while(!isCyListEmpty){
        for(int i=0;i<m;i++)Now=Now->next;
        int order=DeleteCyList(Joseph,Now,m);
        std::cout<<order<<" ";
    }
}
