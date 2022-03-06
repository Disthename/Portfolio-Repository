#include <iostream>
using namespace std;
template <class T>
class Stack {
public:
    Stack() {
        data = new T[defaultCAP];
        top = -1;
    }
    Stack(int newCapacity) {
        data = new T[newCapacity];
        top = -1;
    }
    ~Stack() {
        delete[] data;
    }

    void push(T item);
    void pop();
    bool empty() { return (top == -1); }
    int size() { return (top + 1); }
    T atTop();
    bool expand(int);
    bool shrink(int);
    int getCurrentCapacity() { return CAPACITY; }
private:
    int CAPACITY = 10;
    static const int defaultCAP = 10;
    int top;
    T* data;
};
template <class T>
bool Stack<T>::expand(int newCapacity) {
    //pre-conditions:  newCapacity> CAPACITY.
    if (newCapacity > CAPACITY) {
        T* tmpArray = new T[newCapacity];
        for (int i = 0; i < size(); ++i)
            tmpArray[i] = data[i];
        delete[] data;
        CAPACITY = newCapacity;
        data = tmpArray;
        return true;
    }
    else
        return false;
}

template <class T>
void Stack<T>::push(T item) {
    //pre-conditions:  stack is not Full.
    if (top < (CAPACITY - 1)) {
        ++top;
        data[top] = item;
    }
    else
    {
        //cout << "Stack Overflow!\n";
        int newCAPACITY = CAPACITY + (CAPACITY / 4);
        if (expand(newCAPACITY)) {
            ++top;
            data[top] = item;
        }
        else
            cout << "Was not able to expand the stack!\n";
    }
}

template <class T>
bool Stack<T>::shrink(int newCapacity) {
    //pre-conditions:  newCapacity< CAPACITY.
    //post-conditions: the underlining array will be reduced by CAPACITY - newCapacity
    if (newCapacity < CAPACITY) {
        T* tmpArray = new T[newCapacity];
        for (int i = 0; i < size(); ++i)
            tmpArray[i] = data[i];
        delete[] data;
        CAPACITY = newCapacity;
        data = tmpArray;
        return true;
    }
    else
        return false;
}


template <class T>
void Stack<T>::pop() {
    //pre-conditions:  stack is not empty

    if (!empty()) {
        --top;
        if (size() < (CAPACITY / 2) && size() > defaultCAP) {
            int newCAPACITY = CAPACITY - (CAPACITY / 4);
            if (!shrink(newCAPACITY)) {
                cout << "Can not reduce the CAPACITY\n";
            }

        }
    }
    else
        cout << "Stack Underflow!\n";
}

template <class T>
T Stack<T>::atTop() {
    //pre-conditions:  stack is not empty
    if (!empty())
        return data[top];
    else {
        cout << "Stack Underflow!\n"; // throw an exception
        exit(1); //terminate the program
    }
}
