// https://acmp.ru/index.asp?main=task&id_task=567
// planar geometry + computational geometry + projections + trigonometry

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

typedef vector<ld> vld;
typedef vector<pair<ld, ld>> vpld;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)((x).size())
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define pb push_back
#define fi first
#define se second

static const ld PI = acosl(-1.0L);
static const ld TAU = 2.0L * PI;
static const ld EPS = 1e-10L;

static inline ld nrm(ld x)
{
    x = fmodl(x, TAU);
    if (x < 0) x += TAU;
    if (x < 0) x = 0;
    if (x >= TAU) x = 0;
    return x;
}

struct cf { ld a, b, c; };

static inline int ht(ld phi, ld a)
{
    phi = nrm(phi);
    if (phi <= a + 1e-18L) return 1;
    ld c0 = cosl(phi);
    ld c1 = cosl(phi - a);
    if (c0 >= c1)
    {
        if (c0 <= 0) return 0;
        return 2;
    }
    else
    {
        if (c1 <= 0) return 0;
        return 3;
    }
}

static inline cf tc(int t, ld sh, ld l, ld a)
{
    if (t == 0) return {0, 0, 0};
    if (t == 1) return {0, 0, l};
    if (t == 2) return {l * cosl(sh), -l * sinl(sh), 0};
    ld s = sh - a;
    return {l * cosl(s), -l * sinl(s), 0};
}

static inline cf sc(ld mid, const vld &sh, ld l, ld a)
{
    cf r{0, 0, 0};
    for (ld s : sh)
    {
        int t = ht(mid + s, a);
        cf u = tc(t, s, l, a);
        r.a += u.a;
        r.b += u.b;
        r.c += u.c;
    }
    return r;
}

static inline ld ev(ld r, ld d, ld c, ld t) { return r * cosl(t - d) + c; }

static vpld segs(ld r, ld d, ld c, ld lim, ld L, ld R)
{
    vpld s;

    if (r < 1e-14L) { if (c <= lim + EPS) s.pb({L, R}); return s; }

    ld k = (lim - c) / r;

    if (k >= 1.0L) { s.pb({L, R}); return s; }

    if (k <= -1.0L)
    {
        if (k < -1.0L - 1e-12L) return s;
        ld x = d + PI;
        ll n0 = (ll)floorl((L - x) / TAU) - 2;
        ll n1 = (ll)ceill((R - x) / TAU) + 2;
        for (ll n = n0; n <= n1; n++)
        {
            ld t = x + (ld)n * TAU;
            if (t + 1e-12L < L || t - 1e-12L > R) continue;
            t = min(max(t, L), R);
            s.pb({t, t});
        }
        return s;
    }

    ld th = acosl(k);

    vld pts;
    pts.pb(L);
    pts.pb(R);

    for (int sg : {1, -1})
    {
        ld base = d + (ld)sg * th;
        ll n0 = (ll)floorl((L - base) / TAU) - 2;
        ll n1 = (ll)ceill((R - base) / TAU) + 2;
        for (ll n = n0; n <= n1; n++)
        {
            ld t = base + (ld)n * TAU;
            if (t + 1e-12L < L || t - 1e-12L > R) continue;
            t = min(max(t, L), R);
            pts.pb(t);
        }
    }

    sort(all(pts));
    vld u;
    for (ld x : pts) if (u.empty() || fabsl(x - u.back()) > 1e-9L) u.pb(x);


    rep(i, 0, sz(u) - 1)
    {
        ld a0 = u[i], b0 = u[i + 1];
        if (b0 - a0 < 1e-13L) continue;
        ld mid = (a0 + b0) * 0.5L;
        if (ev(r, d, c, mid) <= lim + EPS) s.pb({a0, b0});
    }

    for (ld x : u) if (ev(r, d, c, x) <= lim + EPS) s.pb({x, x});


    sort(all(s));
    vpld m;
    for (auto p : s)
    {
        if (m.empty()) m.pb(p);
        else
        {
            auto &q = m.back();
            if (p.fi <= q.se + 1e-9L)
            {
                q.fi = min(q.fi, p.fi);
                q.se = max(q.se, p.se);
            }
            else m.pb(p);
        }
    }

    return m;
}

static inline ld mniv(ld r, ld d, ld c, ld L, ld R)
{
    if (r < 1e-14L) return c;

    ld best = min(ev(r, d, c, L), ev(r, d, c, R));

    ld t0 = d + PI;
    ld mid = (L + R) * 0.5L;
    ll k = llround((mid - t0) / TAU);
    ld tt = t0 + (ld)k * TAU;
    if (tt + 1e-12L >= L && tt - 1e-12L <= R) best = min(best, c - r);

    return best;
}

static inline ld h(ld phi, ld l, ld a)
{
    phi = nrm(phi);
    ld s;
    if (phi <= a + 1e-18L) s = 1.0L;
    else s = max(cosl(phi), cosl(phi - a));
    if (s <= 0) return 0.0L;
    return l * s;
}

static inline ld sp(ld phi, ld l, ld a) { return h(phi, l, a) + h(phi + PI, l, a); }

static inline ld fv(ld b, ld l, ld a) { return sp(b + PI / 2, l, a); }

static inline ld gv(ld b, ld l, ld a) { return sp(b, l, a); }

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    ll ri, li, di;
    cin >> ri >> li >> di;

    ld r = (ld)ri;
    ld l = (ld)li;
    ld d = (ld)di;

    ld a = TAU * r / l;

    vld base;
    base.pb(0.0L);
    base.pb(a);
    base.pb(a * 0.5L);
    base.pb(a * 0.5L + PI);
    base.pb(PI * 0.5L);
    base.pb(PI * 1.5L);
    base.pb(a + PI * 0.5L);
    base.pb(a + PI * 1.5L);

    vld ph;
    for (ld b : base)
    { ph.pb(nrm(b)); ph.pb(nrm(b - PI)); }

    vld br;
    for (ld p : ph)
    { br.pb(nrm(p)); br.pb(nrm(p - PI / 2)); }
    br.pb(0.0L);
    br.pb(TAU);

    sort(all(br));
    vld q;
    for (ld x : br)
    {
        if (q.empty()) q.pb(x);
        else
        {
            if (fabsl(x - q.back()) > 1e-12L || (x == TAU && q.back() != TAU)) q.pb(x);
        }
    }
    if (fabsl(q.front()) > 1e-12L) q.insert(q.begin(), 0.0L);
    if (q.back() != TAU) q.pb(TAU);

    ld ans = 1e100L;

    vld shg{0.0L, PI};
    vld shf{PI / 2, PI * 1.5L};

    rep(i, 0, sz(q) - 1)
    {
        ld L = q[i], R = q[i + 1];
        if (R - L < 1e-13L) continue;
        ld mid = (L + R) * 0.5L;

        cf cg = sc(mid, shg, l, a);
        cf cf0 = sc(mid, shf, l, a);

        ld rg = hypotl(cg.a, cg.b);
        ld dg = (rg > 0 ? atan2l(cg.b, cg.a) : 0.0L);
        ld rf = hypotl(cf0.a, cf0.b);
        ld df = (rf > 0 ? atan2l(cf0.b, cf0.a) : 0.0L);

        auto ss = segs(rg, dg, cg.c, d, L, R);
        for (auto p : ss)
        {
            ld v = mniv(rf, df, cf0.c, p.fi, p.se);
            if (v < ans) ans = v;
        }
    }

    for (ld x : q) if (gv(x, l, a) <= d + 1e-9L) ans = min(ans, fv(x, l, a));


    cout.setf(ios::fixed);
    cout << setprecision(10) << (double)ans << '\n';

    return 0;
}
