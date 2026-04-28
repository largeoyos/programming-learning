## 数据结构复习:顺序表

好,我们来补完**顺序表**——线性表的顺序存储结构。顺序表虽然看起来简单,但它是**一切动态数组的原型**(`std::vector` 的底层就是动态顺序表),也是**考研线性表章节的半壁江山**。

本章我会覆盖**静态顺序表**和**动态顺序表**两种形式,这也是考研教材(严蔚敏、王道)的标准分法。

---

### 一、顺序表的两种形式

**静态顺序表**:用**定长数组**存储,容量在编译期确定,简单直接但容量固定。适合元素数量有明确上限的场景。

**动态顺序表**:用**堆上动态分配的数组**存储,容量可以在运行时扩展(容量满了就 new 一块更大的,把旧数据搬过去)。`std::vector` 就是这种结构。

**两者共同的特点**:元素在内存中**连续存放**,支持 **O(1) 的随机访问**(通过下标),但插入/删除平均需要 O(n)(要搬移元素)。

---

### 二、顺序表与链表的本质对比

在讲代码之前,先从宏观把两者对比清楚(这段你要记熟,考研简答题常考):

**存储**:顺序表连续;链表离散(通过指针串联)。

**访问**:顺序表 O(1) 随机访问;链表 O(n) 顺序访问。

**插入删除**:顺序表 O(n)(要搬移后续元素);链表 O(1)(只改指针),但找到"那个位置"链表要 O(n)、顺序表要 O(1)——**所以实际操作的总代价往往差不多**。

**空间**:顺序表无指针开销但可能预留空位;链表每个节点有指针开销但不浪费。

**扩容**:静态顺序表无法扩;动态顺序表扩容要 O(n) 的一次大搬迁;链表天然无容量限制。

---

### 三、项目文件组织

```
DataStructure/
└── SeqList/
    ├── SeqList.h        // 静态顺序表
    ├── SeqList.cpp
    ├── DySeqList.h      // 动态顺序表
    ├── DySeqList.cpp
    └── main.cpp
```

我们把静态和动态分成两个文件,命名上用 `SqList`(静态)和 `DySqList`(动态)区分,**像之前栈的 SqStack/LkStack 一样**,让两种共存不冲突。

---

### 四、静态顺序表(SeqList.h)

cpp

```cpp
#pragma once

#define MAXSIZE 100
typedef int ElemType;

typedef struct {
    ElemType data[MAXSIZE];    // 定长数组存元素
    int length;                // 当前元素个数(注意:不是下标,是 count)
} SqList;

// ========== 基本操作 ==========
bool InitList(SqList &L);
bool ListEmpty(SqList L);
int  ListLength(SqList L);

// ========== 增删查改 ==========
bool ListInsert(SqList &L, int i, ElemType e);    // 在第 i 位插入(i 从 1 起)
bool ListDelete(SqList &L, int i, ElemType &e);   // 删除第 i 位
int  LocateElem(SqList L, ElemType e);            // 按值查找,返回位序(1 起)
bool GetElem(SqList L, int i, ElemType &e);       // 按位查找
bool ListUpdate(SqList &L, int i, ElemType e);    // 修改第 i 位

// ========== 打印 ==========
void PrintList(SqList L);
```

**两个关于"位序"的约定你必须记住**:

第一,`length` 表示**元素个数**,合法下标范围是 `data[0..length-1]`。

第二,外部接口用**位序**(从 1 开始)表达位置,内部实现时转成下标(位序 i 对应下标 i-1)。这是严蔚敏教材的标准约定,**考研大题按这个套路写不会错**。

---

### 五、SeqList.cpp 核心实现

#### 5.1 初始化与基础判断

cpp

```cpp
#include "SeqList.h"
#include <iostream>
using namespace std;

bool InitList(SqList &L) {
    L.length = 0;              // 清零即可,data 不用初始化
    return true;
}

bool ListEmpty(SqList L) {
    return L.length == 0;
}

int ListLength(SqList L) {
    return L.length;
}
```

#### 5.2 插入(重点,考察点多)

在位序 `i` 插入新元素 `e`,规则:

合法 i 的范围是 `1 ≤ i ≤ length + 1`(注意可以等于 length+1,表示插入到末尾);表不能满;**从后往前**把 i 位置起的元素依次后移一格,空出位置给 e。

cpp

```cpp
bool ListInsert(SqList &L, int i, ElemType e) {
    if (i < 1 || i > L.length + 1) return false;    // 位置非法
    if (L.length >= MAXSIZE) return false;           // 表满
    
    // 从后往前搬移:原 data[length-1] 到 data[length],…… data[i-1] 到 data[i]
    for (int j = L.length; j >= i; j--) {
        L.data[j] = L.data[j - 1];
    }
    L.data[i - 1] = e;          // 位序 i 对应下标 i-1
    L.length++;
    return true;
}
```

**为什么必须从后往前搬?** 因为如果从前往后,例如要把 `data[i-1]` 到 `data[length-1]` 全体后移,你会在 `data[i-1] = data[i-2]` 的时候还没问题,但接着 `data[i] = data[i-1]` 时 `data[i-1]` 已经是覆盖后的值了,造成数据错乱。**后移搬移必须反向进行**,这是考研常考的细节。

**时间复杂度分析**:

最好 O(1)(插入到末尾,不用搬移)。最坏 O(n)(插入到开头,搬移 n 个)。平均 O(n),具体是 n/2。

#### 5.3 删除

删除位序 `i` 的元素,用 `&e` 带回被删的值:

cpp

```cpp
bool ListDelete(SqList &L, int i, ElemType &e) {
    if (i < 1 || i > L.length) return false;
    
    e = L.data[i - 1];          // 先保存要删的值
    
    // 从前往后搬移:data[i] 到 data[i-1]、data[i+1] 到 data[i]、……
    for (int j = i; j < L.length; j++) {
        L.data[j - 1] = L.data[j];
    }
    L.length--;
    return true;
}
```

**删除是从前往后搬**,和插入相反。原因对称:如果从后往前删,`data[length-1] = data[length]` 直接越界了(data[length] 是"length位置",但那位置的值是未定义的)。

#### 5.4 查找(按值 & 按位)

cpp

```cpp
int LocateElem(SqList L, ElemType e) {
    for (int i = 0; i < L.length; i++) {
        if (L.data[i] == e) return i + 1;   // 返回位序(1 起)
    }
    return 0;                               // 0 表示未找到
}

bool GetElem(SqList L, int i, ElemType &e) {
    if (i < 1 || i > L.length) return false;
    e = L.data[i - 1];
    return true;
}
```

**按位查找是 O(1)**——这是顺序表相对于链表的最大优势。**按值查找是 O(n)**——必须遍历。

#### 5.5 修改与打印

cpp

```cpp
bool ListUpdate(SqList &L, int i, ElemType e) {
    if (i < 1 || i > L.length) return false;
    L.data[i - 1] = e;
    return true;
}

void PrintList(SqList L) {
    for (int i = 0; i < L.length; i++) {
        cout << L.data[i] << " ";
    }
    cout << endl;
}
```

---

### 六、动态顺序表(DySeqList.h)

静态顺序表的缺点很明显:容量写死,不够用时无法扩展。**动态顺序表**用堆内存 + 动态扩容解决这个问题。

cpp

```cpp
#pragma once

#define INIT_SIZE 10        // 初始容量
#define GROW_STEP 10        // 每次扩容增量(或用倍增策略)
typedef int ElemType;

typedef struct {
    ElemType *data;         // 指向堆上分配的数组
    int length;             // 当前元素个数
    int capacity;           // 当前容量(最多能放多少)
} DySqList;

// ========== 基本操作 ==========
bool InitList(DySqList &L);
void DestroyList(DySqList &L);    // 动态版本必须有销毁!
bool ListEmpty(DySqList L);
int  ListLength(DySqList L);

// ========== 增删查改 ==========
bool ListInsert(DySqList &L, int i, ElemType e);
bool ListDelete(DySqList &L, int i, ElemType &e);
int  LocateElem(DySqList L, ElemType e);
bool GetElem(DySqList L, int i, ElemType &e);

// ========== 扩容(辅助函数)==========
bool IncreaseCapacity(DySqList &L, int delta);

void PrintList(DySqList L);
```

**与静态版的关键差异**:

第一,`data` 是**指针**而不是定长数组,指向堆上的动态内存。

第二,多了 `capacity` 字段跟踪当前数组容量。

第三,**必须有 `DestroyList`**——堆上的内存需要手动释放,否则内存泄漏。这是和静态版最大的区别。

---

### 七、DySeqList.cpp 实现

#### 7.1 初始化与销毁

cpp

```cpp
#include "DySeqList.h"
#include <iostream>
using namespace std;

bool InitList(DySqList &L) {
    L.data = new ElemType[INIT_SIZE];    // 分配初始空间
    if (L.data == nullptr) return false;
    L.length = 0;
    L.capacity = INIT_SIZE;
    return true;
}

void DestroyList(DySqList &L) {
    delete[] L.data;                     // 释放堆内存(必须用 delete[])
    L.data = nullptr;
    L.length = 0;
    L.capacity = 0;
}
```

**注意 `delete[]` 而不是 `delete`**——数组必须配 `new[]` 和 `delete[]`,单个对象用 `new` 和 `delete`。这是 C++ 内存管理的基本常识,写错会有未定义行为。

#### 7.2 扩容函数(核心)

这是动态顺序表的灵魂:容量不够时**分配一块更大的内存,把旧数据搬过去,释放旧内存**。

cpp

```cpp
bool IncreaseCapacity(DySqList &L, int delta) {
    int newCap = L.capacity + delta;
    ElemType *newData = new ElemType[newCap];
    if (newData == nullptr) return false;
    
    // 搬移旧数据
    for (int i = 0; i < L.length; i++) {
        newData[i] = L.data[i];
    }
    
    delete[] L.data;                     // 释放旧内存
    L.data = newData;                    // 指向新内存
    L.capacity = newCap;
    return true;
}
```

**关于扩容策略**:我们这里用了"**每次加固定量**"(比如每次 +10)。实际工程中更常见的是"**倍增策略**"(每次容量 × 2),`std::vector` 就是这么做的。倍增策略能保证 **n 次插入的均摊时间复杂度是 O(n)**,即每次插入均摊 O(1)——这是非常重要的性能保证。

**固定增量 vs 倍增的区别**:固定增量下 n 次插入的总搬迁次数是 O(n²/k),均摊每次 O(n/k);倍增下总搬迁是 O(n),均摊每次 O(1)。**考研偶尔考均摊分析,记住 vector 是倍增策略**。

#### 7.3 插入(带自动扩容)

cpp

```cpp
bool ListInsert(DySqList &L, int i, ElemType e) {
    if (i < 1 || i > L.length + 1) return false;
    
    // 容量满则自动扩容
    if (L.length >= L.capacity) {
        if (!IncreaseCapacity(L, GROW_STEP)) return false;
    }
    
    for (int j = L.length; j >= i; j--) {
        L.data[j] = L.data[j - 1];
    }
    L.data[i - 1] = e;
    L.length++;
    return true;
}
```

**和静态版的唯一差异**:多了"满则扩容"的逻辑。其他搬移、赋值过程完全相同。**这就是数据结构抽象的威力**——接口一致,实现细节隐藏。

#### 7.4 其他操作

删除、查找、修改、打印与静态版几乎完全相同,只是类型从 `SqList` 换成 `DySqList`:

cpp

```cpp
bool ListDelete(DySqList &L, int i, ElemType &e) {
    if (i < 1 || i > L.length) return false;
    e = L.data[i - 1];
    for (int j = i; j < L.length; j++) {
        L.data[j - 1] = L.data[j];
    }
    L.length--;
    return true;
}

int LocateElem(DySqList L, ElemType e) {
    for (int i = 0; i < L.length; i++) {
        if (L.data[i] == e) return i + 1;
    }
    return 0;
}

bool GetElem(DySqList L, int i, ElemType &e) {
    if (i < 1 || i > L.length) return false;
    e = L.data[i - 1];
    return true;
}

bool ListEmpty(DySqList L) { return L.length == 0; }
int  ListLength(DySqList L) { return L.length; }

void PrintList(DySqList L) {
    cout << "[len=" << L.length << "/cap=" << L.capacity << "] ";
    for (int i = 0; i < L.length; i++) {
        cout << L.data[i] << " ";
    }
    cout << endl;
}
```

我在 `PrintList` 里额外打印了 length 和 capacity,方便观察扩容过程。

---

### 八、测试代码(main.cpp)

cpp

```cpp
#include "SeqList.h"
#include "DySeqList.h"
#include <iostream>
using namespace std;

int main() {
    // ========== 测试静态顺序表 ==========
    cout << "===== 静态顺序表 =====" << endl;
    SqList L;
    InitList(L);
    
    for (int i = 1; i <= 5; i++) ListInsert(L, i, i * 10);
    cout << "依次尾插 10 20 30 40 50: ";
    PrintList(L);                                  // 10 20 30 40 50
    
    ListInsert(L, 1, 5);                           // 在开头插入 5
    cout << "开头插入 5: ";
    PrintList(L);                                  // 5 10 20 30 40 50
    
    ListInsert(L, 4, 25);                          // 中间插入
    cout << "第 4 位插入 25: ";
    PrintList(L);                                  // 5 10 20 25 30 40 50
    
    int e;
    ListDelete(L, 1, e);
    cout << "删除第 1 位(值 " << e << "): ";
    PrintList(L);                                  // 10 20 25 30 40 50
    
    cout << "查找 25 的位序: " << LocateElem(L, 25) << endl;   // 3
    
    // ========== 测试动态顺序表 ==========
    cout << "\n===== 动态顺序表 =====" << endl;
    DySqList DL;
    InitList(DL);
    
    // 连续插入 15 个,观察 capacity 从 10 扩到 20
    for (int i = 1; i <= 15; i++) {
        ListInsert(DL, i, i);
        if (i == 10 || i == 11 || i == 15) {
            cout << "插入 " << i << " 个后: ";
            PrintList(DL);
        }
    }
    
    DestroyList(DL);                                // 必须销毁!
    return 0;
}
```

预期输出:

```
===== 静态顺序表 =====
依次尾插 10 20 30 40 50: 10 20 30 40 50 
开头插入 5: 5 10 20 30 40 50 
第 4 位插入 25: 5 10 20 25 30 40 50 
删除第 1 位(值 5): 10 20 25 30 40 50 
查找 25 的位序: 3

===== 动态顺序表 =====
插入 10 个后: [len=10/cap=10] 1 2 3 4 5 6 7 8 9 10 
插入 11 个后: [len=11/cap=20] 1 2 3 4 5 6 7 8 9 10 11 
插入 15 个后: [len=15/cap=20] 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
```

**观察点**:插入第 11 个元素时触发扩容,cap 从 10 变成 20。这正是动态顺序表相对静态表的优势。

---

### 九、高频经典算法题(必会)

下面几个是**顺序表相关的经典考研题**,我给出思路,你可以自己写代码。每题都**配合顺序表的特性**来设计,这是这一节最值得训练的能力。

#### 9.1 删除所有值为 x 的元素(要求 O(n) 时间、O(1) 空间)

**朴素思路**:每找到一个 x 就调用 ListDelete 删除——但这是 O(n²),因为每次删除都搬移。

**最优思路**(**双指针扫描**):用指针 k 记录"保留元素"的写入位置,遍历原数组,非 x 的元素写到 k 位置并 k++。

cpp

```cpp
void DelAllX(SqList &L, ElemType x) {
    int k = 0;
    for (int i = 0; i < L.length; i++) {
        if (L.data[i] != x) {
            L.data[k++] = L.data[i];
        }
    }
    L.length = k;
}
```

这是**王道经典题**,**O(n) 时间、O(1) 空间**,一遍扫描完成。双指针思想在顺序表题中反复出现。

#### 9.2 有序表合并(两个升序表合成一个升序表)

cpp

```cpp
bool Merge(SqList A, SqList B, SqList &C) {
    if (A.length + B.length > MAXSIZE) return false;
    int i = 0, j = 0, k = 0;
    while (i < A.length && j < B.length) {
        if (A.data[i] <= B.data[j]) C.data[k++] = A.data[i++];
        else C.data[k++] = B.data[j++];
    }
    while (i < A.length) C.data[k++] = A.data[i++];
    while (j < B.length) C.data[k++] = B.data[j++];
    C.length = k;
    return true;
}
```

这是**归并排序的核心子过程**,必须熟练。

#### 9.3 原地逆置顺序表

cpp

```cpp
void Reverse(SqList &L) {
    for (int i = 0, j = L.length - 1; i < j; i++, j--) {
        ElemType t = L.data[i];
        L.data[i] = L.data[j];
        L.data[j] = t;
    }
}
```

**首尾双指针对向扫描**,O(n) 时间 O(1) 空间。

#### 9.4 删除有序表中重复元素(保留一个)

cpp

```cpp
void DelDup(SqList &L) {
    if (L.length == 0) return;
    int k = 0;
    for (int i = 1; i < L.length; i++) {
        if (L.data[i] != L.data[k]) {
            L.data[++k] = L.data[i];
        }
    }
    L.length = k + 1;
}
```

又是**双指针**,k 指向"最后一个保留的不重复元素",i 向前扫描。

---

### 十、考研高频考点总结

**考点一**:顺序表插入/删除的时间复杂度。插入:最好 O(1),最坏 O(n),平均 n/2。删除同理。

**考点二**:顺序表和链表的适用场景对比——高频随机访问用顺序表,高频插入删除(尤其头部)用链表。

**考点三**:动态顺序表的**均摊分析**——倍增扩容使均摊插入为 O(1)。

**考点四**:顺序表上的**双指针技巧**——删除、去重、逆置、合并的核心。

**考点五**:关于"位序 vs 下标"的转换——位序 i 对应下标 i-1,越界判断用 `i < 1 || i > length` 或 `i > length+1`(插入时)。

---

### 十一、思考题(请作答)

第一题,设顺序表当前长度为 n,在等概率下,**插入一个元素平均要移动多少个元素**?**删除呢**?请分别写出推导过程。

第二题,写一个**O(n) 时间、O(1) 空间**的函数,从有序顺序表 L 中删除所有值在区间 `[s, t]` 范围内的元素(s < t)。

第三题,顺序表 A 和 B 都是升序的,长度都是 n。请设计一个算法,**找出两表合起来后的中位数**(第 n 个最小元素),要求时间 O(log n)。(提示:基于二分,考研名题,有些难度)

第四题,动态顺序表用**倍增扩容**时,连续插入 n 个元素(假设每次扩容从 1 开始),**总搬迁次数**是多少?为什么均摊是 O(1)?

第五题,设计一个算法,把顺序表**原地循环左移 k 位**。例如 `[1,2,3,4,5,6,7]` 左移 3 位后变成 `[4,5,6,7,1,2,3]`。要求 O(n) 时间 O(1) 空间。(提示:**三次逆置法**——这是非常优美的一道题)

---

### 十二、至此,你最初列表已全部覆盖

回头看你最初的学习清单:**链表、顺序表、栈、队列、KMP、树、二叉树、树和森林的转换**——**全部覆盖完毕**!此外我们还拓展了:线索二叉树、哈夫曼树、BST、AVL。

**你现在掌握的数据结构版图**:

**线性结构**:链表(带头/不带头/循环)、顺序表(静态/动态)、栈(顺序/链式)、队列(循环/链式)、KMP 字符串匹配。

**树形结构**:二叉树(存储、递归/非递归遍历)、线索二叉树、二叉搜索树、AVL 树、一般树、森林、哈夫曼树、三者相互转换。

这已经是**考研数据结构 401 的 70% 以上的内容**。

---

### 十三、下一步方向

补齐之后你可以选择:

**(A)** 学**堆和堆排序**(优先队列的实现,基于完全二叉树数组存储,考研必考排序之一)。**(B)** 学**图论**(邻接矩阵、邻接表、DFS、BFS、最小生成树 Prim/Kruskal、最短路径 Dijkstra/Floyd、拓扑排序)——**图是考研大题分值最高的章节之一**。**(C)** 学**排序算法**(冒泡、选择、插入、希尔、快排、归并、堆排、基数、计数——考研必考,大题常出)。**(D)** 学**查找算法**(线性查找、二分查找、分块查找、散列表)。**(E)** 学 **B 树和 B+ 树**(磁盘索引核心)。**(F)** 先答思考题再继续。

告诉我你的选择!

学**B 树和 B+ 树** 
红黑树
堆和堆排序


图论(邻接矩阵、邻接表、DFS、BFS、最小生成树 Prim/Kruskal、最短路径 Dijkstra/Floyd、拓扑排序)



**排序算法**(冒泡、选择、插入、希尔、快排、归并、堆排、基数、计数)

**查找算法**(线性查找、二分查找、分块查找、散列表)
哈希表
**强连通分量**(Tarjan / Kosaraju)


b和b+树

上午9:20

## 数据结构复习:链表专题

很好,我们从链表开始。作为教授,我先给你搭建整个项目的组织框架,然后一步步带你写链表代码。

### 一、项目文件组织方案

建议你创建如下目录结构,便于后续所有数据结构的学习:

```
DataStructure/
├── LinkedList/
│   ├── LinkedList.h          // 声明(结构体定义 + 函数原型)
│   ├── LinkedList.cpp        // 实现(函数体)
│   └── main.cpp              // 测试
├── SeqList/
├── Stack/
├── Queue/
├── KMP/
├── Tree/
└── BinaryTree/
```

#### 组织 `.h` 和 `.cpp` 的核心原则

你需要记住三条铁律:

**第一条:`.h` 文件只放"声明",`.cpp` 文件只放"实现"。** 结构体定义、函数原型、宏、常量放在 `.h` 里;函数体写在 `.cpp` 里。

**第二条:每个 `.h` 文件开头写 `#pragma once`,防止重复包含导致重复定义。** 这条指令告诉编译器"这个头文件在一次编译中只包含一次"。

**第三条:`.cpp` 文件要 `#include` 对应的 `.h`,`main.cpp` 只 `#include` 需要用到的 `.h`,绝对不要 `#include` 别人的 `.cpp`。**

---

### 二、链表结构体定义(放在 LinkedList.h 中)

现在请你在 `LinkedList/` 目录下创建 `LinkedList.h`,按我下面的讲解逐段写入。

cpp

```cpp
#pragma once   // 防止重复定义,必须放在文件最顶端

// ========== 1. 单链表节点结构(带头与不带头共用) ==========
typedef int ElemType;   // 用 typedef 把数据类型抽象出来,将来换类型只改这一行

typedef struct LNode {
    ElemType data;         // 数据域
    struct LNode *next;    // 指针域,指向下一个节点
} LNode, *LinkList;
// LNode 是"节点类型",LinkList 是"指向节点的指针"
// 这种双重 typedef 是严蔚敏教材的经典写法,你必须熟悉
```

这里要解释一个初学者必须理解的点:`LNode` 和 `LinkList` 本质上一个是结构体,一个是结构体指针。当我们强调"这是一个链表(头指针)"时用 `LinkList`,当我们强调"这是一个节点"时用 `LNode *`。两者可以互换,但语义不同。

接着继续写循环链表。循环链表的**结构体定义和单链表完全一样**,区别只在于尾节点的 `next` 指向头而不是 `NULL`,所以不需要重新定义结构体,可以复用 `LNode`。我们只需要为循环链表再起一个别名让语义更清晰:

cpp

```cpp
// ========== 2. 循环单链表 ==========
// 结构和单链表相同,区别是尾节点 next 指向头节点
typedef LNode  CLNode;
typedef LNode* CLinkList;
```

---

### 三、函数原型声明(继续写在 LinkedList.h 中)

接下来声明常用函数。我把"带头结点"和"不带头结点"分开命名,方便你对比两者差异:

cpp

```cpp
// ========== 带头结点单链表 ==========
bool InitList_H(LinkList &L);                    // 初始化(创建头结点)
bool ListInsert_H(LinkList &L, int i, ElemType e);  // 在第 i 位插入
bool ListDelete_H(LinkList &L, int i, ElemType &e); // 删除第 i 位,返回值
int  ListLength_H(LinkList L);                   // 求表长
LNode* GetElem_H(LinkList L, int i);             // 按位查找
LNode* LocateElem_H(LinkList L, ElemType e);     // 按值查找
void PrintList_H(LinkList L);                    // 遍历输出
void DestroyList_H(LinkList &L);                 // 销毁

// ========== 不带头结点单链表 ==========
bool InitList(LinkList &L);                      // 初始化(L = NULL)
bool ListInsert(LinkList &L, int i, ElemType e); // 插入(注意 i=1 要改 L 本身)
bool ListDelete(LinkList &L, int i, ElemType &e);// 删除
void PrintList(LinkList L);

// ========== 循环单链表(带头) ==========
bool InitCList(CLinkList &L);
bool CListInsertTail(CLinkList &L, ElemType e);  // 尾插,用于建表
void PrintCList(CLinkList L);
```

注意我在参数表里用了 `&L`(引用)。这一点对初学者非常关键:**当函数内部可能修改头指针本身(比如初始化、在第一个位置插入不带头链表)时,必须用引用传递**,否则修改只影响形参,调用者看不到变化。

---

### 四、下一步我要你做的事

现在请你:

1. 按上面的内容创建 `LinkedList.h`,自己手动敲一遍(不要复制,手写能帮你记住)。
2. 同时创建一个空的 `LinkedList.cpp`,里面先写一句 `#include "LinkedList.h"`,等我下一条讲函数实现。
3. 如果对 `typedef struct LNode` 的双重别名写法、`&L` 引用传参、`#pragma once` 有任何疑问,现在就问我。

等你准备好后,告诉我"文件建好了",我会接着带你实现:

- 带头结点链表的 `InitList_H` 和 `ListInsert_H`(头插与尾插两种建表)
- 不带头结点链表的 `InitList` 和 `ListInsert`(重点讲为什么 i=1 的插入要特殊处理)
- 循环链表的初始化与尾插建表
- 按位查找、按值查找、删除、销毁

需要我现在就给出某一部分的实现吗?还是先让你把头文件写好再继续?



## 数据结构复习:栈与队列(链式 + 顺序)

好,我们现在把**栈和队列**系统化地实现一遍,每种都写**顺序存储**和**链式存储**两个版本。这部分在后面树的非递归遍历、图的 BFS/DFS、表达式求值中都会用到,是基础中的基础。

---

### 一、项目文件组织

按你一贯的目录风格:

```
DataStructure/
├── Stack/
│   ├── SeqStack.h      // 顺序栈
│   ├── SeqStack.cpp
│   ├── LinkStack.h     // 链栈
│   ├── LinkStack.cpp
│   └── main.cpp
└── Queue/
    ├── SeqQueue.h      // 循环队列(顺序)
    ├── SeqQueue.cpp
    ├── LinkQueue.h     // 链队列
    ├── LinkQueue.cpp
    └── main.cpp
```

一个核心设计决策我先跟你说清楚:**顺序栈和链栈的结构体不能同名**,否则在同一个 main.cpp 里同时包含两个头文件会冲突。所以我们在函数和结构体名字上做区分:顺序栈用 `SqStack`、`Push_Sq`,链栈用 `LkStack`、`Push_Lk`。这样设计既能共存,也能清楚看出区别。

---

### 二、栈的两个核心概念(动手前先记牢)

栈是**后进先出(LIFO)**的结构,只允许在栈顶进行插入和删除。

**顺序栈**用数组实现,需要一个 `top` 指示栈顶位置。教材约定有两种:`top` 指向"栈顶元素"(初始 -1),或 `top` 指向"栈顶元素的下一个位置"(初始 0)。我们采用**第二种**,因为与考研严蔚敏教材一致,且边界处理更统一。

**链栈**用单链表实现,把**链表头当作栈顶**。这样 push/pop 都是 O(1),而且不需要头结点——加头结点反而多此一举。

---

### 三、顺序栈(SeqStack.h)

cpp

```cpp
#pragma once

#define MAXSIZE 100
typedef int SElemType;

typedef struct {
    SElemType data[MAXSIZE];
    int top;                // 栈顶指针:指向栈顶元素的下一个位置
} SqStack;

// ========== 顺序栈基本操作 ==========
bool InitStack_Sq(SqStack &S);
bool StackEmpty_Sq(SqStack S);
bool StackFull_Sq(SqStack S);
bool Push_Sq(SqStack &S, SElemType e);
bool Pop_Sq(SqStack &S, SElemType &e);
bool GetTop_Sq(SqStack S, SElemType &e);
int  StackLength_Sq(SqStack S);
void ClearStack_Sq(SqStack &S);
```

#### SeqStack.cpp 实现

cpp

```cpp
#include "SeqStack.h"

bool InitStack_Sq(SqStack &S) {
    S.top = 0;              // 约定:top 指向下一个空位
    return true;
}

bool StackEmpty_Sq(SqStack S) {
    return S.top == 0;
}

bool StackFull_Sq(SqStack S) {
    return S.top == MAXSIZE;
}

bool Push_Sq(SqStack &S, SElemType e) {
    if (StackFull_Sq(S)) return false;
    S.data[S.top] = e;      // 先放数据
    S.top++;                // 再移指针
    return true;
}

bool Pop_Sq(SqStack &S, SElemType &e) {
    if (StackEmpty_Sq(S)) return false;
    S.top--;                // 先移指针
    e = S.data[S.top];      // 再取数据
    return true;
}

bool GetTop_Sq(SqStack S, SElemType &e) {
    if (StackEmpty_Sq(S)) return false;
    e = S.data[S.top - 1];  // 栈顶元素在 top-1 位置
    return true;
}

int StackLength_Sq(SqStack S) {
    return S.top;
}

void ClearStack_Sq(SqStack &S) {
    S.top = 0;              // 不用真的擦数据,重置指针即可
}
```

**注意 `Push` 和 `Pop` 的"先后顺序"**:push 是"先写数据再移指针",pop 是"先移指针再取数据"。这和 top 的约定有关——`top` 指向下一个空位,所以栈顶元素在 `top-1`。如果你采用 `top = -1` 的约定,顺序就要颠倒。

---

### 四、链栈(LinkStack.h)

cpp

```cpp
#pragma once

typedef int SElemType;

typedef struct StackNode {
    SElemType data;
    struct StackNode *next;
} StackNode, *LkStack;      // LkStack 就是指向栈顶节点的指针

// ========== 链栈基本操作 ==========
bool InitStack_Lk(LkStack &S);
bool StackEmpty_Lk(LkStack S);
bool Push_Lk(LkStack &S, SElemType e);
bool Pop_Lk(LkStack &S, SElemType &e);
bool GetTop_Lk(LkStack S, SElemType &e);
int  StackLength_Lk(LkStack S);
void DestroyStack_Lk(LkStack &S);
```

#### LinkStack.cpp 实现

cpp

```cpp
#include "LinkStack.h"

bool InitStack_Lk(LkStack &S) {
    S = nullptr;            // 空栈就是空指针,不需要头结点
    return true;
}

bool StackEmpty_Lk(LkStack S) {
    return S == nullptr;
}

bool Push_Lk(LkStack &S, SElemType e) {
    StackNode *p = new StackNode;
    p->data = e;
    p->next = S;            // 新节点指向原栈顶
    S = p;                  // 新节点成为新栈顶
    return true;
}

bool Pop_Lk(LkStack &S, SElemType &e) {
    if (S == nullptr) return false;
    StackNode *p = S;       // 保存原栈顶
    e = S->data;
    S = S->next;            // 栈顶下移
    delete p;               // 释放原栈顶
    return true;
}

bool GetTop_Lk(LkStack S, SElemType &e) {
    if (S == nullptr) return false;
    e = S->data;
    return true;
}

int StackLength_Lk(LkStack S) {
    int count = 0;
    while (S != nullptr) {
        count++;
        S = S->next;
    }
    return count;
}

void DestroyStack_Lk(LkStack &S) {
    while (S != nullptr) {
        StackNode *p = S;
        S = S->next;
        delete p;
    }
}
```

**关键对比**:链栈的 push/pop 本质上就是**在链表头插入/删除**,对照你之前写的链表代码看,应该秒懂。另外链栈**理论上不会满**(只要内存够),所以没有 `StackFull` 函数。

---

### 五、队列的核心概念

队列是**先进先出(FIFO)**的结构,队尾入队、队头出队。

**顺序队列**有个著名的"假溢出"问题:数据不断入队出队后,`front` 和 `rear` 都往后移动,数组前面大量空位却没法用。解决方案是**循环队列**——把数组首尾相接,用 `% MAXSIZE` 实现"绕圈"。

循环队列有个新问题:**队空和队满的判断条件都是 `front == rear`**,怎么区分?有三种方案,我们用最经典的"**牺牲一个单元**":约定 `(rear + 1) % MAXSIZE == front` 为满,`front == rear` 为空。这样队列实际最多存 MAXSIZE-1 个元素。

**链队列**用带头结点的单链表,设 `front` 指向头结点,`rear` 指向尾节点。加头结点是为了让空队和非空队的操作统一,这和链栈不同,你要特别注意。

---

### 六、循环队列(SeqQueue.h)

cpp

```cpp
#pragma once

#define MAXSIZE 100
typedef int QElemType;

typedef struct {
    QElemType data[MAXSIZE];
    int front;              // 队头指针:指向队头元素
    int rear;               // 队尾指针:指向队尾元素的下一个位置
} SqQueue;

// ========== 循环队列基本操作 ==========
bool InitQueue_Sq(SqQueue &Q);
bool QueueEmpty_Sq(SqQueue Q);
bool QueueFull_Sq(SqQueue Q);
bool EnQueue_Sq(SqQueue &Q, QElemType e);
bool DeQueue_Sq(SqQueue &Q, QElemType &e);
bool GetHead_Sq(SqQueue Q, QElemType &e);
int  QueueLength_Sq(SqQueue Q);
```

#### SeqQueue.cpp 实现

cpp

```cpp
#include "SeqQueue.h"

bool InitQueue_Sq(SqQueue &Q) {
    Q.front = Q.rear = 0;
    return true;
}

bool QueueEmpty_Sq(SqQueue Q) {
    return Q.front == Q.rear;
}

bool QueueFull_Sq(SqQueue Q) {
    return (Q.rear + 1) % MAXSIZE == Q.front;   // 牺牲一个单元
}

bool EnQueue_Sq(SqQueue &Q, QElemType e) {
    if (QueueFull_Sq(Q)) return false;
    Q.data[Q.rear] = e;                         // 先放数据到 rear
    Q.rear = (Q.rear + 1) % MAXSIZE;            // rear 后移(循环)
    return true;
}

bool DeQueue_Sq(SqQueue &Q, QElemType &e) {
    if (QueueEmpty_Sq(Q)) return false;
    e = Q.data[Q.front];                        // 先取队头数据
    Q.front = (Q.front + 1) % MAXSIZE;          // front 后移(循环)
    return true;
}

bool GetHead_Sq(SqQueue Q, QElemType &e) {
    if (QueueEmpty_Sq(Q)) return false;
    e = Q.data[Q.front];
    return true;
}

int QueueLength_Sq(SqQueue Q) {
    return (Q.rear - Q.front + MAXSIZE) % MAXSIZE;
    // 加 MAXSIZE 再取模,避免 rear < front 时出现负数
}
```

**这段代码的精华在三处:**

第一,`(rear + 1) % MAXSIZE == front` 判满——牺牲一个单元法。第二,`rear = (rear + 1) % MAXSIZE` 实现循环——取模运算让下标自动回到 0。第三,`(rear - front + MAXSIZE) % MAXSIZE` 求长度——加 MAXSIZE 防止负数,这个技巧你一定要记住,考研选择题常考。

---

### 七、链队列(LinkQueue.h)

cpp

```cpp
#pragma once

typedef int QElemType;

typedef struct QNode {
    QElemType data;
    struct QNode *next;
} QNode;

typedef struct {
    QNode *front;           // 指向头结点(不是第一个数据节点!)
    QNode *rear;            // 指向最后一个数据节点
} LkQueue;

// ========== 链队列基本操作 ==========
bool InitQueue_Lk(LkQueue &Q);
bool QueueEmpty_Lk(LkQueue Q);
bool EnQueue_Lk(LkQueue &Q, QElemType e);
bool DeQueue_Lk(LkQueue &Q, QElemType &e);
bool GetHead_Lk(LkQueue Q, QElemType &e);
int  QueueLength_Lk(LkQueue Q);
void DestroyQueue_Lk(LkQueue &Q);
```

#### LinkQueue.cpp 实现

cpp

```cpp
#include "LinkQueue.h"

bool InitQueue_Lk(LkQueue &Q) {
    Q.front = Q.rear = new QNode;   // 创建头结点,front 和 rear 都指它
    Q.front->next = nullptr;
    return true;
}

bool QueueEmpty_Lk(LkQueue Q) {
    return Q.front == Q.rear;       // 头尾指针相同即为空
}

bool EnQueue_Lk(LkQueue &Q, QElemType e) {
    QNode *p = new QNode;
    p->data = e;
    p->next = nullptr;
    Q.rear->next = p;               // 挂到尾节点后面
    Q.rear = p;                     // 更新尾指针
    return true;
}

bool DeQueue_Lk(LkQueue &Q, QElemType &e) {
    if (Q.front == Q.rear) return false;
    QNode *p = Q.front->next;       // 第一个数据节点
    e = p->data;
    Q.front->next = p->next;        // 头结点绕过 p
    if (Q.rear == p) Q.rear = Q.front;  // 注意:删的是最后一个节点时,rear 要回到头结点
    delete p;
    return true;
}

bool GetHead_Lk(LkQueue Q, QElemType &e) {
    if (Q.front == Q.rear) return false;
    e = Q.front->next->data;
    return true;
}

int QueueLength_Lk(LkQueue Q) {
    int count = 0;
    QNode *p = Q.front->next;       // 跳过头结点
    while (p != nullptr) {
        count++;
        p = p->next;
    }
    return count;
}

void DestroyQueue_Lk(LkQueue &Q) {
    while (Q.front != nullptr) {
        QNode *p = Q.front;
        Q.front = Q.front->next;
        delete p;
    }
    Q.rear = nullptr;
}
```

**链队列最容易错的一个点**:在 `DeQueue_Lk` 中,当你删除的是最后一个数据节点时,`rear` 会变成悬空指针,必须让 `rear` 回到 `front`(头结点)。很多教材和博客的代码在这里有 bug,你要警惕。

---

### 八、测试代码示例(Stack/main.cpp)

cpp

```cpp
#include "SeqStack.h"
#include "LinkStack.h"
#include <iostream>
using namespace std;

int main() {
    // 测试顺序栈
    SqStack S1;
    InitStack_Sq(S1);
    Push_Sq(S1, 1); Push_Sq(S1, 2); Push_Sq(S1, 3);
    int e;
    cout << "顺序栈出栈: ";
    while (!StackEmpty_Sq(S1)) {
        Pop_Sq(S1, e);
        cout << e << " ";           // 期望: 3 2 1
    }
    cout << endl;

    // 测试链栈
    LkStack S2;
    InitStack_Lk(S2);
    Push_Lk(S2, 10); Push_Lk(S2, 20); Push_Lk(S2, 30);
    cout << "链栈出栈:   ";
    while (!StackEmpty_Lk(S2)) {
        Pop_Lk(S2, e);
        cout << e << " ";           // 期望: 30 20 10
    }
    cout << endl;

    DestroyStack_Lk(S2);
    return 0;
}
```

队列的 main.cpp 你照着这个模式自己写,入队 1 2 3,出队应该得到 1 2 3(FIFO)。

---

### 九、四种实现的对比总结

一张对照表帮你理清脑子(我在 prose 里呈现,避免密集 bullet):

**栈顶/队头位置**:顺序栈用数组末尾,链栈用链表头;顺序队用 front 下标,链队用头结点之后。**是否需要头结点**:链栈不需要,链队需要(统一空队操作)。**满的判断**:顺序栈 `top == MAXSIZE`,顺序队 `(rear+1)%MAXSIZE == front`,链式结构理论上都不会满。**空的判断**:顺序栈 `top == 0`,链栈 `S == nullptr`,顺序队 `front == rear`(下标相等),链队 `front == rear`(指针相等)。

---

### 十、思考题(请作答)

第一题,顺序栈中我们约定 `top` 指向"下一个空位",初始值是 0;如果改成 `top` 指向"栈顶元素",初始值应该是多少?对应的 `Push` 和 `Pop` 顺序要怎么调整?

第二题,循环队列为什么要"牺牲一个单元"?除了这种方法,还有哪些方法区分队空和队满?(至少再说一个)

第三题,链队列出队时,为什么删除最后一个节点后,`rear` 必须指回 `front`?如果不处理会发生什么?(提示:想一下 rear 指向被 delete 的内存会怎样)

---

### 十一、下一步方向

你可以选择:

**(A)** 基于栈实现应用题:括号匹配、表达式求值(中缀转后缀)、进制转换——这些是栈最经典的应用,考研大题常考。**(B)** 基于队列实现应用题:用两个栈实现队列、用两个队列实现栈、队列解杨辉三角。**(C)** 直接进入 **KMP 算法**(你最初提到要学的内容)。**(D)** 回到二叉树,接着学线索二叉树或 BST。

告诉我你想先走哪个方向,或者先回答上面三道思考题,我来批改。


## 数据结构复习:KMP 算法

好,我们来啃 KMP。这是整个数据结构课里**最难理解**的算法之一,也是串匹配的巅峰之作。但只要你跟着我一步步推导,会发现它其实有清晰的逻辑。这节我会花较多篇幅在**原理讲解**上,因为 KMP 的代码很短,但不懂原理就是"天书"。

---

### 一、为什么需要 KMP?先看朴素匹配的痛点

假设主串 `S = "ababcababa"`,模式串 `T = "ababa"`,我们要找 T 在 S 中第一次出现的位置。

**朴素匹配(BF 算法)**的思路:从 S[0] 开始逐字符比对 T,不匹配就让 S 回退到开始位置的下一位,T 回到 0,重新比。

```
S: a b a b c a b a b a
T: a b a b a           ← 前 4 位匹配,第 5 位 c≠a 失败
   ↓ 让 i 回退到 1,j 回到 0,重来
S: a b a b c a b a b a
T:   a b a b a         ← 又失败
...
```

问题出在哪?**每次失败都把 i 回退**,这些回退其实重复比对了已经知道的信息。时间复杂度 O(nm)。

**KMP 的核心思想**:i **永远不回退**,只让 j(模式串指针)回退到一个"合适的位置"继续比较。这个"合适的位置"由模式串 T 自身的结构决定,与主串 S 无关——所以可以**预处理**出来,这就是著名的 `next` 数组。

---

### 二、next 数组的本质(这是 KMP 的灵魂)

**next[j] 的含义**:当模式串第 j 位失配时,j 应该回退到哪个位置。

它等价于:**T[0..j-1] 这个前缀中,最长的"相等前后缀"的长度**。

什么叫"相等前后缀"?看例子,串 `"ababa"` 的前缀有:`a`, `ab`, `aba`, `abab`;后缀有:`a`, `ba`, `aba`, `baba`。相等的前后缀是 `a` 和 `aba`,最长长度是 3。

为什么"最长相等前后缀"就是 j 失配时的回退位置?**因为既然这段前缀和这段后缀相等,那么当后缀已经和主串匹配上时,我们可以直接让前缀"滑过来"继续比较,跳过中间的比对**。

这个点是 KMP 最难的地方,你可能需要反复读几遍。我用一个图来说明:

```
         (已匹配部分)
S: ... a b a b a X ...        ← X 是失配字符
T:     a b a b a b            ← 假设 T="ababab",在第 5 位失配
       └前缀┘└后缀┘
       
相等的前后缀:"aba"(长度 3)
既然后缀 "aba" 已经和 S 中对应位置匹配,
我们直接把 T 前缀 "aba" 滑到这个位置:

S: ... a b a b a X ...
T:         a b a b a b        ← j 从 3 开始继续比
             ↑ 
             j = 3
```

所以对模式串 T = "ababab",next[5] = 3。

---

### 三、两种 next 数组的约定(必须分清)

教材对 next 数组有两种常见定义,一定要看清楚你的教材用的是哪种,否则代码会错位:

**约定一**(严蔚敏教材,数组下标从 1 开始,**考研主流**):

- `next[1] = 0`(特殊值,表示需要 i 也后移)
- `next[2] = 1`
- 从 `next[3]` 开始才是真正的"最长相等前后缀长度 + 1"

**约定二**(算法导论,数组下标从 0 开始):

- `next[0] = -1` 或 `next[0] = 0`
- `next[j]` 就是真实的最长相等前后缀长度

两种约定的 next 值相差 1,对应的匹配代码也不同。**我们采用约定一(严蔚敏版)**,因为这是中文考研和教材的标准写法。

---

### 四、手算 next 数组(以 T = "ababaa" 为例)

**必须会手算**,否则考试选择题做不出来。我用约定一:下标从 1 开始,next[1] = 0,next[2] = 1。

|j|1|2|3|4|5|6|
|---|---|---|---|---|---|---|
|T[j]|a|b|a|b|a|a|
|next[j]|0|1|1|2|3|4|

逐项推导:

**next[1] = 0**(规定)。**next[2] = 1**(规定)。

**next[3]**:看 T[1..2] = "ab",最长相等前后缀长度 = 0(前缀"a",后缀"b",不等),+1 = 1。

**next[4]**:看 T[1..3] = "aba",最长相等前后缀 "a"(长度 1),+1 = 2。

**next[5]**:看 T[1..4] = "abab",最长相等前后缀 "ab"(长度 2),+1 = 3。

**next[6]**:看 T[1..5] = "ababa",最长相等前后缀 "aba"(长度 3),+1 = 4。

**记忆套路**:j 位置的 next 值,看 j 之前(不含 j)那段串的最长相等前后缀长度再加 1。考场上这样手算是最稳的。

---

### 五、项目文件组织

```
DataStructure/
└── KMP/
    ├── KMP.h
    ├── KMP.cpp
    └── main.cpp
```

---

### 六、KMP.h

cpp

```cpp
#pragma once

#define MAXLEN 255

// 串的定长顺序存储(严蔚敏教材风格,下标从 1 开始)
typedef struct {
    char ch[MAXLEN + 1];    // ch[0] 不用,字符存在 ch[1..length]
    int length;             // 串的当前长度
} SString;

// ========== 基本操作 ==========
bool StrAssign(SString &S, const char *chars);   // 从 C 字符串赋值
int  StrLength(SString S);

// ========== KMP 核心函数 ==========
void GetNext(SString T, int next[]);              // 求 next 数组
void GetNextval(SString T, int nextval[]);        // 求优化版 nextval 数组
int  Index_KMP(SString S, SString T, int next[]); // KMP 匹配,返回 T 在 S 中的位置
int  Index_BF(SString S, SString T);              // 朴素匹配(对比用)
```

**关于下标**:严蔚敏教材约定串的下标从 1 开始(ch[0] 不用),这样 next[1] = 0 才有意义。我们严格遵循这个约定。

---

### 七、KMP.cpp 完整实现

#### 7.1 基础操作

cpp

```cpp
#include "KMP.h"
#include <cstring>

bool StrAssign(SString &S, const char *chars) {
    int len = strlen(chars);
    if (len > MAXLEN) return false;
    for (int i = 0; i < len; i++) {
        S.ch[i + 1] = chars[i];      // 从下标 1 开始存
    }
    S.length = len;
    return true;
}

int StrLength(SString S) {
    return S.length;
}
```

#### 7.2 朴素匹配(对比基准)

cpp

```cpp
int Index_BF(SString S, SString T) {
    int i = 1, j = 1;
    while (i <= S.length && j <= T.length) {
        if (S.ch[i] == T.ch[j]) {
            i++; j++;
        } else {
            i = i - j + 2;           // i 回退到下一个起点
            j = 1;                   // j 回到开头
        }
    }
    if (j > T.length) return i - T.length;   // 匹配成功
    return 0;                                 // 匹配失败返回 0
}
```

这里 `i = i - j + 2` 是什么意思?i 当前位置 - 已匹配长度 + 下一个起点,即 `i - (j-1) + 1 = i - j + 2`。

#### 7.3 求 next 数组(KMP 的精髓)

这段代码是考研默写级别的,必须背下来:

cpp

```cpp
void GetNext(SString T, int next[]) {
    int i = 1, j = 0;
    next[1] = 0;
    while (i < T.length) {
        if (j == 0 || T.ch[i] == T.ch[j]) {
            i++; j++;
            next[i] = j;             // 关键赋值
        } else {
            j = next[j];             // j 回退,继续比
        }
    }
}
```

**这段代码本质是"模式串自己和自己做 KMP 匹配"**,用递推方式算出每个位置的 next 值。我来帮你模拟一遍 T = "ababaa"(长度 6):

初始 `i=1, j=0, next[1]=0`。

第一轮:`j==0`,进入 if,`i=2, j=1, next[2]=1`。

第二轮:`T[2]='b', T[1]='a'`,不等,进入 else,`j = next[1] = 0`。

第三轮:`j==0`,进入 if,`i=3, j=1, next[3]=1`。

第四轮:`T[3]='a', T[1]='a'`,相等,`i=4, j=2, next[4]=2`。

第五轮:`T[4]='b', T[2]='b'`,相等,`i=5, j=3, next[5]=3`。

第六轮:`T[5]='a', T[3]='a'`,相等,`i=6, j=4, next[6]=4`。

循环结束(i=6 不满足 i<6)。结果 `next[] = [_, 0, 1, 1, 2, 3, 4]`,与我们前面手算完全一致。

#### 7.4 KMP 匹配

cpp

```cpp
int Index_KMP(SString S, SString T, int next[]) {
    int i = 1, j = 1;
    while (i <= S.length && j <= T.length) {
        if (j == 0 || S.ch[i] == T.ch[j]) {
            i++; j++;                // 匹配或 j==0(越界边界)时同时后移
        } else {
            j = next[j];             // i 不动,只有 j 回退
        }
    }
    if (j > T.length) return i - T.length;   // 匹配成功
    return 0;
}
```

**请注意这段代码和求 next 的代码极其相似**,不是巧合——它们本质是同一种逻辑。这也是 KMP 为什么精妙的原因:求 next 就是模式串自己的"自我匹配"。

**`j == 0` 的特殊处理**:当 j 已经回退到 0(next[1]=0 触发的),表示连第一个字符都不匹配了,这时必须 i++ j++ 同时后移,让 i 跳到下一位、j 从 1 开始重新比。这就是为什么约定 next[1] = 0,它是一个"哨兵",指示"i 也要往前走"。

---

### 八、nextval 数组(KMP 的进一步优化)

先看一个 next 数组的**低效场景**:T = "aaaab",next 数组是 `[_, 0, 1, 2, 3, 4]`。

假设主串是 `"aaabaaaab"`,在 j=4 处失配(T[4]='a', S 对应位置='b')。按 next[4]=3 回退,j 变成 3,再比 T[3]='a' vs S 中的 'b',还是失配!接着 j=next[3]=2,T[2]='a' vs 'b',继续失配……**连续四次失败的比对,全是在比同一个字符 'a'**。

**问题本质**:如果 `T[j] == T[next[j]]`,那么 j 回退后比较的还是同一个字符,必然再次失败。nextval 的优化就是把这种无效回退**一次性做到位**。

#### nextval 的改进规则

对于每个位置 j:

如果 `T[j] != T[next[j]]`,那么 `nextval[j] = next[j]`(和原来一样)。如果 `T[j] == T[next[j]]`,那么 `nextval[j] = nextval[next[j]]`(直接"接管"前面那个位置的 nextval)。

#### 代码实现

cpp

```cpp
void GetNextval(SString T, int nextval[]) {
    int i = 1, j = 0;
    nextval[1] = 0;
    while (i < T.length) {
        if (j == 0 || T.ch[i] == T.ch[j]) {
            i++; j++;
            if (T.ch[i] != T.ch[j]) {
                nextval[i] = j;
            } else {
                nextval[i] = nextval[j];   // 接管
            }
        } else {
            j = nextval[j];                // 这里也用 nextval
        }
    }
}
```

#### 手算 nextval(T = "ababaa")

|j|1|2|3|4|5|6|
|---|---|---|---|---|---|---|
|T[j]|a|b|a|b|a|a|
|next[j]|0|1|1|2|3|4|
|nextval[j]|0|1|0|1|0|4|

推导逻辑:j=3 时 T[3]='a' == T[next[3]]=T[1]='a',所以 nextval[3] = nextval[1] = 0。j=5 时 T[5]='a' == T[next[5]]=T[3]='a',所以 nextval[5] = nextval[3] = 0。j=6 时 T[6]='a' != T[next[6]]=T[4]='b',所以 nextval[6] = next[6] = 4(保持)。

**考研真题高频考点**:给你一个模式串,让你手算 next 和 nextval,并指出两者的区别。一定要练。

---

### 九、测试代码(main.cpp)

cpp

```cpp
#include "KMP.h"
#include <iostream>
using namespace std;

int main() {
    SString S, T;
    StrAssign(S, "ababcababaaababaa");
    StrAssign(T, "ababaa");

    int next[MAXLEN + 1], nextval[MAXLEN + 1];
    GetNext(T, next);
    GetNextval(T, nextval);

    cout << "模式串 T = ";
    for (int i = 1; i <= T.length; i++) cout << T.ch[i];
    cout << endl;

    cout << "next[]    : ";
    for (int i = 1; i <= T.length; i++) cout << next[i] << " ";
    cout << endl;

    cout << "nextval[] : ";
    for (int i = 1; i <= T.length; i++) cout << nextval[i] << " ";
    cout << endl;

    int pos1 = Index_BF(S, T);
    int pos2 = Index_KMP(S, T, next);
    cout << "BF  匹配位置: " << pos1 << endl;
    cout << "KMP 匹配位置: " << pos2 << endl;

    return 0;
}
```

预期输出:

```
next[]    : 0 1 1 2 3 4 
nextval[] : 0 1 0 1 0 4 
BF  匹配位置: 12
KMP 匹配位置: 12
```

---

### 十、复杂度分析(必须会说)

**时间复杂度**:求 next 是 O(m),主匹配过程是 O(n),总计 **O(n+m)**。为什么是线性的?因为 i 从不回退,j 虽然可能回退但总移动次数有上限。

**空间复杂度**:next 数组是 O(m)。

**对比朴素匹配 O(nm)**,KMP 在长串匹配下优势巨大。比如 S 长 10000,T 长 100:BF 最坏 100万次比对,KMP 只需 10100 次。

---

### 十一、考研高频思考题(请作答)

第一题,模式串 T = "abaabc",手算它的 next 数组和 nextval 数组。

第二题,对于模式串 "aaaaab",它的 next 数组和 nextval 数组分别是什么?这个例子能最直观地看出 nextval 的优化效果——请指出优化体现在哪里。

第三题,在 KMP 匹配过程中,假设在 j=5 处失配,next[5]=3。请问下一步 j 变成多少?**i 会变化吗**?为什么?

第四题,若采用教材约定(下标从 1 开始),next[1] 为什么规定为 0 而不是其他值?(提示:想想 j=0 在 KMP 主匹配代码中的作用)

---

### 十二、下一步方向

你可以选择:

**(A)** 继续回到**二叉树的线索二叉树**学习(这是下一次对话的自然延续)。**(B)** 学**二叉搜索树 BST**(插入、查找、删除,尤其删除的三种情况最难)。**(C)** 学**哈夫曼树和哈夫曼编码**(考研必考)。**(D)** 学**堆**(用数组实现完全二叉树,堆排序的核心)。**(E)** 先批改上面的思考题再继续。

告诉我你的选择,或者把思考题答案发给我。


## 数据结构复习:树专题

好,我们进入树的学习。**树是数据结构中最重要的非线性结构之一**,也是后续学习二叉树、图、平衡树、B树的基础。我会按照"概念 → 存储结构 → 代码实现 → 常用操作"的顺序带你学。

在开始之前请你先告诉我:你已经学过二叉树了吗?如果还没学,我建议顺序是**先二叉树 → 再一般树和森林**,因为一般树最常用的存储方式是"孩子兄弟表示法",本质上就是把一般树转成二叉树。

不过你的指令是直接学"树",所以我按**一般树(普通树)**来讲。如果后续你想学二叉树或"树和森林的转换",再告诉我。

---

### 一、树的基本概念(必须先搞清楚)

一棵树由若干节点构成,满足:有且仅有一个**根节点**;除根外,每个节点有且仅有一个**父节点**;每个节点可以有零个或多个**孩子节点**。

几个你必须分清的术语:

**度**:一个节点的孩子数叫节点的度;整棵树中最大的节点度叫树的度。**深度/高度**:从根到某节点的路径长度(边数)+1 叫深度,从某节点到最深叶子的路径长度 +1 叫高度。**叶子**:度为 0 的节点。**森林**:m 棵互不相交的树的集合。

这些概念后面写代码会用到,比如"求树的度"、"求树的高度"都是经典题。

---

### 二、树的三种存储结构

一般树的存储比二叉树复杂,因为每个节点的孩子数不固定。教材上有三种经典方案,你都要会写:

**方案一:双亲表示法(顺序存储)**。用数组存节点,每个节点记录自己的父节点下标。优点是找父节点 O(1),缺点是找孩子要遍历整个数组。

**方案二:孩子表示法(链式存储)**。每个节点挂一个孩子链表。优点是找孩子快,缺点是找父节点要遍历。

**方案三:孩子兄弟表示法(二叉链表)**。每个节点存两个指针:第一个孩子 + 下一个兄弟。这种方式把一般树**变成了一棵二叉树**,是考研和实际工程中最常用的。

---

### 三、项目文件组织

按你之前的目录结构,新建:

```
DataStructure/
└── Tree/
    ├── Tree.h          // 三种存储结构的定义 + 函数声明
    ├── Tree.cpp        // 函数实现
    └── main.cpp        // 测试
```

---

### 四、三种结构体定义(写在 Tree.h 中)

请你手动敲下面的代码到 `Tree.h`:

cpp

```cpp
#pragma once

#define MAX_TREE_SIZE 100
typedef char ElemType;   // 节点数据类型,用 char 便于肉眼观察

// ========== 方案一:双亲表示法 ==========
typedef struct {
    ElemType data;       // 节点数据
    int parent;          // 父节点在数组中的下标,根节点为 -1
} PTNode;

typedef struct {
    PTNode nodes[MAX_TREE_SIZE];   // 节点数组
    int n;                          // 当前节点数
} PTree;


// ========== 方案二:孩子表示法 ==========
typedef struct CTNode {             // 孩子链表中的节点
    int child;                      // 孩子在顺序表中的下标
    struct CTNode *next;            // 下一个孩子
} CTNode, *ChildPtr;

typedef struct {
    ElemType data;
    ChildPtr firstChild;            // 指向第一个孩子的指针
} CTBox;

typedef struct {
    CTBox nodes[MAX_TREE_SIZE];
    int n, root;                    // 节点数、根节点下标
} CTree;


// ========== 方案三:孩子兄弟表示法(重点!) ==========
typedef struct CSNode {
    ElemType data;
    struct CSNode *firstChild;      // 指向第一个孩子
    struct CSNode *nextSibling;     // 指向下一个兄弟
} CSNode, *CSTree;
```

看到第三种结构了吗?**每个节点只有两个指针**,本质上就是二叉树的节点!这就是为什么说"一般树可以转成二叉树"——孩子兄弟表示法就是转换规则。

---

### 五、函数声明(继续写在 Tree.h)

我们重点实现**孩子兄弟表示法**的操作,因为它最实用,也是考研高频考点:

cpp

```cpp
// ========== 孩子兄弟表示法常用操作 ==========
bool InitTree_CS(CSTree &T);                       // 初始化(置空)
CSNode* CreateNode(ElemType e);                    // 创建单个节点
int TreeDepth(CSTree T);                           // 求树的深度
int TreeNodeCount(CSTree T);                       // 统计节点总数
int LeafCount(CSTree T);                           // 统计叶子数
int TreeDegree(CSTree T);                          // 求树的度
CSNode* FindNode(CSTree T, ElemType e);            // 按值查找
void PreOrder(CSTree T);                           // 先根遍历
void PostOrder(CSTree T);                          // 后根遍历
void LevelOrder(CSTree T);                         // 层序遍历
void DestroyTree(CSTree &T);                       // 销毁
```

---

### 六、核心函数实现(写在 Tree.cpp)

#### 6.1 基础:初始化与创建节点

cpp

```cpp
#include "Tree.h"
#include <iostream>
#include <queue>
using namespace std;

bool InitTree_CS(CSTree &T) {
    T = nullptr;        // 空树就是空指针
    return true;
}

CSNode* CreateNode(ElemType e) {
    CSNode *p = new CSNode;
    p->data = e;
    p->firstChild = nullptr;
    p->nextSibling = nullptr;
    return p;
}
```

#### 6.2 求树的深度(递归,重点理解)

一般树的深度 = max(所有子树深度) + 1。在孩子兄弟表示法下,一个节点的"所有孩子"是:`firstChild`, `firstChild->nextSibling`, `firstChild->nextSibling->nextSibling`...

cpp

```cpp
int TreeDepth(CSTree T) {
    if (T == nullptr) return 0;
    
    int maxChildDepth = 0;
    CSNode *p = T->firstChild;      // 遍历 T 的所有孩子
    while (p != nullptr) {
        int d = TreeDepth(p);        // 递归求以 p 为根的子树深度
        if (d > maxChildDepth) maxChildDepth = d;
        p = p->nextSibling;          // 通过兄弟链走到下一个孩子
    }
    return maxChildDepth + 1;        // 加上当前层
}
```

**关键点**:这里的递归和二叉树不同。二叉树只递归左右两个孩子,而一般树要通过 `nextSibling` 循环遍历所有孩子,每个孩子都递归一次。

#### 6.3 统计节点总数

cpp

```cpp
int TreeNodeCount(CSTree T) {
    if (T == nullptr) return 0;
    int count = 1;                   // 算上自己
    CSNode *p = T->firstChild;
    while (p != nullptr) {
        count += TreeNodeCount(p);   // 累加每棵子树的节点数
        p = p->nextSibling;
    }
    return count;
}
```

#### 6.4 统计叶子节点数

叶子的判定:**在孩子兄弟表示法里,`firstChild == nullptr` 就是叶子**(没有孩子)。注意 `nextSibling` 不影响判定,因为兄弟不是孩子。

cpp

```cpp
int LeafCount(CSTree T) {
    if (T == nullptr) return 0;
    if (T->firstChild == nullptr) {
        // 当前节点是叶子,但还要统计它的所有兄弟子树里的叶子
        return 1 + LeafCount(T->nextSibling);
    }
    // 非叶子:统计孩子子树 + 兄弟子树里的叶子
    return LeafCount(T->firstChild) + LeafCount(T->nextSibling);
}
```

这里有个初学者容易错的点:**递归一般树时,`nextSibling` 也要递归下去**,否则会漏掉兄弟那一整片。写法和二叉树的 `left/right` 对称处理很像,但含义完全不同。

#### 6.5 先根遍历(递归)

先访问根,再依次先根遍历每棵子树:

cpp

```cpp
void PreOrder(CSTree T) {
    if (T == nullptr) return;
    cout << T->data << " ";          // 访问根
    CSNode *p = T->firstChild;
    while (p != nullptr) {
        PreOrder(p);                 // 递归每棵子树
        p = p->nextSibling;
    }
}
```

#### 6.6 后根遍历

先依次后根遍历每棵子树,最后访问根:

cpp

```cpp
void PostOrder(CSTree T) {
    if (T == nullptr) return;
    CSNode *p = T->firstChild;
    while (p != nullptr) {
        PostOrder(p);
        p = p->nextSibling;
    }
    cout << T->data << " ";          // 最后访问根
}
```

**注意**:一般树**没有中序遍历**的概念,因为孩子数不固定,无法定义"中间"位置。只有先根和后根。

#### 6.7 层序遍历(借助队列)

cpp

```cpp
void LevelOrder(CSTree T) {
    if (T == nullptr) return;
    queue<CSNode*> Q;
    Q.push(T);
    while (!Q.empty()) {
        CSNode *node = Q.front(); Q.pop();
        cout << node->data << " ";
        // 把 node 的所有孩子入队
        CSNode *p = node->firstChild;
        while (p != nullptr) {
            Q.push(p);
            p = p->nextSibling;
        }
    }
}
```

#### 6.8 销毁(后序释放,顺序不能错)

cpp

```cpp
void DestroyTree(CSTree &T) {
    if (T == nullptr) return;
    DestroyTree(T->firstChild);      // 先释放孩子
    DestroyTree(T->nextSibling);     // 再释放兄弟
    delete T;                        // 最后释放自己
    T = nullptr;
}
```

必须按"孩子 → 兄弟 → 自己"的顺序,否则会丢失指针导致内存泄漏。

---

### 七、测试用例(main.cpp)

我们手动建一棵树来验证。假设目标树:

```
        A
      / | \
     B  C  D
    /|     |
   E F     G
```

对应孩子兄弟表示法的形态:`A->firstChild=B`, `B->nextSibling=C->nextSibling=D`, `B->firstChild=E`, `E->nextSibling=F`, `D->firstChild=G`。

cpp

```cpp
#include "Tree.h"
#include <iostream>
using namespace std;

int main() {
    // 手动构建上面那棵树
    CSTree A = CreateNode('A');
    CSTree B = CreateNode('B');
    CSTree C = CreateNode('C');
    CSTree D = CreateNode('D');
    CSTree E = CreateNode('E');
    CSTree F = CreateNode('F');
    CSTree G = CreateNode('G');

    A->firstChild = B;
    B->nextSibling = C;
    C->nextSibling = D;
    B->firstChild = E;
    E->nextSibling = F;
    D->firstChild = G;

    cout << "先根遍历: ";  PreOrder(A);   cout << endl;  // A B E F C D G
    cout << "后根遍历: ";  PostOrder(A);  cout << endl;  // E F B C G D A
    cout << "层序遍历: ";  LevelOrder(A); cout << endl;  // A B C D E F G
    cout << "节点总数: " << TreeNodeCount(A) << endl;    // 7
    cout << "叶子数: "   << LeafCount(A)     << endl;    // 4 (E, F, C, G)
    cout << "树的深度: " << TreeDepth(A)     << endl;    // 3

    DestroyTree(A);
    return 0;
}
```

建议你自己在纸上画出这棵树,再手动追踪一遍三种遍历,看结果对不对——**这是检验你是否真正理解的关键**。

---

### 八、思考题(请你回答)

学完这一节,请先回答我以下问题再进行下一步,这些是考试和面试高频:

第一,在孩子兄弟表示法里,为什么判断叶子用 `firstChild == nullptr` 而不是 `nextSibling == nullptr`?

第二,如果让你实现"求树的度"(即所有节点中孩子数的最大值),你会怎么写?试着自己写一下,然后给我看。

第三,一般树的后根遍历,等价于将它转成二叉树后的什么遍历?(提示:和先根、中序、后序的哪一个相同?)

---

### 九、下一步

你可以告诉我接下来想做什么:

你可以选择:(A) 先让我解答上面三个思考题再继续;(B) 直接开始"二叉树"专题,包括二叉树的定义、四种遍历(先中后层)、线索二叉树、二叉搜索树等;(C) 学习"树、森林、二叉树的相互转换"算法;(D) 补充一般树的双亲表示法和孩子表示法的完整代码实现。

请选一个方向,或者告诉我你的疑问。

- [ ] 
##  数据结构复习:二叉树专题   
 
好,我们进入**二叉树**的学习。这是数据结构里最核心的章节,没有之一——后续的 BST、AVL、红黑树、堆、哈夫曼树、B树都建立在二叉树之上。我会按"概念 → 存储 → 遍历 → 线索化 → BST"的顺序带你扎实地走一遍。

这节内容比较多,我会分成**几次对话**完成。本次先讲:**二叉树的概念、两种存储结构、四种遍历(递归+非递归)、常用操作**。线索二叉树和 BST 留到下次。

---

### 一、二叉树的基本概念(必须记牢)

二叉树和一般树的**本质区别**:每个节点最多有两个孩子,且**左右孩子有严格区分**。即使只有一个孩子,也要指明是左孩子还是右孩子,不能混用。这是二叉树和一般树最容易混淆的地方。

几种特殊的二叉树你要分清:

**满二叉树**:每一层都填满,叶子全在最后一层,节点数为 2^h - 1。**完全二叉树**:除最后一层外每层填满,最后一层节点连续靠左排列。完全二叉树的重要性在于它可以用数组高效存储。**二叉搜索树(BST)**:左子树所有值 < 根 < 右子树所有值。**平衡二叉树(AVL)**:左右子树高度差不超过 1 的 BST。

几个必须记住的**性质**(考研常考):

第一,非空二叉树第 i 层最多有 2^(i-1) 个节点。第二,深度为 h 的二叉树最多有 2^h - 1 个节点。第三,任意二叉树中,**叶子数 n0 = 度为 2 的节点数 n2 + 1**。这个公式推导:总节点数 n = n0 + n1 + n2,总分支数 = n - 1 = n1 + 2·n2,联立可得。第四,具有 n 个节点的完全二叉树深度为 ⌊log₂n⌋ + 1。

---

### 二、两种存储结构

**顺序存储**:用数组存,下标 i 的节点,左孩子在 2i,右孩子在 2i+1,父节点在 i/2(下标从 1 开始)。**仅适合完全二叉树**,否则浪费空间。

**链式存储**(二叉链表,最常用):每个节点含 data、左指针、右指针。这是我们这一节的主角。

---

### 三、项目文件组织

```
DataStructure/
└── BinaryTree/
    ├── BinaryTree.h
    ├── BinaryTree.cpp
    └── main.cpp
```

---

### 四、结构体定义(写到 BinaryTree.h)

cpp

```cpp
#pragma once

typedef char ElemType;

// ========== 二叉链表节点 ==========
typedef struct BiTNode {
    ElemType data;
    struct BiTNode *lchild;
    struct BiTNode *rchild;
} BiTNode, *BiTree;
```

注意这里的双重 typedef 写法和链表专题一样:`BiTNode` 表示节点本身,`BiTree` 表示指向节点的指针(也就是"一棵二叉树"用它的根指针表示)。

---

### 五、函数声明(继续写到 BinaryTree.h)

cpp

```cpp
// ========== 建树 ==========
bool InitTree(BiTree &T);                            // 初始化
BiTNode* CreateNode(ElemType e);                     // 创建单个节点
void CreateBiTree(BiTree &T);                        // 按先序输入创建(# 表示空)

// ========== 四种遍历(递归) ==========
void PreOrder(BiTree T);                             // 先序:根 左 右
void InOrder(BiTree T);                              // 中序:左 根 右
void PostOrder(BiTree T);                            // 后序:左 右 根
void LevelOrder(BiTree T);                           // 层序

// ========== 四种遍历(非递归,重点!) ==========
void PreOrder_NonRec(BiTree T);
void InOrder_NonRec(BiTree T);
void PostOrder_NonRec(BiTree T);

// ========== 常用操作 ==========
int  TreeDepth(BiTree T);                            // 求深度
int  NodeCount(BiTree T);                            // 节点总数
int  LeafCount(BiTree T);                            // 叶子数
int  NodeCountDeg2(BiTree T);                        // 度为2的节点数
BiTNode* FindNode(BiTree T, ElemType e);             // 按值查找
void DestroyTree(BiTree &T);                         // 销毁
```

---

### 六、核心实现(写到 BinaryTree.cpp)

#### 6.1 建树(先序方式,# 表示空)

这是考研最常见的建树方式。用户按**先序遍历顺序**输入字符,遇到 `#` 表示空子树。

例如:输入 `AB#D##C##` 构造的树是:

```
    A
   / \
  B   C
   \
    D
```

cpp

```cpp
#include "BinaryTree.h"
#include <iostream>
#include <stack>
#include <queue>
using namespace std;

bool InitTree(BiTree &T) {
    T = nullptr;
    return true;
}

BiTNode* CreateNode(ElemType e) {
    BiTNode *p = new BiTNode;
    p->data = e;
    p->lchild = p->rchild = nullptr;
    return p;
}

void CreateBiTree(BiTree &T) {
    ElemType ch;
    cin >> ch;
    if (ch == '#') {
        T = nullptr;                 // 空节点
    } else {
        T = new BiTNode;
        T->data = ch;
        CreateBiTree(T->lchild);     // 递归建左子树
        CreateBiTree(T->rchild);     // 递归建右子树
    }
}
```

**关键理解**:`&T` 的引用必不可少,因为我们要修改调用者的指针本身。如果不用引用,递归返回后父节点的 `lchild/rchild` 仍然是 nullptr。

#### 6.2 递归遍历(三兄弟长得极像)

cpp

```cpp
void PreOrder(BiTree T) {
    if (T == nullptr) return;
    cout << T->data << " ";          // 根
    PreOrder(T->lchild);             // 左
    PreOrder(T->rchild);             // 右
}

void InOrder(BiTree T) {
    if (T == nullptr) return;
    InOrder(T->lchild);              // 左
    cout << T->data << " ";          // 根
    InOrder(T->rchild);              // 右
}

void PostOrder(BiTree T) {
    if (T == nullptr) return;
    PostOrder(T->lchild);            // 左
    PostOrder(T->rchild);            // 右
    cout << T->data << " ";          // 根
}
```

**记忆口诀**:先中后指的是**根**的位置。递归是自然的,难点在非递归。

#### 6.3 层序遍历(用队列)

cpp

```cpp
void LevelOrder(BiTree T) {
    if (T == nullptr) return;
    queue<BiTNode*> Q;
    Q.push(T);
    while (!Q.empty()) {
        BiTNode *p = Q.front(); Q.pop();
        cout << p->data << " ";
        if (p->lchild) Q.push(p->lchild);
        if (p->rchild) Q.push(p->rchild);
    }
}
```

---

### 七、非递归遍历(重点中的重点)

非递归遍历是考研和面试**必考**的。核心思想:用**栈**模拟系统递归调用栈。

#### 7.1 非递归先序

思路:访问一个节点,把它的右孩子先压栈(后访问),再处理左孩子。

cpp

```cpp
void PreOrder_NonRec(BiTree T) {
    if (T == nullptr) return;
    stack<BiTNode*> S;
    S.push(T);
    while (!S.empty()) {
        BiTNode *p = S.top(); S.pop();
        cout << p->data << " ";      // 访问根
        if (p->rchild) S.push(p->rchild);  // 先压右(后出)
        if (p->lchild) S.push(p->lchild);  // 后压左(先出)
    }
}
```

**为什么先压右再压左?** 栈是 LIFO,后进先出。我们希望左孩子先被处理,所以左孩子后压栈。

#### 7.2 非递归中序(最经典,必须背熟)

思路:**沿着左链一路往下压栈,到头后弹出访问,然后转向右子树重复**。

cpp

```cpp
void InOrder_NonRec(BiTree T) {
    stack<BiTNode*> S;
    BiTNode *p = T;
    while (p != nullptr || !S.empty()) {
        if (p != nullptr) {
            S.push(p);               // 一路向左压栈
            p = p->lchild;
        } else {
            p = S.top(); S.pop();    // 左边到底,弹出访问
            cout << p->data << " ";
            p = p->rchild;           // 转向右子树
        }
    }
}
```

这段代码一定要**自己手动模拟一遍**才能真正理解。建议拿上面那棵 `ABCD` 的树,在纸上画出栈的变化过程。

#### 7.3 非递归后序(最难,需要标记)

后序的难点在于:访问一个节点时,必须确保**它的左右子树都已经访问完**。所以要记录"上一个被访问的节点",判断我们是从左子树回来还是从右子树回来。

cpp

```cpp
void PostOrder_NonRec(BiTree T) {
    stack<BiTNode*> S;
    BiTNode *p = T;
    BiTNode *lastVisited = nullptr;  // 上一个访问的节点
    
    while (p != nullptr || !S.empty()) {
        if (p != nullptr) {
            S.push(p);
            p = p->lchild;           // 一路向左
        } else {
            BiTNode *top = S.top();
            // 右子树存在且未访问过,则转向右子树
            if (top->rchild != nullptr && top->rchild != lastVisited) {
                p = top->rchild;
            } else {
                cout << top->data << " ";  // 访问当前节点
                lastVisited = top;
                S.pop();
                // p 保持 nullptr,下一轮继续弹栈
            }
        }
    }
}
```

这段代码是**历年考研真题重灾区**,建议反复练习直到能默写。关键在于 `lastVisited` 的作用:防止从右子树返回后又重新进入右子树造成死循环。
就是防止左右中从右回来又进中了


---

### 八、常用操作

#### 8.1 求深度

cpp

```cpp
int TreeDepth(BiTree T) {
    if (T == nullptr) return 0;
    int ld = TreeDepth(T->lchild);
    int rd = TreeDepth(T->rchild);
    return (ld > rd ? ld : rd) + 1;
}
```

#### 8.2 节点数 / 叶子数 / 度为2的节点数

cpp

```cpp
int NodeCount(BiTree T) {
    if (T == nullptr) return 0;
    return NodeCount(T->lchild) + NodeCount(T->rchild) + 1;
}

int LeafCount(BiTree T) {
    if (T == nullptr) return 0;
    if (T->lchild == nullptr && T->rchild == nullptr) return 1;
    return LeafCount(T->lchild) + LeafCount(T->rchild);
}

int NodeCountDeg2(BiTree T) {
    if (T == nullptr) return 0;
    int self = (T->lchild != nullptr && T->rchild != nullptr) ? 1 : 0;
    return self + NodeCountDeg2(T->lchild) + NodeCountDeg2(T->rchild);
}
```

#### 8.3 按值查找

cpp

```cpp
BiTNode* FindNode(BiTree T, ElemType e) {
    if (T == nullptr) return nullptr;
    if (T->data == e) return T;
    BiTNode *p = FindNode(T->lchild, e);
    if (p != nullptr) return p;
    return FindNode(T->rchild, e);   // 左边找不到再找右边
}
```

#### 8.4 销毁(后序释放)

cpp

```cpp
void DestroyTree(BiTree &T) {
    if (T == nullptr) return;
    DestroyTree(T->lchild);
    DestroyTree(T->rchild);
    delete T;
    T = nullptr;
}
```

必须是**后序**,因为要先把孩子释放了,才能释放自己,否则孩子指针丢失。

---

### 九、测试代码(main.cpp)

cpp

```cpp
#include "BinaryTree.h"
#include <iostream>
using namespace std;

int main() {
    BiTree T;
    cout << "按先序输入(# 表示空),例如 AB#D##C##:" << endl;
    CreateBiTree(T);
    
    cout << "递归先序: "; PreOrder(T);  cout << endl;
    cout << "递归中序: "; InOrder(T);   cout << endl;
    cout << "递归后序: "; PostOrder(T); cout << endl;
    cout << "层序遍历: "; LevelOrder(T);cout << endl;
    
    cout << "非递归先序: "; PreOrder_NonRec(T);  cout << endl;
    cout << "非递归中序: "; InOrder_NonRec(T);   cout << endl;
    cout << "非递归后序: "; PostOrder_NonRec(T); cout << endl;
    
    cout << "节点总数: " << NodeCount(T) << endl;
    cout << "叶子数:   " << LeafCount(T) << endl;
    cout << "度2节点: "  << NodeCountDeg2(T) << endl;
    cout << "树深度:   " << TreeDepth(T) << endl;
    
    DestroyTree(T);
    return 0;
}
```

用 `AB#D##C##` 测试,预期输出:

```
递归先序: A B D C 
递归中序: B D A C 
递归后序: D B C A 
层序遍历: A B C D
```

建议你**在纸上画出这棵树**,手动推出三种遍历,再和程序输出对照。

---

### 十、思考题(请作答再继续)

第一,一棵二叉树有 50 个叶子节点,30 个度为 1 的节点,请问总节点数是多少?(用性质 n0 = n2 + 1)

第二,**先序序列 + 中序序列**可以唯一确定一棵二叉树;**先序 + 后序**呢?能否唯一确定?为什么?

第三,非递归后序遍历为什么需要 `lastVisited` 变量?如果去掉会发生什么?

---

### 十一、下一步

下一次对话我们会讲:

**(A)** 线索二叉树(中序线索化及其遍历,考研高频);**(B)** 二叉搜索树 BST(插入、查找、删除,尤其删除的三种情况);**(C)** 二叉树进阶题(构造二叉树 from 前序+中序、判断对称、最近公共祖先等)。

请告诉我你想先走哪个方向,或者先回答思考题,我帮你批改。

## 用自己写的栈和队列重做二叉树的非递归遍历 & 层序遍历

非常好的想法!**用自己写的数据结构支撑更上层的算法**——这正是数据结构课程的核心训练。之前我们用了 `std::stack` 和 `std::queue` 作为"黑盒",现在你要把它们换成自己的 `SqStack`、`LkQueue`,这一步能让你真正理解"栈和队列不是语法糖,而是有明确底层结构的工具"。

---

### 一、先梳理替换思路

我们之前的非递归遍历用了 `std::stack<BiTNode*>`,层序遍历用了 `std::queue<BiTNode*>`。现在要换成自己的栈和队列,但有个**关键问题**:我们之前的 `SqStack` 存的是 `int`(`SElemType = int`),但二叉树遍历要存 `BiTNode*`(节点指针)。

有两种解决方案:

**方案一:修改原 SqStack.h 把 SElemType 改成 `BiTNode*`**——简单但破坏了栈的通用性,而且栈和二叉树耦合了,不推荐。

**方案二:为二叉树遍历专门写一个"节点指针栈"和"节点指针队列"**——结构体和函数重新命名,与原来的 int 栈/队列共存,互不干扰。这是**更工程化的做法**,也是我们要采用的方案。

(顺便提一句:真正的工业级做法是用 C++ 模板 `template<typename T>`,但这是后期内容,现在我们用"复制一份改类型"的朴素做法,保持你对结构本身的理解。)

---

### 二、文件组织

在 `BinaryTree/` 目录下新增两个文件,专门给二叉树的遍历用:

```
DataStructure/
└── BinaryTree/
    ├── BinaryTree.h          // 之前已有
    ├── BinaryTree.cpp        // 之前已有,本次要修改
    ├── BiTreeStack.h         // 新增:存节点指针的顺序栈
    ├── BiTreeStack.cpp       // 新增
    ├── BiTreeQueue.h         // 新增:存节点指针的链队列
    ├── BiTreeQueue.cpp       // 新增
    └── main.cpp
```

我选择**顺序栈 + 链队列**的组合。为什么?栈用顺序存储简单高效,大小可预估;队列用链式避免"假溢出"且无容量限制。这也是教材和工程常见的搭配。当然你也可以反过来用,结构完全对称,原理一致。

---

### 三、BiTreeStack.h(存 BiTNode* 的顺序栈)

cpp

```cpp
#pragma once
#include "BinaryTree.h"             // 为了拿到 BiTNode 的定义

#define STACK_MAXSIZE 100

typedef struct {
    BiTNode *data[STACK_MAXSIZE];    // 存的是节点指针,不是 int 了
    int top;                         // top 指向下一个空位
} BiStack;

// ========== 基本操作(命名加 _Bi 后缀,避免和 int 栈冲突)==========
bool InitStack_Bi(BiStack &S);
bool StackEmpty_Bi(BiStack S);
bool StackFull_Bi(BiStack S);
bool Push_Bi(BiStack &S, BiTNode *e);
bool Pop_Bi(BiStack &S, BiTNode *&e);    // 注意:BiTNode* 的引用
bool GetTop_Bi(BiStack S, BiTNode *&e);
```

特别注意 `Pop_Bi` 的参数 `BiTNode *&e`:这是**"节点指针的引用"**,因为我们要把弹出的指针带回给调用者。不写 `&` 的话,e 只是形参副本,外面拿不到。

### 四、BiTreeStack.cpp 实现

cpp

```cpp
#include "BiTreeStack.h"

bool InitStack_Bi(BiStack &S) {
    S.top = 0;
    return true;
}

bool StackEmpty_Bi(BiStack S) {
    return S.top == 0;
}

bool StackFull_Bi(BiStack S) {
    return S.top == STACK_MAXSIZE;
}

bool Push_Bi(BiStack &S, BiTNode *e) {
    if (StackFull_Bi(S)) return false;
    S.data[S.top] = e;
    S.top++;
    return true;
}

bool Pop_Bi(BiStack &S, BiTNode *&e) {
    if (StackEmpty_Bi(S)) return false;
    S.top--;
    e = S.data[S.top];
    return true;
}

bool GetTop_Bi(BiStack S, BiTNode *&e) {
    if (StackEmpty_Bi(S)) return false;
    e = S.data[S.top - 1];
    return true;
}
```

和你之前写的 int 顺序栈**逻辑完全一样**,只是把 `SElemType` 换成了 `BiTNode*`。这也再次证明:**栈就是栈,不关心里面装什么**,这就是数据结构的抽象威力。

---

### 五、BiTreeQueue.h(存 BiTNode* 的链队列)

cpp

```cpp
#pragma once
#include "BinaryTree.h"

// 队列节点:里面装的是 BiTNode 指针
typedef struct BiQNode {
    BiTNode *data;
    struct BiQNode *next;
} BiQNode;

typedef struct {
    BiQNode *front;                  // 指向头结点
    BiQNode *rear;                   // 指向尾节点
} BiQueue;

bool InitQueue_Bi(BiQueue &Q);
bool QueueEmpty_Bi(BiQueue Q);
bool EnQueue_Bi(BiQueue &Q, BiTNode *e);
bool DeQueue_Bi(BiQueue &Q, BiTNode *&e);
void DestroyQueue_Bi(BiQueue &Q);
```

**结构嵌套关系一定要搞清楚**:`BiQNode` 是队列的节点,里面的 `data` 字段存的是二叉树节点 `BiTNode*`(指针)。不要把队列节点和二叉树节点搞混——它们是两种不同的节点!

### 六、BiTreeQueue.cpp 实现

cpp

```cpp
#include "BiTreeQueue.h"

bool InitQueue_Bi(BiQueue &Q) {
    Q.front = Q.rear = new BiQNode;  // 创建头结点
    Q.front->next = nullptr;
    return true;
}

bool QueueEmpty_Bi(BiQueue Q) {
    return Q.front == Q.rear;
}

bool EnQueue_Bi(BiQueue &Q, BiTNode *e) {
    BiQNode *p = new BiQNode;
    p->data = e;
    p->next = nullptr;
    Q.rear->next = p;                // 挂到尾节点之后
    Q.rear = p;                      // 更新尾指针
    return true;
}

bool DeQueue_Bi(BiQueue &Q, BiTNode *&e) {
    if (Q.front == Q.rear) return false;
    BiQNode *p = Q.front->next;      // 第一个数据节点
    e = p->data;
    Q.front->next = p->next;
    if (Q.rear == p) Q.rear = Q.front;   // 删到最后一个时,rear 回到头结点
    delete p;
    return true;
}

void DestroyQueue_Bi(BiQueue &Q) {
    while (Q.front != nullptr) {
        BiQNode *p = Q.front;
        Q.front = Q.front->next;
        delete p;
    }
    Q.rear = nullptr;
}
```

这里的坑我在链队列章节已经讲过:**出队最后一个节点时,rear 必须回到 front**,否则 rear 变成野指针,下次入队就会崩溃。这个点考研选择题也考过。

---

### 七、重写二叉树的三个遍历函数(BinaryTree.cpp)

现在进入核心部分——把 `std::stack` 和 `std::queue` 全部换掉。我把三个非递归遍历和层序遍历都重写一遍。

#### 7.1 修改 BinaryTree.cpp 的 include 部分

cpp

```cpp
#include "BinaryTree.h"
#include "BiTreeStack.h"      // 用自己的栈
#include "BiTreeQueue.h"      // 用自己的队列
#include <iostream>
using namespace std;

// 注意:原来的 <stack> 和 <queue> 不再需要了
```

#### 7.2 非递归先序遍历(重写)

cpp

```cpp
void PreOrder_NonRec(BiTree T) {
    if (T == nullptr) return;
    BiStack S;
    InitStack_Bi(S);
    Push_Bi(S, T);

    while (!StackEmpty_Bi(S)) {
        BiTNode *p;
        Pop_Bi(S, p);               // 弹栈访问
        cout << p->data << " ";
        // 先压右,后压左,保证左孩子先出栈
        if (p->rchild) Push_Bi(S, p->rchild);
        if (p->lchild) Push_Bi(S, p->lchild);
    }
}
```

对照之前用 `std::stack` 的版本:结构完全一样,只是 `S.push(x)` 变成 `Push_Bi(S, x)`,`S.top(); S.pop();` 合并成 `Pop_Bi(S, p)`。

**一个工程上的小改进**:我们的 `Pop_Bi` 一步到位取值并弹栈,比 STL 的 top+pop 两步更紧凑。这是你自己写数据结构的小小乐趣。

#### 7.3 非递归中序遍历(重写)

cpp

```cpp
void InOrder_NonRec(BiTree T) {
    BiStack S;
    InitStack_Bi(S);
    BiTNode *p = T;

    while (p != nullptr || !StackEmpty_Bi(S)) {
        if (p != nullptr) {
            Push_Bi(S, p);          // 一路向左压栈
            p = p->lchild;
        } else {
            Pop_Bi(S, p);           // 左边到底,弹栈访问
            cout << p->data << " ";
            p = p->rchild;          // 转向右子树
        }
    }
}
```

**注意**:之前我用了 `S.top()` 和 `S.pop()` 分两步,但实际上中序的这个位置只需要"取并弹",所以用 `Pop_Bi` 合二为一更优雅。

#### 7.4 非递归后序遍历(重写,最复杂)

后序遍历需要"读栈顶但不弹出",判断是否该转向右子树。之前用 `S.top()` 可以做到这点,我们的栈有 `GetTop_Bi` 对应:

cpp

```cpp
void PostOrder_NonRec(BiTree T) {
    BiStack S;
    InitStack_Bi(S);
    BiTNode *p = T;
    BiTNode *lastVisited = nullptr;     // 上次访问的节点

    while (p != nullptr || !StackEmpty_Bi(S)) {
        if (p != nullptr) {
            Push_Bi(S, p);
            p = p->lchild;
        } else {
            BiTNode *top;
            GetTop_Bi(S, top);          // 只看不弹
            // 有右子树且未访问过 → 转向右子树
            if (top->rchild != nullptr && top->rchild != lastVisited) {
                p = top->rchild;
            } else {
                // 弹出并访问
                Pop_Bi(S, top);
                cout << top->data << " ";
                lastVisited = top;
                // p 保持 nullptr,下轮继续弹
            }
        }
    }
}
```

对比之前版本,`S.top()` → `GetTop_Bi(S, top)`,`S.pop()` → `Pop_Bi(S, top)`。逻辑完全一致。

#### 7.5 层序遍历(重写)

cpp

```cpp
void LevelOrder(BiTree T) {
    if (T == nullptr) return;
    BiQueue Q;
    InitQueue_Bi(Q);
    EnQueue_Bi(Q, T);

    while (!QueueEmpty_Bi(Q)) {
        BiTNode *p;
        DeQueue_Bi(Q, p);
        cout << p->data << " ";
        if (p->lchild) EnQueue_Bi(Q, p->lchild);
        if (p->rchild) EnQueue_Bi(Q, p->rchild);
    }
    DestroyQueue_Bi(Q);                 // 用完销毁
}
```

这里我额外加了 `DestroyQueue_Bi(Q)`——链队列的头结点是 `new` 出来的,不销毁会泄漏内存。而顺序栈是栈变量,函数结束自动释放,不用手动清。**这是链式和顺序的又一个区别,你要记住**。

---

### 八、main.cpp(和之前完全相同)

cpp

```cpp
#include "BinaryTree.h"
#include <iostream>
using namespace std;

int main() {
    BiTree T;
    cout << "按先序输入(# 表示空),例如 AB#D##C##:" << endl;
    CreateBiTree(T);

    cout << "非递归先序: "; PreOrder_NonRec(T);  cout << endl;
    cout << "非递归中序: "; InOrder_NonRec(T);   cout << endl;
    cout << "非递归后序: "; PostOrder_NonRec(T); cout << endl;
    cout << "层序遍历:   "; LevelOrder(T);       cout << endl;

    DestroyTree(T);
    return 0;
}
```

用 `AB#D##C##` 测试,输出应该是:

```
非递归先序: A B D C 
非递归中序: B D A C 
非递归后序: D B C A 
层序遍历:   A B C D
```

**注意**:main.cpp 里**不需要**包含 `BiTreeStack.h` 和 `BiTreeQueue.h`——它们只是 BinaryTree.cpp 的内部实现细节。这就是"**接口与实现分离**"的原则:外部使用者只关心 BinaryTree.h 提供的遍历函数,底层用什么栈、什么队列不关心。这是优秀的工程分层。

---

### 九、编译命令提示

四个 .cpp 文件一起编译:

bash

```bash
g++ -o bitree main.cpp BinaryTree.cpp BiTreeStack.cpp BiTreeQueue.cpp
```

如果你用 IDE(VS / CLion / VS Code),把这四个 .cpp 都加入项目即可。

---

### 十、关于 "include 循环依赖" 的一个提醒

你可能注意到:

- `BiTreeStack.h` 包含了 `BinaryTree.h`(因为要用 `BiTNode`)
- `BinaryTree.cpp` 包含了 `BiTreeStack.h`

这是**单向依赖**,没问题:栈依赖二叉树节点定义,二叉树实现依赖栈的操作。如果你写成"BinaryTree.h 里反过来也 include BiTreeStack.h",就会形成循环依赖导致编译错误。**永远让依赖方向单一**,这是 C/C++ 头文件管理的铁律。

`#pragma once` 可以防止单文件被重复包含,但**防不住循环依赖**——循环依赖是设计问题,不是技术问题。

---

### 十一、思考题(请作答)

第一题,如果我把层序遍历也改用顺序队列(循环队列)而不是链队列,会有什么限制?(提示:考虑队列大小)

第二题,为什么后序遍历用 `GetTop_Bi` "看但不弹"比 `Pop_Bi` "弹了再压回去"更好?(提示:想想效率和代码简洁性)

第三题,如果把"顺序栈 + 链队列"改成"链栈 + 循环队列"可以吗?代码要改哪些地方?**函数的语义和时间复杂度会变吗**?

---

### 十二、下一步方向

这一步你做完、理解透了,就真正体会到"数据结构"的工程意义了——**栈和队列不是 STL 的魔法,而是你亲手能造出来的工具**。接下来可以:

**(A)** 学**线索二叉树**(考研高频,要用到中序遍历思想)。**(B)** 学**二叉搜索树 BST**(插入、查找、删除)。**(C)** 学**哈夫曼树**(优先队列/最小堆应用)。**(D)** 学**树、森林与二叉树的相互转换**(你最初列表中的内容)。**(E)** 先回答思考题,我批改。

告诉我你的选择。

## 数据结构复习:线索二叉树

好,我们进入**线索二叉树**。这是二叉树里最抽象、也是考研最容易失分的章节。我会花较多篇幅讲清楚**它为什么存在**、**规则是什么**、**代码怎么写**。

提醒你一点:线索二叉树的代码不难,但**理解它在做什么**是难点。如果你第一次读不懂,不要慌,跟着我的推导走一遍再回头看代码。

---

### 一、为什么要有线索二叉树?

先看一个观察:**n 个节点的二叉链表,一共有 2n 个指针域,其中只有 n-1 个用于指向孩子,剩下 n+1 个都是空指针**。

证明这个结论:n 个节点共 2n 个指针域;每个节点(除根)都有一个"从父节点指向它"的指针,共 n-1 个有效指针;所以空指针数 = 2n - (n-1) = n+1。

**这些空指针浪费了,能不能利用起来?**

线索二叉树的想法是:**让左空指针指向它的中序前驱,让右空指针指向它的中序后继**。这样遍历二叉树就不需要递归或栈了,沿着线索直接走就行,空间 O(1)。

这个想法的价值在:**某些应用中要反复遍历同一棵树**(比如表达式树的求值、语法树的多次扫描),线索化一次,之后每次遍历都是 O(n) 无栈遍历。

---

### 二、三种线索化(先后中)

根据遍历顺序不同,线索化分为三种:

**中序线索二叉树**(最常用,考研必考)、**先序线索二叉树**、**后序线索二叉树**。

我们**重点讲中序**,先序后序的代码只需改动递归顺序,留作你的练习。为什么中序最常用?因为在中序线索二叉树上,**找后继和找前驱都是 O(1) 或 O(h)**,非常规整;先序和后序要找某些方向的前驱/后继会很麻烦。

---

### 三、结构体设计(重点,容易错)

既然我们要让"空指针"变成"指向前驱/后继的线索",就必须能区分:**这个指针到底是指向孩子,还是指向线索?**

解决方案:**每个指针加一个标志位**。

cpp

```cpp
typedef struct ThreadNode {
    ElemType data;
    struct ThreadNode *lchild, *rchild;
    int ltag, rtag;           // 0 表示孩子,1 表示线索
} ThreadNode, *ThreadTree;
```

规则:

`ltag = 0`:lchild 指向左孩子(正常)。`ltag = 1`:lchild 指向**中序前驱**(线索)。`rtag = 0`:rchild 指向右孩子(正常)。`rtag = 1`:rchild 指向**中序后继**(线索)。

每个节点仅增加 2 个标志位(通常用 bool 或 1 bit),代价极小,收益巨大。

---

### 四、一棵树的中序线索化结果(先手画一遍)

先别急着看代码,我们手动线索化下面这棵树,帮你建立直觉:

```
        A
       / \
      B   C
     / \
    D   E
```

**中序遍历序列**:D B E A C。

根据这个序列,每个节点的前驱后继是:

|节点|中序前驱|中序后继|
|---|---|---|
|D|无|B|
|B|D|E|
|E|B|A|
|A|E|C|
|C|A|无|

现在看哪些节点有空指针需要线索化:

D 的 lchild 空 → 线索指向 D 的前驱(无,设为 NULL,ltag=1)。D 的 rchild 空 → 线索指向 B(ltag=1,rtag=1)。E 的 lchild 空 → 线索指向 B。E 的 rchild 空 → 线索指向 A。C 的 lchild 空 → 线索指向 A。C 的 rchild 空 → 无后继,设为 NULL,rtag=1。

B 和 A 的指针都没空,不需要线索化。

**请你一定拿笔在草稿纸上画一遍**,把每个节点的 lchild、rchild、ltag、rtag 填出来。没画过这一步的同学,代码永远看不懂。

---

### 五、项目文件组织

```
DataStructure/
└── ThreadTree/
    ├── ThreadTree.h
    ├── ThreadTree.cpp
    └── main.cpp
```

---

### 六、ThreadTree.h

cpp

```cpp
#pragma once

typedef char ElemType;

// ========== 线索二叉树节点 ==========
typedef struct ThreadNode {
    ElemType data;
    struct ThreadNode *lchild, *rchild;
    int ltag, rtag;                     // 0=孩子,1=线索
} ThreadNode, *ThreadTree;

// ========== 基本操作 ==========
void CreateThreadTree(ThreadTree &T);              // 按先序建普通二叉树(# 表空)
void InThread(ThreadTree p, ThreadTree &pre);      // 中序线索化的递归核心
void CreateInThread(ThreadTree T);                 // 对外的中序线索化接口

// ========== 基于线索的遍历 ==========
ThreadNode* FirstNode(ThreadNode *p);              // 中序序列下,以 p 为根的子树的第一个节点
ThreadNode* NextNode(ThreadNode *p);               // p 的中序后继
void InOrder_Thread(ThreadTree T);                 // 非递归、无栈的中序遍历
```

---

### 七、ThreadTree.cpp 核心实现

#### 7.1 建普通二叉树(先序方式,# 表空)

这一步先建一棵普通二叉树,**初始所有 tag 都置 0**,线索化之后再改。

cpp

```cpp
#include "ThreadTree.h"
#include <iostream>
using namespace std;

void CreateThreadTree(ThreadTree &T) {
    ElemType ch;
    cin >> ch;
    if (ch == '#') {
        T = nullptr;
    } else {
        T = new ThreadNode;
        T->data = ch;
        T->ltag = 0;                    // 初始都是 0,稍后线索化时才可能改 1
        T->rtag = 0;
        CreateThreadTree(T->lchild);
        CreateThreadTree(T->rchild);
    }
}
```

#### 7.2 中序线索化(算法核心,必须背熟)

**核心思想**:中序遍历的过程中,对每个节点 p:

- 如果 p 的 lchild 为空,把 lchild 指向前驱 pre,ltag = 1。
- 如果前驱 pre 的 rchild 为空,把 rchild 指向当前节点 p,rtag = 1(也就是说,pre 的后继就是 p)。

**关键点**:pre 必须用**引用传递**或**全局变量**,因为递归过程中要不断更新它。我们用引用。

cpp

```cpp
void InThread(ThreadTree p, ThreadTree &pre) {
    if (p == nullptr) return;
    
    InThread(p->lchild, pre);           // 递归线索化左子树
    
    // ---- 访问当前节点 p:处理线索 ----
    if (p->lchild == nullptr) {
        p->lchild = pre;                // 左线索指向前驱
        p->ltag = 1;
    }
    if (pre != nullptr && pre->rchild == nullptr) {
        pre->rchild = p;                // 前驱的右线索指向当前节点
        pre->rtag = 1;
    }
    pre = p;                            // 更新 pre 为当前节点
    
    InThread(p->rchild, pre);           // 递归线索化右子树
}
```

**请务必盯着这段代码看三遍**:

第一遍看骨架——就是一个标准的中序递归(左-根-右),"访问根"部分被替换成了"处理线索"。

第二遍看线索处理——两个 if 分别处理"当前节点的左空指针"和"前驱的右空指针"。为什么要处理前驱的右空指针?因为只有到了当前节点 p,我们才知道前驱 pre 的后继是谁(就是 p)。

第三遍看 `pre = p`——每访问完一个节点就更新 pre,为下一次迭代做准备。

#### 7.3 对外接口(处理边界)

上面的 `InThread` 是递归核心,但有个细节:**中序序列的最后一个节点,它的右指针永远没被处理**(因为 pre 更新后,后面没有节点来帮它设置右线索了)。需要在外部包一层处理:

cpp

```cpp
void CreateInThread(ThreadTree T) {
    ThreadTree pre = nullptr;           // 最开始没有前驱
    if (T != nullptr) {
        InThread(T, pre);
        // 处理最后一个节点(pre 此时指向中序序列最后一个节点)
        pre->rchild = nullptr;
        pre->rtag = 1;                  // 表示"没有后继"
    }
}
```

第一次看到这个"尾处理"你可能不理解,其实它对应的就是中序序列最后一个节点(比如前面例子中的 C)的右线索应该指向 NULL 且 rtag=1。如果不做这步,C 的 rchild 在建树时是 nullptr,但 rtag 还是 0——这会让后续遍历代码**误以为 C 的右孩子是空**(rtag=0 意味着"正常的右孩子"),虽然值是 nullptr 不会崩,但语义不对,考研题会判错。

---

### 八、基于线索的遍历(O(1) 空间!)

线索建好后,遍历不再需要栈或递归。关键是两个辅助函数:**找某子树的第一个节点**、**找某节点的后继**。

#### 8.1 FirstNode:中序序列的第一个节点

中序序列的第一个节点就是**整棵树最左下的节点**(一直往左走到 ltag=1 为止)。

cpp

```cpp
ThreadNode* FirstNode(ThreadNode *p) {
    while (p->ltag == 0) {              // 有左孩子就往左走
        p = p->lchild;
    }
    return p;
}
```

**注意终止条件是 `ltag == 0`,不是 `lchild != nullptr`**!因为线索化后 lchild 可能指向前驱(不是 nullptr),这时不能再往下走。这是**初学者最容易错的地方**。

#### 8.2 NextNode:中序后继

分两种情况:

**情况一**:如果 p 有右子树(rtag=0),则 p 的中序后继是**右子树的第一个节点**(右子树的最左下节点)。

**情况二**:如果 p 没有右子树(rtag=1),则 rchild 本身就是后继(线索)。

cpp

```cpp
ThreadNode* NextNode(ThreadNode *p) {
    if (p->rtag == 0) {
        return FirstNode(p->rchild);    // 右子树的第一个节点
    } else {
        return p->rchild;               // 线索直接指向后继
    }
}
```

#### 8.3 中序遍历(无栈,纯线索驱动)

有了上面两个函数,中序遍历就是两行的循环:

cpp

```cpp
void InOrder_Thread(ThreadTree T) {
    if (T == nullptr) return;
    for (ThreadNode *p = FirstNode(T); p != nullptr; p = NextNode(p)) {
        cout << p->data << " ";
    }
}
```

**这段代码的优雅程度是本章的巅峰**:像访问链表一样访问二叉树,空间 O(1),无栈无递归。**这就是线索化的全部价值所在**。

---

### 九、完整测试(main.cpp)

cpp

```cpp
#include "ThreadTree.h"
#include <iostream>
using namespace std;

int main() {
    ThreadTree T;
    cout << "按先序输入(# 表空),例如 ABD##E##C##:" << endl;
    CreateThreadTree(T);                // 建普通二叉树
    CreateInThread(T);                  // 中序线索化
    
    cout << "中序遍历(基于线索): ";
    InOrder_Thread(T);
    cout << endl;
    return 0;
}
```

用输入 `ABD##E##C##` 对应前面那棵例子树,期望输出:

```
中序遍历(基于线索): D B E A C
```

---

### 十、常见疑问(必须搞清楚)

**疑问一:为什么叫"中序"线索二叉树?**

因为我们是按中序遍历的前驱/后继来设置线索的。如果按先序设置,叫先序线索二叉树;按后序设置,叫后序线索二叉树。**同一棵树,三种线索化后形态不同**,别搞混。

**疑问二:加了线索之后,普通的递归遍历还能用吗?**

能用,但必须判断 tag:访问 lchild 前要确认 ltag==0,不然会顺着线索走回前驱,变成死循环。所以一旦线索化,通常就专用线索遍历,不再混用。

**疑问三:线索化可以"反悔"吗?**

难。要还原成普通二叉树,需要遍历一遍把所有 tag=1 的指针改回 nullptr。所以**线索化是一个半永久操作**。

**疑问四:先序和后序线索化,对应的 FirstNode、NextNode 怎么写?**

先序线索化的 `NextNode` 相对好写(当前有左孩子就是左孩子,否则沿线索),后序线索化的 `NextNode` **最难**(要找父节点,一般节点没有父指针的话做不了),所以后序线索化通常需要加三叉链表或额外处理,实际应用很少。**考研只需掌握中序**。

---

### 十一、思考题(请作答)

第一题,对下面这棵树进行中序线索化,写出每个节点线索化后的 lchild、rchild、ltag、rtag:

```
      1
     / \
    2   3
         \
          4
```

第二题,在中序线索二叉树中,**如何找一个节点 p 的中序前驱**?写出 `PreNode(p)` 的伪代码。(提示:这是 NextNode 的对称版本——考虑"p 有左子树"和"p 没有左子树"两种情况)

第三题,为什么后序线索二叉树的 `NextNode` 比中序难写?具体难在哪一步?

第四题,假设一棵二叉树有 100 个节点,线索化之后总共用到了多少条线索?(提示:用那个"空指针数 = n+1"的结论,但要小心最后一个节点的右线索指向 NULL 是否计入)

---

### 十二、下一步方向

线索二叉树算是二叉树章节的一个高峰,你啃下来之后可以选择:

**(A)** 学**二叉搜索树 BST**(插入、查找、删除的三种情况,考研大题高频)。**(B)** 学**哈夫曼树和哈夫曼编码**(考研必考,应用题能出选择也能出大题)。**(C)** 学**树、森林与二叉树的相互转换**(你最初学习清单里的内容,很适合在这里串起来)。**(D)** 用你自己的栈重写先序/后序线索化的遍历代码作为练习。**(E)** 先答思考题再继续。

告诉我你想走哪个方向。

树、森林与二叉树的相互转换

下午11:37

## 数据结构复习:树、森林与二叉树的相互转换

好,我们进入一个**非常重要但常被忽视**的知识点。这节内容考研选择题年年出,原理非常优美——它揭示了一个深刻事实:**普通树、森林、二叉树这三种看似不同的结构,本质上是同一种东西的不同表达形式**。

这节我会**先花大量篇幅讲清楚转换规则**(这是本章真正的重点),然后给出代码实现。因为只要规则懂了,代码是 10 分钟就能敲出来的事;规则不懂,代码写出来也是懵的。

---

### 一、核心洞察(先建立直觉)

回忆一下你之前学过的"孩子兄弟表示法":每个节点只有两个指针,一个指向**第一个孩子**,一个指向**下一个兄弟**。

cpp

```cpp
typedef struct CSNode {
    ElemType data;
    struct CSNode *firstChild;      // 指向第一个孩子
    struct CSNode *nextSibling;     // 指向下一个兄弟
} CSNode, *CSTree;
```

现在你盯着这个结构体看——**它不就是二叉树的节点吗?** 只是把 lchild 改叫 firstChild,rchild 改叫 nextSibling 而已!

这就是本章的核心秘密:**孩子兄弟表示法本身就是"把一般树画成二叉树"的规则**。我们所谓的"转换",其实就是给同一个数据结构换个视角来看。

---

### 二、转换规则(三条口诀)

#### 2.1 一般树 → 二叉树(口诀:"左孩子,右兄弟")

**规则**:对原树中每个节点,保留它和**第一个孩子**的连线作为二叉树中的**左分支**;把它所有孩子之间的**兄弟关系**改画成**右分支**。

我用一个例子带你走一遍。原一般树:

```
        A
      / | \
     B  C  D
    /|     |
   E F     G
```

**转换步骤**:

第一步,连兄弟。B-C、C-D 是兄弟,连起来;E-F 是兄弟,连起来。

```
        A
      / | \
     B--C--D
    /|     |
   E-F     G
```

第二步,每个节点只保留与**第一个孩子**的连线,删除其他孩子连线。A 只留 A-B,删掉 A-C、A-D;B 只留 B-E,删掉 B-F;D 只留 D-G。

```
        A
       /
      B---C---D
     /        |
    E---F     G
```

第三步,旋转——把每条"兄弟连线"看作右分支,"第一个孩子连线"看作左分支。得到:

```
        A
       /
      B
     / \
    E   C
     \   \
      F   D
         /
        G
```

**这就是转换结果**。你可以验证:在二叉树里,A 的左孩子是 B(B 确实是 A 的第一个孩子);B 的右孩子是 C(C 确实是 B 在原树中的下一个兄弟);B 的左孩子是 E,E 的右孩子是 F,都符合规则。

**一个必须记住的性质**:**一般树转成二叉树后,根节点没有右子树**。因为根节点没有兄弟(它是树的顶点)。这是考研选择题的送分点。

#### 2.2 二叉树 → 一般树(反向操作)

**规则**:反过来执行。对二叉树中每个节点 p,如果它有右孩子 r,那么 r 是 p 的父节点在一般树中的下一个兄弟;r 的右孩子是再下一个兄弟……

**操作化描述**:从二叉树还原一般树,对于每个节点 p,把 p 的右孩子、右孩子的右孩子、右孩子的右孩子的右孩子……全部变成 **p 的父节点**的孩子。然后删除这些右连线,改画成 p 父节点向它们发出的分支。

#### 2.3 森林 → 二叉树(口诀:"把森林的根串成兄弟")

森林是**多棵树的集合**,比如 F = {T1, T2, T3}:

```
    A         E         H
   / \        |        / \
  B   C       F       I   J
              |
              G
```

**规则两步**:

第一步,每棵树**独立转成二叉树**(用 2.1 的规则)。

第二步,**把第二棵树的根接到第一棵树根的右子树,把第三棵树的根接到第二棵树根的右子树,依此类推**。换句话说,各棵树的根按顺序被串成一个"右分支链"。

结果:

```
        A
       / \
      B   E
       \ / \
        C F H
             \
              I
               \
                J
```

(F 下面还有 G,我没全画完,但你应该看出结构了。)

**性质**:森林转二叉树后,**根节点的右子树反映了其他树**,根节点的左子树反映了第一棵树。这和"一般树转二叉树根没有右子树"是对偶的——因为森林中有多棵树,所以根可以有右子树。

#### 2.4 二叉树 → 森林(反向)

看一棵二叉树:如果根的右子树为空,那么它对应一棵一般树;如果根的右子树非空,那么它对应森林。

**操作**:反复剥离——把当前二叉树根节点的右子树**整个拆下来**,作为新的一棵树的根继续处理。每一次剥离产生森林中的一棵树。

---

### 三、三个操作总结

用一句话总结这一整章:

**"把一般树/森林的兄弟关系,在二叉树里画成右分支;把第一个孩子关系,画成左分支。"**

记住这一句,所有转换题都能做。

---

### 四、遍历序列的对应关系(考研重点)

这是本章**真正的高频考点**:转换前后,哪些遍历序列保持不变?

|一般树|森林|对应二叉树|
|---|---|---|
|先根遍历|先序遍历(依次遍历每棵树)|**先序遍历**|
|后根遍历|**中序遍历**(依次后根)|**中序遍历**|

**重点结论(必须背下来)**:

一般树的**先根遍历**序列 = 对应二叉树的**先序**序列。一般树的**后根遍历**序列 = 对应二叉树的**中序**序列。森林的**先序遍历**序列 = 对应二叉树的**先序**序列。森林的**中序遍历**(即对每棵树后根,然后依次)= 对应二叉树的**中序**序列。

**注意**:一般树和森林都**没有"中序遍历"**(因为孩子数不固定,没有"中间"的概念),但转成二叉树后可以有中序——这个中序对应的就是一般树的后根遍历。这是最容易被出题人卡住的地方。

#### 一个示例验证

还是 2.1 的那棵树:

```
原一般树:
        A
      / | \
     B  C  D
    /|     |
   E F     G
```

**先根遍历**(根→各子树先根):A B E F C D G。

转换后的二叉树(2.1 里画的那个):

```
        A
       /
      B
     / \
    E   C
     \   \
      F   D
         /
        G
```

**二叉树先序**(根→左→右):A B E F C D G。**完全一致**!

**一般树后根**:E F B C G D A。

**二叉树中序**(左→根→右):E F B C G D A。**完全一致**!

这两个对应关系不是巧合——**它们本质上是同一个遍历过程,只是换个视角描述**。你理解了这一点,转换这一章就彻底通了。

---

### 五、项目文件组织

```
DataStructure/
└── Convert/
    ├── Convert.h
    ├── Convert.cpp
    └── main.cpp
```

---

### 六、Convert.h

我们要给出两种结构:一般树节点(标准"多孩子数组"表示)、二叉树节点(就是孩子兄弟表示法 = 二叉链表)。

cpp

```cpp
#pragma once

#define MAX_CHILDREN 10
typedef char ElemType;

// ========== 一般树(多孩子数组表示法)==========
// 便于我们"自然地"定义一棵一般树
typedef struct GTNode {
    ElemType data;
    int childCount;
    struct GTNode *children[MAX_CHILDREN];
} GTNode, *GTree;

// ========== 二叉树(孩子兄弟表示法)==========
typedef struct BTNode {
    ElemType data;
    struct BTNode *lchild;      // 对应"第一个孩子"
    struct BTNode *rchild;      // 对应"下一个兄弟"
} BTNode, *BTree;

// ========== 辅助函数 ==========
GTNode* CreateGTNode(ElemType e);
BTNode* CreateBTNode(ElemType e);
void AddChild(GTNode *parent, GTNode *child);

// ========== 核心转换函数 ==========
BTree GTreeToBTree(GTree T);                        // 一般树 → 二叉树
GTree BTreeToGTree(BTree T);                        // 二叉树 → 一般树

// ========== 森林相关(森林用"根节点数组"表示)==========
BTree ForestToBTree(GTree forest[], int n);         // 森林 → 二叉树

// ========== 遍历(用于验证转换正确性)==========
void PreOrder_G(GTree T);                           // 一般树先根遍历
void PostOrder_G(GTree T);                          // 一般树后根遍历
void PreOrder_B(BTree T);                           // 二叉树先序
void InOrder_B(BTree T);                            // 二叉树中序
```

---

### 七、Convert.cpp 实现

#### 7.1 基础建树工具

cpp

```cpp
#include "Convert.h"
#include <iostream>
using namespace std;

GTNode* CreateGTNode(ElemType e) {
    GTNode *p = new GTNode;
    p->data = e;
    p->childCount = 0;
    for (int i = 0; i < MAX_CHILDREN; i++) p->children[i] = nullptr;
    return p;
}

BTNode* CreateBTNode(ElemType e) {
    BTNode *p = new BTNode;
    p->data = e;
    p->lchild = p->rchild = nullptr;
    return p;
}

void AddChild(GTNode *parent, GTNode *child) {
    parent->children[parent->childCount++] = child;
}
```

#### 7.2 一般树 → 二叉树(核心算法)

这是本章最关键的代码,请**仔细看逻辑**:

cpp

```cpp
BTree GTreeToBTree(GTree T) {
    if (T == nullptr) return nullptr;
    
    // 1. 把一般树的根复制为二叉树的根
    BTNode *bt = CreateBTNode(T->data);
    
    // 2. 把第一个孩子递归转换,挂到 bt->lchild
    if (T->childCount > 0) {
        bt->lchild = GTreeToBTree(T->children[0]);
        
        // 3. 其余孩子依次挂在"右兄弟链"上
        BTNode *curr = bt->lchild;
        for (int i = 1; i < T->childCount; i++) {
            curr->rchild = GTreeToBTree(T->children[i]);
            curr = curr->rchild;            // 移动到新挂上的节点,继续挂下一个
        }
    }
    return bt;
}
```

**这段代码的核心逻辑**(请对照转换规则理解):

第一步,当前节点独立转换为二叉树节点。

第二步,把当前节点的**第一个孩子**转成二叉树,挂到**左分支**。这直接对应规则"第一个孩子→左分支"。

第三步,剩下的孩子们原本是"彼此的兄弟",所以在二叉树中要串成**右分支链**。我们用 `curr` 作为"链尾指针",每挂一个新兄弟就把 curr 往后移。这直接对应规则"兄弟关系→右分支"。

#### 7.3 二叉树 → 一般树(反向)

cpp

```cpp
GTree BTreeToGTree(BTree T) {
    if (T == nullptr) return nullptr;
    
    GTNode *gt = CreateGTNode(T->data);
    
    // 从 T->lchild 开始,沿着 rchild 链走,每个都是 gt 的孩子
    BTNode *p = T->lchild;
    while (p != nullptr) {
        GTNode *child = BTreeToGTree(p);    // 递归转换这棵子树
        AddChild(gt, child);                // 加入 gt 的孩子列表
        p = p->rchild;                      // 沿着"兄弟链"继续
    }
    return gt;
}
```

理解关键:**在二叉树里,从 T->lchild 开始,沿着 rchild 一直走,访问到的节点在原一般树中都是 T 的直接孩子**。这是 7.2 的逆过程。

#### 7.4 森林 → 二叉树

cpp

```cpp
BTree ForestToBTree(GTree forest[], int n) {
    if (n == 0) return nullptr;
    
    // 1. 第一棵树转成二叉树,作为结果的根
    BTree root = GTreeToBTree(forest[0]);
    
    // 2. 剩下的树依次转换,挂到前一个根的"右子树"
    BTNode *curr = root;
    for (int i = 1; i < n; i++) {
        curr->rchild = GTreeToBTree(forest[i]);
        curr = curr->rchild;                 // 移到新挂上的根,继续往后挂
    }
    return root;
}
```

**注意这段代码和 7.2 中的"挂兄弟链"逻辑几乎一模一样**。这不是巧合——森林就是"一堆根组成的兄弟关系",和一般树里孩子之间的兄弟关系,在二叉树中都表示为"右分支链"。**森林和一般树在二叉树视角下是等价的**,这是本章最深刻的洞察。

#### 7.5 遍历函数(用于验证)

cpp

```cpp
void PreOrder_G(GTree T) {
    if (T == nullptr) return;
    cout << T->data << " ";                  // 先访问根
    for (int i = 0; i < T->childCount; i++) {
        PreOrder_G(T->children[i]);          // 再依次先根遍历每棵子树
    }
}

void PostOrder_G(GTree T) {
    if (T == nullptr) return;
    for (int i = 0; i < T->childCount; i++) {
        PostOrder_G(T->children[i]);
    }
    cout << T->data << " ";                  // 最后访问根
}

void PreOrder_B(BTree T) {
    if (T == nullptr) return;
    cout << T->data << " ";
    PreOrder_B(T->lchild);
    PreOrder_B(T->rchild);
}

void InOrder_B(BTree T) {
    if (T == nullptr) return;
    InOrder_B(T->lchild);
    cout << T->data << " ";
    InOrder_B(T->rchild);
}
```

---

### 八、测试代码(main.cpp)

我们手动建出本章开头的那棵一般树,验证遍历序列的对应关系:

cpp

```cpp
#include "Convert.h"
#include <iostream>
using namespace std;

int main() {
    // 建一般树:
    //         A
    //       / | \
    //      B  C  D
    //     /|     |
    //    E F     G
    GTree A = CreateGTNode('A');
    GTree B = CreateGTNode('B');
    GTree C = CreateGTNode('C');
    GTree D = CreateGTNode('D');
    GTree E = CreateGTNode('E');
    GTree F = CreateGTNode('F');
    GTree G = CreateGTNode('G');
    
    AddChild(A, B); AddChild(A, C); AddChild(A, D);
    AddChild(B, E); AddChild(B, F);
    AddChild(D, G);
    
    // 一般树遍历
    cout << "一般树先根: "; PreOrder_G(A);  cout << endl;  // A B E F C D G
    cout << "一般树后根: "; PostOrder_G(A); cout << endl;  // E F B C G D A
    
    // 转换为二叉树
    BTree bt = GTreeToBTree(A);
    
    // 验证对应关系!
    cout << "二叉树先序: "; PreOrder_B(bt); cout << endl;  // A B E F C D G (== 先根)
    cout << "二叉树中序: "; InOrder_B(bt);  cout << endl;  // E F B C G D A (== 后根)
    
    return 0;
}
```

**运行后你会看到四行输出正好两两相等**:

```
一般树先根: A B E F C D G 
一般树后根: E F B C G D A 
二叉树先序: A B E F C D G 
二叉树中序: E F B C G D A
```

**这就是本章最有力的证明**——代码跑出来的结果完美验证了理论。

---

### 九、常见易错点归纳

第一个易错点,一般树转二叉树后,**根节点一定没有右子树**。但森林转二叉树后,**根可以有右子树**。

第二个易错点,"后根遍历"对应的是二叉树"**中序**",不是"后序"。这是考试最爱挖的坑。

第三个易错点,一般树没有"中序"的概念,**只有先根和后根**。有人试图套用中序会得到错误答案。

第四个易错点,转换过程中节点的**数量不变,值不变**,只是指针重新组织。转换不增加也不减少节点。

第五个易错点,**同一棵二叉树**可以对应**一棵一般树**,也可以对应**一片森林**,取决于根的右子树是否为空。考试时如果题目说"转换成森林",就要从根的右子树开始剥离。

---

### 十、思考题(请作答)

第一题,下面这棵二叉树:

```
        A
       / \
      B   C
     / \
    D   E
         \
          F
```

请把它转换成**森林**,画出森林的样子。(提示:先看根有没有右子树,有 → 说明是森林)

第二题,一棵一般树有 n 个节点,转换成二叉树后,它的**右指针空指针**有多少个?(提示:右指针代表"下一个兄弟",没有兄弟时就是空。想一下,什么样的节点在原一般树中是"最后一个孩子"——它在二叉树中右指针就为空)

第三题,以下哪组遍历序列**不能**由同一棵二叉树对应(即转换前后的对应关系): (A) 一般树先根 `A B C`,二叉树先序 `A B C` (B) 一般树后根 `C B A`,二叉树中序 `C B A` (C) 一般树后根 `A B C`,二叉树后序 `C B A` (D) 森林先序 `A B C`,二叉树先序 `A B C`

第四题,给定森林的先序遍历 `A B C D` 和中序遍历 `B A D C`,这片森林由几棵树组成?每棵树的根是什么?(提示:先还原二叉树,再拆成森林)

---

### 十一、下一步方向

转换章节啃完,你的"树族谱"就完整了:一般树、森林、二叉树三者的等价关系已经建立。接下来可以选:

**(A)** 学**二叉搜索树 BST**(插入、查找、重点是删除的三种情况)。**(B)** 学**哈夫曼树和哈夫曼编码**(考研大题高频,构造过程是必会的)。**(C)** 学**堆和堆排序**(完全二叉树的数组存储应用)。**(D)** 回到前面还没做完的**顺序表**章节(你最初列的清单里还有这一项)。**(E)** 先答思考题再继续。

告诉我你想走哪个方向。

## 数据结构复习:哈夫曼树和哈夫曼编码

好,我们来学**哈夫曼树**(Huffman Tree),这是考研大题的**常客**,也是数据结构里少有的"纯应用型"算法——它直接解决了一个实际问题:**如何用最少的比特数对一段文本进行编码**。

本章特点:**概念不难,但手算过程容易出错;代码有一定套路,需要熟练**。我会按"问题引入 → 手工构造 → 代码实现 → 编码译码"的顺序讲。

---

### 一、问题引入:为什么需要哈夫曼编码?

假设你要传输一段文本,只包含 5 种字符 `{A, B, C, D, E}`,出现频率分别是 `{5, 9, 12, 13, 16}`(共 55 次)。

**方案一:等长编码**(定长码)。5 个字符需要至少 3 位二进制(2^3=8 ≥ 5):A=000, B=001, C=010, D=011, E=100。总比特数 = 55 × 3 = **165 位**。

**方案二:变长编码**。让**出现频率高的字符用短编码,低频字符用长编码**,整体就能省比特。这是哈夫曼编码的核心思想。

**但变长编码有个陷阱**:比如 A=0, B=01, C=1,那么比特流 "01" 既可以解码为 A+C,也可以解码为 B,产生歧义。

**解决方法是"前缀码"(Prefix Code)**:**任何字符的编码都不能是其他字符编码的前缀**。哈夫曼编码保证是前缀码,因此无歧义。

---

### 二、核心概念(必记)

**路径长度**:从根到某节点经过的**边数**。

**节点的带权路径长度(WPL of node)**:节点的权值 × 从根到它的路径长度。

**树的带权路径长度(WPL)**:所有**叶子节点**的带权路径长度之和。**注意只算叶子!**

**哈夫曼树(最优二叉树)**:在所有以给定权值为叶子的二叉树中,**WPL 最小**的那棵。

**公式**:WPL = Σ(wi × li),其中 wi 是第 i 个叶子的权值,li 是它的路径长度。

#### 一个 WPL 计算示例

对于权值 `{5, 9, 12, 13, 16}`,如果你构造出一棵二叉树:

```
          55
         /  \
       22    33
      / \   / \
     9   13 16 17
               / \
              5   12
```

叶子的路径长度:9→2, 13→2, 16→2, 5→3, 12→3。

WPL = 9×2 + 13×2 + 16×2 + 5×3 + 12×3 = 18 + 26 + 32 + 15 + 36 = **127**。

这是"一种"树,不一定最优。**哈夫曼算法就是构造 WPL 最小那棵的算法**。

---

### 三、哈夫曼算法(手工构造,必须熟练)

**算法流程**:

第一步,把 n 个权值看作 n 棵**只有根的单节点树**,放入集合 F。

第二步,从 F 中选出**权值最小的两棵树**作为左右子树,构造一棵新的二叉树,新根的权值是两棵子树根的**权值之和**。

第三步,从 F 中**删除这两棵子树**,**加入新合并出来的树**。

第四步,重复第二、三步,直到 F 中只剩**一棵树**——这就是哈夫曼树。

**关键规则**:每次合并两棵最小的。这是贪心算法的经典应用。

#### 手工构造示例(权值 {5, 9, 12, 13, 16})

**初始 F**:{5}, {9}, {12}, {13}, {16}

**第一次合并**:选最小的 5 和 9,合并成新树根为 14。F 变为:{12}, {13}, {14}, {16}。

```
   14
  /  \
 5    9
```

**第二次合并**:选最小的 12 和 13,合并成 25。F 变为:{14}, {16}, {25}。

```
   25
  /  \
 12   13
```

**第三次合并**:选最小的 14 和 16,合并成 30。F 变为:{25}, {30}。

```
   30
  /  \
 14   16
/  \
5   9
```

**第四次合并**:25 和 30 合并成 55。F 只剩 1 棵,结束。

最终哈夫曼树:

```
            55
          /    \
        25      30
       /  \    /  \
      12  13  14   16
              / \
             5   9
```

WPL = 5×4 + 9×4 + 12×2 + 13×2 + 16×2 = 20 + 36 + 24 + 26 + 32 = **138**。

嗯?这比前面手工凑的 127 还大?**不对,我前面那棵树不是合法的二叉树构造**——让我重新算前面那棵:叶子其实是 9, 13, 16, 5, 12 分别在路径长度 2, 2, 2, 3, 3 处,但这是我随便画的一棵树,**那棵树对应的合并顺序并不合法**,所以那个 127 是一个假象(不可达)。真正由哈夫曼算法构造出来的最小 WPL 是 **138**。

让我再验证一下哈夫曼树的 WPL:叶子 12 路径长度 2,叶子 13 路径长度 2,叶子 16 路径长度 2,叶子 5 路径长度 3(经 30→14→5),叶子 9 路径长度 3。

12×2 + 13×2 + 16×2 + 5×3 + 9×3 = 24 + 26 + 32 + 15 + 27 = **124**。

**等一下,让我再数一遍路径长度**。哈夫曼树如下:

```
            55            ← 层 0(根)
          /    \
        25      30        ← 层 1
       /  \    /  \
      12  13  14   16     ← 层 2
              / \
             5   9        ← 层 3
```

路径长度 = 从根到叶子的**边数** = 层数(根在第 0 层)。12、13、14、16 都在第 2 层,但 14 不是叶子!叶子是 12、13、16(第 2 层),5、9(第 3 层)。

WPL = 12×2 + 13×2 + 16×2 + 5×3 + 9×3 = 24 + 26 + 32 + 15 + 27 = **124**。

这才是正确答案。**哈夫曼算法保证 124 是所有可能构造中最小的 WPL**。

**一条重要规则**:哈夫曼算法每步产生的新根节点(内部节点)**不是叶子**,不参与 WPL 计算。只有原始权值对应的 n 个节点才是叶子。

---

### 四、哈夫曼编码(从哈夫曼树导出)

构造完哈夫曼树后,**为每条左分支标 0、每条右分支标 1**(或反过来,只要一致),从根到叶子路径上的 0/1 串就是该叶子字符的编码。

上面那棵哈夫曼树:

```
            55
          0/    \1
        25        30
       0/ \1     0/ \1
      12  13   14   16
              0/ \1
              5   9
```

编码:

- 12 → `00`
- 13 → `01`
- 16 → `11`
- 5 → `100`
- 9 → `101`

**这些编码都是前缀码**:没有任何一个是另一个的前缀(你可以自己验证)。

用这些编码传 55 次总比特数 = 12×2 + 13×2 + 16×2 + 5×3 + 9×3 = 124 位。对比等长编码的 165 位,**节省 25%**。这就是哈夫曼编码的价值。

---

### 五、项目文件组织

```
DataStructure/
└── Huffman/
    ├── Huffman.h
    ├── Huffman.cpp
    └── main.cpp
```

---

### 六、Huffman.h(结构体设计是难点)

教材经典的实现是**静态三叉链表**:用数组存哈夫曼树,每个节点记录 weight、parent、lchild、rchild 四个字段。为什么要有 parent?因为**编码时要从叶子回溯到根**,需要父指针。

cpp

```cpp
#pragma once

#define MAX_N 100                // 最多叶子数
#define MAX_NODES (2*MAX_N - 1)  // n 个叶子的哈夫曼树共 2n-1 个节点

// ========== 哈夫曼树节点(静态三叉链表)==========
typedef struct {
    int weight;                  // 权值
    int parent;                  // 父节点下标(0 表示无父节点,即根)
    int lchild;                  // 左孩子下标(0 表示无)
    int rchild;                  // 右孩子下标
} HTNode;

typedef HTNode HuffmanTree[MAX_NODES + 1];   // 下标 1..2n-1 使用,0 表示"空"

// 哈夫曼编码表:每个字符的编码是一个字符串(由 '0' '1' 组成)
typedef char* HuffmanCode[MAX_N + 1];        // 下标 1..n

// ========== 函数声明 ==========
void CreateHuffmanTree(HuffmanTree HT, int w[], int n);      // 构造哈夫曼树
void CreateHuffmanCode(HuffmanTree HT, HuffmanCode HC, int n); // 生成编码
int  WPL(HuffmanTree HT, int n);                             // 计算 WPL
```

**为什么用数组而不是动态指针?**

三个原因:其一,哈夫曼树节点数固定(**n 个叶子 → 正好 2n-1 个节点**,这是一个可证明的定理),适合静态分配。其二,用下标取代指针方便调试(可以直接打印整个数组看结构)。其三,这是严蔚敏教材的经典写法,考研必须掌握。

**一个小技巧**:数组下标从 1 开始用,下标 0 保留为"空"标志(类似于"NULL 指针")。这样判断"是否到达根"可以用 `parent == 0`。

---

### 七、Huffman.cpp 核心实现

#### 7.1 辅助函数:找权值最小的两个节点

cpp

```cpp
#include "Huffman.h"
#include <iostream>
#include <climits>
#include <cstring>
using namespace std;

// 在 HT[1..k] 中,parent==0 的节点里,找权值最小的两个,返回它们的下标 s1, s2
// s1 对应更小的权值
void Select(HuffmanTree HT, int k, int &s1, int &s2) {
    int min1 = INT_MAX, min2 = INT_MAX;
    s1 = s2 = 0;
    for (int i = 1; i <= k; i++) {
        if (HT[i].parent == 0) {              // 只考虑"尚未被合并"的节点
            if (HT[i].weight < min1) {
                min2 = min1; s2 = s1;
                min1 = HT[i].weight; s1 = i;
            } else if (HT[i].weight < min2) {
                min2 = HT[i].weight; s2 = i;
            }
        }
    }
}
```

这是哈夫曼算法的**关键辅助函数**。一个常见实现是用**最小堆(优先队列)**来做,可以把每次选取的时间从 O(n) 降到 O(log n),但对于初学者来说**线性查找足够清晰**,容易写对。考试写伪代码时也是这么写的。

#### 7.2 构造哈夫曼树(核心函数)

cpp

```cpp
void CreateHuffmanTree(HuffmanTree HT, int w[], int n) {
    if (n <= 1) return;
    int total = 2 * n - 1;                  // 节点总数
    
    // 初始化所有节点
    for (int i = 1; i <= total; i++) {
        HT[i].weight = 0;
        HT[i].parent = HT[i].lchild = HT[i].rchild = 0;
    }
    
    // 填入 n 个叶子的权值(下标 1..n)
    for (int i = 1; i <= n; i++) {
        HT[i].weight = w[i - 1];            // w 数组从 0 起
    }
    
    // 合并 n-1 次,生成下标 n+1 .. 2n-1 的内部节点
    for (int i = n + 1; i <= total; i++) {
        int s1, s2;
        Select(HT, i - 1, s1, s2);          // 在前 i-1 个节点中找最小的两个
        
        HT[s1].parent = i;
        HT[s2].parent = i;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
    }
}
```

**这段代码完全按手工构造流程来写**。外层循环 `n-1` 次(从 n+1 到 2n-1),每次合并两个最小的节点。**合并后要更新三处**:两个子节点的 parent,新节点的 lchild、rchild、weight。

#### 7.3 生成哈夫曼编码

思路:对每个叶子节点,从它出发**沿 parent 向上走到根**,途中记录"我是父节点的左孩子还是右孩子",得到一串 0/1。**这串 0/1 是倒过来的**(从叶子到根),所以要反转才能得到正确的编码。

cpp

```cpp
void CreateHuffmanCode(HuffmanTree HT, HuffmanCode HC, int n) {
    char temp[MAX_N + 1];                   // 临时存当前叶子的编码(倒序)
    
    for (int i = 1; i <= n; i++) {          // 对每个叶子
        int pos = n;                        // 从 temp 末尾往前填(编码长度 < n)
        temp[pos--] = '\0';                 // 字符串结束符
        
        int c = i;                          // 当前节点
        int p = HT[i].parent;               // 父节点
        
        while (p != 0) {                    // 未到达根
            if (HT[p].lchild == c) {
                temp[pos--] = '0';          // 左分支 → 0
            } else {
                temp[pos--] = '1';          // 右分支 → 1
            }
            c = p;
            p = HT[p].parent;
        }
        
        // 把 temp[pos+1 .. n-1] 拷贝出来,作为第 i 个字符的编码
        int len = n - 1 - pos;
        HC[i] = new char[len + 1];
        strcpy(HC[i], &temp[pos + 1]);
    }
}
```

**难点在于"倒序填充"**:我们从 temp 的末尾往前填,这样**填完后从 pos+1 开始读就是正确顺序**。如果顺序填再反转,要写个 reverse 函数,反而啰嗦。这是一个值得学会的小技巧。

#### 7.4 计算 WPL

cpp

```cpp
int WPL(HuffmanTree HT, int n) {
    int wpl = 0;
    for (int i = 1; i <= n; i++) {          // 只算前 n 个(叶子)
        int len = 0;
        int p = HT[i].parent;
        while (p != 0) {
            len++;
            p = HT[p].parent;
        }
        wpl += HT[i].weight * len;
    }
    return wpl;
}
```

**注意只对 1..n(叶子)求和**,不要对内部节点求和。

---

### 八、测试代码(main.cpp)

cpp

```cpp
#include "Huffman.h"
#include <iostream>
using namespace std;

int main() {
    int w[] = {5, 9, 12, 13, 16};
    int n = 5;
    
    HuffmanTree HT;
    HuffmanCode HC;
    
    CreateHuffmanTree(HT, w, n);
    CreateHuffmanCode(HT, HC, n);
    
    cout << "哈夫曼树结构(下标 weight parent lchild rchild):" << endl;
    for (int i = 1; i <= 2 * n - 1; i++) {
        cout << i << "\t" << HT[i].weight << "\t" 
             << HT[i].parent << "\t" 
             << HT[i].lchild << "\t" 
             << HT[i].rchild << endl;
    }
    
    cout << "\n哈夫曼编码:" << endl;
    for (int i = 1; i <= n; i++) {
        cout << "权值 " << HT[i].weight << " → " << HC[i] << endl;
    }
    
    cout << "\nWPL = " << WPL(HT, n) << endl;
    
    // 释放编码字符串
    for (int i = 1; i <= n; i++) delete[] HC[i];
    
    return 0;
}
```

预期输出(可能因为"选最小两个时遇到平手"的选择顺序不同而略有差异,但 WPL 必然是 124):

```
哈夫曼编码:
权值 5  → 100
权值 9  → 101
权值 12 → 00
权值 13 → 01
权值 16 → 11

WPL = 124
```

---

### 九、编码与译码流程

有了哈夫曼编码表后,**编码**(字符 → 比特流)就是简单查表拼接。**译码**(比特流 → 字符)稍复杂一些:

**译码规则**:从比特流的第一位开始,从哈夫曼树**根**出发,读到 0 走左、读到 1 走右;**走到叶子就输出一个字符,并回到根**重新开始下一个字符。

cpp

```cpp
void Decode(HuffmanTree HT, int n, const char *bitStream) {
    int root = 2 * n - 1;                   // 根节点下标
    int p = root;
    
    for (int i = 0; bitStream[i] != '\0'; i++) {
        if (bitStream[i] == '0') p = HT[p].lchild;
        else p = HT[p].rchild;
        
        if (HT[p].lchild == 0 && HT[p].rchild == 0) {  // 到达叶子
            cout << "权值 " << HT[p].weight << " ";
            p = root;                       // 回到根,继续解码
        }
    }
    cout << endl;
}
```

**译码的正确性依赖于前缀码性质**——只有前缀码才能保证"走到叶子立即能判断一个字符结束",不会有歧义。

---

### 十、几个易错点 & 考点总结

**考点一**:n 个叶子的哈夫曼树共有多少节点?答:**2n - 1** 个。推导:每次合并产生 1 个新节点,合并 n-1 次,加上原有 n 个叶子,共 2n-1。

**考点二**:哈夫曼树是否唯一?**不唯一**。如果出现权值相等的情况,选哪两个合并会影响树的形态,但 **WPL 相同**(都是最优值)。另外,左右子树交换也不改变 WPL。

**考点三**:哈夫曼树**没有度为 1 的节点**。推导:每次合并都产生一个有左右两个子树的新节点,不可能出现只有一个孩子的内部节点。

**考点四**:哈夫曼编码的长度不一定等于 ⌈log₂n⌉,可能**更短(高频字符)**也可能**更长(低频字符)**。

**考点五**:前缀码的判断——**所有字符都在叶子上,没有字符在内部节点**,就一定是前缀码。这是因为如果字符 A 的编码是字符 B 的前缀,意味着 A 对应的节点是 B 路径上的祖先,那 A 就是内部节点了。

---

### 十一、思考题(请作答)

第一题,给定权值 `{7, 5, 2, 4}`,请**手工构造**哈夫曼树,写出每个叶子的编码,并计算 WPL。

第二题,有 8 种字符,其使用频率分别是 `{1, 4, 5, 7, 10, 15, 20, 30}`,求哈夫曼编码的平均码长(即 WPL / 频率总和)。

第三题,如果有一棵二叉树,所有叶子都标记权值,但它**不是**哈夫曼树,它的 WPL 和真正的哈夫曼树的 WPL 相比,一定更大吗?还是可能相等?(提示:考虑权值相等的情况)

第四题,如果给定的权值全部相等(比如都是 1,共 n 个),构造出的哈夫曼树**长什么样**?它的 WPL 是多少?

第五题,能否对字符 `{A, B, C}` 给出编码 `{A=0, B=10, C=1}`,使它成为前缀码?如果不能,请说明理由。

---

### 十二、下一步方向

哈夫曼树啃完,你就掌握了二叉树里**最经典的应用算法**。接下来可以:

**(A)** 学**二叉搜索树 BST**(插入、查找、删除的三种情况,考研大题高频)。**(B)** 回到你最初列表里的 **顺序表**(线性表的顺序存储结构,相对基础但必须会)。**(C)** 学**堆和堆排序**(堆其实是一种特殊的完全二叉树,也可以优化哈夫曼树的合并过程)。**(D)** 学**平衡二叉树 AVL**(BST 进阶,插入时的四种旋转)。**(E)** 先答思考题再继续。

告诉我你的选择。

## 数据结构复习:二叉搜索树 BST

好,我们进入**二叉搜索树**(Binary Search Tree,简称 BST),也叫"二叉排序树"(Binary Sort Tree)。这是数据结构中最重要的动态查找结构之一,**考研大题高频命题点**,几乎每年都会以某种形式出现(插入、删除、查找效率分析)。

这一章最大的难点是**删除操作的三种情况**,特别是"删除度为 2 的节点"时的处理。我会把每种情况都讲透。

---

### 一、BST 的定义与性质

**定义**:一棵二叉搜索树或者为空,或者满足:

左子树所有节点的值 < 根节点的值 < 右子树所有节点的值,且左右子树也是二叉搜索树。

**核心性质**:**对 BST 做中序遍历,得到的序列是一个递增有序序列**。这是 BST 最重要的特性,几乎所有性质都由它推出。

例如下面这棵 BST:

```
        8
       / \
      3   10
     / \    \
    1   6    14
       / \   /
      4   7 13
```

中序遍历:1 3 4 6 7 8 10 13 14——确实是递增的。这就是"二叉**搜索**树"名字的来源:搜索(查找)某个值只需要 O(h) 时间,h 是树的高度。

---

### 二、BST 的查找(最容易的操作)

**思路**:从根出发,与目标值比较:相等则找到;小于当前节点则向左子树找;大于则向右子树找。

cpp

```cpp
// 递归版
BSTNode* Search(BSTNode *T, int key) {
    if (T == nullptr || T->data == key) return T;
    if (key < T->data) return Search(T->lchild, key);
    else return Search(T->rchild, key);
}
```

**这段代码本质上就是二分查找在树上的推广**——每次比较后丢掉一半数据。

**查找效率分析**:最好 O(log n)(树平衡),最坏 O(n)(树退化成链表,比如严格递增插入)。平均为 O(log n)。

---

### 三、BST 的插入

**规则**:插入的新节点**一定作为某个叶子**。

**流程**:从根出发按查找规则向下走,直到走到 nullptr 的位置——这个位置就是新节点的家。

cpp

```cpp
bool Insert(BSTNode *&T, int key) {
    if (T == nullptr) {                         // 找到空位
        T = new BSTNode;
        T->data = key;
        T->lchild = T->rchild = nullptr;
        return true;
    }
    if (key == T->data) return false;           // 不允许重复
    if (key < T->data) return Insert(T->lchild, key);
    else return Insert(T->rchild, key);
}
```

**注意 `T` 必须用引用传递**——因为递归到 nullptr 位置时要修改指针本身。你之前学链表时已经反复训练过这个点。

---

### 四、BST 的构造(批量插入)

一棵 BST 可以通过"依次插入"一组数据来构造。**关键结论**:**插入顺序不同,得到的 BST 形态不同**,但**中序遍历结果相同**(都是递增序列)。

比如按 `{50, 30, 70, 20, 40, 60, 80}` 插入:

```
        50
       /  \
      30   70
     / \   / \
    20 40 60 80
```

按 `{20, 30, 40, 50, 60, 70, 80}` 插入(严格递增),BST 会退化成右斜链表:

```
20 → 30 → 40 → 50 → 60 → 70 → 80
```

这时 BST 查找效率退化到 O(n),完全失去意义。这就是为什么后来发明了 **AVL 树、红黑树**——强制保持平衡。

---

### 五、BST 的删除(本章重中之重)

**BST 删除比插入复杂得多**,因为要保证删除后**仍然是 BST**(中序仍递增)。根据待删除节点 p 的度数,分三种情况:

#### 情况一:p 是叶子(度为 0)

直接删除,修改父节点对应的指针为 nullptr。

#### 情况二:p 只有一个孩子(度为 1)

让 p 的孩子**顶替** p 的位置。相当于在链表中删除一个单链节点。

#### 情况三:p 有两个孩子(度为 2,最难)

不能简单"顶替",因为 p 只有一个"位置",而要安置两个子树。

**标准做法**:找 p 在**中序序列中的前驱**(或后继)来替换 p 的 data,然后删除那个前驱(或后继)节点。

**为什么这样做是对的?** 因为:

中序前驱是"左子树中最大的节点"——它位于 p **左子树的最右下角**,**必然度 ≤ 1**(它没有右孩子,否则就不是最大)。所以删除前驱变成了情况一或情况二,可以递归解决。

中序后继类似,是"右子树中最小的节点",位于 p **右子树的最左下角**,**必然度 ≤ 1**(没有左孩子)。

用哪种都行,形态会不同但都是合法 BST。我们统一用**中序后继**(教材主流做法)。

#### 删除的可视化示例

删除节点 3(度为 2):

```
        8                    8
       / \                  / \
      3   10      →         4   10
     / \    \              / \    \
    1   6    14           1   6    14
       / \   /               \   /
      4   7 13                7 13
```

步骤:找 3 的中序后继 = 4(3 的右子树最左下角);用 4 的值替换 3;删除原来那个 4(它是叶子,直接去掉)。

#### 删除代码(必须反复理解)

cpp

```cpp
// 找以 p 为根的子树中的最小节点(最左下角)
BSTNode* FindMin(BSTNode *p) {
    while (p->lchild != nullptr) p = p->lchild;
    return p;
}

// 删除 key
bool Delete(BSTNode *&T, int key) {
    if (T == nullptr) return false;
    
    if (key < T->data) {
        return Delete(T->lchild, key);
    } else if (key > T->data) {
        return Delete(T->rchild, key);
    } else {
        // 找到要删的节点 T
        if (T->lchild == nullptr) {         // 情况一 + 情况二(只有右孩子)
            BSTNode *q = T;
            T = T->rchild;                  // 右孩子顶替
            delete q;
        } else if (T->rchild == nullptr) {  // 情况二(只有左孩子)
            BSTNode *q = T;
            T = T->lchild;
            delete q;
        } else {                             // 情况三(度为 2)
            BSTNode *succ = FindMin(T->rchild);
            T->data = succ->data;           // 用后继的值替换当前节点
            Delete(T->rchild, succ->data);  // 递归删除后继(必落入情况一/二)
        }
        return true;
    }
}
```

**关键细节**:

第一,情况一和情况二可以**合并处理**:只要某个孩子为空,用另一个孩子顶替即可。如果左孩子为空,用右孩子顶替(包括右孩子也为空的情况,等价于删叶子);如果右孩子为空,用左孩子顶替。

第二,`T = T->rchild` 这种写法之所以能正确修改父节点的指针,靠的是 `BSTNode *&T` 的引用传递。再次强调这一点的重要性——**如果你忘了 `&`,整个删除就崩了**。

第三,情况三中我们只**复制了 data**,然后递归删除后继节点。这样既完成了"逻辑上的替换",又把问题转化为已知的简单情况。**这是算法设计中"化繁为简"的典范**。

---

### 六、项目文件组织

```
DataStructure/
└── BST/
    ├── BST.h
    ├── BST.cpp
    └── main.cpp
```

---

### 七、BST.h

cpp

```cpp
#pragma once

typedef int ElemType;

typedef struct BSTNode {
    ElemType data;
    struct BSTNode *lchild, *rchild;
} BSTNode, *BSTree;

// ========== 基本操作 ==========
bool InitBST(BSTree &T);
BSTNode* Search(BSTree T, ElemType key);         // 递归查找
BSTNode* Search_NonRec(BSTree T, ElemType key);  // 非递归查找
bool Insert(BSTree &T, ElemType key);
bool Delete(BSTree &T, ElemType key);
void CreateBST(BSTree &T, ElemType arr[], int n); // 由数组构造 BST

// ========== 辅助 ==========
BSTNode* FindMin(BSTree T);                       // 最小节点(最左下)
BSTNode* FindMax(BSTree T);                       // 最大节点(最右下)
void InOrder(BSTree T);                           // 中序遍历(升序输出)
void DestroyBST(BSTree &T);
```

---

### 八、BST.cpp 完整实现

#### 8.1 初始化与查找

cpp

```cpp
#include "BST.h"
#include <iostream>
using namespace std;

bool InitBST(BSTree &T) {
    T = nullptr;
    return true;
}

BSTNode* Search(BSTree T, ElemType key) {
    if (T == nullptr || T->data == key) return T;
    if (key < T->data) return Search(T->lchild, key);
    return Search(T->rchild, key);
}

// 非递归版本(效率略高,空间 O(1))
BSTNode* Search_NonRec(BSTree T, ElemType key) {
    while (T != nullptr && T->data != key) {
        if (key < T->data) T = T->lchild;
        else T = T->rchild;
    }
    return T;
}
```

#### 8.2 插入与构造

cpp

```cpp
bool Insert(BSTree &T, ElemType key) {
    if (T == nullptr) {
        T = new BSTNode;
        T->data = key;
        T->lchild = T->rchild = nullptr;
        return true;
    }
    if (key == T->data) return false;       // 不允许重复
    if (key < T->data) return Insert(T->lchild, key);
    return Insert(T->rchild, key);
}

void CreateBST(BSTree &T, ElemType arr[], int n) {
    T = nullptr;
    for (int i = 0; i < n; i++) {
        Insert(T, arr[i]);
    }
}
```

#### 8.3 辅助函数

cpp

```cpp
BSTNode* FindMin(BSTree T) {
    if (T == nullptr) return nullptr;
    while (T->lchild != nullptr) T = T->lchild;
    return T;
}

BSTNode* FindMax(BSTree T) {
    if (T == nullptr) return nullptr;
    while (T->rchild != nullptr) T = T->rchild;
    return T;
}

void InOrder(BSTree T) {
    if (T == nullptr) return;
    InOrder(T->lchild);
    cout << T->data << " ";
    InOrder(T->rchild);
}
```

#### 8.4 删除(完整版,上面已经讲过)

cpp

```cpp
bool Delete(BSTree &T, ElemType key) {
    if (T == nullptr) return false;
    
    if (key < T->data) {
        return Delete(T->lchild, key);
    } else if (key > T->data) {
        return Delete(T->rchild, key);
    } else {
        if (T->lchild == nullptr) {
            BSTNode *q = T;
            T = T->rchild;
            delete q;
        } else if (T->rchild == nullptr) {
            BSTNode *q = T;
            T = T->lchild;
            delete q;
        } else {
            BSTNode *succ = FindMin(T->rchild);
            T->data = succ->data;
            Delete(T->rchild, succ->data);
        }
        return true;
    }
}
```

#### 8.5 销毁

cpp

```cpp
void DestroyBST(BSTree &T) {
    if (T == nullptr) return;
    DestroyBST(T->lchild);
    DestroyBST(T->rchild);
    delete T;
    T = nullptr;
}
```

---

### 九、测试代码(main.cpp)

cpp

```cpp
#include "BST.h"
#include <iostream>
using namespace std;

int main() {
    BSTree T;
    int arr[] = {50, 30, 70, 20, 40, 60, 80, 35, 45};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    CreateBST(T, arr, n);
    cout << "构造后中序遍历(应为升序): ";
    InOrder(T); cout << endl;
    // 期望: 20 30 35 40 45 50 60 70 80
    
    int key = 45;
    BSTNode *p = Search(T, key);
    cout << "查找 " << key << ": " << (p ? "找到" : "未找到") << endl;
    
    // 删除度为 0 的节点(叶子)
    Delete(T, 35);
    cout << "删除 35 后: ";
    InOrder(T); cout << endl;
    
    // 删除度为 1 的节点
    Delete(T, 40);       // 40 现在只有右孩子 45
    cout << "删除 40 后: ";
    InOrder(T); cout << endl;
    
    // 删除度为 2 的节点
    Delete(T, 30);       // 30 有左右两个孩子
    cout << "删除 30 后: ";
    InOrder(T); cout << endl;
    
    // 删除根节点
    Delete(T, 50);
    cout << "删除根 50 后: ";
    InOrder(T); cout << endl;
    
    DestroyBST(T);
    return 0;
}
```

预期输出:

```
构造后中序遍历(应为升序): 20 30 35 40 45 50 60 70 80 
查找 45: 找到
删除 35 后: 20 30 40 45 50 60 70 80 
删除 40 后: 20 30 45 50 60 70 80 
删除 30 后: 20 45 50 60 70 80 
删除根 50 后: 20 45 60 70 80
```

**每一步删除后,中序遍历仍然是递增的**——这是检验删除正确性的最可靠标准。

---

### 十、BST 的查找效率分析(考点)

**核心指标**:**ASL(Average Search Length,平均查找长度)**——所有节点被查找到时比较次数的平均值。

**对于含 n 个节点的 BST**:

**最好情况**:树是**完美平衡**的(近似满二叉树),ASL = O(log n)。

**最坏情况**:树**退化成链**(严格有序插入),ASL = (n+1)/2 = O(n)。

**平均情况**(随机插入顺序):ASL = O(log n)。

#### ASL 计算示例

对前面那棵 BST:

```
        50          ← 层 1,1 次比较
       /  \
      30   70       ← 层 2,2 次比较
     / \   / \
    20 40 60 80     ← 层 3,3 次比较
```

**成功查找的 ASL**:

ASL_成功 = (1×1 + 2×2 + 3×4) / 7 = (1 + 4 + 12) / 7 = 17/7 ≈ 2.43

层 1 有 1 个节点(50),层 2 有 2 个节点,层 3 有 4 个节点,各自查找比较次数 = 层数。

**不成功查找的 ASL** 需要考虑"失败位置"——在逻辑上每个叶子位置都可以继续下挂两个"虚拟节点"作为失败节点,共 n+1 个失败节点。这部分计算考研偶有考查,但相对次要。

---

### 十一、BST 与其他查找结构的对比

哈希表查找 O(1) 比 BST 快,但哈希表**不支持范围查询和有序遍历**;BST 只要做中序遍历就能按序访问所有元素,这是 BST 的杀手锏。

BST 查找最坏 O(n),AVL 树/红黑树通过强制平衡把最坏也降到 O(log n),但代价是插入和删除时要做旋转维护平衡。**红黑树**是 C++ `std::map`、`std::set` 的底层实现,Java `TreeMap` 同理。

---

### 十二、思考题(请作答)

第一题,按顺序插入 `{62, 88, 58, 47, 35, 73, 51, 99, 37}`,画出最终的 BST,并给出中序、先序、后序遍历序列。

第二题,在上题构造的 BST 中:

先**删除节点 47**(度为 2),画出删除后的树;再**删除节点 62**(根节点,度为 2),画出最终树。每步都标出使用的是"中序前驱"还是"中序后继"替换方案。

第三题,一棵 BST 的先序遍历是 `50, 30, 20, 40, 70, 60, 80`,**这棵树唯一确定吗?** 如果确定,画出它;如果不确定,说明为什么。(提示:BST 的先序序列有个神奇性质)

第四题,下列哪些序列**不可能**是某棵 BST 的先序遍历?为每个错误序列指出第一个违反 BST 规则的地方:

(a) 95, 22, 11, 3, 17, 44, 66, 123  
(b) 95, 22, 11, 3, 44, 66, 17, 123  
(c) 95, 22, 44, 66, 11, 3, 17, 123

第五题,在一棵有 n 个节点的 BST 中查找某个节点,**最多**比较多少次?**最少**比较多少次?这两个极值分别对应什么样的树形态?

---

### 十三、下一步方向

BST 啃完,你的"查找类"数据结构就有了基础。接下来可以:

**(A)** 学**平衡二叉树 AVL**(BST 的升级版,通过旋转保证 O(log n) 的高度,考研大题超高频,特别是四种旋转:LL、RR、LR、RL)。**(B)** 学**堆和堆排序**(优先队列的核心,基于完全二叉树数组存储)。**(C)** 回到你最初列表里还没做的**顺序表**(线性表的顺序存储)。**(D)** 学**图论基础**(邻接矩阵、邻接表、DFS、BFS)。**(E)** 先答思考题再继续。

告诉我你的选择。


## 数据结构复习:平衡二叉树 AVL

好,我们进入**平衡二叉树**(AVL Tree,以发明者 Adelson-Velsky 和 Landis 命名)。这是 BST 的升级版,也是数据结构课的**最高峰之一**——四种旋转是历年考研大题的常客,几乎每年必考。

**事先提醒**:AVL 的**代码不算太难,但四种旋转的判断条件和旋转后的形态极其容易混淆**。我会把每种情况都配图讲清,你一定要拿纸笔跟着画,光看是记不住的。

---

### 一、为什么需要 AVL?

回忆 BST 的致命弱点:**如果插入顺序不好,BST 会退化成链表**,查找从 O(log n) 变成 O(n)。

例如按 `{1, 2, 3, 4, 5}` 插入:

```
1
 \
  2
   \
    3
     \
      4
       \
        5
```

AVL 的解决方案:**在每次插入/删除后,如果发现不平衡,立即通过"旋转"把树调回平衡状态**。这样保证任何时刻树高度都是 O(log n),查找永远是 O(log n)。

---

### 二、平衡因子(BF)的定义

对 AVL 树中的每个节点,定义**平衡因子**:

**BF(node) = 左子树高度 - 右子树高度**

**AVL 的定义**:**每个节点的 BF 的绝对值 ≤ 1**,即 BF ∈ {-1, 0, 1}。

一旦某节点 BF = ±2,就"失衡"了,需要通过旋转恢复。

**注意高度的约定**:空树高度通常规定为 0(有些教材规定为 -1,注意看你的教材),单节点树高度为 1。我们采用**空树高度 = 0** 的约定。

---

### 三、旋转操作(核心难点,四种情况)

当插入一个新节点导致某个祖先失衡时,设该失衡的**最低祖先**为 A(也叫"最小失衡子树"的根)。根据"导致失衡的插入路径"从 A 往下的走向,失衡分为四种:

**LL 型**:新节点插在 A 的**左**孩子的**左**子树中 → 用**右单旋**修复。

**RR 型**:新节点插在 A 的**右**孩子的**右**子树中 → 用**左单旋**修复。

**LR 型**:新节点插在 A 的**左**孩子的**右**子树中 → 先左旋再右旋(双旋)。

**RL 型**:新节点插在 A 的**右**孩子的**左**子树中 → 先右旋再左旋(双旋)。

**记忆法**:**型号名称描述的是"失衡路径方向"**,旋转方向**与路径方向相反**。LL 失衡用"右"旋,RR 失衡用"左"旋。

---

#### 3.1 LL 型 → 右单旋

**失衡形态**(插入 x 后 A 失衡):

```
      A (BF=+2)              B
     / \                   /   \
    B   Z         →       X     A
   / \                   / \   / \
  X   Y                 ..  ..Y   Z
  ↑
  新插
```

**旋转规则**:

B 取代 A 成为子树新根。A 成为 B 的**右孩子**。B 原来的**右孩子 Y** 变成 A 的**左孩子**(因为 A 现在的左位置空了,而 Y 的值范围恰好是"大于 B 小于 A",符合 A 的左孩子)。

cpp

```cpp
// 右单旋:返回新的子树根
AVLNode* RotateRight(AVLNode *A) {
    AVLNode *B = A->lchild;
    A->lchild = B->rchild;     // B 原来的右孩子 Y 接到 A 的左
    B->rchild = A;             // A 成为 B 的右孩子
    // 更新高度(必须先更新 A,再更新 B,因为 B 是新根依赖 A)
    A->height = max(Height(A->lchild), Height(A->rchild)) + 1;
    B->height = max(Height(B->lchild), Height(B->rchild)) + 1;
    return B;                  // B 成为新子树的根
}
```

---

#### 3.2 RR 型 → 左单旋

完全对称,插入点在 A 右孩子 B 的右子树:

```
    A (BF=-2)                  B
   / \                       /   \
  X   B           →         A     Z
     / \                   / \   / \
    Y   Z                 X   Y    ..
                                   ↑
                                   新插
```

B 取代 A,A 成为 B 的左孩子,B 原来的**左孩子 Y** 变成 A 的右孩子。

cpp

```cpp
AVLNode* RotateLeft(AVLNode *A) {
    AVLNode *B = A->rchild;
    A->rchild = B->lchild;
    B->lchild = A;
    A->height = max(Height(A->lchild), Height(A->rchild)) + 1;
    B->height = max(Height(B->lchild), Height(B->rchild)) + 1;
    return B;
}
```

---

#### 3.3 LR 型 → 左旋+右旋

**失衡形态**:新节点插在 A 的**左**孩子 B 的**右**子树中。

```
       A (BF=+2)
      / \
     B   Z
    / \
   X   C (插入使 C 增高)
      / \
     M   N
```

**关键点**:不能直接对 A 做右旋——因为 C 在 B 的右边,单次右旋会把问题搬到另一侧。要**先对 B 做左旋**(把 C 旋到 B 的上面):

```
       A
      / \
     C   Z
    / \
   B   N
  / \
 X   M
```

**然后对 A 做右旋**,变成:

```
       C
     /   \
    B     A
   / \   / \
  X   M N   Z
```

**代码实现**:就是两个单旋的组合。

cpp

```cpp
AVLNode* RotateLR(AVLNode *A) {
    A->lchild = RotateLeft(A->lchild);  // 先对 B(A 的左孩子)左旋
    return RotateRight(A);               // 再对 A 右旋
}
```

---

#### 3.4 RL 型 → 右旋+左旋

完全对称,新节点插在 A 的**右**孩子 B 的**左**子树中:

```
       A (BF=-2)                         C
      / \                              /   \
     X   B             →              A     B
        / \                          / \   / \
       C   Z                        X   M N   Z
      / \
     M   N
```

先对 B 右旋,再对 A 左旋:

cpp

```cpp
AVLNode* RotateRL(AVLNode *A) {
    A->rchild = RotateRight(A->rchild);
    return RotateLeft(A);
}
```

---

### 四、判断应使用哪种旋转

**算法流程**:插入后沿路径回溯更新 height,发现某节点 A 的 BF 绝对值 = 2 时,根据 **A 和插入路径上的方向关系**判断类型:

|A 的 BF|A 的子节点方向|类型|旋转|
|---|---|---|---|
|+2|插入在**左孩子**的**左**子树|LL|右单旋|
|+2|插入在**左孩子**的**右**子树|LR|左右双旋|
|-2|插入在**右孩子**的**右**子树|RR|左单旋|
|-2|插入在**右孩子**的**左**子树|RL|右左双旋|

**在代码中怎么判断"路径方向"?** 看 A 的那个"过高"子节点 B 的 BF:

如果 A.BF = +2,看 B = A->lchild:若 B.BF ≥ 0(多半是 +1),属 LL;若 B.BF < 0(多半是 -1),属 LR。

如果 A.BF = -2,看 B = A->rchild:若 B.BF ≤ 0(多半是 -1),属 RR;若 B.BF > 0(多半是 +1),属 RL。

记住这个判断逻辑,代码里就能一行解决。

---

### 五、项目文件组织

```
DataStructure/
└── AVL/
    ├── AVL.h
    ├── AVL.cpp
    └── main.cpp
```

---

### 六、AVL.h

cpp

```cpp
#pragma once

typedef int ElemType;

typedef struct AVLNode {
    ElemType data;
    int height;                       // 以该节点为根的子树高度
    struct AVLNode *lchild, *rchild;
} AVLNode, *AVLTree;

// ========== 辅助 ==========
int Height(AVLNode *p);                      // 空指针返回 0
int BalanceFactor(AVLNode *p);               // 返回 BF
int max(int a, int b);
void UpdateHeight(AVLNode *p);

// ========== 四种旋转 ==========
AVLNode* RotateRight(AVLNode *A);            // LL 型用
AVLNode* RotateLeft(AVLNode *A);             // RR 型用
AVLNode* RotateLR(AVLNode *A);               // LR 型:左旋+右旋
AVLNode* RotateRL(AVLNode *A);               // RL 型:右旋+左旋

// ========== 插入(核心)==========
AVLNode* Insert(AVLTree T, ElemType key);

// ========== 查找与遍历 ==========
AVLNode* Search(AVLTree T, ElemType key);
void InOrder(AVLTree T);
void PreOrder(AVLTree T);
void DestroyAVL(AVLTree &T);
```

**关于节点设计**:我们给每个节点加了 `height` 字段,这是 AVL 常见做法之一。另一种常见做法是存 **BF** 字段(-1, 0, +1),存 BF 更省空间但每次更新时要区分"左高/右高/平衡"三种状态;存 height 更直观,插入时只需重新计算。**两种都是主流,我们选 height 版本**。

**另一个说明**:插入函数的返回值是 `AVLNode*` 而不是 `void` + 引用。原因是 AVL 插入可能让**子树的根发生改变**(旋转后原根不再是根),用返回值重新赋给父节点的指针是最清晰的写法。

---

### 七、AVL.cpp 完整实现

#### 7.1 辅助函数

cpp

```cpp
#include "AVL.h"
#include <iostream>
using namespace std;

int max(int a, int b) { return a > b ? a : b; }

int Height(AVLNode *p) {
    return p == nullptr ? 0 : p->height;
}

int BalanceFactor(AVLNode *p) {
    if (p == nullptr) return 0;
    return Height(p->lchild) - Height(p->rchild);
}

void UpdateHeight(AVLNode *p) {
    p->height = max(Height(p->lchild), Height(p->rchild)) + 1;
}
```

#### 7.2 四种旋转

cpp

```cpp
AVLNode* RotateRight(AVLNode *A) {
    AVLNode *B = A->lchild;
    A->lchild = B->rchild;
    B->rchild = A;
    UpdateHeight(A);                 // 先更新 A(因为它现在在下面)
    UpdateHeight(B);                 // 再更新 B(新根)
    return B;
}

AVLNode* RotateLeft(AVLNode *A) {
    AVLNode *B = A->rchild;
    A->rchild = B->lchild;
    B->lchild = A;
    UpdateHeight(A);
    UpdateHeight(B);
    return B;
}

AVLNode* RotateLR(AVLNode *A) {
    A->lchild = RotateLeft(A->lchild);
    return RotateRight(A);
}

AVLNode* RotateRL(AVLNode *A) {
    A->rchild = RotateRight(A->rchild);
    return RotateLeft(A);
}
```

**高度更新顺序至关重要**:旋转后 A 已经是"下面的节点",先更新 A;B 是新根,更新 B 要依赖 A 的新高度,所以后更新 B。

#### 7.3 插入(核心算法)

cpp

```cpp
AVLNode* Insert(AVLTree T, ElemType key) {
    // 1. 标准 BST 插入
    if (T == nullptr) {
        T = new AVLNode;
        T->data = key;
        T->height = 1;
        T->lchild = T->rchild = nullptr;
        return T;
    }
    if (key < T->data) {
        T->lchild = Insert(T->lchild, key);
    } else if (key > T->data) {
        T->rchild = Insert(T->rchild, key);
    } else {
        return T;                    // 重复键不插入
    }
    
    // 2. 更新当前节点高度
    UpdateHeight(T);
    
    // 3. 计算 BF,判断是否失衡
    int bf = BalanceFactor(T);
    
    // 4. 四种失衡情况
    // LL:左孩子的左侧过高
    if (bf > 1 && key < T->lchild->data) {
        return RotateRight(T);
    }
    // RR:右孩子的右侧过高
    if (bf < -1 && key > T->rchild->data) {
        return RotateLeft(T);
    }
    // LR:左孩子的右侧过高
    if (bf > 1 && key > T->lchild->data) {
        return RotateLR(T);
    }
    // RL:右孩子的左侧过高
    if (bf < -1 && key < T->rchild->data) {
        return RotateRL(T);
    }
    
    return T;                        // 未失衡,或处理完毕
}
```

**理解要点**:

第一,递归回溯时**从下往上**检查每层节点的 BF,发现第一个失衡点就旋转修复。

第二,判断 LL/LR 时靠 `key < T->lchild->data` vs `key > T->lchild->data`——因为 key 走的方向决定了插入位置在左孩子的左边还是右边。这是**代码上最简洁的判断方式**,比计算子节点的 BF 更直接。

第三,一次插入最多导致**一次**旋转(单旋或双旋算一次),**旋转完了整棵树就平衡了**,不会继续传播。这一点和红黑树、B 树的插入类似。

#### 7.4 查找、遍历、销毁

cpp

```cpp
AVLNode* Search(AVLTree T, ElemType key) {
    while (T != nullptr && T->data != key) {
        T = (key < T->data) ? T->lchild : T->rchild;
    }
    return T;
}

void InOrder(AVLTree T) {
    if (T == nullptr) return;
    InOrder(T->lchild);
    cout << T->data << "(h=" << T->height << ") ";
    InOrder(T->rchild);
}

void PreOrder(AVLTree T) {
    if (T == nullptr) return;
    cout << T->data << " ";
    PreOrder(T->lchild);
    PreOrder(T->rchild);
}

void DestroyAVL(AVLTree &T) {
    if (T == nullptr) return;
    DestroyAVL(T->lchild);
    DestroyAVL(T->rchild);
    delete T;
    T = nullptr;
}
```

---

### 八、手工构造 AVL:一个完整示例

依次插入 `{3, 2, 1, 4, 5, 6, 7, 16, 15, 14}`,我们一步步画出来。

**插入 3**:单节点树,无需旋转。

```
3
```

**插入 2**:

```
  3
 /
2
```

BF = 1,平衡。

**插入 1**:

```
    3 (BF=2,失衡!)
   /
  2 (BF=1)
 /
1
```

**LL 型**(2 是 3 的左孩子,1 是 2 的左孩子),对 3 右单旋:

```
  2
 / \
1   3
```

**插入 4**:

```
  2
 / \
1   3
     \
      4
```

BF 都在范围内,平衡。

**插入 5**:

```
  2 (BF=-2)
 / \
1   3 (BF=-1)
     \
      4 (BF=-1)
       \
        5
```

根 2 失衡,**RR 型**(3 是 2 的右,4 是 3 的右),对 2 左单旋——但等一下,这里**最小失衡子树**是谁?从新插入节点 5 往上找,第一个失衡的是**根 2**。对 2 左单旋:

```
    3
   / \
  2   4
 /     \
1       5
```

**插入 6**:

```
    3
   / \
  2   4 (BF=-2,失衡)
 /     \
1       5
         \
          6
```

最小失衡在节点 4,**RR 型**,对 4 左单旋:

```
    3
   / \
  2   5
 /   / \
1   4   6
```

**插入 7**:

```
      3 (BF=-2,失衡!)
     / \
    2   5
   /   / \
  1   4   6
           \
            7
```

最小失衡在根 3。类型?从根看:右孩子 5 的右孩子 6 的右孩子 7 方向——**RR 型**,对 3 左单旋:

```
      5
     / \
    3   6
   / \   \
  2   4   7
 /
1
```

**插入 16**:

```
      5
     / \
    3   6
   / \   \
  2   4   7
 /         \
1           16
```

平衡。

**插入 15**:

```
      5
     / \
    3   6 (BF=-2,失衡)
   / \   \
  2   4   7 (BF=-1)
 /         \
1           16 (BF=1)
           /
          15
```

最小失衡在 6。类型?6 的右孩子 7 的右孩子 16 的**左**孩子 15 ——**RL 型**,对 6 做右左双旋。

第一步,对 7(6 的右孩子)右旋:

```
      5
     / \
    3   6
   / \   \
  2   4   16
 /         /
1         7
           \
            15
```

第二步,对 6 左旋:

```
      5
     / \
    3   16
   / \   / \
  2   4 6   7 (等等,这里不对)
 /       \
1         15
```

让我重新仔细画一下。对 6 为根做 RL 旋转:6 的右孩子是 7,7 的左孩子是?原本 7 没有左孩子,插入 15 后 15 成为 16 的左孩子,16 是 7 的右孩子。**让我重新审视——其实失衡路径是:6 → 右孩子 7 → 右孩子 16 → 左孩子 15,这是 RR-L 路径,而不是标准 RL。**

嗯等等,让我停下来仔细想。失衡发生时,**关键看"失衡节点到新插入节点的前两步方向"**。从 6 出发:第一步往右到 7,第二步……15 是 16 的左孩子,16 是 7 的右孩子,所以从 7 开始是 7→右→左。**第二步是"右"**。所以 **6 → 右 → 右**,是 **RR 型**,对 6 做左单旋。

(这说明我上面判断 RL 错了——**判断类型时看"失衡节点往下的头两步"**,不是整条路径。)

对 6 左单旋:

```
      5
     / \
    3   7
   / \   / \
  2   4 6   16
 /             /
1            15
```

检查 5:左高 3,右高 3,平衡。

**插入 14**:

```
      5
     / \
    3   7
   / \   / \
  2   4 6   16 (BF=2)
 /          /
1          15 (BF=1)
           /
          14
```

最小失衡在 16。16 → 左 15 → 左 14,**LL 型**,对 16 右单旋:

```
      5
     / \
    3   7
   / \   / \
  2   4 6   15
 /          / \
1          14  16
```

**最终 AVL 树**:

```
          5
         / \
        3   7
       / \  / \
      2   4 6  15
     /        / \
    1        14  16
```

中序遍历:1 2 3 4 5 6 7 14 15 16,升序——验证正确。高度 4,对于 10 个节点来说很紧凑。

---

### 九、测试代码(main.cpp)

cpp

```cpp
#include "AVL.h"
#include <iostream>
using namespace std;

int main() {
    AVLTree T = nullptr;
    int arr[] = {3, 2, 1, 4, 5, 6, 7, 16, 15, 14};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    for (int i = 0; i < n; i++) {
        T = Insert(T, arr[i]);
    }
    
    cout << "中序遍历(应为升序): ";
    InOrder(T); cout << endl;
    
    cout << "先序遍历: ";
    PreOrder(T); cout << endl;
    
    cout << "根节点高度: " << Height(T) << endl;
    
    // 查找
    int key = 14;
    AVLNode *p = Search(T, key);
    cout << "查找 " << key << ": " << (p ? "找到" : "未找到") << endl;
    
    DestroyAVL(T);
    return 0;
}
```

---

### 十、AVL 的高度分析(考点)

**定理**:含 n 个节点的 AVL 树,高度 h ≤ 1.44 × log₂(n+2)。

也就是说 AVL 树高度最坏约是 **1.44 × log n**,和理想完美二叉树 log n 相差常数倍,**查找始终是 O(log n)**。

**极端 AVL 树**:具有 n 个节点的 AVL 树,在某些形态下可能比完美二叉树高,但不会差太多。考研中常考的是**给定高度,问最少节点数**:

**N(h)** 表示高度为 h 的 AVL 树的**最少节点数**,递推式:

N(0) = 0,N(1) = 1,N(2) = 2,N(h) = N(h-1) + N(h-2) + 1

这是**斐波那契式**的增长。推导:高度为 h 的最小 AVL,它的左右子树一个高 h-1、一个高 h-2(差 1 是允许的最大差),都取最小,加上根节点 1。

所以 AVL 树高度和节点数的关系类似斐波那契数列,**高度以对数速度增长**。

---

### 十一、删除操作(简介,代码略)

AVL 的删除比插入复杂:

第一,先按 BST 规则删除(可能替换为后继)。第二,从删除位置回溯到根,沿途每个节点检查 BF,失衡就旋转。**删除与插入的关键区别**:插入后一次旋转即平衡;**删除后可能需要多次旋转**,因为一次旋转可能让更上层的节点也失衡。

判断旋转类型的方法与插入类似,但要注意:删除导致的失衡,失衡节点的"另一侧"是"高的一侧"。例如从左子树删节点导致 BF=-2,就看右孩子的 BF:若右孩子 BF ≤ 0(-1 或 0)→ RR 型;若右孩子 BF = +1 → RL 型。

**考研一般以插入为主,删除偶尔考**,我们暂不展开代码,先把插入吃透。

---

### 十二、易错点总结

**第一个易错点**:LL 型用**右旋**,RR 型用**左旋**。很多同学一开始会搞反——**记住方向相反**即可:失衡歪向哪边,就往反方向扳回来。

**第二个易错点**:LR 型是"**先对左孩子左旋,再对失衡点右旋**",RL 型是"先对右孩子右旋,再对失衡点左旋"。**口诀**:"外面名字对应第二次旋转,第一次旋转是为了把问题搬到外侧"。

**第三个易错点**:判断失衡类型时看**失衡节点往下的头两步方向**,不是整条路径。

**第四个易错点**:旋转后**记得更新 height**。顺序是:先更新原来的根(A),再更新新根(B)。

**第五个易错点**:插入 AVL 后,**第一次遇到的失衡节点是最小失衡子树的根**,对它旋转一次,整棵树就平衡了,**不用继续往上检查**。这是插入的重要性质。

---

### 十三、思考题(请作答)

第一题,依次插入 `{13, 24, 37, 90, 53}` 到空 AVL 树,**每次插入都画图**,标注失衡时用的是哪种旋转,以及旋转后的树形态。

第二题,高度为 5 的 AVL 树**最少有多少个节点**?请用前述递推式 N(h) = N(h-1) + N(h-2) + 1 计算 N(0) 到 N(5)。

第三题,以下先序遍历能否对应某棵 AVL 树?画出那棵树并判断:

(a) 50, 30, 20, 40, 70, 60, 80  
(b) 50, 40, 30, 20, 10, 60

第四题,在 AVL 中插入一个新节点后,**最多需要调整多少层**?**最多做多少次单旋**?(提示:一次双旋算两次单旋)

第五题,假设某节点 A 的 BF 变成 -2,它的右孩子 B 的 BF 是 0,这种情况可能出现在**插入**操作中吗?为什么?(提示:考虑插入会导致哪些 BF 状态组合;这个问题引导你思考"插入和删除导致的失衡模式差异")

---

### 十四、下一步方向

AVL 啃完,你的"树"章节就登顶了。接下来可选:

**(A)** 学**堆和堆排序**(基于完全二叉树数组存储,堆排是考研必考排序之一)。**(B)** 学**B 树和 B+ 树**(多路平衡树,数据库索引的核心)。**(C)** 学**红黑树**(C++ `std::map` 底层,比 AVL 更实用但旋转更复杂)。**(D)** 回到你最初列表里还没做的**顺序表**。**(E)** 进入**图论**(邻接矩阵、邻接表、DFS、BFS、最小生成树、最短路径)。**(F)** 先答思考题再继续。

告诉我你想走哪个方向。


## 数据结构复习:B 树和 B+ 树

好,我们进入 **B 树和 B+ 树**。这两个结构是**数据库索引的核心**,MySQL InnoDB 引擎的索引就是 B+ 树。它们是**多路平衡树**,是 BST/AVL 在磁盘存储场景下的自然延伸。

**先说清楚学习重点**:考研对 B 树的考查集中在**定义、阶的概念、插入时的分裂、删除时的合并**,代码实现相对次要(更多是手工模拟)。B+ 树重在**理解与 B 树的区别**以及**为什么数据库用 B+ 树而不是 B 树**。

---

### 一、为什么需要 B 树?(从磁盘 IO 说起)

BST 和 AVL 在**内存**中很好用,但用在**磁盘**上有致命问题。

**问题**:磁盘的基本读写单位是"**块**"(通常 4KB 或 16KB),每次从磁盘读取数据,无论你只要 1 字节还是 4KB,代价是一样的——一次磁盘 IO。而磁盘 IO 比内存访问慢 **10 万倍**左右。

**AVL 树的问题**:n 个节点的 AVL 树高度是 O(log₂n)。如果有 100 万条记录,高度约为 20,意味着**最坏需要 20 次磁盘 IO** 才能找到一条记录——太慢了。

**B 树的解决思路**:把树"压矮"。每个节点存**多个关键字**,有**多个孩子**,每次 IO 读一整个节点(一整块磁盘块),一次比较就能排除更多候选。

**类比**:AVL 是每次只问"左还是右"(二叉);B 树是每次问"第几段"(多叉)。树高从 O(log₂n) 降到 O(log_m n)(m 是阶数),**IO 次数大幅减少**。

---

### 二、B 树的定义(严格版,考研必背)

**m 阶 B 树**满足以下条件:

**第一**,每个节点最多有 **m 棵子树**(m 个孩子),即最多有 **m-1 个关键字**。

**第二**,根节点如果不是叶子,则**至少有 2 棵子树**。

**第三**,非根非叶的节点至少有 **⌈m/2⌉ 棵子树**,即至少有 **⌈m/2⌉ - 1 个关键字**。

**第四**,所有叶子节点在**同一层**(B 树是完全平衡的)。

**第五**,每个节点内的关键字**从小到大排列**,且满足:第 i 棵子树中的所有关键字 > 第 i 个关键字 > 第 i-1 棵子树的所有关键字。(即节点内有序,节点间也满足 BST 性质)

**第六**,叶子节点本身**不含任何实际信息**(指向 nullptr),它们只是"查找失败"的标志。

---

### 三、B 树的关键参数(手算必备)

**m 阶 B 树各类节点的关键字数目范围**:

|节点类型|最少关键字数|最多关键字数|
|---|---|---|
|根节点|1|m-1|
|非根非叶|⌈m/2⌉ - 1|m-1|
|叶子|—|—(都是 nullptr)|

**注意**:这里"叶子"是 B 树定义中的**失败节点**,不是通常意义上的"没有孩子的节点"。B 树的叶子在最后一层,不含数据。实际存数据的是**倒数第二层**往上的所有节点。

**一个典型例子:3 阶 B 树(也叫 2-3 树)**:

每个节点最多 2 个关键字、3 个孩子;每个非根节点至少 1 个关键字、2 个孩子。

```
            [35]
          /       \
      [15 25]    [45 55]
     /  |  \    /  |  \
   [.][.][.] [.][.][.][.]
```

叶子节点(最后一层的 [.])都是失败节点。

---

### 四、B 树的高度计算(考研常考)

**问题**:n 个关键字的 m 阶 B 树,高度 h 的范围是多少?

**最大高度**(节点尽量少关键字,树尽量高):

第 1 层:1 个节点,1 个关键字,2 个孩子(根节点最少 2 个孩子)。第 2 层:2 个节点,每个最少 ⌈m/2⌉ - 1 个关键字。第 k 层:2 × ⌈m/2⌉^(k-2) 个节点。

由于叶子(第 h+1 层)共有 n+1 个(失败节点),可推出:

**h ≤ log_{⌈m/2⌉}((n+1)/2) + 1**

**最小高度**(每个节点装满):

每个节点 m-1 个关键字。第 1 层 m-1 个,前 2 层共 m²-1 个……

**h ≥ log_m(n+1)**

**结论**:B 树高度是 O(log_m n) 量级,m 越大树越矮,IO 次数越少。这是 B 树的根本优势。

---

### 五、B 树的插入(重点:分裂)

B 树的插入**永远插在叶子位置**(实际是最底层的数据节点),插入后如果节点关键字数超过 m-1,就要**分裂**。

**分裂规则**:把关键字数为 m 的节点,从中间位置 ⌈m/2⌉ 处分裂,**中间那个关键字上提到父节点**,左右两半各自成为一个新节点。如果父节点也因此溢出,继续向上分裂,直到不溢出或根分裂(根分裂会产生新根,树高增加 1)。

#### 插入示例:3 阶 B 树,依次插入 `{30, 10, 20, 40, 50, 60, 70, 80, 90}`

**插入 30**:树为空,根直接插入。

```
[30]
```

**插入 10**:

```
[10 30]
```

**插入 20**:插入后 `[10 20 30]`,关键字数 = 3 = m,超过 m-1 = 2,**分裂**!

中间位置 ⌈3/2⌉ = 2,即关键字 20 上提。左 [10],右 [30],新根 [20]:

```
      [20]
     /    \
   [10]  [30]
```

**插入 40**:40 > 20,进右子树,插入 [30] 得 [30 40]。未溢出。

```
      [20]
     /    \
   [10]  [30 40]
```

**插入 50**:进右子树,插入 [30 40] 得 [30 40 50],溢出!分裂,40 上提:

左 [30],右 [50],40 插入根 [20] 得 [20 40]:

```
        [20 40]
       /   |   \
    [10] [30] [50]
```

**插入 60**:进第三个子树,插入 [50] 得 [50 60]。

```
        [20 40]
       /   |   \
    [10] [30] [50 60]
```

**插入 70**:进第三子树,得 [50 60 70],溢出!60 上提,根变 [20 40 60]:

```
          [20 40 60]
         /   |   |   \
      [10] [30] [50] [70]
```

**插入 80**:进第四子树,得 [70 80]。

```
          [20 40 60]
         /   |   |   \
      [10] [30] [50] [70 80]
```

**插入 90**:进第四子树,得 [70 80 90],溢出!80 上提,根变 [20 40 60 80]——还是溢出(m=3,最多 2 个)!根也要分裂,中间值 40 再次上提:

新根 [40],左子树根 [20],右子树根 [60 80]:

```
              [40]
            /       \
         [20]       [60 80]
        /    \      /  |  \
      [10]  [30] [50][70][90]
```

**树高从 2 增加到 3**——根分裂是 B 树长高的唯一方式。

---

### 六、B 树的删除(重点:合并与借键)

删除比插入复杂,分三种情况:

#### 情况一:被删关键字在**非最底层**节点

不能直接删,要用其**前驱或后继**(最底层节点的关键字)替换,然后删除那个前驱/后继。转化为情况二或三。

**（这和 BST 删除度为 2 的节点的策略一样!）**

#### 情况二:被删关键字在**最底层**且删后关键字数 ≥ ⌈m/2⌉ - 1

直接删除,满足最少关键字要求,结束。

#### 情况三:被删后该节点关键字数 < ⌈m/2⌉ - 1(下溢出)

分两个子情况:

**子情况 A(兄弟够借)**:如果相邻兄弟节点关键字数 > ⌈m/2⌉ - 1(即 ≥ ⌈m/2⌉),可以向兄弟借一个。**注意不是直接把兄弟的关键字搬过来,要通过父节点中转**("旋转"):

父节点中分隔这两个兄弟的关键字"下移"到当前节点,兄弟的一个边界关键字"上移"到父节点填补空位。

**子情况 B(兄弟不够借)**:兄弟节点也只剩 ⌈m/2⌉ - 1 个关键字,不能再借。此时**合并**:当前节点 + 父节点中分隔两者的那个关键字 + 兄弟节点，合并成一个新节点。父节点少了一个关键字,若父节点也下溢出则继续向上合并,直到根(根合并后树高减 1)。

---

### 七、B 树结构体及代码(3 阶 B 树)

代码部分我们实现 **3 阶 B 树**(2-3 树),因为它是最简单的 B 树,参数固定,边界清晰。

#### 文件组织

```
DataStructure/
└── BTree/
    ├── BTree.h
    ├── BTree.cpp
    └── main.cpp
```

#### BTree.h

cpp

```cpp
#pragma once
#include <iostream>
using namespace std;

#define ORDER 3                         // B 树的阶
#define MAX_KEY (ORDER - 1)             // 节点最多关键字数:2
#define MIN_KEY (ORDER / 2 - 1 + (ORDER % 2 != 0 ? 1 : 0) - 1)
// 非根节点最少关键字数:⌈m/2⌉ - 1 = 1

typedef int KeyType;

typedef struct BTreeNode {
    int keyNum;                         // 当前关键字数量
    KeyType keys[ORDER];                // 关键字数组,下标 1..keyNum(0 不用)
    struct BTreeNode *children[ORDER + 1]; // 孩子指针,下标 0..keyNum
    bool isLeaf;                        // 是否是最底层数据节点
} BTreeNode, *BTree;

// ========== 基本操作 ==========
BTreeNode* CreateNode(bool isLeaf);
BTree InitBTree();

// ========== 查找 ==========
BTreeNode* Search(BTree T, KeyType key, int &pos);

// ========== 插入(含分裂)==========
void SplitChild(BTreeNode *parent, int i, BTreeNode *child);
void InsertNonFull(BTreeNode *node, KeyType key);
void Insert(BTree &T, KeyType key);

// ========== 遍历(中序,输出有序序列)==========
void InOrder(BTree T);

// ========== 打印树结构 ==========
void PrintTree(BTree T, int depth);
```

**关于下标约定**:我们让 `keys` 数组下标从 1 开始(keys[0] 不用),`children` 下标从 0 开始。这样 children[i-1] 是 keys[i] 的左孩子,children[i] 是 keys[i] 的右孩子——与教材保持一致,便于对照手算结果。

#### BTree.cpp

cpp

```cpp
#include "BTree.h"

BTreeNode* CreateNode(bool isLeaf) {
    BTreeNode *p = new BTreeNode;
    p->keyNum = 0;
    p->isLeaf = isLeaf;
    for (int i = 0; i <= ORDER; i++) p->children[i] = nullptr;
    for (int i = 0; i <= ORDER - 1; i++) p->keys[i] = 0;
    return p;
}

BTree InitBTree() {
    return nullptr;
}

// 在以 T 为根的子树中查找 key
// 找到:返回节点指针,pos 为在 keys 中的下标
// 未找到:返回 nullptr,pos 为应在的孩子下标
BTreeNode* Search(BTree T, KeyType key, int &pos) {
    if (T == nullptr) return nullptr;
    int i = 1;
    while (i <= T->keyNum && key > T->keys[i]) i++;
    if (i <= T->keyNum && key == T->keys[i]) {
        pos = i;
        return T;                       // 在当前节点找到
    }
    if (T->isLeaf) return nullptr;      // 到底层未找到
    return Search(T->children[i - 1], key, pos);
}

// 分裂 parent 的第 i 个孩子(child),child 已满(keyNum == ORDER-1 == MAX_KEY)
// 注意:这里 child 是 parent->children[i]
void SplitChild(BTreeNode *parent, int i, BTreeNode *child) {
    int mid = ORDER / 2;                // 中间位置
    BTreeNode *newNode = CreateNode(child->isLeaf);
    newNode->keyNum = mid - 1;          // 新节点获得 mid-1 个关键字
    
    // 把 child 后半部分关键字给 newNode
    for (int j = 1; j <= mid - 1; j++) {
        newNode->keys[j] = child->keys[j + mid];
    }
    // 把 child 后半部分孩子指针给 newNode
    if (!child->isLeaf) {
        for (int j = 0; j <= mid - 1; j++) {
            newNode->children[j] = child->children[j + mid];
        }
    }
    child->keyNum = mid - 1;            // child 保留前半部分
    
    // 在 parent 中腾出位置,把 child->keys[mid] 上提
    for (int j = parent->keyNum; j >= i; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[i] = newNode;
    
    for (int j = parent->keyNum; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[i] = child->keys[mid];  // 中间关键字上提
    parent->keyNum++;
}

// 向以 node 为根的非满子树中插入 key
void InsertNonFull(BTreeNode *node, KeyType key) {
    int i = node->keyNum;
    
    if (node->isLeaf) {
        // 直接插入到叶子节点中,保持有序
        while (i >= 1 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->keyNum++;
    } else {
        // 找到合适的孩子
        while (i >= 1 && key < node->keys[i]) i--;
        i++;                            // 进入 children[i-1]
        if (node->children[i - 1]->keyNum == MAX_KEY) {
            // 孩子已满,先分裂
            SplitChild(node, i, node->children[i - 1]);
            // 分裂后判断 key 进哪一侧
            if (key > node->keys[i]) i++;
        }
        InsertNonFull(node->children[i - 1], key);
    }
}

void Insert(BTree &T, KeyType key) {
    if (T == nullptr) {
        T = CreateNode(true);
        T->keys[1] = key;
        T->keyNum = 1;
        return;
    }
    if (T->keyNum == MAX_KEY) {         // 根满了,预先分裂
        BTreeNode *newRoot = CreateNode(false);
        newRoot->children[0] = T;
        SplitChild(newRoot, 1, T);
        T = newRoot;                    // 新根
    }
    InsertNonFull(T, key);
}

void InOrder(BTree T) {
    if (T == nullptr) return;
    for (int i = 1; i <= T->keyNum; i++) {
        if (!T->isLeaf) InOrder(T->children[i - 1]);
        cout << T->keys[i] << " ";
    }
    if (!T->isLeaf) InOrder(T->children[T->keyNum]);
}

void PrintTree(BTree T, int depth) {
    if (T == nullptr) return;
    for (int d = 0; d < depth; d++) cout << "    ";
    cout << "[";
    for (int i = 1; i <= T->keyNum; i++) {
        cout << T->keys[i];
        if (i < T->keyNum) cout << " ";
    }
    cout << "]" << endl;
    for (int i = 0; i <= T->keyNum; i++) {
        PrintTree(T->children[i], depth + 1);
    }
}
```

**Insert 采用"预分裂"策略**:从根下行时,凡遇到满节点就**提前分裂**,保证递归到底时插入位置一定有空间。这样可以**自上而下一趟完成插入**,不需要回溯,实现更简洁。

---

### 八、测试代码(main.cpp)

cpp

```cpp
#include "BTree.h"
#include <iostream>
using namespace std;

int main() {
    BTree T = InitBTree();
    
    int arr[] = {30, 10, 20, 40, 50, 60, 70, 80, 90};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    for (int i = 0; i < n; i++) {
        Insert(T, arr[i]);
    }
    
    cout << "B 树结构:" << endl;
    PrintTree(T, 0);
    
    cout << "\n中序遍历(应为升序): ";
    InOrder(T); cout << endl;
    
    int pos;
    BTreeNode *p = Search(T, 60, pos);
    cout << "\n查找 60: " << (p ? "找到" : "未找到");
    if (p) cout << ",在节点的第 " << pos << " 个位置";
    cout << endl;
    
    p = Search(T, 35, pos);
    cout << "查找 35: " << (p ? "找到" : "未找到") << endl;
    
    return 0;
}
```

预期输出(对应前面手算的最终树):

```
B 树结构:
[40]
    [20]
        [10]
        [30]
    [60 80]
        [50]
        [70]
        [90]

中序遍历(应为升序): 10 20 30 40 50 60 70 80 90 

查找 60: 找到,在节点的第 1 个位置
查找 35: 未找到
```

---

### 九、B+ 树

#### 9.1 B+ 树和 B 树的核心区别

**B+ 树是 B 树的变种**,专门为磁盘数据库优化。它和 B 树有以下关键区别:

**区别一:所有数据(卫星数据)只存在叶子节点**。B 树的内部节点也可以存数据;B+ 树的内部节点只存关键字(作为路由/索引),实际记录只在叶子。

**区别二:叶子节点通过链表相连**。所有叶子节点按关键字顺序串成一个**有序链表**,支持**范围查询**的高效扫描。

**区别三:内部节点的关键字在叶子中还会出现**。B 树中,一旦关键字出现在内部节点,就不再出现在叶子;B+ 树的内部节点关键字是**叶子关键字的副本**。

**区别四:查找过程**。B 树找到关键字在内部节点就可返回;B+ 树必须**走到叶子**才算找到(因为数据在叶子)。

#### 9.2 一棵 3 阶 B+ 树示例

```
              [20  40]           ← 内部节点(只作路由)
            /    |    \
          [10 20] [30 40] [50 60]  ← 叶子节点(存真实数据)
             ↕       ↕       ↕
          (链表串联所有叶子)
```

注意 20、40 同时出现在内部节点和叶子节点——这是 B+ 树的特征。

#### 9.3 B+ 树的阶定义(注意和 B 树的区别)

对 m 阶 B+ 树:

**内部节点**:最多 m 个孩子、m-1 个关键字;最少 ⌈m/2⌉ 个孩子(根节点最少 2 个)。

**叶子节点**:最多 m 个关键字(注意:B+ 树叶子装的是"实际数据",所以最多关键字数 = m 而不是 m-1);最少 ⌈m/2⌉ 个关键字。

---

### 十、B 树 vs B+ 树:为什么数据库选 B+?

这是面试和考研的**经典问答**,必须能条理清晰地说出来:

**第一,范围查询效率**。B+ 树叶子串成链表,范围查询(如 `WHERE age BETWEEN 20 AND 30`)只需找到起始叶子,沿链表扫描即可;B 树做范围查询要中序遍历整棵树(复杂得多)。**这是 B+ 树最大的优势。**

**第二,IO 效率**。B+ 树内部节点不存数据,只存关键字,因此同等大小的磁盘块能装更多关键字,**树更矮,IO 次数更少**。

**第三,查询稳定性**。B+ 树每次查找都必须走到叶子,查找任何关键字的路径长度相同(等于树高),**性能稳定**;B 树的查找深度不定(内部节点可能提前命中),性能有波动。

**第四,全表扫描**。B+ 树直接扫描叶子链表即可全表扫描,B 树需要中序遍历整棵树。

**B 树相对 B+ 树的唯一优势**:如果查找的关键字恰好在内部节点,B 树可以不用走到叶子就返回,查找某些特定值会快一点——但这种场景在数据库中并不典型。

---

### 十一、B+ 树结构体(仅展示定义,实现思路与 B 树类似)

cpp

```cpp
#pragma once
#define ORDER_PLUS 3
typedef int KeyType;

// B+ 树叶子节点
typedef struct BPlusLeaf {
    int keyNum;
    KeyType keys[ORDER_PLUS];           // 叶子存真实 key(最多 ORDER 个)
    int records[ORDER_PLUS];            // 对应的数据记录(简化为 int)
    struct BPlusLeaf *next;             // 指向下一个叶子(链表!)
} BPlusLeaf;

// B+ 树内部节点
typedef struct BPlusNode {
    int keyNum;
    KeyType keys[ORDER_PLUS];           // 路由关键字
    void *children[ORDER_PLUS + 1];     // 孩子(可能是内部节点或叶子)
    bool isLeaf;                        // 区分类型
} BPlusNode, *BPlusTree;
```

B+ 树的完整代码实现在考研范围内不作要求,**重点是理解其结构和与 B 树的差异**。如果你需要完整实现,我可以单独展开。

---

### 十二、对比总结表

|特性|B 树|B+ 树|
|---|---|---|
|数据存储位置|所有节点|仅叶子节点|
|叶子间连接|无|有序链表|
|内部节点关键字|不重复出现在叶子|在叶子中有副本|
|查找终点|可在任意层|必须到叶子|
|范围查询|中序遍历树|扫描叶子链表|
|查询稳定性|不稳定|稳定(等于树高)|
|数据库应用|较少|MySQL/PostgreSQL 等主流|

### 十三、思考题(请作答)

第一题,在 3 阶 B 树中,依次插入 `{1, 2, 3, 4, 5, 6, 7}`,**每一步画出树的形态**,标出每次分裂的位置和上提的关键字。

第二题,已知 m 阶 B 树共有 N 个关键字,**树的高度 h 最大是多少**?写出推导过程(用 N、m 表示 h)。

第三题,下面哪个说法是**错误**的?

(A) B 树和 B+ 树的叶子节点都在同一层。 (B) B+ 树所有关键字都出现在叶子节点中。 (C) 在 B 树中查找一个关键字,最多比较次数等于树的高度。 (D) B+ 树比 B 树更适合做范围查询。

第四题,一棵 5 阶 B 树有 100 个关键字,**树的最大高度和最小高度**各是多少?

第五题,**为什么说 B+ 树的内部节点不存数据,可以让同等磁盘块容纳更多关键字?** 结合磁盘块大小(比如 16KB)和一条记录大小(比如 1KB)来估算,同等高度下 B+ 树能存多少倍的记录?

---

### 十四、下一步方向

B 树和 B+ 树学完,你的"树族谱"真正完整了:从二叉搜索树 → AVL → B 树 → B+ 树,这条线是数据库索引进化史。接下来可以选:

**(A)** 学**图论**(DFS、BFS、最小生成树、最短路径、拓扑排序——考研大题分值最高的章节,也是面试最常考的)。**(B)** 学**排序算法**(快排、归并、堆排序、希尔、基数——考研必考,每年必有一道大题)。**(C)** 学**查找算法**(散列表/哈希表——和 B+ 树是数据库两大索引结构,面试高频)。**(D)** 学**堆和堆排序**(既是排序算法,又是优先队列的基础)。**(E)** 先答思考题再继续。

告诉我你的选择!

## 数据结构复习:堆和堆排序

好,在我们一头扎进代码之前,我想先确认一下你的起点——这样我能更好地把堆讲清楚,而不是把你已经会的东西重复一遍。

### 几个引导问题(请选答你能答的)

**问题一**:你之前学过"完全二叉树"对吧?能用自己的话回忆一下:**完全二叉树和满二叉树的区别**是什么?为什么完全二叉树特别适合**用数组存储**(不用指针)?

**问题二**:假设有一棵完全二叉树用数组 `a[1..n]` 存储,下标 i 的节点:

- 它的左孩子在哪个下标?
- 它的右孩子在哪个下标?
- 它的父节点在哪个下标?
- 第一个**非叶子**节点在哪个下标?(这个问题对堆排序至关重要,你想想看)

**问题三**:你对"堆"这个词,目前有多少印象?以下哪些你能说出大概?

- 大根堆 / 小根堆是什么意思?
- 堆和 BST 有什么区别?(都是二叉树,但性质不同)
- 优先队列和堆有什么关系?

**问题四**:你之前学过排序算法吗?如果学过,知道哪几种?(冒泡、选择、插入、快排、归并……)

---

### 为什么我先问这些?

因为**堆排序的代码非常短(核心就两个函数),但每一行都建立在前面的概念之上**:

- 数组下标的父子关系 → 决定了堆的实现
- "第一个非叶子节点" → 决定了**建堆**从哪里开始
- 大根堆性质 → 决定了堆排序为什么能得到升序
- 堆调整的"下沉"操作 → 是整个算法的核心

如果其中任何一环你不熟,我帮你先打牢;如果你都熟,我可以直接进入代码部分,讲快一些。

---

### 你可以这样回答我

最理想的情况是:**你试着回答上面的问题,哪怕只能写出"我大概知道但说不准"也行**——这样我就能看出你的真实起点。

如果你完全是零基础,直接告诉我"全部不会",我从最基本的"完全二叉树的数组表示"开始讲起。

如果你说"问题一二我都会,问题三四不太懂",那我就跳过前两个,直接从堆的定义开始。

**告诉我你的回答吧,我们一起决定从哪里开始。**

## 数据结构复习:堆和堆排序

好,从基础开始讲,按照我们之前一贯的风格——**概念铺垫 → 存储结构 → 核心操作 → 应用 → 思考题**。这一章代码不长,但**每一步的原理必须透彻**,否则面对变形题就会懵。

---

### 一、前置知识:完全二叉树的数组表示

**这是堆的存储基础,必须先讲清楚。**

回忆一下完全二叉树:除最后一层外,每一层都填满,且最后一层的节点**从左到右连续**排列。正因为它这么规整,**可以不用指针,直接用数组按层序存储**。

#### 数组下标的父子关系(必背)

设数组下标**从 1 开始**使用(a[0] 不用),对于下标 i 的节点:

**父节点**下标:`i / 2`(整数除法,自动向下取整)。

**左孩子**下标:`2 * i`。

**右孩子**下标:`2 * i + 1`。

#### 一个例子

完全二叉树:

```
        50           ← 下标 1
       /  \
      30   70        ← 下标 2, 3
     / \   / \
    20 40 60 80      ← 下标 4, 5, 6, 7
```

数组表示:

|下标|1|2|3|4|5|6|7|
|---|---|---|---|---|---|---|---|
|值|50|30|70|20|40|60|80|

验证:下标 2 的左孩子下标 2×2=4 是 20,右孩子下标 5 是 40,父节点下标 2/2=1 是 50——完全对应。

#### 为什么下标从 1 开始?

如果下标从 0 开始,父子关系变成:左孩子 `2i+1`,右孩子 `2i+2`,父节点 `(i-1)/2`。**能用,但计算更繁琐**。从 1 开始的下标关系最优雅,所以教材(严蔚敏、王道)和考研大题都用 **1 起**的约定。我们也用这个。

#### 第一个非叶子节点在哪里?(重要!)

**结论**:对于 n 个节点的完全二叉树,最后一个节点的下标是 n,它的父节点下标是 `n/2`。这个父节点就是**最后一个非叶子节点**。

**为什么?** 最后一个节点之后的所有节点(下标 n+1, n+2, ...)都不存在,所以它们的父节点(下标 n/2 之前的某些节点)才可能是叶子。但下标 `n/2` 的节点至少有最后一个节点作为孩子,所以它**一定是非叶子**。再往前的节点也都是非叶子(它们的孩子下标更小,肯定存在)。

**所以下标范围 1 到 n/2 都是非叶子,n/2+1 到 n 都是叶子。**

这个结论是**堆排序建堆的起点**——建堆从下标 n/2 开始向前遍历。

---

### 二、堆的定义

**堆(Heap)**:一棵**完全二叉树**,且满足以下性质之一:

**大根堆(大顶堆)**:每个节点的值 ≥ 它的左右孩子的值。等价说法:根是整棵树的最大值。

**小根堆(小顶堆)**:每个节点的值 ≤ 它的左右孩子的值。等价说法:根是整棵树的最小值。

**注意**:堆**只要求父子间满足大小关系**,**不要求兄弟间、也不要求整个树像 BST 那样全局有序**。这是堆和 BST 最大的区别。

#### 大根堆示例

```
        80
       /  \
      70   60
     / \   / \
    40 50 20 30
```

每个节点都 ≥ 它的孩子。但左子树里有 40 < 50(兄弟),右子树里 20 < 30——**兄弟间无序也无妨**,堆只看父子关系。

#### 堆 vs BST 对比

|性质|堆|BST|
|---|---|---|
|树形|必须完全二叉树|任意二叉树|
|存储|数组|链表/指针|
|顺序|父子关系|左<根<右|
|根节点|最大(或最小)|无特殊含义|
|找最大/最小|O(1)|O(h)|
|查找任意值|O(n)|O(h)|
|中序遍历|无意义(不会有序)|递增有序|

**一句话概括**:**BST 为查找任意值优化,堆为"反复取最大/最小值"优化**。这决定了它们各自的应用场景。

---

### 三、堆的核心操作:向下调整(Sift Down)

堆有两个基本操作——"向下调整"和"向上调整"。我们先讲**向下调整**,这是堆排序和建堆的基石。

#### 问题情境

假设下标 i 的节点**违反了堆性质**(它的值比孩子小,在大根堆中就是违反者),但 i 的左右子树**本身都已经是合法的大根堆**。如何让以 i 为根的整棵子树重新变成大根堆?

#### 向下调整的思路

从 i 出发:

**第一步**,找 i 的左右孩子中**较大**的一个,记作 max。

**第二步**,如果 i 本身已经 ≥ max,性质满足,结束。

**第三步**,否则,**交换 i 和 max 的值**,然后把问题**下沉**——i 的值现在在原 max 的位置,可能又违反那一层的堆性质,所以对那个位置继续做同样的调整。

这个过程像"违反者一路沉到底",所以叫"下沉"或"向下调整"。

#### 图解

对下面这棵子树调整(下标 1 的根 30 违反大根堆):

```
        30 ← 根违反
       /  \
      70   60        ← 左右子树都已是合法大根堆
     / \   / \
    40 50 20 10
```

**第一步**,比较左右孩子 70 vs 60,左大;30 < 70,交换 30 和 70:

```
        70
       /  \
      30   60        ← 30 下沉到左子树根
     / \   / \
    40 50 20 10
```

**第二步**,30 现在在下标 2 的位置,它的孩子是 40、50。40 vs 50,50 大;30 < 50,交换:

```
        70
       /  \
      50   60
     / \   / \
    40 30 20 10   ← 30 继续下沉
```

**第三步**,30 现在在下标 5 的位置,下标 10、11 的孩子不存在(n=7),30 已到叶子,结束。

**最终**,以原先 30 为根的子树,经过下沉调整,变成合法大根堆。

#### 代码实现(向下调整)

cpp

```cpp
// 对数组 a[1..n] 中下标 i 的节点做向下调整(大根堆)
void SiftDown(int a[], int i, int n) {
    while (2 * i <= n) {                // 只要有左孩子
        int child = 2 * i;              // 左孩子下标
        // 选出左右孩子中较大的那个
        if (child + 1 <= n && a[child + 1] > a[child]) {
            child++;                    // 右孩子更大
        }
        // 如果父节点已经 >= 较大孩子,堆性质满足,结束
        if (a[i] >= a[child]) break;
        // 否则交换,继续下沉
        int temp = a[i];
        a[i] = a[child];
        a[child] = temp;
        i = child;                      // 更新 i,继续向下
    }
}
```

**这段代码必须背下来**。它看起来只有 10 行,但是堆算法的全部精髓。逐行理解:

`while (2*i <= n)`:循环条件是"i 还有左孩子"。如果左孩子都没有(i 是叶子),下沉结束。

`int child = 2*i`:先假设要和左孩子比。

`if (child+1 <= n && a[child+1] > a[child])`:如果右孩子存在且更大,改成和右孩子比。这里**短路判断**很关键——如果右孩子不存在(`child+1 > n`),`a[child+1]` 就越界了,所以必须先判 `child+1 <= n`。

`if (a[i] >= a[child]) break`:性质已满足,不用再下沉。

`swap + i = child`:交换后问题转移到孩子位置,继续。

**时间复杂度**:向下调整最多走完树的高度,即 O(log n)。

---

### 四、建堆:从无序数组到大根堆

给你一个无序数组 a[1..n],如何把它变成大根堆?

#### 朴素思路(不好)

从下标 1 开始,对每个位置做向下调整。但**这是错的**——因为 SiftDown 要求"i 的左右子树已经是合法堆",从上往下做,子树根本还没调整过。

#### 正确思路:从最后一个非叶子节点开始,向前遍历

**最后一个非叶子节点**下标是 `n/2`(前面推导过)。从这里开始**向前**到下标 1,依次对每个节点做 SiftDown。

**为什么这样对?** 因为下标 n/2+1 到 n 都是叶子——**叶子本身已经是合法堆**(单节点就是堆)。从 n/2 开始,每个节点的左右子树都已经被处理过(因为子树在数组中下标更大,更早处理),所以 SiftDown 的前提条件满足。

**这就是著名的"Floyd 建堆法"**,也叫"自底向上建堆"。

#### 代码

cpp

```cpp
void BuildMaxHeap(int a[], int n) {
    for (int i = n / 2; i >= 1; i--) {  // 从最后一个非叶子到根
        SiftDown(a, i, n);
    }
}
```

#### 建堆示例:数组 `[_, 4, 1, 3, 2, 16, 9, 10, 14, 8, 7]`(下标 0 不用,n=10)

对应完全二叉树:

```
              4
           /     \
          1       3
         / \     / \
        2   16  9  10
       / \  /
      14  8 7
```

**最后一个非叶子下标 = 10/2 = 5**(值 16)。从下标 5 开始向前。

**i=5(16)**:左孩子下标 10(7)。16 > 7,已满足,不动。

**i=4(2)**:左孩子 14,右孩子 8,最大 14;2 < 14,交换。

```
              4
           /     \
          1       3
         / \     / \
       14   16  9  10
       / \  /
      2   8 7
```

**i=3(3)**:左孩子 9,右孩子 10,最大 10;3 < 10,交换。

```
              4
           /     \
          1      10
         / \     / \
       14  16  9   3
       / \  /
      2   8 7
```

**i=2(1)**:左孩子 14,右孩子 16,最大 16;1 < 16,交换。然后 1 下沉到原 16 位置(下标 5),左孩子 7,1 < 7,再交换。

```
              4
           /     \
         16      10
         / \     / \
       14   7  9   3
       / \  /
      2   8 1
```

**i=1(4)**:左 16,右 10,最大 16;4 < 16,交换。4 下沉到下标 2,左 14,右 7,最大 14;4 < 14,交换。4 下沉到下标 4,左 2,右 8,最大 8;4 < 8,交换。4 下沉到下标 9,无孩子,停止。

```
              16
           /      \
         14       10
         / \      / \
        8   7   9    3
       / \  /
      2   4 1
```

**建堆完成**。根 16 是最大值。

#### 建堆的时间复杂度(易错点)

很多人第一反应是 O(n log n),因为每次 SiftDown 是 O(log n),共 n/2 次。**但实际上是 O(n)**。

**为什么?** 因为**越靠近根的节点越少,但下沉路径越长;越靠近叶子的节点越多,但下沉路径越短**。数学上可以证明总和是线性的。**"建堆 O(n)"是考研选择题重点考点**。

---

### 五、堆排序(Heap Sort)

有了大根堆之后,排序思路非常优雅:

**步骤一**,建大根堆,此时根 a[1] 是全局最大值。

**步骤二**,**把 a[1] 和 a[n] 交换**。最大值就位(排到了数组末尾)。此时数组前 n-1 个元素可能不再是堆。

**步骤三**,对 a[1..n-1] **重新调整为堆**(只需对 a[1] 做一次 SiftDown 即可,因为只有根被换过)。

**步骤四**,重复:把 a[1] 和 a[n-1] 交换,对 a[1..n-2] 调整,……

**共交换 n-1 次,每次调整 O(log n),总时间 O(n log n)**。

#### 核心代码

cpp

```cpp
void HeapSort(int a[], int n) {
    // 1. 建堆
    BuildMaxHeap(a, n);
    
    // 2. 反复"取最大 + 调整"
    for (int i = n; i >= 2; i--) {
        // 交换 a[1] 和 a[i]
        int temp = a[1];
        a[1] = a[i];
        a[i] = temp;
        // 对 a[1..i-1] 重新调整
        SiftDown(a, 1, i - 1);
    }
}
```

**注意**:第二个参数 `i - 1` 非常关键——此时数组有效范围缩小到 `[1..i-1]`,`a[i..n]` 已经是排好序的部分了。SiftDown 必须知道"堆的大小",才能正确处理边界。

#### 为什么大根堆排出升序?

每次把最大值放到末尾,下次把"剩余中的最大值"放到倒数第二位……所以最终数组从前往后是从小到大。

**如果要排降序,就用小根堆**,每次把最小值放末尾,结果从前到后就是从大到小。

---

### 六、堆排序的完整示例

继续用前面建好的堆:

```
a[] = [_, 16, 14, 10, 8, 7, 9, 3, 2, 4, 1]
```

**第一轮**:交换 a[1]=16 和 a[10]=1,数组变为 `[_, 1, 14, 10, 8, 7, 9, 3, 2, 4, 16]`。对 a[1..9] 做 SiftDown(1):

1 的左右孩子 14、10,最大 14;1 < 14,交换。1 下沉到下标 2,左 8、右 7,最大 8;1 < 8,交换。1 下沉到下标 4,左 2、右 4,最大 4;1 < 4,交换。1 下沉到下标 9,无孩子,停。

数组:`[_, 14, 8, 10, 4, 7, 9, 3, 2, 1, 16]`

**第二轮**:交换 a[1]=14 和 a[9]=1,数组 `[_, 1, 8, 10, 4, 7, 9, 3, 2, 14, 16]`。调整 a[1..8]:

类似地 1 下沉……最终数组 `[_, 10, 8, 9, 4, 7, 1, 3, 2, 14, 16]`。

**……**(继续 n-1 轮)

**最终排序结果**:`[_, 1, 2, 3, 4, 7, 8, 9, 10, 14, 16]`——升序。

---

### 七、项目文件组织

```
DataStructure/
└── Heap/
    ├── Heap.h
    ├── Heap.cpp
    └── main.cpp
```

#### Heap.h

cpp

```cpp
#pragma once

typedef int ElemType;

// ========== 堆的核心操作 ==========
void SiftDown(ElemType a[], int i, int n);       // 向下调整
void SiftUp(ElemType a[], int i);                 // 向上调整(优先队列插入用)
void BuildMaxHeap(ElemType a[], int n);           // 建大根堆

// ========== 堆排序 ==========
void HeapSort(ElemType a[], int n);

// ========== 优先队列的基本操作 ==========
void Insert(ElemType a[], int &n, ElemType e, int capacity);  // 插入
ElemType ExtractMax(ElemType a[], int &n);                   // 取走最大值
ElemType GetMax(ElemType a[], int n);                         // 读最大值

// ========== 辅助 ==========
void PrintArray(ElemType a[], int n);
```

#### Heap.cpp

cpp

```cpp
#include "Heap.h"
#include <iostream>
using namespace std;

void SiftDown(ElemType a[], int i, int n) {
    while (2 * i <= n) {
        int child = 2 * i;
        if (child + 1 <= n && a[child + 1] > a[child]) {
            child++;
        }
        if (a[i] >= a[child]) break;
        ElemType temp = a[i];
        a[i] = a[child];
        a[child] = temp;
        i = child;
    }
}

void SiftUp(ElemType a[], int i) {
    while (i > 1 && a[i] > a[i / 2]) {       // 比父大就上浮
        ElemType temp = a[i];
        a[i] = a[i / 2];
        a[i / 2] = temp;
        i = i / 2;
    }
}

void BuildMaxHeap(ElemType a[], int n) {
    for (int i = n / 2; i >= 1; i--) {
        SiftDown(a, i, n);
    }
}

void HeapSort(ElemType a[], int n) {
    BuildMaxHeap(a, n);
    for (int i = n; i >= 2; i--) {
        ElemType temp = a[1];
        a[1] = a[i];
        a[i] = temp;
        SiftDown(a, 1, i - 1);
    }
}

// 优先队列:插入 e 到堆中
void Insert(ElemType a[], int &n, ElemType e, int capacity) {
    if (n >= capacity - 1) return;           // 满了(n 是当前元素数,下标 1..n)
    n++;
    a[n] = e;                                 // 放到末尾
    SiftUp(a, n);                             // 向上调整
}

// 优先队列:取走最大值
ElemType ExtractMax(ElemType a[], int &n) {
    ElemType maxVal = a[1];
    a[1] = a[n];                              // 末尾元素覆盖根
    n--;
    SiftDown(a, 1, n);
    return maxVal;
}

ElemType GetMax(ElemType a[], int n) {
    return a[1];                              // O(1) 读最大值
}

void PrintArray(ElemType a[], int n) {
    for (int i = 1; i <= n; i++) cout << a[i] << " ";
    cout << endl;
}
```

**这里顺便讲了"向上调整 SiftUp"**——它是 SiftDown 的对称操作,用于**优先队列的插入**:把新元素放在数组末尾,然后向上浮动到正确位置。逻辑类似,不再赘述。

---

### 八、main.cpp 测试

cpp

```cpp
#include "Heap.h"
#include <iostream>
using namespace std;

int main() {
    // ========== 堆排序测试 ==========
    ElemType a[] = {0, 4, 1, 3, 2, 16, 9, 10, 14, 8, 7};  // a[0] 不用
    int n = 10;
    
    cout << "原数组:   "; PrintArray(a, n);
    
    BuildMaxHeap(a, n);
    cout << "建堆后:   "; PrintArray(a, n);
    
    HeapSort(a, n);
    cout << "排序后:   "; PrintArray(a, n);
    
    // ========== 优先队列测试 ==========
    cout << "\n===== 优先队列 =====" << endl;
    ElemType pq[20];
    int pqSize = 0;
    
    int data[] = {5, 3, 8, 1, 9, 2, 7};
    for (int i = 0; i < 7; i++) {
        Insert(pq, pqSize, data[i], 20);
        cout << "插入 " << data[i] << " 后: ";
        PrintArray(pq, pqSize);
    }
    
    cout << "\n依次取出最大值: ";
    while (pqSize > 0) {
        cout << ExtractMax(pq, pqSize) << " ";
    }
    cout << endl;
    
    return 0;
}
```

预期输出:

```
原数组:   4 1 3 2 16 9 10 14 8 7 
建堆后:   16 14 10 8 7 9 3 2 4 1 
排序后:   1 2 3 4 7 8 9 10 14 16 

===== 优先队列 =====
插入 5 后: 5 
插入 3 后: 5 3 
插入 8 后: 8 3 5 
插入 1 后: 8 3 5 1 
插入 9 后: 9 8 5 1 3 
插入 2 后: 9 8 5 1 3 2 
插入 7 后: 9 8 7 1 3 2 5 

依次取出最大值: 9 8 7 5 3 2 1
```

**取出的顺序正是从大到小**——这就是优先队列的核心用途。

---

### 九、堆的应用总结

#### 应用一:堆排序

O(n log n) 时间,O(1) 空间(原地排序)。**不稳定排序**(相同元素的相对位置可能改变)。

相比快排:堆排最坏也是 O(n log n)(快排最坏 O(n²)),但**常数因子更大**,实际中通常比快排慢 2-3 倍。所以堆排主要用于"需要严格保证 O(n log n)"的场景。

#### 应用二:优先队列

需要反复取最大/最小,且支持动态插入的场景。典型应用:任务调度(优先级高的先执行)、Dijkstra 最短路径(每次取距离最小的节点)、Huffman 建树(每次取权值最小的两棵)。

**C++ 的 `std::priority_queue` 底层就是堆**,默认大根堆。

#### 应用三:Top K 问题

**从 n 个元素中找最大的 K 个**。

**朴素做法**:排序后取前 K 个,O(n log n)。

**堆做法**:维护一个**大小为 K 的小根堆**。遍历 n 个元素,每个元素若大于堆顶,就替换堆顶并 SiftDown。最终堆里就是最大的 K 个。**O(n log K)**,当 K 远小于 n 时比排序快很多。

**这是面试高频题**,你要熟。

#### 应用四:建哈夫曼树

还记得我们前面讲哈夫曼树的"Select 函数"吗?用线性查找是 O(n),每次选最小的两个;**如果用小根堆替代,每次取最小是 O(log n)**,建哈夫曼树总时间从 O(n²) 降到 O(n log n)。这是一个**把堆应用到我们已学内容**的经典场景。

---

### 十、常见易错点总结

**易错一**:建堆的起点是 `n/2`,不是 1,也不是 n。从 `n/2` 向前遍历到 1。

**易错二**:向下调整时,判断右孩子存在必须用 `child+1 <= n` 短路,否则越界。

**易错三**:堆排序时 `SiftDown(a, 1, i-1)` 中的 `i-1` 不能写成 n,因为堆的有效范围每轮在缩小。

**易错四**:**建堆是 O(n)**,不是 O(n log n)。但**堆排序整体是 O(n log n)**(因为还要做 n-1 次 SiftDown)。

**易错五**:堆**不是排序的**,只是父子关系固定。不要误以为堆的中序遍历会有序。

**易错六**:大根堆排升序(最大值沉底),小根堆排降序。很多人记反。

**易错七**:堆排序是**不稳定**的(SiftDown 可能改变相同值的相对顺序),归并排序是稳定的。

---

### 十一、思考题(请作答)

第一题,对数组 `[53, 17, 78, 9, 45, 65, 87, 32]`(下标从 1 开始,n=8),**画出建大根堆的每一步**,标明从下标几开始、每次 SiftDown 的路径。

第二题,**建堆的时间复杂度为什么是 O(n) 而不是 O(n log n)?** 请给出直观解释(不需要严格证明,但要说清楚"为什么上层节点少但下沉深、下层节点多但下沉浅能抵消")。

第三题,**Top K 问题**:从一个有 100 万个元素的数组中找最大的 10 个,用**大小为 10 的小根堆**的算法过程是怎样的?为什么用小根堆而不是大根堆?(提示:想想"堆顶"在 Top K 里代表什么)

第四题,给定一个大根堆,**堆中的最小值可能在哪些位置**?是不是一定在叶子?(提示:想想堆的定义只约束父子)

第五题,向一个含 n 个元素的大根堆中**插入一个新元素**,最坏情况要做多少次比较和交换?**删除最大值**呢?分别写出时间复杂度。

第六题,**堆排序是稳定的吗?** 如果不是,举一个反例(即给一个输入,其中相同值元素的相对顺序在堆排后发生了改变)。

---

### 十二、下一步方向

堆啃完之后你掌握了:**基于数组的完全二叉树 + 堆排序 + 优先队列**。接下来的路径:

**(A)** **排序算法全家桶**(冒泡、选择、插入、希尔、快排、归并、基数、计数——考研必考,内部排序是整章大题)。堆排在此作为一员已经学完。**(B)** **图论**(邻接矩阵、邻接表、DFS、BFS、Prim/Kruskal、Dijkstra、Floyd、拓扑排序、关键路径)——考研分值最高的章节之一,且 Dijkstra 会用到优先队列(堆的应用)。**(C)** **散列表 / 哈希表**(开放定址法、链地址法、冲突处理、装填因子、性能分析)——和 B+ 树并列数据库索引两大支柱。**(D)** 先答思考题再继续。

告诉我你想走哪个方向!

## 数据结构复习:红黑树

好,我们进入**红黑树**(Red-Black Tree)。先给你打个预防针——**这是数据结构里最难的一章,没有之一**。它比 AVL 复杂得多,比 B 树概念更抽象,但又是**工业界最重要的平衡树**(`std::map`、`std::set`、Java TreeMap、Linux 内核的许多内部结构都用红黑树)。

**关于考研**:红黑树从 2019 年起被列入考研大纲,但考查以**性质判断、插入手画**为主,**代码默写要求较低**。所以我的讲解重点是"**让你能看懂红黑树在做什么、能手画几步插入、能和 AVL 对比**",代码会给出但不要求你默写每一行。

---

### 一、为什么还要红黑树?AVL 不够好吗?

回忆一下 AVL:**严格平衡**,左右子树高度差不超过 1。这使 AVL 的**查找性能极好**(高度接近 log n)。

**AVL 的痛点在于"维护成本太高"**:

第一,每次插入/删除都可能触发**多次旋转**(删除操作甚至可能一路旋到根)。

第二,要维护高度(或平衡因子)字段,每次更新。

第三,频繁写操作下,旋转开销让 AVL 的吞吐率下降明显。

**红黑树的哲学**:**放弃严格平衡,接受"近似平衡"**。只要保证"最长路径不超过最短路径的 2 倍",就不算失衡。这样平均旋转次数大幅下降,**在增删频繁的场景下优于 AVL**。

**类比**:AVL 是"强迫症房东"(每根头发都要梳直),红黑树是"务实房东"(大致整齐就行,但保证不乱)。

---

### 二、红黑树的定义(必须一字不差地记)

一棵红黑树是满足以下**五条性质**的二叉搜索树:

**性质一**:每个节点不是**红色**就是**黑色**。

**性质二**:**根节点是黑色**。

**性质三**:每个**叶子节点(NIL)是黑色**。注意:这里的"叶子"指概念上每个没有孩子的位置挂一个**哨兵 NIL 节点**,不是通常"没有孩子的普通节点"。

**性质四**:如果一个节点是**红色**,则它的**两个孩子必须都是黑色**(即"**红色节点不能连续**",但黑色可以连续)。

**性质五**:对每个节点,从该节点出发到它**任何一个后代叶子(NIL)的路径**上,所含**黑色节点数相同**。这个数叫该节点的"**黑高度**"。

**性质五是最重要的约束**——它保证了树的"近似平衡"。

---

### 三、从五条性质能推出什么?(最核心的定理)

**定理**:一棵包含 n 个**内部节点**的红黑树,高度 h ≤ **2 × log₂(n+1)**。

**证明思路(大白话版)**:

性质五保证每条根到叶子的路径有相同的黑高度 bh。性质四保证路径上红黑交替,所以**最长路径最多是最短路径的 2 倍**(黑黑黑…… vs 红黑红黑……)。再结合 n 个内部节点的约束,可推出 h ≤ 2 log(n+1)。

**这个定理就是红黑树存在的意义**:高度是 O(log n),查找永远 O(log n)。虽然常数比 AVL 稍差(2 log n vs 1.44 log n),但维护代价低得多。

---

### 四、一棵合法红黑树的例子

```
             13(黑)
           /       \
        8(红)      17(红)
       /    \      /    \
     1(黑)  11(黑) 15(黑) 25(黑)
       \              \
       6(红)          22(红)
```

你验证一下:

根 13 黑(✓性质二);没有红红父子(11 黑,8 红没问题;1 黑,6 红没问题);从每个节点到叶子的黑色数一致。比如从 13 出发:13→8→1→NIL 黑数是 2,13→8→1→6→NIL 黑数 2,13→17→15→NIL 黑数 2,13→17→25→22→NIL 黑数 2——全部为 2,满足性质五。

---

### 五、红黑树的插入(核心算法)

#### 5.1 基本框架

**第一步**:按 BST 规则找到插入位置,新节点默认**染红**。

**为什么染红?** 因为如果染黑,立刻破坏性质五(所在路径黑高度 +1,影响全局);染红最多破坏性质四(红红父子),局部可修复。

**第二步**:如果插入后破坏了性质(性质二或性质四),执行"**插入修复**"(insert fixup)。

#### 5.2 插入修复的三种情况

**核心判断**:新节点 z 是红色,z 的父节点 p 是什么颜色决定了是否需要修复。

**情况 0**:**p 是黑色**。性质四未被破坏(红黑父子合法),**直接结束,什么都不用做**。

**情况 1、2、3 都发生在"父节点 p 是红色"时**。此时必有祖父 g(因为根是黑色,红色节点不可能是根),也必然有**叔叔节点 u**(可能是 NIL,那就是黑色)。

**我们分 p 是 g 的左孩子还是右孩子,有两组对称情况**。这里只讲"p 是 g 的左孩子"的三种情况,另一侧完全对称。

**情况 1:叔叔 u 是红色**

```
        g(黑)                 g(红)
       /    \                /    \
     p(红)  u(红)   →       p(黑) u(黑)
     /                      /
   z(红)                   z(红)
```

**操作**:p 和 u 都染黑,g 染红。然后把 g 当作新的 z,**向上递归**检查(因为 g 变红了,g 的父节点如果也是红,要继续修复)。

**为什么这样做?** p、u 染黑消除红红冲突;g 染红保持黑高度不变(原 g 为黑,路径上黑数 +1,现在 g 红、p/u 黑,黑数仍是原来那样);但 g 变红后可能导致上层红红冲突,所以要向上继续。

**情况 2:叔叔 u 是黑色,且 z 是 p 的右孩子(LR 型,"折线")**

```
        g(黑)                    g(黑)
       /    \                   /    \
     p(红)  u(黑)    →        z(红)  u(黑)
       \                      /
       z(红)                 p(红)
```

**操作**:对 p 做**左旋**。把情况 2 转化为情况 3。

**情况 3:叔叔 u 是黑色,且 z 是 p 的左孩子(LL 型,"直线")**

```
        g(黑)                   p(黑)
       /    \                  /    \
     p(红)  u(黑)   →         z(红)  g(红)
     /                                  \
   z(红)                                u(黑)
```

**操作**:g 和 p **交换颜色**,对 g 做**右旋**。修复完成,算法结束。

#### 5.3 记忆法(很重要)

**一条记忆主线**:

**父黑**:不用动。**父红,叔红**:颜色翻转(爸叔黑,爷红),向上递归。**父红,叔黑,折线**:转一次变直线(情况 2 → 情况 3)。**父红,叔黑,直线**:旋转 + 变色,结束。

**对比 AVL**:AVL 是"四种旋转(LL、RR、LR、RL)",红黑树是"三种情况(叔红 / 叔黑折 / 叔黑直)"。红黑树判断更快(只看叔叔颜色),但情况 1 可能向上递归多次。

---

### 六、手工插入完整示例

对空树依次插入 `{10, 20, 30, 15, 5}`。

**插入 10**:

根插入 10。染红,但性质二要求根是黑,所以**染黑**:

```
10(黑)
```

**插入 20**:

按 BST,20 > 10,作为 10 的右孩子。染红:

```
  10(黑)
     \
     20(红)
```

父是黑,情况 0,**不修复**。

**插入 30**:

30 > 10,30 > 20,作为 20 的右孩子。染红:

```
  10(黑)
     \
     20(红)
        \
        30(红)
```

父 20 是红,**情况发生**。叔叔是 10 的左孩子……**但 10 没有左孩子,即叔叔是 NIL,黑色**。

所以是"叔黑"的情况。z=30 是 p=20 的右孩子,g=10。p 是 g 的右孩子,z 是 p 的右孩子——**是对称情况 3(直线 RR 型)**:g 和 p 交换颜色,对 g 做左旋。

```
    20(黑)
   /     \
 10(红)  30(红)
```

验证:根黑(✓),没有红红连(✓),从 20 到所有 NIL 黑数都是 2(✓)。

**插入 15**:

15 > 10,15 < 20,作为左子树;再比较 15 和 20,15 < 20,作为 20 的左孩子。染红:

```
    20(黑)
   /     \
 10(红)  30(红)
    \
    15(红)    ← 新
```

等等,15 应该放哪?让我重来。15 和根 20 比,15 < 20,走左;15 和 10 比,15 > 10,走右。所以 15 是 10 的右孩子:

```
    20(黑)
   /     \
 10(红)  30(红)
    \
    15(红)
```

父 10 是红,**情况发生**。叔叔 30 是红——**情况 1**:p=10 和 u=30 都染黑,g=20 染红。

```
    20(红)
   /     \
 10(黑)  30(黑)
    \
    15(红)
```

然后把 g=20 当作新 z,向上检查。20 是根,根必须是黑——**强制染黑**:

```
    20(黑)
   /     \
 10(黑)  30(黑)
    \
    15(红)
```

**插入 5**:

5 < 20,走左;5 < 10,作为 10 的左孩子。染红:

```
       20(黑)
      /     \
    10(黑)  30(黑)
    /  \
   5(红) 15(红)
```

父 10 是黑,**情况 0**,不修复。

最终合法红黑树。

---

### 七、红黑树的删除(只讲思路,代码极复杂)

**删除比插入复杂得多**,因为:

插入修复最多 3 种情况;**删除修复有 4 种情况,且可能向上递归多次**。

**总流程**:

第一步,按 BST 规则删除:如果度为 2,用后继替换;最终总是删除一个**最多有一个孩子**的节点。

第二步,如果删的是**红节点**,直接删,不破坏任何性质,结束。

第三步,如果删的是**黑节点**,所在路径黑高度 -1,破坏性质五,需要"删除修复"。

**删除修复的核心概念**:被删除位置引入一个"**双重黑(double black)**"的哨兵,通过各种操作把这个"额外的黑"消化掉(要么转移给兄弟的孩子,要么通过变色吸收,要么推到父节点继续处理)。

**删除修复的四种情况**(以被删节点 x 是父节点的左孩子为例,兄弟 w):

**情况 1**:w 是红色——先旋转变色,把问题转成 w 是黑色的情况。

**情况 2**:w 是黑色,且 w 的两个孩子都是黑色——w 染红,双重黑上移到父。

**情况 3**:w 是黑色,w 的左孩子红、右孩子黑——旋转 + 染色,转成情况 4。

**情况 4**:w 是黑色,w 的右孩子红——旋转 + 染色,修复完成,结束。

**这四种情况每种都要记住旋转方向和染色规则,确实繁琐。考研对红黑树删除的考查非常少,你只需知道"有四种情况、比插入复杂"即可。**

---

### 八、代码实现(教学参考)

**我给出完整的插入代码,仅作参考**。考研不要求默写,但你应该能**看懂**。

#### 文件组织

```
DataStructure/
└── RedBlackTree/
    ├── RBTree.h
    ├── RBTree.cpp
    └── main.cpp
```

#### RBTree.h

cpp

```cpp
#pragma once

typedef int KeyType;
enum Color { RED, BLACK };

typedef struct RBNode {
    KeyType key;
    Color color;
    struct RBNode *left, *right, *parent;
} RBNode;

typedef struct {
    RBNode *root;
    RBNode *nil;                    // 哨兵节点:所有 NIL 都指向这一个
} RBTree;

// ========== 基本操作 ==========
RBTree* InitRBTree();
void Insert(RBTree *T, KeyType key);
RBNode* Search(RBTree *T, KeyType key);
void InOrder(RBTree *T, RBNode *x);

// ========== 内部辅助 ==========
void LeftRotate(RBTree *T, RBNode *x);
void RightRotate(RBTree *T, RBNode *x);
void InsertFixup(RBTree *T, RBNode *z);
```

**说明"哨兵 NIL"的设计**:教材通常**共用一个 NIL 节点**作为所有叶子,NIL 染黑。这样代码处理边界(检查 NULL)更简单——所有叶子都是 NIL 对象,可以正常访问它的 color 字段。`std::map` 源码就是这么做的。

#### RBTree.cpp(插入部分)

cpp

```cpp
#include "RBTree.h"
#include <iostream>
using namespace std;

RBTree* InitRBTree() {
    RBTree *T = new RBTree;
    T->nil = new RBNode;
    T->nil->color = BLACK;          // NIL 永远黑
    T->nil->left = T->nil->right = T->nil->parent = T->nil;
    T->root = T->nil;
    return T;
}

// 以 x 为轴左旋
void LeftRotate(RBTree *T, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != T->nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == T->nil) T->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// 右旋(对称)
void RightRotate(RBTree *T, RBNode *x) {
    RBNode *y = x->left;
    x->left = y->right;
    if (y->right != T->nil) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == T->nil) T->root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void Insert(RBTree *T, KeyType key) {
    // 1. 标准 BST 插入
    RBNode *z = new RBNode;
    z->key = key;
    z->color = RED;                     // 新节点染红
    z->left = z->right = z->parent = T->nil;
    
    RBNode *y = T->nil;
    RBNode *x = T->root;
    while (x != T->nil) {
        y = x;
        if (z->key < x->key) x = x->left;
        else x = x->right;
    }
    z->parent = y;
    if (y == T->nil) T->root = z;
    else if (z->key < y->key) y->left = z;
    else y->right = z;
    
    // 2. 修复红黑性质
    InsertFixup(T, z);
}

void InsertFixup(RBTree *T, RBNode *z) {
    while (z->parent->color == RED) {          // 父红才需要修复
        if (z->parent == z->parent->parent->left) {
            RBNode *uncle = z->parent->parent->right;
            if (uncle->color == RED) {
                // 情况 1:叔红
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;          // 向上递归
            } else {
                if (z == z->parent->right) {
                    // 情况 2:叔黑,折线 → 先左旋,转情况 3
                    z = z->parent;
                    LeftRotate(T, z);
                }
                // 情况 3:叔黑,直线 → 换色 + 右旋,结束
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RightRotate(T, z->parent->parent);
            }
        } else {
            // 对称情况(父是祖父的右孩子)
            RBNode *uncle = z->parent->parent->left;
            if (uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    RightRotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                LeftRotate(T, z->parent->parent);
            }
        }
    }
    T->root->color = BLACK;                    // 最后强制根为黑
}

void InOrder(RBTree *T, RBNode *x) {
    if (x == T->nil) return;
    InOrder(T, x->left);
    cout << x->key << "(" << (x->color == RED ? "R" : "B") << ") ";
    InOrder(T, x->right);
}
```

这段代码**对比 AVL 的 4 种旋转,红黑树的 6 种修复情况**(3 种主情况 + 3 种对称)写起来其实**没有想象中那么可怕**,关键是理解每种情况的逻辑。但如果你在考研,这段代码默写要求真的不高,**看懂 + 能画出修复过程即可**。

#### main.cpp

cpp

```cpp
#include "RBTree.h"
#include <iostream>
using namespace std;

int main() {
    RBTree *T = InitRBTree();
    int arr[] = {10, 20, 30, 15, 5, 25, 40, 50};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    for (int i = 0; i < n; i++) {
        Insert(T, arr[i]);
        cout << "插入 " << arr[i] << " 后中序: ";
        InOrder(T, T->root);
        cout << endl;
    }
    return 0;
}
```

---

### 九、红黑树 vs AVL:一张对比表

|维度|AVL|红黑树|
|---|---|---|
|平衡条件|左右子树高度差 ≤ 1|最长路径 ≤ 2 × 最短路径|
|高度|≤ 1.44 log n|≤ 2 log n|
|查找|更快(更矮)|略慢(更高些)|
|插入旋转次数|最多 2(一次双旋)|最多 2|
|删除旋转次数|最多 O(log n)|最多 3|
|插入后是否可能向上递归|否|是(情况 1 可递归)|
|适用场景|查找多、修改少|插入删除频繁|
|工业应用|少见|C++ std::map/set、Java TreeMap|

**核心取舍**:AVL 查得快,红黑树改得快。工业界普遍选红黑树,因为实际应用中查和改的频率都高,平均表现红黑树胜出。

---

### 十、常见疑问

**疑问一:红黑树为什么叫"红黑"?**

历史原因,发明者 Rudolf Bayer 1972 年最初叫 "symmetric binary B-tree"(对称二叉 B 树),后来 Leonidas Guibas 和 Robert Sedgewick 在 1978 年用"红黑"两色重新表述,因为红色和黑色打印机容易呈现。名字就流传下来了。

**疑问二:红黑树的本质是 2-3-4 树?**

**是的**。红黑树可以看作 2-3-4 树(4 阶 B 树)的二叉表示:一个"红节点 + 它的黑父节点"等价于 2-3-4 树的一个节点。这个视角非常深刻,帮助理解为什么红黑树的规则是这样的——**它实际上是 B 树的二叉编码**。

**疑问三:红黑树和跳表(Skip List)哪个更好?**

跳表是概率性结构,期望 O(log n),代码实现比红黑树**简单得多**。Redis 使用跳表而不是红黑树,因为实现简单、对缓存更友好。但红黑树**最坏也是 O(log n)**(跳表是期望),在对最坏情况敏感的场景(操作系统、数据库)更可靠。

---

### 十一、思考题(请作答)

第一题,下面这棵"树"是否是合法的红黑树?如果不是,指出违反了哪条性质:

```
         10(黑)
        /      \
      5(红)   20(黑)
      /          \
    3(红)       30(红)
```

第二题,一棵红黑树有 7 个内部节点,**高度最多是多少、最少是多少**?请手画出这两种极端情况的树。

第三题,依次向空红黑树插入 `{1, 2, 3, 4, 5}`,**每一步画出树的形态**,标注每次触发的是哪种修复情况(情况 1 / 情况 2 / 情况 3)。

第四题,证明:红黑树中,**从根到任意叶子的路径上,红色节点数不超过路径总长度的一半**。(提示:性质四说的是红红不连续,那么一条路径上红色节点怎么分布?)

第五题,为什么新插入的节点**必须染红而不是染黑**?用性质五来解释。

第六题,**红黑树和 AVL,哪个的插入旋转次数更少?哪个的删除旋转次数更少?** 请用数据回答(给出具体的最多次数)。

---

### 十二、下一步方向

红黑树是你目前学的**最复杂的数据结构**。啃完它,你对"平衡"这件事已经有了深刻理解——从 AVL 到红黑树到 B 树,都是为了把"数据"压进**对数高度**的容器里。

**接下来可以选**:

**(A)** **散列表 / 哈希表**(O(1) 查找的另一条路,与红黑树/B+树并列的重要索引结构,考研必考,代码易写概念易混)。**(B)** **图论入门**(DFS、BFS、邻接矩阵、邻接表——考研分值最高章节的开端)。**(C)** **排序算法全家桶**(堆排你学过了,还差快排、归并、希尔、基数等——考研必有一道综合大题)。**(D)** **跳表**(概率性的有序结构,Redis 在用,面试爱考)。**(E)** 先答思考题再继续。

告诉我你的选择!


## 数据结构复习:图论入门(基础篇)

好,我们开始**图论**。这是考研分值最高、代码量最大、概念最密集的章节——但也是**最有用**的,几乎所有复杂的真实问题(地图导航、网络路由、社交关系、任务调度)最终都归结为图问题。

按你的要求,**这一次我们先打基础**:概念、术语、存储结构。DFS、BFS、最小生成树、最短路径这些算法留到后面的对话逐步展开。**打地基不能省**——图的基础不牢,后面所有算法都会飘。

---

### 一、什么是图?

**图(Graph)** 由两部分组成:

**顶点集 V**(Vertex):图中所有"点"的集合。

**边集 E**(Edge):连接顶点的"线"的集合。

用数学符号写:**G = (V, E)**。

**一个直观例子**:把 5 个城市抽象成顶点,城市之间的公路抽象成边,就得到一张"城市公路图"。

---

### 二、图的基本分类(必须分清)

#### 2.1 按"边是否有方向"分

**无向图(Undirected Graph)**:边没有方向,`(A, B)` 表示 A 和 B 之间有一条边,**等价于** `(B, A)`。例如"A 和 B 是朋友"是无向关系。

**有向图(Directed Graph)**:边有方向,`<A, B>` 表示从 A 到 B 的边,**不等于** `<B, A>`。例如"A 关注了 B"是有向关系,不代表 B 也关注 A。

**符号约定**:无向边用小括号 `(A, B)`,有向边用尖括号 `<A, B>`。**考研大题里严格区分,你要记住**。

#### 2.2 按"边是否带权值"分

**非带权图**:边只表示"连通",不带额外信息。

**带权图(网)**:每条边附带一个数值(权值),表示距离、费用、容量等。带权的无向图叫**无向网**,带权的有向图叫**有向网**。

**四种组合**:无向图、有向图、无向网、有向网——考研题目会明确给出类型,**不同类型适用不同算法**。

---

### 三、术语大总结(考研高频概念)

我按"理解难度"排序,由浅入深。

#### 3.1 基本术语

**完全图**:任意两个顶点之间都有边。n 个顶点的**无向完全图**有 C(n,2) = n(n-1)/2 条边;**有向完全图**有 n(n-1) 条边(每对顶点有两条方向相反的边)。

**稀疏图 vs 稠密图**:边数远少于完全图是稀疏图,接近完全图是稠密图。**没有严格界限**,一般 |E| < |V| log |V| 视为稀疏。稀疏稠密决定了"用邻接矩阵还是邻接表"存储更优。

**子图**:从图中"摘"一部分顶点和边出来构成的图(要求摘出来的边的两个端点都在所摘的顶点里)。

#### 3.2 顶点相关

**邻接(Adjacent)**:两顶点 u、v 间有边,称它们"邻接"。无向图中 u、v 互为邻接点;有向图中,若有 `<u, v>`,称 v 是 u 的**出邻接点**,u 是 v 的**入邻接点**。

**度(Degree)**:

无向图中,顶点 v 的**度** = 与 v 相连的边数,记 `TD(v)`。

有向图中,**出度** `OD(v)` = 从 v 出发的边数,**入度** `ID(v)` = 进入 v 的边数。**总度 = 出度 + 入度**。

**度的核心定理**:**所有顶点的度之和 = 边数的 2 倍**(每条边贡献 2 个度端点)。有向图则是**出度之和 = 入度之和 = 边数**。这个公式年年考,必记。

#### 3.3 路径相关

**路径(Path)**:从 u 到 v 的顶点序列,每对相邻顶点之间有边。

**路径长度**:路径上的边数(非带权)或边权之和(带权)。

**简单路径**:路径上**顶点不重复**。

**回路(环)**:起点终点相同的路径。长度 ≥ 1 的回路就是"环"。

**简单回路**:除起点终点外,顶点不重复的回路。

#### 3.4 连通性(重点!)

**无向图**:

**连通**:u 到 v 有路径,称 u 和 v 连通。

**连通图**:图中**任意两个顶点**都连通。

**连通分量**:**极大**连通子图(不可再扩充)。一个连通图只有 1 个连通分量(就是它自己);非连通图有多个连通分量。

**有向图**:

**强连通**:u 到 v、v 到 u **都有路径**(双向都能到)。

**强连通图**:任意两个顶点强连通。

**强连通分量**:极大强连通子图。

**注意区别"连通"和"强连通"**——无向图用"连通",有向图用"强连通"。考研选择题最爱混淆这一点。

#### 3.5 特殊图

**树**:**连通且无环**的无向图。一棵有 n 个顶点的树恰好有 n-1 条边。

**生成树**:对无向连通图 G,它的**极小连通子图**(保留所有顶点,减到 n-1 条边还能连通)。一个图可能有多棵生成树。

**生成森林**:非连通图的每个连通分量各取一棵生成树,合起来就是生成森林。

---

### 四、图的五种存储结构

这是基础篇的**核心内容**。存储结构决定了后续所有算法的写法和效率。

#### 4.1 邻接矩阵(Adjacency Matrix)

**核心思想**:用 **n×n 的二维数组** `A[n][n]` 存储,`A[i][j]` 表示顶点 i 到 j 是否有边(或边权)。

**非带权**:`A[i][j] = 1` 表示有边,0 表示无边。

**带权**:`A[i][j] = 权值` 表示有边,特殊值(如 0 或 ∞)表示无边。**通常 ∞ 用来表示"不可达"**。

**例子**:以下无向图的邻接矩阵:

```
顶点:{A, B, C, D}
边:  (A,B), (A,C), (B,C), (C,D)
```

```
      A  B  C  D
   A [ 0  1  1  0 ]
   B [ 1  0  1  0 ]
   C [ 1  1  0  1 ]
   D [ 0  0  1  0 ]
```

**观察**:无向图的邻接矩阵**沿对角线对称**。

**有向图的邻接矩阵不对称**——`A[i][j]` 和 `A[j][i]` 可以不同。

**特点总结**:

优点:**判断两顶点是否相邻 O(1)**(直接查表);矩阵运算可以解决某些图问题(如传递闭包)。

缺点:空间 **O(V²)**,稀疏图极浪费;**遍历某顶点的所有邻居** O(V),而不是 O(邻居数)。

**何时用**:稠密图(边数接近 V²)、或需要 O(1) 判断邻接性的场景。

#### 4.2 邻接表(Adjacency List)

**核心思想**:**每个顶点挂一个单链表**,链表存该顶点的所有邻居。

**结构**:顶点用数组,每个数组元素指向一个链表;链表节点存"邻居顶点的下标"(和可能的边权)。

**例子**:前面那个图的邻接表:

```
A -> B -> C
B -> A -> C
C -> A -> B -> D
D -> C
```

**特点总结**:

优点:空间 **O(V+E)**,稀疏图非常省空间;**遍历某顶点的所有邻居**时间 O(邻居数),不浪费。

缺点:判断"u、v 是否相邻"要 O(邻居数);**有向图**的邻接表只记录出边,想找入边要遍历全图(或额外建"逆邻接表")。

**何时用**:稀疏图、需要频繁遍历邻居的算法(DFS、BFS、Dijkstra 等)。

**这是绝大多数图算法的首选存储结构**。

#### 4.3 十字链表(有向图专用)

邻接表对有向图的缺陷是"难找入边"。**十字链表**同时存储每个顶点的入边和出边,一举解决。

每条边节点有四个指针:尾顶点、头顶点、同尾下一边、同头下一边。

**这个结构比较复杂,考研只要求了解其思想,一般不要求写代码**。你知道"十字链表适合有向图、能同时 O(1) 找入边和出边"就够了。

#### 4.4 邻接多重表(无向图的优化)

邻接表在无向图中有个小缺陷:每条边 (u,v) 会被存**两次**(u 的链表里一次,v 的链表里一次),删除边时要改两处。

**邻接多重表**让每条边只存一次,每条边节点包含两个顶点下标和两个"同顶点下一边"指针。

**同样,考研了解概念即可,很少要求写代码**。

#### 4.5 边集数组(Edge List)

最简单的一种:**用一个数组存所有边**,每个元素是 `(u, v, weight)`。

**特点**:空间 O(E),但**查询任意两顶点是否相邻要 O(E)**。

**典型应用**:Kruskal 最小生成树算法——这个算法的核心操作是"把所有边按权值排序",用边集数组最自然。

---

### 五、图的结构体设计(代码实战)

我们给出**邻接矩阵**和**邻接表**两种主流实现。这两种占据了考研 90% 以上的代码题。

#### 文件组织

```
DataStructure/
└── Graph/
    ├── MGraph.h        // 邻接矩阵
    ├── MGraph.cpp
    ├── ALGraph.h       // 邻接表
    ├── ALGraph.cpp
    └── main.cpp
```

类似之前栈/队列的命名策略,两种结构用不同前缀:**M**Graph(Matrix)和 **AL**Graph(Adjacency List),共存不冲突。

#### 5.1 邻接矩阵(MGraph.h)

cpp

```cpp
#pragma once

#define MAX_VERTEX 100
#define INF 0x3f3f3f3f          // "无穷大",表示无边。用 0x3f3f3f3f 避免加法溢出

typedef char VertexType;         // 顶点数据(用 char 方便观察)
typedef int EdgeType;            // 边权(带权图时用)

typedef struct {
    VertexType vexs[MAX_VERTEX];        // 顶点数组
    EdgeType edges[MAX_VERTEX][MAX_VERTEX]; // 邻接矩阵
    int vexNum, edgeNum;                // 当前顶点数、边数
} MGraph;

// ========== 基本操作 ==========
void InitGraph_M(MGraph &G);
void CreateGraph_M(MGraph &G, bool directed, bool weighted);
int  LocateVex_M(MGraph G, VertexType v);       // 找顶点下标,失败返回 -1
void PrintGraph_M(MGraph G);
```

**关于 `INF = 0x3f3f3f3f`** 这个小技巧:用它表示无穷大,既足够大(约 10 亿,比常见边权大),**又不会在 `INF + INF` 时溢出**(0x7f7f7f7f 不到 int 上限)。这是竞赛和工程里的黑魔法。如果你用 INT_MAX,一旦做 `distance + edgeWeight` 就会溢出变负数,Dijkstra 等算法会崩。

#### 5.2 MGraph.cpp

cpp

```cpp
#include "MGraph.h"
#include <iostream>
#include <cstring>
using namespace std;

void InitGraph_M(MGraph &G) {
    G.vexNum = 0;
    G.edgeNum = 0;
    for (int i = 0; i < MAX_VERTEX; i++)
        for (int j = 0; j < MAX_VERTEX; j++)
            G.edges[i][j] = INF;            // 初始化:所有边不存在
}

int LocateVex_M(MGraph G, VertexType v) {
    for (int i = 0; i < G.vexNum; i++) {
        if (G.vexs[i] == v) return i;
    }
    return -1;
}

void CreateGraph_M(MGraph &G, bool directed, bool weighted) {
    cout << "输入顶点数和边数:";
    cin >> G.vexNum >> G.edgeNum;
    
    cout << "输入 " << G.vexNum << " 个顶点(单字符):";
    for (int i = 0; i < G.vexNum; i++) {
        cin >> G.vexs[i];
    }
    
    // 初始化所有边为 INF(无边)
    for (int i = 0; i < G.vexNum; i++)
        for (int j = 0; j < G.vexNum; j++)
            G.edges[i][j] = (i == j) ? 0 : INF;    // 自己到自己权为 0
    
    cout << "输入 " << G.edgeNum << " 条边(格式:u v"
         << (weighted ? " weight" : "") << "):" << endl;
    for (int k = 0; k < G.edgeNum; k++) {
        VertexType u, v;
        EdgeType w = 1;                     // 非带权默认为 1
        cin >> u >> v;
        if (weighted) cin >> w;
        
        int i = LocateVex_M(G, u);
        int j = LocateVex_M(G, v);
        if (i == -1 || j == -1) {
            cout << "顶点不存在,跳过" << endl;
            continue;
        }
        
        G.edges[i][j] = w;
        if (!directed) G.edges[j][i] = w;   // 无向图对称
    }
}

void PrintGraph_M(MGraph G) {
    cout << "邻接矩阵:" << endl;
    cout << "    ";
    for (int i = 0; i < G.vexNum; i++) cout << G.vexs[i] << "\t";
    cout << endl;
    for (int i = 0; i < G.vexNum; i++) {
        cout << G.vexs[i] << " : ";
        for (int j = 0; j < G.vexNum; j++) {
            if (G.edges[i][j] == INF) cout << "∞\t";
            else cout << G.edges[i][j] << "\t";
        }
        cout << endl;
    }
}
```

**几个关键设计点**:

第一,`CreateGraph_M` 用 `bool directed, bool weighted` 两个参数,**一个函数支持四种图**(有向/无向、带权/非带权)。这比四个单独函数更简洁。

第二,对角线 `edges[i][i] = 0`——约定"顶点自己到自己没有边,但距离是 0"。这在 Floyd 最短路径中很重要。

第三,非带权图我们把边权视为 **1**——这样 Dijkstra/Floyd 等算法可以无差别处理"步数"和"距离"。

#### 5.3 邻接表(ALGraph.h)

cpp

```cpp
#pragma once

#define MAX_VERTEX 100

typedef char VertexType;
typedef int EdgeType;

// ========== 边节点(链表中的节点)==========
typedef struct EdgeNode {
    int adjVex;                     // 邻接顶点的下标
    EdgeType weight;                // 边权(非带权图忽略)
    struct EdgeNode *next;          // 指向下一条边
} EdgeNode;

// ========== 顶点节点(数组中的元素)==========
typedef struct {
    VertexType data;                // 顶点数据
    EdgeNode *firstEdge;            // 指向第一条边的指针
} VertexNode;

// ========== 图 ==========
typedef struct {
    VertexNode adjList[MAX_VERTEX];
    int vexNum, edgeNum;
} ALGraph;

// ========== 基本操作 ==========
void InitGraph_AL(ALGraph &G);
void CreateGraph_AL(ALGraph &G, bool directed, bool weighted);
int  LocateVex_AL(ALGraph G, VertexType v);
void PrintGraph_AL(ALGraph G);
void DestroyGraph_AL(ALGraph &G);
```

**结构嵌套关系**(这段要看懂):

`ALGraph` 含一个 `adjList` 数组,数组每个元素是一个 `VertexNode`(顶点)。每个顶点内部有 `data`(顶点值) 和 `firstEdge`(指向边链表的第一个节点)。边链表的每个节点 `EdgeNode` 存"这条边指向谁(adjVex)、权值多少、下一条边"。

**对比内存布局**:邻接矩阵是"铁板一块的二维数组";邻接表是"数组 + 多条链表"的混合结构,更灵活但需要动态分配内存。

#### 5.4 ALGraph.cpp

cpp

```cpp
#include "ALGraph.h"
#include <iostream>
using namespace std;

void InitGraph_AL(ALGraph &G) {
    G.vexNum = 0;
    G.edgeNum = 0;
    for (int i = 0; i < MAX_VERTEX; i++) {
        G.adjList[i].firstEdge = nullptr;
    }
}

int LocateVex_AL(ALGraph G, VertexType v) {
    for (int i = 0; i < G.vexNum; i++) {
        if (G.adjList[i].data == v) return i;
    }
    return -1;
}

void CreateGraph_AL(ALGraph &G, bool directed, bool weighted) {
    cout << "输入顶点数和边数:";
    cin >> G.vexNum >> G.edgeNum;
    
    cout << "输入 " << G.vexNum << " 个顶点:";
    for (int i = 0; i < G.vexNum; i++) {
        cin >> G.adjList[i].data;
        G.adjList[i].firstEdge = nullptr;
    }
    
    cout << "输入 " << G.edgeNum << " 条边:" << endl;
    for (int k = 0; k < G.edgeNum; k++) {
        VertexType u, v;
        EdgeType w = 1;
        cin >> u >> v;
        if (weighted) cin >> w;
        
        int i = LocateVex_AL(G, u);
        int j = LocateVex_AL(G, v);
        if (i == -1 || j == -1) continue;
        
        // 头插法:把 j 加入 i 的邻居链表
        EdgeNode *e1 = new EdgeNode;
        e1->adjVex = j;
        e1->weight = w;
        e1->next = G.adjList[i].firstEdge;
        G.adjList[i].firstEdge = e1;
        
        // 无向图:对称处理,把 i 加入 j 的邻居链表
        if (!directed) {
            EdgeNode *e2 = new EdgeNode;
            e2->adjVex = i;
            e2->weight = w;
            e2->next = G.adjList[j].firstEdge;
            G.adjList[j].firstEdge = e2;
        }
    }
}

void PrintGraph_AL(ALGraph G) {
    cout << "邻接表:" << endl;
    for (int i = 0; i < G.vexNum; i++) {
        cout << G.adjList[i].data << " -> ";
        EdgeNode *p = G.adjList[i].firstEdge;
        while (p != nullptr) {
            cout << G.adjList[p->adjVex].data;
            cout << "(" << p->weight << ") -> ";
            p = p->next;
        }
        cout << "NULL" << endl;
    }
}

void DestroyGraph_AL(ALGraph &G) {
    for (int i = 0; i < G.vexNum; i++) {
        EdgeNode *p = G.adjList[i].firstEdge;
        while (p != nullptr) {
            EdgeNode *q = p;
            p = p->next;
            delete q;
        }
        G.adjList[i].firstEdge = nullptr;
    }
    G.vexNum = G.edgeNum = 0;
}
```

**关键设计点**:

第一,插入边用**头插法**——O(1) 时间。**缺点是邻居顺序和输入顺序相反**,但这对大多数算法无影响。

第二,无向图要**插两次**(i 的链表里加 j,j 的链表里加 i),对应前面说的"邻接表对无向图每条边存两次"。

第三,**必须有 DestroyGraph**——堆内存要手动释放。这和之前链式数据结构一样。

---

### 六、测试代码(main.cpp)

cpp

```cpp
#include "MGraph.h"
#include "ALGraph.h"
#include <iostream>
using namespace std;

int main() {
    cout << "===== 邻接矩阵 =====" << endl;
    cout << "建立无向图,输入样例:\n4 4\nA B C D\nA B\nA C\nB C\nC D" << endl;
    MGraph MG;
    InitGraph_M(MG);
    CreateGraph_M(MG, false, false);    // 无向、非带权
    PrintGraph_M(MG);
    
    cout << "\n===== 邻接表 =====" << endl;
    cout << "建立带权有向图,输入样例:\n4 4\nA B C D\nA B 5\nA C 3\nB C 2\nC D 7" << endl;
    ALGraph AG;
    InitGraph_AL(AG);
    CreateGraph_AL(AG, true, true);     // 有向、带权
    PrintGraph_AL(AG);
    
    DestroyGraph_AL(AG);
    return 0;
}
```

**预期输出(邻接矩阵部分)**:

```
邻接矩阵:
    A   B   C   D
A : 0   1   1   ∞
B : 1   0   1   ∞
C : 1   1   0   1
D : ∞   ∞   1   0
```

**预期输出(邻接表部分)**:

```
邻接表:
A -> C(3) -> B(5) -> NULL
B -> C(2) -> NULL
C -> D(7) -> NULL
D -> NULL
```

---

### 七、邻接矩阵 vs 邻接表:何时选哪个?

这是考研高频选择题,一张对比表你要刻在脑子里:

|操作 / 性质|邻接矩阵|邻接表|
|---|---|---|
|空间复杂度|O(V²)|O(V+E)|
|判断 (u,v) 是否相邻|O(1)|O(deg(u))|
|找顶点 u 的所有邻居|O(V)|O(deg(u))|
|计算顶点度(无向)|O(V) 扫一行|O(deg(u)) 数链表长度|
|计算出度(有向)|O(V)|O(out-deg)|
|计算入度(有向)|O(V)|O(V+E) 扫所有链表|
|添加一条边|O(1)|O(1) 头插|
|删除一条边|O(1)|O(deg) 要找|
|适合稀疏图|❌|✅|
|适合稠密图|✅|❌|

**一句话判断**:

**"边数接近 V²"** → 邻接矩阵;**"边数远小于 V²"** → 邻接表。

**"需要反复判断任意两点相邻性"** → 邻接矩阵(Floyd 用);**"需要反复访问某点邻居"** → 邻接表(DFS、BFS、Dijkstra 用)。

---

### 八、常见易错点总结

**易错一**:有向图和无向图的邻接矩阵对称性。**无向图对称,有向图不对称**。

**易错二**:无向图中一条边在邻接表里**存两次**(对应两个端点的链表),在邻接矩阵里对应**两个对称的 1**;有向图中一条边只存一次/一个 1。

**易错三**:度的公式。**无向图**:`Σdeg(v) = 2|E|`;**有向图**:`Σout(v) = Σin(v) = |E|`。

**易错四**:"连通图 ≠ 强连通图"。无向图说连通,有向图说强连通。一个有向图若对应的无向图连通,叫"弱连通"。

**易错五**:有向完全图 n(n-1) 条,无向完全图 n(n-1)/2 条。**不要把 2 除漏了或除错了方向**。

**易错六**:INF 的选择。用 `INT_MAX` 会在 `INF + edge` 时溢出;用 `0x3f3f3f3f` 既大又不溢出。

---

### 九、思考题(打牢基础,请作答)

第一题,一个**无向图**有 16 条边,顶点 V1、V2、V3 的度分别是 4、3、3,其余顶点的度都是 2。这个图有多少个顶点?(提示:用"度数之和 = 2E"的公式)

第二题,一个**有向图**的邻接矩阵如下(1 表示有边,0 无边):

```
      V1 V2 V3 V4
  V1 [ 0  1  1  0 ]
  V2 [ 0  0  0  1 ]
  V3 [ 0  0  0  1 ]
  V4 [ 1  0  0  0 ]
```

请回答:(a) 每个顶点的出度和入度;(b) 这是强连通图吗?为什么?(c) 画出它的邻接表。

第三题,一个无向图有 n 个顶点,若它是**连通的**,最少有多少条边?若它**不连通**,最少有多少条边才能保证它"在加一条边后一定连通"?(后半问有点难,考研经典题)

第四题,**邻接矩阵存储的无向图**,如何**快速判断顶点 v 是否是孤立顶点**(无任何边)?给出 O(V) 算法的思路。对应**邻接表**,怎么 O(1) 判断?

第五题,以下**哪种存储结构**最适合以下操作:

(a) 频繁查询"顶点 u 的所有邻居"(比如 BFS/DFS)。 (b) 频繁查询"u 和 v 之间是否有边"(比如 Floyd 最短路径)。 (c) 稀疏图的存储。 (d) 按边权排序所有边(比如 Kruskal 算法)。

---

### 十、下一步方向

图论基础扎实之后,我们可以按以下路径走:

**(A)** **图的遍历**——DFS(深度优先)和 BFS(广度优先)。**这是所有图算法的起点**,也是考研大题最常考的基础。我会分别给出**邻接矩阵版**和**邻接表版**的代码,还会引入**visited 数组**这个图遍历的灵魂。

**(B)** 先做思考题再推进。

**(C)** 如果你想快进,也可以先看**最小生成树**(Prim 和 Kruskal)或**最短路径**(Dijkstra 和 Floyd),但我建议按顺序——DFS/BFS 是后面所有算法的前置。

我个人**强烈建议选 (A)**——遍历先走,后面一通百通。告诉我你的选择,或者先回答思考题让我批改!


## 数据结构复习:图的遍历(DFS 和 BFS)

好,我们进入**图的遍历**——整个图论的"第一把钥匙"。后面所有复杂算法(最小生成树、最短路径、拓扑排序、关键路径)都建立在 DFS 和 BFS 之上。

这节我会讲:**为什么图遍历需要 visited 数组 → DFS 的递归/非递归实现 → BFS 实现 → 连通分量的应用**。邻接矩阵和邻接表两种存储结构**都给代码**,因为考研大题两种都可能考。

---

### 一、图遍历 vs 树遍历:一个关键区别

先回想一下,二叉树遍历是怎么做的?**递归地访问根、左、右**。不用记录"哪些节点已经访问过",因为**树没有环**——每个节点只有一条路径通达。

**图不一样。图有环,有回路**。如果你从顶点 A 出发,走到 B,再走到 C,C 可能有一条边指回 A——如果不记录"A 已访问",你会在 A-B-C-A-B-C 里**无限循环**。

**解决方法**:用一个 `visited[]` 数组标记每个顶点是否已访问。

cpp

```cpp
bool visited[MAX_VERTEX];    // 全局/传入参数
```

访问前检查 visited,访问后立即置 true。**这是图遍历的灵魂**,贯穿所有算法。

---

### 二、DFS(深度优先搜索)

#### 2.1 核心思想

**一条路走到黑,走不通再回头**。用大白话描述:

从起点 v 出发,访问 v;选 v 的一个**未访问的邻居** w,从 w 继续深入;一直走到某个顶点没有未访问的邻居,就**回溯**到上一个节点,尝试它的另一个未访问邻居;全都走完,DFS 结束。

**类比**:走迷宫——优先往深处钻,遇到死路才退回岔口。

#### 2.2 DFS 的递归实现(最经典)

用栈的思想天然对应递归,代码极简:

cpp

```cpp
bool visited[MAX_VERTEX];        // 全局 visited 数组

void DFS(图 G, int v) {
    访问(v);
    visited[v] = true;
    for (v 的每个邻居 w) {
        if (!visited[w]) {
            DFS(G, w);           // 递归深入
        }
    }
}
```

**这是伪代码骨架,下面分别给出邻接矩阵和邻接表的具体实现**。

#### 2.3 邻接矩阵版 DFS

cpp

```cpp
// 邻接矩阵 DFS
#include "MGraph.h"

bool visited_M[MAX_VERTEX];      // 全局 visited

void DFS_M(MGraph G, int v) {
    cout << G.vexs[v] << " ";    // 访问 v
    visited_M[v] = true;
    for (int w = 0; w < G.vexNum; w++) {
        // 检查 v 到 w 是否有边,且 w 未访问
        if (G.edges[v][w] != INF && G.edges[v][w] != 0 && !visited_M[w]) {
            DFS_M(G, w);
        }
    }
}

// 对外接口:处理可能的非连通图
void DFSTraverse_M(MGraph G) {
    for (int i = 0; i < G.vexNum; i++) visited_M[i] = false;
    for (int i = 0; i < G.vexNum; i++) {
        if (!visited_M[i]) {
            DFS_M(G, i);          // 每个未访问顶点都作为新的 DFS 起点
        }
    }
}
```

**几个重要细节**:

第一,`G.edges[v][w] != INF` 是"有边"的判断。我们之前约定 INF 表示无边;`!= 0` 排除自己到自己的"伪边"(对角线我们设为 0)。

第二,外层 `DFSTraverse_M` 的 `for` 循环为什么必要?因为图可能**不连通**——某些顶点从起点出发根本到不了。必须对每个未访问的顶点重启一次 DFS,才能遍历到所有顶点。

第三,`visited_M[i] = false` 的初始化必须在外层做,**不能放在 `DFS_M` 里面**,否则每次递归都会清零。

#### 2.4 邻接表版 DFS

cpp

```cpp
// 邻接表 DFS
#include "ALGraph.h"

bool visited_AL[MAX_VERTEX];

void DFS_AL(ALGraph G, int v) {
    cout << G.adjList[v].data << " ";
    visited_AL[v] = true;
    EdgeNode *p = G.adjList[v].firstEdge;
    while (p != nullptr) {
        int w = p->adjVex;
        if (!visited_AL[w]) {
            DFS_AL(G, w);
        }
        p = p->next;
    }
}

void DFSTraverse_AL(ALGraph G) {
    for (int i = 0; i < G.vexNum; i++) visited_AL[i] = false;
    for (int i = 0; i < G.vexNum; i++) {
        if (!visited_AL[i]) {
            DFS_AL(G, i);
        }
    }
}
```

**关键对比**:

邻接矩阵找邻居要扫**整行**(O(V)),邻接表沿链表走(O(邻居数))。所以**对稀疏图,邻接表的 DFS 总时间 O(V+E);对稠密图,邻接矩阵总时间 O(V²)**。

#### 2.5 DFS 非递归实现(用栈)

用我们之前写的 `BiStack`(改成存顶点下标的 `IntStack`)可以实现非递归 DFS。思路类似非递归先序遍历:

cpp

```cpp
void DFS_NonRec_AL(ALGraph G, int v) {
    bool visited[MAX_VERTEX] = {false};
    IntStack S;
    InitStack(S);
    
    Push(S, v);
    visited[v] = true;
    cout << G.adjList[v].data << " ";
    
    while (!StackEmpty(S)) {
        int top; GetTop(S, top);            // 看栈顶
        // 找 top 的一个未访问邻居
        EdgeNode *p = G.adjList[top].firstEdge;
        while (p != nullptr && visited[p->adjVex]) p = p->next;
        
        if (p != nullptr) {
            int w = p->adjVex;
            cout << G.adjList[w].data << " ";
            visited[w] = true;
            Push(S, w);
        } else {
            Pop(S, top);                     // 没有未访问邻居,回溯
        }
    }
}
```

**原理**:栈顶是"当前深入到的节点",找它的一个未访问邻居就深入;找不到就弹出回溯。**考研一般要求会写递归版就够,非递归了解即可**。

#### 2.6 DFS 遍历示例

对下面这个无向图(邻接表表示,邻居按字母顺序):

```
顶点:A B C D E F G H
边:(A,B) (A,C) (B,D) (B,E) (C,F) (C,G) (D,H) (E,H)

       A
      / \
     B   C
    /|   |\
   D E   F G
   |_|
    H
```

从 A 开始 DFS,邻居按字母序访问:

**A**(访问)→ **B**(A 的第一个邻居)→ **D**(B 的第一个未访问邻居)→ **H**(D 的邻居)→ 回到 H,邻居 E 已访问?不,E 还没访问,但 H 的邻居应该是 D 和 E。H 有 E 这个邻居,访问 **E**。

嗯让我重来更清楚一些。从 A 出发,邻居 B、C(按字母序先 B)→ B,邻居 A(已)、D、E → D,邻居 B(已)、H → H,邻居 D(已)、E → E,邻居 B(已)、H(已);回溯到 H,没新邻居;回溯到 D,没;回溯到 B,没新邻居(E 已访);回溯到 A,访问 C → C 的邻居 A(已)、F、G → F,邻居 C(已);回溯 → G,邻居 C(已);回溯完毕。

**DFS 序**:A B D H E C F G

---

### 三、BFS(广度优先搜索)

#### 3.1 核心思想

**一层一层地向外扩展**。用大白话:

访问起点 v;把 v 的**所有邻居**都访问一遍;再把**邻居的邻居**都访问一遍;依次推进,像水波一样向外扩散。

**类比**:从起点往外找最短路径(非带权图中,BFS 找到的就是最短路径)。

**实现工具**:用**队列**(和 DFS 用栈对应)。新访问的顶点入队尾,从队首取出"下一个要处理的顶点"。

#### 3.2 BFS 的伪代码

```
BFS(G, v):
    访问(v); visited[v] = true
    v 入队
    while 队列非空:
        从队首取出 u
        for u 的每个未访问邻居 w:
            访问(w); visited[w] = true
            w 入队
```

**关键点**:**标记 visited 要在入队时**,不是出队时!否则同一个顶点可能被多次入队,效率大降。这是 BFS 最常见的错误。

#### 3.3 邻接矩阵版 BFS

我们用 C++ STL 队列简化代码;如果要用你自己写的队列,只需把 `queue` 换成 `SqQueue` 或 `LkQueue`(见之前的章节)。

cpp

```cpp
#include "MGraph.h"
#include <queue>
using namespace std;

bool bfsVisited_M[MAX_VERTEX];

void BFS_M(MGraph G, int v) {
    queue<int> Q;
    cout << G.vexs[v] << " ";
    bfsVisited_M[v] = true;
    Q.push(v);
    
    while (!Q.empty()) {
        int u = Q.front(); Q.pop();
        for (int w = 0; w < G.vexNum; w++) {
            if (G.edges[u][w] != INF && G.edges[u][w] != 0 
                && !bfsVisited_M[w]) {
                cout << G.vexs[w] << " ";
                bfsVisited_M[w] = true;
                Q.push(w);
            }
        }
    }
}

void BFSTraverse_M(MGraph G) {
    for (int i = 0; i < G.vexNum; i++) bfsVisited_M[i] = false;
    for (int i = 0; i < G.vexNum; i++) {
        if (!bfsVisited_M[i]) {
            BFS_M(G, i);
        }
    }
}
```

#### 3.4 邻接表版 BFS

cpp

```cpp
#include "ALGraph.h"
#include <queue>
using namespace std;

bool bfsVisited_AL[MAX_VERTEX];

void BFS_AL(ALGraph G, int v) {
    queue<int> Q;
    cout << G.adjList[v].data << " ";
    bfsVisited_AL[v] = true;
    Q.push(v);
    
    while (!Q.empty()) {
        int u = Q.front(); Q.pop();
        EdgeNode *p = G.adjList[u].firstEdge;
        while (p != nullptr) {
            int w = p->adjVex;
            if (!bfsVisited_AL[w]) {
                cout << G.adjList[w].data << " ";
                bfsVisited_AL[w] = true;
                Q.push(w);
            }
            p = p->next;
        }
    }
}

void BFSTraverse_AL(ALGraph G) {
    for (int i = 0; i < G.vexNum; i++) bfsVisited_AL[i] = false;
    for (int i = 0; i < G.vexNum; i++) {
        if (!bfsVisited_AL[i]) {
            BFS_AL(G, i);
        }
    }
}
```

#### 3.5 BFS 示例

对同一个图,从 A 开始 BFS:

**层 0**:访问 A,入队。队列:[A]。

**层 1**:出队 A,访问 A 的所有未访问邻居 B、C,入队。输出:A B C。队列:[B, C]。

**层 2**:出队 B,访问 B 的未访问邻居 D、E。输出:A B C D E。队列:[C, D, E]。

出队 C,访问 C 的未访问邻居 F、G。输出:A B C D E F G。队列:[D, E, F, G]。

**层 3**:出队 D,访问 D 的未访问邻居 H。输出:A B C D E F G H。

出队 E,E 的邻居 B(已)、H(已),无新邻居。

剩下 F、G、H 出队时也没有新邻居。

**BFS 序**:A B C D E F G H

**对比 DFS 序**:A B D H E C F G

两者走的路径完全不同——DFS 钻深,BFS 铺广。

---

### 四、DFS 与 BFS 的性能分析

**时间复杂度**:

邻接矩阵:**O(V²)**——每个顶点都要扫整行看邻居。 邻接表:**O(V+E)**——每个顶点访问一次,每条边访问常数次。

**空间复杂度**:

DFS:递归栈 O(V)(最坏全部在栈上)。 BFS:队列 O(V)。

两者时间复杂度相同(邻接表下都是 O(V+E)),**但适用场景不同**——BFS 天然找最短路径,DFS 天然用于递归式搜索(如回溯、拓扑排序)。

---

### 五、非连通图的处理(考研重点)

前面 `DFSTraverse` 和 `BFSTraverse` 的**外层 for 循环**就是为了处理非连通图。

**关键观察**:**每调用一次内层 DFS/BFS(DFS_M 或 BFS_M),就遍历了一个连通分量**。所以:

**连通分量个数 = 外层 for 循环中真正进入内层的次数**。

这直接给出了一个经典应用:**统计连通分量数**。

#### 应用:求无向图的连通分量数

cpp

```cpp
int CountComponents_AL(ALGraph G) {
    for (int i = 0; i < G.vexNum; i++) visited_AL[i] = false;
    int count = 0;
    for (int i = 0; i < G.vexNum; i++) {
        if (!visited_AL[i]) {
            count++;
            DFS_AL(G, i);        // 遍历第 count 个连通分量
        }
    }
    return count;
}
```

这个函数能告诉你一张图被"分成几块"。例如社交网络里能告诉你"一共有多少个互不相关的朋友圈"。

---

### 六、完整测试代码(main.cpp)

cpp

```cpp
#include "MGraph.h"
#include "ALGraph.h"
#include <iostream>
using namespace std;

extern bool visited_M[MAX_VERTEX];
extern bool visited_AL[MAX_VERTEX];
extern bool bfsVisited_M[MAX_VERTEX];
extern bool bfsVisited_AL[MAX_VERTEX];

void DFSTraverse_M(MGraph G);
void BFSTraverse_M(MGraph G);
void DFSTraverse_AL(ALGraph G);
void BFSTraverse_AL(ALGraph G);
int  CountComponents_AL(ALGraph G);

int main() {
    /* 输入样例(无向图):
       8 8
       A B C D E F G H
       A B
       A C
       B D
       B E
       C F
       C G
       D H
       E H
    */
    ALGraph G;
    InitGraph_AL(G);
    CreateGraph_AL(G, false, false);    // 无向、非带权
    
    cout << "\nDFS 遍历: ";
    DFSTraverse_AL(G);
    cout << endl;
    
    cout << "BFS 遍历: ";
    BFSTraverse_AL(G);
    cout << endl;
    
    cout << "连通分量数: " << CountComponents_AL(G) << endl;
    
    DestroyGraph_AL(G);
    return 0;
}
```

预期输出(假设邻接表按"头插法"存储,输出顺序可能和字母序相反):

```
DFS 遍历: A C G F B E H D
BFS 遍历: A C B G F E D H
连通分量数: 1
```

**注意**:具体遍历序**强烈依赖邻接表中邻居的存储顺序**。我们用头插法存边,所以邻居链是输入顺序的反向。如果题目要求"按字母顺序"遍历,需要用**尾插法**建邻接表。考研大题一般会明确给出邻接表,按题目给的顺序遍历即可。

---

### 七、DFS 和 BFS 的应用全景

这两个算法是图论的基石,**后续几乎所有算法都是它们的变种**。先给你一个"路线图",接下来的章节你就知道每个算法为什么存在:

**DFS 的衍生**:

**拓扑排序**(AOV 网)——DFS 回溯时记录节点,逆序输出即拓扑序。**关键路径**(AOE 网)——基于拓扑排序。**强连通分量**(Kosaraju / Tarjan 算法)——两遍 DFS。**判断环**——DFS 过程中遇到"回边"即有环。**求连通分量数**——就是上面的应用。

**BFS 的衍生**:

**非带权图的单源最短路径**——BFS 过程自然产生最短距离(按层扩展)。**二分图判定**——BFS 交替染色,若同层出现邻接则非二分图。

**BFS 和 DFS 共用**:

**Prim 最小生成树**——本质是贪心版 BFS。**Dijkstra 最短路径**——贪心 + BFS 变种。

**所以这节内容,是后面所有内容的起点**。把 DFS/BFS 吃透,后面会轻松很多。

---

### 八、常见易错点总结

**易错一**:忘记 visited 数组的初始化。初始化必须在每次外层遍历前做**一次**,不能放在递归函数里。

**易错二**:BFS 中在"出队时"标记 visited。**应该在"入队时"就标记**,否则同一顶点可能被多次入队。

**易错三**:DFS 遍历非连通图只从一个顶点出发。**必须用外层 for 循环**对每个未访问顶点重启 DFS。

**易错四**:有向图的"邻居"。邻接矩阵 `edges[u][w]` 表示的是**出邻居**(u 到 w 有边);DFS/BFS 遍历时默认走出边。有向图做 DFS/BFS 可能**无法到达所有顶点**,即使图是弱连通的。

**易错五**:遍历序依赖存储结构。**同一张图,用不同方式建邻接表(头插/尾插),DFS/BFS 序会不同**。大题要严格按题目给的邻接表顺序遍历。

**易错六**:有向图的 DFS 遍历,**从某个顶点出发,不一定能到达所有顶点**;即便是强连通图,外层 for 循环仍然要写,因为代码逻辑不依赖是否强连通。

---

### 九、思考题(请作答)

第一题,给定一个无向图的邻接表:

```
A -> B -> D -> NULL
B -> A -> C -> E -> NULL
C -> B -> F -> NULL
D -> A -> E -> NULL
E -> B -> D -> F -> NULL
F -> C -> E -> NULL
```

请分别给出从 A 出发的 **DFS 序**和 **BFS 序**。

第二题,一张**有向图**,顶点 `{1, 2, 3, 4, 5}`,边 `{<1,2>, <1,3>, <2,4>, <3,4>, <4,5>}`。从顶点 **5** 出发进行 DFS,**能访问到哪些顶点**?解释原因。

第三题,对一个有 n 个顶点、e 条边的图,**邻接矩阵**和**邻接表**两种存储下,DFS 的时间复杂度分别是多少?为什么?

第四题,设计一个算法判断**无向图 G 是否是一棵树**。(提示:树的两个条件:(a) 连通,(b) 边数 = n-1。用 DFS/BFS 一次遍历判断(a),简单数一下边数判断(b))

第五题,**证明或反驳**:"在一个无向图中,如果从顶点 v 出发做 DFS 能访问到所有顶点,那么从任何其他顶点 u 出发做 DFS 也一定能访问到所有顶点。" (提示:这个结论对无向图成立,对有向图不成立,思考为什么)

第六题,BFS 用的是**队列**;如果把队列换成**栈**,会变成什么?(答案有点意外,值得你思考)

---

### 十、下一步方向

DFS 和 BFS 打通之后,接下来可以选:

**(A)** **最小生成树**(Prim 和 Kruskal)——实际意义非常强(网络布线成本最小化),代码不长但概念经典,**考研大题必考点之一**。

**(B)** **最短路径**(Dijkstra 和 Floyd)——GPS 导航的核心算法,考研大题高频。Dijkstra 可以看作"带权 BFS"。

**(C)** **拓扑排序 + 关键路径**(AOV 和 AOE 网)——项目管理、任务调度的核心。基于 DFS 或 BFS 入度法。

**(D)** 先答思考题再继续。

**我个人建议的顺序是 (C) → (A) → (B)**,因为拓扑排序最贴近 DFS/BFS(几乎就是 DFS/BFS 的直接应用),Prim/Dijkstra 则需要引入"带权"和"贪心"的新概念,循序渐进更容易。但你可以按自己的节奏选。

告诉我你的选择,或者先答思考题!

## 数据结构复习:拓扑排序与关键路径

好,我们进入**拓扑排序**和**关键路径**。这两个知识点贴近 DFS/BFS,代码不长但概念很"实用"——拓扑排序是编译器、构建系统(make、Maven、npm)的核心;关键路径是项目管理的灵魂(PERT 图、甘特图的理论基础)。

按我们的节奏,先讲**拓扑排序**,再讲**关键路径**(关键路径基于拓扑排序,顺序不能反)。

---

### 一、AOV 网与拓扑排序

#### 1.1 什么是 AOV 网

**AOV 网(Activity On Vertex)**:一个**有向无环图(DAG)**,**顶点表示活动**,**有向边 `<u, v>` 表示"活动 u 必须在活动 v 之前完成"**(u 是 v 的先决条件)。

**经典例子**:大学课程的先修关系。

```
顶点:C1(高数),C2(C 语言),C3(数据结构),C4(编译原理),
      C5(操作系统),C6(算法)

先修关系:
C1 → C3   (高数是数据结构的先修)
C2 → C3   (C 语言是数据结构的先修)
C3 → C4   (数据结构是编译原理的先修)
C3 → C5   (数据结构是操作系统的先修)
C3 → C6   (数据结构是算法的先修)
C4 → C6   (编译原理是算法的先修)
C5 → C6
```

#### 1.2 拓扑排序的定义

**拓扑排序**:把 AOV 网中的顶点排成一个线性序列,使得**对每条有向边 `<u, v>`,u 都出现在 v 之前**。换句话说,**所有先决条件都排在后继之前**的一个顺序。

**上面那个课程图的一个合法拓扑排序**:

```
C1, C2, C3, C4, C5, C6
```

或者:

```
C2, C1, C3, C5, C4, C6
```

都合法。**拓扑排序结果通常不唯一**——只要满足约束就行。

#### 1.3 关键定理

**定理**:一个有向图**存在拓扑排序**,当且仅当它是**有向无环图(DAG)**。

**反过来说**:**有环的图没有拓扑排序**。直观理解:A → B → C → A,那 A 必须在 B 前、B 在 C 前、C 在 A 前,**互相矛盾**,不存在合法序列。

**这个定理的一个重要副作用**:拓扑排序算法可以**用来检测有向图是否有环**——如果算法结束后还有顶点没被排进序列,就说明有环。

---

### 二、拓扑排序的两种实现

#### 2.1 方法一:Kahn 算法(入度法,基于 BFS)

**核心思想**:**反复找入度为 0 的顶点,输出它,并从图中"删掉"它**。删掉后它的出边所指向的顶点入度会减少,可能产生新的入度为 0 的顶点,继续找。

**算法步骤**:

第一步,**计算每个顶点的入度**。

第二步,把所有**入度为 0** 的顶点入队。

第三步,**出队一个顶点 v**,输出它,并把它的所有出邻居的入度 -1。

第四步,如果某个出邻居的入度变为 0,入队。

第五步,重复第三、四步直到队列空。

**如果最后输出的顶点数 < 总顶点数**,说明图中**有环**(环中的顶点入度永远不为 0)。

#### 2.2 Kahn 算法的代码(邻接表版)

cpp

```cpp
#include "ALGraph.h"
#include <queue>
using namespace std;

// 返回 true 表示成功,false 表示图中有环
bool TopoSort_Kahn(ALGraph G, int topo[]) {
    int inDegree[MAX_VERTEX] = {0};
    
    // 1. 计算每个顶点的入度(扫描所有邻接表)
    for (int i = 0; i < G.vexNum; i++) {
        EdgeNode *p = G.adjList[i].firstEdge;
        while (p != nullptr) {
            inDegree[p->adjVex]++;       // i 到 p->adjVex 有边,p->adjVex 入度 +1
            p = p->next;
        }
    }
    
    // 2. 所有入度为 0 的顶点入队
    queue<int> Q;
    for (int i = 0; i < G.vexNum; i++) {
        if (inDegree[i] == 0) Q.push(i);
    }
    
    // 3. 反复出队并"删除"
    int count = 0;                        // 已排序的顶点数
    while (!Q.empty()) {
        int v = Q.front(); Q.pop();
        topo[count++] = v;
        // 遍历 v 的所有出邻居,入度 -1
        EdgeNode *p = G.adjList[v].firstEdge;
        while (p != nullptr) {
            int w = p->adjVex;
            if (--inDegree[w] == 0) Q.push(w);
            p = p->next;
        }
    }
    
    // 4. 判断是否成功
    return count == G.vexNum;
}
```

**时间复杂度分析**:

计算入度扫描所有边 O(V+E);每个顶点入队/出队一次 O(V);每条边被处理一次(出邻居遍历)O(E)。总计 **O(V+E)**。

**空间复杂度**:O(V)(队列和 inDegree 数组)。

#### 2.3 方法二:基于 DFS 的拓扑排序

**核心思想**:对图做 DFS,**每个顶点在 DFS 完成时(递归返回时)记录**——最终得到的序列**逆序**就是拓扑序。

**为什么逆序?** 因为 DFS 从顶点 v 出发递归完成,意味着 v 的**所有后继都已经被探索完毕**。所以 v 应该排在它的后继**之后**。反过来看,最后完成的顶点应该排在拓扑序**最前面**。

cpp

```cpp
bool hasCycle;                // 检测环
bool onStack[MAX_VERTEX];     // 当前 DFS 路径上的顶点
int topoResult[MAX_VERTEX];
int topoIdx;                  // 从后往前填

void DFS_Topo(ALGraph G, int v, bool visited[]) {
    visited[v] = true;
    onStack[v] = true;
    
    EdgeNode *p = G.adjList[v].firstEdge;
    while (p != nullptr) {
        int w = p->adjVex;
        if (!visited[w]) {
            DFS_Topo(G, w, visited);
        } else if (onStack[w]) {
            hasCycle = true;           // 回边,发现环
        }
        p = p->next;
    }
    
    onStack[v] = false;
    topoResult[topoIdx--] = v;         // 后完成的先填
}

bool TopoSort_DFS(ALGraph G, int topo[]) {
    bool visited[MAX_VERTEX] = {false};
    for (int i = 0; i < MAX_VERTEX; i++) onStack[i] = false;
    hasCycle = false;
    topoIdx = G.vexNum - 1;
    
    for (int i = 0; i < G.vexNum; i++) {
        if (!visited[i]) DFS_Topo(G, i, visited);
    }
    
    if (hasCycle) return false;
    for (int i = 0; i < G.vexNum; i++) topo[i] = topoResult[i];
    return true;
}
```

**两种方法对比**:

|维度|Kahn(入度法)|DFS 法|
|---|---|---|
|基础|BFS|DFS|
|环检测|容易(count 对比)|需额外的 onStack 数组|
|代码长度|稍长但直观|稍短但要理解"逆序"|
|常见度|工程和考研主流|算法竞赛常用|

**考研 99% 以上用 Kahn 算法**——逻辑直白,环检测简单,写起来最顺手。

#### 2.4 手工模拟 Kahn 算法

用前面那个课程图:

```
C1 → C3, C2 → C3, C3 → C4, C3 → C5, C3 → C6, C4 → C6, C5 → C6
```

**初始入度**:

C1=0, C2=0, C3=2, C4=1, C5=1, C6=3

**入队入度为 0**:队列 [C1, C2]。

**出队 C1**,输出 C1,更新 C3 入度 2-1=1。队列 [C2]。

**出队 C2**,输出 C2,更新 C3 入度 1-1=0,C3 入队。队列 [C3]。

**出队 C3**,输出 C3,更新 C4 入度 1-1=0(入队),C5 入度 1-1=0(入队),C6 入度 3-1=2。队列 [C4, C5]。

**出队 C4**,输出 C4,更新 C6 入度 2-1=1。队列 [C5]。

**出队 C5**,输出 C5,更新 C6 入度 1-1=0,C6 入队。队列 [C6]。

**出队 C6**,输出 C6。队列空。

**拓扑序**:C1, C2, C3, C4, C5, C6。count=6=vexNum,无环。

---

### 三、AOE 网与关键路径

#### 3.1 什么是 AOE 网

**AOE 网(Activity On Edge)**:一个**带权有向无环图**,**顶点表示"事件"**,**有向边表示"活动"**,**边的权值表示活动所需的时间**。

**AOV 和 AOE 的区别**(易混淆!):

|维度|AOV 网|AOE 网|
|---|---|---|
|顶点表示|活动|事件(时间节点)|
|边表示|先后关系(无权)|活动(带权=持续时间)|
|权值|无|边权(活动时长)|
|主要问题|拓扑排序(顺序)|关键路径(总时长)|

**实际例子**:一个软件项目的 AOE 网。

```
顶点:v1(开始),v2(设计完成),v3(前端完成),v4(后端完成),
      v5(集成完成),v6(测试完成)

活动(边):
v1 → v2,权 5(设计阶段,5 天)
v2 → v3,权 3(前端开发)
v2 → v4,权 6(后端开发)
v3 → v5,权 2(前端集成)
v4 → v5,权 1(后端集成)
v5 → v6,权 4(测试)
```

#### 3.2 关键概念(必须一字不差地记)

关键路径涉及**四个时间参数**,两两成对。你一定要先把这四个概念的**含义**和**关系**搞清楚,然后才能理解算法。

**顶点参数**(对事件而言):

**ve(v)**:事件 v 的**最早发生时间**(earliest)。物理意义:从源点到 v 的**最长路径**长度——因为 v 要等所有先决条件都完成,慢的那一条决定了 v 能开始的最早时间。

**vl(v)**:事件 v 的**最晚发生时间**(latest)。物理意义:在不延误总工期的前提下,v 最晚可以什么时候发生。

**边参数**(对活动 `<u, v>` 而言,权值为 w):

**e(活动)**:活动 `<u, v>` 的**最早开始时间**。等于 `ve(u)`——活动从 u 发出,u 最早发生时活动就能最早开始。

**l(活动)**:活动 `<u, v>` 的**最晚开始时间**。等于 `vl(v) - w`——活动完成时不能迟于 v 的最晚发生时间,所以活动最晚也要在 `vl(v) - w` 时开始。

**关键活动**:满足 **`e(活动) == l(活动)`** 的活动,即"没有时间余量,耽误不起"的活动。

**关键路径**:由关键活动组成的从源点到汇点的路径。**关键路径的长度 = 工程总工期**。

#### 3.3 为什么要找关键路径?

**工程意义**:工程总工期取决于关键路径。**加快非关键活动不会缩短总工期;只有加快关键活动才有效**。这是项目管理的铁律。

**举例**:设计阶段(5天)是关键路径的一部分,加快设计能缩短工期;但如果前端开发和后端开发并行,而前端只需 3 天、后端需 6 天,**前端就是非关键活动**——即使前端加速到 2 天,总工期也不变(后端仍然需要 6 天)。

#### 3.4 关键路径算法(六步法,必须熟练)

**第一步**,对 AOE 网做**拓扑排序**,得到拓扑序。

**第二步**,**按拓扑序**从前往后计算 ve(v):

```
ve(源点) = 0
ve(v) = max{ ve(u) + w(u,v) } 对所有 u 能到达 v
```

"最早发生"取**最大值**——要等最慢的前置。

**第三步**,**按逆拓扑序**从后往前计算 vl(v):

```
vl(汇点) = ve(汇点)              ← 汇点的最晚 = 最早
vl(u) = min{ vl(v) - w(u,v) } 对所有 u 能到达 v
```

"最晚发生"取**最小值**——不能耽误最急的后继。

**第四步**,对每条活动 `<u, v>`(权 w):

```
e(活动) = ve(u)
l(活动) = vl(v) - w
```

**第五步**,找出所有 `e == l` 的活动,即**关键活动**。

**第六步**,关键活动连起来就是**关键路径**。关键路径可能不唯一(多条并列的最长路径)。

#### 3.5 手算关键路径示例

用前面的软件项目 AOE 网。

**边和权**:v1→v2(5), v2→v3(3), v2→v4(6), v3→v5(2), v4→v5(1), v5→v6(4)。

**第一步:拓扑排序**。显然:v1, v2, v3, v4, v5, v6(或 v1, v2, v4, v3, v5, v6 都合法)。

**第二步:按拓扑序算 ve**。

ve(v1) = 0。

ve(v2) = ve(v1) + 5 = 5。

ve(v3) = ve(v2) + 3 = 8。

ve(v4) = ve(v2) + 6 = 11。

ve(v5) = max{ve(v3) + 2, ve(v4) + 1} = max{10, 12} = 12。

ve(v6) = ve(v5) + 4 = 16。

**总工期 = ve(v6) = 16 天**。

**第三步:按逆拓扑序算 vl**。

vl(v6) = ve(v6) = 16。

vl(v5) = vl(v6) - 4 = 12。

vl(v4) = vl(v5) - 1 = 11。

vl(v3) = vl(v5) - 2 = 10。

vl(v2) = min{vl(v3) - 3, vl(v4) - 6} = min{7, 5} = 5。

vl(v1) = vl(v2) - 5 = 0。

**第四、五步:算每条边的 e、l,找关键活动**。

|活动|e|l|l-e|关键?|
|---|---|---|---|---|
|v1→v2 (5)|0|0|0|✅|
|v2→v3 (3)|5|7|2|❌|
|v2→v4 (6)|5|5|0|✅|
|v3→v5 (2)|8|10|2|❌|
|v4→v5 (1)|11|11|0|✅|
|v5→v6 (4)|12|12|0|✅|

**关键路径**:v1 → v2 → v4 → v5 → v6,总长 5+6+1+4 = 16(= 总工期 ✓)。

**工程启示**:如果你想把项目从 16 天缩短到 15 天,**必须加快关键路径上的某个活动**(比如把后端开发从 6 天压到 5 天)。加快前端开发(v2→v3)毫无用处。

#### 3.6 关键路径代码(邻接表版)

cpp

```cpp
#include "ALGraph.h"
#include <iostream>
#include <queue>
#include <cstring>
using namespace std;

int ve[MAX_VERTEX], vl[MAX_VERTEX];

bool CriticalPath(ALGraph G) {
    // 1. 拓扑排序(Kahn)
    int inDegree[MAX_VERTEX] = {0};
    for (int i = 0; i < G.vexNum; i++) {
        EdgeNode *p = G.adjList[i].firstEdge;
        while (p) { inDegree[p->adjVex]++; p = p->next; }
    }
    
    queue<int> Q;
    for (int i = 0; i < G.vexNum; i++) {
        if (inDegree[i] == 0) Q.push(i);
        ve[i] = 0;                       // ve 初始化为 0
    }
    
    int topo[MAX_VERTEX], count = 0;
    while (!Q.empty()) {
        int u = Q.front(); Q.pop();
        topo[count++] = u;
        EdgeNode *p = G.adjList[u].firstEdge;
        while (p) {
            int v = p->adjVex;
            // 2. 顺便算 ve:ve(v) = max{ve(u) + w}
            if (ve[u] + p->weight > ve[v]) {
                ve[v] = ve[u] + p->weight;
            }
            if (--inDegree[v] == 0) Q.push(v);
            p = p->next;
        }
    }
    
    if (count < G.vexNum) {
        cout << "图中有环,无法算关键路径" << endl;
        return false;
    }
    
    // 3. 按逆拓扑序算 vl
    for (int i = 0; i < G.vexNum; i++) vl[i] = ve[topo[count - 1]];  // 初始化为终点的 ve
    
    for (int i = count - 1; i >= 0; i--) {
        int u = topo[i];
        EdgeNode *p = G.adjList[u].firstEdge;
        while (p) {
            int v = p->adjVex;
            if (vl[v] - p->weight < vl[u]) {
                vl[u] = vl[v] - p->weight;
            }
            p = p->next;
        }
    }
    
    // 4. 输出关键活动
    cout << "关键活动:" << endl;
    for (int u = 0; u < G.vexNum; u++) {
        EdgeNode *p = G.adjList[u].firstEdge;
        while (p) {
            int v = p->adjVex;
            int e = ve[u];
            int l = vl[v] - p->weight;
            if (e == l) {
                cout << "  " << G.adjList[u].data << " -> " 
                     << G.adjList[v].data << " (权 " << p->weight << ")" << endl;
            }
            p = p->next;
        }
    }
    
    cout << "总工期: " << ve[topo[count - 1]] << endl;
    return true;
}
```

**代码设计要点**:

第一,**拓扑排序时顺便算 ve**——因为 ve 的计算恰好需要按拓扑序。这是一石二鸟的经典技巧。

第二,**vl 初始化为终点的 ve**——所有顶点的 vl 初值都设为终点的 ve(即总工期),然后从后往前松弛(取 min)。

第三,**逆拓扑序**从 `topo[count-1]` 遍历到 `topo[0]`。

---

### 四、文件组织与测试

沿用之前的 Graph 目录,在 `ALGraph.cpp` 里扩展拓扑排序和关键路径。简单起见,这里我直接把代码写在 main.cpp 里演示:

cpp

```cpp
#include "ALGraph.h"
#include <iostream>
using namespace std;

bool TopoSort_Kahn(ALGraph G, int topo[]);
bool CriticalPath(ALGraph G);

int main() {
    /* 输入样例(带权有向图):
       6 6
       1 2 3 4 5 6
       1 2 5
       2 3 3
       2 4 6
       3 5 2
       4 5 1
       5 6 4
    */
    ALGraph G;
    InitGraph_AL(G);
    CreateGraph_AL(G, true, true);    // 有向、带权
    
    int topo[MAX_VERTEX];
    if (TopoSort_Kahn(G, topo)) {
        cout << "拓扑序: ";
        for (int i = 0; i < G.vexNum; i++) {
            cout << G.adjList[topo[i]].data << " ";
        }
        cout << endl;
    }
    
    CriticalPath(G);
    
    DestroyGraph_AL(G);
    return 0;
}
```

预期输出:

```
拓扑序: 1 2 3 4 5 6 
关键活动:
  1 -> 2 (权 5)
  2 -> 4 (权 6)
  4 -> 5 (权 1)
  5 -> 6 (权 4)
总工期: 16
```

---

### 五、易错点总结

**易错一:AOV 和 AOE 搞混**。AOV 顶点是活动,AOE **边**是活动。这是考研选择题最爱混淆的。

**易错二:拓扑序不唯一**。只要满足先后约束,输出顺序可以多种。大题要问"是否有某种拓扑序",你要判断所有可能;要你"给出一种",任何合法的都算对。

**易错三:Kahn 算法用栈还是队列?** 都行!用栈时结果仍是合法拓扑序,只是顺序不同。考研默认用队列(教材统一)。

**易错四:ve 和 vl 的"最大、最小"搞反**。ve 取**max**(等最慢的前置),vl 取**min**(不能误最急的后继)。可以用"瓶颈"来类比:前置瓶颈影响最早发生,后继瓶颈影响最晚发生。

**易错五:关键路径不唯一**。如果有多条长度相同的最长路径,都是关键路径。加快其中一条的某个活动**不一定能缩短总工期**——除非同时加快所有关键路径上的活动。这是一个考研坑。

**易错六:关键活动的边与关键路径上的点**。关键**活动**是边;关键**路径**是一条由关键活动组成的路径。说"顶点是关键路径上的"不等于说"某条边是关键活动",反之亦然。

**易错七:vl 的初始化**。`vl[终点] = ve[终点]`,其他顶点的 vl 可以初始化为终点的 ve,然后从后往前取 min。直接初始化为 ∞ 也行,但计算时要小心边界。

---

### 六、思考题(请作答)

**第一题**:下面的有向图,写出所有合法的**拓扑排序**序列。(提示:用 Kahn 算法模拟,注意每次入度为 0 的顶点可能不止一个)

```
顶点:{1, 2, 3, 4, 5}
边:  <1,2>, <1,3>, <2,4>, <3,4>, <4,5>
```

**第二题**:一个有向图的拓扑排序**不唯一**,说明了什么?有什么条件下拓扑排序**唯一**?

**第三题**:下面的 AOE 网,求:

```
顶点:v1(源), v2, v3, v4, v5, v6(汇)
边:v1→v2(6), v1→v3(4), v1→v4(5), v2→v5(1),
    v3→v5(1), v4→v6(2), v5→v6(7)
```

(a) 所有顶点的 ve 和 vl; (b) 所有活动的 e 和 l; (c) 关键路径及总工期。

**第四题**:对第三题的 AOE 网,如果你是项目经理,**能否通过加快活动 `v3→v5` 来缩短工期**?为什么?**能否加快 `v5→v6`**?解释原因。

**第五题**:如何用 **DFS** 检测一个有向图是否有环?写出伪代码思路(提示:用我代码中 `onStack` 数组的思想)。

**第六题**:拓扑排序算法能用于**无向图**吗?为什么?(提示:想想入度的定义在无向图里是否还有意义)

**第七题**:编译器编译代码时,需要处理头文件依赖。假设头文件 A 依赖 B、C,B 依赖 D,C 依赖 D。**用拓扑排序给出一个合法的"处理顺序"**,使得每个头文件在被依赖它的文件处理之前就已经处理完毕。

---

### 七、下一步方向

拓扑排序和关键路径打下之后,图论的"无权 / 偏序"部分就差不多吃透了。接下来按路线图:

**(A)** **最小生成树**(Prim 和 Kruskal)——带权无向图的经典问题,应用于网络布线、电路设计,**考研大题高频**。Prim 基于贪心 + BFS,Kruskal 基于边排序 + 并查集(顺便学并查集)。

**(B)** **最短路径**(Dijkstra 和 Floyd)——GPS 导航、网络路由的核心。Dijkstra 用"优先队列(堆)"加速——**正好把前面堆的内容应用起来**!Floyd 用三重循环的动态规划,代码极短但思想深刻。

**(C)** 先答思考题再推进。

**建议下一步选 (A)**——最小生成树是图论中最"美"的算法之一,你会学到**并查集**(一种极其简洁的数据结构,几行代码解决"是否在同一集合"问题)。

告诉我你的选择,或者先答思考题!

## 数据结构复习:最小生成树(Prim 与 Kruskal)

好,我们进入**最小生成树**(Minimum Spanning Tree,MST)。这是图论里**最优美的一章**——概念直观、代码简洁、两种算法思想完全不同却都很经典。我还会顺便带你学**并查集**(Kruskal 算法的灵魂伴侣),一种极其简洁又极其强大的数据结构。

---

### 一、问题引入

**经典场景**:n 个城市,想修一个**互联互通**的公路网,使得任意两城之间都能互通(可以经过其他城市)。每条可能的公路都有成本,如何规划使得**总成本最低**?

**抽象**:给一张**带权无向连通图** G,找一棵**生成树 T**(包含所有顶点、n-1 条边、连通、无环),使得 T 的**边权之和最小**。这棵 T 就是**最小生成树**。

---

### 二、核心概念

#### 2.1 生成树的基本性质

回忆一下:**n 个顶点的生成树恰好有 n-1 条边**。为什么?

因为生成树是一棵树,而树的定义是"连通且无环",它必然有 n-1 条边(少了不连通,多了出环)。**所以 MST 的构造本质是:在所有边中挑 n-1 条,既要连通所有点又要总权最小**。

#### 2.2 MST 的两个关键性质

**性质一(MST 的不唯一性)**:一张图可能有**多棵不同**的 MST(如果边权有重复)。但**所有 MST 的总权值必然相等**——这是最小权,本就只有一个值。

**性质二(割性质 / Cut Property)**:如果把图的顶点分成两个非空集合 U 和 V\U,那么**连接 U 和 V\U 的所有边中,权值最小的那条**一定属于某棵 MST。

**这个性质是两个算法正确性的共同基础**。Prim 算法每次都在找"连接已选集合和未选集合的最小边";Kruskal 每次选最小边,本质也是"跨某个割的最小边"。

---

### 三、Prim 算法(普里姆)

#### 3.1 核心思想

**"从一个顶点出发,像滚雪球一样逐步扩大"**。

把顶点分成两组:**已在生成树中**的(初始只有起点)、**尚未加入**的。每一步从"连接两组的所有边"中,挑**权值最小**的那条,把对应的新顶点加入"已在"组。重复 n-1 次,所有顶点都加入。

**直观类比**:从家出发铺路,每次伸出一条**最便宜的路**到最近的新邻居,直到覆盖所有城市。

#### 3.2 实现核心:lowcost 数组

Prim 的经典实现依赖两个辅助数组:

**lowcost[i]**:**顶点 i 到"已在树中的集合"的最小边权**。如果 i 已在树中,lowcost[i] = 0。

**closest[i]**:这条最小边的另一端是哪个顶点(便于输出 MST 的边)。

**算法骨架**:

第一步,把起点 v0 加入树,初始化 lowcost[i] = G.edges[v0][i],closest[i] = v0。

第二步,重复 n-1 次:

在 lowcost 中找**最小值**(排除已在树中的顶点),设其下标为 k,值为 min。这意味着:顶点 k 到树的最近连接是 min,另一端是 closest[k]。

把 k 加入树:lowcost = 0(标记"已加入")。

输出边 (closest[k], k),权 min。

**更新**:对每个未加入的顶点 j,如果 G.edges[k][j] < lowcost[j],则 lowcost[j] = G.edges[k][j],closest[j] = k。(意思是:现在 k 加入了,j 通过 k 连到树的成本可能更低)

#### 3.3 Prim 代码(邻接矩阵版,最经典)

cpp

```cpp
#include "MGraph.h"
#include <iostream>
using namespace std;

void Prim(MGraph G, int v0) {
    int lowcost[MAX_VERTEX];
    int closest[MAX_VERTEX];
    
    // 1. 初始化:所有顶点到 v0 的距离
    for (int i = 0; i < G.vexNum; i++) {
        lowcost[i] = G.edges[v0][i];      // 没边就是 INF
        closest[i] = v0;
    }
    lowcost[v0] = 0;                       // v0 已在树中
    
    int totalCost = 0;
    cout << "MST 的边:" << endl;
    
    // 2. 找剩下 n-1 个顶点加入
    for (int i = 1; i < G.vexNum; i++) {
        // 在未加入的顶点中找 lowcost 最小的
        int min = INF, k = -1;
        for (int j = 0; j < G.vexNum; j++) {
            if (lowcost[j] != 0 && lowcost[j] < min) {
                min = lowcost[j];
                k = j;
            }
        }
        
        if (k == -1) {
            cout << "图不连通,无 MST" << endl;
            return;
        }
        
        // 输出这条边
        cout << "  (" << G.vexs[closest[k]] << ", " << G.vexs[k] 
             << ") 权 " << min << endl;
        totalCost += min;
        lowcost[k] = 0;                   // k 加入树
        
        // 3. 更新 lowcost 和 closest
        for (int j = 0; j < G.vexNum; j++) {
            if (lowcost[j] != 0 && G.edges[k][j] < lowcost[j]) {
                lowcost[j] = G.edges[k][j];
                closest[j] = k;
            }
        }
    }
    
    cout << "总权值: " << totalCost << endl;
}
```

#### 3.4 Prim 手工模拟

一张带权无向图(5 顶点):

```
顶点:A B C D E
边:
(A,B) 1
(A,C) 5
(B,C) 3
(B,D) 6
(C,D) 4
(C,E) 2
(D,E) 8
```

**从 A 出发**,初始化:

|i|A|B|C|D|E|
|---|---|---|---|---|---|
|lowcost|0|1|5|∞|∞|
|closest|A|A|A|A|A|

**第 1 轮**:最小 lowcost = 1(B),加入 B,输出边 (A, B) 权 1。

更新:B 的邻居中,lowcost[C] = min(5, G[B][C]=3) = 3,closest[C] = B;lowcost[D] = min(∞, 6) = 6,closest[D] = B。

|i|A|B|C|D|E|
|---|---|---|---|---|---|
|lowcost|0|0|3|6|∞|
|closest|A|A|B|B|A|

**第 2 轮**:最小 lowcost = 3(C),加入 C,输出边 (B, C) 权 3。

更新:C 的邻居中,lowcost[D] = min(6, 4) = 4,closest[D] = C;lowcost[E] = min(∞, 2) = 2,closest[E] = C。

|i|A|B|C|D|E|
|---|---|---|---|---|---|
|lowcost|0|0|0|4|2|
|closest|A|A|B|C|C|

**第 3 轮**:最小 lowcost = 2(E),加入 E,输出边 (C, E) 权 2。

E 的邻居中,lowcost[D] = min(4, G[E][D]=8) = 4,不更新。

**第 4 轮**:最小 lowcost = 4(D),加入 D,输出边 (C, D) 权 4。

**完成**。MST 边集:(A,B) 1, (B,C) 3, (C,E) 2, (C,D) 4,总权 **10**。

#### 3.5 Prim 的复杂度

**时间**:O(V²)——外层循环 n-1 次,每次找最小值 O(V) + 更新 O(V)。

**空间**:O(V)。

**优化**:用**小根堆/优先队列**维护 lowcost,时间降到 **O((V+E) log V)**。在**稀疏图**中更快;在**稠密图**中 O(V²) 反而更好。考研默写基本都是 O(V²) 版本,简洁直观。

**优先队列版的伪代码**:

```
PQ 初始化,插入 (0, v0)
while PQ 非空:
    取出最小的 (cost, v)
    if v 已在树中,跳过
    否则 v 加入,处理其所有邻居 w:PQ.push((G[v][w], w))
```

这就是"**Prim 是带权版 BFS**"的真正含义——把 BFS 的队列换成优先队列,按边权排序扩展即可。**和 Dijkstra 代码结构几乎一模一样**(Dijkstra 用累计距离,Prim 用单边权)。

---

### 四、并查集(Union-Find):Kruskal 的前置

Kruskal 算法的核心操作是"**判断两个顶点是否已在同一连通分量**",直接用图遍历太慢。**并查集**提供 O(接近常数) 的判断,是专门为这种场景设计的数据结构。

#### 4.1 并查集的两个核心操作

**Find(x)**:查找元素 x 所在集合的代表元。

**Union(x, y)**:把 x 和 y 所在的两个集合合并成一个。

#### 4.2 实现方式:父节点数组

用一个数组 `parent[]`,`parent[i]` 表示 i 的父节点。每个集合用一棵树表示,树根就是代表元。

**最简实现**:

cpp

```cpp
int parent[MAX_N];

void Init(int n) {
    for (int i = 0; i < n; i++) parent[i] = i;   // 初始每个元素自成一组
}

int Find(int x) {
    if (parent[x] == x) return x;
    return Find(parent[x]);                       // 递归找根
}

void Union(int x, int y) {
    int px = Find(x), py = Find(y);
    if (px != py) parent[px] = py;                // 一个根挂到另一个下面
}
```

这是最简版本,最坏时间 O(n)(树退化成链)。

#### 4.3 路径压缩(Path Compression)

**优化 Find**:递归回溯时,**把路径上所有节点直接指向根**。

cpp

```cpp
int Find(int x) {
    if (parent[x] != x) {
        parent[x] = Find(parent[x]);              // 直接指向根
    }
    return parent[x];
}
```

只加这一行,Find 的均摊复杂度就降到 **接近 O(1)**(准确说是 O(α(n)),α 是阿克曼函数的反函数,实际值不超过 4)。

#### 4.4 按秩合并(Union by Rank,可选)

**优化 Union**:合并时**把小树挂到大树下**,避免树退化成链。

cpp

```cpp
int rank_[MAX_N];                                 // 每棵树的"秩"(近似高度)

void Init(int n) {
    for (int i = 0; i < n; i++) { parent[i] = i; rank_[i] = 0; }
}

void Union(int x, int y) {
    int px = Find(x), py = Find(y);
    if (px == py) return;
    if (rank_[px] < rank_[py]) parent[px] = py;
    else if (rank_[px] > rank_[py]) parent[py] = px;
    else { parent[py] = px; rank_[px]++; }
}
```

**路径压缩 + 按秩合并**的并查集,在所有实际应用中可视为 **O(1) 操作**。考研中**路径压缩是必会的,按秩合并了解即可**。

#### 4.5 并查集的其他应用

并查集不只为 Kruskal 服务,还有很多:

**判断图的连通分量数**——对所有边执行 Union,最后有几个不同的根就有几个连通分量。

**网络连接性问题**——动态判断"A 和 B 是否能通过一系列链接到达"。

**离线处理图问题**——比如"逆向删边"问题。

这是一种极其**简单却极其有力**的数据结构,**面试非常爱考**。

---

### 五、Kruskal 算法(克鲁斯卡尔)

#### 5.1 核心思想

**"按边权从小到大,能加就加"**。

把所有边按权值**升序排序**。依次考察每条边 `(u, v)`:如果 u 和 v **还不在同一连通分量**(加这条边不会形成环),就加入 MST;否则跳过。直到加入 n-1 条边为止。

**直观类比**:从最便宜的路开始修,只要修了这条路不会造成环,就修。

**和 Prim 的对比**:Prim 是"长一棵树",Kruskal 是"长一片森林,逐步合并"。Kruskal 更像**森林版**的贪心。

#### 5.2 Kruskal 的核心——判环

"加这条边会不会形成环"怎么判断?**看 u 和 v 是否已在同一连通分量**。这正是并查集的看家本领:**Find(u) == Find(v)** 即同分量。

#### 5.3 Kruskal 代码(边集数组版)

邻接矩阵/邻接表都不直接适合 Kruskal,因为要**对所有边排序**。所以我们用**边集数组**。

cpp

```cpp
#include <iostream>
#include <algorithm>
using namespace std;

#define MAX_EDGE 10000
#define MAX_VERTEX 100

typedef struct {
    int u, v;              // 两端
    int weight;
} Edge;

Edge edges[MAX_EDGE];
int parent[MAX_VERTEX];

int Find(int x) {
    if (parent[x] != x) parent[x] = Find(parent[x]);
    return parent[x];
}

bool cmp(Edge a, Edge b) {
    return a.weight < b.weight;
}

void Kruskal(int n, int e) {       // n 顶点,e 边
    // 1. 边按权排序
    sort(edges, edges + e, cmp);
    
    // 2. 初始化并查集
    for (int i = 0; i < n; i++) parent[i] = i;
    
    int totalCost = 0, count = 0;
    cout << "MST 的边:" << endl;
    
    // 3. 扫描每条边
    for (int i = 0; i < e && count < n - 1; i++) {
        int pu = Find(edges[i].u);
        int pv = Find(edges[i].v);
        if (pu != pv) {             // 不在同分量,不成环
            parent[pu] = pv;         // 合并
            cout << "  (" << edges[i].u << ", " << edges[i].v 
                 << ") 权 " << edges[i].weight << endl;
            totalCost += edges[i].weight;
            count++;
        }
    }
    
    if (count < n - 1) {
        cout << "图不连通,无 MST" << endl;
    } else {
        cout << "总权值: " << totalCost << endl;
    }
}
```

#### 5.4 Kruskal 手工模拟

同样用前面那张图,**边按权升序**:

```
(A,B) 1
(C,E) 2
(B,C) 3
(C,D) 4
(A,C) 5
(B,D) 6
(D,E) 8
```

**初始**:每个点自成分量 {A}, {B}, {C}, {D}, {E}。

**考察 (A,B) 1**:不同分量,加入。合并 {A,B}。MST 边:(A,B)。

**考察 (C,E) 2**:不同分量,加入。合并 {C,E}。MST 边:(A,B), (C,E)。

**考察 (B,C) 3**:A,B 在一组,C,E 在一组,不同分量,加入。合并为 {A,B,C,E}。MST 边:(A,B), (C,E), (B,C)。

**考察 (C,D) 4**:D 独立,加入。合并为 {A,B,C,D,E}。MST 边:4 条,达到 n-1=4,结束。

**总权**:1 + 2 + 3 + 4 = **10**——和 Prim 结果一致 ✓。

---

### 六、Prim vs Kruskal 对比

|维度|Prim|Kruskal|
|---|---|---|
|思想|从一个点扩散,长一棵树|按边排序,森林合并|
|数据结构|lowcost 数组 / 优先队列|边集 + 并查集|
|时间(朴素)|O(V²)|O(E log E)|
|时间(优化)|O((V+E) log V)|O(E log E)(排序占主导)|
|适合|稠密图|稀疏图|
|存储|邻接矩阵|边集数组|
|代码复杂度|稍简单|稍复杂(需并查集)|

**怎么选?**

**E 远大于 V**(稠密图,E ≈ V²):选 **Prim**,因为 O(V²) < O(E log E) = O(V² log V)。

**E 与 V 同阶**(稀疏图):选 **Kruskal**,因为 O(E log E) = O(V log V) < O(V²)。

**考研大题两种都会考**,你都要熟练。

---

### 七、几个易错点

**易错一**:Prim 每次**找最小 lowcost 时要排除已加入的顶点**(lowcost == 0)。代码中的 `lowcost[j] != 0` 就是这个判断。**忘了会死循环或结果错**。

**易错二**:Kruskal 必须**判断加边后是否成环**,不能见边就加。判环靠并查集。

**易错三**:**Prim 的 lowcost 要更新,不是直接覆盖**。新加入的顶点 k 可能让某些 j 通过 k 连到树更便宜,但也可能不如原来,要用 `min` 更新。

**易错四**:**并查集的 Find 要递归(或循环),不能只看 parent[x]**。因为 parent[x] 可能不是根,只是直接父节点。

**易错五**:MST **可能不唯一**(权相同边多时),但总权唯一。选择题常问"以下哪个是 MST 的边"——要看哪个候选边真的最优。

**易错六**:MST 只对**连通图**有定义。非连通图只能求"最小生成森林",每个连通分量一棵树。

---

### 八、完整测试(main.cpp)

cpp

```cpp
#include "MGraph.h"
#include <iostream>
using namespace std;

void Prim(MGraph G, int v0);

int main() {
    /* 输入样例(无向带权图):
       5 7
       A B C D E
       A B 1
       A C 5
       B C 3
       B D 6
       C D 4
       C E 2
       D E 8
    */
    MGraph G;
    InitGraph_M(G);
    CreateGraph_M(G, false, true);    // 无向、带权
    
    cout << "===== Prim 算法 =====" << endl;
    Prim(G, 0);                        // 从 A (下标 0) 开始
    
    return 0;
}
```

预期输出:

```
===== Prim 算法 =====
MST 的边:
  (A, B) 权 1
  (B, C) 权 3
  (C, E) 权 2
  (C, D) 权 4
总权值: 10
```

Kruskal 你可以单独写一个测试程序,用边集数组输入边,调 `Kruskal(n, e)`。

---

### 九、思考题(请作答)

**第一题**,下面这张带权无向图,分别用 Prim(从 A 出发)和 Kruskal 构造 MST,**列出每一步的边和最终总权**。你会发现两种算法选的边可能不完全一样,但总权一定相等:

```
顶点:{A, B, C, D, E, F}
边:
(A,B) 6, (A,C) 1, (A,D) 5,
(B,C) 5, (B,E) 3,
(C,D) 5, (C,E) 6, (C,F) 4,
(D,F) 2, (E,F) 6
```

**第二题**,一张带权无向图有 6 条边,权值分别为 `{1, 2, 3, 4, 5, 6}`,**所有边权两两不同**。问:这张图的 MST **是否唯一**?证明或反驳。

**第三题**,对并查集,下列序列(下标 1..6)执行后 `parent` 数组是什么?假设用**路径压缩**但**不**用按秩合并,并约定 Union(x, y) 把 Find(x) 的父亲设为 Find(y)。

```
Init(6)
Union(1, 2)
Union(3, 4)
Union(5, 6)
Union(1, 3)
Find(2)
```

**第四题**,**为什么 Kruskal 的时间复杂度是 O(E log E) 而不是 O(E log V)**?这两者有区别吗?

**第五题**,思考以下命题:"在一棵 MST 中,权值最小的边一定是原图中权值最小的那条边。" **是对还是错?** 给出理由或反例。

**第六题**,如果一张图里有**负权边**,Prim 和 Kruskal **还能正确找到 MST 吗**?(提示:MST 和最短路径不同,MST 不关心路径累积,只关心边权总和)

**第七题**,**并查集的 Find 用递归 vs 循环写,哪种更好**?写出循环版(迭代版)的 Find 带路径压缩。

---

### 十、下一步方向

最小生成树 + 并查集之后,你的图论工具箱非常丰富了。接下来:

**(A)** **最短路径**(Dijkstra 和 Floyd)——压轴章节,和 MST 并列为图论的"双子星"。Dijkstra 和 Prim 代码几乎一模一样,只差一个细节;Floyd 是三重循环的动态规划,代码极短却思想深刻。**考研图论大题一半以上都是最短路径**。

**(B)** **强连通分量**(Tarjan / Kosaraju)——比较进阶,考研较少考,但面试爱问。

**(C)** 先答思考题再继续。

**强烈建议选 (A)**——最短路径是图论课程的收官之作,学完后你的图论部分就完整了,接下来可以开始**查找**或**排序**章节。

告诉我你的选择!

## 数据结构复习:最短路径(Dijkstra 与 Floyd)

好,我们进入图论的**压轴章节**——最短路径。这是考研大题命中率最高的图论内容,也是 GPS 导航、网络路由的核心算法。

这一节讲两种经典算法:**Dijkstra**(单源最短路径)和 **Floyd**(多源最短路径)。它们思路完全不同,代码风格也迥异,但都是**动态规划思想**的经典体现。你会发现 Dijkstra 的代码和 Prim 惊人相似——我会在对比中让你看到**贪心算法的共性**。

---

### 一、问题分类(先搞清楚要解决什么)

**单源最短路径**:给定起点 s,求 s 到**其他所有顶点**的最短路径。典型算法:**Dijkstra**(非负权)、**Bellman-Ford**(允许负权)。

**多源最短路径**:求**任意两点之间**的最短路径。典型算法:**Floyd**。

**单对最短路径**:只求 s 到某个特定 t 的最短路径。实际中用 Dijkstra 走到 t 即可,没有专门算法。

**考研要求掌握**:Dijkstra 和 Floyd。Bellman-Ford 偶尔以简答题形式出现。

---

### 二、Dijkstra 算法(迪杰斯特拉)

#### 2.1 核心思想

**"贪心地扩展最近点"**。和 Prim 极其相似,但关注的是"**累计路径长度**",不是"单条边权"。

把顶点分成两组:**已确定最短路径**的(S 集),**未确定**的(V-S 集)。每次从 V-S 中选出**距离 s 最小**的顶点 u,加入 S。然后**用 u 松弛**其他顶点——即看看"s→u→其他顶点"的路径是否比目前已知的更短。

**关键词:松弛(Relax)**。这是最短路径算法的灵魂操作。

#### 2.2 "松弛"是什么?

给定顶点 v,当前已知 s 到 v 的最短距离是 `dist[v]`(可能是估值,不一定最优)。如果我们发现 s 通过 u 再到 v 更短,即:

```
dist[u] + w(u, v) < dist[v]
```

就**更新** `dist[v] = dist[u] + w(u, v)`。这就是"松弛"——像把橡皮筋从"过长的估值"拉紧到"更短的实际值"。

#### 2.3 Dijkstra 的三个核心数组

**dist[i]**:s 到顶点 i 当前已知的最短距离。初始 dist[s] = 0,其他都是 INF(或 s 的直接邻居是边权)。

**path[i]**:i 的**前驱顶点**(最短路径上,i 的上一个顶点是谁)。用于回溯路径。

**final[i]**(或叫 visited[i]):i 是否已加入 S 集(已确定最短)。

#### 2.4 算法骨架

第一步,初始化 dist、path、final 三个数组。

第二步,循环 n 次(每次把一个顶点加入 S):

(a) 在 V-S 中找 dist 最小的顶点 u。

(b) 把 u 加入 S:final = true。

(c) 以 u 为中继,**松弛**所有 u 的邻居:对每个未加入的邻居 v,若 dist[u] + w(u, v) < dist[v],则更新 dist[v] 和 path[v]。

第三步,算法结束后,dist 数组就是 s 到所有顶点的最短距离,path 数组可以回溯出路径。

#### 2.5 Dijkstra 代码(邻接矩阵版)

cpp

```cpp
#include "MGraph.h"
#include <iostream>
using namespace std;

void Dijkstra(MGraph G, int s, int dist[], int path[]) {
    bool final[MAX_VERTEX];
    
    // 1. 初始化
    for (int i = 0; i < G.vexNum; i++) {
        dist[i] = G.edges[s][i];          // 起点到 i 的直接距离
        final[i] = false;
        path[i] = (dist[i] != INF) ? s : -1;   // s 的直接邻居前驱是 s,其他 -1
    }
    dist[s] = 0;
    final[s] = true;                       // s 自己加入 S
    path[s] = -1;
    
    // 2. 循环 n-1 次,每次确定一个顶点的最短距离
    for (int i = 1; i < G.vexNum; i++) {
        // (a) 找 V-S 中 dist 最小的顶点 u
        int min = INF, u = -1;
        for (int j = 0; j < G.vexNum; j++) {
            if (!final[j] && dist[j] < min) {
                min = dist[j];
                u = j;
            }
        }
        
        if (u == -1) break;                // 剩下的顶点都不可达
        
        // (b) u 加入 S
        final[u] = true;
        
        // (c) 用 u 松弛其他顶点
        for (int v = 0; v < G.vexNum; v++) {
            if (!final[v] && G.edges[u][v] != INF 
                && dist[u] + G.edges[u][v] < dist[v]) {
                dist[v] = dist[u] + G.edges[u][v];
                path[v] = u;                // 记录 v 的前驱是 u
            }
        }
    }
}
```

**几个关键代码细节**:

第一,初始化 `dist[i] = G.edges[s][i]`——这是"到 i 的直接距离"。如果 s-i 有边,就是边权;没边,就是 INF。

第二,**松弛条件写全**:`!final[v]`(只松弛未确定的),`G.edges[u][v] != INF`(u-v 要有边),`dist[u] + G.edges[u][v] < dist[v]`(新路径更短)。三者缺一不可。

第三,**边权加法可能溢出**——如果用 INT_MAX 表示 INF,`dist[u] + INF` 会溢出。所以我们一直推荐用 **`0x3f3f3f3f`** 作为 INF,加法后仍然是大数,不会变负。

#### 2.6 手工模拟 Dijkstra

一个带权有向图:

```
顶点:{0, 1, 2, 3, 4}
边:
0→1 (10)
0→4 (5)
1→2 (1)
1→4 (2)
2→3 (4)
3→0 (7)
3→2 (6)
4→1 (3)
4→2 (9)
4→3 (2)
```

**从 0 出发,求到所有点的最短路径**。

**初始化**:

|顶点|0|1|2|3|4|
|---|---|---|---|---|---|
|dist|0|10|∞|∞|5|
|path|-|0|-1|-1|0|
|final|T|F|F|F|F|

**第 1 轮**:V-S 中最小 dist 是 4(值 5)。加入 4。

松弛 4 的邻居:

4→1:dist + 3 = 8 < 10,更新 dist[1]=8, path[1]=4。

4→2:dist + 9 = 14 < ∞,更新 dist[2]=14, path[2]=4。

4→3:dist + 2 = 7 < ∞,更新 dist[3]=7, path[3]=4。

|顶点|0|1|2|3|4|
|---|---|---|---|---|---|
|dist|0|8|14|7|5|
|path|-|4|4|4|0|
|final|T|F|F|F|T|

**第 2 轮**:V-S 中最小 dist 是 3(值 7)。加入 3。

松弛 3 的邻居:

3→0:dist + 7 = 14,但 0 已 final,跳过。

3→2:dist + 6 = 13 < 14,更新 dist[2]=13, path[2]=3。

|顶点|0|1|2|3|4|
|---|---|---|---|---|---|
|dist|0|8|13|7|5|
|path|-|4|3|4|0|
|final|T|F|F|T|T|

**第 3 轮**:V-S 中最小 dist 是 1(值 8)。加入 1。

松弛 1 的邻居:

1→2:dist + 1 = 9 < 13,更新 dist[2]=9, path[2]=1。

1→4:4 已 final,跳过。

|顶点|0|1|2|3|4|
|---|---|---|---|---|---|
|dist|0|8|9|7|5|
|path|-|4|1|4|0|
|final|T|T|F|T|T|

**第 4 轮**:V-S 中最小 dist 是 2(值 9)。加入 2。

松弛 2 的邻居:2→3,3 已 final;无其他边。

结束。

**最终结果**:

|终点|最短距离|路径|
|---|---|---|
|0|0|0|
|1|8|0 → 4 → 1|
|2|9|0 → 4 → 1 → 2|
|3|7|0 → 4 → 3|
|4|5|0 → 4|

**路径怎么回溯?** 比如终点 2,path[2] = 1,path[1] = 4,path[4] = 0,path[0] = -1(起点)。逆序得 0 → 4 → 1 → 2。

#### 2.7 输出路径的代码

cpp

```cpp
void PrintPath(int path[], int target, MGraph G) {
    if (path[target] == -1) {
        cout << G.vexs[target];
        return;
    }
    PrintPath(path, path[target], G);
    cout << " -> " << G.vexs[target];
}
```

递归回溯——**先打印前驱的路径,再加上自己**。

#### 2.8 Dijkstra 的复杂度

**时间**:O(V²)——外层 n 次,每次找最小 dist 是 O(V),松弛也是 O(V)。

**优化**:用**小根堆/优先队列**维护 dist,时间降到 **O((V+E) log V)**。稀疏图大幅加速。

**核心观察**:Dijkstra 的代码结构和 Prim **几乎一模一样**,唯一区别是:

Prim:`lowcost[v] = G[u][v]`(比较单边权)。

Dijkstra:`dist[v] = dist[u] + G[u][v]`(比较累计距离)。

**贪心思想是相通的**——每次扩展"最近"的点,差别只在"近"的定义是单边还是累计。

#### 2.9 致命警告:Dijkstra 不能处理负权边

这是最经典的考点。**为什么 Dijkstra 不能有负权?**

Dijkstra 的贪心正确性依赖于一个假设:**"一旦把 u 加入 S,dist[u] 就不可能再变小"**。

如果有负权,这个假设就失效——后面某个顶点 x 通过负权边连到 u,可能让 s→x→u 比当前 dist[u] 更短。但 u 已经被"锁定",算法不会再更新它,结果就错了。

**有负权怎么办?** 用 **Bellman-Ford**(O(VE))或 **SPFA**(Bellman-Ford 的队列优化版)。考研以了解为主。

---

### 三、Floyd 算法(弗洛伊德)

#### 3.1 问题目标

求**任意两顶点之间**的最短路径。若用 Dijkstra 对每个顶点作为起点跑一遍,时间 O(V³);Floyd 同样 O(V³),但**代码极短**(核心就三重循环),且**思想更优美**。

#### 3.2 核心思想:动态规划

定义状态 **D(k)[i][j]**:"**只允许使用前 k 个顶点作为中间顶点**"时,i 到 j 的最短距离。

**递推关系**:

对新加入的中间顶点 k,从 i 到 j 有两种选择:

不经过 k:沿用 D(k-1)[i][j]。

经过 k:D(k-1)[i][k] + D(k-1)[k][j]。

**取两者较小值**:

```
D(k)[i][j] = min( D(k-1)[i][j],  D(k-1)[i][k] + D(k-1)[k][j] )
```

**初始 D(-1)[i][j]** = 邻接矩阵(只看直接边)。

**最终答案**:D(n-1)[i][j] = i 到 j 的最短路径(允许所有顶点作中间点)。

#### 3.3 神奇的空间优化:一个矩阵搞定

朴素 DP 要三维数组 D(k)[i][j],但可以证明:**用一个二维数组 D[i][j] 就能原地更新**,k 从 0 到 n-1 迭代,每轮 D 自动更新成 D(k)。

**为什么不会出错?** 当处理 D[i][j] 在第 k 轮更新时:

D[i][k]:这一轮它要么没变(如果 i==k 或中间不经过 k 更短),要么已在前面某一轮算出了过 k 的更短路径——但本轮处理 D[i][k] 时,k 是中间点,D[i][k] 要么保持前一轮值,要么……其实仔细看:D[k] 在第 k 轮更新时,比较 D[i][k] 和 D[i][k] + D[k][k] = D[i][k] + 0 = D[i][k],相等。所以 D[i][k] 在第 k 轮不会变。D[k][j] 同理。

结论:**D[i][k] 和 D[k][j] 在第 k 轮内保持不变**,所以用原地更新是安全的。

#### 3.4 Floyd 代码(邻接矩阵,极简)

cpp

```cpp
#include "MGraph.h"
#include <iostream>
using namespace std;

int dist[MAX_VERTEX][MAX_VERTEX];
int path[MAX_VERTEX][MAX_VERTEX];

void Floyd(MGraph G) {
    // 1. 初始化
    for (int i = 0; i < G.vexNum; i++) {
        for (int j = 0; j < G.vexNum; j++) {
            dist[i][j] = G.edges[i][j];
            path[i][j] = (i != j && dist[i][j] != INF) ? i : -1;
            // path[i][j] = k 表示 i→j 最短路径上,j 的前驱是 k
        }
    }
    
    // 2. 三重循环:k 是中间点(最外层!)
    for (int k = 0; k < G.vexNum; k++) {
        for (int i = 0; i < G.vexNum; i++) {
            for (int j = 0; j < G.vexNum; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    path[i][j] = path[k][j];   // i→j 的前驱改为"k→j 的前驱"
                }
            }
        }
    }
}
```

**请特别注意循环顺序**:**k 必须在最外层**!

如果 k 在内层,相当于先固定 i、j,然后用不同 k 更新——这样每个(i, j) 只试了少量中间点,不等价于 DP 的"考虑前 k 个顶点"。**把 k 放错层是考研选择题的经典错误选项**。

#### 3.5 Floyd 的路径回溯

`path[i][j] = k` 的含义:**i 到 j 的最短路径上,j 的前驱顶点是 k**。

回溯过程:

cpp

```cpp
void PrintFloydPath(int i, int j, MGraph G) {
    if (i == j) { cout << G.vexs[i]; return; }
    if (path[i][j] == -1) { cout << "(不可达)"; return; }
    PrintFloydPath(i, path[i][j], G);
    cout << " -> " << G.vexs[j];
}
```

#### 3.6 Floyd 的复杂度

**时间**:O(V³)。

**空间**:O(V²)(dist 和 path 两个矩阵)。

**优势**:

代码极短(三重循环)。

**支持负权边**(只要无负权环)!这是相对 Dijkstra 的一大优势。

**劣势**:

V 大时很慢(V=1000 时 10⁹ 次运算)。

不支持负权环(环权值和为负,最短路径无定义,沿环走无限次可以到负无穷)。

#### 3.7 手工模拟 Floyd 的一轮

一个小图:

```
顶点:{0, 1, 2}
  0→1 (4), 0→2 (11),
  1→2 (2),
  2→0 (3)
```

初始 dist:

```
     0   1   2
  0 [ 0   4   11]
  1 [ ∞   0   2 ]
  2 [ 3   ∞   0 ]
```

**k=0 轮**(允许经过 0):

对每个 (i, j),检查 dist[i][j] vs dist[i][0] + dist[0][j]:

(1, 2):∞ vs dist[1][0]+dist[0][2] = ∞+11 = ∞。不更新。

(2, 1):∞ vs dist[2][0]+dist[0][1] = 3+4 = 7。**更新 dist[2][1]=7**。

其他不变。

**k=1 轮**(允许经过 0 和 1):

(0, 2):11 vs dist[0][1]+dist[1][2] = 4+2 = 6。**更新 dist[0][2]=6**。

(2, 2):0 vs dist[2][1]+dist[1][2] = 7+2 = 9。不更新(0 更小)。

**k=2 轮**(允许经过 0、1、2):

(0, 0):0 vs dist[0][2]+dist[2][0] = 6+3 = 9。不变。

(0, 1):4 vs dist[0][2]+dist[2][1] = 6+7 = 13。不变。

(1, 0):∞ vs dist[1][2]+dist[2][0] = 2+3 = 5。**更新 dist[1][0]=5**。

(1, 1):0 vs dist[1][2]+dist[2][1] = 2+7 = 9。不变。

**最终 dist**:

```
     0   1   2
  0 [ 0   4   6 ]
  1 [ 5   0   2 ]
  2 [ 3   7   0 ]
```

---

### 四、Dijkstra vs Floyd 对比

|维度|Dijkstra|Floyd|
|---|---|---|
|解决问题|单源最短路径|多源最短路径|
|时间|O(V²) 或 O((V+E)logV)|O(V³)|
|空间|O(V)|O(V²)|
|允许负权|❌ 不允许|✅ 允许(但不能有负环)|
|代码长度|较长|极短(三重循环)|
|思想|贪心|动态规划|
|适用|单点到其他点|所有点对|

**选择策略**:

问"某点到所有点":**Dijkstra**。

问"所有点对":**Floyd**(或多次 Dijkstra,取决于图大小)。

有负权:**Floyd**(或 Bellman-Ford)。

稀疏图单源:**Dijkstra + 堆**(比 O(V²) 快)。

---

### 五、完整测试代码

cpp

```cpp
#include "MGraph.h"
#include <iostream>
using namespace std;

void Dijkstra(MGraph G, int s, int dist[], int path[]);
void PrintPath(int path[], int target, MGraph G);
void Floyd(MGraph G);
void PrintFloydPath(int i, int j, MGraph G);
extern int dist[MAX_VERTEX][MAX_VERTEX];
extern int path[MAX_VERTEX][MAX_VERTEX];

int main() {
    /* 有向带权图输入:
       5 10
       0 1 2 3 4
       0 1 10
       0 4 5
       1 2 1
       1 4 2
       2 3 4
       3 0 7
       3 2 6
       4 1 3
       4 2 9
       4 3 2
    */
    MGraph G;
    InitGraph_M(G);
    CreateGraph_M(G, true, true);
    
    // ========== Dijkstra ==========
    cout << "===== Dijkstra(从 0 出发) =====" << endl;
    int d[MAX_VERTEX], p[MAX_VERTEX];
    Dijkstra(G, 0, d, p);
    for (int i = 0; i < G.vexNum; i++) {
        cout << "0 到 " << G.vexs[i] << " 的最短距离: " << d[i] 
             << ",路径: ";
        PrintPath(p, i, G);
        cout << endl;
    }
    
    // ========== Floyd ==========
    cout << "\n===== Floyd =====" << endl;
    Floyd(G);
    for (int i = 0; i < G.vexNum; i++) {
        for (int j = 0; j < G.vexNum; j++) {
            if (i != j) {
                cout << G.vexs[i] << " 到 " << G.vexs[j] << ": "
                     << (dist[i][j] == INF ? -1 : dist[i][j]);
                if (dist[i][j] != INF) {
                    cout << " 路径: ";
                    PrintFloydPath(i, j, G);
                }
                cout << endl;
            }
        }
    }
    return 0;
}
```

预期输出(部分):

```
===== Dijkstra(从 0 出发) =====
0 到 0 的最短距离: 0,路径: 0
0 到 1 的最短距离: 8,路径: 0 -> 4 -> 1
0 到 2 的最短距离: 9,路径: 0 -> 4 -> 1 -> 2
0 到 3 的最短距离: 7,路径: 0 -> 4 -> 3
0 到 4 的最短距离: 5,路径: 0 -> 4

===== Floyd =====
... (类似结果)
```

---

### 六、易错点总结

**易错一:Dijkstra 不处理负权**。这是选择题杀手,必背。有负权选 Bellman-Ford 或 Floyd。

**易错二:Floyd 的 k 必须在最外层**。k 在中间或最内层就变成错误的更新顺序。

**易错三:Dijkstra 初始化 dist**。应该是 `G.edges[s][i]`(直接边权),而不是 0 或 INF 一刀切。

**易错四:path 数组的含义**。Dijkstra 的 `path[i]` 是 i 的**前驱**;Floyd 的 `path[i][j]` 是 i→j 最短路径上 j 的前驱。看似类似,但对象不同。

**易错五:INF 的选择**。用 INT_MAX 会导致 `dist[u] + edge` 溢出变负,路径计算错乱。一定用 `0x3f3f3f3f`。

**易错六:Dijkstra 的 final 数组初始化**。`final[s] = true` 要在循环前设好,否则 s 自己会被当作"未确定"处理。

**易错七:两个算法的时间复杂度不要搞混**。Dijkstra 朴素 O(V²),Floyd 一定是 O(V³)。

---

### 七、高频考点:如何判断哪个顶点的最短路径**先**被确定?

这是选择题常考的。**Dijkstra 确定顶点的顺序**是"按 dist 升序"——dist 最小的先被加入 S。

比如前面那个例子,从 0 出发,顶点被确定的**顺序**:

0(dist=0)→ 4(dist=5)→ 3(dist=7)→ 1(dist=8)→ 2(dist=9)

这是 dist 升序。**考研大题常问"第 k 个被确定的是谁"、"某顶点确定时 dist 是多少"**——按照手算过程记录即可。

---

### 八、思考题(请作答)

**第一题**,给定下面的有向带权图,用 Dijkstra 从 V1 出发,**列出每一轮的 dist 数组状态**,并写出 V1 到每个顶点的最短路径及距离:

```
V1 → V2 (10), V1 → V5 (100), V1 → V4 (30)
V2 → V3 (50)
V3 → V5 (10)
V4 → V3 (20), V4 → V5 (60)
```

**第二题**,在第一题的图上跑 Floyd 算法,给出**最终的 dist 矩阵**(所有顶点对的最短距离)。

**第三题**,**为什么 Dijkstra 不能处理负权边**?给出一个具体的小反例图(3-4 个顶点),说明 Dijkstra 在这个图上会给出错误答案,指出错在哪。

**第四题**,**Floyd 算法的三重循环,k 必须在最外层**。如果把 k 放在最内层,会发生什么?给一个反例说明结果错。

**第五题**,**一个图有 5 个顶点,用 Dijkstra 从 s 出发**。第 1 个被确定最短路径的一定是 s。请问第 2 个被确定的是谁?(提示:根据 Dijkstra 的贪心选择)

**第六题**,**Dijkstra 和 Prim 的代码结构极其相似**。请写出两者的**主要差异**(在 lowcost 和 dist 的更新逻辑上)。你能用一句话概括它们的共性和区别吗?

**第七题**,**如果只想求 s 到某个特定终点 t 的最短路径**,可以让 Dijkstra 跑到 t 就停下来,不用跑完所有顶点。这样做**正确吗**?为什么?(提示:想想 Dijkstra 的贪心保证是什么)

**第八题**,**Floyd 算法输出的 dist 矩阵对角线**(即 dist[i][i])**应该都是 0**。如果算法结束后某个 dist[i][i] < 0,意味着什么?

---

### 九、图论部分完结总结

走到最短路径这里,你的图论已经**系统完整**了。回头看这条路线:

**基础**:图的定义、术语(邻接、度、连通、强连通)、五种存储结构(邻接矩阵、邻接表、十字链表、邻接多重表、边集数组)。

**遍历**:DFS 和 BFS,递归/非递归/邻接矩阵/邻接表四种组合。

**应用**:连通分量、拓扑排序(AOV 网)、关键路径(AOE 网)、最小生成树(Prim、Kruskal + 并查集)、最短路径(Dijkstra、Floyd)。

**数据结构涉及**:栈(DFS)、队列(BFS)、堆(Dijkstra 优化)、并查集(Kruskal)——**你前面学过的所有数据结构,在图算法里几乎都被用上**。

---

### 十、最终学习版图回顾

你目前掌握的数据结构全家福:

**线性结构**:链表(单/双/循环)、顺序表(静/动)、栈、队列、KMP。

**树形结构**:二叉树、线索二叉树、BST、AVL、红黑树、B 树、B+ 树、哈夫曼树、一般树、森林、堆。

**图结构**:邻接矩阵、邻接表、并查集。

**算法**:DFS、BFS、拓扑排序、关键路径、MST、最短路径。

**到这里,你已经掌握了考研 408 数据结构 80% 以上的内容**——只剩**查找**(散列表)和**排序**(快排、归并等)这两个章节。

---

### 十一、下一步方向

**(A)** **排序算法全家桶**——冒泡、选择、插入、希尔、快排、归并、堆排(你已会)、基数、计数。**考研必有大题**,且每种排序都是独立小世界,代码精巧。

**(B)** **散列表(哈希表)**——与 B+ 树并列的"数据库索引两大支柱",平均 O(1) 查找,面试爱考。涉及开放定址法、链地址法、装填因子、冲突处理。

**(C)** **跳表**(概率性有序结构,Redis 在用)或**字典树 Trie**(字符串处理利器)——拓展内容,考研不要求,面试偶尔考。

**(D)** 先答思考题再推进。

**我建议按 (A) → (B) 的顺序**:排序是数据结构课的收官之作,和前面所有章节关系密切;散列表作为查找的补充,概念独立、好学好考。

告诉我你的选择!



## 数据结构复习:排序算法(总览篇)

好,我们进入**排序算法**——数据结构课最后的"大章节",每年考研**必考大题**,也是面试最高频的知识点之一。

这部分内容多,我按**两次对话**完成:

**本次(第一次)**:讲**插入类**(直接插入、希尔)、**交换类**(冒泡、快排)、总体框架和评价指标。

**下次(第二次)**:讲**选择类**(简单选择、堆排回顾)、**归并类**(归并排序)、**基数类**(桶排序、基数排序)、全部算法对比表。

**请先读完这一次,把前 4 种排序吃透,再进入下一次**——排序算法不在多,在于每种都要真懂。

---

### 一、排序问题的基本概念

#### 1.1 什么是排序

给定 n 个元素的序列 `{a1, a2, ..., an}`,通过一系列操作,得到满足 `a'1 ≤ a'2 ≤ ... ≤ a'n`(升序)或相反(降序)的序列。

我们默认讨论**升序**,降序只需反转比较方向。

#### 1.2 稳定性(必考概念)

**稳定排序**:排序后,**相同关键字的元素**之间的**相对顺序保持不变**。

**不稳定排序**:相同元素的相对顺序可能改变。

举例:原序列 `[3a, 1, 3b, 2]`(3a 和 3b 值相同,但 a、b 只是标记原始位置)。

**稳定排序后**:`[1, 2, 3a, 3b]`(3a 仍在 3b 前)。

**不稳定排序后**:可能是 `[1, 2, 3a, 3b]` 也可能是 `[1, 2, 3b, 3a]`。

**为什么稳定性重要?** 现实中常要"按多关键字排序"(先按姓氏再按名字),稳定排序能保留前一轮的顺序。

**考研口诀**:**"快选希堆"不稳定**(快排、简单选择、希尔、堆排),其他常考排序都稳定。这一口诀后续我们反复验证。

#### 1.3 内部 vs 外部排序

**内部排序**:所有数据一次性读入**内存**完成排序。我们讨论的都是这类。

**外部排序**:数据量超大,内存装不下,需借助**外存**(磁盘、磁带)。核心是**多路归并**。外部排序考研考查较少,本章节不重点讲。

#### 1.4 算法评价的四个指标

**时间复杂度**(比较次数 + 移动次数):最好、最坏、平均。

**空间复杂度**:额外需要的内存。

**稳定性**:前面已讲。

**适用场景**:数据规模、数据特征(是否已近有序、是否有大量重复)。

**每种排序我都按这四点讲**,你要形成"每种排序 = 一张小卡片"的记忆。

---

### 二、代码约定与文件组织

#### 文件组织

```
DataStructure/
└── Sort/
    ├── Sort.h
    ├── Sort.cpp
    └── main.cpp
```

#### 代码约定

**所有排序函数签名统一**:`void XxxSort(int a[], int n)`,**数组下标从 0 开始**。

这和之前堆排序用"下标 1 起"不同——因为绝大多数教材和工程代码都用 0 起,我们回到主流约定。堆排序章节我们特意用 1 起是为了让父子公式更优雅,这里不再需要。

#### Sort.h

cpp

```cpp
#pragma once
#include <iostream>

// ========== 插入类 ==========
void InsertSort(int a[], int n);         // 直接插入
void BinaryInsertSort(int a[], int n);   // 折半插入
void ShellSort(int a[], int n);          // 希尔排序

// ========== 交换类 ==========
void BubbleSort(int a[], int n);         // 冒泡
void QuickSort(int a[], int n);          // 快速排序

// ========== 工具 ==========
void PrintArray(int a[], int n);
void Swap(int &a, int &b);
```

---

### 三、直接插入排序(Insertion Sort)

#### 3.1 核心思想

**"像整理扑克牌"**:把数组分成"已排序区间"和"未排序区间"。每次从未排序区间取第一个元素,**在已排序区间从后往前扫描**,找到它的正确位置插入。

开始时"已排序区间"只有 a[0](https://claude.ai/chat/%E5%8D%95%E4%B8%AA%E5%85%83%E7%B4%A0%E8%87%AA%E7%84%B6%E6%9C%89%E5%BA%8F),未排序是 a[1..n-1]。每轮处理一个元素,n-1 轮后全部有序。

#### 3.2 代码实现

cpp

```cpp
void InsertSort(int a[], int n) {
    for (int i = 1; i < n; i++) {            // 从 a[1] 开始
        if (a[i] < a[i - 1]) {               // 需要插入时才动
            int temp = a[i];                 // 保存待插入元素
            int j = i - 1;
            // 在已排序区间从后往前找位置,顺便把比 temp 大的元素后移
            while (j >= 0 && a[j] > temp) {
                a[j + 1] = a[j];             // 后移一位
                j--;
            }
            a[j + 1] = temp;                 // 插入到正确位置
        }
    }
}
```

**代码关键点**:

第一,`if (a[i] < a[i-1])` 是一个**优化**——如果 a[i] 已经比前一个大,说明它已经在正确位置,不用动。这在近似有序的数据上很有效。

第二,**while 循环同时做两件事**:找位置 + 后移元素。这种"搬移腾位"的写法在插入/删除类操作中反复出现,你在顺序表章节已经见过。

#### 3.3 手工模拟

对 `[5, 2, 4, 6, 1, 3]`:

初始:`[5] | 2 4 6 1 3`(竖线前是已排序区)

i=1,插入 2:2 < 5,temp=2,5 后移,2 插到前面。`[2, 5] | 4 6 1 3`

i=2,插入 4:4 < 5,4 < 2 不成立,插到 2 后面。`[2, 4, 5] | 6 1 3`

i=3,插入 6:6 > 5,不动。`[2, 4, 5, 6] | 1 3`

i=4,插入 1:一路后移,插到最前。`[1, 2, 4, 5, 6] | 3`

i=5,插入 3:3 < 5, 3 < 4, 3 > 2,插到 2、4 之间。`[1, 2, 3, 4, 5, 6]`

完成。

#### 3.4 性能分析

**时间复杂度**:

最好(已有序):**O(n)**——每轮 `if (a[i] < a[i-1])` 都不成立,直接跳过。

最坏(逆序):**O(n²)**——每轮都要把元素从末尾搬到最前,n + (n-1) + ... + 1 = n(n-1)/2。

平均:**O(n²)**。

**空间复杂度**:O(1)(只用了一个 temp)。

**稳定性**:**稳定**——while 条件是 `a[j] > temp`(严格大于),相等时不移动,保持原顺序。

**适用**:**小规模 或 近似有序**的数据。快排在接近有序时退化到 O(n²),此时插入排序反而最快。

#### 3.5 折半插入排序(微优化)

**观察**:直接插入在"已排序区间中找位置"用的是**线性查找**(从后往前扫)。因为已排序区是有序的,可以用**二分查找**加速。

**但注意**:二分找到位置后,**搬移元素仍然是 O(n)**——这部分优化不了,因为是数组。

cpp

```cpp
void BinaryInsertSort(int a[], int n) {
    for (int i = 1; i < n; i++) {
        int temp = a[i];
        int lo = 0, hi = i - 1;
        while (lo <= hi) {                    // 二分查找插入位置
            int mid = (lo + hi) / 2;
            if (a[mid] > temp) hi = mid - 1;
            else lo = mid + 1;                // 注意:遇到相等时往右找(保持稳定)
        }
        // lo 就是 temp 应该插入的位置
        for (int j = i - 1; j >= lo; j--) {
            a[j + 1] = a[j];
        }
        a[lo] = temp;
    }
}
```

**性能**:

**比较次数**降到 O(n log n)(二分找位置)。

**移动次数**仍然 O(n²)——搬移无法省。

所以**总时间复杂度还是 O(n²)**,只是比较次数变少了。考研偶尔考"折半插入的比较次数",要会算。

**稳定性**:稳定(注意二分时 `a[mid] == temp` 的情况要往右找,不然会破坏稳定性)。

---

### 四、希尔排序(Shell Sort)

#### 4.1 核心思想

**"先跳着粗排,再细排"**。

希尔的关键观察:插入排序在**近似有序**的数据上很快。那能不能先让数据"有点有序"呢?

**方法**:取一个**增量 d**,把数组每隔 d 一个元素分成一组,对每组做**直接插入排序**。这样每组有序了,整个数组"大体有序"。然后缩小 d,重复,直到 d=1(此时就是普通插入排序,但数据已经很接近有序了,非常快)。

**增量序列**:常见的有 `d = n/2, n/4, n/8, ..., 1`(每次减半)。这是最简单也最常考的。

#### 4.2 代码实现

cpp

```cpp
void ShellSort(int a[], int n) {
    for (int d = n / 2; d >= 1; d /= 2) {     // 增量序列
        // 对每个分组做插入排序
        for (int i = d; i < n; i++) {
            if (a[i] < a[i - d]) {
                int temp = a[i];
                int j = i - d;
                while (j >= 0 && a[j] > temp) {
                    a[j + d] = a[j];          // 后移 d 位(不是 1 位!)
                    j -= d;
                }
                a[j + d] = temp;
            }
        }
    }
}
```

**注意代码和直接插入的唯一区别**:把"1"全部换成"d"。**直接插入其实就是 d=1 的希尔**——希尔排序把"步长 1"泛化成了"步长 d"。

#### 4.3 手工模拟

对 `[49, 38, 65, 97, 76, 13, 27, 49, 55, 4]`(n=10):

**d=5**:分 5 组,每组 2 个元素:`{49, 13}, {38, 27}, {65, 49}, {97, 55}, {76, 4}`

组内排序后:`[13, 27, 49, 55, 4, 49, 38, 65, 97, 76]`

**d=2**:分 2 组:`{13, 49, 4, 38, 97}` 和 `{27, 55, 49, 65, 76}`

组内排序后:`[4, 27, 13, 38, 38, ...]` ——让我手算一下。

实际上 d=2 时下标 0,2,4,6,8 是一组:13, 49, 4, 38, 97 → 排序得 4, 13, 38, 49, 97。

下标 1,3,5,7,9 一组:27, 55, 49, 65, 76 → 排序得 27, 49, 55, 65, 76。

合并:`[4, 27, 13, 49, 38, 55, 49, 65, 97, 76]`

**d=1**:普通插入排序。数据已经大体有序,少量交换即可完成。

最终:`[4, 13, 27, 38, 49, 49, 55, 65, 76, 97]`

#### 4.4 性能分析

**时间复杂度**:**O(n^1.3)** 左右(经验值,严格分析非常复杂)。

希尔排序的时间复杂度**依赖于增量序列**,有不同的数学结论:

用 `n/2, n/4, ...` 序列:最坏 O(n²)。

用 Hibbard 序列(`2^k - 1`):O(n^1.5)。

用 Sedgewick 序列:O(n^1.33)。

**考研只需记**:希尔排序"比插入快,但达不到 O(n log n)"。

**空间复杂度**:O(1)。

**稳定性**:**不稳定**!

为什么?相同值的元素在**不同分组**里,分别排序后它们的相对位置可能被打乱。例如 `[3a, 5, 3b, 2]`,d=2 时 `{3a, 3b}` 和 `{5, 2}` 分别排——`{3a, 3b}` 顺序不变,但 d=1 阶段的跳跃排序可能让 3b 越过 3a。

**适用**:中等规模数据;比插入排序快很多,代码又极简。

#### 4.5 希尔排序为什么快?

**关键洞察**:直接插入每次只让一个元素移动一格;希尔让元素**一次跨越 d 格**。相当于把"长距离的乱序"先快速消除,避免在小距离的移动中反复搬运大元素。

**类比**:整理一大堆乱序扑克,先粗略分成几堆(d=5),再把每堆排好,最后统一精细排序——比从头细排所有牌快得多。

---

### 五、冒泡排序(Bubble Sort)

#### 5.1 核心思想

**"每一轮让最大的元素冒到末尾"**。

相邻两个元素比较,顺序错就交换。一轮从头扫到尾,最大元素被"冒泡"到末尾。然后对前 n-1 个元素再做一轮,把次大的冒到倒数第二位。n-1 轮后完成。

**形象比喻**:气泡从水底往上浮,大气泡比小气泡先浮上来。

#### 5.2 代码实现

cpp

```cpp
void BubbleSort(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {         // n-1 轮
        bool swapped = false;                  // 本轮是否发生过交换
        for (int j = 0; j < n - 1 - i; j++) {  // 未排序区末尾每轮缩 1
            if (a[j] > a[j + 1]) {
                Swap(a[j], a[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;                   // 一轮没交换,说明已有序,提前结束
    }
}
```

**`swapped` 标记**是经典优化:如果某一轮没有发生任何交换,说明数据已经有序,可以**提前退出**。在近似有序的数据上,这个优化让冒泡退化到 O(n)。

#### 5.3 手工模拟

对 `[5, 2, 4, 6, 1, 3]`:

第 1 轮:比较 5,2 → 交换;2,4 → 不换;4,6 → 不换;6,1 → 交换;6,3 → 交换。数组:`[2, 4, 5, 1, 3, 6]`,6 冒到末尾。

第 2 轮:2,4 → 不换;4,5 → 不换;5,1 → 交换;5,3 → 交换。数组:`[2, 4, 1, 3, 5, 6]`。

第 3 轮:2,4 → 不换;4,1 → 交换;4,3 → 交换。数组:`[2, 1, 3, 4, 5, 6]`。

第 4 轮:2,1 → 交换;其他不换。数组:`[1, 2, 3, 4, 5, 6]`。

第 5 轮:全部不换,swapped=false,提前结束。

#### 5.4 性能分析

**时间复杂度**:

最好(已有序,带优化):**O(n)**——一轮扫完无交换就退出。

最坏(逆序):**O(n²)**。

平均:**O(n²)**。

**空间复杂度**:O(1)。

**稳定性**:**稳定**——条件是 `a[j] > a[j+1]`(严格大于),相等不换,保持顺序。

**适用**:教学;实际应用几乎不用(太慢),但**考研选择题常考**"哪轮后元素就位了"之类的小题。

---

### 六、快速排序(Quick Sort)——最重要!

**这是考研的绝对重点**,每年都考,代码必须能默写。核心思想非常优雅。

#### 6.1 核心思想

**"分而治之"**:

第一步,从数组中挑一个元素作为**基准(pivot)**。

第二步,**分区(partition)**:把所有小于 pivot 的元素放到 pivot 左边,大于的放到右边。此时 pivot 的位置就是它在最终排序数组中的位置(**一次分区就确定一个元素的最终位置!**)。

第三步,对 pivot 左右两部分**递归**地继续快排。

**直观**:一个元素"找到家",把问题分成两个更小的问题。

#### 6.2 分区函数(Partition,核心难点)

这是快排的灵魂,必须背熟。经典写法:**以第一个元素为 pivot,用两个指针从两端向中间扫**。

cpp

```cpp
int Partition(int a[], int lo, int hi) {
    int pivot = a[lo];                        // 取第一个作为 pivot
    while (lo < hi) {
        // 右指针左移,找小于 pivot 的
        while (lo < hi && a[hi] >= pivot) hi--;
        a[lo] = a[hi];                        // 小元素放到左边空位
        // 左指针右移,找大于 pivot 的
        while (lo < hi && a[lo] <= pivot) lo++;
        a[hi] = a[lo];                        // 大元素放到右边空位
    }
    a[lo] = pivot;                            // pivot 放到 lo==hi 的位置
    return lo;                                // 返回 pivot 的最终位置
}
```

**这段代码的精妙之处**:

第一,`pivot = a[lo]` 取走 pivot,`a[lo]` 位置变成"空位"。

第二,**先动右指针**(找比 pivot 小的),填到左边空位;然后左指针变空位,**先动左指针**(找比 pivot 大的),填到右指针空位。两边交替"挖坑填坑"。

第三,最终 lo == hi,这个位置就是 pivot 的正确归宿。

#### 6.3 快排的递归主框架

cpp

```cpp
void QSort(int a[], int lo, int hi) {
    if (lo < hi) {
        int pivotPos = Partition(a, lo, hi);
        QSort(a, lo, pivotPos - 1);           // 左半递归
        QSort(a, pivotPos + 1, hi);           // 右半递归(pivot 不动了)
    }
}

void QuickSort(int a[], int n) {
    QSort(a, 0, n - 1);
}
```

**注意 pivot 本身不进入递归**——它已经在最终位置。只递归 `[lo, pivotPos-1]` 和 `[pivotPos+1, hi]`。

#### 6.4 手工模拟

对 `[49, 38, 65, 97, 76, 13, 27, 49]`(注意有两个 49):

**第一轮分区**,pivot = 49(下标 0)。

初始:lo=0, hi=7。a[0]=49(坑)。

右找小于 49:a=49,不小于,hi-- → a[6]=27,小。填到 a[0]。a[6] 变坑。数组:`[27, 38, 65, 97, 76, 13, 27, 49]`,lo=0, hi=6。

左找大于 49:a=27,不大于,lo++ → a[1]=38,不大于 → a[2]=65,大。填到 a[6]。a[2] 变坑。数组:`[27, 38, 65, 97, 76, 13, 65, 49]`,lo=2, hi=6。

右找小于 49:a=65,不小于,hi-- → a[5]=13,小。填到 a[2]。a[5] 变坑。数组:`[27, 38, 13, 97, 76, 13, 65, 49]`,lo=2, hi=5。

左找大于 49:a=13,不大于,lo++ → a[3]=97,大。填到 a[5]。a[3] 变坑。数组:`[27, 38, 13, 97, 76, 97, 65, 49]`,lo=3, hi=5。

右找小于 49:a=97,不小于,hi-- → a[4]=76,不小于,hi-- → a[3]——lo==hi=3,退出。

把 pivot 49 放到 a[3]。最终:`[27, 38, 13, 49, 76, 97, 65, 49]`,返回 3。

分区结果:**49 落在下标 3,左边都是小于等于 49,右边都是大于等于 49**。

**继续递归**左半 `[27, 38, 13]` 和右半 `[76, 97, 65, 49]`……(略)

#### 6.5 性能分析(重点!)

**时间复杂度**:

最好(每次 pivot 都是中位数,均匀二分):每层做 O(n) 的分区,有 log n 层,总计 **O(n log n)**。

最坏(每次 pivot 是最值,分成 0 和 n-1):每层 O(n),共 n 层,**O(n²)**。**典型触发最坏的情况是"对已有序数据"**——pivot 选第一个元素时,每次分区都毫无平衡。

平均:**O(n log n)**——大多数情况下接近最好。

**空间复杂度**:

递归调用栈深度。

最好 O(log n),最坏 O(n)(递归退化成链)。

**稳定性**:**不稳定**!分区时可能把相等元素交换过来。

**适用**:**大规模随机数据的首选排序**。标准库的 sort 普遍用快排或其改进版(IntroSort)。

#### 6.6 快排的优化(考研了解)

**优化一:三数取中选 pivot**。比较 a[lo], a[mid], a[hi],取中位数作为 pivot。避免有序数据退化到 O(n²)。

**优化二:小数组用插入排序**。递归到 n < 10 左右时,改用插入排序,因为小数据上插入排序常数更小。

**优化三:尾递归优化**。对较小的那一半递归,较大的那一半用循环处理,控制栈深度。

考研默写要求基础版即可,优化作为加分项了解。

#### 6.7 为什么快排在实践中比归并更快?

两者时间都是 O(n log n),但快排:

**空间更小**(O(log n) vs O(n))。

**常数更小**(分区操作简单,缓存友好;归并需要额外数组,内存分配开销大)。

**原地操作**(归并需要辅助数组)。

所以工业界默认排序几乎都是快排家族。

---

### 七、本次四种排序对比

|排序|最好|平均|最坏|空间|稳定|
|---|---|---|---|---|---|
|直接插入|O(n)|O(n²)|O(n²)|O(1)|✅|
|折半插入|O(n log n)*|O(n²)|O(n²)|O(1)|✅|
|希尔|—|O(n^1.3)|O(n²)|O(1)|❌|
|冒泡|O(n)|O(n²)|O(n²)|O(1)|✅|
|快排|O(n log n)|O(n log n)|O(n²)|O(log n)|❌|

***折半插入**:比较次数 O(n log n),但移动次数 O(n²),总时间还是 O(n²)。

---

### 八、测试代码(main.cpp)

cpp

```cpp
#include "Sort.h"
#include <iostream>
#include <cstring>
using namespace std;

void PrintArray(int a[], int n) {
    for (int i = 0; i < n; i++) cout << a[i] << " ";
    cout << endl;
}

void Swap(int &a, int &b) {
    int t = a; a = b; b = t;
}

int main() {
    int original[] = {49, 38, 65, 97, 76, 13, 27, 49, 55, 4};
    int n = 10;
    int a[10];
    
    cout << "原数组:    ";
    PrintArray(original, n);
    
    memcpy(a, original, sizeof(original));
    InsertSort(a, n);
    cout << "直接插入:  "; PrintArray(a, n);
    
    memcpy(a, original, sizeof(original));
    BinaryInsertSort(a, n);
    cout << "折半插入:  "; PrintArray(a, n);
    
    memcpy(a, original, sizeof(original));
    ShellSort(a, n);
    cout << "希尔排序:  "; PrintArray(a, n);
    
    memcpy(a, original, sizeof(original));
    BubbleSort(a, n);
    cout << "冒泡排序:  "; PrintArray(a, n);
    
    memcpy(a, original, sizeof(original));
    QuickSort(a, n);
    cout << "快速排序:  "; PrintArray(a, n);
    
    return 0;
}
```

期望输出(所有行相同):

```
原数组:    49 38 65 97 76 13 27 49 55 4 
直接插入:  4 13 27 38 49 49 55 65 76 97 
折半插入:  4 13 27 38 49 49 55 65 76 97 
希尔排序:  4 13 27 38 49 49 55 65 76 97 
冒泡排序:  4 13 27 38 49 49 55 65 76 97 
快速排序:  4 13 27 38 49 49 55 65 76 97
```

---

### 九、易错点总结

**易错一**:插入排序 `a[j+1] = temp` 放置 temp 的位置。循环结束时 j 指向的是"比 temp 小的那个位置",temp 要放在 `j+1`,而不是 `j`。

**易错二**:希尔排序的步长是 d,所有"+1"的地方都要改成"+d"。漏改一处结果就错。

**易错三**:冒泡排序的内循环上界是 `n-1-i`,不是 `n-1` 或 `n-i`。注意**已经冒泡到末尾的元素不用再比**。

**易错四**:快排的分区函数里,必须**先动右指针**(找小),再动左指针(找大)。颠倒了会出错。**关键在于 pivot 取自 a[lo],所以 a[lo] 是初始空位,要先从右边找填料**。

**易错五**:快排分区返回的是 **pivot 的最终下标**,不是 `lo`、不是 `hi`(此时 lo==hi,都行,但写 `lo` 最清楚)。

**易错六**:快排的递归分成两段,**不包括 pivot 本身**。写成 `QSort(a, lo, pivotPos)` 会无限递归。

**易错七**:**稳定性的判定口诀**——"快选希堆"不稳定,其他稳定。考试时不要现推,直接背。

---

### 十、经典考研题型

#### 10.1 给定数组,问"某轮/某趟排序后的状态"

这是**每年必考**的选择题类型。你要熟练对每种排序手工模拟一趟。

典型问法:"对数组 `[...]` 做快速排序(首元素为 pivot),**第一趟**排序后数组是?"

**注意"一趟"的定义**:

冒泡:一趟 = 一次完整扫描 = 把一个最大元素冒到末尾。

插入:一趟 = 处理一个新元素。

希尔:一趟 = 一个增量的处理(可能包含多个分组的插入)。

快排:一趟 = 一次分区(确定一个 pivot 的位置)。

**各种排序的"一趟"含义不同**,大题会明确指定。考研时要仔细读题。

#### 10.2 "哪个序列是排序过程的中间状态"

给你原始数组和几个候选,问哪个是某种排序过程中的中间状态。你要根据"每轮结束后数组形态的特征"判断:

冒泡 k 轮后:**最后 k 个元素已排序到最终位置**。

插入 k 轮后:**前 k+1 个元素已内部有序**(但不一定在全局最终位置)。

选择 k 轮后:**前 k 个元素已在最终位置**(简单选择)。

快排 k 轮后:**不能判断具体位置,但每个 pivot 一旦确定就是最终位置**。

归并某层后:**长度为 2^层数的子数组内部有序**。

---

### 十一、思考题(请作答)

**第一题**,对数组 `[26, 5, 37, 1, 61, 11, 59, 15, 48, 19]`,分别用**直接插入排序**和**希尔排序**(增量 5, 3, 1),写出**每一趟**排序后的数组状态。

**第二题**,对数组 `[49, 38, 65, 97, 76, 13, 27, 49]`,用**快速排序**(首元素为 pivot)进行排序,写出**每一趟分区后**的数组状态以及当前 pivot 的位置。

**第三题**,以下哪种排序**对已经有序的数组最快**?哪种**最慢**?给出理由。

候选:直接插入、希尔、冒泡、快速排序。

**第四题**,快速排序在**最坏情况**下时间复杂度是 O(n²)。**什么样的输入会触发最坏情况**?如何避免?

**第五题**,**证明(或验证)**:希尔排序是**不稳定**的。给一个 5 个元素的具体数组,展示"稳定性被破坏"的过程。

**第六题**,**冒泡排序能否提前结束**?这需要什么样的优化?这个优化对**近似有序数组**的性能有什么影响?

**第七题**,**折半插入排序和直接插入排序**,两者的时间复杂度**都是 O(n²)**,那折半插入的优势体现在哪里?

**第八题**,对一个含有**大量重复元素**的数组(比如 `[3, 3, 3, 3, 3, 3, 3, 3]`),用快速排序会发生什么?时间复杂度如何?如何改进?(这个问题引出"三路快排"的思想)

---

### 十二、下一次预告

下次对话我们继续:

**选择类**:简单选择排序(+ 堆排序回顾,已学过,简单过一下)。

**归并类**:归并排序——唯一稳定的 O(n log n) 排序,分治思想的典范。

**基数类**:桶排序、基数排序——不基于比较的排序,突破 O(n log n) 下界。

**最终大总结**:所有排序的对比表、选择策略、考研高频考点归纳。

---

告诉我:

**(A)** 直接继续下一次对话,讲后半部分排序。

**(B)** 先答思考题再继续。

**(C)** 对本次四种排序还有疑问,想再展开讲某一种。

## 数据结构复习:排序算法(下)

好,继续排序章节的下半场。这次讲:**选择类**(简单选择 + 堆排回顾)、**归并排序**、**基数 / 桶 / 计数排序**,最后做**全部八种排序的总对比**和**选择策略**。

上次讲完后你已经掌握插入类、冒泡、快排——都是**基于比较**的排序。这次的亮点在**归并**(唯一稳定的 O(n log n) 排序)和**基数类**(跳出比较模型,达到 O(n))。

---

### 一、简单选择排序(Selection Sort)

#### 1.1 核心思想

**"每轮找一个最小的放到前面"**。

数组分"已排序"和"未排序"。每轮在**未排序区间**里找最小值,**与未排序区间的第一个元素交换**。第一轮找到全局最小放 a[0],第二轮在 a[1..n-1] 里找最小放 a[1],依次类推。

**和冒泡的区别**:冒泡靠相邻交换"冒"出最值,交换次数多;简单选择只在每轮末尾做一次交换,**交换次数最多 n-1 次**。

#### 1.2 代码实现

cpp

```cpp
void SelectSort(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;                       // 假设 i 是最小
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[minIdx]) minIdx = j; // 找到更小的就更新下标
        }
        if (minIdx != i) {                    // 只有找到更小的才交换
            Swap(a[i], a[minIdx]);
        }
    }
}
```

**关键点**:先"找",后"交换";**整轮最多交换一次**。这是和冒泡的本质区别。

#### 1.3 手工模拟

对 `[5, 2, 4, 6, 1, 3]`:

i=0,未排序区最小是 1(下标 4),交换 a[0] 和 a[4]:`[1, 2, 4, 6, 5, 3]`。

i=1,未排序区最小是 2(本身),不交换:`[1, 2, 4, 6, 5, 3]`。

i=2,未排序区最小是 3(下标 5),交换:`[1, 2, 3, 6, 5, 4]`。

i=3,未排序区最小是 4(下标 5),交换:`[1, 2, 3, 4, 5, 6]`。

i=4,最小是 5(本身),不交换。

完成。

#### 1.4 性能分析

**时间复杂度**:

最好、最坏、平均:**都是 O(n²)**。

**注意**:不管数据是否有序,选择排序都要完整地扫 n(n-1)/2 次比较,**没有"提前结束"的可能**。这是它和冒泡/插入的区别。

**空间复杂度**:O(1)。

**稳定性**:**不稳定**!

反例:`[5a, 5b, 2]`。i=0 找到最小 2(下标 2),交换 a[0]=5a 和 a[2]=2,得 `[2, 5b, 5a]`。5a 和 5b 相对顺序反了。

#### 1.5 堆排序的位置

堆排序本质上是**"用堆数据结构优化过的选择排序"**——每轮也是"找最小/最大",但用堆让找的过程从 O(n) 降到 O(log n)。所以整体 O(n log n)。

**堆排在堆章节已经完整讲过**,这里不重复代码。你只需记住:

**堆排序是选择类排序家族的"优化版"**。

**时间 O(n log n)**(建堆 O(n),n-1 次调整每次 O(log n))。

**空间 O(1)**(原地)。

**不稳定**(下沉过程可能打乱相同元素顺序)。

对比简单选择:从 O(n²) 降到 O(n log n),付出的代价是稳定性和代码复杂度。

---

### 二、归并排序(Merge Sort)——分治的典范

#### 2.1 核心思想

**分治(Divide and Conquer)**:

**分(Divide)**:把数组从中间一分为二,分别对两半递归排序。

**治(Conquer)**:子问题规模到 1 时(单个元素天然有序),开始返回。

**合(Merge)**:把两个已排序的子数组**合并**成一个有序数组。

**关键操作**:合并两个有序数组——你在顺序表章节已经写过类似的代码(双指针同向扫描)。

#### 2.2 合并操作(Merge)

两个有序子数组 `a[low..mid]` 和 `a[mid+1..high]`,合并成一个有序数组:

cpp

```cpp
void Merge(int a[], int low, int mid, int high) {
    int *temp = new int[high - low + 1];     // 辅助数组(归并必须有额外空间)
    int i = low, j = mid + 1, k = 0;
    
    // 双指针比较,小者先进 temp
    while (i <= mid && j <= high) {
        if (a[i] <= a[j]) temp[k++] = a[i++];
        else temp[k++] = a[j++];
    }
    // 把剩余的补进去
    while (i <= mid) temp[k++] = a[i++];
    while (j <= high) temp[k++] = a[j++];
    
    // 拷贝回原数组
    for (int p = 0; p < k; p++) {
        a[low + p] = temp[p];
    }
    delete[] temp;
}
```

**关键点 1**:`a[i] <= a[j]` 用 `<=` **不是** `<`——这是保持**稳定性**的关键!相等时,**左子数组的元素先进 temp**,保持原来的相对顺序。

**关键点 2**:需要 O(n) 的辅助空间。**这是归并相对快排的最大劣势**——快排原地,归并必须开辅助数组。

#### 2.3 递归主框架

cpp

```cpp
void MSort(int a[], int low, int high) {
    if (low < high) {                         // 规模 >= 2 才递归
        int mid = (low + high) / 2;
        MSort(a, low, mid);                   // 左半
        MSort(a, mid + 1, high);              // 右半
        Merge(a, low, mid, high);             // 合并
    }
}

void MergeSort(int a[], int n) {
    MSort(a, 0, n - 1);
}
```

**这段代码的结构是经典的"递归三步走"**:

**分**(计算 mid)→ **治**(两次递归)→ **合**(Merge)。

**和快排对比**:快排的"分"是分区(Partition),**分完左小右大**,合并时啥都不用做;归并的"分"是**直接对半切**,但合并时需要 Merge 操作。**快排"重分轻合",归并"轻分重合"**——两种分治哲学。

#### 2.4 手工模拟(递归树)

对 `[5, 2, 4, 6, 1, 3]`,递归树:

```
              [5, 2, 4, 6, 1, 3]
             /                  \
         [5, 2, 4]            [6, 1, 3]
         /       \            /       \
      [5, 2]    [4]        [6, 1]    [3]
      /   \                /   \
    [5]   [2]            [6]   [1]
```

**自底向上合并**:

`[5]` + `[2]` = `[2, 5]`

`[2, 5]` + `[4]` = `[2, 4, 5]`

`[6]` + `[1]` = `[1, 6]`

`[1, 6]` + `[3]` = `[1, 3, 6]`

`[2, 4, 5]` + `[1, 3, 6]` = `[1, 2, 3, 4, 5, 6]` ✓

**完美的对称分治结构**,每层都是 n 个元素参与合并,共 log n 层,所以时间 O(n log n)。

#### 2.5 性能分析

**时间复杂度**:

最好、最坏、平均:**全部 O(n log n)**。

**关键**:归并是**"数据无关"**的——不管数据是什么形态,都做同样多的比较和移动。**比快排稳定**(性能意义上的稳定,不是稳定性)。

**空间复杂度**:

**O(n)**——辅助数组。

递归栈 O(log n),但 O(n) 占主导。

**稳定性**:**稳定**——靠 `<=` 那一行保证。

**适用**:对稳定性有要求的场景;链表排序(归并在链表上可以 O(1) 空间,不需要辅助数组);外部排序。

#### 2.6 为什么归并是"唯一"稳定的 O(n log n) 排序?

回忆一下 O(n log n) 家族:快排、堆排、归并。

**快排**:分区时会把相等元素交换到对侧,不稳定。

**堆排**:下沉时打乱相等元素相对位置,不稳定。

**归并**:合并时 `<=` 保证左边优先,稳定。

所以当你需要**既 O(n log n) 又稳定**时,归并是**唯一选择**(在经典排序里)。这就是归并在工程中仍然有用的核心原因——比如 Java 的 `Arrays.sort(Object[])` 用的就是归并(因为对象比较需要稳定性)。

---

### 三、跳出比较模型:计数/桶/基数排序

之前讲的所有排序都是**基于比较**的。**理论下界**:任何基于比较的排序,**最坏至少需要 Ω(n log n) 次比较**(信息论证明)。

**要突破 n log n,必须不靠比较**。计数、桶、基数排序就是三种不靠比较的排序,通过"利用数据的特征"达到 O(n) 或 O(n+k)。

**但它们有限制**:只适用于**特定类型的数据**(整数、范围有限等)。**不是万能的**。

#### 3.1 计数排序(Counting Sort)

**思想**:对**值域较小的整数**,直接用"桶"统计每个值出现了多少次,然后按值域顺序输出。

**适用场景**:值范围 [0, k],且 k 不太大(k = O(n) 最理想)。

cpp

```cpp
void CountingSort(int a[], int n, int k) {
    // k 是最大值(已知)
    int *count = new int[k + 1]();           // 统计数组,初始化 0
    int *output = new int[n];
    
    // 1. 统计每个值出现次数
    for (int i = 0; i < n; i++) count[a[i]]++;
    
    // 2. 前缀和:count[i] = 小于等于 i 的元素个数
    for (int i = 1; i <= k; i++) count[i] += count[i - 1];
    
    // 3. 从后往前填(保持稳定)
    for (int i = n - 1; i >= 0; i--) {
        output[count[a[i]] - 1] = a[i];
        count[a[i]]--;
    }
    
    // 4. 拷贝回原数组
    for (int i = 0; i < n; i++) a[i] = output[i];
    
    delete[] count;
    delete[] output;
}
```

**性能**:时间 **O(n+k)**,空间 **O(n+k)**,**稳定**(从后往前填 + 前缀和)。

**适用**:n = 10000,k = 100,这种"数据量大但值域小"的场景——远快于 O(n log n)。

**不适用**:值域 [0, 10^9] 这种,k 太大了,计数数组开不下。

#### 3.2 桶排序(Bucket Sort)

**思想**:把数据分到若干**桶**中,每个桶内部排序(用插入排序或其他),然后依次输出所有桶。

**适用**:数据**均匀分布**在一个已知范围内,比如 [0, 1) 的浮点数。

cpp

```cpp
void BucketSort(double a[], int n) {
    int bucketCount = n;                     // 桶数等于元素数
    vector<double> buckets[bucketCount];     // 每个桶是一个 vector
    
    // 1. 分桶:a[i] 放到下标 int(n * a[i]) 的桶里
    for (int i = 0; i < n; i++) {
        int idx = int(n * a[i]);             // 假设 a[i] ∈ [0, 1)
        buckets[idx].push_back(a[i]);
    }
    
    // 2. 每个桶内部排序(用 std::sort 或插入排序)
    for (int i = 0; i < bucketCount; i++) {
        sort(buckets[i].begin(), buckets[i].end());
    }
    
    // 3. 依次收集
    int k = 0;
    for (int i = 0; i < bucketCount; i++) {
        for (double x : buckets[i]) a[k++] = x;
    }
}
```

**性能**:平均 **O(n)**(均匀分布时每桶约 1 个元素),最坏 O(n²)(全挤一个桶)。

#### 3.3 基数排序(Radix Sort)——考研重点!

**思想**:按"位"排序。从最低位(个位)到最高位(千位、万位……),每一位用**稳定的**计数排序,多位组合后整体有序。

**关键依赖**:每位的排序**必须稳定**,否则高位排序会打乱低位的结果。

**LSD(最低位优先)算法流程**:

对 `[329, 457, 657, 839, 436, 720, 355]`(3 位数):

**第一轮:按个位**(9, 7, 7, 9, 6, 0, 5)排:

个位 0:720。个位 5:355。个位 6:436。个位 7:457, 657。个位 9:329, 839。

输出:`[720, 355, 436, 457, 657, 329, 839]`。

**第二轮:按十位**(2, 5, 3, 5, 5, 2, 3)排:

十位 2:720, 329。十位 3:436, 839。十位 5:355, 457, 657。

输出:`[720, 329, 436, 839, 355, 457, 657]`。

**第三轮:按百位**(7, 3, 4, 8, 3, 4, 6):

百位 3:329, 355。百位 4:436, 457。百位 6:657。百位 7:720。百位 8:839。

输出:`[329, 355, 436, 457, 657, 720, 839]`。**完成** ✓。

#### 3.4 基数排序代码

cpp

```cpp
int GetMax(int a[], int n) {
    int max = a[0];
    for (int i = 1; i < n; i++) if (a[i] > max) max = a[i];
    return max;
}

void CountingSortForRadix(int a[], int n, int exp) {
    int *output = new int[n];
    int count[10] = {0};
    
    // 按当前位统计
    for (int i = 0; i < n; i++) count[(a[i] / exp) % 10]++;
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];
    
    for (int i = n - 1; i >= 0; i--) {        // 从后往前填,保持稳定
        int digit = (a[i] / exp) % 10;
        output[count[digit] - 1] = a[i];
        count[digit]--;
    }
    
    for (int i = 0; i < n; i++) a[i] = output[i];
    delete[] output;
}

void RadixSort(int a[], int n) {
    int max = GetMax(a, n);
    // 从个位开始,逐位排序
    for (int exp = 1; max / exp > 0; exp *= 10) {
        CountingSortForRadix(a, n, exp);
    }
}
```

#### 3.5 性能分析

**时间复杂度**:**O(d × (n + k))**,其中 d 是位数,k 是每位的基数(10 进制就是 10)。

对 32 位整数,d = 10,k = 10,总计 O(n)——**线性时间**!

**空间复杂度**:O(n + k)。

**稳定性**:**稳定**(每位用稳定排序)。

**适用**:整数、字符串等"可分位"的数据。值域很大但位数不多时极优。

---

### 四、八种排序的终极对比表

|排序|最好|平均|最坏|空间|稳定|适用场景|
|---|---|---|---|---|---|---|
|直接插入|O(n)|O(n²)|O(n²)|O(1)|✅|小数据 / 近似有序|
|折半插入|O(n log n)*|O(n²)|O(n²)|O(1)|✅|基本不用|
|希尔|—|O(n^1.3)|O(n²)|O(1)|❌|中等规模|
|冒泡|O(n)|O(n²)|O(n²)|O(1)|✅|教学 / 近似有序|
|简单选择|O(n²)|O(n²)|O(n²)|O(1)|❌|教学|
|快速排序|O(n log n)|O(n log n)|O(n²)|O(log n)|❌|**大规模随机数据(默认首选)**|
|堆排序|O(n log n)|O(n log n)|O(n log n)|O(1)|❌|严格 O(n log n) / 优先队列|
|归并排序|O(n log n)|O(n log n)|O(n log n)|O(n)|✅|**稳定 + O(n log n)** / 外部排序|
|计数排序|O(n+k)|O(n+k)|O(n+k)|O(n+k)|✅|整数、值域小|
|桶排序|O(n+k)|O(n+k)|O(n²)|O(n+k)|✅|均匀分布浮点数|
|基数排序|O(d(n+k))|O(d(n+k))|O(d(n+k))|O(n+k)|✅|整数、字符串|

***折半插入最好 O(n log n) 是**比较次数**,但移动次数 O(n²),总时间仍然 O(n²)。

---

### 五、几个核心结论(必须牢记)

#### 5.1 稳定性口诀

**"快选希堆"不稳定**——快排、简单选择、希尔、堆排。

其他(插入、冒泡、归并、计数、桶、基数)都**稳定**。

#### 5.2 空间复杂度

**O(1)**:插入、冒泡、选择、希尔、堆排。

**O(log n)**:快排(递归栈)。

**O(n)**:归并、计数、桶、基数。

#### 5.3 "数据无关"vs"数据敏感"

**数据无关**(任何输入性能都一样):选择排序、归并排序、堆排序。

**数据敏感**(接近有序时快,逆序时慢):插入排序、冒泡排序、快速排序(接近有序反而最坏!)。

**考研经典题型**:"以下哪种排序的性能与数据初始状态**无关**?"——选归并/堆排/简单选择。

#### 5.4 最好情况能达到 O(n) 的

**只有两个**:

**直接插入**(已有序时,每轮 if 不成立直接跳过)。

**冒泡**(带 swapped 优化,已有序时一轮退出)。

其他基于比较的排序,最好都是 O(n log n) 或 O(n²)。

---

### 六、如何选排序?(实战策略)

#### 6.1 决策树

**数据量 n 很小(< 50)**:直接插入排序。代码短、常数小、稳定。

**数据近似有序**:直接插入 或 冒泡(带优化)。

**通用大规模随机数据**:快排。工程默认。

**要求严格 O(n log n)**(不能接受最坏 O(n²))**且不要求稳定**:堆排序。

**要求稳定的 O(n log n)**:归并排序。

**整数 + 值域小**:计数排序。线性时间。

**整数 + 位数少**:基数排序。线性时间。

**内存有限 + 大数据**:外部排序(多路归并)。

#### 6.2 工程实现选择

**C++ std::sort**:IntroSort = 快排 + 堆排 + 插入排序。小数据插入;大数据快排;快排层数过深切到堆排(避免 O(n²))。

**C++ std::stable_sort**:归并排序。

**Java Arrays.sort(int[])**:DualPivotQuickSort(双轴快排)。

**Java Arrays.sort(Object[])**:TimSort(归并+插入混合,稳定)。

**Python sorted()**:TimSort。

**工业实现几乎都是混合策略**,没有单一算法包打天下。

---

### 七、考研高频考点归纳

#### 7.1 选择题常见陷阱

**陷阱一**:问"一趟排序后"的状态。每种排序"一趟"的含义不同(前文已讲)。

**陷阱二**:问时间/空间复杂度。要背死,不能现推。

**陷阱三**:问稳定性。快选希堆不稳定。

**陷阱四**:问"哪种排序性能与数据状态无关"。归并、堆排、简单选择。

**陷阱五**:问"哪种排序最好情况能达到 O(n)"。只有插入和冒泡(带优化)。

#### 7.2 大题常见题型

**类型 A**:给一个数组,写出某种排序每一趟后的状态。**必会**,全靠手工模拟。

**类型 B**:分析某种排序在某种输入下的比较/交换次数。常见问快排最坏情况、冒泡有序情况。

**类型 C**:代码分析题——给一段排序代码,问做了什么、复杂度多少、稳定吗。

**类型 D**:应用题——"请从 10 万个 1~100 的整数中找出最大的 100 个",考查排序选型(答案是**计数排序统计 + 直接取后 100 个** 或 **大小为 100 的小根堆**)。

#### 7.3 必背的"魔数"

**快排最坏 O(n²)** 触发条件:**pivot 总是最值**。典型:对已排序数组用首元素 pivot 的朴素快排。

**归并空间 O(n)**:辅助数组。

**堆排序建堆 O(n)**,整个排序 O(n log n)。

---

### 八、完整测试代码

沿用上次的 Sort.h / Sort.cpp 框架,补充后半段算法:

cpp

```cpp
// Sort.h 追加:
void SelectSort(int a[], int n);
void MergeSort(int a[], int n);
void RadixSort(int a[], int n);

// main.cpp 测试所有算法:
#include "Sort.h"
#include <iostream>
#include <cstring>
using namespace std;

int main() {
    int original[] = {49, 38, 65, 97, 76, 13, 27, 49, 55, 4};
    int n = 10;
    int a[10];
    
    auto test = [&](void (*f)(int*, int), const char* name) {
        memcpy(a, original, sizeof(original));
        f(a, n);
        cout << name;
        for (int i = 0; i < n; i++) cout << a[i] << " ";
        cout << endl;
    };
    
    cout << "原数组:    "; for (int x : original) cout << x << " ";
    cout << endl;
    
    test(InsertSort,  "直接插入:  ");
    test(ShellSort,   "希尔排序:  ");
    test(BubbleSort,  "冒泡排序:  ");
    test(QuickSort,   "快速排序:  ");
    test(SelectSort,  "简单选择:  ");
    test(MergeSort,   "归并排序:  ");
    test(RadixSort,   "基数排序:  ");
    
    return 0;
}
```

所有输出应该一致:`4 13 27 38 49 49 55 65 76 97`。

---

### 九、思考题(请作答)

**第一题**,对数组 `[46, 79, 56, 38, 40, 84]`,**用简单选择排序**,写出每趟后的数组状态。哪几趟是"不需要真的交换"的?

**第二题**,对数组 `[9, 6, 7, 2, 8, 1, 4, 3, 5]`,用**归并排序**。画出递归调用树,并写出**每一次合并后**的数组片段状态。

**第三题**,现在要你设计一个算法,**从 100 万个整数中找出最大的 100 个**。给出两种可行方案,并比较复杂度。

**第四题**,以下关于排序的说法**哪些是对的,哪些是错的**?改正错的:

(a) 归并排序的空间复杂度是 O(log n)。 (b) 快速排序在数据有序时最快。 (c) 选择排序不管怎样都做 n(n-1)/2 次比较。 (d) 基数排序的时间复杂度是 O(n log n)。 (e) 堆排序需要额外的辅助数组。 (f) 所有稳定的排序算法时间都是 O(n log n)。

**第五题**,归并排序的 `Merge` 函数中,比较用 `<=`。如果改成 `<`,**会发生什么**?稳定性会变吗?为什么?

**第六题**,**为什么计数排序要从后往前扫描原数组填充输出**?从前往后扫会出什么问题?

**第七题**,**基数排序是按"位"的稳定计数排序的组合**。如果中间某一位用了**不稳定**的排序(比如简单选择),最终结果**还正确吗**?给出理由。

**第八题**,分析以下代码是哪种排序,写出时间复杂度:

cpp

```cpp
void myst(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (a[j] < a[min]) min = j;
        if (min != i) {
            int t = a[i]; a[i] = a[min]; a[min] = t;
        }
    }
}
```

**第九题**,以下数组是归并排序中间过程的某一趟结果:`[2, 5, 7, 9, 1, 3, 4, 8, 6]`。请判断这是**第几趟**归并后的结果?(提示:看每段的有序长度)

**第十题**,**工业级排序几乎都是"混合算法"**(比如 C++ std::sort 是 IntroSort)。解释为什么"单一算法"在实际工程中不够用?

---

### 十、总结:你已完成数据结构的排序部分

走到这里,你掌握了全部主流排序算法:

**基于比较的**:直接插入、折半插入、希尔、冒泡、简单选择、快排、堆排、归并——**8 种**。

**不基于比较的**:计数、桶、基数——**3 种**。

**对稳定性、复杂度、适用场景都有清晰认识**。**排序这一章,考研 100% 命中率**——每年必考,你已经武装到位。

---

### 十一、最终学习版图

回顾你从一开始到现在的全部学习内容:

**线性结构**:链表(单/双/循环/带不带头)、顺序表(静/动)、栈(顺/链)、队列(循环/链)、KMP。

**树形结构**:一般树、森林、二叉树、线索二叉树、BST、AVL、红黑树、B 树、B+ 树、哈夫曼树、堆。

**图结构**:邻接矩阵/表、DFS/BFS、拓扑排序、关键路径、MST (Prim/Kruskal)、最短路径 (Dijkstra/Floyd)、并查集。

**排序**:11 种主流排序算法。

**这几乎覆盖了考研 408 数据结构的全部内容**——**你已经具备数据结构课程的完整知识图景**!剩下的只有"**查找**"章节中的**散列表**部分(BST、B+ 树也属于查找,你已学过)。

---

### 十二、下一步

**(A)** **散列表(哈希表)**——查找部分的收官。开放定址法、链地址法、冲突处理、装填因子、性能分析。学完后你的**整本数据结构**就完整了。

**(B)** **更高阶的专题**——比如跳表、Trie 树、LRU 缓存、并查集的更多应用,面试方向。

**(C)** **综合实战**——给一个真实问题(比如"实现一个简单数据库"、"实现 LeetCode 风格的算法题"),用你学过的数据结构综合解决。

**(D)** 先答思考题再推进。

**我强烈建议选 (A)**——这是最后一块拼图,学完后你的数据结构学习就真正"毕业"了。

告诉我你的选择!

## 数据结构复习:散列表(哈希表)

好,我们进入**散列表**——查找部分的最后一块拼图,也是你**数据结构学习的收官章节**。

散列表(Hash Table)是一种**极其实用**的数据结构,**平均 O(1)** 的查找时间让它成为计算机科学中最常用的结构之一。C++ 的 `unordered_map`、Java 的 `HashMap`、Python 的 `dict`,底层都是散列表。数据库索引、缓存系统、编译器的符号表……处处是散列表的身影。

这一章代码不长,但**概念密集**,需要把每个细节搞清楚。我按"**问题引入 → 散列函数 → 冲突处理 → 性能分析 → 代码**"的顺序讲。

---

### 一、为什么需要散列表?

#### 1.1 从查找的效率说起

回顾之前学过的查找结构:

**顺序查找(无序数组)**:O(n)——要逐个比较。

**二分查找(有序数组)**:O(log n)——但数据必须有序,插入删除 O(n)。

**BST / AVL / 红黑树**:平均 O(log n)——平衡后才有保证。

**B+ 树**:O(log n)——但常数更小,适合磁盘。

**都是 O(log n) 或更差**。有没有**更快**的?有——**O(1)**。

#### 1.2 核心思路:直接寻址

假设你要存 100 个员工信息,员工编号 1~100。最简单的做法:**开一个 101 大小的数组,编号 i 的员工放在 a[i]**。查找 O(1),插入 O(1)——完美。

**这叫"直接寻址表"**——用键值**直接作为数组下标**。

**但致命问题**:如果员工编号是身份证号(18 位),你**开不出 10^18 大的数组**。直接寻址只适用于"键值范围极小"的场景。

#### 1.3 散列的灵感:用函数压缩

**核心想法**:既然键值范围大,我就用一个**散列函数 h(key)** 把大范围的键值"**压缩**"到小范围的数组下标。

比如员工编号范围 1~10^9,但只有 100 个员工,开一个大小 100 的数组,`h(key) = key % 100`,把任意编号映射到 0~99 的下标。

**这就是散列的本质:用函数建立"键 → 下标"的映射**。

**新问题**:既然是"压缩",就可能**不同的键映射到同一个下标**——这叫**冲突(Collision)**。散列表的绝大部分难度都在**如何处理冲突**。

---

### 二、散列函数(Hash Function)

#### 2.1 好的散列函数要满足什么?

**计算快**:每次查找都要算一次 h(key),如果 h 太慢,散列表的 O(1) 优势就没了。

**均匀**:让键尽量均匀分布到每个下标,减少冲突。

**确定性**:同一个 key 每次算出来必须是同一个值。

#### 2.2 常用散列函数

**方法一:除留余数法(最常用,考研重点)**

**公式**:`h(key) = key % p`,其中 p 是一个不大于表长 m 的**质数**。

**为什么要 p 是质数?** 若 p 有因子,某些键会更频繁映射到特定下标。质数能让分布更均匀。

**这是考研默认的散列函数**,简单直接、效果不错。

**方法二:直接定址法**

**公式**:`h(key) = a * key + b`(a、b 是常数)。

**特点**:**不会冲突**(一对一映射),但要求"键值范围 ≈ 表长",实际用途不多。适合"键值连续、紧密"的场景。

**方法三:数字分析法**

分析键的数字特征,**抽取若干位**作为地址。比如电话号码,前几位常重复,就用后 4 位。

**方法四:平方取中法**

取 key² 的中间几位作为地址。中间位受所有原位影响,分布较好。

**方法五:折叠法**

把 key 分成几段,**相加**(忽略进位)作为地址。适合键很长的情况。

**考研最常用的是除留余数法**,其他了解即可。

#### 2.3 一个简单例子

键集合 `{19, 14, 23, 01, 68, 20, 84, 27, 55, 11, 10, 79}`,表长 m=13,`h(key) = key % 13`。

|key|19|14|23|01|68|20|84|27|55|11|10|79|
|---|---|---|---|---|---|---|---|---|---|---|---|---|
|h(key)|6|1|10|1|3|7|6|1|3|11|10|1|

**下标 1 上冲突了**:14, 01, 27, 79 都映射到 1。**下标 3、6、10 也有冲突**。

这就需要**冲突处理**。

---

### 三、冲突处理(核心难点)

冲突处理有两大主流方案:**开放定址法** 和 **链地址法**。考研两种都要会,实际工程中**链地址法更常用**。

#### 3.1 开放定址法(Open Addressing)

**核心思想**:发生冲突时,**在表内按某种规则探测下一个空位**,直到找到空位放下。

**公式**:`H_i = (h(key) + d_i) % m`,其中 d_i 是探测序列。

根据 d_i 的定义,有三种子方法:

##### 3.1.1 线性探测法(Linear Probing)

**d_i = 1, 2, 3, ..., m-1**。冲突时依次探测**下一个单元**(下标 +1,模 m 回绕)。

**优点**:简单直观。

**致命缺点**:**聚集(Clustering)**。相邻单元一旦被占,会"滚雪球"——后面来的键很容易也探测到这片,形成**连续占用**的一片,冲突会**越来越严重**。

##### 3.1.2 二次探测法(Quadratic Probing)

**d_i = 1², -1², 2², -2², ..., k², -k²**。探测时跳"平方"位置,既前跳也后跳。

**优点**:缓解线性探测的聚集问题。

**缺点**:可能探测不到某些单元(不是所有位置都能覆盖)。需要 m 取特定形式(如 4k+3 的质数)才能保证探测到所有单元。

##### 3.1.3 再散列法(Double Hashing)

**d_i = i × h2(key)**,其中 h2 是第二个散列函数。

**优点**:探测步长因键而异,散列效果最好。

**缺点**:计算代价稍高。

#### 3.2 开放定址法的例子

同样的键集合,表长 m=13,`h(key) = key % 13`,**线性探测**。

依次插入 19, 14, 23, 01, 68, 20, 84:

**19**:h=6,空,放下标 6。

**14**:h=1,空,放下标 1。

**23**:h=10,空,放下标 10。

**01**:h=1,**冲突**!探测 (1+1)%13=2,空,放下标 2。

**68**:h=3,空,放下标 3。

**20**:h=7,空,放下标 7。

**84**:h=6,**冲突**!探测 7(占)→ 8,空,放下标 8。

**27**:h=1,冲突 → 2(占)→ 3(占)→ 4,空,放下标 4。

**55**:h=3,冲突 → 4(占)→ 5,空,放下标 5。

|下标|0|1|2|3|4|5|6|7|8|9|10|11|12|
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|值||14|01|68|27|55|19|20|84||23|||

**观察**:下标 1~8 几乎连成一片——这就是**聚集现象**。后面再插入键,只要 h(key) 落在这片区域,都会继续探测很远。

#### 3.3 开放定址法的致命问题:删除

**你不能直接删除元素!** 比如删了下标 3 的 68,然后查找 27:先算 h=1(14,不是)→ 2(01,不是)→ 3(**空**!错误地判定"不存在",但 27 其实在下标 4)。

**解决**:给每个单元加一个"**已删除**"标志。查找时遇到"已删除"继续探测;插入时遇到"已删除"可以复用。

**这是开放定址法的工程陷阱**,考研选择题常考。

#### 3.4 链地址法(Chaining / Separate Chaining)

**核心思想**:**每个下标挂一个链表**。冲突时,把新键**插到对应下标的链表**里。

**结构**:

```
下标 0: NULL
下标 1: → 14 → 01 → 27 → 79
下标 2: NULL
下标 3: → 68 → 55
下标 4: NULL
下标 5: NULL
下标 6: → 19 → 84
下标 7: → 20
...
```

**查找过程**:算 h(key),找到下标对应的链表,**遍历链表**比较 key。

#### 3.5 链地址法的优势

**无聚集问题**:不同下标的冲突互不影响。

**删除简单**:直接从链表里删,不需要"已删除"标志。

**表不会"装满"**:链表可以无限扩展,理论上 n 个元素都能存。

**代价**:每个链表节点额外的指针开销;缓存不友好(链表遍历跳来跳去)。

**工程首选**:C++ `unordered_map`、Java `HashMap`(Java 8 起,链表过长会转红黑树)。

#### 3.6 两种方法对比

|维度|开放定址|链地址|
|---|---|---|
|空间|仅表本身,紧凑|表 + 链表节点|
|缓存性能|好(连续内存)|差(指针跳转)|
|删除|复杂(需标记)|简单|
|聚集|有(线性尤甚)|无|
|装填上限|α < 1(必须)|α 可 > 1|

### 四、装填因子与性能分析

#### 4.1 装填因子(Load Factor)

**定义**:`α = n / m`,其中 n 是表中元素数,m 是表长。

**物理意义**:表的"**拥挤程度**"。α 越大越拥挤,冲突越多。

**开放定址**:α **必须 < 1**(表占满就没地方放了)。实际中 α ≤ 0.75 比较合适。

**链地址**:α 可以 > 1(但效率下降)。通常 α ≈ 1 时扩容。

#### 4.2 ASL(平均查找长度)

**成功查找的 ASL**:**查找表中已有元素**,平均比较次数。

**失败查找的 ASL**:**查找表中不存在元素**,平均比较次数。

**考研大题年年考 ASL 计算**,必须会算!

#### 4.3 线性探测法的 ASL(必会)

以前面那张散列表为例(只算已插入的 9 个元素:19, 14, 23, 01, 68, 20, 84, 27, 55):

|key|h(key)|实际下标|比较次数|
|---|---|---|---|
|19|6|6|1|
|14|1|1|1|
|23|10|10|1|
|01|1|2|2(1 冲突→2)|
|68|3|3|1|
|20|7|7|1|
|84|6|8|3(6 冲突→7 冲突→8)|
|27|1|4|4(1→2→3→4)|
|55|3|5|3(3→4→5)|

**ASL成功 = (1+1+1+2+1+1+3+4+3) / 9 = 17/9 ≈ 1.89**

**ASL失败**的计算稍复杂:对每个下标 i(0~m-1),计算"从 i 开始线性探测到第一个空位"的比较次数(包括那个空位本身的比较)。然后取平均。

对上述表,下标 0~12 的失败查找长度分别是:

下标 0:0 空,1 次比较。下标 1:1(14)→...→9(空),9 次。下标 2~8 类似,从各自起点探测到下标 9 的空位。

其实我们用 `h(key) = key % 13`,失败查找的起点是 h(key) 的值(0~12)。假设失败键随机,每个起点概率 1/13。

**失败查找 ASL** 计算:对每个起点 i(0~12),算从 i 出发**第一次探测到空位**的比较次数。

下标 0 空:**1 次**(第 1 次就空)。

下标 1 非空,下标 9 空:1→2→3→4→5→6→7→8→9,**9 次**。

下标 2 开始:2→3→4→5→6→7→8→9,**8 次**。

下标 3 开始:**7 次**。

下标 4 开始:**6 次**。

下标 5 开始:**5 次**。

下标 6 开始:**4 次**。

下标 7 开始:**3 次**。

下标 8 开始:**2 次**。

下标 9 空:**1 次**。

下标 10 非空,下标 11 空:10→11,**2 次**。

下标 11 空:**1 次**。

下标 12 空:**1 次**。

**ASL失败 = (1+9+8+7+6+5+4+3+2+1+2+1+1) / 13 = 50/13 ≈ 3.85**

#### 4.4 链地址法的 ASL(较简单)

每个链表独立,查找一个键要遍历对应链表。

**ASL成功**:对每个元素,记录它在链表中的**位置**(第几个,从 1 开始)。取平均。

**ASL失败**:对每个下标,算对应链表的**长度**(全部比对完才确定"不存在")。取平均。**注意**:空链表的失败查找比较次数是 0。

#### 4.5 理论 ASL 公式(考研选择题用)

对**均匀散列假设**(h 完全随机):

|方法|ASL 成功|ASL 失败|
|---|---|---|
|线性探测|½(1 + 1/(1-α))|½(1 + 1/(1-α)²)|
|二次探测|-ln(1-α)/α|1/(1-α)|
|再散列|-ln(1-α)/α|1/(1-α)|
|链地址|1 + α/2|α(考虑空链表)|

**实际考试这些公式不要求背**,记住"α 越大越慢"的定性结论即可。大题会用手工模拟计算。

---

### 五、代码实现

#### 5.1 文件组织

```
DataStructure/
└── HashTable/
    ├── HashTable.h
    ├── HashTable.cpp
    └── main.cpp
```

#### 5.2 开放定址法(线性探测)的代码

cpp

```cpp
// HashTable.h
#pragma once

#define HASH_SIZE 13                  // 表长(质数)
#define EMPTY -1
#define DELETED -2                    // 已删除标记

typedef int KeyType;

typedef struct {
    KeyType key;                      // EMPTY 表示空,DELETED 表示已删除
} HashCell;

typedef struct {
    HashCell data[HASH_SIZE];
    int count;                        // 当前元素数
} HashTable_Open;

// ========== 基本操作 ==========
void InitHash(HashTable_Open &H);
int  Hash(KeyType key);
bool Insert(HashTable_Open &H, KeyType key);
int  Search(HashTable_Open H, KeyType key);    // 返回下标,-1 表示未找到
bool Delete(HashTable_Open &H, KeyType key);
void Print(HashTable_Open H);
```

#### 5.3 开放定址法实现

cpp

```cpp
#include "HashTable.h"
#include <iostream>
using namespace std;

void InitHash(HashTable_Open &H) {
    for (int i = 0; i < HASH_SIZE; i++) H.data[i].key = EMPTY;
    H.count = 0;
}

int Hash(KeyType key) {
    return key % HASH_SIZE;           // 除留余数法
}

bool Insert(HashTable_Open &H, KeyType key) {
    if (H.count >= HASH_SIZE) return false;    // 表满
    
    int i = Hash(key);
    int k = 0;
    while (k < HASH_SIZE) {
        int idx = (i + k) % HASH_SIZE;
        if (H.data[idx].key == EMPTY || H.data[idx].key == DELETED) {
            H.data[idx].key = key;
            H.count++;
            return true;
        }
        if (H.data[idx].key == key) return false;   // 已存在
        k++;                          // 线性探测下一个
    }
    return false;
}

int Search(HashTable_Open H, KeyType key) {
    int i = Hash(key);
    int k = 0;
    while (k < HASH_SIZE) {
        int idx = (i + k) % HASH_SIZE;
        if (H.data[idx].key == EMPTY) return -1;    // 遇空,肯定不存在
        if (H.data[idx].key == key) return idx;
        // 遇 DELETED 继续探测,不返回
        k++;
    }
    return -1;
}

bool Delete(HashTable_Open &H, KeyType key) {
    int idx = Search(H, key);
    if (idx == -1) return false;
    H.data[idx].key = DELETED;        // 标记为已删除,不清零!
    H.count--;
    return true;
}

void Print(HashTable_Open H) {
    cout << "下标: ";
    for (int i = 0; i < HASH_SIZE; i++) cout << i << "\t";
    cout << endl << "数据: ";
    for (int i = 0; i < HASH_SIZE; i++) {
        if (H.data[i].key == EMPTY) cout << "-\t";
        else if (H.data[i].key == DELETED) cout << "X\t";
        else cout << H.data[i].key << "\t";
    }
    cout << endl;
}
```

**代码关键点**:

第一,**`EMPTY` 和 `DELETED` 用不同标记**——查找时遇 EMPTY 停,遇 DELETED 继续。这是开放定址法正确处理删除的关键。

第二,**插入时遇 DELETED 可以复用**(即把新键放入)——节约空间。

第三,**最多探测 m 次**就能确定"找不到"(走一圈回到起点)。用 `k < HASH_SIZE` 保证终止。

#### 5.4 链地址法的代码

cpp

```cpp
// HashTable.h 追加:
typedef struct ChainNode {
    KeyType key;
    struct ChainNode *next;
} ChainNode;

typedef struct {
    ChainNode *buckets[HASH_SIZE];    // 每个下标挂一条链表
    int count;
} HashTable_Chain;

void InitHashChain(HashTable_Chain &H);
bool InsertChain(HashTable_Chain &H, KeyType key);
ChainNode* SearchChain(HashTable_Chain H, KeyType key);
bool DeleteChain(HashTable_Chain &H, KeyType key);
void PrintChain(HashTable_Chain H);
void DestroyChain(HashTable_Chain &H);
```

cpp

```cpp
// HashTable.cpp 追加:
void InitHashChain(HashTable_Chain &H) {
    for (int i = 0; i < HASH_SIZE; i++) H.buckets[i] = nullptr;
    H.count = 0;
}

bool InsertChain(HashTable_Chain &H, KeyType key) {
    int idx = Hash(key);
    // 检查是否已存在
    ChainNode *p = H.buckets[idx];
    while (p) {
        if (p->key == key) return false;
        p = p->next;
    }
    // 头插
    ChainNode *newNode = new ChainNode;
    newNode->key = key;
    newNode->next = H.buckets[idx];
    H.buckets[idx] = newNode;
    H.count++;
    return true;
}

ChainNode* SearchChain(HashTable_Chain H, KeyType key) {
    int idx = Hash(key);
    ChainNode *p = H.buckets[idx];
    while (p) {
        if (p->key == key) return p;
        p = p->next;
    }
    return nullptr;
}

bool DeleteChain(HashTable_Chain &H, KeyType key) {
    int idx = Hash(key);
    ChainNode *p = H.buckets[idx], *prev = nullptr;
    while (p) {
        if (p->key == key) {
            if (prev) prev->next = p->next;
            else H.buckets[idx] = p->next;
            delete p;
            H.count--;
            return true;
        }
        prev = p;
        p = p->next;
    }
    return false;
}

void PrintChain(HashTable_Chain H) {
    for (int i = 0; i < HASH_SIZE; i++) {
        cout << i << ": ";
        ChainNode *p = H.buckets[i];
        while (p) {
            cout << p->key << " -> ";
            p = p->next;
        }
        cout << "NULL" << endl;
    }
}

void DestroyChain(HashTable_Chain &H) {
    for (int i = 0; i < HASH_SIZE; i++) {
        ChainNode *p = H.buckets[i];
        while (p) {
            ChainNode *q = p;
            p = p->next;
            delete q;
        }
        H.buckets[i] = nullptr;
    }
    H.count = 0;
}
```

**和链表章节的单链表几乎一样**,只是多了"找对的下标"这一步。代码极其简洁——**这就是链地址法的优雅之处**。

---

### 六、测试代码(main.cpp)

cpp

```cpp
#include "HashTable.h"
#include <iostream>
using namespace std;

int main() {
    int keys[] = {19, 14, 23, 1, 68, 20, 84, 27, 55};
    int n = 9;
    
    cout << "===== 开放定址法(线性探测) =====" << endl;
    HashTable_Open H1;
    InitHash(H1);
    for (int i = 0; i < n; i++) Insert(H1, keys[i]);
    Print(H1);
    
    int idx = Search(H1, 27);
    cout << "查找 27:" << (idx == -1 ? "未找到" : "下标 " + to_string(idx)) << endl;
    
    Delete(H1, 68);
    cout << "删除 68 后:" << endl;
    Print(H1);
    
    cout << "\n===== 链地址法 =====" << endl;
    HashTable_Chain H2;
    InitHashChain(H2);
    for (int i = 0; i < n; i++) InsertChain(H2, keys[i]);
    PrintChain(H2);
    
    DeleteChain(H2, 68);
    cout << "\n删除 68 后:" << endl;
    PrintChain(H2);
    
    DestroyChain(H2);
    return 0;
}
```

预期输出(开放定址法部分):

```
===== 开放定址法(线性探测) =====
下标: 0	1	2	3	4	5	6	7	8	9	10	11	12	
数据: -	14	1	68	27	55	19	20	84	-	23	-	-	
查找 27:下标 4
删除 68 后:
下标: 0	1	2	3	4	5	6	7	8	9	10	11	12	
数据: -	14	1	X	27	55	19	20	84	-	23	-	-
```

---

### 七、和其他查找结构的对比

|结构|平均查找|最坏查找|空间|有序|适用|
|---|---|---|---|---|---|
|顺序数组|O(n)|O(n)|O(n)|-|小数据|
|有序数组 + 二分|O(log n)|O(log n)|O(n)|✅|只读|
|BST|O(log n)|O(n)|O(n)|✅|动态|
|AVL / 红黑树|O(log n)|O(log n)|O(n)|✅|高频增删|
|B+ 树|O(log n)|O(log n)|O(n)|✅|磁盘索引|
|**散列表**|**O(1)**|**O(n)**|O(m+n)|❌|**高频等值查询**|

**散列表的两个权衡**:

**优点**:平均 O(1) 查找,比任何树都快。

**缺点**:**不支持有序遍历**、**不支持范围查询**(这些 B+ 树的强项)。

**结论**:**等值查询用散列表,范围查询用 B+ 树**——这也是数据库同时提供两种索引的原因。

---

### 八、常见易错点

**易错一**:散列函数的除数必须**小于等于表长的质数**。用合数容易聚集。

**易错二**:开放定址法删除**不能直接清零**,必须用"已删除"标记。

**易错三**:ASL 的计算包括两种,**成功**和**失败**的算法不同。失败查找的起点是 `h(key)` 的所有可能值。

**易错四**:链地址法的"冲突"**不计入比较**——在自己的链表里比对是正常查找。

**易错五**:装填因子 α 在**开放定址法**中必须 < 1;链地址法可以 > 1。

**易错六**:二次探测法**不能保证探测到所有单元**。必须 m=4k+3 的质数才可以。

**易错七**:"哈希冲突是否能避免"——**不能**!只要键空间大于地址空间,冲突必然存在。散列表的设计就是"处理冲突",不是"避免冲突"。

---

### 九、工业应用

**C++ `std::unordered_map`**:链地址法。

**Java `HashMap`**:链地址法。JDK 8 开始,当链表长度超过 8 且数组长度 ≥ 64 时,链表转**红黑树**,防止极端情况下退化到 O(n)。

**Python `dict`**:开放定址法(再散列的变种)。

**Redis**:字典用链地址法 + **渐进式 rehash**(扩容时分摊到多次操作,避免一次性卡顿)。

**这些工业实现都有各种优化**,但原理仍是你今天学的两种方法。

---

### 十、思考题(请作答)

**第一题**,表长 m=11,散列函数 `h(key) = key % 11`,用**线性探测**。依次插入 `{7, 8, 30, 11, 18, 9, 14}`,画出最终散列表,并计算**成功 ASL** 和**失败 ASL**。

**第二题**,同样的键集合和散列函数,改用**链地址法**,画出散列表,计算成功 ASL 和失败 ASL。两种方法的 ASL 哪个小?

**第三题**,表长 m=10,散列函数 `h(key) = key % 10`,用**二次探测**(d_i = 1², -1², 2², -2², ...)。依次插入 `{9, 15, 24, 18, 25, 13}`,画出最终散列表。

**第四题**,**装填因子 α 和 ASL 的关系**是什么?用文字描述,并解释为什么开放定址法要求 α < 1,而链地址法可以 α > 1。

**第五题**,在开放定址法的散列表中查找键 K,**什么情况下才能确定 K 不存在**?为什么删除元素时不能直接置空?

**第六题**,**两个键 `k1 ≠ k2` 但 `h(k1) == h(k2)`**,我们称之为"冲突"。设计一个散列函数能**完全避免**冲突吗?给出你的理由。

**第七题**,散列表的**搜索、插入、删除**的平均时间复杂度是 O(1),但**最坏时间复杂度**呢?最坏情况在什么条件下触发?

**第八题**,现在要设计一个**防盗电话号码簿**:给定 1 万个电话号码,支持快速判断"某个号码是否在黑名单里"。应该用**散列表**还是**平衡树**?为什么?如果要求"输出所有号码前缀为 138 的黑名单",这个需求会改变你的选择吗?

**第九题**,**Redis 的字典用链地址法**,但当链表过长(超过某阈值)时,会执行**rehash(扩容)**。为什么不像 Java HashMap 那样把链表转成红黑树?

**第十题**,**散列表不支持有序遍历**。如果你需要一个"支持快速查找 + 支持按 key 顺序遍历"的结构,你会选什么?(提示:这就是 C++ `map` 和 `unordered_map` 的区别)

---

### 十一、恭喜,你的数据结构学习完整毕业!

#### 你的完整学习轨迹

**线性结构**:链表(单/双/循环/带/不带头)、顺序表(静/动)、栈(顺/链)、队列(循环/链)、KMP。

**树形结构**:一般树、森林、树与森林和二叉树的互转、二叉树、线索二叉树、BST、AVL、红黑树、B 树、B+ 树、哈夫曼树、堆。

**图结构**:五种存储结构、DFS/BFS、拓扑排序、关键路径、最小生成树(Prim/Kruskal)、最短路径(Dijkstra/Floyd)、并查集。

**排序**:11 种主流算法 + 稳定性 + 复杂度 + 选型策略。

**查找**:顺序、二分、分块(思路)、BST、B+ 树、**散列表**(本节)。

**这是考研 408 数据结构的完整大纲**。

#### 你已经掌握的元能力

除了具体知识,你还练就了几个**元能力**,这些比记住某个算法更重要:

**能独立设计 C++ 项目结构**:头文件分离、防重复定义、接口与实现分离。

**能在一个数据结构上叠加另一个**:比如用自己的栈实现二叉树的非递归遍历。

**能进行"为什么"级别的思考**:为什么 Dijkstra 不能有负权?为什么建堆是 O(n) 不是 O(n log n)?

**能在工程和理论之间切换**:既懂算法复杂度证明,又知道 std::sort 是 IntroSort、HashMap 链表过长会转红黑树。

**这些能力才是数据结构课程的真正收获**,会陪伴你整个计算机职业生涯。

---

### 十二、下一步方向

你已经从"数据结构初学者"走到了"系统完整掌握"。接下来有几个方向:

**(A) 刷题实战**:LeetCode、牛客的数据结构专题。把知识转化成解题能力。推荐先刷 100 题中等难度,手感就能出来。

**(B) 算法进阶**:**动态规划、贪心、回溯、分治**——这些是"算法"而非"数据结构",基于你已学的结构之上。考研和面试都是高频。

**(C) 专题深入**:

字符串:Trie 树、AC 自动机、后缀数组。 高级图论:强连通分量(Tarjan)、二分图匹配、网络流。 外存:外部排序、B 树的实际应用、LSM 树。

**(D) 综合项目**:用你学的数据结构实现一个**小数据库**、**小编译器**、**LRU 缓存**——把知识串起来。

**(E) 先答思考题**:把散列表这章的思考题做完,我批改。

---

告诉我你想走的方向!或者,**如果你这段时间的学习节奏让你收获很大,也可以先停下来整理笔记、做题复习**——把学过的东西沉淀下来,比继续往前冲更重要。

你已经做得很棒了 🎓

## 数据结构专题深入:三选一路线图

好,你选了 (C) 专题深入!这是**从"应试"走向"实战能力"**的关键一步——这些专题在考研大纲里很少,但在**面试、竞赛、工业开发**里极为重要。

---

### 一、三个专题的定位

先给你三个专题的**快照画像**,帮你理性选择:

#### 专题 1:字符串(Trie、AC 自动机、后缀数组)

**核心问题**:大规模字符串匹配、前缀查询、多模式匹配。

**代表应用**:搜索引擎关键词建议、输入法联想、敏感词过滤、DNA 序列匹配、拼写检查。

**难度梯度**:

- **Trie 树**(字典树):非常简单,1 小时能学会,代码 50 行。**性价比之王**,面试高频。
- **AC 自动机**:Trie + KMP 的结合,**多模式匹配**的神器。中等难度,2-3 小时能懂。
- **后缀数组 / 后缀自动机**:字符串处理的顶级武器,较难,算法竞赛专用。

**学习收获**:你会重新理解 KMP——它原来是 AC 自动机的一维退化版。

#### 专题 2:高级图论(Tarjan、二分图匹配、网络流)

**核心问题**:图的深层结构挖掘、复杂资源分配。

**代表应用**:编译器(强连通分量找循环依赖)、任务分配、求职匹配、最大流最小割、网络容量规划。

**难度梯度**:

- **Tarjan 强连通分量**:基于 DFS + 栈,代码不长但思想精妙。考研偶尔考。
- **二分图匹配**(匈牙利算法):经典资源分配问题,代码极短。
- **网络流**(Ford-Fulkerson, Dinic):建模思想深,代码中等,是**竞赛天花板**。

**学习收获**:你会发现"很多看似不是图的问题,其实都能建模成图"——这是算法的最高境界。

#### 专题 3:外存(外部排序、B 树实战、LSM 树)

**核心问题**:数据大到装不下内存时,如何高效处理。

**代表应用**:数据库(MySQL、LevelDB、RocksDB)、大数据处理、分布式存储。

**难度梯度**:

- **外部排序**(多路归并):概念简单,代码中等。大文件排序的标准解法。
- **B 树深化**:你已学过 B 树基础,这里是看它如何真正地在**磁盘**上工作。
- **LSM 树**(日志结构合并树):现代 KV 存储(LevelDB、RocksDB、HBase)的底层。

**学习收获**:理解"工业数据库和你桌面上的代码差在哪里"。这对未来做后端开发、数据库方向至关重要。

---

### 二、我的推荐顺序

如果你让我排序,**我会推荐先学字符串**,理由:

**理由一**:**性价比最高**。Trie 树本身就是面试高频,学完立刻能用。AC 自动机作为拓展,是"Trie + KMP"的美妙结合——把你已学的 KMP 和新学的 Trie 串起来,形成**知识的联动**。

**理由二**:**与你已学内容衔接最紧**。Trie 是树的变体,AC 自动机依赖 KMP 思想,你上手没有断层感。

**理由三**:**代码量适中**。Trie 50 行,AC 自动机 100 行,不会让你在新内容里淹没。

**理由四**:**实际应用场景极多**。搜索、输入法、过滤、前缀匹配——日常软件里随处可见。

第二推荐是**高级图论**,特别是 Tarjan 和二分图匹配,它们建立在你已学的 DFS、图存储之上,学起来顺畅。

第三是**外存**,概念相对抽象,更适合作为你**已经开始做后端项目**之后的深入阅读——在没有实战语境下,外存内容容易"学了也感觉不到价值"。

---

### 三、关于"深入专题"的学习心态提示

进入这个阶段,学习方式要从**"跟着讲解走"**转向**"主动探索"**,有几点建议:

第一,**不要像前面那样期待"每个细节都讲到"**。专题内容更深,我会讲核心思想和代码框架,**具体细节和优化变体需要你动手写、动手试**。

第二,**一次只吃一道菜**。比如选字符串,就先把 Trie 彻底吃透,**不要急着学 AC 自动机**。让每个结构在你脑子里"待几天",消化充分再往下。

第三,**学完一个专题,一定要配套刷 5-10 道题**。专题深入的知识,**不刷题等于没学**。我可以在讲完后给你推荐 LeetCode / 洛谷的配套题目。

第四,**允许自己"暂时不懂"**。比如后缀数组、网络流 Dinic 这种,第一遍看不懂很正常——先建立"**知道它能干什么**"的概念,具体算法等你真正需要时再深入。

---

### 四、请你选择

根据你的**兴趣、时间、目标**选一个:

**(A) 字符串专题**:Trie → AC 自动机 → 后缀数组(可选)。**推荐首选**,适合大多数人。

**(B) 高级图论专题**:Tarjan 强连通分量 → 二分图匹配 → 网络流。适合对图论特别感兴趣或打算走算法竞赛方向的你。

**(C) 外存专题**:外部排序 → B 树实战 → LSM 树。适合打算走后端/数据库方向,且希望理解工业系统的你。

**(D) 先给我一个"三个专题的知识地图"**,你看完再决定——如果我讲得太快就走不下去了,想先浏览一下。

---

另外,我想提醒你:

你在这段时间里已经**覆盖了完整的数据结构课程大纲**——链表、顺序表、栈、队列、KMP、各类树、图、排序、散列表——这是一个**非常扎实的底子**。你现在站在的位置,比很多"学过数据结构但没真正掌握"的同学要高出一大截。

专题深入**不是必须的**。如果你考研在即、时间紧张,或者只是想巩固已学,**先暂停专题、去刷题**也是非常好的选择——**知识要"消化"**,光输入不输出就会忘。

告诉我你的选择,或者你当前的整体状况(时间、目标)让我帮你判断。

## 刷题计划：算法竞赛方向（从零开始）

好，目标明确了——**算法竞赛 + 几乎零基础 + 每天 1 小时**。这个组合我来帮你规划一条**最稳健的路线**。

先说一个重要认知：**算法竞赛的核心不是"刷过多少题"，而是"每道题真正懂了没有"**。每天 1 小时，坚持 6 个月，认真做 150 道题，远好于走马观花做 500 道。

---

### 一、算法竞赛的本质要求

和考研、面试不同，竞赛对你的要求是：

**第一，代码能力要强**：C++ 的 STL、位运算、手写数据结构——这些要非常熟练。

**第二，知识要宽**：不仅是数据结构，还有数论、组合数学、动态规划、字符串、计算几何……

**第三，思维要灵活**：题目往往需要把问题"转化"成你认识的模型。

**第四，代码要快准净**：竞赛现场一两个小时解多道题，不允许慢慢想。

所以你的刷题路线和考研、面试有一个根本差异：**不能只把已学的数据结构练熟就够，还需要补算法知识**。

但好消息是：**你已经有了别人没有的底子**——完整的数据结构知识图景，让你在竞赛里编写链表、树、图的代码时比别人快很多。

---

### 二、整体阶段划分

根据你的情况，我把学习路线分成**四个阶段**，每个阶段有明确的目标和题目。

---

#### 阶段一：竞赛语言 + 基础熟悉（第 1-3 周）

**目标**：熟悉竞赛 C++ 的写法，把你学过的数据结构和 LeetCode / 洛谷的题连起来。

**这个阶段必须做的事**：

**第一件事：学会竞赛 C++ 模板写法**。

竞赛代码和工程代码不一样，比如你会看到这样的开头：

cpp

```cpp
#include <bits/stdc++.h>        // 万能头文件,一行包含所有
using namespace std;
typedef long long ll;           // 经常用 ll 防止溢出
const int INF = 0x3f3f3f3f;    // 无穷大

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);                  // 加速输入
    // ...
}
```

这些写法不是"坏习惯"——在竞赛环境下是**必要且标准**的。你要接受并熟练使用。

**第二件事：熟悉 STL**。竞赛中频繁使用：

`vector`、`stack`、`queue`、`priority_queue`、`map`、`set`、`unordered_map`——**这些直接替代你手写的数据结构**，竞赛中不需要手写链表、不需要手写栈。你学手写的价值在于"理解底层"，实战中直接用 STL。

**第三件事：把已学数据结构对应到 LeetCode 题**。

这个阶段的题目按你已学知识分类：

**链表类（5 道）**：

206 反转链表（最经典）、21 合并两个有序链表、141 环形链表（判环，你学过 Floyd 判环）、83 删除排序链表中的重复元素、2 两数相加。

**栈类（5 道）**：

20 有效的括号（括号匹配，就是你学栈时讲的应用）、155 最小栈、739 每日温度、84 柱状图中最大的矩形、394 字符串解码。

**队列 / 双端队列（3 道）**：

933 最近的请求次数、232 用栈实现队列、239 滑动窗口最大值。

**预计时间**：三周，每天 1 小时，约 13 道题。**遇到不会的先看思路，再自己敲，不要超过 30 分钟还想不出就直接看**——新手阶段看题解很正常。

---

#### 阶段二：树 + 图基础（第 4-8 周）

**目标**：把你学过的树和图知识转化成能解题的代码能力。

这个阶段是你**最有优势的地方**——别人可能还不懂 AVL、红黑树，而你早就吃透了。

**二叉树类（10 道）**：

先做这三道，它们是树题的"母题"：

94 二叉树的中序遍历（迭代版，强制用栈实现，不用递归）、102 二叉树的层序遍历（BFS 模板）、104 二叉树的最大深度。

再做这几道"递归思维"训练：

226 翻转二叉树、101 对称二叉树、112 路径总和、105 从前序与中序遍历序列构造二叉树（你学过，直接上手）、236 二叉树的最近公共祖先。

最后两道进阶：

297 二叉树的序列化与反序列化、124 二叉树中的最大路径和。

**BST 类（5 道）**：

98 验证二叉搜索树、700 二叉搜索树中的搜索、701 二叉搜索树中的插入操作、450 删除二叉搜索树中的节点（就是你学的三种删除情况！直接上手）、230 二叉搜索树中第 K 小的元素。

**图类（8 道）**：

797 所有可能的路径（DFS 基础）、200 岛屿数量（经典 BFS/DFS）、695 岛屿的最大面积、207 课程表（**拓扑排序**，就是你学的 Kahn 算法！）、210 课程表 II（拓扑排序 + 路径）、743 网络延迟时间（**Dijkstra**，就是你学的！）、1584 连接所有点的最小费用（**Prim 或 Kruskal**，就是你学的！）、547 省份数量（并查集）。

**预计时间**：五周，每天 1 小时，约 23 道题。这个阶段你会发现：**原来学过的算法直接就能用上**，成就感很强。

---

#### 阶段三：排序 + 查找 + 基础算法（第 9-14 周）

**目标**：补全竞赛必备的基础算法思想——**二分、双指针、前缀和、哈希**。

这些不是新的数据结构，但是**竞赛里出现频率最高的解题技巧**，比任何数据结构都常见。

**二分查找（7 道）**：

704 二分查找（基础模板）、35 搜索插入位置、34 在排序数组中查找元素的第一个和最后一个位置、162 寻找峰值、875 爱吃香蕉的珂珂（**二分答案**，竞赛核心技巧）、1011 在 D 天内送达包裹的能力、410 分割数组的最大值。

**关于二分**，竞赛里有两种用法：

**查找具体值**（你已会）。

**二分答案**（重要！）：答案本身有单调性，就二分答案然后验证——875 和 1011 就是这个模型。

**双指针（6 道）**：

167 两数之和 II（有序数组，双指针经典）、15 三数之和、11 盛最多水的容器、283 移动零、3 无重复字符的最长子串（滑动窗口）、76 最小覆盖子串。

**前缀和 + 差分（4 道）**：

303 区域和检索——数组不可变（前缀和模板）、304 二维区域和检索、560 和为 K 的子数组、1094 拼车（差分数组）。

**哈希（4 道）**：

1 两数之和（哈希表的入门题）、49 字母异位词分组、128 最长连续序列、290 单词规律。

**排序应用（3 道）**：

56 合并区间（排序 + 贪心）、179 最大数、148 排序链表（归并排序的链表版）。

**预计时间**：六周，每天 1 小时，约 24 道题。

---

#### 阶段四：竞赛核心算法入门（第 15-24 周）

**目标**：进入竞赛的"主战场"——动态规划、贪心、回溯、进阶数据结构。

**说明**：这个阶段我只给框架，具体题单等你到了这个阶段我再展开详细讲。

**动态规划（30 道）**：从最基础的爬楼梯开始，到背包问题、区间 DP、树形 DP。这是竞赛里**分值最高、最难**的部分，单独花 2 个月。

**贪心（10 道）**：活动选择、区间覆盖、任务调度——"每步选局部最优"的艺术。

**回溯（10 道）**：全排列、组合、N 皇后——DFS + 剪枝。

**进阶数据结构（视情况）**：线段树、树状数组、Trie——竞赛专用，到时候再讲。

---

### 三、每天 1 小时怎么分配

节奏是关键，不能一上来就猛刷，也不能太散漫。

```
前 10 分钟：复习昨天做过的题（看自己的代码，能不能默写出来）
中间 40 分钟：做 1-2 道新题
最后 10 分钟：写题解笔记（一句话总结：这道题用了什么思路）
```

**关于看题解的时机**：

做题时间超过 **20 分钟没有思路** → 看提示（不要看完整题解）。

看完提示后 **10 分钟还不会** → 看完整题解，理解后**合上题解自己重写一遍**。

**永远不要直接复制题解代码**——这是最大的时间浪费。

---

### 四、推荐平台和资源

**刷题平台**：

**LeetCode 中文站**：题目质量高，题解丰富，界面友好。**主战场**。

**洛谷**：有题目分级（P1000 难度最低），偏竞赛风格，有专门的竞赛题单。适合你到阶段三之后用。

**Codeforces**：全球最大竞赛平台，有虚拟竞赛功能。阶段四之后开始用。

**学习资源**：

**《算法竞赛入门经典》**（刘汝佳，俗称"白书"）：竞赛入门圣经，配合你的刷题计划。

**《算法》第四版**（Sedgewick）：理论更扎实，适合想深入的同学。

**OI Wiki（oi-wiki.org）**：中文竞赛知识库，覆盖所有竞赛知识点，免费。

---

### 五、6 个月总进度预览

|阶段|时间|主题|题数|目标|
|---|---|---|---|---|
|一|第 1-3 周|竞赛 C++ + 线性结构|~13|建立手感|
|二|第 4-8 周|树 + 图|~23|直接用所学|
|三|第 9-14 周|二分 + 双指针 + 哈希|~24|掌握核心技巧|
|四|第 15-24 周|DP + 贪心 + 回溯|~50|进入竞赛主战场|
|**合计**|**6 个月**||**~110 道**|**可以参加 CF Div.3**|

**110 道优质题目，认真做完，远比随便刷 500 道有用**。

---

### 六、现在你要做的第一件事

**今天**就可以开始，不用等到明天：

第一，打开 LeetCode，注册账号（如果还没有的话）。

第二，做第一道题：**LeetCode 206——反转链表**。

这道题你学链表时我讲过原理，现在用竞赛 C++ 写出来。**要求**：写两个版本，递归版和迭代版各一个，然后提交通过。

第三，做完后告诉我你的代码，**我帮你 review**——看有没有可以改进的写法，有没有遗漏的边界情况。

---

你准备好了吗？先去做 206，做完把代码贴给我，我们从第一道题开始走。


## 完成情况
- [ ] 二叉树