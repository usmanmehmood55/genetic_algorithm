/**
 * @file     app_init.c
 * @author   Usman Mehmood (usmanmehmood55@gmail.com)
 * @brief    Checks and initializes the command line arguments passed to the application
 * @version  0.1
 * @date     21-09-2023
 * 
 * Copyright (c) 2023, Usman Mehmood
 * 
 */

#include "app_init.h"

/** 
 * @brief Validate that all characters in the given string are in the \ref GENE_POOL
 * 
 * @param str The string to validate
 * 
 * @return true if all characters in the string are in the \ref GENE_POOL, false otherwise
 */
static bool validate_target_string(const char *str)
{
    uint16_t str_len = strnlen(str, UINT16_MAX);

    for (uint16_t i = 0; i < str_len; ++i)
    {
        if (strchr(GENE_POOL, str[i]) == NULL)
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief Processes the given arguments to fill the target genome and offspring count
 * 
 * @details This function performs the following steps:
 * - Validates that the correct number of command line arguments are passed
 * - Validates that the target string contains only characters from the \ref GENE_POOL
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
bool app_init(int argc, char ** argv, genome_t * p_target, uint16_t * p_offspring_count)
{
    if (argc < 3)
    {
        (void)printf("\rInvalid args.\n");
        (void)printf("\rExpected: \"<target string>\" <offspring count>\n");
        (void)printf("\rExample:  ./genetic_algorithm.exe \"This is my target\" 500\n");
        return false;
    }

    bool is_target_valid = validate_target_string(argv[1]);
    if (is_target_valid == false)
    {
        (void)printf("\rInvalid args.\n");
        (void)printf("\rTarget string must not contain any character outside of the gene pool.\n");
        return false;
    }

    int offspring_count = atoi(argv[2]);
    if (offspring_count <= 5)
    {
        (void)printf("\rInvalid args.\n");
        (void)printf("\rOffspring count must be greater than 5.\n");
        return false;
    }

    if (offspring_count >= UINT16_MAX)
    {
        (void)printf("\rInvalid args.\n");
        (void)printf("\rOffspring count must be less than %u.\n", UINT16_MAX);
        return false;
    }

    (void)printf("\r\n");
    srand(time(NULL)); // required for random number generation
    * p_offspring_count = (uint16_t)offspring_count;
    * p_target = genome_target_init(argv[1]);

    return true;
}