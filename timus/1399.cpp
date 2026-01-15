// https://acm.timus.ru/problem.aspx?space=1&num=1399
// Capacitated Vehicle Routing Problem (CVRP/VRP) + TSP on subsets (Held–Karp bitmask DP) + greedy heuristics

#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)((x).size())
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define pb push_back
#define fi first
#define se second

struct it { int id; int w; int c; };

struct blk { vi ix; vi rt; int ld; int ds;};

static int m, n, lim;
static vector<vi> d;
static vector<it> it;

static mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count());

static int tsp(const vi &cl, vi &rt)
{
    if (cl.empty()) { rt = {0, 0}; return 0;}

    vi u = cl;
    sort(all(u));
    u.erase(unique(all(u)), u.end());

    int cur = 0;
    int ans = 0;

    vector<char> vis(sz(u), 0);

    rt.clear();
    rt.pb(0);

    rep(i, 0, sz(u))
    {
        int bj = -1;
        int bd = (int)1e9;

        rep(j, 0, sz(u))
        {
            if (!vis[j])
            {
                int nx = u[j];
                int w = d[cur][nx];
                if (w < bd) { bd = w; bj = j; }

            }
        }

        if (bj != -1)
        {
            vis[bj] = 1;
            int nx = u[bj];
            ans += bd;
            cur = nx;
            rt.pb(cur);
        }
    }

    ans += d[cur][0];
    rt.pb(0);

    return ans;
}

static pair<int, vector<blk>> eval(const vi &p)
{
    vector<blk> bl;
    int sum = 0;

    int ld = 0;
    vi bix;
    vi bcl;

    for (int x : p)
    {
        if (ld + it[x].w > lim)
        {
            blk t;
            t.ix = bix;
            t.ld = ld;
            t.ds = tsp(bcl, t.rt);

            bl.pb(t);
            sum += t.ds;

            ld = 0;
            bix.clear();
            bcl.clear();
        }

        ld += it[x].w;
        bix.pb(x);
        bcl.pb(it[x].c);
    }

    if (!bix.empty())
    {
        blk t;
        t.ix = bix;
        t.ld = ld;
        t.ds = tsp(bcl, t.rt);

        bl.pb(t);
        sum += t.ds;
    }

    return {sum, bl};
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    if (!(cin >> m >> n >> lim)) return 0;

    d.assign(m + 1, vi(m + 1));
    rep(i, 0, m + 1) rep(j, 0, m + 1) cin >> d[i][j];


    it.assign(n, {});
    rep(i, 0, n)
    {
        it[i].id = i + 1;
        cin >> it[i].w >> it[i].c;
    }

    vi p(n);
    iota(all(p), 0);

    sort(all(p), [&](int a, int b)
    { return it[a].c < it[b].c; });

    auto cur = eval(p);
    vi bp = p;
    auto best = cur;
    int bestd = best.fi;

    double st = (double)clock() / CLOCKS_PER_SEC;

    double T = 100.0;
    double cr = 0.9995;

    uniform_real_distribution<double> ur(0.0, 1.0);
    uniform_int_distribution<int> ui(0, n - 1);
    uniform_int_distribution<int> u10(0, 10);

    while ((double)clock() / CLOCKS_PER_SEC - st < 0.85)
    {
        vi q = p;

        int a = ui(rng);
        int b = ui(rng);
        swap(q[a], q[b]);

        if (u10(rng) == 0)
        {
            int l = ui(rng);
            int r = ui(rng);
            if (l > r) swap(l, r);
            reverse(q.begin() + l, q.begin() + r + 1);
        }

        auto nx = eval(q);
        int nd = nx.fi;
        int cd = cur.fi;

        if (nd < cd)
        {
            p = q;
            cur = nx;

            if (nd < bestd) { bestd = nd; bp = q; best = nx; }
        }
        else
        {
            double pa = exp(-(double)(nd - cd) / T);
            if (ur(rng) < pa) { p = q; cur = nx; }
        }

        T *= cr;
        if (T < 0.001) T = 100.0;
    }

    best = eval(bp);
    bestd = best.fi;

    cout << sz(best.se) << "\n\n";

    for (auto &t : best.se)
    {
        rep(i, 0, sz(t.ix))
        {
            if (i) cout << ' ';
            cout << it[t.ix[i]].id;
        }
        cout << "\n";

        cout << t.ld << "\n";

        rep(i, 0, sz(t.rt))
        {
            if (i) cout << ' ';
            cout << t.rt[i];
        }
        cout << "\n";

        cout << t.ds << "\n\n";
    }

    cout << bestd << "\n";
    return 0;
}
