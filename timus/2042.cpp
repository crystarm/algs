// https://acm.timus.ru/problem.aspx?space=1&num=2042
// segment tree + lazy range assignment + counting palindromic substrings with length cap using small-k boundary merging (Manacher on small buffer)

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef char ch;
typedef unsigned char uch;

#define sz(x) (int)(x).size()

static const int MAXK = 50;
static const int MAXP = 1 << 17;
static const int MAXBUF = 2 * MAXK + 5;

static int K, KL, ODDLIM, EVENLIM;

struct node
{
    ll cnt;
    int len;
    uch p_len, s_len;
    uch uniform;
    uch u_char;
    ch pref[MAXK];
    ch suff[MAXK];
};

static node t[2 * MAXP];
static uch lz[MAXP];
static int H, N;

static ch mbuf[MAXBUF];
static int d1[MAXBUF];
static int d2[MAXBUF];

static inline ll uni_cnt(int len)
{
    int lim = len < K ? len : K;
    return (ll)lim * (ll)(len + 1) - (ll)lim * (ll)(lim + 1) / 2;
}

static inline node emptify()
{
    node z;
    z.cnt = 0;
    z.len = 0;
    z.p_len = 0;
    z.s_len = 0;
    z.uniform = 1;
    z.u_char = 0;
    return z;
}

static inline void make_uni(node &n, int len, uch c)
{
    n.len = len;
    n.cnt = (len ? uni_cnt(len) : 0);
    n.uniform = 1;
    n.u_char = c;
    int x = len < KL ? len : KL;
    n.p_len = (uch)x;
    n.s_len = (uch)x;
}

static inline void fill_pref(ch *dst, const node &A, int take)
{
    if (take <= 0) return;
    if (A.uniform) memset(dst, (int)A.u_char, (size_t)take);
    else memcpy(dst, A.pref, (size_t)take);
}

static inline void fill_suff(ch *dst, const node &A, int take)
{
    if (take <= 0) return;
    if (A.uniform) memset(dst, (int)A.u_char, (size_t)take);
    else memcpy(dst, A.suff + ((int)A.s_len - take), (size_t)take);
}

static inline ll cross_cnt(int split, int n)
{
    if (n <= 0) return 0;
    if (split <= 0 || split >= n) return 0;

    ll ans = 0;

    int l = 0, r = -1;
    for (int i = 0; i < n; ++i)
    {
        int k = (i > r ? 1 : min(d1[l + r - i], r - i + 1));
        while (i - k >= 0 && i + k < n && mbuf[i - k] == mbuf[i + k]) ++k;
        d1[i] = k;

        int tmax = k;
        if (tmax > ODDLIM) tmax = ODDLIM;

        int tmin = 1;
        int a = split - i + 1;
        int b = i - split + 2;
        if (a > tmin) tmin = a;
        if (b > tmin) tmin = b;

        if (tmin <= tmax) ans += (ll)(tmax - tmin + 1);

        if (i + k - 1 > r)
        {
            l = i - k + 1;
            r = i + k - 1;
        }
    }

    if (EVENLIM)
    {
        l = 0; r = -1;
        for (int i = 0; i < n; ++i)
        {
            int k = (i > r ? 0 : min(d2[l + r - i + 1], r - i + 1));
            while (i - k - 1 >= 0 && i + k < n && mbuf[i - k - 1] == mbuf[i + k]) ++k;
            d2[i] = k;

            int tmax = k;
            if (tmax > EVENLIM) tmax = EVENLIM;

            int diff = i - split;
            if (diff < 0) diff = -diff;
            int tmin = diff + 1;

            if (tmin <= tmax) ans += (ll)(tmax - tmin + 1);

            if (i + k - 1 > r)
            {
                l = i - k;
                r = i + k - 1;
            }
        }
    }

    return ans;
}

static inline node merge_node(const node &L, const node &R)
{
    if (L.len == 0) return R;
    if (R.len == 0) return L;

    if (L.uniform && R.uniform && L.u_char == R.u_char)
    {
        node res;
        make_uni(res, L.len + R.len, L.u_char);
        return res;
    }

    node res;
    res.len = L.len + R.len;
    res.uniform = 0;
    res.u_char = 0;
    res.cnt = L.cnt + R.cnt;

    int sl = (int)L.s_len;
    int pr = (int)R.p_len;

    if (K > 1 && sl && pr)
    {
        if (L.uniform) memset(mbuf, (int)L.u_char, (size_t)sl);
        else memcpy(mbuf, L.suff + ((int)L.s_len - sl), (size_t)sl);

        if (R.uniform) memset(mbuf + sl, (int)R.u_char, (size_t)pr);
        else memcpy(mbuf + sl, R.pref, (size_t)pr);

        res.cnt += cross_cnt(sl, sl + pr);
    }

    int need = res.len < KL ? res.len : KL;
    res.p_len = (uch)need;
    res.s_len = (uch)need;

    int takeL = L.len < need ? L.len : need;
    int takeR = need - takeL;
    if (takeL) fill_pref(res.pref, L, takeL);
    if (takeR) fill_pref(res.pref + takeL, R, takeR);

    int takeRR = R.len < need ? R.len : need;
    int takeLL = need - takeRR;
    if (takeLL) fill_suff(res.suff, L, takeLL);
    if (takeRR) fill_suff(res.suff + takeLL, R, takeRR);

    return res;
}

static inline void apply(int p, uch c)
{
    make_uni(t[p], t[p].len, c);
    if (p < N) lz[p] = c;
}

static inline void calc(int p)
{
    if (lz[p]) return;
    t[p] = merge_node(t[p << 1], t[p << 1 | 1]);
}

static inline void push(int p)
{
    if (!lz[p]) return;
    uch c = lz[p];
    apply(p << 1, c);
    apply(p << 1 | 1, c);
    lz[p] = 0;
}

static inline void push_path(int p)
{
    for (int s = H; s >= 1; --s) push(p >> s);
}

static inline void build_path(int p)
{
    for (p >>= 1; p; p >>= 1) calc(p);
}

static inline void range_assign(int l, int r, uch c)
{
    int l0 = l + N;
    int r0 = r + N;

    int L = l0, R = r0 - 1;
    push_path(L);
    push_path(R);

    while (l0 < r0)
    {
        if (l0 & 1) apply(l0++, c);
        if (r0 & 1) apply(--r0, c);
        l0 >>= 1;
        r0 >>= 1;
    }

    build_path(L);
    build_path(R);
}

static inline node range_query(int l, int r)
{
    int l0 = l + N;
    int r0 = r + N;

    push_path(l0);
    push_path(r0 - 1);

    node left = emptify();
    node right = emptify();

    while (l0 < r0)
    {
        if (l0 & 1) left = merge_node(left, t[l0++]);
        if (r0 & 1) right = merge_node(t[--r0], right);
        l0 >>= 1;
        r0 >>= 1;
    }

    return merge_node(left, right);
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    string s;
    cin >> s >> K;

    KL = K - 1;
    if (KL < 0) KL = 0;
    ODDLIM = (K + 1) >> 1;
    EVENLIM = K >> 1;

    int n0 = sz(s);
    N = 1;
    H = 0;
    while (N < n0)
    {
        N <<= 1;
        ++H;
    }

    for (int i = 1; i < N; ++i) lz[i] = 0;

    for (int i = 0; i < N; ++i)
    {
        t[N + i] = emptify();
        if (i < n0) make_uni(t[N + i], 1, (uch)s[i]);
    }

    for (int p = N - 1; p >= 1; --p)
    {
        t[p] = merge_node(t[p << 1], t[p << 1 | 1]);
        lz[p] = 0;
    }

    int q;
    cin >> q;

    for (int it = 0; it < q; ++it)
    {
        int tp;
        cin >> tp;
        if (tp == 1)
        {
            int l, r;
            ch c;
            cin >> l >> r >> c;
            range_assign(l - 1, r, (uch)c);
        }
        else
        {
            int l, r;
            cin >> l >> r;
            node res = range_query(l - 1, r);
            cout << res.cnt << '\n';
        }
    }

    return 0;
}
