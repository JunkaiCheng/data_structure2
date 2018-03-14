#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <algorithm>
#include "priority_queue.h"

// OVERVIEW: A specialized version of the 'heap' ADT implemented as a binary
//           heap.
template<typename TYPE, typename COMP = std::less<TYPE> >
class binary_heap: public priority_queue<TYPE, COMP> {
public:
    typedef unsigned size_type;
    
    // EFFECTS: Construct an empty heap with an optional comparison functor.
    //          See test_heap.cpp for more details on functor.
    // MODIFIES: this
    // RUNTIME: O(1)
    binary_heap(COMP comp = COMP());
    
    // EFFECTS: Add a new element to the heap.
    // MODIFIES: this
    // RUNTIME: O(log(n))
    virtual void enqueue(const TYPE &val);
    
    // EFFECTS: Remove and return the smallest element from the heap.
    // REQUIRES: The heap is not empty.
    // MODIFIES: this
    // RUNTIME: O(log(n))
    virtual TYPE dequeue_min();
    
    // EFFECTS: Return the smallest element of the heap.
    // REQUIRES: The heap is not empty.
    // RUNTIME: O(1)
    virtual const TYPE &get_min() const;
    
    // EFFECTS: Get the number of elements in the heap.
    // RUNTIME: O(1)
    virtual size_type size() const;
    
    // EFFECTS: Return true if the heap is empty.
    // RUNTIME: O(1)
    virtual bool empty() const;
    
private:
    // Note: This vector *must* be used in your heap implementation.
    std::vector<TYPE> data;
    // Note: compare is a functor object
    COMP compare;
    
private:
    // Add any additional member functions or data you require here.
    void PercolateUp(int index);
    void PercolateDown(int index);
};


template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP> :: PercolateUp(int index) {
    index--;//change the index of binary heap to that of vector
    while (compare(data[index], data[(index-1)/2]) && index > 0)
    {
        TYPE t=data[index];
        data[index]=data[(index-1)/2];
        data[(index-1)/2]=t;
        index=(index-1)/2;
    }
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP> :: PercolateDown(int index)
{
    index--;//change the index of binary heap to that of vector
    while ((index+1)*2<=data.size())
    {
        int intT;//index of the element to swap
        if ((index+1)*2 < data.size())
        {
            if (!compare(data[index], data[(index+1)*2-1])&& !compare(data[index], data[(index+1)*2]))
            {
                if (compare(data[(index+1)*2], data[(index+1)*2-1]))
                    intT=(index+1)*2;
                else intT=(index+1)*2-1;
            }
            else if (!compare(data[index], data[(index+1)*2-1]))
                intT=(index+1)*2-1;
            else if (!compare(data[index], data[(index+1)*2]))
                intT=(index+1)*2;
            else break;
        }
        
        else if (!compare(data[index], data[(index+1)*2-1]))
            intT=(index+1)*2-1;
        else break;
        TYPE t=data[index];
        data[index]=data[intT];
        data[intT]=t;
        index=intT;
    }
}

template<typename TYPE, typename COMP>
binary_heap<TYPE, COMP> :: binary_heap(COMP comp) {
    compare = comp;
    // Fill in the remaining lines if you need.
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP> :: enqueue(const TYPE &val) {
    // Fill in the body.
    data.push_back(val);
    int index=data.size();
    PercolateUp(index);
}

template<typename TYPE, typename COMP>
TYPE binary_heap<TYPE, COMP> :: dequeue_min() {
    // Fill in the body.
    TYPE min = data[0];
    int size=data.size();
    TYPE t=data[0];
    data[0]=data[size-1];
    data[size-1]=t;
    data.pop_back();
    PercolateDown(1);
    return min;
}

template<typename TYPE, typename COMP>
const TYPE &binary_heap<TYPE, COMP> :: get_min() const {
    // Fill in the body.
    return data[0];
}

template<typename TYPE, typename COMP>
bool binary_heap<TYPE, COMP> :: empty() const {
    // Fill in the body.
    return data.empty();
}

template<typename TYPE, typename COMP>
unsigned binary_heap<TYPE, COMP> :: size() const { 
    // Fill in the body.
    return data.size();
}

#endif //BINARY_HEAP_H
