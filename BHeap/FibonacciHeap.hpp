#ifndef FIBONACCIHEAP_HPP_INCLUDED
#define FIBONACCIHEAP_HPP_INCLUDED
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <ctime>
#include "Mylib.hpp"
using namespace std;
extern int fhopcnt;
struct FibNode {
    /*
     * Fib Node
     * id : id of this vertex
     * dis : distance from start point to this vertex
     * lsb : left sibling
     * rsb : right sibling
     * fa : father node
     * ch : one of the child node
     * deg : degree, number of child
     * operator : < by dis
     */
    int id, dis, lsb, rsb, fa, ch, deg;
    bool operator < (const FibNode &rhs) const {
        return dis < rhs.dis;
    }
};
struct FibHeap {
    /*
     * pos[] : map from vertex id to position in vector a
     * vector a : store each FibNode
     * Min : the index of the min key in a
     * cnt : help to count
     */
    int pos[MAXN];
    vector <FibNode> a;
    int FibHeapSize, Min, cnt;
    void clear() { // initialize heap
        a.clear();
        FibHeapSize = Min = cnt = 0;
        FibNode x;
        a.push_back(x); // push_back 0 element to make the whole program start from 1
    }

    void link(int y, int x) { // make y as child of x
        fhopcnt++;
        a[a[y].lsb].rsb = a[y].rsb; // cut from original link list
        a[a[y].rsb].lsb = a[y].lsb;
        if (a[x].ch) { // update child
            int child = a[x].ch;
            a[a[child].lsb].rsb = y;
            a[y].lsb = a[child].lsb;
            a[child].lsb = y;
            a[y].rsb = child;
        }
        else { // y is only child
            a[x].ch = a[y].rsb = a[y].lsb = y;
        }
        a[y].fa = x; // make father
        a[x].deg++; // increase degree
    }

    void consolidate() { // merge root node with same degree
        vector <int> rtlist; // store root list node
        rtlist.push_back(0);
        int w = Min, nxt = w, rtSize = 0, A[D], x, y, d;
        memset(A, 0, sizeof(A));
        do {
            fhopcnt++;
            rtlist.push_back(nxt);
            ++rtSize;
            nxt = a[nxt].lsb;
        } while (nxt != w);

        for (int i = 1; i <= rtSize; i++) // visit each node
        {
            fhopcnt++;
            w = rtlist[i]; // w : root list node i
            x = w;         // x : copy of w
            d = a[x].deg;  // degree
            while (A[d])   // like carry bit, this position is 1
            {
                y = A[d];
                if (a[y].dis < a[x].dis) // make y->dis smaller than x->dis
                    swap(x, y);
                link(y, x); // merge
                A[d] = 0; // clear 0
                d++; // increase degree, merge higher bit
            }
            A[d] = x; // no more higher bit to merge, end
        }
        Min = 0; // find real Min
        for (int i = 0; i < D; i++) {
            fhopcnt++;
            if (A[i]) { // have tree
                if (!Min) { // update Min
                    Min = a[A[i]].lsb = a[A[i]].rsb = A[i];
                }
                else { // link to root list
                    a[a[Min].lsb].rsb = A[i];
                    a[A[i]].lsb = a[Min].lsb;
                    a[Min].lsb = A[i];
                    a[A[i]].rsb = Min;
                    if (a[A[i]].dis < a[Min].dis) // update Min
                        Min = A[i];
                }
            }
        }
    }

    /*
     * insert an element into heap
     * Time Complexity : O(1)
     */
    void push(FibNode u) { // we will put it to root list
        fhopcnt++;
        u.fa = u.ch = u.deg = 0; // new element into root list set 0
        a.push_back(u);          // store element
        ++cnt;
        pos[u.id] = cnt;         // store position
        if (!Min) {              // the heap has no Min Node -> empty
            Min = a[cnt].lsb = a[cnt].rsb = cnt; // insert
        }
        else {
            a[a[Min].lsb].rsb = cnt; // link it to the root list
            a[cnt].lsb = a[Min].lsb; // Min must be in the root list
            a[Min].lsb = cnt;        // list are double-linked
            a[cnt].rsb = Min;
            if (a[cnt].dis < a[Min].dis) // update Min node
                Min = cnt;
        }
        ++FibHeapSize;                   // update heap size
    }

    /*
     * delete Min
     * Time Complexity : O(1og n)
     */
    void pop() {
        fhopcnt++;
        int z = Min;       // z : Min
        int x = a[Min].ch; // x : one of the children
        if (x) {           // have child
            int childlist[D]; // visit each child, store it
            for (int i = 1; i <= a[z].deg; i++) {
                childlist[i] = x;
                x = a[x].lsb;
            }
            for (int i = 1; i <= a[z].deg; i++) { // link each child to the root list
                fhopcnt++;
                x = childlist[i];
                a[a[Min].lsb].rsb = x;
                a[x].lsb = a[Min].lsb;
                a[Min].lsb = x;
                a[x].rsb = Min;
                a[x].fa = 0;
            }
        }
        a[a[z].lsb].rsb = a[z].rsb; // remove Min
        a[a[z].rsb].lsb = a[z].lsb;
        if (z == a[z].rsb) { // make heap empty
            Min = 0;
        }
        else { //not empty
            Min = a[z].rsb; // temporary Min, still in root list
            consolidate(); // consolidate the root list and calculate real Min
        }
        --FibHeapSize; // update size
    }

    /*
     * x is child
     * y is father
     * to cut x from y
     */
    void cut(int x, int y) {
        fhopcnt++;
        if (a[x].rsb == x) // x has no sibling
            a[y].ch = 0;   // y has no child after cutting
        else { // delete x
            a[a[x].rsb].lsb = a[x].lsb; // link its left and right sibling
            a[a[x].lsb].rsb = a[x].rsb;
            if (a[y].ch == x) // update child if deleted
                a[y].ch = a[x].rsb;
        }
        a[y].deg--; //decrease deg
        // link x into root list
        a[a[Min].rsb].lsb = x;
        a[x].rsb = a[Min].rsb;
        a[Min].rsb = x;
        a[x].lsb = Min;
        a[x].fa = 0; // clear father
    }

    /*
     * decrease position x with key k
     * Time Complexity : O(1)
     */
    void decrease_key(int x, int k) {
        fhopcnt++;
        a[x].dis = k; // directly update
        int y = a[x].fa;
        if (y && a[x].dis < a[y].dis) // destroy heap structure
            cut(x, y); // cut x from y
        if (a[x].dis < a[Min].dis) // update Min
            Min = x;
    }

    FibNode front() { // return with Min Fib heap Node
        return a[Min];
    }

    bool empty() { // to check whether heap is empty
        return FibHeapSize ? 0 : 1;
    }
}q3;

/*
 * dijkstra with Fibonacci heap
 * Time Complexity O(V log V + E)
 */
void Fibdijk(int s, int t) { // dijkstra with Fibonacci heap, most part is the same as binary heap
    memset(dis, INF, sizeof(dis));    // initialize
    memset(vis, false, sizeof(vis));
    q3.clear();
    dis[s] = 0;                       // start point
    FibNode u, v;
    u.id = s, u.dis = 0;
    q3.push(u);                       // push start point into heap
    while (!q3.empty()) {
        u = q3.front();               // min element
        q3.pop();
        if (vis[u.id])                // already out of heap, no need to calculate again
            continue;
        if (t == u.id)                // calculated, return
            return ;
        vis[u.id] = true;             // tag of out of heap
        for (int i = head[u.id]; i; i = e[i].nxt) { // visit each edge
            v.id = e[i].v;
            if (!vis[v.id] && dis[v.id] > dis[u.id] + e[i].w) { // update
                v.dis = dis[u.id] + e[i].w;
                if (dis[v.id] == INF) // not in the heap
                    q3.push(v); // directly push
                else
                    q3.decrease_key(q3.pos[v.id], v.dis); // use decrease key to adjust its position
                dis[v.id] = v.dis; // update distance
            }
        }
    }
}

int FibonacciHeapSolve(string input_file) {
    srand(time(0));                               // random seed
    preconduct(input_file);                       // read and other works
    int u, v;
    clock_t start, finish;                        // time tools
    start = clock();                              // start
    for (int k = 1; k <= 10; k++) {                // random query
        u = rand() % n + 1;                       // from point u
        v = rand() % n + 1;                       // to point v
        Fibdijk(u, v);                               // dijkstra with binary heap
        cout << "Test" << k << ": Distance between " << u << " and " << v << " is " << dis[v] << '\n';
    }
    finish = clock();                             // test finish
    cout << (double)(finish - start) / CLOCKS_PER_SEC << '\n';
    fclose(stdin);
    return 0;
}



#endif // FIBONACCIHEAP_HPP_INCLUDED
