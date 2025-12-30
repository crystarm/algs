// https://acmp.ru/index.asp?main=task&id_task=937
// scopes + recursive parsing + interpreting

#include <bits/stdc++.h>
using namespace std;

typedef string str;

#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)

static str parse_id(const str& s, int& ptr)
{
    str id;
    while (ptr < sz(s) && s[ptr] >= 'a' && s[ptr] <= 'z') id += s[ptr++];
    if (ptr < sz(s) && s[ptr] == ' ') ptr++;
    return id;
}

static int parse_int(const str& s, int& ptr)
{
    int x = 0;
    bool any = false;
    while (ptr < sz(s) && s[ptr] >= '0' && s[ptr] <= '9')
    {
        any = true;
        x = x * 10 + (s[ptr++] - '0');
    }
    if (ptr < sz(s) && s[ptr] == ' ') ptr++;
    return any ? x : 0;
}

static str parse_blk(const str& s, int& ptr)
{
    int bal = 1;
    int start = ++ptr;
    while (ptr < sz(s) && bal)
    {
        if (s[ptr] == '{') bal++;
        else if (s[ptr] == '}') bal--;
        if (bal) ptr++;
    }
    return s.substr(start, ptr++ - start);
}

static void exec(const str& s, map<str, str> ctx)
{
    int ptr = 0;
    int n = sz(s);

    while (ptr < n)
    {
        if (s[ptr] == '#')
        {
            if (ptr + 1 < n && s[ptr + 1] == '#')
            {
                ptr += 2;
                str id = parse_id(s, ptr);
                if (ctx.count(id)) exec(ctx[id], ctx);
            }
            else
            {
                ptr++;
                str id = parse_id(s, ptr);
                if (id == "rep")
                {
                    int k = parse_int(s, ptr);
                    str blk = parse_blk(s, ptr);
                    while (k--) exec(blk, ctx);
                }
                else ctx[id] = parse_blk(s, ptr);
            }
        }
        else cout << s[ptr++];
    }
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    if (!(cin >> n)) return 0;

    str tmp, s;
    getline(cin, tmp);

    rep(i, 0, n)
    {
        getline(cin, tmp);
        s += tmp;
        if (i + 1 < n) s += '\n';
    }

    exec(s, {});
    return 0;
}
