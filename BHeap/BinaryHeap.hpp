#ifndef BINARYHEAP_HPP_INCLUDED
#define BINARYHEAP_HPP_INCLUDED
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include "Mylib.hpp"
using namespace std;
extern int bhopcnt;
struct Node {
    /*
     * Use a structure Node to store id and distance of a node
     * when pushed into heap
     * Define operator < of the structure with distance
     */
    int id, dis, delay, bw;
    bool operator < (const Node &rhs) const {
        return dis < rhs.dis;
    }
};
struct Heap {
    /*
     * array to store information of pushed Nodes
     * HeapSize means the number of elements in the heap
     */
    Node a[MAXN];
    int HeapSize;

    /*
     * initialize the heap
     */
    void clear() {
        memset(a, 0, sizeof(a));
        HeapSize = 0;
    }
    /*
     * Push a Node into the Heap
     * Time Complexity is O(log n)
     */
    void push(Node u) {
        int i;
        for (i = ++HeapSize; u < a[i >> 1]; i >>= 1) //first place the element at the last position
            a[i] = a[i >> 1], bhopcnt+=2; //then adjust with upper nodes
        a[i] = u;
        //bhopcnt++;
    }

    /*
     * pop the minimum node
     * then replace the position 1 with the last element, then adjust
     * Worst Time complexity is O(log n)
     */
    void pop() {
        int i, ch;
        Node u = a[HeapSize--]; //imagine we put the last element at the top now
        bhopcnt++;
        for (i = 1; (i << 1) <= HeapSize; i = ch) {
            bhopcnt++;
            ch = i << 1;
            if (ch != HeapSize && a[ch | 1] < a[ch]) //compare with two children
                ++ch;
            if (a[ch] < u) //push down
                a[i] = a[ch];
            else //end
                break;
        }
        a[i] = u;
    }

    /*
     * return the minimum value
     */
    Node front() {
        bhopcnt++;
        return a[1];
    }

    /*
     * To judge whether the heap is empty
     */
    bool empty() {
        bhopcnt++;
        return HeapSize ? 0 : 1;
    }

    void calcbw() {
        int totbw = 0, leavebw = 0;
        for (int i = 1; i <= HeapSize; i++) {
            totbw += a[i].bw;
            if ((i << 1) > HeapSize)
                leavebw += a[i].bw;
        }
        printf("Total Bandwidth = %d\n", totbw);
        printf("Leave Bandwidth = %d\n", leavebw);
        printf("Utilization = %.2f\n", 1.0 * (totbw - leavebw) / totbw);
    }
}q1;

/*
 * Dijkstra optimized by binary heap
 * Time Complexity is E log E
 */
void Bindijk(int s, int t) {
    memset(dis, INF, sizeof(dis));   // initialize
    memset(vis, false, sizeof(vis));
    q1.clear();
    dis[s] = 0;                      // start point
    Node u, v;                       // temp Node
    u.id = s, u.dis = 0;
    q1.push(u);                      // push start point into heap
    while (!q1.empty()) {
        u = q1.front();              // min element
        q1.pop();                    // pop
        if (vis[u.id])               // already out of heap, no need to calculate again
            continue;
        if (t == u.id)               // calculated, return
            return ;
        vis[u.id] = true;            // tag of out of heap
        for (int i = head[u.id]; i; i = e[i].nxt) { // visit each edge
            v.id = e[i].v;                          // to-point
            if (!vis[v.id] && dis[v.id] > dis[u.id] + e[i].w) { // update
                dis[v.id] = dis[u.id] + e[i].w;
                v.dis = dis[v.id];
                q1.push(v);
            }
        }
    }
}

int BinaryHeapSolve(string input_file) {
    srand(time(0));                               // random seed
    preconduct(input_file);                       // read and other works
    int u, v;
    clock_t start, finish;                        // time tools
    start = clock();                              // start
    for (int k = 1; k <= 10; k++) {                // random query
        u = rand() % n + 1;                       // from point u
        v = rand() % n + 1;                       // to point v
        Bindijk(u, v);                               // dijkstra with binary heap
        cout << "Test" << k << ": Distance between " << u << " and " << v << " is " << dis[v] << '\n';
    }
    finish = clock();                             // test finish
    cout << (double)(finish - start) / CLOCKS_PER_SEC << '\n';
    fclose(stdin);
    return 0;
}



#endif // BINARYHEAP_HPP_INCLUDED
