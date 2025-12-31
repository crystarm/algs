// https://acmp.ru/index.asp?main=task&id_task=880
// minimum set cover

#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
typedef vector<int> vi;
typedef string str;

#define all(x) (x).begin(), (x).end()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr(v,n) (v).reserve(n)
#define pb push_back

const int INF = 1e9;

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int m, n;
    if (!(cin >> m >> n)) return 0;

    vector<pii> p;
    rsr(p, 9);
    rep(r, 0, 3)
    {
        str s;
        cin >> s;
        rep(c, 0, 3) if (s[c] == 'X') p.pb({r, c});
    }

    int M = 1 << m;
    int FL = M - 1;
    int K = m + 2;
    int L = 1 << K;

    vi f(M * M, INF), g(M * M);
    f[0] = 0;

    vi u0(L), u1(L), u2(L), w(L);
    vi b0(K), b1(K), b2(K);
    vi z(M);

    rep(y, -2, n)
    {
        rep(i, 0, K)
        {
            int ox = i - 2;
            int m0 = 0, m1 = 0, m2 = 0;
            for (auto [dr, dc] : p)
            {
                int cy = y + dr;
                int cx = ox + dc;
                if (cx >= 0 && cx < m && cy >= 0 && cy < n)
                {
                    if (dr == 0) m0 |= (1 << cx);
                    else if (dr == 1) m1 |= (1 << cx);
                    else m2 |= (1 << cx);
                }
            }
            b0[i] = m0;
            b1[i] = m1;
            b2[i] = m2;
        }

        u0[0] = u1[0] = u2[0] = w[0] = 0;
        rep(s, 1, L)
        {
            int bit = s & -s;
            int prev = s ^ bit;
            int idx = __builtin_ctz(bit);
            u0[s] = u0[prev] | b0[idx];
            u1[s] = u1[prev] | b1[idx];
            u2[s] = u2[prev] | b2[idx];
            w[s] = w[prev] + 1;
        }

        bool need = (y >= 0 && y < n);
        bool kp1 = (y + 1 >= 0 && y + 1 < n);
        bool kp2 = (y + 2 >= 0 && y + 2 < n);

        fill(all(g), INF);

        rep(mk1, 0, M)
        {
            int base = mk1 * M;
            bool ok = false;
            rep(mk0, 0, M)
            {
                z[mk0] = f[base + mk0];
                if (z[mk0] < INF) ok = true;
            }
            if (!ok) continue;

            rep(i, 0, m)
                rep(mask, 0, M)
                    if (!(mask & (1 << i))) z[mask] = min(z[mask], z[mask | (1 << i)]);

            rep(s, 0, L)
            {
                int req = need ? (FL & ~u0[s]) : 0;
                int val = z[req];
                if (val >= INF) continue;

                int n_mk0 = kp1 ? (mk1 | u1[s]) : 0;
                int n_mk1 = kp2 ? u2[s] : 0;

                int& cell = g[n_mk1 * M + n_mk0];
                int cand = val + w[s];
                if (cand < cell) cell = cand;
            }
        } f.swap(g);
    }

    int ans = INF;
    for (int x : f) ans = min(ans, x);

    cout << ans << '\n';
    return 0;
}
