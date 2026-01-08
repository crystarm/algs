// https://acm.timus.ru/problem.aspx?space=1&num=2188
// prefix sums + backward DP for the minimum safe lead + monotonicity + binary search over answers (per query)

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<ll> vll;

#define rep(i,a,b) for (int i = (a); i <= (b); ++i)

int main()
{

    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, q;
    cin >> n >> q;

    vll a(n + 1), b(n + 1), g(n + 1);
    rep(i, 1, n) cin >> a[i];
    rep(i, 1, n) cin >> b[i];

    rep(i, 1, n) g[i] = a[i] - b[i];

    vll C(n + 1, 0), M(n + 1, 0);
    rep(i, 1, n)
    {
        C[i] = C[i - 1] + g[i];
        M[i] = max(M[i - 1], C[i]);
    }

    vll need(n + 1, 1);
    need[n] = 1;
    for (int i = n - 1; i >= 0; --i) need[i] = max(1LL, need[i + 1] - g[i + 1]);


    vll best(n + 1, 0);
    rep(t, 1, n)
    {
        ll T = C[t] - need[t];
        ll dmax = (T >= M[t - 1] ? T : -1LL);
        best[t] = max(best[t - 1], dmax);
    }

    while (q--)
    {
        ll D;
        cin >> D;
        auto it = lower_bound(best.begin() + 1, best.end(), D);
        if (it == best.end()) cout << -1 << '\n';
        else cout << (int)(it - best.begin()) << '\n';
    }

    return 0;
}
