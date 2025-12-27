// https://coderun.yandex.ru/problem/stalker
// BFS or smth

#include <cstdio>
#include <cstring>

const int N = 100005;
const int M = 2000005;

int ri()
{
    int x = 0, c = getchar();
    while (c < 48 || c > 57) { if (c == EOF) return 0; c = getchar(); }
    while (c >= 48 && c <= 57) x = x * 10 + c - 48, c = getchar();
    return x;
}

int h1[N], n1[M], t1[M], e1;
int h2[M], n2[M], v2[M], e2;
int h3[N], n3[M], v3[M], e3;

int buf[M], bc;
int vis[N], vid;

int q[M], d[N];
bool used[M];

void add1(int u, int v) { t1[e1] = v; n1[e1] = h1[u]; h1[u] = e1++; }

void add2(int c, int u) { v2[e2] = u; n2[e2] = h2[c]; h2[c] = e2++; }

void add3(int u, int c) { v3[e3] = c; n3[e3] = h3[u]; h3[u] = e3++; }

int main()
{
    memset(h1, -1, sizeof(h1));
    memset(h2, -1, sizeof(h2));
    memset(h3, -1, sizeof(h3));
    memset(d, -1, sizeof(d));

    int n = ri(), k = ri();
    int cid = 0;

    while (k--)
    {
        int r = ri();
        e1 = 0; bc = 0;

        while (r--)
        {
            int u = ri(), v = ri();
            buf[bc++] = u; buf[bc++] = v;
            add1(u, v); add1(v, u);
        }

        vid++;
        for (int i = 0; i < bc; ++i)
        {
            int s = buf[i];
            if (vis[s] == vid) continue;

            int c = cid++;
            int qh = 0, qt = 0;

            vis[s] = vid;
            q[qt++] = s;
            add2(c, s); add3(s, c);

            while (qh < qt)
            {
                int u = q[qh++];
                for (int e = h1[u]; ~e; e = n1[e])
                {   int v = t1[e];
                    if (vis[v] != vid)
                    {
                        vis[v] = vid;
                        q[qt++] = v;
                        add2(c, v); add3(v, c);
                    }
                }
            }
        }

        for (int i = 0; i < bc; ++i) h1[buf[i]] = -1;
    }

    int qh = 0, qt = 0;
    d[1] = 0;
    q[qt++] = 1;

    while (qh < qt) {
        int u = q[qh++];
        if (u == n) { printf("%d\n", d[n]); return 0; }

        for (int e = h3[u]; ~e; e = n3[e])
        {
            int c = v3[e];
            if (used[c]) continue;
            used[c] = 1;

            for (int j = h2[c]; ~j; j = n2[j])
            {
                int v = v2[j];
                if (d[v] == -1) { d[v] = d[u] + 1; q[qt++] = v; }
            }
        }
    }

    printf("%d\n", d[n]);
    return 0;
}
