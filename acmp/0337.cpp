// https://acmp.ru/index.asp?main=task&id_task=337
// modified inclusion-exclusion principle + a bit of number theory + LCM value states compression

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

typedef long long ll;

template <typename t_iter>

void mysort(t_iter L, t_iter R)
{
    if (R <= L) return;
    auto M = *(L + distance(L, R) / 2);
    t_iter i = L, j = R;

    do
    {
        while (M > *i) i++;
        while (*j > M) j--;
        if (i >= j) break;
        swap(*i, *j);
        i++; j--;

    } while (i <= j);

    mysort(L, j);
    mysort(j + 1, R);
}

ll gcd(ll a, ll b) { while (b) { a %= b; swap(a, b); } return a; }

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    ll n;
    int k;
    if (!(cin >> n >> k)) return 0;

    map<int, int> m;
    for (int i = 0; i < k; ++i) { int x; cin >> x; m[x]++; }

    vector<int> v;
    for (auto const& [val, cnt] : m) if (cnt % 2 != 0) v.push_back(val);

    if (!v.empty()) mysort(v.rbegin(), v.rend() - 1);

    vector<pair<ll, ll>> s;
    s.reserve(100000);

    for (int x : v)
    {
        vector<pair<ll, ll>> tmp;

        if (x <= n) tmp.push_back({(ll)x, 1});
        for (const auto& p : s)
        {
            ll l = p.first;
            ll c = p.second;
            ll g = gcd(l, x);
            ll mult = x / g;

            if (l > n / mult) continue;

            tmp.push_back({l * mult, c * -2});
        }

        s.insert(s.end(), tmp.begin(), tmp.end());

        if (!s.empty()) mysort(s.begin(), s.end() - 1);

        vector<pair<ll, ll>> next_s;
        next_s.reserve(s.size());

        for (const auto& p : s)
        {
            if (!next_s.empty() && next_s.back().first == p.first) next_s.back().second += p.second;
            else next_s.push_back(p);
        }

        s.clear();
        for (const auto& p : next_s) if(p.second != 0) s.push_back(p);
    }

    ll ans = 0;
    for (const auto& p : s) ans += (n / p.first) * p.second;

    cout << ans << endl;

    return 0;
}
