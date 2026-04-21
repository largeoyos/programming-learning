#include <iostream>
#include <cstdio>
#include "stack.h"
#include "queue.h"
using namespace std;

//bug记录
//1.scanf没有加&
//2.scanf判断数量!= 写成==
//if()内写成=
//isStackTemp忘了加变量
//往回拿的时候检查temp非空写成park了

int main() {
    int n, m, p;
    cin >> n >> m >> p;
    Stack Park;
    Stack Temp;
    Queue Wait;
    InitStack(Park, n);
    InitStack(Temp, n - 1);
    InitQueue(Wait);
    char command;
    int number, time;
    int QueuePosition = 0;
    while (scanf(" %c %d %d", &command, &number, &time) == 3 && command != 'E') {
        switch (command) {
            case 'A': {
                if (Park.top == n - 1) {
                    EnQueue(Wait, number);
                    QueuePosition++;
                    cout << "W " << QueuePosition << endl;
                } else {
                    Car e = {number, time};
                    PushStack(Park, e);
                    cout << "P " << Park.top + 1 << endl;
                }
                break;
            }
            case 'D': {
                int e;
                while (GetTop(Park, e) && e != number) {
                    Car tem;
                    PopStack(Park, tem);
                    PushStack(Temp, tem);
                }
                Car temp;
                PopStack(Park, temp);
                int deltaTime = time - temp.time;
                cout << "time:" << deltaTime << " fee:" << p * deltaTime << endl;
                while (Temp.top != -1) {
                    Car outStack;
                    PopStack(Temp, outStack);
                    PushStack(Park, outStack);
                }
                if (!isQueueEmpty(Wait)) {
                    int s;
                    DeQueue(Wait, s);
                    Car inStack;
                    inStack.num = s;
                    inStack.time = time;
                    PushStack(Park, inStack);
                }
                break;
            }
            case 'P': {
                cout << "P:";
                TraveseStack(Park);
                cout << endl;
                break;
            }
            case 'W': {
                cout << "W:";
                TraverseQueue(Wait);
                cout << endl;
                break;
            }
        }
    }
    return 0;
}
