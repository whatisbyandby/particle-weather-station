
class CircularBuffer
{
private:
    int size;
    int head;
    int tail;
    float *buffer;

public:
    CircularBuffer(int size);

    // Push new value to the end of the buffer
    bool push(float value);

    // Pop value from the beginning of the buffer
    float pop();

    // Find out if the buffer is empty
    bool isEmpty();

    // Find out if the buffer is full
    bool isFull();

    // Get the average of the buffer
    float getAverage();
};
