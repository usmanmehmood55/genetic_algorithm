#include "genetic_algorithm.h"

genome_t initialize_target(char * string)
{
    return (genome_t)
    {
        .genes   = string,
        .length  = strnlen(string, UINT16_MAX),
        .fitness = 0,
    };
}

/**
 * @brief Create a genome object
 * 
 * @param genome string created form mutated genes
 * @param length length of genome
 */
genome_t genome_create(uint16_t length)
{
    genome_t genome =
    {
        .genes   = calloc(length, sizeof(gene_t)),
        .length  = length,
        .fitness = ((int)length * -3)
    };

    for (uint16_t this_gene = 0; this_gene < length; this_gene++)
    {
        gene_t mutated_gene = get_mutated_gene();
        genome.genes[this_gene] = mutated_gene;
    }

    return genome;
}

void genome_destroy(genome_t * p_genome)
{
    free(p_genome->genes);
    p_genome->fitness = 0;
    p_genome->length  = 0;
}

/**
 * @brief Function for printing the genomes in a readable format
 * 
 * @param genome_1 first genome string
 * @param genome_2 second genome string
 * @param length   length of both genomes
 */
void print_genomes(char *genome_1, char *genome_2, uint16_t length)
{
    (void)printf("\rGenome 1: \"");
    for (uint16_t gene = 0; gene < length; gene++)
    {
        (void)printf("%c", genome_1[gene]);
    }
    (void)printf("\"\tGenome 2: \"");
    for (uint16_t gene = 0; gene < length; gene++)
    {
        (void)printf("%c", genome_2[gene]);
    }
    (void)printf("\"\n");
}

/**
 * @brief Provides a pseudo random number between a positive range
 * 
 * @param upper_limit upper limit number
 * @param lower_limit lower limit number
 * 
 * @return int        pseudo-random number, -ERANGE if upper limit is invalid, -EINVAL if limits are negative
 */
int random_in_pos_range(int upper_limit, int lower_limit)
{
    int result = 0;

    if ((upper_limit >= 0) && (lower_limit >= 0))
    {
        if (upper_limit > lower_limit)
        {
            result = ((rand() % (upper_limit - lower_limit + 1)) + lower_limit);
        }
        else
        {
            result = -ERANGE;
        }
    }
    else
    {
        result = -EINVAL;
    }

    return result;
}

/**
 * @brief Function for extracting a mutated / random gene from the available gene pool.
 *
 * @return char mutated gene character
 */
char get_mutated_gene(void)
{
    return GENES[random_in_pos_range((sizeof(GENES) - 1U), 0)];
}

/**
 * @brief Calculates fitness of the genome based on how close it is to the target
 * 
 * @param target target genome string
 * @param genome input genome string for which fitness has to be calculated
 * @param length length of both genomes
 * 
 * @return int   fitness score
 */
int fitness_score(const char *target, const char *genome, uint16_t length)
{
    int fitness = (int)length * -3;

    // Check for the characters that are exactly in their places.
    for (uint16_t i = 0; i < length; i++)
    {
        if (genome[i] == target[i])
        {
            fitness += 3;
        }
    }

    // Check for characters that are in the genome and also in the target (regardless of position)
    for (uint16_t i = 0; i < length; i++)
    {
        bool found = false;
        for (uint16_t j = 0; j < length; j++)
        {
            if (genome[i] == target[j])
            {
                found = true;
                break;
            }
        }

        if (found)
        {
            fitness += 1;
        }
        else
        {
            fitness -= 1;
        }
    }

    return fitness;
}

/**
 * @brief Mating combines the genomes of two parents over a random crossover point,
 * and the sequence of parents for the crossover is randomly selected.
 * 
 * @details
 * 
 * Take these two parents, of size 7:
 * 
 * index     = 0, 1, 2, 3, 4, 5, 6
 * parent 1  = a, b, c, d, e, f, g
 * parent 2  = h, i, j, k, l, m, n
 * 
 * random crossover point is 3
 * 
 * index     = 0, 1, 2, 3, 4, 5, 6
 * parent 1  = a, b, c, d,
 * parent 2  =             l, m, n
 * -------------------------------
 * offspring = a, b, c, d, l, m, n
 * 
 * However to prevent parent 1 from always contributing to the first few genes
 * and parent 2 to the remaining last, their sequence is randomly (should be 50/50)
 * selected so with the same crossover point at 3, this can also happen
 * 
 * index     = 0, 1, 2, 3, 4, 5, 6
 * parent 1  =             e, f, g
 * parent 2  = h, i, j, k,        
 * -------------------------------
 * offspring = h, i, j, k, e, f, g
 * 
 * @param[in]  parent_1  first parent genome
 * @param[in]  parent_2  second parent genome
 * @param[out] offspring buffer to place offspring genome
 * @param[in]  length    length of all genomes
 * 
 * @return char*         offspring genome
 */
char *mate(const char *parent_1, const char *parent_2, char *offspring, uint16_t length)
{
    uint16_t crossover_point = (uint16_t)random_in_pos_range((length - 1U), 0);
    bool flip_sequence = (bool)random_in_pos_range(1, 0);

    // Perform single-point crossover
    (void)memcpy(offspring, flip_sequence ? parent_1 : parent_2, crossover_point);
    (void)memcpy(offspring + crossover_point, (flip_sequence ? parent_2 : parent_1) + crossover_point, length - crossover_point);

    // Perform mutation on 1 gene with a 50/50 chance
    mutate_genome(offspring, length, 3U, 0U);

    return offspring;
}

/**
 * @brief Provides a mutated genome
 * 
 * @param genome       genome string to mutate
 * @param length       length of genome
 * @param max_mutation maximum possible genes to be mutated
 * @param min_mutation minimum possible genes to be mutated
 */
void mutate_genome(char *genome, uint16_t length, uint16_t max_mutation, uint16_t min_mutation)
{
    uint16_t total_mutations = (uint16_t)random_in_pos_range(max_mutation, min_mutation);
    for (uint16_t mutation = 0U; mutation < total_mutations; mutation++)
    {
        uint16_t random_gene_index = (uint16_t)random_in_pos_range(length - 1U, 0);
        genome[random_gene_index] = get_mutated_gene();
    }
}