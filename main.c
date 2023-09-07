#include <stdio.h>
#include "genetic_algorithm.h"

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
    if (argc <= 1)
    {
        return -EINVAL;
    }
    
    genome_t target        = initialize_target(argv[1]);
    int      target_size   = strnlen(target.genes, UINT16_MAX);

    char     * parent_1    = calloc(target_size, sizeof(char));
    char     * parent_2    = calloc(target_size, sizeof(char));
    char     * offspring_1 = calloc(target_size, sizeof(char));
    char     * offspring_2 = calloc(target_size, sizeof(char));

    uint16_t   offspring_max_mutation  = (uint16_t)((float)target_size * 0.1f);
    uint16_t   offspring_min_mutation  = (uint16_t)((float)target_size * 0.05f);
    uint16_t   sof_max_mutation  = (uint16_t)((float)target_size * 0.3f);
    uint16_t   sof_min_mutation  = (uint16_t)((float)target_size * 0.1f);

    srand(time(NULL));

    initialize_genome(parent_1, target_size);
    initialize_genome(parent_2, target_size);

    uint64_t iterations = 0;
    while (1)
    {
        offspring_1 = mate(parent_1, parent_2, offspring_1, target_size, offspring_max_mutation, offspring_min_mutation);
        offspring_2 = mate(parent_1, parent_2, offspring_2, target_size, offspring_max_mutation, offspring_min_mutation);

        int offspring_1_fitness = fitness_score(target.genes, offspring_1, target_size);
        int offspring_2_fitness = fitness_score(target.genes, offspring_2, target_size);

        if (offspring_1_fitness > offspring_2_fitness)
        {
            mutate_genome(offspring_2, target_size, sof_max_mutation, sof_min_mutation);
        }
        else
        {
            mutate_genome(offspring_1, target_size, sof_max_mutation, sof_min_mutation);
        }

        print_genomes(parent_1, parent_2, target_size);
        if ((fitness_score(target.genes, parent_1, target_size) == 0) || (fitness_score(target.genes, parent_2, target_size) == 0))
        {
            (void)printf("\rConvergence Achieved!\n");
            break;
        }

        memcpy(parent_1, offspring_1, target_size);
        memcpy(parent_2, offspring_2, target_size);

        iterations++;
    }

    print_genomes(parent_1, parent_2, target_size);
    (void)printf("\rNumber of Iterations: %llu\n", iterations);

    free(parent_1);
    free(parent_2);
    free(offspring_1);
    free(offspring_2);

    return 0;
}