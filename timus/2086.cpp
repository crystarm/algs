// https://acm.timus.ru/author.aspx?id=404938
// analytic geometry + law of cosines + unit vectors + line parameterization + numeric stability + epsilon handling + deduplication

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define ins insert
#define pb push_back
#define fi first
#define se second
#define llr llround

static ld dist2(ld ax, ld ay, ld bx, ld by)
{
    ld dx = ax - bx;
    ld dy = ay - by;
    return dx * dx + dy * dy;
}

static ld dist(ld ax, ld ay, ld bx, ld by) { return sqrtl(dist2(ax, ay, bx, by)); }

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    ll Xi, Yi, Zi;
    ll x1i, y1i, x2i, y2i;
    if (!(cin >> Xi >> Yi >> Zi)) return 0;
    cin >> x1i >> y1i;
    cin >> x2i >> y2i;

    ld X = (ld)Xi, Y = (ld)Yi, Z = (ld)Zi;
    ld x1 = (ld)x1i, y1 = (ld)y1i;
    ld x2 = (ld)x2i, y2 = (ld)y2i;

    ld vx = x1 - x2;
    ld vy = y1 - y2;

    ld a = vx * vx + vy * vy;
    ld r = sqrtl(a);

    const ld eps = 1e-12L;

    ld A = Z;
    ld B = Y * Y - Z * Z - X * X;
    ld C = Z * (X * X - a);

    ld D = B * B - 4.0L * A * C;

    vector<pair<ld, ld>> pts;

    auto add_point = [&](ld px, ld py) { pts.pb({px, py}); };

    if (D >= -1e-10L)
    {
        if (D < 0) D = 0;
        ld sq = sqrtl(D);
        ld t1 = (-B + sq) / (2.0L * A);
        ld t2 = (-B - sq) / (2.0L * A);

        vector<ld> roots;
        auto add_root = [&](ld t)
        {
            if (t < -1e-8L || t > Z + 1e-8L) return;
            if (fabsl(t) < 1e-9L) t = 0;
            if (fabsl(t - Z) < 1e-9L) t = Z;
            for (ld u : roots) if (fabsl(t - u) < 1e-8L) return;
            roots.pb(t);
        };

        add_root(t1);
        add_root(t2);

        for (ld t : roots)
        {
            if (t == 0)
            {
                if (fabsl(r - X) > 1e-7L) continue;
                ld s = (a + Z * Z - Y * Y) / (2.0L * Z);
                ld c = s / r;
                if (fabsl(c) > 1.0L + 1e-9L) continue;
                add_point(x2, y2);
                continue;
            }

            ld s = (X * X - a - t * t) / (2.0L * t);
            ld c = s / r;

            if (fabsl(c) > 1.0L + 1e-9L) continue;
            if (c > 1) c = 1;
            if (c < -1) c = -1;

            ld si = sqrtl(max((ld)0.0L, 1.0L - c * c));

            ld e1x = vx / r;
            ld e1y = vy / r;
            ld e2x = -e1y;
            ld e2y = e1x;

            for (int sign : {1, -1})
            {
                if (si < 1e-12L && sign == -1) continue;

                ld ux = c * e1x + (ld)sign * si * e2x;
                ld uy = c * e1y + (ld)sign * si * e2y;

                ld dx = x2 - t * ux;
                ld dy = y2 - t * uy;

                ld fx = x2 + (Z - t) * ux;
                ld fy = y2 + (Z - t) * uy;

                if (fabsl(dist(dx, dy, x1, y1) - X) > 1e-5L) continue;
                if (fabsl(dist(fx, fy, x1, y1) - Y) > 1e-5L) continue;
                if (fabsl(dist(dx, dy, fx, fy) - Z) > 1e-5L) continue;

                add_point(dx, dy);
            }
        }
    }

    set<pair<ll, ll>> seen;
    vector<pair<ld, ld>> out;
    for (auto [px, py] : pts)
    {
        ll kx = llr(px * 1e7L);
        ll ky = llr(py * 1e7L);
        pair<ll, ll> key = {kx, ky};
        if (seen.count(key)) continue;
        seen.ins(key);
        out.pb({px, py});
    }

    cout << sz(out) << "\n";
    cout.setf(std::ios::fixed);
    cout << setprecision(15);
    for (auto [px, py] : out) cout << (double)px << " " << (double)py << "\n";

    return 0;
}
