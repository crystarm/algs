// https://acmp.ru/index.asp?main=task&id_task=884
// sliding window + bitmasks

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define A_BITS 13
#define B_BITS 13
#define A_STATES (1u << A_BITS)
#define B_STATES (1u << B_BITS)
#define WORDS (B_STATES / 64u)
#define FULLA (A_STATES - 1u)
#define FULLB (B_STATES - 1u)

#define POOL_SIZE 1594323

static uint64_t bad[A_STATES][WORDS];
static uint64_t pat0[B_BITS][WORDS];
static uint32_t off[B_STATES + 1];
static uint16_t pool[POOL_SIZE];

static inline void build_patterns(void)
{
    for (unsigned m = 0; m < B_STATES; ++m)
    {
        unsigned w = m >> 6;
        unsigned b = m & 63u;
        uint64_t bit = 1ULL << b;
        for (unsigned i = 0; i < B_BITS; ++i) if (((m >> i) & 1u) == 0u) pat0[i][w] |= bit;
    }
}

static inline void build_pool(void)
{
    off[0] = 0;
    for (unsigned b = 0; b < B_STATES; ++b)
    {
        unsigned zeros = B_BITS - (unsigned)__builtin_popcount(b);
        off[b + 1] = off[b] + (1u << zeros);
    }

    uint32_t idx = 0;
    for (unsigned b = 0; b < B_STATES; ++b)
    {
        unsigned c = (~b) & FULLB;
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
    uint64_t *row = bad[wa];

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

    if (wb == 0u) { for (unsigned w = 0; w < WORDS; ++w) row[w] = ~0ULL; return; }

    uint64_t tmp[WORDS];
    unsigned first = (unsigned)__builtin_ctz(wb);
    memcpy(tmp, pat0[first], sizeof(tmp));

    unsigned rest = wb & (wb - 1u);
    while (rest)
    {
        unsigned i = (unsigned)__builtin_ctz(rest);
        rest &= rest - 1u;
        uint64_t *p = pat0[i];
        for (unsigned w = 0; w < WORDS; ++w) tmp[w] &= p[w];

    }

    for (unsigned w = 0; w < WORDS; ++w) row[w] |= tmp[w];

}

static inline void sos_or_transform(void)
{
    for (unsigned bit = 0; bit < A_BITS; ++bit)
    {
        unsigned b = 1u << bit;
        for (unsigned m = 0; m < A_STATES; ++m)
        {
            if (m & b)
            {
                uint64_t *dst = bad[m];
                uint64_t *src = bad[m ^ b];
                for (unsigned w = 0; w < WORDS; ++w) dst[w] |= src[w];
            }
        }
    }
}

int main(void)
{
    int K;
    static char s[100000 + 5];

    if (scanf("%d", &K) != 1) return 0;
    if (scanf("%100000s", s) != 1) return 0;

    int L = (int)strlen(s);

    build_patterns();
    build_pool();

    int cnt[26];
    memset(cnt, 0, sizeof(cnt));
    uint32_t mask = 0u;

    for (int i = 0; i < K; ++i)
    {
        int c = s[i] - 'A';
        if (cnt[c]++ == 0) mask |= 1u << (unsigned)c;
    }

    int windows = L - K + 1;
    for (int start = 0; start < windows; ++start)
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
    for (unsigned pc = 0; pc <= B_BITS; ++pc)
        for (unsigned tb = 0; tb < B_STATES; ++tb)
            if ((unsigned)__builtin_popcount(tb) == pc) tb_order[pos++] = (uint16_t)tb;

    int bestN = 27;
    uint32_t bestTa = 0, bestTb = 0;

    for (uint32_t ta = 0; ta < A_STATES; ++ta)
    {
        int pa = __builtin_popcount(ta);
        if (pa >= bestN) continue;

        uint32_t x = (~ta) & FULLA;
        uint64_t *inv = bad[x];

        for (unsigned i = 0; i < B_STATES; ++i)
        {
            uint32_t tb = tb_order[i];
            int pb = __builtin_popcount(tb);
            if (pa + pb >= bestN) break;

            if ((inv[tb >> 6] & (1ULL << (tb & 63u))) == 0u)
            {
                bestN = pa + pb;
                bestTa = ta;
                bestTb = tb;
                break;
            }
        }
    }

    printf("%d\n", bestN);
    for (unsigned i = 0; i < A_BITS; ++i)
    {
        if ((bestTa >> i) & 1u)
        { putchar('A' + (int)i); putchar('\n'); }
    }
    for (unsigned i = 0; i < B_BITS; ++i)
    {
        if ((bestTb >> i) & 1u)
        { putchar('A' + (int)(A_BITS + i)); putchar('\n'); }
    }

    return 0;
}
