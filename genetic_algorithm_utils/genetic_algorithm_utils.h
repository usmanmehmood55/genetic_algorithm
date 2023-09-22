/**
 * @file     genetic_algorithm_utils.h
 * @author   Usman Mehmood (usmanmehmood55@gmail.com)
 * @brief    Utility functions for performing different operations on a genome.
 * @version  0.1
 * @date     21-09-2023
 * 
 * Copyright (c) 2023, Usman Mehmood
 * 
 */

#ifndef GENETIC_ALGORITHM_H_
#define GENETIC_ALGORITHM_H_

#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief This is a gene pool. You can add your own characters to this as long as they don't mess with the
 * functions and stuff.
 */
#define GENE_POOL "!@#$^&*()_-=+,.;:'/\\\"{}[]<>? 1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

/**
 * @brief A singular gene is just a character
 */
typedef char gene_t;

/**
 * @brief The genome structure. It contains genes, the length of those genes and a fitness score.
 * The genome must be initialized with the \ref genome_target_init function before use.
 */
typedef struct genome_t
{
    gene_t   * genes;   ///< Pointer to an array of genes.
    uint16_t   length;  ///< length of the genes.
    int        fitness; ///< Fitness of the genes.
} genome_t;

/**
 * @brief Initialization of the target genome is slightly different
 * as it already has allocated memory and does not need any mutation.
 * 
 * @param[in,out] string target string
 * 
 * @return genome_t initialized target
 */
genome_t genome_target_init(char * string);

/**
 * @brief Creates a new genome object with allocated memory
 * of the given size. The genome must be manually freed by
 * genome_destroy() after it has been used.
 * 
 * @param[in] length length of genome
 * 
 * @return genome_t a new genome
 */
genome_t genome_init(uint16_t length);

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
 * @param[out] destination 
 * @param[in]  source 
 */
void genome_copy(genome_t * destination, const genome_t * source);

/**
 * @brief Function for printing genomes in a readable format
 * 
 * @param[in] genome
 */
void genome_print(const genome_t genome);

/**
 * @brief Provides a pseudo random number between a positive range
 * 
 * @param[in] upper_limit upper limit number
 * @param[in] lower_limit lower limit number
 * 
 * @return int pseudo-random number, -ERANGE if upper limit is invalid, -EINVAL if limits are negative
 */
int random_in_pos_range(const int upper_limit, const int lower_limit);

/**
 * @brief Function for extracting a mutated / random gene from the available gene pool
 * specified in the \ref GENE_POOL macro.
 *
 * @return char mutated gene character
 */
char get_mutated_gene(void);

/**
 * @brief Calculates fitness of the genome based on how close it is to the target
 * 
 * @param[in] target target genome string
 * @param[in] genome input genome string for which fitness has to be calculated
 * @param[in] length length of both genomes
 * 
 * @return int fitness score
 */
int genome_calculate_fitness(const char *target, const char *genome, uint16_t length);

/**
 * @brief Sorts the given genome array by ascending fitness
 * 
 * @param[in,out] genomes      array of genomes
 * @param[in]     genome_count number of genomes in the array
 */
void genomes_sort_by_fitness(genome_t genomes[], uint16_t genome_count);

/**
 * @brief Provides a mutated genome based on the maximum and minimum possible
 * mutations.
 * 
 * @param[in,out] genome       genome string to mutate
 * @param[in]     length       length of genome
 * @param[in]     max_mutation maximum possible genes to be mutated
 * @param[in]     min_mutation minimum possible genes to be mutated
 */
void mutate_genome(char *genome, uint16_t length, uint16_t max_mutation, uint16_t min_mutation);

/**
 * @brief Mating combines the genomes of two parents over a random crossover point,
 * while the sequence of parents for the crossover is randomly selected. After a
 * crossover, a slight mutation is performed to avoid a local maxima from occurring.
 * Fitness of the new offspring is then calculated, compared to the provided target.
 * 
 * @param[in]  p_target    pointer to target
 * @param[in]  p_parent_1  pointer to parent 1
 * @param[in]  p_parent_2  pointer to parent 2
 * @param[out] p_offspring buffer to store offspring genome in
 */
void genomes_mate(const genome_t * p_target, const genome_t * p_parent_1, const genome_t * p_parent_2, genome_t * p_offspring);

#endif // GENETIC_ALGORITHM_H_