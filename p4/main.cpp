//
//  main.cpp
//  p4
//
//  Created by Cheng on 2017/11/25.
//  Copyright © 2017年 Cheng. All rights reserved.
//


#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <getopt.h>
#include <set>
#include <map>
#include <queue>
#include "implementation.h"

using namespace std;

map<string, priority_queue<order>> sell;
map<string, priority_queue<order>> buy;
map<string, Client> Clients;
map<string, multiset<Trade>> Trades;
map<string, Median> Medians;
int commission;
int total;
int num_trade;
int num_shares;

void newday(int median, int midpoint, int verbose);
void median_output(int ctime);
void midpoint_output(int ctime);
void transfer_output();
void ttt_output(string* args, int ttt);
void insert_order(order neworder, int type, string equity);
bool matchOrder(order neworder, int ctime);
void trading(const order buyer, const order seller, const int quantity, const int price, const int verbose, int ctime);


int main(int argc, char * argv[]) {
    commission=0;
    total=0;
    num_trade=0;
    num_shares=0;
    int verbose=0;//1:verbose; 0: not;
    int median=0;//1: median; 0: not;
    int midpoint=0;//1: midpoint; 0: not;
    int transfer=0;//1: show transfer; 0: not;
    int ttt=0;//number of ttt
    
    int opt;
    string *args=new string[64];

    static struct option longopts[] =
    {
        {"verbose",  0, NULL, 'v'},
        {"median", 0, NULL, 'm'},
        {"midpoint", 0, NULL, 'p'},
        {"transfers", 0, NULL, 't'},
        {"ttt", 1, NULL, 'g'},
        {0, 0, 0, 0}
    };
    while((opt = getopt_long(argc, argv, "vmptg:", longopts, NULL)) != -1)
    {
        switch(opt)
        {
            case 'g':
                args[ttt]=optarg;
                ++ttt;
                break;
            case 'v':
                verbose=1;
                break;
            case 'm':
                median=1;
                break;
            case 'p':
                midpoint=1;
                break;
            case 't':
                transfer=1;
                break;
            case '?':
                cout << "unknown option\n";
                break;
        }
    }
    //median=1;
    //midpoint=1;
    
    newday(median, midpoint, verbose);
    
    cout << "---End of Day---\nCommission Earnings: $" << commission << "\nTotal Amount of Money Transferred: $" << total << "\nNumber of Completed Trades: " << num_trade << "\nNumber of Shares Traded: "<<num_shares<<'\n';
    
    if (transfer)
        transfer_output();
    if (ttt!=0)
        ttt_output(args, ttt);
    delete []args;
    return 0;
}

void newday(int median, int midpoint, int verbose)
{//start marketing
    int ctime=0;//current timestamp
    int id=0;//ID for order
    string line;
    while (getline(cin, line))
    {
        stringstream line_ss;
        line_ss<<line;
        int time, duration, type, price, quantity;
        string client, type_s, equity, price_s, quantity_s, duration_s;
        line_ss >> time >> client >> type_s >> equity >> price_s >> quantity_s >> duration;
       
        if (type_s == "BUY")
            type = 0;
        else type = 1;
        quantity_s=quantity_s.substr(1, quantity_s.length()-1);
        quantity=atoi(quantity_s.c_str());
        price_s=price_s.substr(1, price_s.length()-1);
        price=atoi(price_s.c_str());
        
        
        if (ctime!=time)
        {
            if (median)
                median_output(ctime);
            if (midpoint)
                midpoint_output(ctime);
            ctime=time;
        }
        int ifinsert=1;
        
        
        int ex_time;
        if (duration>0)
            ex_time=ctime+duration;
        else ex_time=-1;
        order neworder={id, client, type, equity, price, quantity, ex_time};
        while(matchOrder(neworder, ctime))
        {
            if (type==0)
            {
                int cl_q=neworder.quantity;
                int cu_q=sell.find(equity)->second.top().quantity;
            
                if (cl_q < cu_q)
                {
                    ifinsert=0;
                    order temp =sell.find(equity)->second.top();
                    temp.quantity=cu_q-cl_q;
                    sell.find(equity)->second.pop();
                    sell.find(equity)->second.push(temp);
                    trading(neworder, sell.find(equity)->second.top(), cl_q, sell.find(equity)->second.top().price, verbose, ctime);
                    break;
                }
                else if (cl_q == cu_q)
                {
                    ifinsert=0;
                    trading(neworder, sell.find(equity)->second.top(), cl_q, sell.find(equity)->second.top().price, verbose, ctime);
                    sell.find(neworder.equity)->second.pop();
                    break;
                }
                else if (cl_q > cu_q)
                {
                    neworder.quantity=cl_q-cu_q;
                    trading(neworder, sell.find(equity)->second.top(), cu_q, sell.find(equity)->second.top().price, verbose, ctime);
                    sell.find(neworder.equity)->second.pop();
                }
            }
            else
            {
                int cl_q=neworder.quantity;
                int cu_q=buy.find(equity)->second.top().quantity;
                if (cl_q < cu_q)
                {
                    ifinsert=0;
                    order temp =buy.find(equity)->second.top();
                    temp.quantity=cu_q-cl_q;
                    buy.find(equity)->second.pop();
                    buy.find(equity)->second.push(temp);
                    trading(buy.find(equity)->second.top(), neworder, cl_q, buy.find(equity)->second.top().price, verbose, ctime);
                    break;
                }
                else if (cl_q == cu_q)
                {
                    ifinsert=0;
                    trading(buy.find(equity)->second.top(), neworder, cl_q, buy.find(equity)->second.top().price, verbose, ctime);
                    buy.find(neworder.equity)->second.pop();
                    break;
                }
                else if (cl_q > cu_q)
                {
                    neworder.quantity=cl_q-cu_q;
                    trading(buy.find(equity)->second.top(), neworder, cu_q, buy.find(equity)->second.top().price, verbose, ctime);
                    buy.find(neworder.equity)->second.pop();
                }
            }
            if (duration==0)
            {
                ifinsert=0;
                break;
            }
        }
        if (duration==0)
            ifinsert=0;
        if (ifinsert==1)
            insert_order(neworder, type, equity);
        ++id;
    }
    if (median)
        median_output(ctime);
    if (midpoint)
        midpoint_output(ctime);
}

void insert_order(order neworder, int type, string equity)
{
    if (type==0)
    {
        map<string, priority_queue<order>>::iterator it = buy.find(equity);
        if (it!=buy.end())
            it->second.push(neworder);
        else
        {
            priority_queue<order> new_pq;
            new_pq.push(neworder);
            buy.insert(pair<string, priority_queue<order>>(equity, new_pq));
        }
    }
    else
    {
        map<string, priority_queue<order>>::iterator it = sell.find(equity);
        if (it!=sell.end())
            it->second.push(neworder);
        else
        {
            priority_queue<order> new_pq;
            new_pq.push(neworder);
            sell.insert(pair<string, priority_queue<order>>(equity, new_pq));
        }
    }
}

void median_output(int ctime)
{//output for median price
    stringstream ctime_ss;
    string ctime_str;
    ctime_ss<<ctime;
    ctime_ss>>ctime_str;
 
    map<string, Median>::iterator it=Medians.begin();
    while(it!=Medians.end())
    {
        string output;
        int median_price=it->second.median;
        stringstream median_price_ss;
        string median_price_str;
        median_price_ss<<median_price;
        median_price_ss>>median_price_str;
        output="Median match price of " + it->first + " at time " + ctime_str + " is $"+ median_price_str + '\n';
        cout << output;
        ++it;
    }
}


void midpoint_output(int ctime)
{//output for midpoint price
    stringstream ctime_ss;
    string ctime_str;
    ctime_ss<<ctime;
    ctime_ss>>ctime_str;
    
    int highestBuy;
    int lowestSell;
    int midPrice;
    map<string,priority_queue<order>>::iterator it1;
    map<string,priority_queue<order>>::iterator it2;
    it1 = buy.begin();
    it2 = sell.begin();
    
    while(it1 != buy.end() && it2 != sell.end())
    {
        if (it1->first < it2->first)
        {
            string output="Midpoint of "+it1->first+" at time "+ctime_str+" is undefined\n";
            cout<<output;
            ++it1;
        }
        else if (it1->first > it2->first)
        {
            string output="Midpoint of "+it2->first+" at time "+ctime_str+" is undefined\n";
            cout<<output;
            ++it2;
        }
        else
        {
            while(!it1->second.empty() && it1->second.top().etime<=ctime && it1->second.top().etime>=0)
                it1->second.pop();
            while(!it2->second.empty() && it2->second.top().etime<=ctime && it2->second.top().etime>=0)
                it2->second.pop();
            if (it1->second.empty() || it2->second.empty())
            {
                string output="Midpoint of "+it1->first+" at time "+ctime_str+" is undefined\n";
                cout<<output;
            }
            else
            {
                highestBuy=it1->second.top().price;
                lowestSell=it2->second.top().price;
                midPrice=(highestBuy+lowestSell)/2;
                stringstream midPrice_ss;
                string midPrice_str;
                midPrice_ss<<midPrice;
                midPrice_ss>>midPrice_str;
                string output="Midpoint of "+it1->first+" at time "+ctime_str+" is $" + midPrice_str + '\n';
                cout << output;
            }
            ++it1;
            ++it2;
        }
    }
 
    while(it1 != buy.end())
    {
        string output="Midpoint of "+it1->first+" at time "+ctime_str+" is undefined\n";
        cout<<output;
        ++it1;
    }
    while(it2 != sell.end())
    {
        string output="Midpoint of "+it2->first+" at time "+ctime_str+" is undefined\n";
        cout<<output;
        ++it2;
    }
}



bool matchOrder(order neworder, int ctime)
{//whether there is an order in data structure matches the current order
    if (neworder.type==0)
    {
        if (sell.find(neworder.equity)==sell.end() || sell.find(neworder.equity)->second.empty())
            return false;
        else
        {
            while(!sell.find(neworder.equity)->second.empty()
                  && sell.find(neworder.equity)->second.top().etime<=ctime
                  && sell.find(neworder.equity)->second.top().etime>=0)
                sell.find(neworder.equity)->second.pop();
            if(sell.find(neworder.equity)->second.empty())
                return false;
            order customer=sell.find(neworder.equity)->second.top();
            if (customer.price<=neworder.price)
                return true;
            else
                return false;
        }
    }
    else
    {
        if (buy.find(neworder.equity)==buy.end() || buy.find(neworder.equity)->second.empty())
            return false;
        else
        {
            while(!buy.find(neworder.equity)->second.empty()
                  &&buy.find(neworder.equity)->second.top().etime<=ctime
                  && buy.find(neworder.equity)->second.top().etime>=0)
                buy.find(neworder.equity)->second.pop();
            if(buy.find(neworder.equity)->second.empty())
                return false;
            order customer=buy.find(neworder.equity)->second.top();
            if (customer.price>=neworder.price)
                return true;
            else
                return false;
        }
    }
}


void trading(const order buyer, const order seller, const int quantity, const int price, const int verbose, int ctime)
{//add traded element into Trades and Clients
    commission+=quantity*price/100+quantity*price/100;
    total+=quantity*price;
    num_shares+=quantity;
    ++num_trade;

    if (verbose)
    {
        stringstream q_ss, p_ss;
        string q_str, p_str;
        
        q_ss<<quantity;
        p_ss<<price;
        
        q_ss>>q_str;
        p_ss>>p_str;
        
        string output= buyer.c_name + " purchased " + q_str  + " shares of " + buyer.equity  + " from " + seller.c_name + " for $" + p_str  +"/share\n";
        cout<< output;
    }
    map<string, Median>::iterator it_m = Medians.find(buyer.equity);
    map<string, multiset<Trade>>::iterator it1 =Trades.find(buyer.equity);
    map<string, Client>::iterator it2 =Clients.find(buyer.c_name);
    map<string, Client>::iterator it3 =Clients.find(seller.c_name);
    
    if (it_m==Medians.end())
    {
        Median new_m;
        new_m.median=price;
        new_m.maxHeap.push(price);
        new_m.number=1;
        Medians.insert(pair<string, Median>(buyer.equity, new_m));
    }
    else
    {
        if (it_m->second.number%2==0)
        {
            if (price <= it_m->second.minHeap.top())
            {
                it_m->second.maxHeap.push(price);
                it_m->second.median=it_m->second.maxHeap.top();
            }
            else
            {
                int temp=it_m->second.minHeap.top();
                it_m->second.minHeap.pop();
                it_m->second.minHeap.push(price);
                it_m->second.maxHeap.push(temp);
                it_m->second.median=it_m->second.maxHeap.top();
            }
        }
        else
        {
            if (price >= it_m->second.maxHeap.top())
            {
                it_m->second.minHeap.push(price);
                it_m->second.median=(it_m->second.maxHeap.top()+it_m->second.minHeap.top())/2;
            }
            else
            {
                int temp=it_m->second.maxHeap.top();
                it_m->second.maxHeap.pop();
                it_m->second.maxHeap.push(price);
                it_m->second.minHeap.push(temp);
                it_m->second.median=(it_m->second.maxHeap.top()+it_m->second.minHeap.top())/2;
            }
        }
        ++it_m->second.number;
    }
    

    
    if (it2==Clients.end())
    {
        Client new_client;
        new_client.name=buyer.c_name;
        new_client.buy=quantity;
        new_client.sell=0;
        new_client.transfer=0-quantity*price;
        Clients.insert(pair<string, Client>(buyer.c_name, new_client));
    }
    else
    {
        it2->second.buy+=quantity;
        it2->second.transfer-=(quantity*price);
    }
    
    if (it3==Clients.end())
    {
        Client new_client;
        new_client.name=seller.c_name;
        new_client.sell=quantity;
        new_client.buy=0;
        new_client.transfer=quantity*price;
        Clients.insert(pair<string, Client>(seller.c_name, new_client));
    }
    else
    {
        it3->second.sell+=quantity;
        it3->second.transfer+=(quantity*price);
    }
}

void transfer_output()
{//output for transfer
    map<string, Client>::iterator it =Clients.begin();
    while(it!=Clients.end())
    {
        Client temp=it->second;
        stringstream buy_ss, sell_ss, trans_ss;
        string buy_str, sell_str, trans_str;
        buy_ss<<temp.buy;
        buy_ss>>buy_str;
        sell_ss<<temp.sell;
        sell_ss>>sell_str;
        trans_ss<<temp.transfer;
        trans_ss>> trans_str;
        
        string output= temp.name+ " bought " + buy_str + " and sold " + sell_str+ " for a net transfer of $" + trans_str + '\n';
        cout << output;
        ++it;
    }

}

void ttt_output(string* args, int ttt)
{//output for ttt
    for (int i=0; i<ttt; ++i)
    {
        int to_buy, to_sell;
        map<string, multiset<Trade>>::iterator it =Trades.find(args[i]);
        multiset<Trade>::iterator ite;
        ite=it->second.begin();
        to_buy=ite->time;
        ite=it->second.end();
        to_sell=ite->time;
        
        stringstream buy_ss, sell_ss;
        string buy_str, sell_str;
        
        buy_ss<<to_buy;
        sell_ss<<to_sell;
        
        buy_ss>>buy_str;
        sell_ss>>sell_str;
        
        string output="Time travelers would buy " + args[i] + " at time: " + buy_str + " and sell it at time: " + sell_str + '\n';
        cout << output;

    }
}




