#include <iostream>

#define QUEUE_INIT_SIZE 100
#define QUEUE_INCREMENT 10
template <typename T>
struct SqQueue{
    T* base;
    int front;
    int rear;
    int queuesize;
};

