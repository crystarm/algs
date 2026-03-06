// https://acm.timus.ru/problem.aspx?space=1&num=2077
// LCP + prefix groups + trie idea + DSU + greedy construction

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned char uc;
typedef vector<int> vi;
typedef string str;
typedef pair<int, int> pii;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr(v,n) (v).reserve(n)
#define pb push_back
#define ppb pop_back
#define fi first
#define se second
#define clr clear
#define asg assign
#define rsz resize
#define apd append

const uc TP_DASH = 0;
const uc TP_DOT = 1;
const uc TP_FULL = 2;
const int INF = 1e9;

struct seg
{
    int n;
    vi t;

    void init(int m)
    {
        n = 1;
        while (n < m) n <<= 1;
        t.asg(n << 1, INF);
    }

    int get(int l, int r)
    {
        if (l > r) return 0;
        int res = INF;
        for (l += n, r += n; l <= r; l >>= 1, r >>= 1)
        {
            if (l & 1) res = min(res, t[l++]);
            if (!(r & 1)) res = min(res, t[r--]);
        }
        return res;
    }
};

struct edg { int w; int u; int v; };

int n;
str buf;
vi off, len, rr, btot, bk;
vector<uc> btp;
seg sg;

vi ord, dsu_p, dsu_sz, dsu_act, hp_rt, hp_l, hp_r, hp_d, hp_key, lim, mrk, tch, rts;
vector<edg> es;
vector<pii> add, ask;
int timr;

int lcp(int i, int j)
{
    int a = off[i];
    int b = off[j];
    int m = min(len[i], len[j]);
    int p = 0;
    while (p < m && buf[a + p] == buf[b + p]) ++p;
    return p;
}

int lcs(int i, int j)
{
    int a = off[i] + len[i] - 1;
    int b = off[j] + len[j] - 1;
    int m = min(len[i], len[j]);
    int p = 0;
    while (p < m && buf[a - p] == buf[b - p]) ++p;
    return p;
}

bool rev_less(int i, int j)
{
    int a = off[i] + len[i] - 1;
    int b = off[j] + len[j] - 1;
    int la = off[i];
    int lb = off[j];
    while (a >= la && b >= lb)
    {
        char ca = buf[a];
        char cb = buf[b];
        if (ca != cb) return ca < cb;
        --a;
        --b;
    }
    return len[i] < len[j];
}

void upd(int id, int tot, uc tp, int k)
{
    if (tot < btot[id])
    {
        btot[id] = tot;
        btp[id] = tp;
        bk[id] = k;
        return;
    }
    if (tot > btot[id]) return;
    if (tp > btp[id])
    {
        btp[id] = tp;
        bk[id] = k;
        return;
    }
    if (tp == btp[id] && tp == TP_DASH && k > bk[id]) bk[id] = k;
}

int find(int v)
{
    while (dsu_p[v] != v) { dsu_p[v] = dsu_p[dsu_p[v]]; v = dsu_p[v]; }
    return v;
}

int mrg(int a, int b)
{
    if (a == -1) return b;
    if (b == -1) return a;
    if (hp_key[a] < hp_key[b]) swap(a, b);
    hp_r[a] = mrg(hp_r[a], b);
    int l = hp_l[a];
    int r = hp_r[a];
    int dl = (l == -1 ? 0 : hp_d[l]);
    int dr = (r == -1 ? 0 : hp_d[r]);
    if (dl < dr)
    {
        swap(hp_l[a], hp_r[a]);
        swap(dl, dr);
    }
    hp_d[a] = dr + 1;
    return a;
}

int uni(int a, int b)
{
    a = find(a);
    b = find(b);
    if (a == b) return a;
    if (dsu_sz[a] < dsu_sz[b]) swap(a, b);
    dsu_p[b] = a;
    dsu_sz[a] += dsu_sz[b];
    dsu_act[a] += dsu_act[b];
    hp_rt[a] = mrg(hp_rt[a], hp_rt[b]);
    return a;
}

void proc(int dep, int l, int r)
{
    int m = r - l + 1;
    if (m <= 1) return;

    ord.rsz(m);
    rep(i,0,m) ord[i] = l + i;
    sort(all(ord), [&](int a, int b) { return rr[a] < rr[b]; });

    dsu_p.rsz(m); dsu_sz.rsz(m); dsu_act.rsz(m); hp_rt.rsz(m); hp_l.rsz(m); hp_r.rsz(m); hp_d.rsz(m); hp_key.rsz(m); lim.rsz(m);
    mrk.asg(m, 0);
    tch.clr(); rts.clr(); es.clr(); add.clr(); ask.clr();
    timr = 1;

    rep(i,0,m)
    {
        dsu_p[i] = i;
        dsu_sz[i] = 1;
        dsu_act[i] = 0;
        hp_l[i] = -1;
        hp_r[i] = -1;
        hp_d[i] = 1;
        lim[i] = len[ord[i]] - dep;
        hp_key[i] = lim[i] - 1;
        hp_rt[i] = (lim[i] > 0 ? i : -1);
        add.pb({lim[i], i});
        if (lim[i] > 0) ask.pb({lim[i] - 1, i});
    }

    rsr(es, max(0, m - 1));
    rep(i,1,m)
    {
        int a = rr[ord[i - 1]];
        int b = rr[ord[i]];
        int w = sg.get(a + 1, b);
        es.pb({w, i - 1, i});
    }

    sort(all(es), [&](const edg &a, const edg &b) { return a.w > b.w; });
    sort(all(add), greater<pii>());
    sort(all(ask), greater<pii>());

    int ie = 0;
    int ia = 0;
    int iq = 0;

    while (ie < sz(es) || ia < sz(add) || iq < sz(ask))
    {
        int cur = -1;
        if (ie < sz(es)) cur = max(cur, es[ie].w);
        if (ia < sz(add)) cur = max(cur, add[ia].fi);
        if (iq < sz(ask)) cur = max(cur, ask[iq].fi);

        tch.clr();

        while (ia < sz(add) && add[ia].fi == cur)
        {
            int v = add[ia].se;
            ++ia;
            int rt = find(v);
            ++dsu_act[rt];
            tch.pb(rt);
        }

        while (ie < sz(es) && es[ie].w == cur)
        {
            int rt = uni(es[ie].u, es[ie].v);
            ++ie;
            tch.pb(rt);
        }

        while (iq < sz(ask) && ask[iq].fi == cur)
        {
            int v = ask[iq].se;
            ++iq;
            tch.pb(find(v));
        }

        ++timr;
        rts.clr();
        for (int x : tch)
        {
            int rt = find(x);
            if (mrk[rt] == timr) continue;
            mrk[rt] = timr;
            rts.pb(rt);
        }

        for (int rt : rts)
        {
            if (dsu_act[rt] < 2) continue;
            while (hp_rt[rt] != -1 && hp_key[hp_rt[rt]] >= cur)
            {
                int v = hp_rt[rt];
                hp_rt[rt] = mrg(hp_l[v], hp_r[v]);
                upd(ord[v], dep + cur + 2, TP_DASH, dep);
            }
        }
    }
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n;

    off.rsz(n);
    len.rsz(n);

    vi mx(n, 0);
    rsr(buf, 1000005);

    str s, p;
    rep(i,0,n)
    {
        cin >> s;
        off[i] = sz(buf);
        len[i] = sz(s);
        buf += s;

        if (i > 0)
        {
            int m = min(sz(p), sz(s));
            int j = 0;
            while (j < m && p[j] == s[j]) ++j;
            mx[i] = max(mx[i], j);
            mx[i - 1] = max(mx[i - 1], j);
        }
        p = s;
    }

    btot.rsz(n);
    bk.rsz(n);
    btp.rsz(n);

    rep(i,0,n)
    {
        btot[i] = len[i];
        bk[i] = len[i];
        btp[i] = TP_FULL;
    }

    rep(i,0,n)
    {
        int k = mx[i] + 1;
        if (k < len[i]) upd(i, k + 1, TP_DOT, k);
    }

    vi id(n);
    iota(all(id), 0);

    sort(all(id), [&](int a, int b) { return rev_less(a, b); });

    rr.rsz(n);
    rep(i,0,n) rr[id[i]] = i;

    sg.init(n);
    rep(i,1,n) sg.t[sg.n + i] = lcs(id[i - 1], id[i]);
    for (int i = sg.n - 1; i >= 1; --i) sg.t[i] = min(sg.t[i << 1], sg.t[i << 1 | 1]);

    vi st;
    rep(i,0,n)
    {
        int lc = 0;
        if (i > 0) lc = lcp(i - 1, i);

        while (sz(st) > lc)
        {
            int dep = sz(st);
            int lf = st.back();
            st.ppb();
            proc(dep, lf, i - 1);
        }

        for (int d = sz(st) + 1; d <= len[i]; ++d) st.pb(i);
    }

    while (!st.empty())
    {
        int dep = sz(st);
        int lf = st.back();
        st.ppb();
        proc(dep, lf, n - 1);
    }

    str out;
    rsr(out, sz(buf) + n * 3);

    rep(i,0,n)
    {
        int p0 = off[i];
        int m = len[i];

        if (btp[i] == TP_FULL) out.apd(buf, p0, m);
        else if (btp[i] == TP_DOT)
        {
            out.apd(buf, p0, bk[i]);
            out.pb('.');
        }
        else
        {
            int k = bk[i];
            int sfx = btot[i] - k - 1;
            out.apd(buf, p0, k);
            out.pb('-');
            out.apd(buf, p0 + m - sfx, sfx);
        }

        out.pb('\n');
    }

    cout << out;
    return 0;
}
