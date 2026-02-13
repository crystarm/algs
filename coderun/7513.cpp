// https://coderun.yandex.ru/problem/open-end
// number theory + GCD subarrays + offline queries + Fenwick tree (BIT) + CDQ divide`n`conquer

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<ll> vll;
typedef vector<int> vi;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr(v,n) (v).reserve(n)
#define pb push_back

struct bit
{
    int n;
    vll b1, b2;

    bit(int n_ = 0) { init(n_); }

    void init(int n_)
    {
        n = n_;
        b1.assign(n + 2, 0);
        b2.assign(n + 2, 0);
    }

    void add(vll &b, int idx, ll val) { for (int i = idx; i <= n; i += i & -i) b[i] += val; }

    ll sum(const vll &b, int idx) const
    {
        ll res = 0;
        for (int i = idx; i > 0; i -= i & -i) res += b[i];
        return res;
    }

    void range_add(int l, int r, ll val)
    {
        if (l > r) return;
        add(b1, l, val);
        add(b1, r + 1, -val);
        add(b2, l, val * (l - 1));
        add(b2, r + 1, -val * r);
    }

    ll prefix_sum(int x) const
    {
        if (x <= 0) return 0;
        if (x > n) x = n;
        ll s1 = sum(b1, x);
        ll s2 = sum(b2, x);
        return s1 * x - s2;
    }
};

struct event
{
    ll x;
    int y;
    int type;
    int z1, z2;
    int z;
    int idx;
};

static vector<event> ev;
static vll ans;
static bit fen;

static void cdq(int l, int r)
{
    if (l >= r) return;
    int mid = (l + r) >> 1;
    cdq(l, mid);
    cdq(mid + 1, r);

    vi left, right;
    rsr(left, mid - l + 1);
    rsr(right, r - mid);

    for (int i = l; i <= mid; ++i) if (ev[i].type == 0) left.pb(i);
    for (int i = mid + 1; i <= r; ++i) if (ev[i].type == 1) right.pb(i);

    sort(all(left), [&](int a, int b) { return ev[a].y < ev[b].y; });
    sort(all(right), [&](int a, int b) { return ev[a].y < ev[b].y; });

    int i = 0;
    for (int qi : right)
    {
        while (i < sz(left) && ev[left[i]].y <= ev[qi].y)
        {
            const event &e = ev[left[i]];
            fen.range_add(e.z1, e.z2, 1);
            ++i;
        }
        ans[ev[qi].idx] += fen.prefix_sum(ev[qi].z);
    }

    rep(j, 0, i)
    {
        const event &e = ev[left[j]];
        fen.range_add(e.z1, e.z2, -1);
    }
}

static inline ll f_len(int len)
{
    return 1LL * len * (len - 1) / 2;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    vll a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];

    vll b(n + 1, 0);
    for (int i = 2; i <= n; ++i) b[i] = llabs(a[i] - a[i - 1]);

    vi run_id(n + 1, 0);
    vi run_start(1), run_end(1);
    int rid = 0;

    for (int i = 1; i <= n; )
    {
        int j = i;
        while (j <= n && a[j] == a[i]) ++j;
        ++rid;
        run_start.pb(i);
        run_end.pb(j - 1);
        for (int t = i; t < j; ++t) run_id[t] = rid;
        i = j;
    }

    vll pref_f(rid + 1, 0);
    for (int id = 1; id <= rid; ++id)
    {
        int len = run_end[id] - run_start[id] + 1;
        pref_f[id] = pref_f[id - 1] + f_len(len);
    }

    ans.assign(q, 0);
    vll eq(q, 0);

    ev.clear();
    ev.shrink_to_fit();

    vector<pair<ll, int>> prev, cur;
    prev.clear();

    for (int r1 = 2; r1 <= n; ++r1)
    {
        cur.clear();

        for (const auto &pr : prev)
        {
            ll ng = std::gcd(pr.first, b[r1]);
            if (cur.empty() || cur.back().first != ng) cur.pb({ng, pr.second});
        }

        ll g0 = b[r1];
        int l0 = r1 - 1;
        if (cur.empty() || cur.back().first != g0) cur.pb({g0, l0});

        rep(i, 0, sz(cur))
        {
            ll g = cur[i].first;
            int L1 = cur[i].second;
            int L2 = (i + 1 < sz(cur) ? cur[i + 1].second - 1 : (r1 - 1));
            if (L1 > L2) continue;
            if (g == 0) continue;

            int z1 = n - L2 + 1;
            int z2 = n - L1 + 1;
            if (z1 > z2) swap(z1, z2);

            ev.pb(event{-g, r1, 0, z1, z2, 0, -1});
        }

        prev.swap(cur);
    }

    for (int i = 0; i < q; ++i)
    {
        int l, r;
        ll k;
        cin >> l >> r >> k;

        int idl = run_id[l];
        int idr = run_id[r];

        ll equal_cnt = 0;
        if (idl == idr)
        {
            equal_cnt = f_len(r - l + 1);
        }
        else
        {
            int left_len = run_end[idl] - l + 1;
            int right_len = r - run_start[idr] + 1;
            equal_cnt = f_len(left_len) + f_len(right_len);
            if (idl + 1 <= idr - 1) equal_cnt += pref_f[idr - 1] - pref_f[idl];
        }
        eq[i] = equal_cnt;

        int z = n - l + 1;
        ev.pb(event{-k, r, 1, 0, 0, z, i});
    }

    sort(all(ev), [&](const event &a, const event &b)
    {
        if (a.x != b.x) return a.x < b.x;
        return a.type < b.type;
    });

    fen.init(n);
    if (!ev.empty()) cdq(0, sz(ev) - 1);

    for (int i = 0; i < q; ++i)
    {
        ll total = ans[i] + eq[i];
        cout << total << '\n';
    }

    return 0;
}
