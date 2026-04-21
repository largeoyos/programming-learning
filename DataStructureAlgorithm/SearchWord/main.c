#include<stdio.h>
#include<string.h>
int next[10005];
void get_next(char word[],int next[]){
    int j=0;
    int k=-1;
    next[0]=-1;
    int len=strlen(word);
    while(j<=len-1){
        if(k==-1||word[j]==word[k]){
            j++;
            k++;
            if(word[j]!=word[k]){
                next[j]=k;
            }else{
                next[j]=next[k];
            }
        }else k=next[k];
    }
}

int my_index(char line[],char word[]){
    int i=0;
    int j=0;
    int len_line=strlen(line);
    int len_word=strlen(word);
    while(i<len_line&&j<len_word){
        if(j==-1||line[i]==word[j]){
            i++;
            j++;
        }else{
            j=next[j];
        }
    }
    if(j==len_word){
        return i-j;
    }
    return -1;
}


int main(){
    char name[1000];
    char word[1000];
    while(scanf("%s %s",name,word)==2){
        char line[10005];
        get_next(word,next);
        FILE *fp;
        int linenum=1;
        fp=fopen(name,"r");
        if(fp==NULL){
            printf("Error Occurred!");
            return -1;
        }
        while(fgets(line,10005,fp)!=NULL){
            if(my_index(line,word)!=-1){
                printf("%s %d\n",line,linenum);
            }
            linenum++;
        }
    }
    return 0;
}