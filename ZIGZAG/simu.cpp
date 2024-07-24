#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <vector>
#include <cstring>
#include <ctime>
#include <cmath>
using namespace std;
const int MAXN = 10005;
struct node
{
    int id;
    int branch;
    int bandwidth;
    int parent;
    bool deleted;
    int depth;
    int childnum;
    double eps;
    vector <int> children;
}a[MAXN];
int root, N, H;
int G[MAXN][MAXN];
void init()
{
    for (int i = 1; i < MAXN; i++) // generate graph
    {
        for (int j = 1; j < i; j++)
        {
            G[i][j] = rand() % 1024 + 31;
            G[j][i] = G[i][j];
        }
    }
    a[1].id = 1;
    a[1].branch = 10;
    a[1].bandwidth = 1000;
    a[1].parent = 0;
    a[1].depth = 1;
    root = 1;
    N = 1;
}
void insert(int u, int v)
{
    if (a[u].childnum < a[u].branch) // insert
    {
        a[v].parent = u;
        a[v].depth = a[u].depth + 1;
        a[u].children.push_back(v);
        a[u].childnum++;
        if (a[v].depth > H)
            H = a[v].depth;
    }
    else // move to children
    {
        int w = 0;
        for (auto it = a[u].children.cbegin(); it != a[u].children.cend(); it++)
        {
            int i = *it;
            if (w == 0 || (G[i][v] < G[w][v]))
                w = i;
        }
        insert(w, v);
    }
}
void departure(int x)
{
    ;
}
void maintain(int u)
{
    int i;
    double w = 0;
    for (auto it = a[u].children.cbegin(); it != a[u].children.cend(); it++)
    {
        i = *it;
        w += a[i].bandwidth;
    }
    for (auto it = a[u].children.cbegin(); it != a[u].children.cend(); it++)
    {
        i = *it;
        a[i].eps = sqrt(w * a[i].bandwidth / a[u].childnum / a[u].childnum / a[u].bandwidth / a[u].bandwidth) / 2.718;
        if (a[i].eps < 1)
            a[i].eps = 1;
    }
    for (auto it = a[u].children.cbegin(); it != a[u].children.cend(); it++)
    {
        i = *it;
        maintain(i);
    }
}
double calcdelay(int u)
{
    if (u == 1)
        return 0;
    else
        return G[u][a[u].parent] + calcdelay(a[u].parent);
}
int calcbw(int u)
{
    if (u == 1)
        return 1000;
    else
        return min(a[u].bandwidth, calcbw(a[u].parent));
}
void printlog()
{
    maintain(1);
    double maxdelay = 0;
    int totbw = 0, leavebw = 0;
    for (int i = 1; i <= N; i++)
    {
        maxdelay = max(calcdelay(i) + G[1][i], maxdelay);
        totbw += a[i].bandwidth;
        if (a[i].childnum == 0)
            leavebw += a[i].bandwidth;
    }
    printf("N = %d\n", N);
    printf("Max Delay = %.2f\n", maxdelay);
    printf("Total Bandwidth = %d\n", totbw);
    printf("Leave Bandwidth = %d\n", leavebw);
    printf("Bandwidth Utilization = %.2f\n", 1.0 * (totbw - leavebw) / totbw);
    printf("H = %d\n", H);
    printf("\n");
}
int main()
{
    srand(time(0));
    init();
    for (int i = 2; i <= 10000; i++)
    {
        N++;
        a[i].id = i;
        a[i].branch = rand() % 10 + 5;
        a[i].bandwidth = rand() % 1024 + 31;
        insert(root, i);
        if (i % 1000 == 0)
            printlog();
    }
    int op = 0;
    for (int i = 1; i <= 1000; i++)
    {
        int x = rand() % 9999 + 2;
        op += H - a[x].depth;
        if (i % 10 == 0)
        {
            op += rand() % 6 + 4;
        }
        if (i % 100 == 0)
        {
            printf("Operation count = %d\n", op);
        }
    }
    return 0;
}