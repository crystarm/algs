// https://acmp.ru/index.asp?main=task&id_task=959
// computational geometry

#include <bits/stdc++.h>
using namespace std;

#define all(x) (x).begin(), (x).end()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr reserve
#define pb push_back
#define se second

struct pt { double x, y; };

const double EPS = 1e-9;

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    if (!(cin >> n)) return 0;

    double a[3];
    cin >> a[0] >> a[1] >> a[2];

    vector<pt> p(n);
    rep(i, 0, n) cin >> p[i].x >> p[i].y;

    if (n == 0) { cout << 0 << '\n'; return 0; }
    if (n == 1) { cout << 1 << '\n'; return 0; }

    int ans = 1;
    int perm[3][3] = {{0, 2, 1}, {1, 0, 2}, {2, 1, 0}};

    rep(i, 0, n)
    {
        rep(j, 0, n)
        {
            if (i == j) continue;

            double dx = p[j].x - p[i].x;
            double dy = p[j].y - p[i].y;
            double d = sqrt(dx * dx + dy * dy);

            double co = dx / d;
            double si = dy / d;

            rep(k, 0, 3)
            {
                double base = a[perm[k][0]];
                double left = a[perm[k][1]];
                double right = a[perm[k][2]];

                if (d > base + EPS) continue;

                double min_s = d - base;
                double max_s = 0.0;

                if (min_s > max_s + EPS) continue;

                double top_x = (base * base + left * left - right * right) / (2 * base);
                double h_sq = left * left - top_x * top_x;
                double h = (h_sq < 0) ? 0 : sqrt(h_sq);

                for (int dir = -1; dir <= 1; dir += 2)
                {
                    vector<pair<double, int>> ev;
                    ev.rsr(n * 2);

                    rep(m, 0, n)
                    {
                        double tx = p[m].x - p[i].x;
                        double ty = p[m].y - p[i].y;

                        double rx = tx * co + ty * si;
                        double ry = -tx * si + ty * co;

                        if (dir == -1) ry = -ry;

                        if (ry < -EPS || ry > h + EPS) continue;

                        double s_start, s_end;

                        if (h < EPS)
                        {
                            if (abs(ry) > EPS) continue;
                            s_start = rx - base;
                            s_end = rx;
                        }
                        else
                        {
                            double d1 = ry * (top_x / h);
                            double d2 = base + ry * ((top_x - base) / h);
                            s_start = rx - d2;
                            s_end = rx - d1;
                        }

                        double real_start = max(s_start, min_s - EPS);
                        double real_end = min(s_end, max_s + EPS);

                        if (real_start <= real_end + EPS)
                        {
                            ev.pb({real_start, -1});
                            ev.pb({real_end, 1});
                        }
                    }

                    sort(all(ev));

                    int cur = 0;
                    for (auto &e : ev)
                    {
                        if (e.se == -1) cur++;
                        else { if (cur > ans) ans = cur; cur--; }
                    }
                    if (cur > ans) ans = cur;
                }
            }
        }
    }

    cout << ans << '\n';
    return 0;
}
