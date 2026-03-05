// https://acm.timus.ru/problem.aspx?space=1&num=2223
// bitwise operations + carry propagation + XOR/AND adder (Ripple-Carry) + recurrence/termination analysis

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef string str;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)((x).size())
#define rep(i,a,b) for (int i = (a); i < (b); i++)
#define pb push_back

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    str x, y;
    cin >> x >> y;

    bool z = 1;
    for (char c : y) if (c == '1') { z = 0; break; }
    if (z)
    {
        cout << 1 << '\n';
        return 0;
    }

    int n = max(sz(x), sz(y));
    if (sz(x) < n) x = str(n - sz(x), '0') + x;
    if (sz(y) < n) y = str(n - sz(y), '0') + y;

    int r = 0;
    int mx = 1;

    rep(i,0,n)
    {
        if (x[i] == '1' && y[i] == '1') mx = max(mx, r + 2);
        if (x[i] != y[i]) r++;
        else r = 0;
    }

    cout << (ll)mx + 1 << '\n';
    return 0;
}
