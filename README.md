# Project Documentation: Counting Homomorphisms, Embeddings, Subgraphs, and k-Matchings on CFI Graphs
Falco Wolf and Christoph Greger

## Theoretical Background
**CFI graphs** (named after Cai, F"urer, and Immerman) are graph constructions used to expose limitations of certain logic-based graph invariants. We denote by `CFI(G)` the CFI graph constructed from a base graph G, and by `CFI'(G)` the variant in which exactly one edge is inverted (see [Curticapean2023, Roberson2022] for details).  
**Graph homomorphisms** are vertex maps `f : V(G) -> V(H)` between graphs G and H such that `(u,v) ∈ E(G)` implies `(f(u),f(v)) ∈ E(H)`. The set of homomorphisms from a *pattern* graph H to an *input* graph G is denoted `HOM(H,G)`; we also write the same symbol for its cardinality.  
**Embeddings** are injective graph homomorphisms; we write `EMB(H,G)`.  
The number of **subgraphs** equals `EMB(H,G)` divided by the number of automorphisms of H; we denote it by `SUB(H,G)`.  
A **k-matching** is a set of k edges in a graph G such that no two edges share a vertex.

## Software Architecture
- **Language:** C++23  
- **Libraries:** Boost (thread pools and `int256`) and `nauty` (graph canonisation and the generation of all non-isomorphic graphs of a given order and size)

## Features
- Generation of CFI graphs based on an arbitrary graph G
- Counting homomorphisms
- Counting embeddings
- Counting subgraphs
- Counting k-matchings (for k ≤ 9 using pre-computed spasms)

All operations can be performed on ordinary graphs as well as on their corresponding `CFI(G)` and `CFI'(G)` variants.

## Methods
In what follows we focus on `HOM(H,CFI(G))`; every statement carries over verbatim to `HOM(H,CFI'(G))`.

- `HOM(H,G)` is computed by a straightforward back-tracking algorithm. Besides obtaining the count, it is also possible to iterate over the set of homomorphisms.
- `HOM(H,CFI(G))` is obtained by enumerating all homomorphisms from H to G and, for each of them, setting up a linear system of equations as in [Roberson2022]. Summing the numbers of solutions over all systems yields the desired count.
- `EMB(H,G)` and `EMB(H,CFI(G))` are reduced to homomorphism counting via the spasm construction described in [Curticapean2017]. We generate all quotient graphs explicitly, but [Curticapean2017] also outlines a significantly faster matrix-based approach for certain graph classes.
- `SUB(H,G)` and `SUB(H,CFI(G))` are obtained by dividing `EMB(H,G)` (`EMB(H,CFI(G))`) by `EMB(H,H)`, the number of automorphisms of H.
- k-matchings are reduced to subgraph counting via the standard reduction. With pre-computed spasms the implementation supports k < 10; the matrix approach from [Curticapean2017] should extend this to k < 13, but this optimisation has not yet been fully implemented.

## Frontend
A web front-end is available at <https://github.com/FalcoWolff/Graph-homomorphisms-web>; it is still under active development. A Docker image bundling front- and back-end can be found on Docker Hub under `christophgreger/counthomcfi`.

## References
1. R. Curticapean, H. Dell, and D. Marx.  
   *Homomorphisms Are a Good Basis for Counting Small Subgraphs*.  
   arXiv:1705.01595, 2017.

2. R. Curticapean.  
   *Count on CFI Graphs for #P-Hardness*.  
   arXiv:2305.04767, 2023.

3. D. E. Roberson.  
   *Oddomorphisms and Homomorphism Indistinguishability over Graphs of Bounded Degree*.  
   arXiv:2206.10321, 2022.
