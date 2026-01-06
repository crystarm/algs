// https://coderun.yandex.ru/selections/2025-winter-common/problems/energy-strike
// computational geometry + enumeration backtracking

#include <stdio.h>
#include <math.h>

#define rep(i,a,b) for (int i = (a); i <= (b); ++i)

const int MAXN = 12;
const int MAXC = 400;

const double EPS_COVER = 1e-9;
const double EPS_DIST = 1e-9;

struct pt { double x; double y; };

static struct pt p[MAXN];
static struct pt c[MAXC];
static int msk[MAXC];
static unsigned char bad[MAXC][MAXC];

static int n;
static int cn;

static int pick[MAXN];
static int pick_n;
static unsigned char ok;

static inline double dsq(struct pt a, struct pt b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx * dx + dy * dy;
}

static void add(double x, double y)
{
    if (cn >= MAXC) return;

    struct pt q;
    q.x = x;
    q.y = y;

    int m = 0;
    double lim = 1.0 + EPS_COVER;

    rep(i,0,n - 1) if (dsq(q, p[i]) <= lim) m |= (1 << i);

    if (m == 0) return;

    rep(i,0,cn - 1) if (msk[i] == m && dsq(c[i], q) < 1e-10) return;

    c[cn] = q;
    msk[cn] = m;
    ++cn;
}

static void bt(int cur, int cnt)
{
    if (ok) return;
    if (cur == (1 << n) - 1)
    { pick_n = cnt; ok = 1; return; }

    unsigned int need = ((1U << n) - 1U) & (unsigned int)(~cur);
    int bit = __builtin_ctz(need);

    rep(i,0,cn - 1)
    {
        if (!((msk[i] >> bit) & 1)) continue;

        unsigned char good = 1;
        rep(j,0,cnt - 1) if (bad[i][pick[j]]) { good = 0; break; }

        if (good)
        {
            pick[cnt] = i;
            bt(cur | msk[i], cnt + 1);
            if (ok) return;
        }
    }
}

static void solve()
{
    if (scanf("%d", &n) != 1) return;
    rep(i,0,n - 1) scanf("%lf %lf", &p[i].x, &p[i].y);

    cn = 0;
    ok = 0;

    rep(i,0,n - 1)
    {
        add(p[i].x, p[i].y);
        add(floor(p[i].x), floor(p[i].y));
        add(ceil(p[i].x), floor(p[i].y));
        add(floor(p[i].x), ceil(p[i].y));
        add(ceil(p[i].x), ceil(p[i].y));
    }

    rep(i,0,n - 1) rep(j,i + 1,n - 1)
    {
        double d2 = dsq(p[i], p[j]);
        if (d2 > 4.0 + EPS_DIST) continue;

        add((p[i].x + p[j].x) / 2.0, (p[i].y + p[j].y) / 2.0);

        double d = sqrt(d2);
        if (d > 1e-9)
        {
            double h2 = 1.0 - d2 / 4.0;
            double h = (h2 > 0.0) ? sqrt(h2) : 0.0;

            double mx = (p[i].x + p[j].x) / 2.0;
            double my = (p[i].y + p[j].y) / 2.0;

            double dx = h * (p[i].y - p[j].y) / d;
            double dy = h * (p[j].x - p[i].x) / d;

            add(mx + dx, my + dy);
            add(mx - dx, my - dy);
        }
    }

    double lim = 4.0 - 1e-9;
    rep(i,0,cn - 1) rep(j,i,cn - 1)
    {
        unsigned char x = (dsq(c[i], c[j]) < lim);
        bad[i][j] = x;
        bad[j][i] = x;
    }

    bt(0, 0);

    if (ok)
    {
        printf("YES\n%d\n", pick_n);
        rep(i,0,pick_n - 1) printf("%.10f %.10f\n", c[pick[i]].x, c[pick[i]].y);
    } else printf("NO\n");
}

int main()
{
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) solve();
    return 0;
}
