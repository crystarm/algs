// https://acm.timus.ru/problem.aspx?space=1&num=1704
// DSP + correlation detector + FSK decoding + bit operations + precompute trig

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;
typedef string str;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define pb push_back
#define fi first
#define se second

const int INF = 1e9;
const ll LINF = (ll)4e18;

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, t;
    if (!(cin >> n >> t)) return 0;

    vector<ld> a(n);
    rep(i, 0, n)
    {
        double x;
        cin >> x;
        a[i] = (ld)x;
    }

    int blk = 8 * t;
    int m = (blk ? (n / blk) : 0);

    const ld PI = acosl(-1.0L);
    vector<ld> w1(t), w0(t);

    if (t > 0)
    {
        ld step = 2.0L * PI / (ld)t;
        rep(k, 0, t)
        {
            ld ang = step * (ld)k;
            w1[k] = sinl(ang);
            w0[k] = sinl(2.0L * ang);
        }
    }

    int idx = 0;
    str out;
    out.reserve(m);

    rep(ci, 0, m)
    {
        int code = 0;
        rep(bit, 0, 8)
        {
            ld s1 = 0, s0 = 0;
            rep(k, 0, t)
            {
                ld v = a[idx++];
                s1 += v * w1[k];
                s0 += v * w0[k];
            }
            if (s1 > s0) code |= (1 << bit);
        }
        out.pb((char)code);
    }

    cout << out;
    return 0;
}
