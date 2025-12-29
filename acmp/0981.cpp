// https://acmp.ru/index.asp?main=task&id_task=981
// boolean expressions theory + functional bases

#include <bits/stdc++.h>
using namespace std;

struct Parser
{
    string s;
    size_t p = 0;
    unordered_map<char, int> vidx;
    vector<uint64_t> varMask;
    uint64_t fullMask;

    Parser(string str, unordered_map<char, int> m, vector<uint64_t> vm, uint64_t fm)
        : s(std::move(str)), vidx(std::move(m)), varMask(std::move(vm)), fullMask(fm) {}

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
        while (true) {
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

    string in;
    if (!getline(cin, in)) return 0;

    vector<char> vars;
    {
        bool seen[256]{0};
        for (unsigned char ch : in)
            if ('a' <= ch && ch <= 'z' && !seen[ch])
            {
                seen[ch] = true;
                vars.push_back((char)ch);
            }
        sort(vars.begin(), vars.end());
    }

    int k = (int)vars.size();
    unordered_map<char, int> vidx;
    vidx.reserve(k * 2 + 1);
    for (int i = 0; i < k; i++) vidx[vars[i]] = i;

    int bits = 1 << k;
    uint64_t fullMask = (bits == 64) ? ~0ULL : ((1ULL << bits) - 1ULL);

    vector<uint64_t> varMask(k, 0);
    for (int i = 0; i < k; i++)
    {
        uint64_t m = 0;
        for (int a = 0; a < bits; a++) if ((a >> i) & 1) m |= (1ULL << a);
        varMask[i] = m;
    }

    Parser ps(in, vidx, varMask, fullMask);
    uint64_t target = ps.parse();

    unordered_map<uint64_t, string> repr;
    repr.reserve(256);
    vector<uint64_t> funcs;
    funcs.reserve(256);

    for (int i = 0; i < k; i++)
    {
        uint64_t m = varMask[i];
        repr.emplace(m, string(1, vars[i]));
        funcs.push_back(m);
    }

    bool added = true;
    while (added)
    {
        added = false;
        int n = (int)funcs.size();
        for (int i = 0; i < n; i++)
        {
            uint64_t a = funcs[i];
            for (int j = i; j < n; j++)
            {
                uint64_t b = funcs[j];
                for (int t = j; t < n; t++)
                {
                    uint64_t c = funcs[t];
                    uint64_t m = med(a, b, c);
                    if (repr.find(m) == repr.end())
                    {
                        const string &ea = repr[a];
                        const string &eb = repr[b];
                        const string &ec = repr[c];
                        string s;
                        s.reserve(2 + ea.size() + eb.size() + ec.size());
                        s.push_back('<');
                        s += ea;
                        s += eb;
                        s += ec;
                        s.push_back('>');
                        repr.emplace(m, std::move(s));
                        funcs.push_back(m);
                        added = true;
                    }
                }
            }
        }
    }

    auto it = repr.find(target);
    if (it != repr.end()) cout << it->second << "\n";
    return 0;
}
