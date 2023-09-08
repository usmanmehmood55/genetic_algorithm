#ifndef APP_INIT_H_
#define APP_INIT_H_

#include <stdint.h>
#include <stdbool.h>

#include "genetic_algorithm.h"

bool app_init(int argc, char ** argv, genome_t * p_target, uint16_t * p_offspring_count);

#endif // APP_INIT_H_