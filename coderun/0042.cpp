// https://coderun.yandex.ru/problem/mugs
// topological sort

#include <bits/stdc++.h>
using namespace std;

#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define pb push_back

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    if (!(cin >> n)) return 0;

    vector<vector<int>> g(n + 1);
    vector<int> deg(n + 1, 0);

    rep(i, 1, n + 1)
    {
        int k;
        cin >> k;
        g[i].reserve(k);
        rep(j, 0, k)
        {
            int v;
            cin >> v;
            g[i].pb(v);
            deg[v]++;
        }
    }

    priority_queue<int> q;
    rep(i, 1, n + 1) if (deg[i] == 0) q.push(i);

    vector<int> ans;
    ans.reserve(n);

    while (!q.empty())
    {
        int u = q.top();
        q.pop();
        ans.pb(u);
        for (int v : g[u]) if (--deg[v] == 0) q.push(v);
    }

    for (int i = n - 1; i >= 0; --i) cout << ans[i] << (i ? ' ' : '\n');
    return 0;
}
