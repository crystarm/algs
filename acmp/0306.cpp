// https://acmp.ru/index.asp?main=task&id_task=306
// simulation + permutations/process modeling  + constructive

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); i++)

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    ifstream fin("INPUT.TXT");
    ofstream fout("OUTPUT.TXT");

    int n;
    fin >> n;

    deque<int> q;
    rep(i,1,n+1) q.push_back(i);

    vector<int> ord;
    ord.reserve(n);

    while (!q.empty())
    {
        ord.push_back(q.front());
        q.pop_front();
        if (!q.empty())
        {
            q.push_back(q.front());
            q.pop_front();
        }
    }

    string s(n, 'B');
    rep(i,0,n)
        s[ord[i]-1] = (i % 2 == 0 ? 'B' : 'R');

    fout << s;
    return 0;
}
