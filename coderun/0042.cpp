// https://coderun.yandex.ru/problem/mugs
// topological sort

#include <iostream>
#include <vector>
#include <queue>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<std::vector<int>> adj(n + 1);
    std::vector<int> deg(n + 1, 0);

    for (int i = 1; i <= n; ++i)
    {
        int k;
        std::cin >> k;
        adj[i].reserve(k);
        for (int j = 0; j < k; ++j)
        {
            int p;
            std::cin >> p;
            adj[i].push_back(p);
            deg[p]++;
        }
    }

    std::priority_queue<int> pq;

    for (int i = 1; i <= n; ++i) if (deg[i] == 0) pq.push(i);


    std::vector<int> res;
    res.reserve(n);

    while (!pq.empty())
    {
        int u = pq.top();
        pq.pop();
        res.push_back(u);

        for (int v : adj[u]) if (--deg[v] == 0) pq.push(v);
    }
    for (int i = n - 1; i >= 0; --i)
        std::cout << res[i] << (i == 0 ? '\n' : ' ');
    return 0;
}
