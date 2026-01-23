// https://coderun.yandex.ru/problem/venue
// сomputational geometry + perpendicular bisectors + parity/invariants on integer lattice+ randomized/heuristic pair search (Monte Carlo)

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<ll> vll;
typedef vector<int> vi;

#define rep(i,a,b) for(int i=(a); i<(b); i++)

static inline ll absll(ll x) { return x < 0 ? -x : x; }

void solve()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;

    vll x(n), y(n);
    rep(i,0,n) cin >> x[i] >> y[i];

    int i0 = -1, i1 = -1;
    rep(i,0,n)
    {
        int p = (int)((x[i] + y[i]) & 1LL);
        if(p == 0 && i0 == -1) i0 = i;
        if(p == 1 && i1 == -1) i1 = i;
    }

    if(i0 != -1 && i1 != -1)
    {
        cout << i0 + 1 << " " << i1 + 1 << "\n";
        return;
    }

    vll n2(n);
    rep(i,0,n)
    {
        __int128 v = (__int128)x[i] * x[i] + (__int128)y[i] * y[i];
        n2[i] = (ll)v;
    }

    auto chk_q = [&](int a, int b) -> bool
    {
        ll dx = x[b] - x[a];
        ll dy = y[b] - y[a];
        ll g = gcd(absll(dx), absll(dy));
        ll m = 2 * g;
        ll rhs = n2[b] - n2[a];
        ll r = rhs % m;
        if(r < 0) r += m;
        return r != 0;
    };

    auto chk_f = [&](int a, int b) -> bool
    {
        ll dx = x[b] - x[a];
        ll dy = y[b] - y[a];
        __int128 rhs = (__int128)n2[b] - (__int128)n2[a];
        rep(i,0,n)
        {
            if(i == a || i == b) continue;
            __int128 lhs = (__int128)2 * ((__int128)x[i] * dx + (__int128)y[i] * dy);
            if(lhs == rhs) return false;
        }
        return true;
    };

    vi cnd;

    auto add = [&](int v)
    {
        for(int u : cnd) if(u == v) return;
        cnd.push_back(v);
    };

    int mn_x = 0, mx_x = 0, mn_y = 0, mx_y = 0;
    int mn_s = 0, mx_s = 0, mn_d = 0, mx_d = 0;

    rep(i,1,n)
    {
        if(x[i] < x[mn_x] || (x[i] == x[mn_x] && y[i] < y[mn_x])) mn_x = i;
        if(x[i] > x[mx_x] || (x[i] == x[mx_x] && y[i] > y[mx_x])) mx_x = i;

        if(y[i] < y[mn_y] || (y[i] == y[mn_y] && x[i] < x[mn_y])) mn_y = i;
        if(y[i] > y[mx_y] || (y[i] == y[mx_y] && x[i] > x[mx_y])) mx_y = i;

        ll s = x[i] + y[i], s0 = x[mn_s] + y[mn_s], s1 = x[mx_s] + y[mx_s];
        if(s < s0 || (s == s0 && x[i] < x[mn_s])) mn_s = i;
        if(s > s1 || (s == s1 && x[i] > x[mx_s])) mx_s = i;

        ll d = x[i] - y[i], d0 = x[mn_d] - y[mn_d], d1 = x[mx_d] - y[mx_d];
        if(d < d0 || (d == d0 && x[i] < x[mn_d])) mn_d = i;
        if(d > d1 || (d == d1 && x[i] > x[mx_d])) mx_d = i;
    }

    add(mn_x); add(mx_x);
    add(mn_y); add(mx_y);
    add(mn_s); add(mx_s);
    add(mn_d); add(mx_d);

    int cs = (int)cnd.size();
    rep(i,0,cs)
    {
        rep(j,i+1,cs)
        {
            int a = cnd[i], b = cnd[j];
            if(a == b) continue;
            if(chk_q(a, b) || chk_f(a, b)) { cout << a + 1 << " " << b + 1 << "\n"; return; }
        }
    }

    mt19937_64 r(chrono::high_resolution_clock::now().time_since_epoch().count());

    const int TRIES_Q = 800;
    rep(t,0,TRIES_Q)
    {
        int a = (int)(r() % n); int b = (int)(r() % n);
        if(a == b) continue;
        if(chk_q(a, b))
        { cout << a + 1 << " " << b + 1 << "\n"; return; }
    }

    const int TRIES_F = 70;
    rep(t,0,TRIES_F)
    {
        int a = (int)(r() % n); int b = (int)(r() % n);
        if(a == b) continue;
        if(chk_f(a, b))
        { cout << a + 1 << " " << b + 1 << "\n"; return; }
    }

    cout << "0 0\n";
}

int main() { solve(); return 0; }
