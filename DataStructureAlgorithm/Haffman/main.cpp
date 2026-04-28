#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>

using namespace std;
struct Stack {
    char * elem;
    int capacity;
    int top;
};
void InitStack(Stack& S, int n) {
    S.elem = new char[n];
    S.capacity = n;
    S.top = -1;
}

void DestroyStack(Stack& S) {
    delete[] S.elem;
    S.top = -1;
    S.capacity = 0;
}

bool GetTop(Stack& S, char& e) {
    if (S.top == -1)
        return false;
    e = S.elem[S.top];
    return true;
}

void PushStack(Stack& S, char e) {
    S.top++;
    S.elem[S.top] = e;
}

bool PopStack(Stack& S, char& e) {
    if (S.top == -1)
        return false;
    e = S.elem[S.top];
    S.top--;
    return true;
}

void TraveseStack(Stack& S) {
    for (int i = 0; i <= S.top; i++) {
        cout << S.elem[i] << " ";
    }
}

typedef struct{
    char value;
    int weight;
    int parent,lchild,rchild;

}HTNode;
typedef HTNode* HuffTree;
typedef char** HuffmanCode;

void PrintUI(bool TreeState){
    cout<<"=====================================\n";
    cout<<"      HuffmanTree编码解码系统\n";
    cout<<"=====================================\n";
    cout<<"    已读取树?";
    TreeState==0?cout<<"否,请按I键进行读取.":cout<<"是.";
    cout<<"\n";
    cout<<"    编码(对ToBeTran文件进行编码):E键.\n";
    cout<<"    解码:对CodeFile文件解码:D键.\n";
    cout<<"    打印代码文件:P键\n";
    cout<<"    打印Huffman树:T键\n";
    cout<<"    按Q键退出.\n";
    cout<<"--------------------------------------\n";
}
void swap(int &a,int &b){
    int t=a;
    a=b;
    b=t;
}
void Select(HuffTree T,int i,int &s1,int &s2){
    //找到第一 二个parent等于零的节点
    int k=1;
    while(T[k].parent!=0){
        k++;
    }
    s1=k;
    k++;
    while(T[k].parent!=0){
        k++;
    }
    s2=k;
    k++;
    if(T[s1].weight>T[s2].weight){
        swap(s1,s2);
    }
    for(int j=k;j<=i-1;j++){
        if(T[j].parent==0&&T[j].weight<=T[s1].weight){
            s2=s1;
            s1=j;
        }else if(T[j].parent==0&&T[j].weight<=T[s2].weight){
            s2=j;
        }
    }
}
//写这个函数遇到bug,因为不知道先找到两个可以取的节点怎么写

void HuffmanTree(HuffTree &T,int n){
    int m=2*n-1;
    for(int i=1;i<=m;i++){
        if(i>n){
            T[i].weight=0;
            T[i].value='#';
        }
        T[i].lchild=T[i].rchild=T[i].parent=0;
    }
    for(int i=n+1;i<=m;i++){
        int s1,s2;
        Select(T,i,s1,s2);
        T[i].lchild=s1;
        T[i].rchild=s2;
        T[i].weight=T[s1].weight+T[s2].weight;
        T[s1].parent=T[s2].parent=i;
    }
}
void InitHuffman(HuffTree &T,bool &TreeState,int &n){
    cout<<"请输入字符种类数量\n";
    cin>>n;
    cout<<"\n";
    // char word[n];
    // int freq[n];
    cout<<"请输入依次字符及其权值.\n";
    T=new HTNode[2*n];
    cin.ignore(1000,'\n');
    //noskipws是一个流操纵符,告诉 cin 不要跳过空白符
    //cin.ignore(1000,'\n');可以把前文的回车都忽略
    //记得切换回来
    for(int i=1;i<=n;i++){
        cin>>noskipws>>T[i].value;
        cin>>skipws>>T[i].weight;
        cin.ignore(1000,'\n');
    }
    HuffmanTree(T,n);
    TreeState=1;
    ofstream outFile("hfmTree.txt");
    if(outFile.is_open()){
        // outFile<<"    读入的哈夫曼树:\n";
        outFile<<n<<endl;
        for(int i=1;i<=2*n-1;i++){
            outFile<<left<<setw(5)<<T[i].value<<left<<setw(6)<<T[i].weight<<left<<setw(3)<<T[i].parent<<left<<setw(3)<<T[i].lchild<<left<<setw(3)<<T[i].rchild<<"\n";
        }
        outFile.close();
    }else{
        cout<<"无法打开文件.\n";
    }
   

}
void  Coding(HuffTree T,int root ,HuffmanCode C,Stack &S){
    char ch='\0';
    if(root!=0){
        if(T[root].lchild==0){
            C[root]=new char[S.top+1+1];
            for(int i=0;i<=S.top;i++){
                C[root][i]=S.elem[i];
            }
            C[root][S.top+1]='\0';
            
            // PopStack(S,ch);
        }
        {
            PushStack(S,'0');
            Coding(T,T[root].lchild,C,S);
            PopStack(S,ch);
            PushStack(S,'1');
            Coding(T,T[root].rchild,C,S);
            PopStack(S,ch);
        }
        

    }
}
void HuffmanCoding(HuffTree T,HuffmanCode &C,int n){
    Stack S;
    InitStack(S,n);
    // C=new char *[n+1];
    Coding(T,2*n-1,C,S);
    DestroyStack(S);
}
void EncodingTree(HuffTree &T, bool &TreeState,int&n){//忘了改&n了
    if(TreeState==0){
        ifstream inFile("hfmTree.txt");
        if(inFile.is_open()){
            inFile>>n;
            inFile.ignore(1000,'\n');
            T=new HTNode[2*n];
            for(int i=1;i<=2*n-1;i++){

                inFile>>noskipws;
                inFile.get(T[i].value);
                inFile>>skipws;
                inFile>>skipws>>T[i].weight;
                inFile>>T[i].parent;
                inFile>>T[i].lchild;
                inFile>>T[i].rchild;
                inFile.ignore(1000,'\n');

            }
        }else{
            cout<<"打开文件失败\n";
            return;
        }
        TreeState=1;
    }
    HuffmanCode C=new char*[2*n];
    //new了两次
    HuffmanCoding(T,C,n);
    ifstream inFile("TobeTran.txt");
    ofstream outFile("CodeFile.txt");
    if(!inFile.is_open()){
        cout<<"无法找到待编码文件\n";
        return;
    }
    cout<<"正在编码\n";
    char ch;
    while(inFile.get(ch)){
        bool emerge=0;
        for(int i=1;i<=n;i++){
            if(T[i].value==ch){
                outFile<<C[i];
                cout<<C[i];
                emerge=1;
                break;
            }
        }
        if(!emerge){
            cout<<"字符未定义\n";
        }

    }
    inFile.close();
    outFile.close();
    cout<<"编码完成\n";
    //这里才是删除二重指针正确方式
    for(int i=1;i<=n;i++){
        if(T[i].lchild==0)
            delete[] C[i];
    }
    delete [] C;
    
}
void DecodingTree(HuffTree T,int n){
    if(T==nullptr){
        cout<<"哈夫曼树为空\n";
        return;
    }
    ifstream inFile("CodeFile.txt");
    ofstream outFile("TextFile.txt");
    if(!inFile.is_open()){
        cout<<"CodeFile打开失败\n";

    }
    cout<<"正在解码\n";
    int root=2*n-1;
    int cur=root;
    char bit;
    //获取带着空格的一个字符

    while(inFile.get(bit)){
        if(bit=='0'){
            cur=T[cur].lchild;
        }else if(bit=='1'){
            cur=T[cur].rchild;
        }
        if(T[cur].lchild==0&&T[cur].rchild==0){
            cout<<T[cur].value;
            outFile<<T[cur].value;
            cur=root;
        }
    }
    inFile.close();
    outFile.close();
    cout<<"解码完成\n";


}
void PrintCode(){
    ifstream inFile("CodeFile.txt");
    ofstream outFile("CodePrint.txt");
    if(!inFile.is_open()){
        cout<<"无法找到文件\n";
    }
    char bit;
    int count=0;
    while(inFile>>bit){
        cout<<bit;
        outFile<<bit;
        count++;
        if(count==50){
            cout<<endl;
            outFile<<endl;
            count=0;
        }
    }
    if(count!=0){
        cout<<endl;
    }
    inFile.close();
    outFile.close();
    cout<<"打印完毕\n";
}
void PrintHuffmanTree(HuffTree T,int root,int depth, ofstream&outFile){
    if(root==0){
        return;
    }
    PrintHuffmanTree(T,T[root].rchild,depth+1,outFile);
    string indent="";
    for(int i=0;i<depth;i++){
        indent+="    ";
    }
    string nodeInfo;
    if(T[root].lchild==0&&T[root].rchild==0){
        nodeInfo="["+to_string(T[root].weight)+":'"+T[root].value+"']";

    }else{
        nodeInfo="("+to_string(T[root].weight)+")";
    }
    cout<<indent<<"---"<<nodeInfo<<endl;
    outFile<<indent<<"---"<<nodeInfo<<"\n";
    PrintHuffmanTree(T,T[root].lchild,depth+1,outFile);
    
}
void PrintTree(HuffTree T,int n){
    if(T==NULL){
        cout<<"树不存在\n";
    }
    ofstream outFile("TreePrint.txt");
    if(!outFile.is_open()){
        cout<<"无法打开文件\n";
    }
    int root=2*n-1;
    cout<<"打印哈夫曼树\n";
    PrintHuffmanTree(T,root,0,outFile);
    outFile.close();
}

int main(){
    char command;
    bool TreeState=0;
    int num=0;
    HuffTree T=nullptr;
    PrintUI(TreeState);
    while(cin>>command&&(command!='q'&&command!='Q')){
        if(!((command>='a'&&command<='z')||(command>='A'&&command<='Z'))){
            cout<<"错误输入,请输入字母"<<endl;
        }else 
        if(command=='I'||command=='i'){
            InitHuffman(T,TreeState,num);
        }else
        if(command=='E'||command=='e'){
            EncodingTree(T,TreeState,num);
        }else
        if(command=='D'||command=='d'){
            DecodingTree(T,num);
        }else
        if(command=='P'||command=='p'){
            PrintCode();
        }else
        if(command=='T'||command=='t'){
            PrintTree(T,num);
        }else{
        cout<<"指令不存在,请重复输入\n";
        
        }
        PrintUI(TreeState);
    }
    return 0;
}