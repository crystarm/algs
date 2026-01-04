// https://coderun.yandex.ru/problem/security
// sweep line technique + prefix sums

#include <bits/stdc++.h>
using namespace std;

#define rep(i,a,b) for (int i = (a); i < (b); ++i)

const int MAXT = 10000;

struct g { int l, r; };

int diff_[MAXT + 5];
int cov[MAXT + 5];
int pref[MAXT + 5];

void solve()
{
    int n;
    cin >> n;

    vector<g> a(n);
    memset(diff_, 0, sizeof(diff_));

    rep(i, 0, n)
    {
        cin >> a[i].l >> a[i].r;
        if (a[i].l < a[i].r)
        {
            int l = a[i].l + 1;
            int r = a[i].r;
            if (l <= MAXT) diff_[l]++;
            if (r + 1 <= MAXT) diff_[r + 1]--;
        }
    }

    int cur = 0;
    bool ok = true;

    rep(t, 1, MAXT + 1)
    {
        cur += diff_[t];
        cov[t] = cur;
        if (cur == 0) ok = false;
    }

    if (!ok)
    {
        cout << "Wrong Answer\n";
        return;
    }

    pref[0] = 0;
    rep(t, 1, MAXT + 1) pref[t] = pref[t - 1] + (cov[t] == 1);

    rep(i, 0, n)
    {
        int l = a[i].l;
        int r = a[i].r;

        if (l >= r)
        {
            cout << "Wrong Answer\n";
            return;
        }

        int essential = pref[r] - pref[l];
        if (essential == 0)
        {
            cout << "Wrong Answer\n";
            return;
        }
    }

    cout << "Accepted\n";
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int k;
    if (!(cin >> k)) return 0;
    while (k--) solve();
    return 0;
}
