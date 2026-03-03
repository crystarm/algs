// https://acmp.ru/index.asp?main=task&id_task=617
// combinatorics + incl–excl principle + placement counting

#include <bits/stdc++.h>
using namespace std;

typedef string str;

typedef long long ll;
typedef unsigned long long ull;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)((x).size())
#define rep(i,a,b) for (int i=(a); i<(b); i++)
#define pb push_back
#define tstr to_string


struct bi
{
    static const uint32_t base = 1000000000;
    static const int bd = 9;

    vector<uint32_t> a;
    int s;

    bi() : s(0) {}

    bi(ll v)
    {
        *this = v;
    }

    bi& operator=(ll v)
    {
        a.clear();
        if (v == 0)
        {
            s = 0;
            return *this;
        }
        s = (v < 0 ? -1 : 1);
        ull x;
        if (v < 0)
        {
            ull t = (ull)(-(v + 1));
            x = t + 1;
        }
        else x = (ull)v;

        while (x)
        {
            a.pb((uint32_t)(x % base));
            x /= base;
        }
        tr();
        return *this;
    }

    void tr()
    {
        while (!a.empty() && a.back() == 0) a.pop_back();
        if (a.empty()) s = 0;
    }

    static int acmp(const bi &x, const bi &y)
    {
        if (sz(x.a) != sz(y.a)) return sz(x.a) < sz(y.a) ? -1 : 1;
        for (int i = (int)sz(x.a) - 1; i >= 0; i--)
        {
            if (x.a[i] != y.a[i]) return x.a[i] < y.a[i] ? -1 : 1;
        }
        return 0;
    }

    static bi aadd(const bi &x, const bi &y)
    {
        bi r;
        r.s = 1;
        uint64_t carry = 0;
        int n = max(sz(x.a), sz(y.a));
        r.a.resize(n);
        for (int i = 0; i < n; i++)
        {
            uint64_t cur = carry;
            if (i < sz(x.a)) cur += x.a[i];
            if (i < sz(y.a)) cur += y.a[i];
            r.a[i] = (uint32_t)(cur % base);
            carry = cur / base;
        }
        if (carry) r.a.pb((uint32_t)carry);
        r.tr();
        return r;
    }

    static bi asub(const bi &x, const bi &y)
    {
        bi r;
        r.s = 1;
        r.a.resize(sz(x.a));
        ll carry = 0;
        for (int i = 0; i < sz(x.a); i++)
        {
            ll cur = (ll)x.a[i] - carry - (i < sz(y.a) ? (ll)y.a[i] : 0LL);
            if (cur < 0)
            {
                cur += base;
                carry = 1;
            }
            else carry = 0;
            r.a[i] = (uint32_t)cur;
        }
        r.tr();
        return r;
    }

    friend bi operator+(const bi &x, const bi &y)
    {
        if (x.s == 0) return y;
        if (y.s == 0) return x;
        if (x.s == y.s)
        {
            bi r = aadd(x, y);
            r.s = x.s;
            r.tr();
            return r;
        }
        int c = acmp(x, y);
        if (c == 0) return bi(0);
        if (c > 0)
        {
            bi r = asub(x, y);
            r.s = x.s;
            r.tr();
            return r;
        }
        else
        {
            bi r = asub(y, x);
            r.s = y.s;
            r.tr();
            return r;
        }
    }

    friend bi operator-(const bi &x, const bi &y)
    {
        if (y.s == 0) return x;
        bi t = y;
        t.s = -t.s;
        return x + t;
    }

    friend bi operator*(const bi &x, const bi &y)
    {
        if (x.s == 0 || y.s == 0) return bi(0);
        bi r;
        r.s = x.s * y.s;
        r.a.assign(sz(x.a) + sz(y.a) + 1, 0);
        for (int i = 0; i < sz(x.a); i++)
        {
            uint64_t carry = 0;
            for (int j = 0; j < sz(y.a) || carry; j++)
            {
                uint64_t cur = r.a[i + j] + carry;
                if (j < sz(y.a)) cur += (uint64_t)x.a[i] * (uint64_t)y.a[j];
                r.a[i + j] = (uint32_t)(cur % base);
                carry = cur / base;
            }
        }
        r.tr();
        return r;
    }

    bi& operator+=(const bi &v)
    {
        *this = *this + v;
        return *this;
    }

    bi& operator-=(const bi &v)
    {
        *this = *this - v;
        return *this;
    }

    bi& operator*=(const bi &v)
    {
        *this = *this * v;
        return *this;
    }

    str to_str() const
    {
        if (s == 0) return "0";
        str out;
        if (s < 0) out.pb('-');
        out += tstr(a.back());
        for (int i = (int)sz(a) - 2; i >= 0; i--)
        {
            str p = tstr(a[i]);
            out += str(bd - (int)sz(p), '0') + p;
        }
        return out;
    }
};

  typedef vector<bi> vbi;
typedef vector<vbi> vvbi;

static bi f(int r, int c, int k, const vvbi &cnk)
{
    if (k < 0 || k > r * c) return bi(0);
    bi res(0);
    rep(i, 0, r + 1)
    {
        rep(j, 0, c + 1)
        {
            int cells = (r - i) * (c - j);
            if (k > cells) continue;
            bi term = cnk[r][i] * cnk[c][j];
            term *= cnk[cells][k];
            if ((i + j) & 1) res -= term;
            else res += term;
        }
    }
    return res;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int m, n, w, b;
    cin >> m >> n >> w >> b;

    int kmax = m * n;

    vvbi cnk(kmax + 1, vbi(kmax + 1, bi(0)));
    rep(i, 0, kmax + 1)
    {
        cnk[i][0] = bi(1);
        cnk[i][i] = bi(1);
        rep(j, 1, i) cnk[i][j] = cnk[i - 1][j - 1] + cnk[i - 1][j];

    }

    vvbi fw(m + 1, vbi(n + 1, bi(0)));
    vvbi fb(m + 1, vbi(n + 1, bi(0)));

    rep(r, 0, m + 1)
    {
        rep(c, 0, n + 1)
        {
            fw[r][c] = f(r, c, w, cnk);
            fb[r][c] = f(r, c, b, cnk);
        }
    }

    bi ans(0);

    rep(rw, 0, m + 1)
    {
        rep(rb, 0, m - rw + 1)
        {
            bi wr = cnk[m][rw] * cnk[m - rw][rb];
            rep(cw, 0, n + 1)
            {
                rep(cb, 0, n - cw + 1)
                {
                    bi wc = cnk[n][cw] * cnk[n - cw][cb];
                    bi add = wr * wc;
                    add *= fw[rw][cw];
                    add *= fb[rb][cb];
                    ans += add;
                }
            }
        }
    }

    cout << ans.to_str() << "\n";
    return 0;
}
