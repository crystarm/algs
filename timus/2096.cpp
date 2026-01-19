// https://acm.timus.ru/problem.aspx?space=1&num=2096
// version tree (parent pointers) + DFS + rollback (undo stack) + AP-step updates

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef string str;
typedef vector<int> vi;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)((x).size())
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr(v,n) (v).reserve(n)
#define pb push_back
#define resz(v,n) (v).resize(n)

struct FS
{
    static const int S = 1 << 20;
    int it = 0, n = 0;
    char b[S];

    inline char gc()
    {
        if (it >= n)
        {
            n = (int)fread(b, 1, S, stdin);
            it = 0;
            if (!n) return 0;
        }
        return b[it++];
    }

    template <class T>
    bool rd(T &x)
    {
        char c;
        do { c = gc();
             if (!c) return false;
        } while (c <= ' ');

        bool neg = false;
        if (c == '-')
        { neg = true; c = gc(); }

        T v = 0;
        while (c > ' ')
        { v = v * 10 + (c - '0'); c = gc(); }

        x = neg ? -v : v;

        return true;
    }
};

struct QN { int nx, id, d; };
struct MD { int d, k, p, t; };
struct EG { int to, nx; };
struct FR { int v, e, sp, st; };

static inline void app(str &s, int x)
{
    ll y = x;
    if (y < 0)
    { s.pb('-'); y = -y; }

    char t[24];
    int k = 0;

    if (y == 0) t[k++] = '0';
    else
    {
        char u[24];
        int m = 0;
        while (y > 0)
        { u[m++] = char('0' + (y % 10)); y /= 10; }
        while (m--) t[k++] = u[m];
    }

    s.append(t, t + k);
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    FS fs;

    int n, m;
    if (!fs.rd(n)) return 0;
    fs.rd(m);

    vi a(n + 1);
    rep(i, 1, n + 1) fs.rd(a[i]);

    vi par(m + 1, 0);
    vi ofs(m + 1, 0), ln(m + 1, 0);

    vi res;
    rsr(res, 100000);

    vi qh(m + 1, -1);
    vector<QN> qn;
    rsr(qn, 100000);

    vi ms(m + 1, 0), mc(m + 1, 0);
    vector<MD> md;
    rsr(md, 100000);

    ll sumk = 0;

    rep(i, 1, m + 1)
    {
        int q;
        fs.rd(q);

        ofs[i] = sz(res);
        ln[i] = q;
        resz(res, sz(res) + q);

        rep(j, 0, q)
        {
            int f, x;
            fs.rd(f);
            fs.rd(x);
            int id = ofs[i] + j;
            qn.pb({qh[f], id, x});
            qh[f] = sz(qn) - 1;

        }

        int p0, c0;
        fs.rd(p0);
        fs.rd(c0);
        par[i] = p0;

        ms[i] = sz(md);
        mc[i] = c0;
        rep(j, 0, c0)
        {
            int d, k, p, t;
            fs.rd(d);
            fs.rd(k);
            fs.rd(p);
            fs.rd(t);
            md.pb({d, k, p, t});
            sumk += k;
        }
    }

    vi ch(m + 1, -1);
    vector<EG> eg;
    rsr(eg, m);
    rep(i, 1, m + 1)
    {
        int p = par[i];
        eg.pb({i, ch[p]});
        ch[p] = sz(eg) - 1;
    }

    vector<pair<int,int>> stg;
    rsr(stg, (size_t)sumk);

    vector<FR> st;
    rsr(st, m + 1);
    st.pb({0, -1, 0, 0});

    while (!st.empty())
    {
        FR &fr = st.back();
        if (fr.st == 0)
        {
            fr.sp = sz(stg);
            int v = fr.v;

            int s = ms[v];
            int c = mc[v];
            rep(i, 0, c)
            {
                const MD &z = md[s + i];
                int d = z.d;
                rep(r, 0, z.k)
                {
                    int old = a[d];
                    stg.pb({d, old});
                    a[d] = z.t;
                    d += z.p;
                }
            }

            for (int q = qh[v]; q != -1; q = qn[q].nx) res[qn[q].id] = a[qn[q].d];

            fr.st = 1;
            fr.e = ch[v];
        }
        else
        {
            if (fr.e != -1)
            {
                int ei = fr.e;
                fr.e = eg[ei].nx;
                int to = eg[ei].to;
                st.pb({to, -1, 0, 0});
            }
            else
            {
                while (sz(stg) > fr.sp)
                {
                    auto pr = stg.back();
                    stg.pop_back();
                    a[pr.first] = pr.second;
                }
                st.pop_back();
            }
        }
    }

    str out;
    rsr(out, (size_t)sz(res) * 12 + (size_t)m * 2);

    rep(i, 1, m + 1)
    {
        int s = ofs[i];
        int l = ln[i];
        rep(j, 0, l) { if (j) out.pb(' '); app(out, res[s + j]); }
        out.pb('\n');
    }

    fwrite(out.c_str(), 1, out.size(), stdout);



    return 0;
}
