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
 * @brief Create a genome object
 * 
 * @param genome string created form mutated genes
 * @param length length of genome
 */
void initialize_genome(char *genome, uint16_t length)
{
    (void)memset(genome, 0, length);

    for (uint16_t gene = 0; gene < length; gene++)
    {
        char mutated_gene = get_mutated_gene();
        genome[gene] = mutated_gene;
    }
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
    int total_score = 0;

    // for eliminating genes that are not in the TARGET
    for (uint16_t gene = 0; gene < length; gene++)
    {
        int gene_non_existence_score = 0;

        for (uint16_t target_gene = 0; target_gene < length; target_gene++)
        {
            if (genome[gene] != target[target_gene])
            {
                gene_non_existence_score++;
            }
        }

        if (gene_non_existence_score == (int)length)
        {
            total_score--;
        }
    }

    // for enhancing genes that are in the TARGET
    for (uint16_t gene = 0; gene < length; gene++)
    {
        int gene_existence_score = 0;

        for (uint16_t target_gene = 0; target_gene < length; target_gene++)
        {
            if (genome[gene] == target[target_gene])
            {
                gene_existence_score++;
            }
        }

        if (gene_existence_score == (int)length)
        {
            total_score++;
        }
    }

    // for matching the exact string
    for (uint16_t gene = 0; gene < length; gene++)
    {
        if (genome[gene] == target[gene])
        {
            total_score++;
        }
    }

    total_score -= (int)length;

    return total_score;
}

/**
 * @brief Mating criteria has not been finalized. Right now the genome with more
 * fitness is completely copied to the weaker genome, and the two are later mutated
 * to form distinct offsprings. This setp is not in adherence with the typical
 * genetic algorithm, and can be improved a lot.
 * 
 * @param[in]  parent_1  first parent genome
 * @param[in]  parent_2  second parent genome
 * @param[out] offspring buffer to place offspring genome
 * @param[in]  length    length of all genomes
 * 
 * @return char*         offspring genome
 */
char *mate(const char *parent_1, const char *parent_2, char *offspring, uint16_t length, uint16_t max_mutation, uint16_t min_mutation)
{
    uint16_t crossover_point = (uint16_t)random_in_pos_range(length - 1U, 0);

    // Perform single-point crossover
    (void)memcpy(offspring, parent_1, crossover_point);
    (void)memcpy(offspring + crossover_point, parent_2 + crossover_point, length - crossover_point);

    // Perform mutation
    mutate_genome(offspring, length, max_mutation, min_mutation);

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