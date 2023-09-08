#include <stdio.h>
#include "genetic_algorithm.h"

#define OFFSPRING_COUNT 500U

/**
 * @brief This is a very crude implementation of a genetic algorithm. 
 * 
 * @param argc number of arguments, only the first one is considered
 * @param argv pointer to arguments list
 * 
 * @return int return 0 on success, -EINVAL if no arguments
 */
int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        (void)printf("\rInvalid args\n");
        return -EINVAL;
    }

    srand(time(NULL)); // required for random number generation

    const genome_t target = genome_target_init(argv[1]);
    int target_size = target.length;

    genome_t parents[2]; 
    parents[0] = genome_create(target_size);
    parents[1] = genome_create(target_size);

    genome_t offsprings[OFFSPRING_COUNT];
    for (uint16_t i = 0U; i < OFFSPRING_COUNT; i++)
    {
        offsprings[i] = genome_create(target_size);
    }

    uint64_t iterations = 0;
    while (true)
    {
        // create offsprings
        for (uint16_t i = 0U; i < OFFSPRING_COUNT; i++)
        {
            genomes_mate(&target, &parents[0], &parents[1], &offsprings[i]);
        }

        genomes_sort_by_fitness(offsprings, OFFSPRING_COUNT);

        // healthiest two offsprings become parents
        genome_copy(&parents[0], &offsprings[0]);
        genome_copy(&parents[1], &offsprings[1]);
        genomes_print(parents[0], parents[1]);

        // break, if convergence
        if ((parents[0].fitness == 0) && (parents[1].fitness == 0))
        {
            (void)printf("\rConvergence Achieved!\n");
            break;
        }

        iterations++;
    }

    (void)printf("\rNumber of Iterations: %llu\n", iterations);

    // cleanup memory
    genome_destroy(&parents[0]);
    genome_destroy(&parents[1]);
    for (uint16_t i = 0U; i < OFFSPRING_COUNT; i++)
    {
        genome_destroy(&offsprings[i]);
    }

    return 0;
}