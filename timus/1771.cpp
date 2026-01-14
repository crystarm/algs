// https://acm.timus.ru/problem.aspx?space=1&num=1771
// event simulation + greedy/tie-breaking choice model + adversarial min/max + feasibility/constraints reasoning

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef vector<vector<int>> vvi;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for(int i=(a); i<(b); i++)
#define pb push_back

static const int INF = 1e9;

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    vvi rt(m + 1);
    vvi pos(m + 1, vi(n + 1, -1));

    rep(i, 1, m + 1)
    {
        int x;
        while (cin >> x) { if (x == -1) break; rt[i].pb(x); }
        rep(j, 0, sz(rt[i])) pos[i][rt[i][j]] = j;
    }

    int k;
    cin >> k;
    vi t1(k), rid(k), st(k);

    auto parse_time = [&](const string &s) -> int
    {
        int hh = (s[0] - '0') * 10 + (s[1] - '0');
        int mm = (s[3] - '0') * 10 + (s[4] - '0');
        return hh * 60 + mm;
    };

    rep(i, 0, k)
    {
        string ts;
        cin >> ts >> rid[i];
        t1[i] = parse_time(ts);
        int p1 = pos[rid[i]][1];
        st[i] = t1[i] - p1;
    }

    int T = t1[k - 1];
    int rL = rid[k - 1];

    vi &L = rt[rL];
    vi posL(n + 1, -1);
    rep(i, 0, sz(L)) posL[L[i]] = i;

    int p1L = posL[1];
    int p2L = posL[2];
    int stL = T - p1L;

    auto can_deliver = [&](int r, int a, int b) -> bool
    {
        int pa = pos[r][a], pb = pos[r][b];
        return pa != -1 && pb != -1 && pa < pb;
    };

    auto arrive = [&](int bus, int s) -> int
    {
        int r = rid[bus];
        int ps = pos[r][s];
        if (ps == -1) return INF;
        return st[bus] + ps;
    };

    bool DT_pre = false, DT_post = false;
    rep(idx, p1L + 1, sz(L))
    {
        int d = L[idx];

        bool ok = true;
        rep(j, 0, k)
        {
            if (t1[j] >= T) continue;
            if (can_deliver(rid[j], 1, d)) { ok = false; break; }
        }
        if (!ok) continue;

        int best_dist = INF, best_route = INF;
        rep(j, 0, k)
        {
            if (t1[j] != T) continue;
            int r = rid[j];
            if (!can_deliver(r, 1, d)) continue;
            int dist = pos[r][d] - pos[r][1];
            if (dist < best_dist || (dist == best_dist && r < best_route))
            { best_dist = dist; best_route = r; }
        }

        if (best_route == rL)
        { if (idx < p2L) DT_pre = true; else DT_post = true; }

    }

    int min13 = DT_pre ? 0 : 13;
    int max13 = DT_post ? 13 : 0;

    auto feasible_dest_42 = [&](int d) -> bool
    {
        rep(ii, 0, p1L)
        {
            int u = L[ii];
            int tU = stL + ii;

            bool he = false;
            rep(j, 0, k)
            {
                int r = rid[j];
                if (!can_deliver(r, u, d)) continue;
                int aU = arrive(j, u);
                if (aU < tU)
                { he = true; break; }
            }
            if (he) continue;

            int best_dist = INF, best_route = INF;
            rep(j, 0, k)
            {
                int r = rid[j];
                if (!can_deliver(r, u, d)) continue;
                int aU = arrive(j, u);
                if (aU != tU) continue;
                int dist = pos[r][d] - pos[r][u];
                if (dist < best_dist || (dist == best_dist && r < best_route))
                { best_dist = dist; best_route = r; }
            }

            if (best_route == rL) return true;
        }
        return false;
    };

    bool ex_pre = false, ex_post = false;
    rep(idx, p1L + 1, sz(L))
    {
        int d = L[idx];
        if (!feasible_dest_42(d)) continue;
        if (idx < p2L) ex_pre = true;
        else ex_post = true;
    }

    int min42 = ex_pre ? 0 : 42;
    int max42 = ex_post ? 42 : 0;

    cout << (min42 + min13) << " " << (max42 + max13) << "\n";
    return 0;
}
