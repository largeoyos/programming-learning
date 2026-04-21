#include "stack.h"
#include <iostream>
using namespace std;

void InitStack(Stack& S, int n) {
    S.elem = new Car[n];
    S.capacity = n;
    S.top = -1;
}

void DestroyStack(Stack& S) {
    delete[] S.elem;
    S.top = -1;
    S.capacity = 0;
}

bool GetTop(Stack& S, int& e) {
    if (S.top == -1)
        return false;
    e = S.elem[S.top].num;
    return true;
}

void PushStack(Stack& S, Car& e) {
    S.top++;
    S.elem[S.top] = e;
}

bool PopStack(Stack& S, Car& e) {
    if (S.top == -1)
        return false;
    e = S.elem[S.top];
    S.top--;
    return true;
}

void TraveseStack(Stack& S) {
    for (int i = 0; i <= S.top; i++) {
        cout << S.elem[i].num << " ";
    }
}
