//
//  Implement.h
//  p5
//
//  Created by Cheng on 2017/12/16.
//  Copyright © 2017年 Cheng. All rights reserved.
//

#ifndef Implement_h
#define Implement_h

#include <vector>
using namespace std;


class edge{
public:
    int weight;
    int start;
    int end;

    edge(int s, int e, int w);
};

class vertex{
public:
    int index;
    int iDegree;
    //vector<edge> inEdges;
    vector<edge> outEdges;

    int visited;
    int distance;
    int predecessor;
    int known;
    vertex(int n);
    
    void addEdge(edge newEdge);
};

bool edge_inc(edge a, edge b)
{
    return a.weight < b.weight;
}

edge::edge(int s, int e, int w){
    start=s;
    end=e;
    weight=w;
}

vertex::vertex(int n){
    index=n;
    visited =0;
    distance=-1;
    predecessor=-1;
    known=0;
    iDegree=0;
}

void vertex::addEdge(edge newEdge)
{
    if (index==newEdge.start)
        outEdges.push_back(newEdge);
    else if (index==newEdge.end){
        //inEdges.push_back(newEdge);
        ++iDegree;
    }
}

#endif /* Implement_h */
