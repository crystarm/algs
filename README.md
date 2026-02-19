[![build](https://github.com/crystarm/algs/actions/workflows/build.yml/badge.svg)](https://github.com/crystarm/algs/actions/workflows/build.yml)
![last commit](https://img.shields.io/github/last-commit/crystarm/algs)
![tasks](https://img.shields.io/badge/dynamic/json?url=https://raw.githubusercontent.com/crystarm/algs/master/.github/meta/stats.lson&query=$.tasks_total&label=tasks)
[![timus rank](https://img.shields.io/badge/dynamic/json?url=https://raw.githubusercontent.com/crystarm/algs/master/.github/meta/stats.lson&query=$.timus.rank_by_rating_text&label=timus%20rank&color=9400d3)](https://acm.timus.ru/author.aspx?id=404938)
[![acmp rank](https://img.shields.io/badge/dynamic/json?url=https://raw.githubusercontent.com/crystarm/algs/master/.github/meta/stats.lson&query=$.acmp.rank_by_rating_text&label=acmp%20rank&color=9400d3)](https://acmp.ru/index.asp?main=user&id=541249)


## Note on usage

**Important! Check [LICENSE](LICENSE)! All rights reserved, no permission is granted!** The code here is protected by default copyright rules: you may view and read it for personal study, but **please do not copy or reuse complete solutions directly! It's a matter of ethics.**
It’s published **for learning and reference only**, also as a showcase of my approach, not a bundle of copy-paste ready answers.

I only add solutions that I consider **highly above average or hard in difficulty** or personally interesting, i.e. things that taught me something non-trivial. This is **not** a solution graveyard. : - )

## Highlights

Here are the most, in my opinion, notable solutions:

### ACMP

- **0435** - reachable-state enumeration + interning (dedup) + Chinese remainder theorem + subset sum + NFA → DFA (subset construction) + Miller-Rabin primality test.  
  ([solution](acmp/0435.cpp) · [statement](https://acmp.ru/index.asp?main=task&id_task=435))

- **0932** - centroid decomposition + combinatorics (counting/aggregation over a tree with centroid splits).  
  ([solution](acmp/0932.cpp) · [statement](https://acmp.ru/index.asp?main=task&id_task=932))

- **1394** - assignment problem + Hungarian algorithm + augmenting paths + primal-dual potentials.  
  ([solution](acmp/1394.c) · [statement](https://acmp.ru/asp/do/index.asp?main=task&id_course=2&id_section=32&id_topic=57&id_problem=1050))

- **0866** - st-numbering + graph biconnectivity + ear decomposition.  
  ([solution](acmp/0866.cpp) · [statement](https://acmp.ru/index.asp?main=task&id_task=866))

- **0981** - boolean expressions theory + functional bases (logic parsing + basis properties).  
  ([solution](acmp/0981.cpp) · [statement](https://acmp.ru/index.asp?main=task&id_task=981))

### Yandex CodeRun

- **0026** - caching + Belady (farthest next use) + heap.  
  ([solution](coderun/0026.cpp) · [statement](https://coderun.yandex.ru/problem/avto))

- **0117** - sweep line technique + prefix sums.  
  ([solution](coderun/0117.cpp) · [statement](https://coderun.yandex.ru/problem/security))

- **0567** - DSU + 4-connectivity on a grid + path compression (connected components).  
  ([solution](coderun/0567.c) · [statement](https://coderun.yandex.ru/problem/cat-search))

- **0046** - parity + minimax (game-style reasoning).  
  ([solution](coderun/0046.cs) · [statement](https://coderun.yandex.ru/problem/robots))

- **wc21** - computational geometry + enumeration / backtracking.  
  ([solution](coderun/wc21.c) · [statement](https://coderun.yandex.ru/selections/2025-winter-common/problems/energy-strike))

### Timus

- **1399** - Capacitated Vehicle Routing Problem (CVRP/VRP) + TSP on subsets (Held–Karp bitmask DP) + greedy heuristics.  
  ([solution](timus/1399.cpp) · [statement](https://acm.timus.ru/problem.aspx?space=1&num=1399))

- **2122** - combinatorics + hamming contribution trick + generating functions + NTT under 40961.  
  ([solution](timus/2122.cpp) · [statement](https://acm.timus.ru/problem.aspx?space=1&num=2122))

- **2042** - segment tree + lazy range assignment + Manacher on small buffer.  
  ([solution](timus/2042.cpp) · [statement](https://acm.timus.ru/problem.aspx?space=1&num=2042))

- **1829** - routing tables + LPM + bitmasks (IPv4 AND mask) + DFS traversal + rule equivalence checking.  
  ([solution](timus/1829.cpp) · [statement](https://acm.timus.ru/problem.aspx?space=1&num=1829))

- **1598** - DSA + finite fields + modular arithmetic + fast modular exponentiation + BSGS + crypto break via small parameters.  
  ([solution](timus/1598.cpp) · [statement](https://acm.timus.ru/problem.aspx?space=1&num=1598))

- **2196** - computational geometry + point-in-convex-polygon (O(log N)) + point-to-segment distance + Minkowski sum / offset (Steiner formula).  
  ([solution](timus/2196.cpp) · [statement](https://acm.timus.ru/problem.aspx?space=1&num=2196))

## Task index

<details><summary>All tasks</summary>

<!-- TASKS_TABLE_BEGIN -->
<!-- generated: do not edit by hand -->

| Source | ID | Lang | Solution | Statement |
|---|---:|---|---|---|
| ACMP | 0011 | C++ | [acmp/0011.cpp](acmp/0011.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=11) |
| ACMP | 0306 | C++ | [acmp/0306.cpp](acmp/0306.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=306) |
| ACMP | 0337 | C++ | [acmp/0337.cpp](acmp/0337.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=337) |
| ACMP | 0435 | C++ | [acmp/0435.cpp](acmp/0435.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=435) |
| ACMP | 0576 | C++ | [acmp/0576.cpp](acmp/0576.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=576) |
| ACMP | 0652 | C++ | [acmp/0652.cpp](acmp/0652.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=652) |
| ACMP | 0662 | C++ | [acmp/0662.cpp](acmp/0662.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=662) |
| ACMP | 0861 | C++ | [acmp/0861.cpp](acmp/0861.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=861) |
| ACMP | 0866 | C++ | [acmp/0866.cpp](acmp/0866.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=866) |
| ACMP | 0880 | C++ | [acmp/0880.cpp](acmp/0880.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=880) |
| ACMP | 0884 | C++ | [acmp/0884.cpp](acmp/0884.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=884) |
| ACMP | 0914 | C++ | [acmp/0914.cpp](acmp/0914.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=914) |
| ACMP | 0932 | C++ | [acmp/0932.cpp](acmp/0932.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=932) |
| ACMP | 0937 | C++ | [acmp/0937.cpp](acmp/0937.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=937) |
| ACMP | 0959 | C++ | [acmp/0959.cpp](acmp/0959.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=959) |
| ACMP | 0973 | C++ | [acmp/0973.cpp](acmp/0973.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=973) |
| ACMP | 0981 | C++ | [acmp/0981.cpp](acmp/0981.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=981) |
| ACMP | 1000 | C | [acmp/1000.c](acmp/1000.c) | [link](https://acmp.ru/index.asp?main=task&id_task=1000) |
| ACMP | 1394 | C | [acmp/1394.c](acmp/1394.c) | [link](https://acmp.ru/index.asp?main=task&id_task=1394) |
| ACMP | 1690 | C++ | [acmp/1690.cpp](acmp/1690.cpp) | [link](https://acmp.ru/index.asp?main=task&id_task=1690) |
| CodeRun | 0005 | C | [coderun/0005.c](coderun/0005.c) | [link](https://coderun.yandex.ru/catalog?search=5) |
| CodeRun | 0026 | C++ | [coderun/0026.cpp](coderun/0026.cpp) | [link](https://coderun.yandex.ru/catalog?search=26) |
| CodeRun | 0042 | C++ | [coderun/0042.cpp](coderun/0042.cpp) | [link](https://coderun.yandex.ru/catalog?search=42) |
| CodeRun | 0046 | C# | [coderun/0046.cs](coderun/0046.cs) | [link](https://coderun.yandex.ru/catalog?search=46) |
| CodeRun | 0047 | C++ | [coderun/0047.cpp](coderun/0047.cpp) | [link](https://coderun.yandex.ru/catalog?search=47) |
| CodeRun | 0049 | PHP | [coderun/0049.php](coderun/0049.php) | [link](https://coderun.yandex.ru/catalog?search=49) |
| CodeRun | 0078 | C | [coderun/0078.c](coderun/0078.c) | [link](https://coderun.yandex.ru/catalog?search=78) |
| CodeRun | 0113 | C++ | [coderun/0113.cpp](coderun/0113.cpp) | [link](https://coderun.yandex.ru/catalog?search=113) |
| CodeRun | 0115 | C++ | [coderun/0115.cpp](coderun/0115.cpp) | [link](https://coderun.yandex.ru/catalog?search=115) |
| CodeRun | 0117 | C++ | [coderun/0117.cpp](coderun/0117.cpp) | [link](https://coderun.yandex.ru/catalog?search=117) |
| CodeRun | 0153 | Python | [coderun/0153.py](coderun/0153.py) | [link](https://coderun.yandex.ru/catalog?search=153) |
| CodeRun | 0156 | C++ | [coderun/0156.cpp](coderun/0156.cpp) | [link](https://coderun.yandex.ru/catalog?search=156) |
| CodeRun | 0157 | C++ | [coderun/0157.cpp](coderun/0157.cpp) | [link](https://coderun.yandex.ru/catalog?search=157) |
| CodeRun | 0162 | C++ | [coderun/0162.cpp](coderun/0162.cpp) | [link](https://coderun.yandex.ru/catalog?search=162) |
| CodeRun | 0219 | C++ | [coderun/0219.cpp](coderun/0219.cpp) | [link](https://coderun.yandex.ru/catalog?search=219) |
| CodeRun | 0222 | C++ | [coderun/0222.cpp](coderun/0222.cpp) | [link](https://coderun.yandex.ru/catalog?search=222) |
| CodeRun | 0243 | C++ | [coderun/0243.cpp](coderun/0243.cpp) | [link](https://coderun.yandex.ru/catalog?search=243) |
| CodeRun | 0270 | C++ | [coderun/0270.cpp](coderun/0270.cpp) | [link](https://coderun.yandex.ru/catalog?search=270) |
| CodeRun | 0347 | SQL | [coderun/0347.sql](coderun/0347.sql) | [link](https://coderun.yandex.ru/catalog?search=347) |
| CodeRun | 0404 | PHP | [coderun/0404.php](coderun/0404.php) | [link](https://coderun.yandex.ru/catalog?search=404) |
| CodeRun | 0417 | PHP | [coderun/0417.php](coderun/0417.php) | [link](https://coderun.yandex.ru/catalog?search=417) |
| CodeRun | 0547 | C++ | [coderun/0547.cpp](coderun/0547.cpp) | [link](https://coderun.yandex.ru/catalog?search=547) |
| CodeRun | 0567 | C | [coderun/0567.c](coderun/0567.c) | [link](https://coderun.yandex.ru/catalog?search=567) |
| CodeRun | 7271 | C++ | [coderun/7271.cpp](coderun/7271.cpp) | [link](https://coderun.yandex.ru/catalog?search=7271) |
| CodeRun | 7507 | C++ | [coderun/7507.cpp](coderun/7507.cpp) | [link](https://coderun.yandex.ru/catalog?search=7507) |
| CodeRun | 7513 | C++ | [coderun/7513.cpp](coderun/7513.cpp) | [link](https://coderun.yandex.ru/catalog?search=7513) |
| CodeRun | 9012 | C++ | [coderun/9012.cpp](coderun/9012.cpp) | [link](https://coderun.yandex.ru/catalog?search=9012) |
| CodeRun | 9013 | Python | [coderun/9013.py](coderun/9013.py) | [link](https://coderun.yandex.ru/catalog?search=9013) |
| CodeRun | 9098 | C | [coderun/9098.c](coderun/9098.c) | [link](https://coderun.yandex.ru/catalog?search=9098) |
| CodeRun | 9325 | C++ | [coderun/9325.cpp](coderun/9325.cpp) | [link](https://coderun.yandex.ru/catalog?search=9325) |
| Timus | 1092 | C | [timus/1092.c](timus/1092.c) | [link](https://acm.timus.ru/problem.aspx?space=1&num=1092) |
| Timus | 1271 | C++ | [timus/1271.cpp](timus/1271.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=1271) |
| Timus | 1337 | C++ | [timus/1337.cpp](timus/1337.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=1337) |
| Timus | 1399 | C++ | [timus/1399.cpp](timus/1399.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=1399) |
| Timus | 1541 | C++ | [timus/1541.cpp](timus/1541.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=1541) |
| Timus | 1598 | C++ | [timus/1598.cpp](timus/1598.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=1598) |
| Timus | 1618 | C++ | [timus/1618.cpp](timus/1618.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=1618) |
| Timus | 1624 | C++ | [timus/1624.cpp](timus/1624.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=1624) |
| Timus | 1704 | C++ | [timus/1704.cpp](timus/1704.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=1704) |
| Timus | 1771 | C++ | [timus/1771.cpp](timus/1771.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=1771) |
| Timus | 1815 | C++ | [timus/1815.cpp](timus/1815.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=1815) |
| Timus | 1829 | C++ | [timus/1829.cpp](timus/1829.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=1829) |
| Timus | 1839 | C++ | [timus/1839.cpp](timus/1839.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=1839) |
| Timus | 2041 | C++ | [timus/2041.cpp](timus/2041.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=2041) |
| Timus | 2042 | C++ | [timus/2042.cpp](timus/2042.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=2042) |
| Timus | 2086 | C++ | [timus/2086.cpp](timus/2086.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=2086) |
| Timus | 2096 | C++ | [timus/2096.cpp](timus/2096.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=2096) |
| Timus | 2097 | C++ | [timus/2097.cpp](timus/2097.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=2097) |
| Timus | 2122 | C++ | [timus/2122.cpp](timus/2122.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=2122) |
| Timus | 2188 | C++ | [timus/2188.cpp](timus/2188.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=2188) |
| Timus | 2196 | C++ | [timus/2196.cpp](timus/2196.cpp) | [link](https://acm.timus.ru/problem.aspx?space=1&num=2196) |

<!-- TASKS_TABLE_END -->

</details>

## Problem sources

As you might have guessed, most of the problems in this repository come from two major Russian competitive programming training platforms:

- [**ACMP**](https://acmp.ru/) - it’s a long-running site with a huge archive of problems. Alongside solid classic tasks, there are plenty of quirky and non-standard problems - sometimes with unusual constraints, odd formulations, or obscurity in the sense of being more about decoding the statement and handling edge cases than applying a clean textbook algorithm. The site is maintained by **Sergey N. Belyaev** (listed as the author/admin in ACMP materials) and is associated with the **Krasnoyarsk Krai Palace of Pioneers and Schoolchildren**.

- [**Yandex CodeRun**](https://coderun.yandex.ru/) - it’s Yandex’s developer training platform: an online problem catalog and curated selections to practice programming across multiple tracks (e.g., backend, frontend, mobile, ML/analytics). It also has community features and periodic challenges/events. The platform is operated by **Yandex LLC (ООО «Яндекс»)**.

- [**Timus Online Judge**](https://acm.timus.ru/) - a classic ICPC-flavored problem archive and online judge. It hosts Internet versions of many contests held at **Ural Federal University (UrFU)** and includes problems from regional ICPC/NEERC-style contests (and related training events). The site is created and administered by students and graduates of UrFU.

## Languages

- **C/C++** - my primary languages. I use them at work, I enjoy them, and most competitive programming tasks are a natural fit for this toolset.
- **C#** - here mostly out of curiosity. I occasionally poke the .NET ecosystem/infrastructure just because I`m interested.
- **PHP** - work-driven: I deal with the PHP ecosystem from the *runtime/engine and extensions* side (C mostly). We run static analysis on the engine/addon code, triage findings, and do fuzzing. Sometimes I write some PHP to better understand how it behaves in practice.
- **Python** - the baseline language. It’s the quickest way to prototype, validate ideas, or write tiny utilities. I use it when I don`t wanna be bothered.

## Code style

This repository is mostly **competitive-programming style** code: consistent, fast to write/read under time pressure, and focused on the algorithm rather than ceremony.

What you’ll see a lot (generally in **C/C++** files):
- A small personal template: `#include <bits/stdc++.h>`, fast I/O, and a few tiny helpers/aliases.
- **Short, consistent naming**: `n, m, k`, `ans`, `g`, `dp`, `dist`, etc. The goal is to reduce visual noise. I don`t like long words. 
- **Shorthands/macros** like `rep`, `sz`, `all`, `pb`, `fi/se`, `rsr(...)` to compress boilerplate (looping, container ops, pair access). This is intentional and comes from a low-level mindset: make common patterns dense and recognizable. I don`t like long expressions.
- A deliberate trade-off: this style is great for contests and personal notes, but **I do not claim macros are best practice for production**.

## Build & CI

I keep a small CI pipeline that’s intentionally close to my setup. The goal is reproducibility and “works on my machine” parity - if it compiles here, it’ll compile on my box too.

- **Environment:** GitHub Actions runs everything inside a `debian:bookworm-slim` container.
- **C/C++ (LLVM Clang only):** every `*.cpp` is compiled as **C++20**, every `*.c` as **C11** (`-O2 -pipe`; C links with `-lm`). Each file is built standalone and outputs go into `build_ci/` - this catches missing headers, accidental dependencies, etc.
- **C# (Mono):** compiles every `*.cs` with `mcs` (`-optimize+`) into `build_ci_cs/`.
- **Python:** syntax/bytecode sanity via `pypy3 -m compileall`.
- **PHP:** lint via `php -l`.

This is a **build/lint sanity check**, not a full correctness test. Maybe one day in the future I will decide to organize full-fledged tests.
