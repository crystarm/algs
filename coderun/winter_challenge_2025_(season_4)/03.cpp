#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

long long calc(long long level, const vector<long long> &w)
{
    long long s = 0;
    int n = (int) w.size();
    for (int i = 0; i < n; ++i) { if (w[i] >= level) s += level; else s += w[i]; }
    return s;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long m;
    int n;
    if (!(cin >> m >> n)) return 0;

    vector<long long> w(n);
    long long sum = 0;
    long long maxw = 0;

    for (int i = 0; i < n; ++i)
    {
        cin >> w[i];
        sum += w[i];
        if (w[i] > maxw) maxw = w[i];
    }

    long long s = sum - m;
	long long lo = 0;
    long long hi = maxw;
    long long L = 0;

    while (lo <= hi)
    {
        long long mid = (lo + hi) / 2;
        if (calc(mid, w) <= s) { L = mid; lo = mid + 1; }
        else { hi = mid - 1; }
    }

    long long used = calc(L, w);
    long long extra = s - used;

    long long ans = 0;

    for (int i = 0; i < n; ++i)
    {
        long long d;
        if (w[i] <= L) d = w[i];
        else
        {
            if (w[i] >= L + 1 && extra > 0) { d = L + 1; --extra; }
            else d = L;
        }

        long long t = d % MOD;
        ans = (ans + t * t) % MOD;
    }

    cout << ans << '\n';
    return 0;
}
