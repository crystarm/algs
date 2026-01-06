//
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef long long ll;
typedef unsigned int ui;

#ifndef getchar_unlocked
#define getchar_unlocked getchar
#endif

static inline int read_char() { return getchar_unlocked(); }

static inline int read_ll(ll *out)
{
    int c = read_char();
    while (c != '-' && (c < '0' || c > '9')) { if (c == EOF) return 0; c = read_char(); }

    int s = 1;
    if (c == '-') { s = -1; c = read_char(); }

    ll x = 0;
    while (c >= '0' && c <= '9')
    { x = x * 10 + (ll)(c - '0'); c = read_char(); }

    *out = (ll)s * x;
    return 1;
}

struct rng { int L; ll mn; ll mx; };

static inline void append_str(char **buf, size_t *cap, size_t *len, const char *s, size_t sl)
{
    if (*len + sl + 1 > *cap)
    {
        size_t ncap = (*cap ? *cap : 1024);
        while (*len + sl + 1 > ncap) ncap <<= 1;
        char *nbuf = (char*)realloc(*buf, ncap);
        if (!nbuf) exit(0);
        *buf = nbuf;
        *cap = ncap;
    }

    memcpy(*buf + *len, s, sl);
    *len += sl;
    (*buf)[*len] = 0;
}

static inline void append_char(char **buf, size_t *cap, size_t *len, char c)
{
    if (*len + 2 > *cap)
    {
        size_t ncap = (*cap ? *cap : 1024);
        while (*len + 2 > ncap) ncap <<= 1;
        char *nbuf = (char*)realloc(*buf, ncap);
        if (!nbuf) exit(0);
        *buf = nbuf;
        *cap = ncap;
    }
    (*buf)[(*len)++] = c;
    (*buf)[*len] = 0;
}

int main()
{
    ll tll;
    if (!read_ll(&tll)) return 0;
    if (tll <= 0) return 0;

    int T = (int)tll;

    struct rng ranges[8];
    for (int i = 0; i < 8; ++i)
    {
        int L = i + 1;
        int bits = 7 * L;
        ll limit = (ll)1 << (bits - 1);
        ranges[i].L = L;
        ranges[i].mn = -limit;
        ranges[i].mx = limit - 1;
    }

    char *out = NULL;
    size_t cap = 0, len = 0;

    for (int tc = 0; tc < T; ++tc)
    {
        ll num;
        if (!read_ll(&num)) break;

        int L = 9;
        for (int i = 0; i < 8; ++i)
        {
            if (ranges[i].mn <= num && num <= ranges[i].mx)
            { L = ranges[i].L; break; }

        }

        char line[64];
        int nline = 0;

        if (L == 9)
        {
            uint64_t x = (uint64_t)num;
            nline = snprintf(line, sizeof(line), "ff%016llx", (unsigned long long)x);
        } else {
            uint64_t mask;
            if (L == 8) mask = ~0ULL;
            else mask = (1ULL << (8 * L)) - 1ULL;

            uint64_t raw = ((uint64_t)num) & mask;

            if (L > 1)
            {
                int shift = 8 * (L - 1);
                uint64_t msb = (raw >> shift) & 0xFFULL;

                int bit0 = 8 - L;

                ui or_mask = 0;
                for (int b = 0; b < L - 1; ++b) or_mask |= (1U << (7 - b));

                ui and_mask = (~(1U << bit0)) & 0xFFU;

                ui new_msb = ((ui)msb & and_mask) | or_mask;

                uint64_t low_mask = (shift == 0 ? 0ULL : ((1ULL << shift) - 1ULL));
                uint64_t low = raw & low_mask;
                raw = low | ((uint64_t)new_msb << shift);
            }

            nline = snprintf(line, sizeof(line), "%0*llx", L * 2, (unsigned long long)raw);
        }

        append_str(&out, &cap, &len, line, (size_t)nline);
        append_char(&out, &cap, &len, '\n');
    }

    if (out && len) fwrite(out, 1, len, stdout);

    free(out);
    return 0;
}
