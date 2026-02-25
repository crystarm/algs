// https://acm.timus.ru/problem.aspx?space=1&num=1041
// linear algebra + matroid greedy + Gauss elimination + sorting + implementation details

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef vector<int> vi;

typedef vector<vi> vvi;
typedef vector<char> vc;

typedef long double ld;
typedef vector<ld> vd;
typedef vector<vd> vvd;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr(v,n) (v).reserve(n)
#define pb push_back

struct gs
{
    int n;
    vvd b;
    vc h;

    gs()
    {
    }

    gs(int _n)
    {
        n = _n;
        b.assign(n, vd(n, 0));
        h.assign(n, 0);
    }

    bool add(const vi &v)
    {
        const ld eps = 1e-12L;

        vd x(n, 0);
        rep(i, 0, n)
        {
            x[i] = (ld)v[i];
        }

        rep(c, 0, n)
        {
            if (fabsl(x[c]) <= eps)
            {
                x[c] = 0;
                continue;
            }

            if (!h[c])
            {
                ld inv = 1.0L / x[c];
                rep(j, c, n)
                {
                    x[j] *= inv;
                    if (fabsl(x[j]) <= eps)
                    {
                        x[j] = 0;
                    }
                }
                b[c] = x;
                h[c] = 1;
                return 1;
            }

            ld k = x[c];
            rep(j, c, n)
            {
                x[j] -= k * b[c][j];
                if (fabsl(x[j]) <= eps)
                {
                    x[j] = 0;
                }
            }
        }

        return 0;
    }
};

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int m, n;
    cin >> m >> n;

    vvi a(m, vi(n));
    rep(i, 0, m)
    {
        rep(j, 0, n)
        {
            cin >> a[i][j];
        }
    }

    vi c(m);
    rep(i, 0, m)
    {
        cin >> c[i];
    }

    vi ord(m);
    iota(all(ord), 0);

    sort(all(ord), [&](int i, int j)
    {
        if (c[i] != c[j])
        {
            return c[i] < c[j];
        }
        return i < j;
    });

    gs g(n);
    vi ans;
    rsr(ans, n);
    ll s = 0;

    for (int id : ord)
    {
        if (g.add(a[id]))
        {
            ans.pb(id + 1);
            s += c[id];
            if (sz(ans) == n)
            {
                break;
            }
        }
    }

    if (sz(ans) < n)
    {
        cout << 0 << '\n';
        return 0;
    }

    sort(all(ans));
    cout << s << '\n';
    for (int x : ans)
    {
        cout << x << '\n';
    }

    return 0;
}
