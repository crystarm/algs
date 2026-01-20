// https://acm.timus.ru/problem.aspx?space=1&num=1598
// DSA + finite fields + modular arithmetic + fast modular exponentiation + BSGS+ crypto break via small parameters (recover x -> forge signatures)

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)((x).size())
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr(v,n) (v).reserve(n)
#define mlf max_load_factor

static inline ull mulm(ull a, ull b, ull mod) { return (ull)((__uint128_t)a * b % mod); }

static inline ull powm(ull a, ull e, ull mod)
{
    ull r = 1 % mod;
    a %= mod;
    while (e)
    {
        if (e & 1) r = mulm(r, a, mod);
        a = mulm(a, a, mod);
        e >>= 1;
    }
    return r;
}

static inline ull invm(ull a, ull mod) { return powm(a, mod - 2, mod); }

static ull bsgs(ull g, ull y, ull p, ull q)
{
    ull m = (ull)(sqrt((long double)q) + 1);
    while (m * m < q) ++m;

    unordered_map<ull, int> tab;
    rsr(tab, (size_t)(m * 2 + 7));
    tab.mlf(0.7f);

    ull cur = 1;
    for (ull j = 0; j < m; ++j)
    {
        if (!tab.count(cur)) tab[cur] = (int)j;
        cur = mulm(cur, g, p);
    }

    ull step = powm(g, m, p);
    ull invs = invm(step, p);

    cur = y % p;
    for (ull i = 0; i <= m; ++i)
    {
        auto it = tab.find(cur);
        if (it != tab.end())
        {
            ull x = (i * m + (ull)it->second) % q;
            return x;
        }
        cur = mulm(cur, invs, p);
    }

    return 0;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    ull n, l;
    ull q, p, g, y, h;
    cin >> n >> l >> q >> p >> g >> y >> h;

    ull x = bsgs(g, y, p, q);
    ull hm = h % q;

    for (ull k = 1; k < q; ++k)
    {
        ull r = powm(g, k, p) % q;
        if (!r) continue;

        ull ki = invm(k % q, q);
        ull t0 = mulm(x % q, r, q);
        ull s = mulm(ki, (hm + t0) % q, q);
        if (!s) continue;

        cout << r << " " << s << '\n';
        return 0;
    }

    return 0;
}
