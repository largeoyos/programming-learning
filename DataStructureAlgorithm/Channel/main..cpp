#include<iostream>
using namespace std;
#define MAX_VERTEX 100
#define INF 0x3f3f3f3f
typedef char VertexType;
typedef float EdgeType;
typedef struct EdgeNode{
    int adjVex;
    EdgeType weight;
    struct EdgeNode *next;
} EdgeNode;

typedef struct {
    VertexType data;
    EdgeNode *firstEdge;

}VertexNode;
typedef struct {
    VertexNode adjList[MAX_VERTEX];
    int vexNum,edgeNum;
}ALGraph;
void InitGraph_AL(ALGraph &G);
void CreateGraph_AL(ALGraph &G,bool directed,bool weighted);
void DestroyGraph_AL(ALGraph &G);
int LocateVex_AL(ALGraph G,VertexType v);

void InitGraph_AL(ALGraph &G){
    G.vexNum=0;
    G.edgeNum=0;
    for(int i=0;i<MAX_VERTEX;i++){
        G.adjList[i].firstEdge=nullptr;
    
    }
}
int LocateVex_AL(ALGraph G,VertexType v){
    for(int i=0;i<G.vexNum;i++){
        if(G.adjList[i].data==v){
            return i;
        }
    }
    return -1;
}
void CreateGraph_AL(ALGraph &G,bool directed,bool weighted){
    cout<<"Input VexNum and EdgeNum";
    cin>>G.vexNum>>G.edgeNum;
    cout<<"Input "<<G.vexNum<<" vexs";
    for(int i=0;i<G.vexNum;i++){
        cin>>G.adjList[i].data;
        G.adjList[i].firstEdge=nullptr;
    }
    cout<<"Input "<<G.edgeNum<<" edges"<<endl;
    for(int k=0;k<G.edgeNum;k++){
        VertexType u,v;
        EdgeType w=1;
        cin>>u>>v;
        if(weighted)cin>>w;
        int i=LocateVex_AL(G,u);
        int j=LocateVex_AL(G,v);
        if(i==-1||j==-1)continue;
        EdgeNode*e1=new EdgeNode;
        e1->adjVex=j;
        e1->weight=w;
        e1->next=G.adjList[i].firstEdge;
        G.adjList[i].firstEdge=e1;
        if(!directed){
           EdgeNode*e2=new EdgeNode;
            e2->adjVex=i;
            e2->weight=w;
            e2->next=G.adjList[j].firstEdge;
            G.adjList[j].firstEdge=e2;
        }
    }
    

}

void Prim_AL(ALGraph G,VertexType startVex){
    int startIdx=LocateVex_AL(G,startVex);
    if(startIdx==-1){
        cout<<"Error Occurred"<<endl;
        return;

    }
    float lowcost[MAX_VERTEX];
    int parent[MAX_VERTEX];
    bool visted[MAX_VERTEX];
    for(int i=0;i<G.vexNum;i++){
        lowcost[i]=INF;
        parent[i]=-1;
        visted[i]=false;
    }
    lowcost[startIdx]=0;
    cout<<"Output:"<<endl;
    float totalweight=0;
    for(int i=0;i<G.vexNum;i++){
        int u=-1;
        float minWeight=INF;
        for(int v=0;v<G.vexNum;v++){
            if(!visted[v]&&lowcost[v]<minWeight){
                minWeight=lowcost[v];
                u=v;

            }
        }
        if(u==-1)break;
        visted[u]=true;
        totalweight+=minWeight;
        if(parent[u]!=-1){
            cout<<G.adjList[parent[u]].data<<"-("<<minWeight<<")->"<<G.adjList[u].data<<endl;
        }
        EdgeNode *p=G.adjList[u].firstEdge;
        while(p!=nullptr){
            int v=p->adjVex;
            if(!visted[v]&&p->weight<lowcost[v]){
                lowcost[v]=p->weight;
                parent[v]=u;
            }
            p=p->next;
        }
    }
    
}


int main(){
    ALGraph G;
    InitGraph_AL(G);
    CreateGraph_AL(G,0,1);
    Prim_AL(G,'A');
    return 0;
}