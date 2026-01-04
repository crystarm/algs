// https://coderun.yandex.ru/problem/stalker
// BFS or smth

#include <bits/stdc++.h>
using namespace std;

#define rep(i,a,b) for (int i = (a); i < (b); ++i)

const int MAXN = 100000 + 5;
const int MAXM = 2000005;

struct FS
{
    static const int S = 1 << 20;
    int i = 0, n = 0;
    char b[S];

    inline char gc()
    {
        if (i >= n)
        {
            n = (int)fread(b, 1, S, stdin);
            i = 0;
            if (!n) return 0;
        }
        return b[i++];
    }

    int ri()
    {
        int x = 0;
        char c = gc();
        while (c && (c < '0' || c > '9')) c = gc();
        while (c && c >= '0' && c <= '9')
        {
            x = x * 10 + (c - '0');
            c = gc();
        }
        return x;
    }
};

int h_tmp[MAXN], nxt_tmp[MAXM], to_tmp[MAXM], e_tmp;

int h_c[MAXM], nxt_c[MAXM], v_c[MAXM], e_c;

int h_u[MAXN], nxt_u[MAXM], v_u[MAXM], e_u;

int buf[MAXM], bc;
int vis[MAXN], vid;

int q[MAXM];
int dist_[MAXN];
bool used[MAXM];

static inline void add_tmp(int u, int v)
{
    to_tmp[e_tmp] = v;
    nxt_tmp[e_tmp] = h_tmp[u];
    h_tmp[u] = e_tmp++;
}

static inline void add_c(int c, int u)
{
    v_c[e_c] = u;
    nxt_c[e_c] = h_c[c];
    h_c[c] = e_c++;
}

static inline void add_u(int u, int c)
{
    v_u[e_u] = c;
    nxt_u[e_u] = h_u[u];
    h_u[u] = e_u++;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    FS fs;

    memset(h_tmp, -1, sizeof(h_tmp));
    memset(h_c, -1, sizeof(h_c));
    memset(h_u, -1, sizeof(h_u));
    memset(dist_, -1, sizeof(dist_));

    int n = fs.ri(), k = fs.ri();
    int cid = 0;

    while (k--)
    {
        int r = fs.ri();
        e_tmp = 0;
        bc = 0;

        while (r--)
        {
            int u = fs.ri(), v = fs.ri();
            buf[bc++] = u;
            buf[bc++] = v;
            add_tmp(u, v);
            add_tmp(v, u);
        }

        vid++;
        rep(i, 0, bc)
        {
            int s = buf[i];
            if (vis[s] == vid) continue;

            int c = cid++;
            int qh = 0, qt = 0;

            vis[s] = vid;
            q[qt++] = s;
            add_c(c, s);
            add_u(s, c);

            while (qh < qt)
            {
                int u = q[qh++];
                for (int e = h_tmp[u]; e != -1; e = nxt_tmp[e])
                {
                    int v = to_tmp[e];
                    if (vis[v] != vid)
                    {
                        vis[v] = vid;
                        q[qt++] = v;
                        add_c(c, v);
                        add_u(v, c);
                    }
                }
            }
        }

        rep(i, 0, bc) h_tmp[buf[i]] = -1;
    }

    int qh = 0, qt = 0;
    dist_[1] = 0;
    q[qt++] = 1;

    while (qh < qt)
    {
        int u = q[qh++];
        if (u == n) break;

        for (int e = h_u[u]; e != -1; e = nxt_u[e])
        {
            int c = v_u[e];
            if (used[c]) continue;
            used[c] = true;

            for (int j = h_c[c]; j != -1; j = nxt_c[j])
            {
                int v = v_c[j];
                if (dist_[v] == -1)
                {
                    dist_[v] = dist_[u] + 1;
                    q[qt++] = v;
                }
            }
        }
    }

    cout << dist_[n] << '\n';
    return 0;
}
