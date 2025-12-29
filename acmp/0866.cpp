// https://acmp.ru/index.asp?main=task&id_task=866
// st numbering + graph biconnectivity + ear decomposition

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

const int MAXN = 505;
vector<int> g[MAXN];
bool in_res[MAXN];
int n, m;
int q[MAXN], p[MAXN];
bool vis[MAXN];

int main() {

    if (scanf("%d %d", &n, &m) != 2) return 0;

    for (int i = 0; i < m; ++i)
    {
        int u, v;
        scanf("%d %d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int s = 1, t = g[1][0];
    vector<int> res;

    int qh = 0, qt = 0;
    q[qt++] = s;
    memset(vis, 0, sizeof(vis));
    memset(p, 0, sizeof(p));
    vis[s] = true;

    bool ok = false;
    while(qh < qt)
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
        while (cur != s)
        {
            res.push_back(cur);
            cur = p[cur];
        }
        res.push_back(s);
        reverse(res.begin(), res.end());
    } else { res.push_back(s); res.push_back(t); }

    memset(in_res, 0, sizeof(in_res));
    for (int x : res) in_res[x] = true;

    while (res.size() < n)
    {
        bool added = false;
        for (int i = 0; i < res.size(); ++i)
        {
            int u = res[i];
            int start_node = -1;
            for (int v : g[u]) if (!in_res[v]) { start_node = v; break; }

            if (start_node == -1) continue;

            qh = 0; qt = 0;
            q[qt++] = start_node;
            memset(vis, 0, sizeof(vis));
            memset(p, 0, sizeof(p));
            vis[start_node] = true;

            int target = -1;
            int curr_v = -1;

            while (qh < qt)
            {
                int cur = q[qh++];
                bool found = false;
                for (int nxt : g[cur])
                {
                    if (nxt == u && cur == start_node) continue;
                    if (in_res[nxt])
                    {
                        target = nxt;
                        curr_v = cur;
                        found = true;
                        break;
                    }
                    if (!vis[nxt])
                    {
                        vis[nxt] = true;
                        p[nxt] = cur;
                        q[qt++] = nxt;
                    }
                }
                if (found) break;
            }

            if (target != -1)
            {
                vector<int> path;
                while (curr_v != 0)
                {
                    path.push_back(curr_v);
                    curr_v = p[curr_v];
                }
                reverse(path.begin(), path.end());

                int target_idx = -1;
                for (int k = 0; k < res.size(); ++k) if (res[k] == target) { target_idx = k; break; }


                if (i < target_idx) res.insert(res.begin() + i + 1, path.begin(), path.end());
                else
                {
                    reverse(path.begin(), path.end());
                    res.insert(res.begin() + target_idx + 1, path.begin(), path.end());
                }

                for (int x : path) in_res[x] = true;
                added = true;
                break;
            }
        }
        if (!added) break;
    }

    for (int i = 0; i < n; ++i) printf("%d%c", res[i], (i == n - 1 ? '\n' : ' '));

    printf("1");
    for (int i = n - 1; i >= 1; --i) printf(" %d", res[i]);
    printf("\n");

    return 0;
}
