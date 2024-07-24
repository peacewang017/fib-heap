#ifndef MYLIB_HPP_INCLUDED
#define MYLIB_HPP_INCLUDED
#include <string>
#include <iostream>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <algorithm>
using namespace std;
const int MAXN = 3000000; // max vertex
const int MAXM = 6000000; // max edge
const int MAXL = 100;
const int INF = 0x3f3f3f3f;
const int D = 40; // max degree
int n, m; // vertex and edge number
int head[MAXN], tail; // adjacency list head
int dis[MAXN]; // distance
bool vis[MAXN]; // whether out of heap
struct Edge {
    /*
     * u : from
     * v : to
     * w : dis
     * nxt : next edge
     */
    int u, v, w, nxt;
}e[MAXM];
inline int nextInt() { //fast read in integer char by char
    char ch = getchar();
    while (!isdigit(ch)) // not digit -> read again until a digit come up
        ch = getchar();
    int x = 0;
    while (isdigit(ch)) { // get a whole integer
        x = x * 10 + ch - '0'; // the integer has more digit, calculate it
        ch = getchar(); // until read in a space or other char to mark the end of the integer
    }
    return x;
}
void addEdge(int u, int v, int w) { // use adjacency list to store edge
    e[++tail].nxt = head[u];
    head[u] = tail;
    e[tail].u = u;
    e[tail].v = v;
    e[tail].w = w;
}
int preconduct(string input_file) {
    char fp[20];                                  // convert file name
    for (int i = 0; i < input_file.size(); i++)
        fp[i] = input_file[i];
    fp[input_file.size()] = '\0';
    freopen(fp, "r", stdin);                      // file input
    char c, str[MAXL];
    int tmp = 0, u, v, w;
    while ((c = getchar()) != EOF) {              // input
        if (c == 'c') {                           // c : general info
            scanf("%s", c);
            puts(str);
        }
        else if (c == 'p') {                      // p : vertex and edge number
            n = nextInt();                        // next integer function
            m = nextInt();                        // much faster input than scanf
        }
        else if (c == 'a') {                      // a : edge info
            u = nextInt();                        // from vertex u
            v = nextInt();                        // to vertex v
            w = nextInt();                        // with value w
            addEdge(u, v, w);                     // use adjacency list to store
        }
    }
}
#endif // MYLIB_HPP_INCLUDED
