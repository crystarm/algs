// https://acm.timus.ru/problem.aspx?space=1&num=2097
// affine transform (a * x + b) + second differences + KMP + linear invariants

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef vector<ll> vll;

#define rep(i,a,b) for (int i = (a); i < (b); ++i)

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    const ll LINF = (ll)4e18;

    int n;
    cin >> n;

    int m;
    cin >> m;

    vll x(m + 1);
    rep(i, 0, m + 1) cin >> x[i];

    ll mn = LINF;
    int ri = -1, rj = -1;

    if (m == 1)
    {
        ll d0 = x[1] - x[0];

        rep(i, 1, n + 1)
        {
            int k;
            cin >> k;

            vll y(k + 1);
            rep(t, 0, k + 1) cin >> y[t];

            if (k < 1) continue;

            rep(j, 0, k)
            {
                ll d1 = y[j + 1] - y[j];
                ll a = d0 - d1;
                ll v = llabs(a);

                if (v < mn) { mn = v; ri = i; rj = j; }
            }
        }

        if (ri == -1) cout << -1 << '\n';
        else cout << ri << ' ' << rj << '\n';
        return 0;
    }

    int L = m - 1;

    vll p(L);
    rep(i, 2, m + 1) p[i - 2] = x[i] - 2 * x[i - 1] + x[i - 2];

    vi pi(L, 0);
    rep(i, 1, L)
    {
        int j = pi[i - 1];
        while (j > 0 && p[i] != p[j]) j = pi[j - 1];
        if (p[i] == p[j]) ++j;
        pi[i] = j;
    }

    ll d0 = x[1] - x[0];

    rep(i, 1, n + 1)
    {
        int k;
        cin >> k;

        vll y(k + 1);
        rep(t, 0, k + 1) cin >> y[t];

        if (k < m) continue;

        int st = 0;

        rep(t, 0, k - 1)
        {
            ll q = y[t + 2] - 2 * y[t + 1] + y[t];

            while (st > 0 && q != p[st]) st = pi[st - 1];
            if (q == p[st]) ++st;

            if (st == L)
            {
                int j0 = t - L + 1;

                ll d1 = y[j0 + 1] - y[j0];
                ll a = d0 - d1;
                ll v = llabs(a);

                if (v < mn) { mn = v; ri = i; rj = j0; }

                st = pi[st - 1];


            }
        }
    }

    if (ri == -1) cout << -1 << '\n';
    else cout << ri << ' ' << rj << '\n';

    return 0;
}
