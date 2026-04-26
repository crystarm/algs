// https://acmp.ru/index.asp?main=task&id_task=618
// integer partitions + Young diagrams + DP + branch&bound + prefix sums

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int N = 100;

int n;
ll p[N + 1][N + 1];
ll c[N + 2][N + 1];
ll s[N + 2][N + 2];

int v[N + 1];
int bv[N + 1];
int vc;
int bvc;
ll ba;

ll ub(int r, int l, int d)
{
    ll z = 0;

    for (int i = 0; i <= l; ++i) z += c[d][i] * p[r][i];

    return z - 1;
}

void go(int r, int l, int d)
{
    ll z = ub(r, l, d);

    if (z <= ba) return;

    if (r == 0)
    {
        ba = z;
        bvc = vc;

        for (int i = 0; i < vc; ++i) bv[i] = v[i];

        return;
    }

    s[d][l + 1] = 0;

    for (int i = l; i >= 0; --i) s[d][i] = s[d][i + 1] + c[d][i];

    int m = min(l, r);

    for (int x = m; x >= 1; --x)
    {
        ll z2 = 0;

        for (int y = 0; y <= x; ++y)
        {
            c[d + 1][y] = s[d][y];
            z2 += c[d + 1][y] * p[r - x][y];
        }

        if (z2 - 1 <= ba)
            continue;

        v[vc++] = x;
        go(r - x, x, d + 1);
        --vc;
    }
}

int main()
{


    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n;

    ll e[N + 1][N + 1] = {};
    ll dp[N + 1] = {};

    dp[0] = 1;

    for (int m = 0; m <= n; ++m) e[0][m] = 1;

    for (int m = 1; m <= n; ++m)
    {
        for (int i = m; i <= n; ++i) dp[i] += dp[i - m];
        for (int i = 0; i <= n; ++i) e[i][m] = dp[i];
    }

    for (int m = 0; m <= n; ++m)
    {
        ll z = 0;

        for (int i = 0; i <= n; ++i)
        {
            z += e[i][m];
            p[i][m] = z;
        }
    }

    ba = 0;
    bvc = 0;

    for (int x = n; x >= 1; --x)
    {
        for (int i = 0; i <= x; ++i) c[1][i] = 1;

        vc = 1;
        v[0] = x;

        go(n - x, x, 1);
    }

    cout << ba << '\n';

    for (int i = 0; i < bvc; ++i)
    {
        if (i) cout << ' ';
        cout << bv[i];
    }

    cout << '\n';

    return 0;
}
