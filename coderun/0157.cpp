// https://coderun.yandex.ru/problem/qx-d
// FSM + string parsing + data aggregation

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef string str;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define pb push_back
#define fi first
#define se second

const int INF = (int)1e9;
const ll LINF = (ll)4e18;

str s;
int p;

void skip_ws()
{
    while (p < sz(s) && isspace((unsigned char)s[p])) p++;
}

str parse_str()
{
    str r;
    if (p >= sz(s) || s[p] != '"') return r;
    p++;
    while (p < sz(s))
    {
        char c = s[p++];
        if (c == '"') return r;
        if (c == '\\')
        {
            if (p < sz(s)) r += s[p++];
        }
        else r += c;
    }
    return r;
}

int parse_int()
{
    skip_ws();
    int sign = 1;
    if (p < sz(s) && s[p] == '-') { sign = -1; p++; }
    ll x = 0;
    bool ok = false;
    while (p < sz(s) && isdigit((unsigned char)s[p]))
    {
        ok = true;
        x = x * 10 + (s[p] - '0');
        p++;
    }
    if (!ok) return 0;
    x *= sign;
    if (x < (ll)INT_MIN) return INT_MIN;
    if (x > (ll)INT_MAX) return INT_MAX;
    return (int)x;
}

void skip_val()
{
    skip_ws();
    if (p >= sz(s)) return;
    char c = s[p];
    if (c == '"') { parse_str(); return; }
    if (c == '-' || isdigit((unsigned char)c)) { parse_int(); return; }
    if (c == '{' || c == '[')
    {
        char open = c;
        char close = (c == '{' ? '}' : ']');
        int bal = 1;
        p++;
        while (p < sz(s) && bal > 0)
        {
            if (s[p] == open) bal++;
            if (s[p] == close) bal--;
            p++;
        }
        return;
    }
    while (p < sz(s) && s[p] != ',' && s[p] != '}' && s[p] != ']') p++;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    ifstream fin("data.json");
    if (!fin.is_open()) return 1;

    fin.seekg(0, ios::end);
    s.reserve((size_t)fin.tellg());
    fin.seekg(0, ios::beg);
    s.assign((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());

    map<str, int> mp;

    p = 0;
    skip_ws();
    if (p < sz(s) && s[p] == '[') p++;

    while (p < sz(s))
    {
        skip_ws();
        if (p >= sz(s) || s[p] == ']') break;

        if (s[p] == '{')
        {
            p++;

            str cur;
            int val = -1;
            bool okd = false, okc = false;

            while (p < sz(s))
            {
                skip_ws();
                if (p < sz(s) && s[p] == '}') { p++; break; }

                str key = parse_str();

                skip_ws();
                if (p < sz(s) && s[p] == ':') p++;
                skip_ws();

                if (key == "date") { cur = parse_str(); okd = true; }
                else if (key == "count") { val = parse_int(); okc = true; }
                else skip_val();

                skip_ws();
                if (p < sz(s) && s[p] == ',') p++;
            }

            if (okd && okc)
            {
                auto it = mp.find(cur);
                if (it == mp.end()) mp[cur] = val;
                else it->se = max(it->se, val);
            }
        }
        else p++;

        skip_ws();
        if (p < sz(s) && s[p] == ',') p++;
    }

    if (mp.empty()) return 0;

    int mn = INT_MAX;
    for (auto &it : mp) if (it.se < mn) mn = it.se;
    for (auto &it : mp) if (it.se == mn) cout << it.fi << '\n';

    return 0;
}
