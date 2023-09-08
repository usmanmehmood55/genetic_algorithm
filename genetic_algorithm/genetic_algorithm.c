#include "genetic_algorithm.h"

#define WORST_FITNESS(length) (int)((int)length * -2)

/**
 * @brief Initialization of the target genome is slightly different
 * as it already has allocated memory and does not need any mutation.
 * 
 * @param[in,out] string target string
 * 
 * @return genome_t initialized target
 */
genome_t genome_target_init(char * string)
{
    int string_length = strnlen(string, UINT16_MAX);

    return (genome_t)
    {
        .genes   = string,
        .length  = string_length,
        .fitness = WORST_FITNESS(string_length),
    };
}

/**
 * @brief Creates a new genome object with allocated memory
 * of the given size. The genome must be manually freed by
 * genome_destroy() after it has been used.
 * 
 * @param[in] length length of genome
 * 
 * @return genome_t a new genome
 */
genome_t genome_create(uint16_t length)
{
    genome_t genome =
    {
        .genes   = calloc(length, sizeof(gene_t)),
        .length  = length,
        .fitness = WORST_FITNESS(length),
    };

    for (uint16_t this_gene = 0; this_gene < length; this_gene++)
    {
        gene_t mutated_gene = get_mutated_gene();
        genome.genes[this_gene] = mutated_gene;
    }

    return genome;
}

/**
 * @brief Frees the gene memory allocated to the genome and resets
 * its fitness and length
 * 
 * @param[out] p_genome pointer to genome
 */
void genome_destroy(genome_t * p_genome)
{
    free(p_genome->genes);
    p_genome->fitness = 0;
    p_genome->length  = 0;
}

/**
 * @brief Performs a deep copy of source to destination while
 * maintaining their original references
 * 
 * @param[out] destination 
 * @param[in]  source 
 */
void genome_copy(genome_t * destination, const genome_t * source)
{
    destination->fitness = source->fitness;
    destination->length = source->length;
    (void)memcpy(destination->genes, source->genes, source->length);
}

/**
 * @brief Function for printing the genomes in a readable format
 * 
 * @param[in] genome_1 first genome string
 * @param[in] genome_2 second genome string
 */
void genomes_print(const genome_t genome_1, const genome_t genome_2)
{
    (void)printf("\rGenome 1: \"");
    for (uint16_t gene = 0; gene < genome_1.length; gene++)
    {
        (void)printf("%c", genome_1.genes[gene]);
    }
    (void)printf("\" (%02d)", genome_1.fitness);

    (void)printf("\tGenome 2: \"");
    for (uint16_t gene = 0; gene < genome_2.length; gene++)
    {
        (void)printf("%c", genome_2.genes[gene]);
    }
    (void)printf("\" (%02d)", genome_2.fitness);
    (void)printf("\n");
}

/**
 * @brief Provides a pseudo random number between a positive range
 * 
 * @param[in] upper_limit upper limit number
 * @param[in] lower_limit lower limit number
 * 
 * @return int pseudo-random number, -ERANGE if upper limit is invalid, -EINVAL if limits are negative
 */
int random_in_pos_range(const int upper_limit, const int lower_limit)
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
 * @param[in] target target genome string
 * @param[in] genome input genome string for which fitness has to be calculated
 * @param[in] length length of both genomes
 * 
 * @return int fitness score
 */
int genome_calculate_fitness(const char *target, const char *genome, uint16_t length)
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

    return total_score - (int)length;
}

/**
 * @brief Sorts the given genome array by ascending fitness
 * 
 * @param[in,out] genomes      array of genomes
 * @param[in]     genome_count number of genomes in the array
 */
void genomes_sort_by_fitness(genome_t genomes[], uint16_t genome_count)
{
    for (uint16_t i = 0U; i < genome_count; i++)
    {
        for (uint16_t j = (i + 1U); j < genome_count; j++)
        {
            if (genomes[j].fitness > genomes[i].fitness)
            {
                genome_t temp = genomes[i];
                genomes[i] = genomes[j];
                genomes[j] = temp;
            }
        }
    }
}

/**
 * @brief Provides a mutated genome based on the maximum and minimum possible
 * mutations.
 * 
 * @param[in,out] genome       genome string to mutate
 * @param[in]     length       length of genome
 * @param[in]     max_mutation maximum possible genes to be mutated
 * @param[in]     min_mutation minimum possible genes to be mutated
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

/**
 * @brief Mating combines the genomes of two parents over a random crossover point,
 * while the sequence of parents for the crossover is randomly selected. After a
 * crossover, a slight mutation is performed to avoid a local maxima from occurring.
 * Fitness of the new offspring is then calculated, compared to the provided target.
 * 
 * @details
 * 
 * Take these two parents, of size 7:
 * +-----------+---+---+---+---+---+---+---+
 * | index     | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
 * |-----------|---|---|---|---|---|---|---|
 * | parent 1  | a | b | c | d | e | f | g |
 * | parent 2  | h | i | j | k | l | m | n |
 * +-----------+---+---+---+---+---+---+---+
 * 
 * If the random crossover point is 3, the resulting offspring will look like this
 * +-----------+---+---+---+---+---+---+---+
 * | index     | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
 * |-----------|---|---|---|---|---|---|---|
 * | parent 1  | a | b | c | d |   |   |   |
 * | parent 2  |   |   |   |   | l | m | n |
 * +-----------+---+---+---+---+---+---+---+
 * | offspring | a | b | c | d | l | m | n |
 * +-----------+---+---+---+---+---+---+---+
 * 
 * However to prevent parent 1 from always contributing to the first n genes
 * and parent 2 to the remaining n-l, their sequence is randomly (should be 50/50)
 * selected so with the same crossover point at 3, this can also happen
 * +-----------+---+---+---+---+---+---+---+
 * | index     | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
 * |-----------|---|---|---|---|---|---|---|
 * | parent 1  |   |   |   |   | e | f | g |
 * | parent 2  | h | i | j | k |   |   |   |
 * +-----------+---+---+---+---+---+---+---+
 * | offspring | h | i | j | k | e | f | g |
 * +-----------+---+---+---+---+---+---+---+
 * 
 * @param[in]  p_target    pointer to target
 * @param[in]  p_parent_1  pointer to parent 1
 * @param[in]  p_parent_2  pointer to parent 2
 * @param[out] p_offspring buffer to store offspring genome in
 */
void genomes_mate(const genome_t * p_target, const genome_t * p_parent_1, const genome_t * p_parent_2, genome_t * p_offspring)
{
    uint16_t length          = p_target->length;
    uint16_t crossover_point = (uint16_t)random_in_pos_range((length - 1U), 0);
    bool     flip_sequence   = (bool)random_in_pos_range(1, 0);

    // Perform single-point crossover
    (void)memcpy(p_offspring->genes, flip_sequence ? p_parent_1->genes : p_parent_2->genes, crossover_point);
    (void)memcpy(p_offspring->genes + crossover_point, (flip_sequence ? p_parent_2->genes : p_parent_1->genes) + crossover_point, length - crossover_point);

    // Perform mutation on 1 gene with a 50/50 chance
    mutate_genome(p_offspring->genes, length, 1U, 0U);

    // calculate fitness of the new offspring
    p_offspring->fitness = genome_calculate_fitness(p_target->genes, p_offspring->genes, length);
}