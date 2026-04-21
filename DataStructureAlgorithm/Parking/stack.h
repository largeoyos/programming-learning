#pragma once

struct Car {
    int num;
    int time;
};

struct Stack {
    Car* elem;
    int capacity;
    int top;
};

void InitStack(Stack& S, int n);
void DestroyStack(Stack& S);
bool GetTop(Stack& S, int& e);
void PushStack(Stack& S, Car& e);
bool PopStack(Stack& S, Car& e);
void TraveseStack(Stack& S);
