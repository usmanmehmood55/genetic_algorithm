#ifndef GENETIC_ALGORITHM_H_
#define GENETIC_ALGORITHM_H_

#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#define GENES "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ"

typedef char gene_t;

typedef struct genome_t
{
    gene_t   * genes;
    uint16_t   length;
    int        fitness;
} genome_t;

/**
 * @brief Initialization of the target genome is slightly different
 * as it already has allocated memory and does not need any mutation.
 * 
 * @param string 
 * @return genome_t 
 */
genome_t target_genome_init(char * string);

/**
 * @brief Creates a new genome object with allocated memory
 * of the given size. The genome must be manually freed by
 * genome_destroy() after it has been used.
 * 
 * @param length length of genome
 */
genome_t genome_create(uint16_t length);

/**
 * @brief Frees the gene memory allocated to the genome and resets
 * its fitness and length
 * 
 * @param[out] p_genome pointer to genome
 */
void genome_destroy(genome_t * p_genome);

/**
 * @brief Performs a deep copy of source to destination while
 * maintaining their original references
 * 
 * @param destination 
 * @param source 
 */
void genome_copy(genome_t * destination, genome_t * source);

/**
 * @brief Function for printing the genomes in a readable format
 * 
 * @param genome_1 first genome string
 * @param genome_2 second genome string
 */
void print_genomes(genome_t genome_1, genome_t genome_2);

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
 * @brief Provides a mutated genome
 * 
 * @param genome       genome string to mutate
 * @param length       length of genome
 * @param max_mutation maximum possible genes to be mutated
 * @param min_mutation minimum possible genes to be mutated
 */
void mutate_genome(char *genome, uint16_t length, uint16_t max_mutation, uint16_t min_mutation);

/**
 * @brief Mating combines the genomes of two parents over a random crossover point,
 * while the sequence of parents for the crossover is randomly selected. After a
 * crossover, a slight mutation is performed to avoid a local maxima from occuring.
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
 * @param[in]  parent_1  first parent genome
 * @param[in]  parent_2  second parent genome
 * @param[out] offspring buffer to place offspring genome
 * @param[in]  length    length of all genomes
 * 
 * @return char*         offspring genome
 */
char *mate(const char *parent_1, const char *parent_2, char *offspring, uint16_t length);

#endif // GENETIC_ALGORITHM_H_