#include <stdio.h>
#include "include/genetic_algorithm.h"

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
    if (argc == 0)
    {
        return -EINVAL;
    }

    char * target       = argv[1];
    int    target_size  = strnlen(target, UINT16_MAX);

    char * parent_1     = malloc(target_size);
    char * parent_2     = malloc(target_size);
    char * offspring_1  = malloc(target_size);
    char * offspring_2  = malloc(target_size);

    int    max_mutation = (int)((float)target_size * 0.3f);
    int    min_mutation = (int)((float)target_size * 0.1f);

    srand(time(0));

    initialize_genome(parent_1, target_size);
    initialize_genome(parent_2, target_size);

    uint64_t iterations = 0;
    while (1)
    {
        char * offspring = mate(target, parent_1, parent_2, target_size);
        memcpy(offspring_1, offspring, target_size);
        memcpy(offspring_2, offspring, target_size);
        int offspring_1_fitness = fitness_score(target, offspring_1, target_size);
        int offspring_2_fitness = fitness_score(target, offspring_2, target_size);

        if (offspring_1_fitness > offspring_2_fitness)
        {
            mutate_genome(offspring_2, target_size, max_mutation, min_mutation);
        }
        else
        {
            mutate_genome(offspring_1, target_size, max_mutation, min_mutation);
        }

        print_genomes(parent_1, parent_2, target_size);
        if (fitness_score(target, parent_1, target_size) == 0 || fitness_score(target, parent_2, target_size) == 0)
        {
            printf("\rConvergence Achieved!\n");
            break;
        }

        memcpy(parent_1, offspring_1, target_size);
        memcpy(parent_2, offspring_2, target_size);

        iterations++;
    }

    print_genomes(parent_1, parent_2, target_size);
    printf("\rNumber of Iterations: %llu\n", iterations);

    free(parent_1);
    free(parent_2);
    free(offspring_1);
    free(offspring_2);

    return 0;
}