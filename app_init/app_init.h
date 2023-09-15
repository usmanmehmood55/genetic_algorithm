#ifndef APP_INIT_H_
#define APP_INIT_H_

#include <stdint.h>
#include <stdbool.h>

#include "genetic_algorithm_utils.h"

/**
 * @brief Processes the given arguments to fill the target genome and offspring count
 * 
 * @details This function performs the following steps:
 * - Validates that the correct number of command line arguments are passed
 * - Validates that the target string contains only characters from the GENE_POOL
 * - Validates that the offspring count is within the allowed range
 * - Initializes the random number generator
 * 
 * @param[in]     argc              Number of command line arguments
 * @param[in,out] argv              Array of command line arguments
 * @param[out]    p_target          Pointer to the genome_t structure to initialize with the target string
 * @param[out]    p_offspring_count Pointer to a uint16_t to store the offspring count
 * 
 * @return true on successful initialization, false otherwise
 */
bool app_init(int argc, char ** argv, genome_t * p_target, uint16_t * p_offspring_count);

#endif // APP_INIT_H_