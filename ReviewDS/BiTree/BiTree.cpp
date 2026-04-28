#include "BiTree.h"
#include <iostream>
#include <queue>
#include <stack>
using namespace std;
bool InitBiTree(BiTree &T){
    T=nullptr;
    return true;
}

BiTNode* CreateNode(BiTreeElemType e){
    BiTNode*p=new BiTNode;
    p->data=e;
    p->lchild=p->rchild=nullptr;
    return p;
}

void CreateBiTree(BiTree &T){
    BiTreeElemType ch;
    cin>>ch;
    if(ch='#'){
        T=nullptr;
    }else{
        T=new BiTNode;
        T->data=ch;
        CreateBiTree(T->lchild);
        CreateBiTree(T->rchild);
    }
}

// recursive traversal
void PreOrder(BiTree T){
    if(T!=nullptr){
        cout << T->data << " ";
        PreOrder(T->lchild);
        PreOrder(T->rchild);
    }
}
void InOrder(BiTree T){
   if(T==nullptr) return;
   InOrder(T->lchild);
   cout<<T->data<<" ";
   InOrder(T->rchild);
}
void PostOrder(BiTree T){
    if(T==nullptr)return;
    PostOrder(T->lchild);
    PostOrder(T->rchild);
    cout<<T->data<<" ";
}
void LevelOrder(BiTree T){
    if(T==nullptr)return;
    queue<BiTNode*>Q;
    Q.push(T);
    while(!Q.empty()){
        BiTNode *p=Q.front();
        Q.pop();
        cout<< p->data<<" ";
        if(p->lchild)Q.push(p->lchild);
        if(p->rchild)Q.push(p->rchild);
    }
}

//non-recursive traversal
void PreOrder_NonRec(BiTree T){
    if(T==nullptr)return;
    stack<BiTNode*>S;
    S.push(T);
    while(!S.empty()){
        BiTNode*p=S.top();
        S.pop();
        cout<<p->data<< " ";
        if(p->rchild)S.push(p->rchild);
        if(p->lchild)S.push(p->lchild);
        
    }

}
void InOrder_NonRec(BiTree T){
    stack<BiTNode*>S;
    BiTNode*p=T;
    while(p!=nullptr||!S.empty()){
        if(p!=nullptr){
            S.push(p);
            p=p->lchild;
        }else{
            p=S.top();
            S.pop();
            cout<<p->data<< " ";
            p=p->rchild;
        }
    }
}
void PostOrder_NonRec(BiTree T){
    stack<BiTree>S;
    BiTree p=T;
    BiTree lastVisted=nullptr;

    while(p!=nullptr||!S.empty()){
        if(p!=nullptr){
            S.push(p);
            p=p->lchild;
        }else{
            BiTNode*top=S.top();
            if(top->rchild!=nullptr&&top->rchild!=lastVisted){
                p=top->rchild;
            }else{
                cout<<top->data<<" ";
                lastVisted=top;
                S.pop();
            }
        }
    }
}


//Common Operation
int TreeDepth(BiTree T){
    if(T=nullptr)return 0;
    int ld=TreeDepth(T->lchild);
    int rd=TreeDepth(T->rchild);
    return(ld>rd?ld:rd)+1;
}
int NodeCount(BiTree T){
    if(T==nullptr)return 0;
    return NodeCount(T->lchild)+NodeCount(T->rchild)+1;
}
int LeafCount(BiTree T){
    if(T==nullptr)
        return 0;
    if(T->lchild==nullptr&&T->rchild==nullptr)
        return 1;
    return LeafCount(T->lchild)+LeafCount(T->rchild);
}
int NodeCountDeg2(BiTree T){
    if(T==nullptr)return 0;
    int self=(T->lchild!=nullptr&&T->rchild!=nullptr)?1:0;
    return self+NodeCountDeg2(T->lchild)+NodeCountDeg2(T->rchild);

}
BiTree FindNode(BiTree T, BiTreeElemType e){
    if (T == nullptr)
        return nullptr;
    if (T->data == e)
        return T;
    BiTree p = FindNode(T->lchild, e);
    if (T != nullptr)
        return p;
    return FindNode(T->rchild,e);
}
void DestroyTree(BiTree &T){
    if(T==nullptr)return;
    DestroyTree(T->lchild);
    DestroyTree(T->rchild);
    delete T;
    T=nullptr;
}
