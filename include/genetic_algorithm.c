#include "genetic_algorithm.h"

/**
 * @brief Function for printing the genomes in a readable format
 * 
 * @param genome_1 first genome string
 * @param genome_2 second genome string
 * @param length   length of both genomes
 */
void print_genomes(char *genome_1, char *genome_2, uint16_t length)
{
    printf("\rGenome 1: \"");
    for (uint16_t gene = 0; gene < length; gene++)
    {
        printf("%c", genome_1[gene]);
    }
    printf("\"\tGenome 2: \"");
    for (uint16_t gene = 0; gene < length; gene++)
    {
        printf("%c", genome_2[gene]);
    }
    printf("\"\n");
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
char mutated_gene(void)
{
    return GENES[random_in_pos_range((sizeof(GENES) - 1), 0)];
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
        char _ = mutated_gene();
        genome[gene] = _;
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

        if (gene_non_existence_score == length)
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

        if (gene_existence_score == length)
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

    return total_score;
}

/**
 * @brief Mating criteria has not been finalized.
 * TODO: This function causes a segfault because it mixes the two char pointers
 *
 * @param genome_1 first genome
 * @param genome_2 second genome
 * 
 * @return char*   offspring genome
 */
char *mate(char *target, char *genome_1, char *genome_2, uint16_t length)
{
    char *p_dominant_genome = NULL;
    int fitness_1 = fitness_score(target, genome_1, length);
    int fitness_2 = fitness_score(target, genome_2, length);

    if (fitness_1 > fitness_2)
    {
        p_dominant_genome = memcpy(genome_2, genome_1, length);
    }
    else
    {
        p_dominant_genome = memcpy(genome_1, genome_2, length);
    }

    return p_dominant_genome;
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
    int total_mutations = random_in_pos_range(max_mutation, min_mutation);
    for (uint16_t mutation = 0; mutation < total_mutations; mutation++)
    {
        int random_gene_index = random_in_pos_range((sizeof(GENES) - 1), 0);
        genome[random_gene_index] = mutated_gene();
    }
}