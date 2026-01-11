// https://acm.timus.ru/problem.aspx?space=1&num=2196
// computational geometry + point-in-convex-polygon (O(log N)) + point-to-segment distance + Minkowski sum / offset (Steiner formula)

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define pb push_back

const int INF = 1e9;
const ll LINF = (ll)4e18;

struct pt { ll x, y;};

static int n;
static vector<pt> poly;
static ld S0, P0;
static const ld PI = acosl(-1.0L);

static inline ll dist2(const pt& a, const pt& b)
{
    ll dx = a.x - b.x;
    ll dy = a.y - b.y;
    return dx * dx + dy * dy;
}

static inline ld dist_pt(const pt& a, const pt& b) { return sqrtl((ld)dist2(a, b)); }

static inline __int128 cross(const pt& a, const pt& b, const pt& c)
{ return (__int128)(b.x - a.x) * (c.y - a.y) - (__int128)(b.y - a.y) * (c.x - a.x); }

static inline bool inside(const pt& p)
{

    if (cross(poly[0], poly[1], p) < 0) return false;
    if (cross(poly[n - 1], poly[0], p) < 0) return false;

    int l = 1, r = n - 1;
    while (r - l > 1)
    {
        int m = (l + r) >> 1;
        if (cross(poly[0], poly[m], p) >= 0) l = m;
        else r = m;
    }

    return cross(poly[l], poly[l + 1], p) >= 0;
}

static inline int ternary_vertex(int l, int r, const pt& p)
{
    while (r - l > 2)
    {
        int m1 = l + (r - l) / 3;
        int m2 = r - (r - l) / 3;
        if (dist2(poly[m1], p) < dist2(poly[m2], p)) r = m2;
        else l = m1;
    }

    int best = l;
    ll bestd = dist2(poly[l], p);
    for (int i = l + 1; i <= r; ++i)
    {
        ll d = dist2(poly[i], p);
        if (d < bestd) { bestd = d; best = i; }
    }
    return best;
}

static inline int closest_vertex(const pt& p)
{
    int i1 = ternary_vertex(0, n / 2, p);
    int i2 = ternary_vertex(n / 2, n - 1, p);
    if (dist2(poly[i1], p) < dist2(poly[i2], p)) return i1;
    return i2;
}

static inline ld dist_to_seg(const pt& p, const pt& a, const pt& b)
{
    ll l2 = dist2(a, b);
    if (l2 == 0) return dist_pt(p, a);

    ld abx = (ld)(b.x - a.x);
    ld aby = (ld)(b.y - a.y);
    ld apx = (ld)(p.x - a.x);
    ld apy = (ld)(p.y - a.y);

    ld t = (apx * abx + apy * aby) / (ld)l2;
    if (t < 0) t = 0;
    if (t > 1) t = 1;

    ld px = (ld)a.x + t * abx;
    ld py = (ld)a.y + t * aby;

    ld dx = (ld)p.x - px;
    ld dy = (ld)p.y - py;
    return sqrtl(dx * dx + dy * dy);
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    if (!(cin >> n)) return 0;
    poly.assign(n, {});
    rep(i, 0, n) cin >> poly[i].x >> poly[i].y;

    ld a2 = 0;
    P0 = 0;
    rep(i, 0, n)
    {
        int j = (i + 1) % n;
        a2 += (ld)poly[i].x * (ld)poly[j].y - (ld)poly[j].x * (ld)poly[i].y;
        P0 += dist_pt(poly[i], poly[j]);
    }
    S0 = fabsl(a2) / 2.0L;

    int q;
    cin >> q;
    while (q--)
    {
        pt p;
        cin >> p.x >> p.y;

        if (inside(p)) { cout << 0 << '\n'; continue; }

        int id = closest_vertex(p);
        int prv = (id - 1 + n) % n;
        int nxt = (id + 1) % n;

        ld d1 = dist_to_seg(p, poly[prv], poly[id]);
        ld d2 = dist_to_seg(p, poly[id], poly[nxt]);
        ld d = min(d1, d2);

        if (d < 1e-12L) { cout << 0 << '\n'; continue; }

        ld ratio = (S0 + P0 * d + PI * d * d) / S0;

        if (ratio <= 1.0L) { cout << 0 << '\n'; continue; }

        ld v = log2l(ratio);
        ll ans = (ll)ceill(v - 1e-12L);
        if (ans < 0) ans = 0;
        cout << ans << '\n';
    }

    return 0;
}
