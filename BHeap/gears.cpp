#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <set>
#include <map>
#include "BinaryHeap.hpp"
#include "FibonacciHeap.hpp"
#include "Mylib.hpp"
using namespace std;
int bhopcnt;
int fhopcnt;
int cdf[20000];
int cdf2[20];
int maxd;
int totbw, leavebw;
void dfs(int u, int v)
{
    if (u > q1.HeapSize)
        return ;
    else
    {
        cdf[q1.a[u].delay + v]++;
        dfs(u << 1, v + q1.a[u].delay);
        dfs(u << 1 | 1, v + q1.a[u].delay);
    }
}
void query_worst_delay(int u)
{
    if (u > q1.HeapSize)
        return ;
    else
    {
        maxd += q1.a[u].delay;
        if (q1.a[u << 1].delay > q1.a[u << 1 | 1].delay)
            query_worst_delay(u << 1);
        else
            query_worst_delay(u << 1 | 1);
    }
}
int main()
{
    srand(time(0));
    for (int i = 1; i <= 10000; i++)
    {
        int key = rand() % 1000 + 1;
        int delay = rand() % 1000 + 1;
        int bw = rand() % 500 + 1;
        if (i == 1)
            bw = 1000;
        Node x;
        x.id = i;
        x.dis = key;
        x.delay = delay;
        x.bw = bw;
        q1.push(x);
        maxd = 0;
        totbw += bw;
        
        //FibNode y;
        //y.id = i;
        //y.dis = key;
        //q3.push(y);
        
        if (i % 1000 == 0)
        {
            printf("%d %d\n", bhopcnt, fhopcnt);
            q1.calcbw();
        }
    }
    query_worst_delay(1);
    printf("%d\n", maxd);
    //dfs(1, 0);
    /*
    int tmp = bhopcnt;
    for (int i = 1; i <= 10000; i++)
    {
        q1.pop();
        if (i % 1000 == 0)
        {
            printf("%d\n", bhopcnt - tmp);
        }
    }
    
    for (int i = 0; i < 20000; i++)
    {
        cdf2[i / 1000] += cdf[i];
    }
    for (int i = 0; i < 20; i++)
    {
        printf("%d\n", cdf2[i]);
    }*/
    return 0;
}
/*
2624 5228 7660 10202 12778 15200 17672 20288 22800 25366
*/