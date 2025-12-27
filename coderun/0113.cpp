// https://coderun.yandex.ru/problem/advertising
// coordinate compression/event points + bitmasks

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>

#ifdef _MSC_VER
#include <intrin.h>
#define POPCOUNT(x) __popcnt64(x)
#else
#define POPCOUNT(x) __builtin_popcountll(x)
#endif

using namespace std;

struct Customer { int u, v; };

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<Customer> customers;
    customers.reserve(n);

    for (int i = 0; i < n; ++i)
    {
        int u, v;
        cin >> u >> v;
        if (v - u >= 5) customers.push_back({u, v});
    }

    int valid_n = customers.size();
    if (valid_n == 0) { cout << "0 1 6" << endl; return 0; }

    vector<int> pts;
    pts.reserve(valid_n * 2 + 1);

    for (const auto& c : customers)
    { pts.push_back(c.u); pts.push_back(c.u + 5); }
    pts.push_back(2000000000);

    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());

    int u_cnt = pts.size();

    int m_len = (valid_n + 63) / 64;

    vector<vector<uint64_t>> masks(u_cnt, vector<uint64_t>(m_len, 0));

    for (int i = 0; i < u_cnt; ++i)
    {
        int t_start = pts[i];
        int t_end = t_start + 5;

        for (int j = 0; j < valid_n; ++j) {
            if (t_start >= customers[j].u && t_end <= customers[j].v)
                masks[i][j / 64] |= (1ULL << (j % 64));
        }
    }

    int max_p = -1;
    int ans1 = 1;
    int ans2 = 6;

    for (int i = 0; i < u_cnt; ++i)
    {
        for (int j = i; j < u_cnt; ++j)
        {
            if (pts[j] < pts[i] + 5) continue;

            int cur = 0;
            for (int k = 0; k < m_len; ++k)
            {
                uint64_t combined = masks[i][k] | masks[j][k];
                cur += POPCOUNT(combined);
            }

            if (cur > max_p)
            {
                max_p = cur;
                ans1 = pts[i];
                ans2 = pts[j];
            }

            if (max_p == valid_n) goto end_search;
        }
    }

end_search:
    cout << max_p << " " << ans1 << " " << ans2 << endl;
    return 0;
}
