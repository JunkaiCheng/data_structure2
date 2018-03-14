#ifndef FIB_HEAP_H
#define FIB_HEAP_H

#include <algorithm>
#include <cmath>
#include "priority_queue.h"


// OVERVIEW: A specialized version of the 'heap' ADT implemented as a
//           Fibonacci heap.
template<typename TYPE, typename COMP = std::less<TYPE> >
class fib_heap: public priority_queue<TYPE, COMP> {
public:
    typedef unsigned size_type;
    
    // EFFECTS: Construct an empty heap with an optional comparison functor.
    //          See test_heap.cpp for more details on functor.
    // MODIFIES: this
    // RUNTIME: O(1)
    fib_heap(COMP comp = COMP());
    
    // EFFECTS: Add a new element to the heap.
    // MODIFIES: this
    // RUNTIME: O(1)
    ~fib_heap();
    virtual void enqueue(const TYPE &val);
    
    // EFFECTS: Remove and return the smallest element from the heap.
    // REQUIRES: The heap is not empty.
    // MODIFIES: this
    // RUNTIME: Amortized O(log(n))
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
    // Note: compare is a functor object
    COMP compare;
    
private:
    // Add any additional member functions or data you require here.
    // You may want to define a strcut/class to represent nodes in the heap and a
    // pointer to the min node in the heap.
    
    struct node {
        TYPE key;
        int degree;
        node *prev;
        node *next;
        node *child;
        node *parent;
    };
    
    node* min;
    size_type num;//number of nodes
    
    virtual void link(node* nodeY, node* nodeX);
    virtual void addNode(node* newNode);
    virtual void removeNode(node* node);
    virtual void consolidate();
    virtual node* fetchMin();
};

// Add the definitions of the member functions here. Please refer to
// binary_heap.h for the syntax.

template<typename TYPE, typename COMP>
fib_heap<TYPE, COMP> :: fib_heap(COMP comp)
{
    compare = comp;
    num=0;
    min=NULL;
}



template<typename TYPE, typename COMP>
fib_heap<TYPE, COMP> ::~fib_heap()
{
//            while (!min){
//                node* victim = fetchMin();
//                delete victim;
//            }
    while(num!=0)
    {
        dequeue_min();
    }
}

template<typename TYPE, typename COMP>
typename fib_heap<TYPE, COMP>:: node* fib_heap<TYPE, COMP> :: fetchMin()
{
    node* minN = min;
    if (minN == minN->next)
        min=NULL;
    else
    {
        removeNode(minN);
        min=minN->next;
    }
    minN->prev = minN;
    minN->next = minN;
    return minN;
}


template<typename TYPE, typename COMP>
void fib_heap<TYPE, COMP> :: enqueue(const TYPE &val)
{
    node* newNode=new node;
    newNode->degree=0;
    newNode->parent=NULL;
    newNode->child=NULL;
    newNode->key=val;
    if(min==NULL)
    {
        min=newNode;
        min->next=min;
        min->prev=min;
    }
    else
    {
        newNode->next=min->next;
        newNode->prev=min;
        min->next->prev=newNode;
        min->next=newNode;
        if (compare(newNode->key, min->key))
            min=newNode;
    }
    ++num;
}

template<typename TYPE, typename COMP>
void fib_heap<TYPE, COMP> :: addNode(node* newNode)
{
    newNode->parent=NULL;
    newNode->next=min->next;
    newNode->prev=min;
    min->next->prev=newNode;
    min->next=newNode;
}

template<typename TYPE, typename COMP>
void fib_heap<TYPE, COMP> :: removeNode(node* node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    //    node->next=node;
    //    node->prev=node;
}


template<typename TYPE, typename COMP>
void fib_heap<TYPE, COMP> :: link(node* nodeY, node* nodeX)
{
    removeNode(nodeY);
    nodeY->parent=nodeX;
    if (nodeX->child==NULL)
        nodeX->child=nodeY;
    else
    {
        //        node* childX=new node;
        //        childX=nodeX->child;
        //
        //        nodeY->next=childX->next;
        //        nodeY->prev=childX;
        //        childX->next->prev=nodeY;
        //        childX->next=nodeY;
        
        nodeY->next=nodeX->child->next;
        nodeY->prev=nodeX->child;
        nodeX->child->next->prev=nodeY;
        nodeX->child->next=nodeY;
        
    }
    ++nodeX->degree;
}
template<typename TYPE, typename COMP>
TYPE fib_heap<TYPE, COMP> :: dequeue_min()
{
    TYPE r=min->key;
    node* minN=min;
    //    if (min->child!=NULL)
    //    {
    //        node* n=new node;
    //        n=min->child;
    //        //node* n=min->child;
    //        do{
    ////            node* newNode=new node;
    ////            newNode->child=n->child;
    ////            newNode->key=n->key;
    ////            newNode->degree=n->degree;
    ////            addNode(newNode);
    //            removeNode(n);
    //            addNode(n);
    //            n=n->next;
    //        }while(n!=min->child);
    //        //delete n;
    //    }
    node* n = minN;
    while (minN->child!=NULL)
    {
        n = minN->child;
        removeNode(n);
        if (n->next == n)
            minN->child = NULL;
        else
            minN->child = n->next;
        addNode(n);
    }
    if (minN!=NULL)
    {
        minN->prev->next = minN->next;
        minN->next->prev = minN->prev;
    }
    if (num==1)
    {
        num--;
        min=NULL;
    }
    else
    {
        //        //        node* temp=new node;
        //        //        temp=min->next;
        //        node* temp=min->next;
        //        removeNode(min);
        //        delete min;
        //        min=temp;
        //        //delete temp;
        //        num--;
        //        consolidate();
        //        return r;
        num--;
        min=minN->next;
        consolidate();
    }
    delete minN;
    return r;
}


template<typename TYPE, typename COMP>
void fib_heap<TYPE, COMP> :: consolidate()
{
    const float arg = static_cast<float>(num);
    int max = log(arg)/log(1.5) + 1;
    node** A = new node*[max + 1];
    //    node**  A =new node*[num];
    for (int i=0; i<max+1; ++i)
        A[i]=NULL;
    //node* w=new node;
    //    node* w;
    //    w=min;
    //    int numR=0;//num of nodes in the root list
    //    do{
    //        numR++;
    //        w=w->next;
    //    }while(w!=min);
    //node*x =new node;
    //    node* x;
    //    x=w;
    //    for (int k=0;k<numR;k++)
    //    {
    //        int d=x->degree;
    //        while(A[d]!=NULL)
    //        {
    //            //            node* y=new node;
    //            //            y=A[d];
    //            node* y=A[d];
    //            if(compare(y->key, x->key))
    //            {
    //                node* tt;
    //                //node* tt=new node;
    //                tt=x;
    //                x=y;
    //                y=tt;
    //                //delete tt;
    //            }
    //            removeNode(y);
    //            link(y,x);
    //            A[d]=NULL;
    //            d++;
    //        }
    //        A[d]=x;
    //        x=x->next;
    //    }
    while(min != NULL){
        node* x = fetchMin();
        int d = x->degree;
        while(A[d] != NULL){
            node* y = A[d];
            if (compare(y->key, x->key))
            {
                node* tt;
                tt=x;
                x=y;
                y=tt;
            }
            link(y, x);
            A[d]=NULL;
            ++d;
        }
        A[d] = x;
    }
    min=NULL;
    for (int i=0; i<max+1; ++i)
    {
        if(A[i]!=NULL)
        {
            if(min==NULL)
            {
                //                node* newMin=new node;
                //                newMin=A[i];
                //                newMin->parent=NULL;
                //                newMin->next=A[i];
                //                newMin->prev=A[i];
                //                min=newMin;
                //                A[i]->parent=NULL;
                //                A[i]->next=A[i];
                //                A[i]->prev=A[i];
                min=A[i];
            }
            else
            {
                //                node* newMin=new node;
                //                newMin=A[i];
                //                addNode(newMin);
                //                if(compare(A[i]->key, min->key))
                //                    min=newMin;
                addNode(A[i]);
                if(compare(A[i]->key, min->key))
                    min=A[i];
            }
        }
        
    }
    delete []A;
}


template<typename TYPE, typename COMP>
const TYPE &fib_heap<TYPE, COMP> ::get_min() const
{
    return min->key;
}


template<typename TYPE, typename COMP>
unsigned fib_heap<TYPE, COMP> :: size() const
{
    return num;
}


template<typename TYPE, typename COMP>
bool fib_heap<TYPE, COMP> :: empty() const
{
    if (num==0)
        return 1;
    else
        return 0;
}

#endif //FIB_HEAP_H



