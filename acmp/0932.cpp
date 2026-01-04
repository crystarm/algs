// https://acmp.ru/index.asp?main=task&id_task=932
// centroid decomposition + combinatorics

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr(v,n) (v).reserve(n)
#define pb push_back

const int MAXN = 100000 + 5;

int n, k, r;
vi g[MAXN];

int tin[MAXN], tout[MAXN], depth[MAXN], timer_;
vi by_dep[MAXN];

bool rm[MAXN];
int sub[MAXN];

int freq[MAXN];
int cnt[MAXN];

void dfs_pre(int u, int p, int dep)
{
    tin[u] = ++timer_;
    depth[u] = dep;
    by_dep[dep].pb(tin[u]);
    for (int v : g[u])
    {
        if (v == p) continue;
        dfs_pre(v, u, dep + 1);
    }
    tout[u] = timer_;
}

int cnt_sub(int u, int dep)
{
    if (dep < 0 || dep >= MAXN) return 0;
    const auto& v = by_dep[dep];
    int l = (int)(lower_bound(all(v), tin[u]) - v.begin());
    int rr = (int)(upper_bound(all(v), tout[u]) - v.begin());
    return rr - l;
}

void dfs_sub(int u, int p)
{
    sub[u] = 1;
    for (int v : g[u])
    {
        if (v == p || rm[v]) continue;
        dfs_sub(v, u);
        sub[u] += sub[v];
    }
}

int get_centroid(int u, int p, int tot)
{
    for (int v : g[u])
    {
        if (v == p || rm[v]) continue;
        if (sub[v] > tot / 2) return get_centroid(v, u, tot);
    }
    return u;
}

void add_freq(int u, int p, int d, int val)
{
    if (d > r) return;
    freq[d] += val;
    for (int v : g[u])
    {
        if (v == p || rm[v]) continue;
        add_freq(v, u, d + 1, val);
    }
}

void query_cnt(int u, int p, int d)
{
    if (d > r) return;
    int need = r - d;
    if (need >= 0) cnt[u] += freq[need];
    for (int v : g[u])
    {
        if (v == p || rm[v]) continue;
        query_cnt(v, u, d + 1);
    }
}

void solve(int u)
{
    dfs_sub(u, 0);
    int c = get_centroid(u, 0, sub[u]);

    freq[0] = 1;
    for (int v : g[c])
    {
        if (rm[v]) continue;
        add_freq(v, c, 1, 1);
    }

    cnt[c] += freq[r];

    for (int v : g[c])
    {
        if (rm[v]) continue;
        add_freq(v, c, 1, -1);
        query_cnt(v, c, 1);
        add_freq(v, c, 1, 1);
    }

    freq[0] = 0;
    for (int v : g[c])
    {
        if (rm[v]) continue;
        add_freq(v, c, 1, -1);
    }

    rm[c] = true;
    for (int v : g[c]) if (!rm[v]) solve(v);

}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    if (!(cin >> n >> k)) return 0;

    rep(i, 0, n - 1)
    {
        int u, v;
        cin >> u >> v;
        g[u].pb(v);
        g[v].pb(u);
    }

    if (k & 1) { cout << 0 << '\n'; return 0; }

    r = k / 2;

    dfs_pre(1, 0, 0);
    solve(1);

    ll ans = 0;

    rep(u, 1, n + 1)
    {
        vector<ll> a;
        ll sum = 0;

        for (int v : g[u])
        {
            if (depth[v] > depth[u])
            {
                int cur = cnt_sub(v, depth[u] + r);
                if (cur > 0) { a.pb(cur); sum += cur; }
            }
        }

        ll up = (ll)cnt[u] - sum;
        if (up > 0) a.pb(up);
        if (sz(a) < 3) continue;

        ll s1 = 0, s2 = 0, s3 = 0;
        for (ll x : a)
        {
            s1 += x;
            s2 += x * x;
            s3 += x * x * x;
        }

        ans += (s1 * s1 * s1 - 3 * s1 * s2 + 2 * s3) / 6;
    }

    cout << ans << '\n';
    return 0;
}
