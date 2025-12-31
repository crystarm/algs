// https://acmp.ru/index.asp?main=task&id_task=337
// modified inclusion-exclusion principle + a bit of number theory + LCM value states compression

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr(v,n) (v).reserve(n)
#define pb push_back
#define fi first
#define se second

template <typename it>
void mysort(it L, it R)
{
    if (R <= L) return;
    auto M = *(L + distance(L, R) / 2);
    it i = L, j = R;
    do
    {
        while (M > *i) i++;
        while (*j > M) j--;
        if (i >= j) break;
        swap(*i, *j);
        i++;
        j--;
    } while (i <= j);
    mysort(L, j);
    mysort(j + 1, R);
}

ll gcd(ll a, ll b) { while (b) { a %= b; swap(a, b); } return a; }

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    ll n;
    int k;
    if (!(cin >> n >> k)) return 0;

    map<int, int> cnt;
    rep(i, 0, k) { int x; cin >> x; cnt[x]++; }

    vector<int> a;
    for (auto const& it : cnt) if (it.se & 1) a.pb(it.fi);

    if (!a.empty()) mysort(a.rbegin(), a.rend() - 1);

    vector<pair<ll, ll>> cur;
    rsr(cur, 100000);

    for (int x : a)
    {
        vector<pair<ll, ll>> tmp;

        if ((ll)x <= n) tmp.pb({(ll)x, 1});

        for (const auto& p : cur)
        {
            ll l = p.fi;
            ll c = p.se;

            ll g = gcd(l, (ll)x);
            ll mult = (ll)x / g;

            if (l > n / mult) continue;

            tmp.pb({l * mult, c * -2});
        }

        cur.insert(cur.end(), all(tmp));

        if (!cur.empty()) mysort(cur.begin(), cur.end() - 1);

        vector<pair<ll, ll>> nxt;
        rsr(nxt, sz(cur));

        for (const auto& p : cur)
        {
            if (!nxt.empty() && nxt.back().fi == p.fi) nxt.back().se += p.se;
            else nxt.pb(p);
        }

        cur.clear();
        for (const auto& p : nxt) if (p.se != 0) cur.pb(p);
    }

    ll ans = 0;
    for (const auto& p : cur) ans += (n / p.fi) * p.se;

    cout << ans << '\n';
    return 0;
}
