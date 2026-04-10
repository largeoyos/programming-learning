#include<stdio.h>
#include<string.h>


char upper[105];
char lower[105];

int check(char s1[], char s2[]){
	int len1=strlen(s1);
	int len2=strlen(s2);
	for(int offset=0;offset<len1;offset++){
		bool ok=true;
		for(int i=0;i+offset<len1&&s2[i];i++){
			if(s1[offset+i]-'0'+s2[i]-'0'==4){
				ok=false;
				break;
			}

		}
		if(ok==true){
			return len2+offset>len1?len2+offset:len1;
		}

	}
	return len1+len2;
}

int main(){
    while(scanf("%s",upper)==1){
        scanf("%s",lower);
        int len1=check(upper,lower);
        int len2=check(lower,upper);
        printf("%d\n",len1<=len2?len1:len2);
    }
    return 0;
}

// 2. 别忘了“两个方向”
// 这是很多同学（包括厉害的同学）最容易漏掉的点：

// 情况 A：s1 在左，s2 往右滑。

// 情况 B：s2 在左，s1 往右滑。

// 优雅的处理技巧：
// 你不需要写两套复杂的逻辑。写一个函数 int get_length(char* a, char* b)，计算“a 固定，b 尝试往右靠”的最短长度。然后在 main 函数里：

// C
// int len1 = get_length(s1, s2);
// int len2 = get_length(s2, s1);
// printf("%d\n", len1 < len2 ? len1 : len2);
//ai太牛逼了!!!
