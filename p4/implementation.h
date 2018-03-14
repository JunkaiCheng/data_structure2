//
//  order.h
//  p4
//
//  Created by Cheng on 2017/11/25.
//  Copyright © 2017年 Cheng. All rights reserved.
//

#ifndef order_h
#define order_h

#include <string>
#include <algorithm>
#include <queue>


using namespace std;

struct order{
    int ID;
    string c_name;
    int type; //0:buy; 1:sell
    string equity;
    int price;
    int quantity;
    int etime;//expire time
    
    bool operator < (const order &a) const
    {
        if(price==a.price)
            return ID>a.ID;
        else
        {
            if (type==1)
                return price > a.price;
            else
                return price < a.price;
        }
    }
};


struct Client{
    string name;
    int buy;
    int sell;
    int transfer;
};

struct Trade{
    string equity;
    int traded_price;
    int time;
    bool operator < (const Trade &a) const
    {
        return traded_price<a.traded_price;
    }
};

struct Median
{
    priority_queue<int, vector<int>, greater<int> > minHeap;
    priority_queue<int, vector<int>, less<int> > maxHeap;
    int median;
    int number;
};




#endif /* order_h */
