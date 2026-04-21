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
        default:
            return 0;
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

//algorithm 3.14 / homework 3.9
int calculate(char suffix[]){
    char *p=suffix;
    SqStack<int> S;
    InitStack_Sq(S);
    char ch=*p;
    p++;
    int a,b,e;
    while(ch!='#'){
        if(!isoperator(ch))
            Push_Sq(S,(int)(ch-'0'));
        else{
            Pop_Sq(S,b);   // b = 右操作数（后入栈）
            Pop_Sq(S,a);   // a = 左操作数（先入栈）
            Push_Sq(S,operate(a,ch,b));
        }
        ch=*p;
        p++;
    }
    Pop_Sq(S,e);
    DestroyStack_Sq(S);
    return e;
}//计算后缀表达式suffix的值，suffix以#结尾，仅支持单位数字

//优先级辅助：返回运算符优先级（数值越大越高）
static int priority(char op){
    if(op=='+'||op=='-') return 1;
    if(op=='*'||op=='/') return 2;
    return 0;
}
//preop：栈顶运算符c的优先级 >= 当前运算符ch，则应先弹出c
static bool preop(char c,char ch){
    return priority(c)>=priority(ch);
}

//algorithm 3.15 / homework 3.8
void getsuffix(char exp[],char suffix[]){
    char *p=exp;
    int k=0;
    char ch,c;
    SqStack<char> S;
    InitStack_Sq(S);
    Push_Sq(S,'#');
    ch=*p;
    p++;
    while(ch!='#'){
        if(!isoperator(ch)&&ch!='('&&ch!=')'){
            // 操作数直接输出
            suffix[k++]=ch;
        } else {
            switch(ch){
                case '(':
                    Push_Sq(S,ch);
                    break;
                case ')':
                    // 弹出直到遇到'('，丢弃'('
                    while(GetTop_Sq(S,c)&&c!='('){
                        Pop_Sq(S,c);
                        suffix[k++]=c;
                    }
                    Pop_Sq(S,c); // 弹出并丢弃'('
                    break;
                default:
                    // 当前运算符：弹出所有优先级>=自己的栈内运算符
                    while(GetTop_Sq(S,c)&&c!='#'&&c!='('&&preop(c,ch)){
                        Pop_Sq(S,c);
                        suffix[k++]=c;
                    }
                    Push_Sq(S,ch);
                    break;
            }
        }
        ch=*p;
        p++;
    }
    // 弹出栈内剩余运算符
    while(!StackEmpty_Sq(S)){
        Pop_Sq(S,c);
        if(c!='#') suffix[k++]=c;
    }
    suffix[k]='\0';
    DestroyStack_Sq(S);
}//将中缀表达式exp转换为后缀表达式suffix，exp以#结尾



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
//打印杨辉三角的前n行
