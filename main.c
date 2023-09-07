#include <stdio.h>
#include "genetic_algorithm.h"

#define OFFSPRING_COUNT 10000U

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
        return -EINVAL;
    }

    // required for random number generation
    srand(time(NULL));

    const genome_t target = target_genome_init(argv[1]);
    int target_size = target.length;

    genome_t parents[2] = 
    {
        genome_create(target_size),
        genome_create(target_size),
    };

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
            offsprings[i].genes = mate(parents[0].genes, parents[1].genes, offsprings[i].genes, target_size);
        }

        // calculate fitness of offsprings
        for (uint16_t i = 0U; i < OFFSPRING_COUNT; i++)
        {
            offsprings[i].fitness = fitness_score(target.genes, offsprings[i].genes, offsprings[i].length);
        }

        // Sort offsprings by fitness, higher is better
        // This will be made into a separate function
        {
            for (uint16_t i = 0U; i < OFFSPRING_COUNT; i++)
            {
                for (uint16_t j = (i + 1U); j < OFFSPRING_COUNT; j++)
                {
                    if (offsprings[j].fitness > offsprings[i].fitness)
                    {
                        genome_t temp = offsprings[i];
                        offsprings[i] = offsprings[j];
                        offsprings[j] = temp;
                    }
                }
            }
        }

        // only here for ease of debugging
        int fitness_[OFFSPRING_COUNT];
        for (uint16_t i = 0U; i < OFFSPRING_COUNT; i++)
        {
            fitness_[i] = offsprings[i].fitness;
        }

        // offsprings become parents
        parents[0] = offsprings[0];
        parents[1] = offsprings[1];
        print_genomes(parents[0].genes, parents[1].genes, target_size);

        // break, if convergence
        if ((fitness_score(target.genes, parents[0].genes, target_size) == 0) || (fitness_score(target.genes, parents[1].genes, target_size) == 0))
        {
            (void)printf("\rConvergence Achieved!\n");
            (void)printf("%d", fitness_[0]);
            break;
        }

        iterations++;
    }

    (void)printf("\rNumber of Iterations: %llu\n", iterations);

    genome_destroy(&parents[0]);
    genome_destroy(&parents[1]);

    for (uint16_t i = 0U; i < OFFSPRING_COUNT; i++)
    {
        genome_destroy(&offsprings[i]);
    }

    return 0;
}