#include "Realize.h"
    
//algorithm 3.11



void conversion(int N, int d){
    SqStack<int> S;
    InitStack_Sq(S);
    while(N){
        Push_Sq(S,N%d);
        N=N/d;
    }
    while(!StackEmpty_Sq(S)){
        int e;
        Pop_Sq(S,e);
        printf("%d", e);
    }
}//将十进制数N转换为d进制数并输出

//algorithm 3.12
//注意这里的库函数是int类型的但是这里的char类型的，所以要进行类型转换
bool match(char exp[]){
    int matchstat=1;
    SqStack<int> S;

    InitStack_Sq(S);
    char ch=*exp;
    exp++;
    int e;
    while(ch!='#'&&matchstat){
        switch(ch){
            case '(':
            case '[':
            case '{':
                Push_Sq(S, static_cast<int>(ch));
                break;
            case ')':
            if(!Pop_Sq(S,e) || e!='('){
                matchstat=0;
            }
                break;
            case ']':
            if(!Pop_Sq(S,e) || e!='['){
                matchstat=0;
            }
                break;
            case '}':
            if(!Pop_Sq(S,e) || e!='{'){
                matchstat=0;
            }
                break;
        }
        ch=*exp;
        exp++;
        
    }
    if(matchstat && StackEmpty_Sq(S))
        return 1;
    else
        return 0;

}//判断括号表达式exp是否匹配，exp以#结尾

//algorithm 3.13
void knapsack(int w[],int T,int n){
    SqStack<int> S;
    InitStack_Sq(S);
    int k=0;
    do{
        while(T>0&&k<n){
            if(T-w[k]>=0){
                Push_Sq(S,k);
                T=T-w[k];
            }
            k++;

        }
        if(T==0){
            StackTraverse_Sq(S);
        }
        if(!StackEmpty_Sq(S)){
            Pop_Sq(S,k);
            T=T+w[k];
            k++;
        }


    }
    while(!StackEmpty_Sq(S)||k<n);
    DestroyStack_Sq(S);
}//求解背包问题，w为物品重量数组，T为背包容量，n为物品数量

int operate(int a,char ch,int b){
    switch(ch){
        case '+':return a+b;
        case '-':return a-b;
        case '*':return a*b;
        case '/':return a/b;
    }
}//计算a和b的运算结果

int isoperator(char ch){
    switch(ch){
        case '+':
        case '-':
        case '*':
        case '/':return 1;
        default:return 0;
    }
}//判断ch是否为运算符

//algorithm 3.14
int calculate(char suffix[]){
    char *p=suffix;
    SqStack<int> S;
    InitStack_Sq(S);
    char ch=*p++;
    while(ch!='#'){
        if(!isoperator(ch)){
            Push_Sq(S,ch-'0');
        }
        else{
            int a,b;
            Pop_Sq(S,a);
            Pop_Sq(S,b);
            Push_Sq(S,operate(a,ch,b));
        }
        ch=*p++;
    }
    int result;
    Pop_Sq(S,result);
    DestroyStack_Sq(S);
    return result;

}

//algorithm 3.27
void yanghui(int n){
    LinkQueue<int> Q;
    InitQueue_L(Q);
    EnQueue_L(Q,0);
    EnQueue_L(Q,1);
    EnQueue_L(Q,1);
    int k=1;
    int s,e;
    while(k<n){
        for(int i=1;i<=n-k;i++){
            std::cout<<' ';
        }
        EnQueue_L(Q,0);
        do{
            DeQueue_L(Q,s);
            GetHead_L(Q,e);
            if(e){
                std::cout<<e<<" ";
            }
            else{
                std::cout<<std::endl;
            }
            EnQueue_L(Q,s+e);
        }while(e!=0);
        k++;
    }   
    DeQueue_L(Q,e);
    while(!QueueEmpty_L(Q)){
        DeQueue_L(Q,e);        
        std::cout<<e<<" ";
    }
}