// https://acmp.ru/index.asp?main=task&id_task=866
// st numbering + graph biconnectivity + ear decomposition

#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define pb push_back
#define ins insert
#define rev reverse

const int MAXN = 505;

vi g[MAXN];
bool in_res[MAXN];
bool vis[MAXN];
int q[MAXN], p[MAXN];

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    rep(i, 0, m)
    {
        int u, v;
        cin >> u >> v;
        g[u].pb(v);
        g[v].pb(u);
    }

    int s = 1, t = g[1][0];
    vi res;

    int qh = 0, qt = 0;
    q[qt++] = s;
    memset(vis, 0, sizeof(vis));
    memset(p, 0, sizeof(p));
    vis[s] = true;

    bool ok = false;
    while (qh < qt)
    {
        int u = q[qh++];
        if (u == t) { ok = true; break; }
        for (int v : g[u])
        {
            if (u == s && v == t) continue;
            if (!vis[v])
            {
                vis[v] = true;
                p[v] = u;
                q[qt++] = v;
            }
        }
    }

    if (ok)
    {
        int cur = t;
        while (cur != s) { res.pb(cur); cur = p[cur]; }
        res.pb(s);
        rev(all(res));
    }
    else
    { res.pb(s); res.pb(t); }

    memset(in_res, 0, sizeof(in_res));
    for (int x : res) in_res[x] = true;

    while (sz(res) < n)
    {
        bool added = false;

        rep(i, 0, sz(res))
        {
            int u = res[i];

            int start_node = -1;
            for (int v : g[u]) if (!in_res[v]) { start_node = v; break; }
            if (start_node == -1) continue;

            qh = 0;
            qt = 0;
            q[qt++] = start_node;
            memset(vis, 0, sizeof(vis));
            memset(p, 0, sizeof(p));
            vis[start_node] = true;

            int target = -1;
            int cur = -1;

            while (qh < qt)
            {
                int x = q[qh++];
                bool found = false;

                for (int nxt : g[x])
                {
                    if (nxt == u && x == start_node) continue;

                    if (in_res[nxt])
                    {
                        target = nxt;
                        cur = x;
                        found = true;
                        break;
                    }

                    if (!vis[nxt])
                    {
                        vis[nxt] = true;
                        p[nxt] = x;
                        q[qt++] = nxt;
                    }
                }

                if (found) break;
            }

            if (target != -1)
            {
                vi path;
                while (cur != 0)
                {
                    path.pb(cur);
                    cur = p[cur];
                }
                rev(all(path));

                int target_idx = -1;
                rep(j, 0, sz(res)) if (res[j] == target) { target_idx = j; break; }

                if (i < target_idx) res.ins(res.begin() + i + 1, all(path));
                else
                {
                    rev(all(path));
                    res.ins(res.begin() + target_idx + 1, all(path));
                }

                for (int x : path) in_res[x] = true;
                added = true;
                break;
            }
        }

        if (!added) break;
    }

    rep(i, 0, n) cout << res[i] << (i + 1 == n ? '\n' : ' ');

    cout << 1;
    for (int i = n - 1; i >= 1; --i) cout << ' ' << res[i];
    cout << '\n';

    return 0;
}
