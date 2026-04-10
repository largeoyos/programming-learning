<!-- #include<stdio.h>
#include <iostream>
#include<string.h>


using namespace std; -->
# 数据结构第四次作业
PB25151789 刘广易
### 作业5.1

0,0,0,0  0,0,0,1  0,0,0,2  0,0,1,0  0,0,1,1  0,0,1,2  0,0,2,0  0,0,2,1  0,0,2,2
0,1,0,0  0,1,0,1  0,1,0,2  0,1,1,0  0,1,1,1  0,1,1,2  0,1,2,0  0,1,2,1  0,1,2,2
1,0,0,0  1,0,0,1  1,0,0,2  1,0,1,0  1,0,1,1  1,0,1,2  1,0,2,0  1,0,2,1  1,0,2,2
1,1,0,0  1,1,0,1  1,1,0,2  1,1,1,0  1,1,1,1  1,1,1,2  1,1,2,0  1,1,2,1  1,1,2,2


### 作业5.2
(1) 48*6=288字节
(2) 1000+(5*8+7)*6=1000+78*6=1468
(3) 1000+(1*8+3)*6=1000+11*6=1066
(4) 1000+(3*8+2)*6=1000+26*6=1156

### 作业5.3
$$f_{1}=-\frac{i^2}{2}+(n-\frac{1}{2})i$$
$$f{2}=j$$
$$c=1$$

### 作业5.4
$k = 2i + (j \pmod 2)$

### 作业5.5

(1) 三元数组表


|||||
|---|---|---|---|
|0|4|5|7|
|1|0|1|1|
|2|0|4|5|
|3|1|0|2|
|4|1|1|3|
|5|1|3|6|
|6|3|1|4|
|7|3|4|7|

(2)十字链表

行头指针 $R_0, R_1, R_2, R_3$\
列头指针 $C_0, C_1, C_2, C_3, C_4$
    
行链表
$R_0 \rightarrow (0,1,1) \rightarrow (0,4,5) \rightarrow \text{NULL}$\
$R_1 \rightarrow (1,0,2) \rightarrow (1,1,3) \rightarrow (1,3,6) \rightarrow \text{NULL}$\
$R_2 \rightarrow \text{NULL}$ \
$R_3 \rightarrow (3,1,4) \rightarrow (3,4,7) \rightarrow \text{NULL}$
        
列链接
    
$C_0 \rightarrow (1,0,2) \rightarrow \text{NULL}$\
$C_1 \rightarrow (0,1,1) \rightarrow (1,1,3) \rightarrow (3,1,4) \rightarrow \text{NULL}$\
$C_2 \rightarrow \text{NULL}$\
$C_3 \rightarrow (1,3,6) \rightarrow \text{NULL}$\
$C_4 \rightarrow (0,4,5) \rightarrow (3,4,7) \rightarrow \text{NULL}$
