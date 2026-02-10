// https://acm.timus.ru/problem.aspx?space=1&num=1541
// egyptian fractions + rational arithmetic (fractions / GCD) + backtracking / DFS + pruning / branch-and-bound + greedy bounds (ceil for next denominator)

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define pb push_back
#define ppb pop_back
#define fi first
#define se second

static const int MAXD = 100000;

vector<int> cur;

bool dfs(int depth, int start, ll a, ll b)
{
    if (a == 0) return false;
    if (start > MAXD) return false;
    if ((__int128)start * a > (__int128)depth * b) return false;

    if (depth == 1)
    {
        if (b % a) return false;
        ll d = b / a;
        if (d < start || d > MAXD) return false;
        cur.pb((int)d);
        return true;
    }

    ll d0 = max<ll>(start, (b + a - 1) / a);
    ll dmax = min<ll>(MAXD, (ll)(((__int128)depth * b) / a));

    for (ll d = d0; d <= dmax; ++d)
    {
        if ((__int128)a * d <= b) continue;

        __int128 na = (__int128)a * d - b;
        __int128 nb = (__int128)b * d;

        if (nb > (__int128)9000000000000000000LL) break;

        ll na_ll = (ll)na;
        ll nb_ll = (ll)nb;

        ll g = std::gcd(na_ll, nb_ll);
        na_ll /= g;
        nb_ll /= g;

        cur.pb((int)d);
        if (dfs(depth - 1, (int)d + 1, na_ll, nb_ll)) return true;
        cur.ppb();
    }

    return false;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    ll m, n;
    cin >> m >> n;

    for (int k = 1; k <= 19; ++k)
    {
        cur.clear();
        if (dfs(k, 1, m, n))
        {
            cout << k << '\n';
            for (int i = k - 1; i >= 0; --i)
            {
                if (i != k - 1) cout << ' ';
                cout << cur[i];
            }
            cout << '\n';
            return 0;
        }
    }

    cout << -1 << '\n';
    return 0;
}
