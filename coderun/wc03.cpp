// https://coderun.yandex.ru/selections/2025-winter-common/problems/equipment
// water level problem

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<ll> vll;

#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

const ll MOD = 1000000007LL;

static inline ll calc(ll level, const vll& a)
{
    ll sum = 0;
    int n = sz(a);
    rep(i, 0, n) sum += (a[i] >= level ? level : a[i]);
    return sum;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    ll m;
    int n;
    if (!(cin >> m >> n)) return 0;

    vll a(n);
    ll sum = 0;
    ll mx = 0;

    rep(i, 0, n)
    {
        cin >> a[i];
        sum += a[i];
        mx = max(mx, a[i]);
    }

    ll need = sum - m;

    ll lo = 0, hi = mx, lvl = 0;
    while (lo <= hi)
    {
        ll mid = (lo + hi) / 2;
        if (calc(mid, a) <= need) { lvl = mid; lo = mid + 1; }
        else hi = mid - 1;
    }

    ll used = calc(lvl, a);
    ll extra = need - used;

    ll ans = 0;
    rep(i, 0, n)
    {
        ll cur;
        if (a[i] <= lvl) cur = a[i];
        else
        {
            if (extra > 0) { cur = lvl + 1; --extra; }
            else cur = lvl;
        }

        ll t = cur % MOD;
        ans = (ans + t * t) % MOD;
    }

    cout << ans << '\n';
    return 0;
}
