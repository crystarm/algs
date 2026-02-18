// https://acm.timus.ru/problem.aspx?space=1&num=1337
// time simulation + dependency graph (DAG) + prerequisites + reverse adjacency updates + greedy earliest-available scheduling

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef vector<int> vi;
typedef vector<vector<int>> vvi;
typedef vector<char> vc;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for(int i=(a); i<(b); i++)
#define pb push_back
#define fi first
#define se second

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, l;
    if(!(cin >> n >> l)) return 0;

    vi day_doc(l + 1, 0);
    for(int i = 1; i <= n; i++)
    {
        int a;
        cin >> a;
        if(1 <= a && a <= l) day_doc[a] = i;
    }

    vector< bitset<101> > need(n + 1);
    vvi rev(n + 1);
    vi cnt(n + 1, 0);

    for(int i = 1; i <= n; i++)
    {
        int x;
        while(cin >> x)
        {
            if(x == 0) break;
            if(1 <= x && x <= n && !need[i][x])
            {
                need[i][x] = 1;
                cnt[i]++;
                rev[x].pb(i);
            }
        }
    }

    int k;
    cin >> k;

    vi have;
    while(true)
    {
        int x;
        cin >> x;
        if(x == 0) break;
        have.pb(x);
    }

    vc req(n + 1, 0);
    int rem = 0;
    while(true)
    {
        int x;
        cin >> x;
        if(x == 0) break;
        if(1 <= x && x <= n && !req[x]) { req[x] = 1; rem++; }
    }

    vc got(n + 1, 0);

    auto obtain = [&](int x)
    {
        if(x < 1 || x > n) return;
        if(got[x]) return;

        got[x] = 1;

        if(req[x]) { req[x] = 0; rem--; }

        for(int y : rev[x]) if(need[y][x]) { need[y][x] = 0; cnt[y]--; }

        need[x].reset();
        cnt[x] = 0;
    };

    for(int x : have) obtain(x);

    if(rem == 0) { cout << 0 << '\n' << '\n'; return 0; }

    vi ans;
    int t = 0;
    int last = -1;

    while(rem > 0)
    {
        if(t - last > l) { cout << "No Solution"; return 0; }

        int weekday = ((k - 1 + t) % l) + 1;
        int doc = day_doc[weekday];
        if(doc != 0 && !got[doc] && cnt[doc] == 0)
        {
            obtain(doc);
            ans.pb(doc);
            last = t;
        }

        if(rem == 0) break;
        t++;
    }

    cout << t << '\n';
    for(int i = 0; i < sz(ans); i++) { if(i) cout << ' '; cout << ans[i]; }
    cout << '\n';

    return 0;
}
