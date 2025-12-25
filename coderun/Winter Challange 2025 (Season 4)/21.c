#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define MAXN 12
#define MAX_CAND 400
#define EPS_COVER 1e-9
#define EPS_DIST 1e-9

typedef struct { double x, y; } Point;

Point pts[MAXN];
Point cands[MAX_CAND];
int cands_mask[MAX_CAND];
int n, c_cnt;

bool conflict[MAX_CAND][MAX_CAND];

int res_idx[MAXN];
int res_n;
bool found;

inline double dist_sq(Point a, Point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

void add_cand(double x, double y) {
    if (c_cnt >= MAX_CAND) return;

    Point p = {x, y};
    int m = 0;

    double r_sq_limit = 1.0 + EPS_COVER;

    for (int i = 0; i < n; i++) {
        if (dist_sq(p, pts[i]) <= r_sq_limit) {
            m |= (1 << i);
        }
    }

    if (m == 0) return;

    for (int i = 0; i < c_cnt; i++) {
        if (cands_mask[i] == m && dist_sq(cands[i], p) < 1e-10) return;
    }

    cands[c_cnt] = p;
    cands_mask[c_cnt] = m;
    c_cnt++;
}

void backtrack(int current_mask, int count) {
    if (found) return;

    if (current_mask == (1 << n) - 1) {
        res_n = count;
        found = true;
        return;
    }

    int bit = __builtin_ctz(~current_mask);

    for (int i = 0; i < c_cnt; i++) {
        if (!((cands_mask[i] >> bit) & 1)) continue;

        bool ok = true;
        for (int j = 0; j < count; j++) {
            if (conflict[i][res_idx[j]]) {
                ok = false;
                break;
            }
        }

        if (ok) {
            res_idx[count] = i;
            backtrack(current_mask | cands_mask[i], count + 1);
            if (found) return;
        }
    }
}

void solve() {
    if (scanf("%d", &n) != 1) return;
    for (int i = 0; i < n; i++) {
        scanf("%lf %lf", &pts[i].x, &pts[i].y);
    }

    c_cnt = 0;
    found = false;

    for (int i = 0; i < n; i++) {
        add_cand(pts[i].x, pts[i].y);
        add_cand(floor(pts[i].x), floor(pts[i].y));
        add_cand(ceil(pts[i].x), floor(pts[i].y));
        add_cand(floor(pts[i].x), ceil(pts[i].y));
        add_cand(ceil(pts[i].x), ceil(pts[i].y));
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double d2 = dist_sq(pts[i], pts[j]);
            if (d2 > 4.0 + EPS_DIST) continue;

            add_cand((pts[i].x + pts[j].x) / 2.0, (pts[i].y + pts[j].y) / 2.0);

            double d = sqrt(d2);
            if (d > 1e-9) {
                double h_sq = 1.0 - d2 / 4.0;
                double h = (h_sq > 0) ? sqrt(h_sq) : 0.0;

                double mx = (pts[i].x + pts[j].x) / 2.0;
                double my = (pts[i].y + pts[j].y) / 2.0;

                double dx_h = h * (pts[i].y - pts[j].y) / d;
                double dy_h = h * (pts[j].x - pts[i].x) / d;

                add_cand(mx + dx_h, my + dy_h);
                add_cand(mx - dx_h, my - dy_h);
            }
        }
    }

    double dist_limit = 4.0 - 1e-9;

    for (int i = 0; i < c_cnt; i++) {
        for (int j = i; j < c_cnt; j++) {
            bool bad = (dist_sq(cands[i], cands[j]) < dist_limit);
            conflict[i][j] = bad;
            conflict[j][i] = bad;
        }
    }

    backtrack(0, 0);

    if (found) {
        printf("YES\n%d\n", res_n);
        for (int i = 0; i < res_n; i++) {
            printf("%.10f %.10f\n", cands[res_idx[i]].x, cands[res_idx[i]].y);
        }
    } else {
        printf("NO\n");
    }
}

int main() {
    int t;
    if (scanf("%d", &t) == 1) {
        while (t--) {
            solve();
        }
    }
    return 0;
}
