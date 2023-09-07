#ifndef GENETIC_ALGORITHM_H_
#define GENETIC_ALGORITHM_H_

#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define GENES "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ"

typedef char gene_t;

typedef struct genome_t
{
    gene_t   * genes;
    uint16_t   length;
    int        fitness;
} genome_t;

genome_t initialize_target(char * string);

/**
 * @brief Function for printing the genomes in a readable format
 * 
 * @param genome_1 first genome string
 * @param genome_2 second genome string
 * @param length   length of both genomes
 */
void print_genomes(char *genome_1, char *genome_2, uint16_t length);

/**
 * @brief Provides a pseudo random number between a positive range
 * 
 * @param upper_limit upper limit number
 * @param lower_limit lower limit number
 * 
 * @return int        pseudo-random number, -ERANGE if upper limit is invalid, -EINVAL if limits are negative
 */
int random_in_pos_range(int upper_limit, int lower_limit);

/**
 * @brief Function for extracting a mutated / random gene from the available gene pool.
 *
 * @return char mutated gene character
 */
char get_mutated_gene(void);

/**
 * @brief Create a genome object
 * 
 * @param genome string created form mutated genes
 * @param length length of genome
 */
void initialize_genome(char *genome, uint16_t length);

/**
 * @brief Calculates fitness of the genome based on how close it is to the target
 * 
 * @param target target genome string
 * @param genome input genome string for which fitness has to be calculated
 * @param length length of both genomes
 * 
 * @return int   fitness score
 */
int fitness_score(const char *target, const char *genome, uint16_t length);

/**
 * @brief Mating criteria has not been finalized. Right now the genome with more
 * fitness is completely copied to the weaker genome, and the two are later mutated
 * to form distinct offsprings. This setp is not in adherence with the typical
 * genetic algorithm, and can be improved a lot.
 * 
 * @param[in]  target    target genome
 * @param[in]  genome_1  first parent genome
 * @param[in]  genome_2  second parent genome
 * @param[out] offspring buffer to place offspring genome
 * @param[in]  length    length of all genomes
 * 
 * @return char*         offspring genome
 */
char *mate(const char *parent_1, const char *parent_2, char *offspring, uint16_t length, uint16_t max_mutation, uint16_t min_mutation);

/**
 * @brief Provides a mutated genome
 * 
 * @param genome       genome string to mutate
 * @param length       length of genome
 * @param max_mutation maximum possible genes to be mutated
 * @param min_mutation minimum possible genes to be mutated
 */
void mutate_genome(char *genome, uint16_t length, uint16_t max_mutation, uint16_t min_mutation);

#endif // GENETIC_ALGORITHM_H_