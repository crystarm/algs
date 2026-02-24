// https://coderun.yandex.ru/problem/sequence-clustering
// probability theory + Markov chains + clustering + unsupervised learning + MLE


#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;

typedef unsigned char uc;

typedef vector<int> vi;
typedef vector<double> vd;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define pb push_back

struct res { double sc; vi z; vd p0; vd p1; };

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, k;
    if (!(cin >> n >> k)) return 0;
    string ab;
    cin >> ab;

    int m = k - 1;
    int rs = m + 1;
    int csz = m + 1;
    int st = m;
    int fn = m;
    int rc = rs * csz;

    vi idc(256, -1);
    rep(i,0,m) idc[(uc)ab[i]] = i;

    vector<vector<pair<int,int>>> nz(n);
    vector<vi> rw(n, vi(rs, 0));
    vi len(n), fst(n), lst(n);
    vector<ull> hh(n, 0);
    vector<ll> gc(m, 0);

    rep(i,0,n)
    {
        string s;
        cin >> s;
        int l = sz(s);
        len[i] = l;

        vi cnt(rc, 0);
        ull h = 1469598103934665603ull;

        int f = idc[(uc)s[0]];
        fst[i] = f;

        int p = st;
        rep(t,0,l)
        {
            int x = idc[(uc)s[t]];
            h ^= (ull)(x + 1);
            h *= 1099511628211ull;

            cnt[p * csz + x]++;
            rw[i][p]++;
            p = x;
            gc[x]++;
        }

        lst[i] = p;
        cnt[p * csz + fn]++;
        rw[i][p]++;

        h ^= 911382323ull + (ull)l;
        h *= 1099511628211ull;
        hh[i] = h;

        rep(j,0,rc) if (cnt[j]) nz[i].pb({j, cnt[j]});
    }

    auto mk_init = [&](const vi& ord, bool alt)
    {
        vi z(n, 0);
        if (!alt)
        {
            rep(i,0,n)
            {
                if (i >= n / 2) z[ord[i]] = 1;
            }
        }
        else
        {
            rep(i,0,n) z[ord[i]] = (i & 1);
        }

        int c0 = 0;
        rep(i,0,n) c0 += (z[i] == 0);
        if (c0 == 0 || c0 == n)
        {
            rep(i,0,n) z[i] = (i >= n / 2);
        }
        return z;
    };

    auto em = [&](const vi& iz)
    {
        const double a = 0.2;
        const double b = 1.0;
        const double e = 1e-6;
        const int itn = 35;

        vector<array<double,2>> g(n);
        vi z0 = iz;

        int c0 = 0;
        rep(i,0,n) c0 += (z0[i] == 0);
        if (c0 == 0 || c0 == n)
        {
            rep(i,0,n) z0[i] = (i < n / 2 ? 0 : 1);
        }

        rep(i,0,n)
        {
            g[i][0] = (z0[i] == 0 ? 1.0 : 0.0);
            g[i][1] = 1.0 - g[i][0];
        }

        vd w0(rc), w1(rc), s0(rs), s1(rs), p0(rc), p1(rc);
        double q0 = 0.5, q1 = 0.5;
        double prv = -1e300;
        double cur = -1e300;

        rep(it,0,itn)
        {
            fill(all(w0), 0.0);
            fill(all(w1), 0.0);
            fill(all(s0), 0.0);
            fill(all(s1), 0.0);
            double cw0 = 0.0, cw1 = 0.0;

            rep(i,0,n)
            {
                double g0 = g[i][0];
                double g1 = g[i][1];
                cw0 += g0;
                cw1 += g1;

                rep(r,0,rs)
                {
                    s0[r] += g0 * rw[i][r];
                    s1[r] += g1 * rw[i][r];
                }

                for (auto &x : nz[i])
                {
                    w0[x.first] += g0 * x.second;
                    w1[x.first] += g1 * x.second;
                }
            }

            q0 = (cw0 + b) / (n + 2.0 * b);
            q1 = (cw1 + b) / (n + 2.0 * b);

            rep(r,0,rs)
            {
                int alw = (r == st ? m : m + 1);
                double d0 = s0[r] + a * alw;
                double d1 = s1[r] + a * alw;

                rep(c,0,csz)
                {
                    int t = r * csz + c;
                    if (r == st && c == fn)
                    {
                        p0[t] = -1e300;
                        p1[t] = -1e300;
                    }
                    else
                    {
                        p0[t] = log((w0[t] + a) / d0);
                        p1[t] = log((w1[t] + a) / d1);
                    }
                }
            }

            cur = 0.0;
            rep(i,0,n)
            {
                double x0 = log(q0);
                double x1 = log(q1);

                for (auto &x : nz[i])
                {
                    x0 += p0[x.first] * x.second;
                    x1 += p1[x.first] * x.second;
                }

                double mx = max(x0, x1);
                double u0 = exp(x0 - mx);
                double u1 = exp(x1 - mx);
                double s = u0 + u1;

                double gg0 = u0 / s;
                if (gg0 < e) gg0 = e;
                if (gg0 > 1.0 - e) gg0 = 1.0 - e;

                g[i][0] = gg0;
                g[i][1] = 1.0 - gg0;
                cur += mx + log(s);
            }

            if (it >= 2)
            {
                double df = cur - prv;
                if (df >= 0 && df < 1e-7 * (1.0 + fabs(cur))) break;
            }
            prv = cur;
        }

        vi z(n, 0);
        int h0 = 0, h1 = 0;
        rep(i,0,n)
        {
            z[i] = (g[i][1] > g[i][0]);
            if (z[i] == 0) h0++;
            else h1++;
        }

        double sc = cur;
        if (min(h0, h1) < max(1, n / 12)) sc -= 1e100;

        vi ord_r;
        ord_r.pb(st);
        rep(r,0,m) ord_r.pb(r);

        bool ok = false;
        bool sw = false;
        for (int r : ord_r)
        {
            rep(c,0,csz)
            {
                if (r == st && c == fn) continue;
                int t = r * csz + c;
                double x0 = p0[t];
                double x1 = p1[t];
                if (fabs(x0 - x1) > 1e-12)
                {
                    if (x1 > x0) sw = true;
                    ok = true;
                    break;
                }
            }
            if (ok) break;
        }

        if (sw)
        {
            rep(i,0,n) z[i] ^= 1;
            swap(p0, p1);
        }

        return res{sc, z, p0, p1};
    };

    vector<res> v;
    v.reserve(40);

    auto add = [&](auto keyf, bool alt = false)
    {
        vi ord(n);
        iota(all(ord), 0);
        stable_sort(all(ord), [&](int i, int j)
        {
            auto x = keyf(i);
            auto y = keyf(j);
            if (x != y) return x < y;
            return i < j;
        });
        v.pb(em(mk_init(ord, alt)));
    };

    add([&](int i){ return len[i]; }, false);
    add([&](int i){ return len[i]; }, true);
    add([&](int i){ return fst[i]; }, false);
    add([&](int i){ return lst[i]; }, false);
    add([&](int i){ return hh[i]; }, false);

    vi tc(m);
    iota(all(tc), 0);
    sort(all(tc), [&](int x, int y)
    {
        if (gc[x] != gc[y]) return gc[x] > gc[y];
        return x < y;
    });

    rep(t,0,min(m, 3))
    {
        int ch = tc[t];
        add([&](int i){ return rw[i][ch]; }, false);
        add([&](int i){ return rw[i][ch]; }, true);
    }

    mt19937_64 rng(
        chrono::steady_clock::now().time_since_epoch().count()
        ^ (ull)n * 1000003ull
        ^ (ull)k * 911382323ull
    );

    int rr = 10;
    rep(tt,0,rr)
    {
        vi ord(n);
        iota(all(ord), 0);
        shuffle(all(ord), rng);
        v.pb(em(mk_init(ord, false)));
    }

    int bi = 0;

    rep(i,1,sz(v)) if (v[i].sc > v[bi].sc) bi = i;
    rep(i,0,n) cout << v[bi].z[i] << '\n';

    return 0;
}
