// https://acm.timus.ru/problem.aspx?space=1&num=1829
// routing tables + LPM + bitmasks (IPv4 AND mask) + DFS traversal + rule equivalence checking

#include <bits/stdc++.h>
using namespace std;

#define sz(x) (int)((x).size())
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr(v,n) (v).reserve(n)
#define epl emplace
#define pb push_back
#define se second

struct FS
{
    static const int S = 1 << 20;
    int it = 0, n = 0;
    char b[S];

    inline char gc()
    {
        if (it >= n)
        {
            n = (int)fread(b, 1, S, stdin);
            it = 0;
            if (!n) return 0;
        }

        return b[it++];
    }

    inline void skip()
    {
        char c;
        do
        { c = gc(); if (!c) return;
        } while (c <= ' ');
        it--;
    }

    template <class T>
    bool rd(T &x)
    {
        skip();
        char c = gc();
        if (!c) return false;
        bool neg = false;
        if (c == '-')
        { neg = true; c = gc(); }
        T v = 0;
        while (c >= '0' && c <= '9')
        { v = v * 10 + (c - '0'); c = gc(); }
        x = neg ? -v : v;
        return true;
    }

    bool rd_ip(uint32_t &x)
    {
        skip();
        char c = gc();
        if (!c) return false;
        x = 0;
        rep(k, 0, 4)
        {
            int part = 0;
            while (c >= '0' && c <= '9') { part = part * 10 + (c - '0'); c = gc(); }

            x = (x << 8) | (uint32_t)part;

            if (k < 3) { if (c != '.') return false; c = gc(); }
        }
        return true;
    }
};

struct nd
{
    int ch[2];
    int g1 = 0;
    int g2 = 0;
    nd() { ch[0] = ch[1] = -1; }
};

vector<nd> tr;
unordered_map<uint32_t, int> gid;

int get_id(uint32_t ip)
{
    auto it = gid.find(ip);
    if (it != gid.end()) return it->se;
    int id = sz(gid) + 1;
    gid.epl(ip, id);
    return id;
}

void add_route(uint32_t d, uint32_t m, uint32_t g, int which)
{
    int L = __builtin_popcount(m);
    d &= m;
    int v = 0;
    rep(i, 0, L)
    {
        int bit = (d >> (31 - i)) & 1;
        if (tr[v].ch[bit] == -1)
        {
            tr[v].ch[bit] = sz(tr);
            tr.pb(nd());
        }
        v = tr[v].ch[bit];
    }
    int id = get_id(g);
    if (which == 1) tr[v].g1 = id;
    else tr[v].g2 = id;
}

bool dfs(int v, int inh1, int inh2)
{
    if (tr[v].g1) inh1 = tr[v].g1;
    if (tr[v].g2) inh2 = tr[v].g2;

    rep(b, 0, 2)
    {
        int to = tr[v].ch[b];
        if (to != -1) { if (!dfs(to, inh1, inh2)) return false; }
        else { if (inh1 != inh2) return false; }
    } return true;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    FS fs;
    int n1, n2;
    if (!fs.rd(n1)) return 0;

    rsr(tr, 1 + 32 * 65536 + 5);
    tr.pb(nd());
    rsr(gid, 131072);

    rep(i, 0, n1)
    {
        uint32_t d, m, g;
        fs.rd_ip(d);
        fs.rd_ip(m);
        fs.rd_ip(g);
        add_route(d, m, g, 1);
    }

    fs.rd(n2);
    rep(i, 0, n2)
    {
        uint32_t d, m, g;
        fs.rd_ip(d);
        fs.rd_ip(m);
        fs.rd_ip(g);
        add_route(d, m, g, 2);
    }

    cout << (dfs(0, 0, 0) ? "YES\n" : "NO\n");
    return 0;
}
