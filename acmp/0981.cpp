// https://acmp.ru/index.asp?main=task&id_task=981
// boolean expressions theory + functional bases

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define epl emplace
#define rsr reserve
#define mv move
#define pb push_back

struct parser
{
    string s;
    size_t p = 0;
    unordered_map<char, int> vidx;
    vector<uint64_t> varMask;
    uint64_t fullMask;

    parser(string str, unordered_map<char, int> m, vector<uint64_t> vm, uint64_t fm)
        : s(mv(str)), vidx(mv(m)), varMask(mv(vm)), fullMask(fm) {}

    void skip() { while (p < s.size() && isspace((unsigned char)s[p])) p++; }

    bool match(char c)
    {
        skip();
        if (p < s.size() && s[p] == c) { p++; return true; }
        return false;
    }

    void expect(char c)
    {
        skip();
        if (p >= s.size() || s[p] != c) exit(0);
        p++;
    }

    uint64_t parse_or()
    {
        uint64_t v = parse_and();
        while (true)
        {
            if (!match('|')) break;
            v |= parse_and();
        }
        return v;
    }

    uint64_t parse_and()
    {
        uint64_t v = parse_unary();
        while (true)
        {
            if (!match('&')) break;
            v &= parse_unary();
        }
        return v;
    }

    uint64_t parse_unary()
    {
        skip();
        if (match('!'))
        {
            uint64_t v = parse_unary();
            return (~v) & fullMask;
        }

        if (match('('))
        {
            uint64_t v = parse_or();
            expect(')');
            return v;
        }

        skip();
        if (p >= s.size()) exit(0);
        char c = s[p++];
        auto it = vidx.find(c);
        if (it == vidx.end()) exit(0);
        return varMask[it->second];
    }

    uint64_t parse()
    {
        uint64_t v = parse_or();
        skip();
        return v;
    }
};

static inline uint64_t med(uint64_t a, uint64_t b, uint64_t c) { return (a & b) | (a & c) | (b & c); }

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    string s;
    if (!getline(cin, s)) return 0;

    vector<char> vars;
    {
        bool seen[256]{0};
        for (unsigned char ch : s)
            if ('a' <= ch && ch <= 'z' && !seen[ch])
            {
                seen[ch] = true;
                vars.pb((char)ch);
            }
        sort(all(vars));
    }

    int k = sz(vars);
    unordered_map<char, int> vidx;
    vidx.rsr(k * 2 + 1);
    rep(i, 0, k) vidx[vars[i]] = i;

    int n = 1 << k;
    uint64_t fullMask = (n == 64) ? ~0ULL : ((1ULL << n) - 1ULL);

    vector<uint64_t> varMask(k, 0);
    rep(i, 0, k)
    {
        uint64_t m = 0;
        rep(mask, 0, n) if ((mask >> i) & 1) m |= (1ULL << mask);
        varMask[i] = m;
    }

    parser ps(s, vidx, varMask, fullMask);
    uint64_t target = ps.parse();

    unordered_map<uint64_t, string> repr;
    repr.rsr(256);
    vector<uint64_t> funcs;
    funcs.rsr(256);

    rep(i, 0, k)
    {
        uint64_t m = varMask[i];
        repr.epl(m, string(1, vars[i]));
        funcs.pb(m);
    }

    bool added = true;
    while (added)
    {
        added = false;
        int cnt = sz(funcs);
        rep(i, 0, cnt)
        {
            uint64_t a = funcs[i];
            for (int j = i; j < cnt; j++)
            {
                uint64_t b = funcs[j];
                for (int t = j; t < cnt; t++)
                {
                    uint64_t c = funcs[t];
                    uint64_t m = med(a, b, c);
                    if (repr.find(m) == repr.end())
                    {
                        const string& ea = repr[a];
                        const string& eb = repr[b];
                        const string& ec = repr[c];
                        string tmp;
                        tmp.rsr(2 + ea.size() + eb.size() + ec.size());
                        tmp.pb('<');
                        tmp += ea;
                        tmp += eb;
                        tmp += ec;
                        tmp.pb('>');
                        repr.epl(m, mv(tmp));
                        funcs.pb(m);
                        added = true;
                    }
                }
            }
        }
    }

    auto it = repr.find(target);
    if (it != repr.end()) cout << it->second << '\n';
    return 0;
}
