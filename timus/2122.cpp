// https://acm.timus.ru/problem.aspx?space=1&num=2122
// combinatorics + hamming contribution trick + generating functions + NTT under 40961

#if defined(__clang__)
#define TAVX __attribute__((target("bmi,bmi2,lzcnt,popcnt")))
#else
#define TAVX
#endif

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i <= (b); ++i)
#define fi first
#define se second

const int MOD = 40961;
const int G = 3;
const int MAX_NTT = 8192;
const int MAXF = 5005;

static int a[MAX_NTT];
static int ker[MAX_NTT];
static int roots[MAX_NTT];
static int revv[MAX_NTT];

static int fact[MAXF];
static int ifact[MAXF];

static inline int addm(int x, int y) { int r = x + y; return r >= MOD ? r - MOD : r; }

static inline int subm(int x, int y) { int r = x - y; return r < 0 ? r + MOD : r; }

static inline int mulm(int x, int y) { return (int)((ll)x * y % MOD); }

static int mpow(int x, int e)
{
    int r = 1;
    while (e > 0)
    {
        if (e & 1) r = mulm(r, x);
        x = mulm(x, x);
        e >>= 1;
    }
    return r;
}

static inline int invm(int x) { return mpow(x, MOD - 2); }

static void build_fact(int n)
{
    fact[0] = 1;
    rep(i, 1, n) fact[i] = mulm(fact[i - 1], i);
    ifact[n] = invm(fact[n]);
    for (int i = n - 1; i >= 0; --i) ifact[i] = mulm(ifact[i + 1], i + 1);
}

static inline int ncr(int n, int r)
{
    if (r < 0 || r > n) return 0;
    return mulm(fact[n], mulm(ifact[r], ifact[n - r]));
}

TAVX static void init_ntt()
{
    int step = (MOD - 1) / MAX_NTT;
    int wstep = mpow(G, step);
    int w = 1;
    rep(i, 0, MAX_NTT - 1)
    {
        roots[i] = w;
        w = mulm(w, wstep);
    }

    int lg = 13;
    rep(i, 0, MAX_NTT - 1)
    {
        int r = 0;
        rep(j, 0, lg - 1) if ((i >> j) & 1) r |= (1 << (lg - 1 - j));
        revv[i] = r;
    }
}

TAVX static void ntt(int *p, bool inv)
{
    rep(i, 0, MAX_NTT - 1)
    {
        int j = revv[i];
        if (i < j) swap(p[i], p[j]);
    }

    for (int len = 2; len <= MAX_NTT; len <<= 1)
    {
        int half = len >> 1;
        int shift = MAX_NTT / len;

        for (int i = 0; i < MAX_NTT; i += len)
        {
            rep(j, 0, half - 1)
            {
                int idx = j * shift;
                if (inv && idx) idx = MAX_NTT - idx;

                int w = roots[idx];
                int u = p[i + j];
                int v = mulm(p[i + j + half], w);

                p[i + j] = addm(u, v);
                p[i + j + half] = subm(u, v);
            }
        }
    }

    if (inv)
    {
        int ninv = invm(MAX_NTT);
        rep(i, 0, MAX_NTT - 1) p[i] = mulm(p[i], ninv);
    }
}

int main()
{

    ios::sync_with_stdio(0);
    cin.tie(0);

    string s;
    if (!(cin >> s)) return 0;

    int n = sz(s);
    build_fact(min(MAXF - 1, n + 100));
    init_ntt();

    rep(i, 0, MAX_NTT - 1) ker[i] = 0;
    rep(i, 0, n) ker[i] = ifact[i];
    ntt(ker, 0);

    vector<int> ans(n + 1, 0);

    rep(u, 0, n - 1)
    {
        int maxv = n - 1 - u;

        rep(i, 0, MAX_NTT - 1) a[i] = 0;

        bool has0 = 0;
        rep(v, 0, maxv)
        {

            int idx = n - 1 - v;

            if (s[idx] == '0')
            {
                int c = ncr(idx, u);
                if (c)
                {
                    a[maxv - v] = mulm(c, fact[v]);
                    has0 = 1;
                }
            }
        }

        if (!has0) continue;

        ntt(a, 0);
        rep(i, 0, MAX_NTT - 1) a[i] = mulm(a[i], ker[i]);
        ntt(a, 1);

        rep(v, 0, maxv)
        {
            int conv = maxv - v;

            int c0 = mulm(a[conv], ifact[v]);
            int k = u + v + 1;
            int tot = ncr(n, k);
            int c1 = subm(tot, c0);
            int term = mulm(c0, c1);

            ans[k] = addm(ans[k], term);
        }
    }

    rep(k, 1, n) { if (k > 1) cout << ' '; cout << ans[k]; }
    cout << '\n';

    return 0;
}
