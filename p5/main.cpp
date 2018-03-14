//
//  main.cpp
//  p5
//
//  Created by Cheng on 2017/12/16.
//  Copyright © 2017年 Cheng. All rights reserved.
//

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include "Implement.h"
#include "algorithm"

using namespace std;
vector<vertex> vertices;
vector<edge> edges;


void Dijkstra(int sNode, int dNode);
void DAG();
void Kruskal();

int main() {
    int num, sNode, dNode;
    
    string line;
    
    cin >> num >> sNode >> dNode;
    
    for (int i=0; i<num; ++i)
    {
        vertex newvertex=vertex(i);
        vertices.push_back(newvertex);
    }
    getline(cin, line);
    while (getline(cin, line))
    {
        int start, end, weight;
        stringstream line_ss;
        line_ss << line;
        line_ss >> start >> end >> weight;
        edge newedge(start, end, weight);
        edges.push_back(newedge);
        vertices[start].addEdge(newedge);
        vertices[end].addEdge(newedge);
    }
    Dijkstra(sNode,dNode);
    DAG();
    Kruskal();
    
    return 0;
}

void Dijkstra(int sNode, int dNode)
{
    vertices[sNode].distance=0;
    while(1){
        int min_node = -1;
        int min = -1;
        for (int i=0; i<vertices.size(); ++i)
        {
            const vertex current=vertices[i];
            if (!current.known){
                if (min<0 && current.distance>=0)
                {
                    min=current.distance;
                    min_node=i;
                }
                else if (min >=0 && current.distance >=0 && current.distance<min)
                {
                    min=current.distance;
                    min_node=i;
                }
            }
        }
        if(min==-1)
            break;
        
        vertices[min_node].known=1;
        for (int i=0; i<vertices[min_node].outEdges.size(); ++i)
        {
            int current=vertices[min_node].outEdges[i].end;
            if (!vertices[current].known)
            {
                int neighbour=vertices[min_node].outEdges[i].end;
                if((vertices[neighbour].distance>vertices[min_node].distance+vertices[min_node].outEdges[i].weight && vertices[neighbour].distance>=0) || vertices[neighbour].distance<0)
                {
                    vertices[neighbour].distance=vertices[min_node].distance+vertices[min_node].outEdges[i].weight;
                    vertices[neighbour].predecessor=min_node;
                }
            }
        }
    }
    //return vertices[dNode].distance;
    if (vertices[dNode].distance < 0)
        cout<<"No path exists!\n";
    else
        cout << "Shortest path length is " << vertices[dNode].distance<<"\n";
}

void DAG(){
    
    int visitNum=0;
    queue<vertex> topo;
    for (int i=0; i<vertices.size(); ++i)
    {
        if (vertices[i].iDegree==0)
            topo.push(vertices[i]);
    }
    while(!topo.empty())
    {
        vertex current=topo.front();
        topo.pop();
        ++visitNum;
        for (int i=0; i<current.outEdges.size(); ++i)
        {
            int index=current.outEdges[i].end;
            --vertices[index].iDegree;
            if (vertices[index].iDegree==0)
                topo.push(vertices[index]);
        }
    }
    if (visitNum==vertices.size())
        cout << "The graph is a DAG\n";
    else
        cout << "The graph is not a DAG\n";
}

void Kruskal(){
    sort(edges.begin(), edges.end(), edge_inc);
    int n=vertices.size();
    int weights=0;
    int k=0;
    vector<int> unions;
    for (int i=0; i<n; ++i) {
        unions.push_back(i);
    }
    for (int i=0; i<edges.size(); ++i) {
        int s=edges[i].start;
        int e=edges[i].end;
        while (unions[s]!=s)
            s=unions[s];
        while (unions[e]!=e)
            e=unions[e];
        if (s!=e)
        {
            weights=weights+edges[i].weight;
            unions[s]=e;
            ++k;
        }
    }
    if (k==n - 1)
        cout << "The total weight of MST is " << weights << "\n";
    else
        cout << "No MST exists!\n";
}
















