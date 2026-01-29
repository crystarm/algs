// https://coderun.yandex.ru/problem/two-cliques
// complement graph + partition into two cliques + bipartite check via 2-coloring (BFS/DFS)

#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;
typedef queue<int> qi;

#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i <= (b); ++i)
#define pd(x) push_back(x)
#define ppb() pop_back()
#define f() front()
#define b() back()

const int MAXN = 5005;

static bool adj[MAXN][MAXN];
static int color[MAXN];

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    rep(i, 1, m)
    {
        int u, v;
        cin >> u >> v;
        adj[u][v] = 1;
        adj[v][u] = 1;
    }

    bool ok = true;

    rep(i, 1, n)
    {
        if (color[i]) continue;

        qi q;
        q.push(i);
        color[i] = 1;

        while (!q.empty() && ok)
        {
            int u = q.f();
            q.pop();

            rep(v, 1, n)
            {
                if (u == v) continue;
                if (adj[u][v]) continue;

                if (!color[v])
                {
                    color[v] = 3 - color[u];
                    q.push(v);
                }
                else if (color[v] == color[u])
                {
                    ok = false;
                    break;
                }
            }
        }

        if (!ok) break;
    }

    if (!ok)
    {
        cout << -1 << '\n';
        return 0;
    }

    vi a, b;
    rep(i, 1, n)
    {
        if (color[i] == 1) a.pd(i);
        else b.pd(i);
    }

    if (a.empty())
    {
        a.pd(b.b());
        b.ppb();
    }
    else if (b.empty())
    {
        b.pd(a.b());
        a.ppb();
    }

    cout << sz(a) << '\n';
    rep(i, 0, sz(a) - 1)
    {
        if (i) cout << ' ';
        cout << a[i];
    }
    cout << '\n';

    rep(i, 0, sz(b) - 1)
    {
        if (i) cout << ' ';
        cout << b[i];
    }
    cout << '\n';

    return 0;
}
