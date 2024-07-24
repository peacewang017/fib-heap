#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <cmath>
#include <ctime>
using namespace std;
const int k = 5; // parameter for cluster size
const int MAXN = 5000 + 5; // max node number
const int INF = 0x3f3f3f3f;
struct node
{
    /*
    Do not save subordinate information in node
    */
    int nid;
    int delay;
    int bandwidth; // original bandwidth
    int bw; // actual bandwidth
    bool reachable; // true iff there exists a path in the multicast tree from X to a layer-0 peer.
    bool addable; // true iff there exists a pathin the multicast tree from X to a layer-0 peer whose cluster¡¯s size is in [k, 3k - 1]
    int nparent; // the node get content from (is linked by)
    set <int> children; // the node provide content for (link to)
    int degree; // alive children number
    int nhead; // cluster head index
    bool deleted;
    int layer; // highest layer
    int clusterid; // highest cluster

    void init()
    {
        nid = 0;
        delay = 0; // temporarily use random number
        bandwidth = rand() % 1000 + 1; // temporarily use random number
        reachable = false;
        addable = false;
        nparent = 0;
        children.clear();
        degree = 0;
        nhead = 0;
        deleted = false;
        layer = 0;
        clusterid = 0;
    }
}a[MAXN];
struct cluster
{
    int cid;
    int layer;
    int chead; // head node id
    set <int> subordinate; // do not contain head
    int csize; // always setsize + 1 ?
    bool deleted;
    int cparent; // cluster parent
}c[MAXN];
int G[MAXN][MAXN];
int N, NODECOUNT, C, CLUSTERCOUNT, H, op;
void init()
{
    for (int i = 1; i < MAXN; i++) // generate graph
    {
        for (int j = 1; j < i; j++)
        {
            G[i][j] = rand() % 1000 + 1;
            G[j][i] = G[i][j];
        }
    }

    N = 1;
    NODECOUNT = 1;
    a[1].init();
    a[1].nid = 1;
    a[1].delay = 0;
    a[1].bandwidth = 1000;
    a[1].bw = 1000;
    a[1].reachable = true;
    a[1].addable = true;
    a[1].nparent = 0;
    a[1].nhead = 1;
    a[1].layer = 0;
    a[1].clusterid = 1;

    C = 1;
    CLUSTERCOUNT = 1;
    c[1].cid = 1;
    c[1].layer = 0;
    c[1].chead = 1;
    c[1].csize = 1;
    c[1].cparent = 0;

    a[0].bandwidth = 1000;
    a[0].bw = 1000;

    H = 1;

    op += 3;
}
void printlog()
{/*
    for (int i = 1; i <= N; i++)
    {
        printf("id = %d, nparent = %d\n", i, a[i].nparent);
    }
    printf("-------------------------------\n");*/
    /*
    for (int i = 1; i <= C; i++)
    {
        printf("cluster = %d, head = %d, layer = %d\n", i, c[i].chead, c[i].layer);
        for (auto it = c[i].subordinate.cbegin(); it != c[i].subordinate.cend(); it++)
        {
            printf("%d ", *it);
        }
        printf("\n\n");
    }*/

    int avgdelay = 0, avgbw = 0;
    for (int i = 1; i <= N; i++)
    {
        avgdelay += a[i].delay;
        avgbw += a[i].bw;
    }
    printf("N = %d\n", N);
    printf("Avg Delay = %d\n", avgdelay / N);
    printf("Avg Bandwidth = %d\n", avgbw / N);
    printf("Operation count = %d\n", op);
    printf("H = %d\n", H);
    printf("\n");
/*
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            printf("");
        }
    }*/
}
void join(int x)
{
    if (H == 1) // only one layer, direct link to server
    {
        N++;
        NODECOUNT++;

        a[N].init();
        a[N].nid = N;
        a[N].delay = G[1][N];
        a[N].reachable = true;
        a[N].addable = true;
        a[N].nparent = 1;
        a[1].children.insert(N);
        a[1].degree++;
        a[N].nhead = 1;
        a[N].clusterid = a[1].clusterid;
        c[a[1].clusterid].subordinate.insert(N);
        c[a[1].clusterid].csize++;

        op++;
    }
    else
    {
        if (a[x].layer == 0) // forwarded to layer 0, add
        {
            N++;
            NODECOUNT++;

            int curhead = a[x].nhead; // current head
            int curnparent = a[x].nparent; // current nparent
            int curcluster = a[x].clusterid; // current cluster
            a[N].init();
            a[N].nid = N;
            a[N].delay = a[curnparent].delay + G[curnparent][N];
            a[N].nparent = curnparent;
            a[curnparent].children.insert(N);
            a[curnparent].degree++;
            a[N].nhead = curhead;
            a[N].clusterid = curcluster;
            c[curcluster].subordinate.insert(N);
            c[curcluster].csize++;

            op++;
        }
        else // forward to children
        {
            if (a[x].addable)
            {
                int y = 0, d = INF, tmpy, tmpd;
                for (auto it = a[x].children.cbegin(); it != a[x].children.cend(); it++)
                {
                    tmpy = *it;
                    tmpd = a[tmpy].delay + G[tmpy][N + 1];
                    if (a[tmpy].addable && tmpd < d)
                    {
                        y = tmpy;
                        d = tmpd;
                    }
                }
                op++;
                join(y);
            }
            else
            {
                int y = 0, d = INF, tmpy, tmpd;
                for (auto it = a[x].children.cbegin(); it != a[x].children.cend(); it++)
                {
                    tmpy = *it;
                    tmpd = a[tmpy].delay + G[tmpy][N + 1];
                    if (a[tmpy].reachable && tmpd < d)
                    {
                        y = tmpy;
                        d = tmpd;
                    }
                }
                op++;
                join(y);
            }
        }
    }
}
void split(int x) // split cluster x
{
    if (H == 1) // only one layer
    {
        // create new cluster
        C++;
        CLUSTERCOUNT++;
        c[C].cid = C;
        c[C].layer = c[x].layer;
        int i, y, d = INF, tmpy, tmpd;

        // insert nodes into new cluster
        int cnt = 0;
        for (auto it = c[x].subordinate.cbegin(); it != c[x].subordinate.cend(); it++)
        {
            cnt++;
            if (cnt > (c[x].csize / 2))
                break;
            i = *it;
            c[C].subordinate.insert(i);
            c[C].csize++;
            //op++;
        }
        op++;
        // delete from original cluster
        for (auto it = c[C].subordinate.cbegin(); it != c[C].subordinate.cend(); it++)
        {
            i = *it;
            c[x].subordinate.erase(i);
            c[x].csize--;
            //op++;
        }
        // select the min-degree node to be the head
        for (auto it = c[C].subordinate.cbegin(); it != c[C].subordinate.cend(); it++)
        {
            tmpy = *it;
            tmpd = a[tmpy].degree;
            if (tmpd < d)
            {
                y = tmpy;
                d = tmpd;
            }
        }
        c[C].chead = y;
        c[C].subordinate.erase(y);
        for (auto it = c[C].subordinate.cbegin(); it != c[C].subordinate.cend(); it++)
        {
            i = *it;
            a[i].nhead = c[C].chead;
            a[i].clusterid = C;
        }

        // new layer
        H++;
        C++;
        CLUSTERCOUNT++;
        c[C].cid = C;
        c[C].layer = H - 1;
        c[C].chead = c[x].chead; // always 1
        c[C].csize++;
        a[c[C].chead].layer = c[C].layer;
        a[c[C].chead].clusterid = C;

        c[C].subordinate.insert(c[C - 1].chead);
        c[C].csize++;
        a[y].nparent = c[C].chead;
        a[y].nhead = c[C].chead;
        a[y].layer = c[C].chead;
        a[y].clusterid = C;

        // update cluster parent
        c[C].cparent = 0;
        c[x].cparent = C;
        c[C - 1].cparent = C;

        op++;

        // cut edge
        for (auto it = c[x].subordinate.cbegin(); it != c[x].subordinate.cend(); it++)
        {
            i = *it;
            a[i].nparent = y;
            a[y].children.insert(i);
            a[y].degree++;
            a[1].children.erase(i);
            a[1].degree--;
            op++;
        }
    }

    else if (c[x].layer == 0)
    {
        // create new cluster
        C++;
        CLUSTERCOUNT++;
        c[C].cid = C;
        c[C].layer = c[x].layer;
        c[C].cparent = c[x].cparent;
        int i, y, d = INF, tmpy, tmpd;


        // insert nodes into new cluster
        int cnt = 0;
        for (auto it = c[x].subordinate.cbegin(); it != c[x].subordinate.cend(); it++)
        {
            cnt++;
            if (cnt > (c[x].csize / 2))
                break;
            i = *it;
            c[C].subordinate.insert(i);
            c[C].csize++;
            //op++;
        }
        op++;
        // delete from original cluster
        for (auto it = c[C].subordinate.cbegin(); it != c[C].subordinate.cend(); it++)
        {
            i = *it;
            c[x].subordinate.erase(i);
            c[x].csize--;
            //op++;
        }
        // select the min-degree node to be the head
        for (auto it = c[C].subordinate.cbegin(); it != c[C].subordinate.cend(); it++)
        {
            tmpy = *it;
            tmpd = a[tmpy].degree;
            if (tmpd < d)
            {
                y = tmpy;
                d = tmpd;
            }
        }
        c[C].chead = y;
        c[C].subordinate.erase(y);
        for (auto it = c[C].subordinate.cbegin(); it != c[C].subordinate.cend(); it++)
        {
            i = *it;
            a[i].nhead = c[C].chead;
            a[i].clusterid = C;
        }

        // go up to same cluster as c[x].chead (cparent)
        int curcluster = c[C].cparent;
        a[y].nhead = c[curcluster].chead;
        a[y].layer++;
        a[y].clusterid = curcluster;
        c[curcluster].subordinate.insert(y);
        c[curcluster].csize++;
        int tmpnparent = a[y].nparent;
        a[tmpnparent].children.erase(y);
        a[tmpnparent].degree--;
        // parent, same as other subordinate
        auto tmpit = c[curcluster].subordinate.cbegin();
        if (*tmpit == y)
            tmpit++;
        int tmpnode = *tmpit;
        int curnparent = a[tmpnode].nparent;
        a[y].nparent = curnparent;
        a[curnparent].children.insert(y);
        a[curnparent].degree++;

        op += 2;
    }

    else if (c[x].layer == (H - 1)) // highest layer cluster split
    {
        // create new cluster
        C++;
        CLUSTERCOUNT++;
        c[C].cid = C;
        c[C].layer = c[x].layer;
        c[C].cparent = c[x].cparent;
        int i, y, d = INF, tmpy, tmpd;

        // insert nodes into new cluster
        int cnt = 0;
        for (auto it = c[x].subordinate.cbegin(); it != c[x].subordinate.cend(); it++)
        {
            cnt++;
            if (cnt > (c[x].csize / 2))
                break;
            i = *it;
            c[C].subordinate.insert(i);
            c[C].csize++;
            //op++;
        }
        op++;
        // delete from original cluster
        for (auto it = c[C].subordinate.cbegin(); it != c[C].subordinate.cend(); it++)
        {
            i = *it;
            c[x].subordinate.erase(i);
            c[x].csize--;
            //op++;
        }
        // select the min-degree node to be the head
        for (auto it = c[C].subordinate.cbegin(); it != c[C].subordinate.cend(); it++)
        {
            tmpy = *it;
            tmpd = a[tmpy].degree;
            if (tmpd < d)
            {
                y = tmpy;
                d = tmpd;
            }
        }
        c[C].chead = y;
        c[C].subordinate.erase(y);
        for (auto it = c[C].subordinate.cbegin(); it != c[C].subordinate.cend(); it++)
        {
            i = *it;
            a[i].nhead = c[C].chead;
            a[i].clusterid = C;
        }
        op++;

        // delete link
        for (auto it = c[x].subordinate.cbegin(); it != c[x].subordinate.cend(); it++)
        {
            i = *it;
            for (auto jt = a[i].children.cbegin(); jt != a[i].children.cend(); jt++)
            {
                int j = *jt;
                int jhead = a[j].nhead;
                if (c[C].subordinate.count(jhead))
                {
                    a[i].children.erase(j);
                    a[i].degree--;
                    int r = *c[C].subordinate.cbegin();
                    a[j].nparent = r;
                    a[r].degree++;
                    a[r].children.insert(j);
                }
            }
        }

        for (auto it = c[C].subordinate.cbegin(); it != c[C].subordinate.cend(); it++)
        {
            i = *it;
            for (auto jt = a[i].children.cbegin(); jt != a[i].children.cend(); jt++)
            {
                int j = *jt;
                int jhead = a[j].nhead;
                if (c[x].subordinate.count(jhead))
                {
                    a[i].children.erase(j);
                    a[i].degree--;
                    int r = *c[x].subordinate.cbegin();
                    a[j].nparent = r;
                    a[r].degree++;
                    a[r].children.insert(j);
                }
            }
        }
        // go to upper layer
        H++;
        C++;
        CLUSTERCOUNT++;
        c[C].cid = C;
        c[C].layer = H - 1;
        c[C].chead = c[x].chead; // always 1
        c[C].csize++;
        a[c[C].chead].layer = c[C].layer;
        a[c[C].chead].clusterid = C;

        c[C].subordinate.insert(c[C - 1].chead);
        c[C].csize++;
        a[y].nparent = c[C].chead;
        a[y].nhead = c[C].chead;
        a[y].layer = c[C].chead;
        a[y].clusterid = C;

        // update cluster parent
        c[C].cparent = 0;
        c[x].cparent = C;
        c[C - 1].cparent = C;
        op++;
    }

    else // ordinary split
    {
        // create new cluster
        C++;
        CLUSTERCOUNT++;
        c[C].cid = C;
        c[C].layer = c[x].layer;
        c[C].cparent = c[x].cparent;
        int i, y, d = INF, tmpy, tmpd;

        // insert nodes into new cluster
        int cnt = 0;
        for (auto it = c[x].subordinate.cbegin(); it != c[x].subordinate.cend(); it++)
        {
            cnt++;
            if (cnt > (c[x].csize / 2))
                break;
            i = *it;
            c[C].subordinate.insert(i);
            c[C].csize++;
            //op++;
        }
        op++;
        // delete from original cluster
        for (auto it = c[C].subordinate.cbegin(); it != c[C].subordinate.cend(); it++)
        {
            i = *it;
            c[x].subordinate.erase(i);
            c[x].csize--;
            //op++;
        }
        // select the min-degree node to be the head
        for (auto it = c[C].subordinate.cbegin(); it != c[C].subordinate.cend(); it++)
        {
            tmpy = *it;
            tmpd = a[tmpy].degree;
            if (tmpd < d)
            {
                y = tmpy;
                d = tmpd;
            }
        }
        c[C].chead = y;
        c[C].subordinate.erase(y);
        for (auto it = c[C].subordinate.cbegin(); it != c[C].subordinate.cend(); it++)
        {
            i = *it;
            a[i].nhead = c[C].chead;
            a[i].clusterid = C;
        }
        op++;

        // delete link
        for (auto it = c[x].subordinate.cbegin(); it != c[x].subordinate.cend(); it++)
        {
            i = *it;
            for (auto jt = a[i].children.cbegin(); jt != a[i].children.cend(); jt++)
            {
                int j = *jt;
                int jhead = a[j].nhead;
                if (c[C].subordinate.count(jhead))
                {
                    a[i].children.erase(j);
                    a[i].degree--;
                    int r = *c[C].subordinate.cbegin();
                    a[j].nparent = r;
                    a[r].degree++;
                    a[r].children.insert(j);
                }
            }
        }

        for (auto it = c[C].subordinate.cbegin(); it != c[C].subordinate.cend(); it++)
        {
            i = *it;
            for (auto jt = a[i].children.cbegin(); jt != a[i].children.cend(); jt++)
            {
                int j = *jt;
                int jhead = a[j].nhead;
                if (c[x].subordinate.count(jhead))
                {
                    a[i].children.erase(j);
                    a[i].degree--;
                    int r = *c[x].subordinate.cbegin();
                    a[j].nparent = r;
                    a[r].degree++;
                    a[r].children.insert(j);
                }
            }
        }

        // go to upper layer
        ;
    }
}
void scan()
{
    for (int i = 1; i <= CLUSTERCOUNT; i++)
    {
        if (!c[i].deleted && (c[i].csize > (k * 4)))
        {
            split(i);
        }
    }
}
void reset()
{
    for (int i = 1; i <= N; i++)
    {
        a[i].delay = 0;
        a[i].bw = a[i].bandwidth;
    }
}
void maintain(int x)
{
    if (a[x].layer || (x == 1))
    {
        int i;
        a[x].reachable = false;
        a[x].addable = false;
        a[x].delay = a[a[x].nparent].delay + G[a[x].nparent][x];
        a[x].bw = min(a[x].bandwidth, a[a[x].nparent].bw);
        for (auto it = a[x].children.cbegin(); it != a[x].children.cend(); it++)
        {
            i = *it;
            maintain(i);
            if (a[i].reachable)
                a[x].reachable = true;
            if (a[i].addable)
                a[x].addable = true;
        }
    }
    else
    {
        a[x].reachable = true;
        if (c[a[x].clusterid].csize >= k && c[a[x].clusterid].csize < (k * 3))
            a[x].addable = true;
    }
}
void departure(int x)
{
    /*
     * parent of X needs to delete link
     * select the min-degree node Z to forward data for children
     * select layer-0 subordinate X' to replace X, including get content from parent of X
    */
    int parent = a[x].nparent;
    a[x].deleted = true;
    a[parent].children.erase(x);
    a[parent].degree--;
    if (a[x].layer == 0)
    {
        int curcid = a[x].clusterid;
        c[curcid].subordinate.erase(x);
        c[curcid].csize--;
    }
    else
    {
        /*
         * find z
         * special case: x is in the highest layer, may find 1
        */
        int i, mindeg = INF, z, xx;
        int curcid = a[x].clusterid;
        for (auto it = c[curcid].subordinate.cbegin(); it != c[curcid].subordinate.cend(); it++)
        {
            i = *it;
            if (a[i].degree < INF)
            {
                z = i;
                mindeg = a[i].degree;
            }
        }
        // switch parent to z
        for (auto it = a[x].children.cbegin(); it != a[x].children.cend(); it++)
        {
            i = *it;
            a[i].nparent = z;
            a[z].children.insert(i);
            a[z].degree++;
        }
        // find layer-0 subordinate xx
        for (int i = 1; i <= C; i++)
        {
            if ((!c[i].deleted) && (c[i].chead == x) && (c[i].layer == 0))
            {
                xx = *c[i].subordinate.cbegin();
                break;
            }
        }
        for (int i = 1; i <= C; i++)
        {
            if ((!c[i].deleted) && (c[i].chead == x))
            {
                c[i].chead = xx;
                if (c[i].layer == 0)
                {
                    c[i].subordinate.erase(xx);
                    c[i].csize--;
                }
            }
        }
        // copy data to replace x with xx
        a[xx].clusterid = a[x].clusterid;
        c[curcid].subordinate.erase(x);
        c[curcid].subordinate.insert(x);
        a[xx].layer = a[x].layer;
        a[a[xx].nparent].children.erase(xx);
        a[a[xx].nparent].degree--;
        a[xx].nhead = a[x].nhead;
        a[xx].nparent = a[x].nparent;
        a[a[x].nparent].children.insert(xx);
        a[a[x].nparent].degree++;
    }
}
void merge(int u, int v) // merge cluster u and v
{
    int i, y, d = INF, tmpy, tmpd;
    int newparent;

    // find min-degree element to become newparent
    for (auto it = c[c[u].cparent].subordinate.cbegin(); it != c[c[u].cparent].subordinate.cend(); it++)
    {
        tmpy = *it;
        tmpd = a[tmpy].degree;
        if (tmpd < d)
        {
            y = tmpy;
            d = tmpd;
        }
    }
    newparent = y;

    c[v].deleted = true;
    for (auto it = c[v].subordinate.cbegin(); it != c[v].subordinate.cend(); it++)
    {
        i = *it;
        ;
    }
}
int main()
{
    srand(time(0));
    init();

    for (int i = 2; i <= 5000; i++)
    {
        join(1);
        scan();
        reset();
        maintain(1);
        if (i % 500 == 0)
            printlog();
    }
    //printlog();
    return 0;
}
/*
1000 2000 3000 4000 5000
2206 4413 6652 8891 11114
*/