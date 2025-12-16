# Genetic Algorithm for Dynamic Optimization

## Overview

This repository contains a **C++ implementation of a Genetic Algorithm (GA)** designed to solve a constrained dynamic optimization problem.

The core objective is to divide a positive real number (N) into **12 unequal positive real values** such that the sum of these values equals (N). Instead of relying on closed-form mathematical solutions, the problem is reformulated as a multi-variable optimization task and solved using evolutionary computation techniques.

This project was originally developed as part of an academic course on **Dynamic Optimization** and later refined for research-oriented presentation and reproducibility.

---

## Problem Formulation

Given a constant N, find:

x1 + x2 + ... + x12 = N

subject to:

* xi > 0
* values are non-uniform

Each candidate solution is represented as a chromosome of 12 real-valued genes.

---

## Genetic Algorithm Design

### Representation

* **Chromosome**: Array of 12 floating-point values
* **Gene bounds**: (0.01 \le x_i \le N/2)

### Fitness Function

The fitness function is defined as:

Fitness = 1 / (1 + |N - (x1 + x2 + ... + x12)|)

This formulation transforms the problem into a minimization of absolute error while avoiding division-by-zero issues.

---

### Evolutionary Operators

#### Selection

* Roulette Wheel Selection based on relative fitness

#### Crossover

* Single-point crossover
* Genes on the left side of the crossover point are swapped

#### Mutation

* Gene value replacement with a new random value
* Chosen instead of gene swapping to introduce numerical diversity

#### Elitism

* Best individual from the previous generation is preserved
* Prevents loss of high-quality solutions

---

## Implementation Details

* Language: **C++**
* Population-based evolutionary search
* Floating-point gene encoding
* Execution time measured using `std::chrono`
* Modular structure for extensibility and experimentation

Key parameters:

* Population Size
* Number of Generations
* Crossover Rate
* Mutation Rate

(All parameters can be adjusted directly in the source code.)

---

## How to Run

### Requirements

* C++ compiler (GCC / MSVC / Clang)

### Compile

```bash
g++ genetic_algorithm.cpp -o ga
```

### Run

```bash
./ga
```

The program outputs:

* Best chromosome found
* Individual gene values
* Fitness score
* Execution time

---

## Applications

This approach is applicable to scenarios requiring **controlled randomness under constraints**, including:

* Liquidity distribution in early-stage P2P markets
* Resource allocation problems
* Numerical approximation under equality constraints

Genetic Algorithms provide robustness against pattern exploitation compared to deterministic methods.

---

## Research Context

This project is inspired by prior research on **informed population initialization** in Genetic Algorithms, particularly work applying clustering techniques (e.g., k-means) to improve convergence speed in large-scale optimization problems such as the Traveling Salesman Problem.

---

## References

1. G. M. et al., "A genetic algorithm with k-means clustering initialization for the Traveling Salesman Problem", *Computers & Operations Research*, 2007.

---

## Author

**Milad Purahmad**
M.Sc. Applicant – Optimization & Algorithms

---

## License

This project is shared for academic and educational purposes.
