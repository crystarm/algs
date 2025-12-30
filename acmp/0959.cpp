// https://acmp.ru/index.asp?main=task&id_task=959
// computational geometry

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct pt { double x, y; };

const double eps = 1e-9;

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n;
    if (!(cin >> n)) return 0;

    double s[3];
    cin >> s[0] >> s[1] >> s[2];

    vector<pt> p(n);
    for (int i = 0; i < n; ++i) cin >> p[i].x >> p[i].y;

    if (n == 0) { cout << 0 << endl; return 0; }
    if (n == 1) { cout << 1 << endl; return 0; }

    int ans = 1;
    int perms[3][3] = {{0, 2, 1}, {1, 0, 2}, {2, 1, 0}};

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (i == j) continue;

            double dx = p[j].x - p[i].x;
            double dy = p[j].y - p[i].y;
            double d = sqrt(dx * dx + dy * dy);

            double co = dx / d;
            double si = dy / d;

            for (int k = 0; k < 3; ++k)
            {
                double base = s[perms[k][0]];
                double left = s[perms[k][1]];
                double right = s[perms[k][2]];

                if (d > base + eps) continue;

                double min_s = d - base;
                double max_s = 0.0;

                if (min_s > max_s + eps) continue;

                double top_x = (base * base + left * left - right * right) / (2 * base);
                double h_sq = left * left - top_x * top_x;
                double h = (h_sq < 0) ? 0 : sqrt(h_sq);

                for (int dir = -1; dir <= 1; dir += 2)
                {
                    vector<pair<double, int>> ev;
                    ev.reserve(n * 2);

                    for (int m = 0; m < n; ++m)
                    {
                        double tx = p[m].x - p[i].x;
                        double ty = p[m].y - p[i].y;

                        double rx = tx * co + ty * si;
                        double ry = -tx * si + ty * co;

                        if (dir == -1) ry = -ry;

                        if (ry < -eps || ry > h + eps) continue;

                        double s_start, s_end;

                        if (h < eps)
                        {
                            if (abs(ry) > eps) continue;
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

                        double real_start = max(s_start, min_s - eps);
                        double real_end = min(s_end, max_s + eps);

                        if (real_start <= real_end + eps)
                        {
                            ev.push_back({real_start, -1});
                            ev.push_back({real_end, 1});
                        }
                    }

                    sort(ev.begin(), ev.end());

                    int cur = 0;
                    for (auto &e : ev)
                    {
                        if (e.second == -1) cur++;
                        else { if (cur > ans) ans = cur; cur--; }
                    }
                    if (cur > ans) ans = cur;
                }
            }
        }
    }

    cout << ans << endl;

    return 0;
}
