[![build](https://github.com/crystarm/algs/actions/workflows/build.yml/badge.svg)](https://github.com/crystarm/algs/actions/workflows/build.yml)
![last commit](https://img.shields.io/github/last-commit/crystarm/algs)
![repo size](https://img.shields.io/github/repo-size/crystarm/algs)
![tasks](https://img.shields.io/badge/dynamic/json?url=https://raw.githubusercontent.com/crystarm/algs/master/meta/stats.json&query=$.tasks_total&label=tasks)

## Note on usage

Important! No license is provided; all rights reserved. That means the code here is protected by default copyright rules: you may read it, study it, and use it as a reference, but **please do not copy or reuse complete solutions directly!** It's a matter of ethics.
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
