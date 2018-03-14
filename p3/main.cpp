//
//  main.cpp
//  p3
//
//  Created by Cheng on 2017/11/4.
//  Copyright © 2017年 Cheng. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstring>
#include <getopt.h>

#include "priority_queue.h"
#include "binary_heap.h"
#include "unsorted_heap.h"
#include "fib_heap.h"

using namespace std;

struct point
{
    int x;
    int y;
    int pathcost;
    int cellweight;
    int reached;//1 if reached; 0 not
    point* previous=NULL;
};

struct compare_t
{
    bool operator()(point* a, point* b) const
    {
        if (a->pathcost < b->pathcost)
            return 1;
        else if (a->pathcost == b->pathcost)
        {
            if (a->x < b->x)
                return 1;
            else if (a->x==b->x && a->y < b->y)
                return 1;
            else return 0;
        }
        else return 0;
    }
};

void reaching(priority_queue<point*, compare_t> *pq, point* currentP, point* previousP);
void trace(point* p);

int main(int argc, char * argv[]) {
    int width, height, start_x, start_y, end_x, end_y;
    
    int heap=0;//1: binary; 2: unsorted; 3: fib
    int verbose=0;//1:verbose; 0: not;
    int opt;
    char *arg;
    static struct option longopts[] =
    {
        {"implementation", 1, NULL, 'i'},
        {"verbose",  0, NULL, 'v'},
        {0, 0, 0, 0}
    };
    while((opt = getopt_long(argc, argv, "i:v", longopts, NULL)) != -1)
    {
        switch(opt)
        {
            case 'i':
                arg=optarg;
                break;
            case 'v':
                verbose=1;
                break;
            case '?':
                cout << "unknown option\n";
                break;
        }
    }
    
    if (strcmp(arg, "BINARY")==0)
        heap=1;
    else if (strcmp(arg, "UNSORTED")==0)
        heap=2;
    else if (strcmp(arg, "FIBONACCI")==0)
        heap=3;
    
    
    
    cin >> width >> height >> start_x >> start_y >> end_x >> end_y;
    point* points = new point[width*height];
    int n=0;
    for (int i=0; i < height; ++i)
    {
        for (int j=0; j < width; ++j)
        {
            points[n].x=j;
            points[n].y=i;
            cin >> points[n].cellweight;
            points[n].reached=0;
            ++n;
        }
    }
    priority_queue<point*, compare_t> *pq;
    if (heap==1)
        pq= new binary_heap<point*, compare_t>;
    else if (heap==2)
        pq = new unsorted_heap<point*, compare_t>;
    else
        pq = new fib_heap<point*, compare_t>;
    
//    point* start_point=new point;
//    start_point=&points[start_y*width+start_x];
    point* start_point=&points[start_y*width+start_x];
//    point* end_point=new point;
//    end_point=&points[end_y*width+end_x];
    point* end_point=&points[end_y*width+end_x];
    //point end_point=points[end_y*width+end_x];
    start_point->pathcost=start_point->cellweight;
    start_point->reached=1;
    pq->enqueue(start_point);
    point* currP=start_point;//current point
    
    int pathlength=0;
    if (verbose==0){
        while(!pq->empty())
        {
            currP=pq->dequeue_min();
            int x=currP->x;
            int y=currP->y;
            if (x!=width-1 && points[y*width+x+1].reached==0)
            {
                reaching(pq, &points[y*width+x+1], currP);
                points[y*width+x+1].reached=1;
                if (x==end_x-1 && y==end_y)
                {
                    pathlength=currP->pathcost+points[end_y*width+end_x].cellweight;
                    break;
                }
            }
            
            if (y!=height-1 && points[y*width+x+width].reached==0)
            {
                reaching(pq, &points[y*width+x+width], currP);
                points[y*width+x+width].reached=1;
                if (x==end_x && y==end_y-1)
                {
                    pathlength=currP->pathcost+points[end_y*width+end_x].cellweight;
                    break;
                }
            }
            if (x!=0 && points[y*width+x-1].reached==0)
            {
                reaching(pq, &points[y*width+x-1], currP);
                points[y*width+x-1].reached=1;
                if (x==end_x+1 && y==end_y)
                {
                    pathlength=currP->pathcost+points[end_y*width+end_x].cellweight;
                    break;
                }
            }
            if (y!=0 && points[y*width+x-width].reached==0)
            {
                reaching(pq, &points[y*width+x-width], currP);
                points[y*width+x-width].reached=1;
                if (x==end_x && y==end_y+1)
                {
                    pathlength=currP->pathcost+points[end_y*width+end_x].cellweight;
                    break;
                }
            }
            
        }
    }
    if (verbose==1)
    {
        int step=0;
        while(!pq->empty())
        {
            string outstep="Step "+to_string(step)+'\n';
            //cout << "Step " << step << '\n';
            cout << outstep;
            currP=pq->dequeue_min();
            int x=currP->x;
            int y=currP->y;
            string outCell="Choose cell (" +to_string(x) + ", " + to_string(y) + ") with accumulated length " + to_string(currP->pathcost) + ".\n";
            cout << outCell;
            if (x!=width-1 && points[y*width+x+1].reached==0)
            {
                reaching(pq, &points[y*width+x+1], currP);
                points[y*width+x+1].reached=1;
                if (x==end_x-1 && y==end_y)
                {
                    pathlength=currP->pathcost+points[end_y*width+end_x].cellweight;
                    string outEnd="Cell ("+ to_string(x+1)+", "+ to_string(y)+ ") with accumulated length " + to_string(points[y*width+x+1].pathcost) + " is the ending point.\n";
                    cout << outEnd;
                    break;
                }
                string outReach="Cell ("+ to_string(x+1)+", "+ to_string(y)+ ") with accumulated length " + to_string(points[y*width+x+1].pathcost) + " is added into the queue.\n";
                cout << outReach;
            }
            
            if (y!=height-1 && points[y*width+x+width].reached==0)
            {
                reaching(pq, &points[y*width+x+width], currP);
                points[y*width+x+width].reached=1;
                if (x==end_x && y==end_y-1)
                {
                    pathlength=currP->pathcost+points[end_y*width+end_x].cellweight;
                    string outEnd="Cell ("+ to_string(x)+", "+ to_string(y+1)+ ") with accumulated length " + to_string(points[y*width+x+width].pathcost) + " is the ending point.\n";
                    cout << outEnd;
                    break;
                }
                string outReach="Cell ("+ to_string(x)+", "+ to_string(y+1)+ ") with accumulated length " + to_string(points[y*width+x+width].pathcost) + " is added into the queue.\n";
                cout << outReach;
            }
            if (x!=0 && points[y*width+x-1].reached==0)
            {
                reaching(pq, &points[y*width+x-1], currP);
                points[y*width+x-1].reached=1;
                if (x==end_x+1 && y==end_y)
                {
                    pathlength=currP->pathcost+points[end_y*width+end_x].cellweight;
                    string outEnd="Cell ("+ to_string(x-1)+", "+ to_string(y)+ ") with accumulated length " + to_string(points[y*width+x-1].pathcost) + " is the ending point.\n";
                    cout << outEnd;
                    break;
                }
                string outReach="Cell ("+ to_string(x-1)+", "+ to_string(y)+ ") with accumulated length " + to_string(points[y*width+x-1].pathcost) + " is added into the queue.\n";
                cout << outReach;
            }
            if (y!=0 && points[y*width+x-width].reached==0)
            {
                
                reaching(pq, &points[y*width+x-width], currP);
                points[y*width+x-width].reached=1;
                if (x==end_x && y==end_y+1)
                {
                    pathlength=currP->pathcost+points[end_y*width+end_x].cellweight;
                    string outEnd="Cell ("+ to_string(x)+", "+ to_string(y-1)+ ") with accumulated length " + to_string(points[y*width+x-width].pathcost) + " is the ending point.\n";
                    cout << outEnd;
                    break;
                }
                string outReach="Cell ("+ to_string(x)+", "+ to_string(y-1)+ ") with accumulated length " + to_string(points[y*width+x-width].pathcost) + " is added into the queue.\n";
                cout << outReach;
            }
            ++step;
        }
    }
    string out;
    out = "The shortest path from (" + to_string(start_x) + ", " + to_string(start_y) + ") to (" + to_string(end_x) + ", " + to_string(end_y) + ") is " + to_string(pathlength) + ".\nPath:\n" ;
    cout << out;
    
    trace(end_point);
    
    
    //   delete start_point;
    //   delete end_point;
    delete pq;
    delete []points;
    return 0;
}

void reaching(priority_queue<point*, compare_t> *pq, point* currentP, point* previousP)
{
    currentP->pathcost=previousP->pathcost+currentP->cellweight;
    pq->enqueue(currentP);
    currentP->previous=previousP;
}

void trace(point *p)
{
    if (p->previous!=NULL)
    {
        trace(p->previous);
    }
    string out;
    out="(" + to_string(p->x) + ", " + to_string(p->y)+")\n";
    cout << out;
}






