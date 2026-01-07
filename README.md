[![build](https://github.com/crystarm/algs/actions/workflows/build.yml/badge.svg)](https://github.com/crystarm/algs/actions/workflows/build.yml)
![last commit](https://img.shields.io/github/last-commit/crystarm/algs)
![repo size](https://img.shields.io/github/repo-size/crystarm/algs)
[![lines of code](https://img.shields.io/endpoint?url=https%3A%2F%2Ftokei.kojix2.net%2Fbadge%2Fgithub%2Fcrystarm%2Falgs%2Flines)](https://tokei.kojix2.net/github/crystarm/algs)

## Note on usage

Important! No license is provided; all rights reserved. That means the code here is protected by default copyright rules: you may read it, study it, and use it as a reference, but **please do not copy or reuse complete solutions directly**! It's a matter of ethics.
It’s published **for learning and reference only**, also as a showcase of my approach, not a bundle of copy-paste ready answers.

I only add solutions that I consider **highly above average or hard in difficulty** or personally interesting, i.e. things that taught me something non-trivial. This is **not** a solution graveyard. : - )

## Highlights

Here are the most, in my opinion, notable solutions:

### ACMP

- **0435** - reachable-state enumeration + interning (dedup) + Chinese remainder theorem + subset sum + NFA -> DFA (subset construction) + Miller-Rabin primality test.  
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

## Problem sources

As you might have guessed, most of the problems in this repository come from two major Russian competitive programming training platforms:

- [**ACMP**](https://acmp.ru/) - it’s a long-running site with a huge archive of problems. Alongside solid classic tasks, there are plenty of quirky and non-standard problems - sometimes with unusual constraints, odd formulations, or obscurity in the sense of being more about decoding the statement and handling edge cases than applying a clean textbook algorithm. The site is maintained by Sergey N. Belyaev (listed as the author/admin in ACMP materials) and is associated with the Krasnoyarsk Krai Palace of Pioneers and Schoolchildren.

- [**Yandex CodeRun**](https://coderun.yandex.ru/) - it’s Yandex’s developer training platform: an online problem catalog and curated selections to practice programming across multiple tracks (e.g., backend, frontend, mobile, ML/analytics). It also has community features and periodic challenges/events. The platform is operated by Yandex LLC (ООО «Яндекс»).
