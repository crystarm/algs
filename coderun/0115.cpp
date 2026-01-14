// https://coderun.yandex.ru/problem/coevals
// sweep line + interval graphs

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef vector<vector<int>> vvi;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)((x).size())
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr(v,n) (v).reserve(n)
#define ins insert
#define pb push_back

static bool is_leap(int y) { return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0); }

static int dim(int y, int m)
{
    static int md[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2) return md[m] + (is_leap(y) ? 1 : 0);
    return md[m];
}

static int to_ord(int d, int m, int y)
{
    int yy = y - 1;
    int leaps = yy / 4 - yy / 100 + yy / 400;
    int days = 365 * yy + leaps;

    static int pref_norm[13] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    static int pref_leap[13] = {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};

    days += (is_leap(y) ? pref_leap[m] : pref_norm[m]);
    days += d - 1;
    return days;
}

static int add_ord(int d, int m, int y, int add)
{
    int ny = y + add;
    int nd = d, nm = m;
    if (m == 2 && d == 29 && !is_leap(ny)) nd = 28;
    return to_ord(nd, nm, ny);
}

struct event
{
    int t;
    int kind;
    int id;
    bool operator<(event const& o) const
    { if (t != o.t) return t < o.t; return kind < o.kind; }
};

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int N;
    if (!(cin >> N)) return 0;

    vi L(N + 1, 0), R(N + 1, 0);
    vector<event> ev;
    rsr(ev, 2 * N);

    rep(i, 1, N + 1)
    {

        int bd, bm, by, dd, dm, dy;
        cin >> bd >> bm >> by >> dd >> dm >> dy;

        int start = add_ord(bd, bm, by, 18);
        int end80 = add_ord(bd, bm, by, 80);
        int death = to_ord(dd, dm, dy);

        int end = min(death, end80);

        if (start < end)
        {
            L[i] = start;
            R[i] = end;
            ev.pb({start, 1, i});
            ev.pb({end, 0, i});
        }
    }

    sort(all(ev));

    set<int> active;
    multiset<pair<int,int>> ends;
    vvi res;

    int idx = 0;
    while (idx < sz(ev))
    {
        int t = ev[idx].t;

        int j = idx;
        while (j < sz(ev) && ev[j].t == t && ev[j].kind == 0)
        {
            int id = ev[j].id;
            if (active.find(id) != active.end())
            {
                active.erase(id);
                ends.erase(ends.find({R[id], id}));
            }
            ++j;
        }

        int start_cnt = 0;
        int k = j;
        while (k < sz(ev) && ev[k].t == t)
        {
            if (ev[k].kind == 1)
            {
                int id = ev[k].id;
                active.ins(id);
                ends.ins({R[id], id});
                ++start_cnt;
            }
            ++k;
        }

        if (start_cnt > 0 && !active.empty())
        {
            int min_end = ends.begin()->first;
            int next_t = (k < sz(ev)) ? ev[k].t : INT_MAX;
            if (min_end == next_t)
            {
                vi clique;
                rsr(clique, sz(active));
                for (int id : active) clique.pb(id);
                res.pb(move(clique));
            }
        }

        idx = k;
    }

    if (res.empty()) { cout << 0 << '\n'; return 0; }

    for (auto &v : res) { for (int id : v) cout << id << ' '; cout << '\n'; }

    return 0;
}
