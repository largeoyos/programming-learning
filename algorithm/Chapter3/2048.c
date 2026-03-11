#include<stdio.h>
#include<time.h>
#include<Windows.h>
int main()
{
int W[16]={0},D =0   ,*R,*L,X,Y,S,K=0;      srand((0,(    unsigned)time(NULL)));
while(1){for(int c   =rand() % 32 + 1,     f=0,i  =0;c>   0;(++i,i%= 16) || (!f 
         ?exit   (0),0    :0))W[    i]?0:   (f=1,   ((c-=      2)<=0?
         W[i]=   c + 2    :0 ));   system  ("cls")   ;for(     int i=0;
         i<16;   i++){    W[i]?   printf   ("%d\t"   ,1<<W      [i] ):  
  printf(" \t");i%4     ==3?   printf  ("\n"):0;}for(;(K&   (1<<D))^( (GetKeyState
  (VK_LEFT+D)&0x8000   )>>(    15- D  ))?K=(K&(1<<D))^(1    <<D):1;++D,D %=4);for
  (Y=X=          0,S=    (D%2?         4*(2-    D):1-     D);(X<
  3?0:(          X =0,    ++Y))         ,Y<4;    L+=S,     ++ X){
  X?0:(          L=(R=    D%2?W         +Y+(D    ==3?      12:0):
  W+4*Y+(D==2? 3:0))   +S);* L?(!* R ?(*         R=*L,    *L =0 ):*( (* R == *   
  L?(++*R,*L=0):(L!=   R+S?(R [S] =*L,*L         =0) :   (0))),R += S)):(0);};};
}