// https://acmp.ru/index.asp?main=task&id_task=884
// sliding window + bitmasks

#include <bits/stdc++.h>
using namespace std;

typedef string str;

#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define pb push_back

static const int A_BITS = 13;
static const int B_BITS = 13;
static const int A_STATES = 1 << A_BITS;
static const int B_STATES = 1 << B_BITS;
static const int WORDS = B_STATES / 64;
static const unsigned FULLA = (unsigned)A_STATES - 1u;
static const unsigned FULLB = (unsigned)B_STATES - 1u;

static const int POOL_SIZE = 1594323;

static uint64_t bad[A_STATES][WORDS];
static uint64_t pat0[B_BITS][WORDS];
static uint32_t off[B_STATES + 1];
static uint16_t pool[POOL_SIZE];

static inline void build_patterns()
{
    rep(m, 0, B_STATES)
    {
        unsigned w = (unsigned)m >> 6;
        unsigned b = (unsigned)m & 63u;
        uint64_t bit = 1ULL << b;
        rep(i, 0, B_BITS) if ((((unsigned)m >> (unsigned)i) & 1u) == 0u) pat0[i][w] |= bit;
    }
}

static inline void build_pool()
{
    off[0] = 0;
    rep(b, 0, B_STATES)
    {
        unsigned zeros = (unsigned)B_BITS - (unsigned)__builtin_popcount((unsigned)b);
        off[b + 1] = off[b] + (1u << zeros);
    }

    uint32_t idx = 0;
    rep(b, 0, B_STATES)
    {
        unsigned c = (~(unsigned)b) & FULLB;
        unsigned m = c;
        for (;;)
        {
            pool[idx++] = (uint16_t)m;
            if (m == 0u) break;
            m = (m - 1u) & c;
        }
    }
}

static inline void add_window(uint32_t wa, uint32_t wb)
{
    uint64_t* row = bad[wa];

    uint32_t lo = off[wb];
    uint32_t hi = off[wb + 1];
    uint32_t len = hi - lo;
    unsigned k = (unsigned)__builtin_popcount(wb);

    if (len <= (uint32_t)(WORDS * (k + 1u)))
    {
        for (uint32_t i = lo; i < hi; ++i)
        {
            unsigned m = pool[i];
            row[m >> 6] |= 1ULL << (m & 63u);
        }
        return;
    }

    if (wb == 0u)
    {
        rep(w, 0, WORDS) row[w] = ~0ULL;
        return;
    }

    uint64_t tmp[WORDS];
    unsigned first = (unsigned)__builtin_ctz(wb);
    memcpy(tmp, pat0[first], sizeof(tmp));

    unsigned rest = wb & (wb - 1u);
    while (rest)
    {
        unsigned i = (unsigned)__builtin_ctz(rest);
        rest &= rest - 1u;
        uint64_t* p = pat0[i];
        rep(w, 0, WORDS) tmp[w] &= p[w];
    }

    rep(w, 0, WORDS) row[w] |= tmp[w];
}

static inline void sos_or_transform()
{
    rep(bit, 0, A_BITS)
    {
        unsigned b = 1u << (unsigned)bit;
        rep(m, 0, A_STATES)
        {
            if ((unsigned)m & b)
            {
                uint64_t* dst = bad[m];
                uint64_t* src = bad[m ^ (int)b];
                rep(w, 0, WORDS) dst[w] |= src[w];
            }
        }
    }
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int K;
    str s;
    if (!(cin >> K)) return 0;
    if (!(cin >> s)) return 0;

    int L = sz(s);

    build_patterns();
    build_pool();

    int cnt[26];
    memset(cnt, 0, sizeof(cnt));
    uint32_t mask = 0u;

    rep(i, 0, K)
    {
        int c = s[i] - 'A';
        if (cnt[c]++ == 0) mask |= 1u << (unsigned)c;
    }

    int windows = L - K + 1;
    rep(start, 0, windows)
    {
        uint32_t wa = mask & FULLA;
        uint32_t wb = (mask >> A_BITS) & FULLB;
        add_window(wa, wb);

        if (start + K < L)
        {
            int out = s[start] - 'A';
            if (--cnt[out] == 0) mask &= ~(1u << (unsigned)out);

            int in = s[start + K] - 'A';
            if (cnt[in]++ == 0) mask |= 1u << (unsigned)in;
        }
    }

    sos_or_transform();

    static uint16_t tb_order[B_STATES];
    unsigned pos = 0;
    rep(pc, 0, B_BITS + 1)
        rep(tb, 0, B_STATES)
            if ((unsigned)__builtin_popcount((unsigned)tb) == (unsigned)pc)
                tb_order[pos++] = (uint16_t)tb;

    int bestN = 27;
    uint32_t bestTa = 0u, bestTb = 0u;

    rep(ta, 0, A_STATES)
    {
        int pa = __builtin_popcount((unsigned)ta);
        if (pa >= bestN) continue;

        uint32_t x = (~(uint32_t)ta) & FULLA;
        uint64_t* inv = bad[x];

        rep(i, 0, B_STATES)
        {
            uint32_t tb = tb_order[i];
            int pb = __builtin_popcount(tb);
            if (pa + pb >= bestN) break;

            if ((inv[tb >> 6] & (1ULL << (tb & 63u))) == 0u)
            {
                bestN = pa + pb;
                bestTa = (uint32_t)ta;
                bestTb = tb;
                break;
            }
        }
    }

    cout << bestN << '\n';
    rep(i, 0, A_BITS) if (((bestTa >> (unsigned)i) & 1u) != 0u) cout << char('A' + i) << '\n';
    rep(i, 0, B_BITS) if (((bestTb >> (unsigned)i) & 1u) != 0u) cout << char('A' + (A_BITS + i)) << '\n';

    return 0;
}
