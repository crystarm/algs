// https://acmp.ru/index.asp?main=task&id_task=441
// hash maps + multisets + Fenwick tree

#include <bits/stdc++.h>
#include <vector>
using namespace std;

typedef vector<int> vi;
typedef long long ll;
typedef vector<long long> vll;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr(v,n) (v).reserve(n)

struct fwk
{
    int n; vll t;

    fwk(int n_ = 0) { init(n_);}

    void init(int n_) { n = n_; t.assign(n + 1, 0); }

    void add(int i, ll x) { for (; i <= n; i += i & -i) t[i] += x; }

    ll sum(int i)
    {
        ll r = 0;
        for (; i > 0; i -= i & -i) r += t[i];
        return r;
    }
};

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;

    vi aa(n), bb(n);
    rep(i,0,n) cin >> aa[i];
    rep(i,0,n) cin >> bb[i];

    unordered_map<int, int> cc;
    rsr(cc, n * 2);

    for (int x : aa) cc[x]++;
    for (int x : bb) cc[x]--;

    for (auto [k, v] : cc)
    { if (v != 0) { cout << -1 << '\n'; return 0; } }

    unordered_map<int, vi> ps;
    rsr(ps, n * 2);

    rep(i,0,n) ps[bb[i]].push_back(i + 1);

    unordered_map<int, int> pt;
    rsr(pt, n * 2);

    vi pp(n);
    rep(i,0,n) { pp[i] = ps[aa[i]][pt[aa[i]]++]; }

    fwk fw(n);
    ll ans = 0;

    for (int i = n - 1; i >= 0; --i)
    {
        ans += fw.sum(pp[i] - 1);
        fw.add(pp[i], 1);
    }

    cout << ans << '\n';
    return 0;
}
