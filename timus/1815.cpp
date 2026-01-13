// https://acm.timus.ru/problem.aspx?space=1&num=1815
// computational geometry + weighted geometric median (Fermat–Weber) + floating-point robustness

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef double ld;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)((x).size())
#define rep(i,a,b) for (int i = (a); i < (b); i++)
#define pb push_back

struct pt { ld x, y; };

static inline pt operator + (const pt &a, const pt &b) { return pt{a.x + b.x, a.y + b.y}; }
static inline pt operator - (const pt &a, const pt &b) { return pt{a.x - b.x, a.y - b.y}; }
static inline pt operator * (const pt &a, ld k) { return pt{a.x * k, a.y * k}; }

static inline ld dot(const pt &a, const pt &b) { return a.x * b.x + a.y * b.y; }
static inline ld cross(const pt &a, const pt &b) { return a.x * b.y - a.y * b.x; }
static inline ld norm2(const pt &a) { return dot(a, a); }
static inline ld norm(const pt &a) { return sqrt(norm2(a)); }

static inline ld dist_pt(const pt &a, const pt &b)
{
    ld dx = a.x - b.x;
    ld dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

static inline ld fval(const array<pt, 3> &p, const array<ld, 3> &w, const pt &q)
{
    ld s = 0.0;
    rep(i, 0, 3) s += w[i] * dist_pt(q, p[i]);
    return s;
}

static inline pt grad_vec(const array<pt, 3> &p, const array<ld, 3> &w, const pt &q)
{
    pt g{0.0, 0.0};
    rep(i, 0, 3)
    {
        pt d = q - p[i];
        ld r = norm(d);
        if (r < 1e-18) continue;
        g = g + d * (w[i] / r);
    }
    return g;
}

struct cir { pt c; ld r; };

static inline vector<cir> mkcir(const pt &a, const pt &b, ld cos_t, ld sin_t)
{
    ld L = dist_pt(a, b);
    if (L < 1e-18 || sin_t < 1e-18) return {};

    ld r = L / (2.0 * sin_t);
    ld h2 = r * r - (L * 0.5) * (L * 0.5);
    if (h2 < 0.0) h2 = 0.0;
    ld h = sqrt(h2);

    pt m{ (a.x + b.x) * 0.5, (a.y + b.y) * 0.5 };
    pt d = b - a;
    pt n{ -d.y / L, d.x / L };

    cir c1{ m + n * h, r };
    cir c2{ m - n * h, r };
    return { c1, c2 };
}

static inline vector<pt> isec(const cir &A, const cir &B)
{
    const ld EPS = 1e-12;

    pt d = B.c - A.c;
    ld dist = norm(d);
    if (dist < EPS) return {};

    if (dist > A.r + B.r + 1e-10) return {};
    if (dist < fabs(A.r - B.r) - 1e-10) return {};

    ld a = (A.r * A.r - B.r * B.r + dist * dist) / (2.0 * dist);
    ld h2 = A.r * A.r - a * a;
    if (h2 < -1e-10) return {};
    if (h2 < 0.0) h2 = 0.0;
    ld h = sqrt(h2);

    pt u = d * (1.0 / dist);
    pt p0 = A.c + u * a;

    pt perp{-u.y, u.x};
    pt off = perp * h;

    pt p1 = p0 + off;
    pt p2 = p0 - off;

    if (h < 1e-12) return { p0 };
    return { p1, p2 };
}

int main()
{

    ios::sync_with_stdio(0);
    cin.tie(0);

    int t;
    cin >> t;

    cout.setf(std::ios::fixed);
    cout << setprecision(15);

    while (t--)
    {
        ll x0, y0, x1, y1, x2, y2;
        ll c0, c1, c2;
        cin >> x0 >> y0;
        cin >> x1 >> y1;
        cin >> x2 >> y2;
        cin >> c0 >> c1 >> c2;

        vector<pair<ll, ll>> ip = { {x0, y0}, {x1, y1}, {x2, y2} };
        vector<ld> iw = { (ld)c0, (ld)c1, (ld)c2 };

        vector<pair<ll, ll>> mp;
        vector<ld> mw;

        rep(i, 0, 3)
        {
            bool ok = 0;
            rep(j, 0, sz(mp))
            {
                if (mp[j] == ip[i])
                {
                    mw[j] += iw[i];
                    ok = 1;
                    break;
                }
            }
            if (!ok)
            { mp.pb(ip[i]); mw.pb(iw[i]); }

        }

        int n = sz(mp);

        if (n == 1) { cout << 0.0 << '\n'; continue; }

        if (n == 2)
        {
            pt a{ (ld)mp[0].first, (ld)mp[0].second };
            pt b{ (ld)mp[1].first, (ld)mp[1].second };
            ld L = dist_pt(a, b);
            ld ans = L * min(mw[0], mw[1]);
            cout << ans << '\n';
            continue;
        }

        array<pt, 3> p;
        array<ld, 3> w;
        rep(i, 0, 3)
        {
            p[i] = pt{ (ld)mp[i].first, (ld)mp[i].second };
            w[i] = mw[i];
        }

        ld best = 1e100;

        rep(i, 0, 3) best = min(best, fval(p, w, p[i]));

        bool vertex_ok = 0;
        rep(i, 0, 3)
        {
            pt g{0.0, 0.0};
            rep(j, 0, 3)
            {
                if (j == i) continue;
                pt v = p[i] - p[j];
                ld d = norm(v);
                if (d < 1e-18) continue;
                g = g + v * (w[j] / d);
            }
            if (norm(g) <= w[i] + 1e-12)
            {
                best = min(best, fval(p, w, p[i]));
                vertex_ok = 1;
            }
        }

        if (vertex_ok) { cout << best << '\n'; continue; }

        pt a = p[1] - p[0];
        pt b = p[2] - p[0];
        if (fabs(cross(a, b)) <= 1e-12)
        {
            pt dir = p[1] - p[0];
            if (norm(dir) < 1e-18) dir = p[2] - p[0];
            ld Ld = norm(dir);
            dir = dir * (1.0 / Ld);

            array<ld, 3> tt;
            rep(i, 0, 3) tt[i] = dot(p[i] - p[0], dir);

            vector<int> ord = {0, 1, 2};
            sort(all(ord), [&](int i, int j){ return tt[i] < tt[j]; });

            ld W = w[0] + w[1] + w[2];
            ld cur = 0.0;
            int med = ord[0];
            rep(k, 0, 3)
            {
                cur += w[ord[k]];
                if (cur * 2.0 >= W - 1e-15) { med = ord[k]; break; }
            }

            ld tmed = tt[med];
            ld ans = 0.0;
            rep(i, 0, 3) ans += w[i] * fabs(tmed - tt[i]);

            cout << min(best, ans) << '\n';
            continue;
        }

        auto clamp01 = [&](ld x)
        {
            if (x < -1.0) return -1.0;
            if (x > 1.0) return 1.0;
            return x;
        };

        ld w0 = w[0], w1 = w[1], w2 = w[2];

        ld cos01 = clamp01((w2 * w2 - w0 * w0 - w1 * w1) / (2.0 * w0 * w1));
        ld sin01 = sqrt(max<ld>(0.0, 1.0 - cos01 * cos01));

        ld cos02 = clamp01((w1 * w1 - w0 * w0 - w2 * w2) / (2.0 * w0 * w2));
        ld sin02 = sqrt(max<ld>(0.0, 1.0 - cos02 * cos02));

        vector<cir> c01 = mkcir(p[0], p[1], cos01, sin01);
        vector<cir> c02 = mkcir(p[0], p[2], cos02, sin02);

        ld best_g = 1e100;

        rep(i, 0, sz(c01))
        {
            rep(j, 0, sz(c02))
            {
                vector<pt> inter = isec(c01[i], c02[j]);
                for (auto &q : inter)
                {
                    if (!isfinite(q.x) || !isfinite(q.y)) continue;
                    pt g = grad_vec(p, w, q);
                    ld gn = norm(g);
                    ld fv = fval(p, w, q);

                    if (gn < best_g - 1e-12) { best_g = gn; best = fv; }
                    else if (fabs(gn - best_g) <= 1e-12) best = min(best, fv);
                }
            }
        } cout << best << '\n';
    }

    return 0;
}
