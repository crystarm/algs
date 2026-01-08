// https://acm.timus.ru/problem.aspx?space=1&num=1618
// aliasing (reference identity) + parsing deepToString brackets + equivalence relations / partitions + stirling numbers (2nd kind) + DP over levels + modular arithmetic

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef vector<vector<int>> vvi;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i <= (b); ++i)
#define pb push_back

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    const int MOD = 10007;
    string s;
    getline(cin, s);

    vi st;
    vi len(2, -1);
    int maxd = 0;

    for (char c : s)
    {
        if (c == '[')
        {
            st.pb(0);
            maxd = max(maxd, sz(st));
        }
        else if (c == ']')
        {
            int cnt = st.back();
            st.pop_back();
            int depth = sz(st) + 1;
            if (sz(len) <= depth) len.resize(depth + 1, -1);
            if (len[depth] == -1) len[depth] = cnt;
            if (!st.empty()) st.back()++;
        }
    }

    int D = maxd;
    vi a;
    rep(d, 1, D - 1) a.pb(len[d]);

    int N = 512;
    vvi S(N + 1, vi(N + 1, 0));
    S[0][0] = 1;
    rep(n, 1, N) rep(k, 1, n) S[n][k] = (S[n - 1][k - 1] + (ll)k * S[n - 1][k]) % MOD;


    vi dp(N + 1, 0), ndp(N + 1, 0);
    dp[1] = 1;

    for (int x : a)
    {
        fill(all(ndp), 0);
        rep(b, 1, N)
        {
            if (!dp[b]) continue;
            int q = b * x;
            rep(c, 1, q) ndp[c] = (ndp[c] + (ll)dp[b] * S[q][c]) % MOD;

        }
        dp.swap(ndp);
    }

    int ans = 0;
    rep(b, 1, N) ans = (ans + dp[b]) % MOD;
    cout << ans % MOD << '\n';
    return 0;
}
