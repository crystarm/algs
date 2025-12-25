import sys


def main():
    d = sys.stdin.read().split()
    if not d:
        return

    it = iter(d)
    h = int(next(it))
    n = int(next(it))

    a = [int(x) for x in it]

    def ck(w):
        c = 0
        s = 0
        for x in a:
            s += x
            if s >= w:
                c += 1
                s = 0
                if c >= h:
                    return True
        return False

    l = 1
    r = sum(a)
    ans = 0

    while l <= r:
        m = (l + r) // 2
        if m == 0:
            l = 1
            continue

        if ck(m):
            ans = m
            l = m + 1
        else:
            r = m - 1

    print(ans)


if __name__ == "__main__":
    main()
