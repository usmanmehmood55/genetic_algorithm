#include <stdio.h>
#include <time.h>
#include "genetic_algorithm.h"
#include "app_init.h"

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
    uint16_t offspring_count;
    genome_t target;
    bool is_args_valid = app_init(argc, argv, &target, &offspring_count);
    if (is_args_valid == false) { return -EINVAL; }

    genome_t parents[2];
    parents[0] = genome_init(target.length);
    parents[1] = genome_init(target.length);

    genome_t offsprings[offspring_count];
    for (uint16_t i = 0U; i < offspring_count; i++)
    {
        offsprings[i] = genome_init(target.length);
    }

    uint64_t iterations = 0;
    clock_t start_time = clock(); // for profiling time taken to converge

    while (true)
    {
        // create offsprings
        for (uint16_t i = 0U; i < offspring_count; i++)
        {
            genomes_mate(&target, &parents[0], &parents[1], &offsprings[i]);
        }

        genomes_sort_by_fitness(offsprings, offspring_count);

        // healthiest two offsprings become parents
        genome_copy(&parents[0], &offsprings[0]);
        genome_copy(&parents[1], &offsprings[1]);
        genome_print(parents[0]);

        // break, if convergence
        if ((parents[0].fitness == 0) || (iterations == UINT64_MAX))
        {
            break;
        }

        iterations++;
    }

    clock_t end_time = clock();
    clock_t time_taken = (end_time - start_time) * (clock_t)((double)1000.0 / (double)CLOCKS_PER_SEC); // In seconds

    double some = (double)time_taken / (double)iterations;

    (void)printf("\r\nConvergence Achieved!\n");
    (void)printf("\rIterations:      %llu\n", iterations);
    (void)printf("\rTime taken:      %lu milliseconds\n", time_taken);
    (void)printf("\rIteration time:  %f msec per iter\n", some);

    // cleanup memory
    genome_destroy(&parents[0]);
    genome_destroy(&parents[1]);
    for (uint16_t i = 0U; i < offspring_count; i++)
    {
        genome_destroy(&offsprings[i]);
    }

    return 0;
}