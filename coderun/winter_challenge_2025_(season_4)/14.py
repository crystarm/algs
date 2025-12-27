# number theory + combinatorics

import sys

try:
    sys.set_int_max_str_digits(20000)
except AttributeError:
    pass

MOD = 10**9 + 7


def solve():
    input_data = sys.stdin.read().split()
    if not input_data:
        return

    a = int(input_data[0])
    q = int(input_data[1])
    L = int(input_data[2])
    R = int(input_data[3])

    N = R - L + 1

    if a == 0:
        print(0)
        return

    if q == 0:
        if L <= 0 <= R:
            cnt_a = 1
            cnt_0 = N - 1
            valid_denominators = (2 * cnt_a * cnt_0) % MOD
            ans = (valid_denominators * (N % MOD) * (N % MOD)) % MOD
            print(ans)
        else:
            print(0)
        return

    if abs(q) == 1:
        if q == 1:
            print(0)
        else:
            if L % 2 == 0:
                cnt_even = (N + 1) // 2
                cnt_odd = N // 2
            else:
                cnt_even = N // 2
                cnt_odd = (N + 1) // 2

            valid_denominators = (2 * cnt_even * cnt_odd) % MOD
            ans = (valid_denominators * (N % MOD) * (N % MOD)) % MOD
            print(ans)
        return

    N_mod = N % MOD

    term_zero_num = (N_mod * (N_mod * N_mod - N_mod + MOD)) % MOD

    C0 = (2 * (N_mod * N_mod + N_mod)) % MOD
    C1 = (2 * (2 * N_mod + 1)) % MOD
    C2 = 2

    inv2 = pow(2, MOD - 2, MOD)
    inv6 = pow(6, MOD - 2, MOD)

    def get_s1(n):
        n %= MOD
        return n * (n + 1) % MOD * inv2 % MOD

    def get_s2(n):
        n %= MOD
        return n * (n + 1) % MOD * (2 * n + 1) % MOD * inv6 % MOD

    total_sum = 0
    M = N - 1
    l = 1

    while l <= M:
        K = M // l
        if K == 0:
            break
        r = M // K

        count_d = (r - l + 1) % MOD
        sum_d = (get_s1(r) - get_s1(l - 1) + MOD) % MOD
        sum_d2 = (get_s2(r) - get_s2(l - 1) + MOD) % MOD

        term0 = C0 * (K % MOD) % MOD * count_d % MOD

        val_s1_k = get_s1(K)
        term1 = C1 * val_s1_k % MOD * sum_d % MOD

        val_s2_k = get_s2(K)
        term2 = C2 * val_s2_k % MOD * sum_d2 % MOD

        block_val = (term0 - term1 + term2 + MOD) % MOD
        total_sum = (total_sum + block_val) % MOD

        l = r + 1

    if q == -2:
        cnt_odd = (M + 1) // 2

        if cnt_odd > 0:
            K_odd = cnt_odd
            sum_1 = K_odd % MOD
            sum_x = (K_odd % MOD) * (K_odd % MOD) % MOD

            limit = K_odd - 1
            s1_lim = get_s1(limit)
            s2_lim = get_s2(limit)
            sum_x2 = (4 * s2_lim + 4 * s1_lim + K_odd) % MOD

            sum_w_odd = (C0 * sum_1 - C1 * sum_x + C2 * sum_x2) % MOD
            sum_w_odd = (sum_w_odd + MOD) % MOD

            if N >= 2:
                sum_w_odd = (sum_w_odd - 4 + MOD) % MOD

            total_sum = (total_sum + sum_w_odd) % MOD

    ans = (term_zero_num + total_sum) % MOD
    print((ans + MOD) % MOD)


if __name__ == "__main__":
    solve()
