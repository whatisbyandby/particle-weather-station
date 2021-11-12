#include "CircularBuffer.h"

CircularBuffer::CircularBuffer(int size)
{
    this->size = size;
    this->buffer = new float[size];
    this->head = 0;
    this->tail = 0;
}

bool CircularBuffer::push(float value)
{
    buffer[head] = value;
    head = (head + 1) % size;
    if (head == tail)
    {
        tail = (tail + 1) % size;
    }
    return !(head < 9);
};

float CircularBuffer::pop()
{
    float value = buffer[tail];
    tail = (tail + 1) % size;
    return value;
};

bool CircularBuffer::isEmpty()
{
    return head == tail;
};

bool CircularBuffer::isFull()
{
    return (head + 1) % size == tail;
};

float CircularBuffer::getAverage()
{
    int numOfSamples = head + 1;
    float sum = 0;
    for (int i = 0; i < numOfSamples; i++)
    {
        sum += buffer[i];
    }
    return sum / numOfSamples;
};