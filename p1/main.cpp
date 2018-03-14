//
//  main.cpp
//  project1
//
//  Created by Cheng on 2017/9/26.
//  Copyright © 2017年 Cheng. All rights reserved.
//

#include <iostream>
#include <cmath>

using namespace std;


int partition(int *a, int left, int right)
{
    int p=left;
    //int p=left;
    int *T=new int[right-left+1];
    int pivot=a[p];
    a[p]=a[left];
    a[left]=pivot;
    int i=0;
    int j=right-left;
    for (int n=left+1;n<right+1;n++)
    {
        if(a[n]<=pivot) T[i++]=a[n];
        else T[j--]=a[n];
    }
    T[i]=pivot;
    for (int k=0; k<right-left+1; k++)
    {
        a[k+left]=T[k];
    }
    delete []T;
    return left+i;
}

void quickA(int *a, int left, int right) {
    int pivotat;
    if(left >= right) return;
    pivotat = partition(a, left, right);
    quickA(a, left, pivotat-1);
    quickA(a, pivotat+1, right);
}

void quickP(int *a, int left, int right)
{
    if(left>=right) return;
    int i = left;
    int j = right;
    int pivot = a[i];
    
    while(i<j)
    {
        while(i<j && a[j]>pivot) j--;
        if(i<j)
        {
            a[i] = a[j];
            i++;
        }
        while(i<j && a[i]<pivot) i++;
        if(i<j)
        {
            a[j] = a[i];
            j--;
        }
    }
    a[i] = pivot;
    quickP(a, left, i-1);
    quickP(a, i+1, right);
}

void merging (int *a, int left, int mid, int right)
{
    int i=0;
    int j=0;
    int k=0;
    int sizeL=mid-left+1;
    int sizeR=right-mid;
    int *T=new int[right-left+1];
    while(i < sizeL && j < sizeR)
    {
        if(a[i+left]<=a[j+mid+1])
        {
            T[k++]=a[i+left];
            i++;
        }
        else
        {
            T[k++]=a[j+mid+1];
            j++;
        }
    }
    if(i == sizeL)
    {
        for (int t=j;t<sizeR;t++)
        {
            T[t+sizeL]=a[t+mid+1];
        }
    }
    else
    {
        for (int t=i;t<sizeL;t++)
        {
            T[t+sizeR]=a[t+left];
        }
    }
    for (int p=0; p<right-left+1; p++)
    {
        a[p+left]=T[p];
    }
    delete []T;
}

void merge(int *a, int left, int right)
{
    if (left >= right) return;
    int mid = (left+right)/2;
    merge(a, left, mid);
    merge(a, mid+1, right);
    merging(a, left, mid, right);
}

void selection (int *a, const int N)
{
    for (int i=0; i<N-1; i++)
    {
        int min=a[i];
        int k=i;
        for (int j=i+1;j<N;j++)
        {
            if (a[j]<min)
            {
                min=a[j];
                k=j;
            }
        }
        int t=a[i];
        a[i]=a[k];
        a[k]=t;
    }
}


void insertion(int *a, const int N)
{
    for (int i=1; i<N; i++)
    {
        int j=i-1;
        int t=a[i];
        while (j>=0 && t<=a[j])
        {
            a[j+1]=a[j];
            j--;
        }
        a[j+1]=t;
    }
}

void bubble(int *a, const int N)
{
    for (int i=0; i<N; i++)
    {
        for (int j=i+1; j<N; j++)
        {
            if (a[i]>=a[j])
            {
                int t=a[i];
                a[i]=a[j];
                a[j]=t;
            }
        }
    }
}



int main() {
    int sortA;
    cin >> sortA;
    int N;
    cin >> N;
    
    int *a=new int[N];
    for (int i=0; i<N; i++)
    {
        cin >> a[i];
    }
    
    switch (sortA)
    {
        case 0:
        bubble(a, N);
        break;
        case 1:
        insertion(a, N);
        break;
        case 2:
        selection(a, N);
        break;
        case 3:
        merge(a,0,N-1);
        break;
        case 4:
        quickA(a,0,N-1);
        break;
        case 5:
        quickP(a,0,N-1);
        break;
        default:
        break;
    }
    for (int i=0; i<N; i++)
    {
        cout << a[i]<<'\n';
    }
    delete []a;
}





