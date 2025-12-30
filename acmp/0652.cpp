// https://acmp.ru/index.asp?main=task&id_task=652
// range queries + sparse representation + interval splitting

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define pb push_back

struct node { ll len, val; int d; };

typedef vector<node> vn;

vn v;

int split(ll pos)
{
    ll cur = 1;
    rep(i, 0, sz(v))
    {
        if (cur == pos) return i;
        if (cur + v[i].len > pos)
        {
            ll l = pos - cur;
            ll r = v[i].len - l;

            node nxt = {r, v[i].val + l * v[i].d, v[i].d};
            v[i].len = l;

            v.insert(v.begin() + i + 1, nxt);
            return i + 1;
        }
        cur += v[i].len;
    }
    return sz(v);
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    ll n;
    int k;
    if (!(cin >> n >> k)) return 0;

    v.pb({n, 1, 1});

    while (k--)
    {
        char op;
        ll l, r;
        cin >> op >> l >> r;

        int p1 = split(l);
        int p2 = split(r + 1);

        if (op == 'I')
        {
            reverse(v.begin() + p1, v.begin() + p2);
            rep(i, p1, p2)
            {
                v[i].val += (v[i].len - 1) * v[i].d;
                v[i].d = -v[i].d;
            }
        }
        else
        {
            ll ans = 0;
            rep(i, p1, p2) ans += v[i].len * (2 * v[i].val + (v[i].len - 1) * v[i].d) / 2;
            cout << ans << '\n';
        }
    }

    return 0;
}
