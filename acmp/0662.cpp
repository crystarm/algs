// https://acmp.ru/index.asp?main=task&id_task=662
// computational geometry + sweep line

#include <bits/stdc++.h>
using namespace std;

typedef string str;
typedef vector<double> vd;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr(v,n) (v).reserve(n)
#define pb push_back


static const double EPS = 1e-9;

struct pl { int a, b, c; };

static inline double sc(const pl& p, double x) { return (double)p.a * x * x + (double)p.b * x + (double)p.c; }

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    if (!(cin >> n)) return 0;

    vector<pl> v(n);
    rep(i, 0, n) cin >> v[i].a >> v[i].b >> v[i].c;

    vd crit;
    rsr(crit, n * n + 2);
    crit.pb(1.0);

    rep(i, 0, n)
    {
        rep(j, i + 1, n)
        {
            double A = (double)v[i].a - (double)v[j].a;
            double B = (double)v[i].b - (double)v[j].b;
            double C = (double)v[i].c - (double)v[j].c;

            if (fabs(A) < EPS)
            {
                if (fabs(B) > EPS)
                {
                    double x = -C / B;
                    if (x > EPS && x <= 1.0 + EPS) crit.pb(x);
                }
            }
            else
            {
                double D = B * B - 4.0 * A * C;
                if (D > -EPS)
                {
                    if (D < 0) D = 0;
                    double r = sqrt(D);
                    double x1 = (-B - r) / (2.0 * A);
                    double x2 = (-B + r) / (2.0 * A);

                    if (x1 > EPS && x1 <= 1.0 + EPS) crit.pb(x1);
                    if (x2 > EPS && x2 <= 1.0 + EPS) crit.pb(x2);
                }
            }
        }
    }

    sort(all(crit));

    vector<str> out;
    vd scr(n);

    auto chk = [&](double x)
    {
        if (x <= 0 || x > 1.0 + EPS) return;

        rep(i, 0, n) scr[i] = sc(v[i], x);

        str r(n, '\0');
        rep(i, 0, n)
        {
            int bt = 0;
            rep(j, 0, n) if (scr[j] > scr[i] + EPS) ++bt;
            r[i] = (char)(bt + 1);
        }
        out.pb(r);
    };

    double prv = 0.0;
    double last = -1.0;

    for (double p : crit)
    {
        if (fabs(p - last) < EPS) continue;

        if (p > prv + EPS)
        {
            double mid = (prv + p) * 0.5;
            chk(mid);
        }

        chk(p);

        prv = p;
        last = p;
    }

    sort(all(out));
    out.erase(unique(all(out)), out.end());

    cout << sz(out) << '\n';
    return 0;
}
