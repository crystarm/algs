#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;

#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i <= (b); ++i)

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    vi arr;
    int x;
    while (cin >> x) arr.push_back(x);

    ll max_dif = 0;
    int mn = 0;
    int rl = 0, rr = 0;

    rep (i, 0, sz(arr))
    {
        if (arr[i] < arr[mn]) mn = i;
        else if ((ll)arr[i] - (ll)arr[mn] > max_dif)
        { max_dif = (ll)arr[i] - (ll)arr[mn]; rl = mn; rr = i; }
    }

    cout << max_dif << " " << rl << " " << rr << "\n";
    return 0;
}
