struct LNode{
   int position;
   int password;
   LNode* next;
};
using CyList=LNode*;
bool InitCyList(CyList &L);
bool InsertCyList(CyList &L,int position,int password);
bool isCyListEmpty(CyList L);
int DeleteCyList(CyList &L,int position);

