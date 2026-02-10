// https://acm.timus.ru/problem.aspx?space=1&num=1839
// computational geometry + sorting + coordinate compression + Fenwick tree (BIT) + FP EPS

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr(v,n) (v).reserve(n)
#define pb push_back

const double EPS = 1e-9;

struct point { ll x, y; };

struct arc { point p, q, r; };

struct ev { double alpha, beta; int id; }; typedef vector<ev> vev;

static inline ll cross(point a, point b, point c)
{
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static inline ll dist_sq(point a, point b)
{
    ll dx = a.x - b.x;
    ll dy = a.y - b.y;
    return dx * dx + dy * dy;
}

struct fenwick
{
    int n;
    vector<int> bit;

    void init(int n_)
    {
        n = n_;
        bit.assign(n + 1, 0);
    }

    void add(int idx, int val)
    {
        for (; idx <= n; idx += idx & -idx) bit[idx] += val;
    }

    int sum(int idx)
    {
        int s = 0;
        for (; idx > 0; idx -= idx & -idx) s += bit[idx];
        return s;
    }
};

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<arc> arcs(n);
    rep(i, 0, n)
    {
        cin >> arcs[i].p.x >> arcs[i].p.y
            >> arcs[i].q.x >> arcs[i].q.y
            >> arcs[i].r.x >> arcs[i].r.y;
    }

    vector<point> pts(m);
    rep(i, 0, m) cin >> pts[i].x >> pts[i].y;

    ll ans = 0;
    fenwick fw;

    rep(i, 0, n)
    {
        point p = arcs[i].p;
        point q = arcs[i].q;
        point r = arcs[i].r;

        ll cp_q = cross(p, r, q);
        ll pr_len_sq = dist_sq(p, r);

        ll rx = r.x - p.x;
        ll ry = r.y - p.y;

        vev evs;
        rsr(evs, m);

        rep(j, 0, m)
        {
            point a = pts[j];

            ll cp_a = cross(p, r, a);
            if (cp_a == 0) continue;
            if ((cp_q > 0 && cp_a > 0) || (cp_q < 0 && cp_a < 0)) continue;

            ll pa_x = a.x - p.x;
            ll pa_y = a.y - p.y;
            ll dot_p = pa_x * rx + pa_y * ry;
            if (dot_p <= 0) continue;

            ll ra_x = a.x - r.x;
            ll ra_y = a.y - r.y;
            ll dot_r = ra_x * (-rx) + ra_y * (-ry);
            if (dot_r <= 0) continue;

            if (llabs(cp_a) >= 2 * pr_len_sq) continue;

            double h = (double)llabs(cp_a);
            double alpha = atan2(h, (double)dot_p);
            double beta = atan2(h, -(double)dot_r);

            evs.pb({alpha, beta, 0});
        }

        int k = sz(evs);
        if (k < 2) continue;

        vector<double> betas;
        rsr(betas, k);
        for (auto &e : evs) betas.pb(e.beta);
        sort(all(betas));

        vector<double> uniq;
        rsr(uniq, sz(betas));
        for (double v : betas)
        {
            if (uniq.empty() || fabs(v - uniq.back()) > EPS) uniq.pb(v);
        }

        fw.init(sz(uniq));

        for (auto &e : evs)
        {
            int id = (int)(lower_bound(all(uniq), e.beta - EPS) - uniq.begin()) + 1;
            e.id = id;
        }

        sort(all(evs), [](const ev &a, const ev &b)
        {
            if (fabs(a.alpha - b.alpha) > EPS) return a.alpha < b.alpha;
            return a.beta < b.beta;
        });

        int start = 0;
        while (start < k)
        {
            int end = start;
            while (end < k && fabs(evs[end].alpha - evs[start].alpha) < EPS) ++end;

            for (int idx = start; idx < end; ++idx)
            {
                ans += fw.sum(evs[idx].id - 1);
            }

            for (int idx = start; idx < end; ++idx)
            {
                fw.add(evs[idx].id, 1);
            }

            start = end;
        }
    }

    cout << ans << '\n';
    return 0;
}
