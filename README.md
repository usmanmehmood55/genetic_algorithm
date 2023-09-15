# Genetic Algorithm

This project is an implementation of a genetic algorithm in C. The application was 
initially based on this [article by Geeks for Geeks](https://www.geeksforgeeks.org/genetic-algorithms/), 
but has now been developed and changed enough to be more than a mere port from 
their C++ implementation into C. The subsequent paragraph however is straight from 
the above mentioned article. 

[![Build and Test](https://github.com/usmanmehmood55/genetic_algorithm/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/usmanmehmood55/genetic_algorithm/actions/workflows/build_and_test.yml)
[![codecov](https://codecov.io/gh/usmanmehmood55/genetic_algorithm/graph/badge.svg?token=OXCFJ9W6EN)](https://codecov.io/gh/usmanmehmood55/genetic_algorithm)

- [Genetic Algorithm](#genetic-algorithm)
  - [What is a Genetic Algorithm](#what-is-a-genetic-algorithm)
  - [My Implementation](#my-implementation)
    - [Genome Initializations](#genome-initializations)
    - [Genome Mating](#genome-mating)
    - [Fitness Calculation](#fitness-calculation)
    - [Survival of the Fittest](#survival-of-the-fittest)
    - [Generational Evolution and Convergence](#generational-evolution-and-convergence)
  - [Notes](#notes)
  - [Sample Output](#sample-output)
  - [Setup](#setup)

## What is a Genetic Algorithm

Genetic Algorithms(GAs) are adaptive heuristic search algorithms that belong to the 
larger part of evolutionary algorithms. Genetic algorithms are based on the ideas 
of natural selection and genetics. These are intelligent exploitation of random  
search provided with historical data to direct the search into the region of better 
performance in solution space. They are commonly used to generate high-quality 
solutions for optimization problems and search problems.

Genetic algorithms simulate the process of natural selection which means those 
species who can adapt to changes in their environment are able to survive and 
reproduce and go to next generation. In simple words, they simulate “survival of 
the fittest” among individual of consecutive generation for solving a problem. Each 
generation consist of a population of individuals and each individual represents a 
point in search space and possible solution. Each individual is represented as a 
string of character/integer/float/bits. This string is analogous to the Chromosome.

## My Implementation

A singular gene, is represented as a character. 
```c
typedef char gene_t;
```

A genome being a collection of genes, is represented as this structure.
```c
typedef struct genome_t
{
    gene_t   * genes;
    uint16_t   length;
    int        fitness;
} genome_t;
```

The application expects a string argument that it takes as its target, and the 
number of offspring to generate. The string must only contain the characters 
present in the gene pool, which is is defined in 
[`genetic_algorithm_utils.h`](genetic_algorithm_utils/genetic_algorithm_utils.h).
```c
#define GENE_POOL "!@#$^&*()_-=+,.;:'/\\\"{}[]<>? 1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
```

### Genome Initializations

After initializing the target genome, two new parent genomes, and `n` number of 
offspring genomes are initialized. Their genes are selected randomly from the gene 
pool. This is done in the `genome_init()` function. 
```c
genome_t parents[2];
parents[0] = genome_init(target.length);
parents[1] = genome_init(target.length);

genome_t offsprings[offspring_count];
for (uint16_t i = 0U; i < offspring_count; i++)
{
    offsprings[i] = genome_init(target.length);
}
```

### Genome Mating

The algorithm starts by mating the two parent genomes to create all the offspring 
genomes. 
```c
for (uint16_t i = 0U; i < offspring_count; i++)
{
    genomes_mate(&target, &parents[0], &parents[1], &offsprings[i]);
}
```

Mating involves two steps.
1. Joining the parents over a random crossover point
2. Performing a slight mutation

Take these two parents, of size 7:
```
+-----------+---+---+---+---+---+---+---+
| index     | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|-----------|---|---|---|---|---|---|---|
| parent 1  | a | b | c | d | e | f | g |
| parent 2  | h | i | j | k | l | m | n |
+-----------+---+---+---+---+---+---+---+
```

If the random crossover point is 3, the resulting offspring will look like this
```
+-----------+---+---+---+---+---+---+---+
| index     | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|-----------|---|---|---|---|---|---|---|
| parent 1  | a | b | c | d |   |   |   |
| parent 2  |   |   |   |   | l | m | n |
|-----------|---|---|---|---|---|---|---|
| offspring | a | b | c | d | l | m | n |
+-----------+---+---+---+---+---+---+---+
```

However to prevent parent 1 from always contributing to the first `n` genes and 
parent 2 to the remaining `n-length`, their sequence is randomly (should be 50/50) 
selected so with the same crossover point at 3, this can also happen
```
+-----------+---+---+---+---+---+---+---+
| index     | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|-----------|---|---|---|---|---|---|---|
| parent 1  |   |   |   |   | e | f | g |
| parent 2  | h | i | j | k |   |   |   |
|-----------|---|---|---|---|---|---|---|
| offspring | h | i | j | k | e | f | g |
+-----------+---+---+---+---+---+---+---+
```

After joining, a mutation can (at a 50/50 chance) be performed at a random index on 
the offspring. This is done to prevent the evolution from being stuck in a local 
maxima.
```
+-----------+---+---+---+---+---+---+---+
| index     | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|-----------|---|---|---|---|---|---|---|
| offspring | a | b | c | d | l | m | n |
| mutation  |   | X |   |   |   |   |   |
|-----------|---|---|---|---|---|---|---|
| mutated   | a | X | c | d | l | m | n |
+-----------+---+---+---+---+---+---+---+
```

### Fitness Calculation

The fitness of a genome is being calculated based on these parameters.
1. ~~How many genes in this are also present in the target, regardless of the order.~~
2. ~~How many genes in this are NOT present in the target.~~
3. How many genes match the target exactly in order.

Naturally, a heavy bias is given to genes that match with the target in order.

Ideally a genome that EXACTLY matches the target should have a fitness score of 0 
and the less it matches, the more negative its fitness should become. 

Currently the fitness calculation is anything but optimized, but I want to keep it 
that way for ease of understanding and debugging. It is done inside the `genome_mate()`
function when an offspring is created.
```c
p_offspring->fitness = genome_calculate_fitness(p_target->genes, p_offspring->genes, length);
```

### Survival of the Fittest

After creating an `n` number of offspring, they are sorted by ascending fitness. 
This is done using the `genomes_sort_by_fitness()` function which performs a bubble 
sort.

The two fittest offspring are then declared as the new parents, by copying them to 
the parent genomes using the `genome_copy()` function which performs a deep copy to 
preserve the references of the original parents.
```c
genome_copy(&parents[0], &offsprings[0]);
genome_copy(&parents[1], &offsprings[1]);
```

### Generational Evolution and Convergence

As the algorithm iterates, the genomes closer to the target keep getting selected 
to be parents and the resulting offspring keep getting closer to the target with 
slight mutations every time to help them converge.

Convergence is achieved when the healthiest parent has a fitness score of 0, i.e., 
it matches the target exactly. There is also an upper bound to number of iterations 
in case the algorithm never converges.
```c
if ((parents[0].fitness == 0) || (iterations == UINT64_MAX))
{
    (void)printf("\rConvergence Achieved!\n");
    break;
}
```

## Notes

Some observations that can be made about this algorithm:
- Bigger the target, the more iterations it takes to converge
- If a target is too big, it fails to converge (within reasonable limits)
- Larger the offspring count, the less iterations it takes to converge
- Larger the offspring count, the longer a single iteration lasts
- If the offspring count is too small, it fails to converge

It is also noticeable that in some cases having a smaller offspring count leads to 
convergence quicker due to more iterations, than having a large offspring count 
leading to less in number, but more time consuming iterations.

## Sample Output

```bash
genetic_algorithm> ./build/genetic_algorithm.exe "Hi, my name is Usman. :)" 1000

(-23) : "BkGd0 +gh4,KSpX2lTuJfIO"
(-21) : "BkGd0 +gh4,KS UKf_j?nEH"
(-20) : "BkGd0 +gh4,KS Usf_j?nEH"
(-19) : "B,Gd0 +gh4,KS Usf_j?nEH"
(-17) : "B, d0 +gh4,KS Usm_j?nEH"
(-16) : "B, d0 +gh4 KS Usm_j?nEH"
(-16) : "B, d0 +gh4 KS Usm_j?nEH"
(-15) : "B, m0 +gh4 KS Usm_j?nEH"
(-14) : "B, m0 +gh4 KS Usmaj?nEH"
(-13) : "B, m0 +gh4 KS Usmaj.nEH"
(-12) : "B, m0 +ah4 KY Usmaj.nEH"
(-11) : "B, m0 +ah4 Ks Usmaj.nEH"
(-10) : "B, m0 nah4 Ks Usmaj.nEH"
( -9) : "HB, m0 nah4 Ks Usmaj.nEH"
( -8) : "HB, m0 nahe Ks Usmaj.nEH"
( -7) : "HB, m4 nahe is Usmaj.nEH"
( -7) : "HB, m0 nahe is Usmaj.nBH"
( -6) : "Hi, m0 nahe is Usma'.nEH"
( -5) : "Hi, my nahe is Usma'.nEH"
( -4) : "Hi, my nahe is Usma'.n:H"
( -3) : "Hi, my nahe is Usma'. :H"
( -2) : "Hi, my nahe is Usman. :H"
( -1) : "Hi, my name is Usman. :H"
( -1) : "Hi, my name is Usman. :H"
( -1) : "Hi, my name is Usman. :H"
(  0) : "Hi, my name is Usman. :)"

Convergence Achieved!
Iterations:      25
Time taken:      56 milliseconds
Iteration time:  2.240000 msec per iter
```

## Setup

Following tools are required to compile
- [CMake](https://cmake.org/)
- [Ninja](https://ninja-build.org/)
- [GCC](https://gcc.gnu.org/)

In the repository directory, use CMake and Ninja to build
```bash
cmake -GNinja -Bbuild
ninja -C build
```

To make a `Debug` build without optimizations,
```bash
cmake -GNinja -Bbuild -DCMAKE_BUILD_TYPE=Debug
ninja -C build
```

To make a `Test` build with unit tests and coverage data,
```bash
cmake -GNinja -Bbuild -DCMAKE_BUILD_TYPE=Test
ninja -C build
```

To run the application
```bash
./build/genetic_algorithm.exe "Your test string" 1000
```