// https://acm.timus.ru/problem.aspx?space=1&num=1271
// computational geometry + motion planning (no rotation)+ Minkowski sum + convex hull + Dijkstra + EPS

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;
typedef vector<long double> vld;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)((x).size())
#define pb push_back
#define ppb pop_back
#define ins insert
#define resz resize
#define rsr(v,n) (v).reserve(n)

static const ld EPS = 1e-12L;
static const ld INF = 1e100L;

struct pt { ld x, y; }; typedef vector<pt> vld;


static inline pt operator + (const pt &a, const pt &b)          { return {a.x + b.x, a.y + b.y}; }
static inline pt operator - (const pt &a, const pt &b)          { return {a.x - b.x, a.y - b.y}; }
static inline pt operator * (const pt &a, ld k)                 { return {a.x * k, a.y * k}; }
static inline pt operator / (const pt &a, ld k)                 { return {a.x / k, a.y / k}; }

static inline ld xmul(const pt &a, const pt &b)                 { return a.x * b.y - a.y * b.x; }
static inline ld xmul(const pt &a, const pt &b, const pt &c)    { return xmul(b - a, c - a); }
static inline ld dmul(const pt &a, const pt &b)                 { return a.x * b.x + a.y * b.y; }
static inline ld n2(const pt &a)                                { return dmul(a, a); }
static inline ld d2(const pt &a, const pt &b)                   { return n2(a - b); }
static inline ld d1(const pt &a, const pt &b)                   { return sqrtl(d2(a, b)); }



static inline int sgn(ld v) { if (v > EPS) return 1; if (v < -EPS) return -1; return 0; }

static inline bool leq(ld a, ld b) { return a <= b + EPS; }
static inline bool geq(ld a, ld b) { return a + EPS >= b; }

static inline bool onseg(const pt &p, const pt &a, const pt &b)
{   if (sgn(xmul(a, b, p)) != 0) return false;
    return leq(min(a.x, b.x), p.x) && leq(p.x, max(a.x, b.x)) && leq(min(a.y, b.y), p.y) && leq(p.y, max(a.y, b.y));

}


static inline bool segx(const pt &a, const pt &b, const pt &c, const pt &d)
{
    int o1 = sgn(xmul(a, b, c));
    int o2 = sgn(xmul(a, b, d));
    int o3 = sgn(xmul(c, d, a));
    int o4 = sgn(xmul(c, d, b));

    if (o1 == 0 && onseg(c, a, b)) return true;
    if (o2 == 0 && onseg(d, a, b)) return true;
    if (o3 == 0 && onseg(a, c, d)) return true;
    if (o4 == 0 && onseg(b, c, d)) return true;

    return (o1 * o2 < 0) && (o3 * o4 < 0);
}

static inline bool linex(const pt &a, const pt &b, const pt &c, const pt &d, pt &out)
{
    pt r = b - a;
    pt s = d - c;
    ld den = xmul(r, s);
    if (fabsl(den) < EPS) return false;
    ld t = xmul(c - a, s) / den;
    out = a + r * t;
    return true;
}

static inline vpt hull(vpt p)
{
    sort(all(p), [](const pt &a, const pt &b) { if (a.x != b.x) return a.x < b.x; return a.y < b.y; });

    vpt u;
    for (auto &pt : p) if (u.empty() || fabsl(u.back().x - pt.x) > 1e-15L || fabsl(u.back().y - pt.y) > 1e-15L) u.pb(pt);
    p.swap(u);

    if (sz(p) <= 1) return p;

    vpt lo, hi;

    for (auto &pt : p)
    { while (sz(lo) >= 2 && sgn(xmul(lo[sz(lo) - 2], lo.back(), pt)) <= 0) lo.ppb(); lo.pb(pt); }

    for (int i = sz(p) - 1; i >= 0; --i)
    {
        auto pt = p[i];
        while (sz(hi) >= 2 && sgn(xmul(hi[sz(hi) - 2], hi.back(), pt)) <= 0) hi.ppb();
        hi.pb(pt);
    }

    lo.ppb();
    hi.ppb();
    lo.ins(lo.end(), all(hi));

    if (sz(lo) == 2 && d2(lo[0], lo[1]) < 1e-24L) lo.resz(1);
    return lo;
}

static inline ld area2(const vpt &p)
{
    int n = sz(p);
    ld s = 0;
    for (int i = 0; i < n; ++i)
    {
        pt a = p[i];
        pt b = p[(i + 1) % n];
        s += xmul(a, b);
    }
    return s;
}

static inline bool in_strict_ccw(const vpt &poly, const pt &p)
{
    int n = sz(poly);
    for (int i = 0; i < n; ++i)
    {
        pt a = poly[i];
        pt b = poly[(i + 1) % n];
        if (xmul(b - a, p - a) <= EPS) return false;
    }

    return true;
}

struct poly { vpt v; ld minx, maxx, miny, maxy; };

static inline poly mkpoly(vpt v)
{
    if (area2(v) < 0) reverse(all(v));

    poly p;
    p.v = v;
    p.minx = p.miny = INF;
    p.maxx = p.maxy = -INF;
    for (auto &pt : v)
    {
        p.minx = min(p.minx, pt.x);
        p.maxx = max(p.maxx, pt.x);
        p.miny = min(p.miny, pt.y);
        p.maxy = max(p.maxy, pt.y);
    }
    return p;
}

static inline bool bbox(const poly &poly, const pt &a, const pt &b)
{
    ld mnx = min(a.x, b.x);
    ld mxx = max(a.x, b.x);
    ld mny = min(a.y, b.y);
    ld mxy = max(a.y, b.y);

    if (mxx < poly.minx - EPS || mnx > poly.maxx + EPS) return false;
    if (mxy < poly.miny - EPS || mny > poly.maxy + EPS) return false;

    return true;
}

static inline bool seg_hit_interior(const poly &poly, const pt &a, const pt &b)
{
    if (!bbox(poly, a, b)) return false;

    pt d = b - a;
    ld lo = 0, hi = 1;

    int n = sz(poly.v);
    for (int i = 0; i < n; ++i)
    {
        pt p = poly.v[i];
        pt q = poly.v[(i + 1) % n];
        pt e = q - p;

        ld A = xmul(e, a - p);
        ld B = xmul(e, d);

        if (fabsl(B) < EPS)
        { if (A < -EPS) return false; continue; }

        ld t = -A / B;
        if (B > 0) lo = max(lo, t);
        else hi = min(hi, t);

        if (lo > hi + EPS) return false;
    }

    lo = max(lo, (ld)0);
    hi = min(hi, (ld)1);
    if (lo > hi + EPS) return false;

    ld mid = (lo + hi) / 2.0L;
    pt pm = a + d * mid;

    return in_strict_ccw(poly.v, pm);
}

static inline bool vis(const pt &a, const pt &b, const vector<poly> &obs)
{ for (auto &poly : obs) if (seg_hit_interior(poly, a, b)) return false; return true; }

static inline bool inrect(const pt &p, ld x0, ld x1, ld y0, ld y1)
{ return geq(p.x, x0) && leq(p.x, x1) && geq(p.y, y0) && leq(p.y, y1);}

static inline void addx(vpt &pts, const pt &a, const pt &b, const pt &c, const pt &d)
{
    if (!segx(a, b, c, d)) return;

    pt r = b - a;
    pt s = d - c;
    ld den = xmul(r, s);

    if (fabsl(den) < EPS)
    {
        pt cand[4] = {a, b, c, d};
        for (int i = 0; i < 4; ++i)
        {
            pt p = cand[i];
            if (onseg(p, a, b) && onseg(p, c, d)) pts.pb(p);
        }
        return;
    }

    pt ip;
    if (linex(a, b, c, d, ip)) pts.pb(ip);
}

typedef pair<ld, int> pli;
typedef vector<pli> vpli;
typedef priority_queue<pli, vpli, greater<pli>> minpq;

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    ll dxi, dyi;
    if (!(cin >> dxi >> dyi)) return 0;
    ld dx = (ld)dxi;
    ld dy = (ld)dyi;

    pt p0, p1, p2;
    cin >> p0.x >> p0.y;
    cin >> p1.x >> p1.y;
    cin >> p2.x >> p2.y;

    pt r1 = p1 - p0;
    pt r2 = p2 - p0;

    ld x0 = max((ld)0, max(-r1.x, -r2.x));
    ld x1 = min(dx, min(dx - r1.x, dx - r2.x));
    ld y0 = max((ld)0, max(-r1.y, -r2.y));
    ld y1 = min(dy, min(dy - r1.y, dy - r2.y));

    if (!(x0 <= x1 + EPS && y0 <= y1 + EPS))
    { cout << -1 << "\n"; return 0; }

    pt g;
    cin >> g.x >> g.y;
    pt s = p0;

    if (!inrect(s, x0, x1, y0, y1) || !inrect(g, x0, x1, y0, y1))
    { cout << -1 << "\n"; return 0; }

    int n;
    cin >> n;

    vector<poly> obs;
    rsr(obs, n);

    for (int i = 0; i < n; ++i)
    {
        pt q0, q1, q2;
        cin >> q0.x >> q0.y;
        cin >> q1.x >> q1.y;
        cin >> q2.x >> q2.y;

        vpt pts;
        rsr(pts, 9);

        pt qs[3] = {q0, q1, q2};
        for (int k = 0; k < 3; ++k)
        {
            pts.pb(qs[k]);
            pts.pb(qs[k] - r1);
            pts.pb(qs[k] - r2);
        }

        auto hv = hull(pts);
        if (sz(hv) >= 3) obs.pb(mkpoly(hv));
    }

    for (auto &poly : obs)
    {
        if (in_strict_ccw(poly.v, s) || in_strict_ccw(poly.v, g))
        { cout << -1 << "\n"; return 0; }
    }

    vpt vtx;
    rsr(vtx, 20000);

    vtx.pb(s);
    vtx.pb(g);

    pt R[4] = {{x0, y0}, {x1, y0}, {x1, y1}, {x0, y1}};

    for (auto &poly : obs)
    {
        for (auto &pt : poly.v) vtx.pb(pt);

        int m = sz(poly.v);
        for (int i = 0; i < m; ++i)
        {
            pt a = poly.v[i];
            pt b = poly.v[(i + 1) % m];
            for (int e = 0; e < 4; ++e)
            {
                pt c = R[e];
                pt d = R[(e + 1) % 4];
                addx(vtx, a, b, c, d);
            }
        }
    }

    int kobs = sz(obs);
    for (int i = 0; i < kobs; ++i)
    {
        int mi = sz(obs[i].v);
        for (int j = i + 1; j < kobs; ++j)
        {
            int mj = sz(obs[j].v);
            for (int a = 0; a < mi; ++a)
            {
                pt A = obs[i].v[a];
                pt B = obs[i].v[(a + 1) % mi];
                for (int b = 0; b < mj; ++b)
                {
                    pt C = obs[j].v[b];
                    pt D = obs[j].v[(b + 1) % mj];
                    addx(vtx, A, B, C, D);
                }
            }
        }
    }

    for (int i = 0; i < 4; ++i) vtx.pb(R[i]);

    sort(all(vtx), [](const pt &a, const pt &b)
    { if (a.x != b.x) return a.x < b.x; return a.y < b.y; });

    vpt uq;
    rsr(uq, sz(vtx));

    for (auto &p : vtx)
    {
        if (!uq.empty() && fabsl(uq.back().x - p.x) < 1e-11L && fabsl(uq.back().y - p.y) < 1e-11L) continue;
        uq.pb(p);
    }

    vtx.clear();
    rsr(vtx, sz(uq));

    for (auto &p : uq)
    {
        if (!inrect(p, x0, x1, y0, y1)) continue;
        bool bad = false;
        for (auto &poly : obs) if (in_strict_ccw(poly.v, p)) { bad = true; break; }

        if (!bad) vtx.pb(p);
    }

    int m = sz(vtx);

    int si = -1, ti = -1;
    for (int i = 0; i < m; ++i)
    {
        if (d2(vtx[i], s) < 1e-18L) si = i;
        if (d2(vtx[i], g) < 1e-18L) ti = i;
    }

    if (si == -1 || ti == -1)
    { cout << -1 << "\n"; return 0; }

    vld distv(m, INF);
    distv[si] = 0;

    minpq pq;
    pq.push({0, si});

    while (!pq.empty())
    {
        auto [cd, v] = pq.top();
        pq.pop();
        if (cd > distv[v] + 1e-18L) continue;
        if (v == ti) break;

        for (int to = 0; to < m; ++to)
        {
            if (to == v) continue;
            if (!vis(vtx[v], vtx[to], obs)) continue;
            ld nd = cd + d1(vtx[v], vtx[to]);
            if (nd + 1e-18L < distv[to])
            {
                distv[to] = nd;
                pq.push({nd, to});
            }
        }
    }

    if (distv[ti] >= INF / 2)
    { cout << -1 << "\n"; return 0; }

    cout.setf(std::ios::fixed);
    cout << setprecision(3) << (double)distv[ti] << "\n";
    return 0;
}
