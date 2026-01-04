// https://coderun.yandex.ru/problem/advertising
// coordinate compression/event points + bitmasks

#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define pb push_back

static inline int pc(ull x) { return __builtin_popcountll(x); }

struct p { int u, v; };

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    if (!(cin >> n)) return 0;

    vector<p> a;
    a.reserve(n);

    rep(i, 0, n)
    {
        int u, v;
        cin >> u >> v;
        if (v - u >= 5) a.pb({u, v});
    }

    int m = sz(a);
    if (m == 0) { cout << 0 << ' ' << 1 << ' ' << 6 << '\n'; return 0; }

    vector<int> p;
    p.reserve(m * 2 + 1);

    rep(i, 0, m) { p.pb(a[i].u); p.pb(a[i].u + 5); }
    p.pb(2000000000);

    sort(all(p));
    p.erase(unique(all(p)), p.end());

    int u_cnt = sz(p);
    int len = (m + 63) / 64;

    vector<vector<ull>> mask(u_cnt, vector<ull>(len, 0));

    rep(i, 0, u_cnt)
    {
        int t = p[i];
        int te = t + 5;

        rep(j, 0, m) if (t >= a[j].u && te <= a[j].v) mask[i][j / 64] |= (1ULL << (j % 64));

    }

    int best = -1;
    int ans1 = 1;
    int ans2 = 6;

    bool done = false;
    rep(i, 0, u_cnt)
    {
        rep(j, i, u_cnt)
        {
            if (p[j] < p[i] + 5) continue;

            int cur = 0;
            rep(k, 0, len) cur += pc(mask[i][k] | mask[j][k]);

            if (cur > best)
            {
                best = cur;
                ans1 = p[i];
                ans2 = p[j];
                if (best == m) { done = true; break; }
            }
        }

        if (done) break;
    }

    cout << best << ' ' << ans1 << ' ' << ans2 << '\n';
    return 0;
}
