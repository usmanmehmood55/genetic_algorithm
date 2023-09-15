#include "c_asserts.h"
#include "genetic_algorithm_utils.h"

static bool test_genome_target_init(void)
{
    char * target_string = "Hello, World!";
    genome_t target = genome_target_init(target_string);

    ASSERT_TEST(target.length == strlen(target_string));
    ASSERT_TEST(target.genes == target_string); // Pointing to the same string
    ASSERT_TEST(target.fitness == 0);

    return true;
}

static bool test_genome_init(void)
{
    uint16_t length = 10;
    genome_t genome = genome_init(length);

    ASSERT_TEST(genome.length == length);
    ASSERT_TEST(genome.fitness == (int)-length);
    ASSERT_TEST(genome.genes != NULL);

    genome_destroy(&genome);
    return true;
}

static bool test_genome_destroy(void)
{
    uint16_t length = 10;
    genome_t genome = genome_init(length);

    genome_destroy(&genome);

    ASSERT_TEST(genome.genes == NULL);
    ASSERT_TEST(genome.length == 0U);
    ASSERT_TEST(genome.fitness == 0);

    return true;
}

static bool test_genome_copy(void)
{
    uint16_t length = 5;
    genome_t source = genome_init(length);
    genome_t destination = genome_init(length);
    const genome_t * source_ref = &source;
    const genome_t * dest_ref = &destination;

    genome_copy(&destination, &source);

    ASSERT_TEST(destination.length == source.length);
    ASSERT_TEST(destination.fitness == source.fitness);
    ASSERT_TEST(memcmp(destination.genes, source.genes, length) == 0);
    ASSERT_TEST(source_ref == &source);
    ASSERT_TEST(dest_ref == &destination);

    genome_destroy(&source);
    genome_destroy(&destination);
    return true;
}

static bool test_genome_calculate_fitness_completely_fit(void)
{
    char * target_string = "ABCDEFGHIJK";
    genome_t target = genome_target_init(target_string);
    genome_t genome = genome_init(target.length);
    (void)memcpy(genome.genes, "ABCDEFGHIJK", target.length);

    int fitness = genome_calculate_fitness(target.genes, genome.genes, target.length);

    ASSERT_TEST(fitness == 0);

    genome_destroy(&genome);
    return true;
}

static bool test_genome_calculate_fitness_half_fit(void)
{
    char * target_string = "ABCDEFGHIJK";
    genome_t target = genome_target_init(target_string);
    genome_t genome = genome_init(target.length);
    (void)memcpy(genome.genes, "ABCDEFXXXXX", target.length);

    int fitness = genome_calculate_fitness(target.genes, genome.genes, target.length);

    ASSERT_TEST(fitness == -5);

    genome_destroy(&genome);
    return true;
}

static bool test_genome_calculate_fitness_fully_unfit(void)
{
    char * target_string = "ABCDEFGHIJK";
    genome_t target = genome_target_init(target_string);
    genome_t genome = genome_init(target.length);
    (void)memcpy(genome.genes, "LMNOPQRSTUV", target.length);

    int fitness = genome_calculate_fitness(target.genes, genome.genes, target.length);

    ASSERT_TEST(fitness == (int)-target.length);

    genome_destroy(&genome);
    return true;
}

static bool test_genomes_sort_by_fitness(void)
{
    genome_t genomes[10];
    for (uint16_t i = 0U; i < 10U; i++)
    {
        genomes[i].fitness = (int)i;
    }

    genomes_sort_by_fitness(genomes, 10U);

    for (uint16_t i = 0U; i < 10U; i++)
    {
        int expected = (9 - (int)i);
        int actual = genomes[i].fitness;

        ASSERT_TEST(expected == actual);
    }

    return true;
}

static bool test_genome_mutation(void)
{
    char * target_string = "ABCDEFGHIJK";
    genome_t target = genome_target_init(target_string);
    genome_t genome = genome_init(target.length);
    (void)memcpy(genome.genes, target_string, target.length);

    uint16_t min_mutation = 3U;
    uint16_t max_mutation = 6U;

    mutate_genome(genome.genes, genome.length, max_mutation, min_mutation);

    uint16_t mismatch = 0;
    for (uint16_t gene = 0; gene < genome.length; gene++)
    {
        mismatch += (target_string[gene] != genome.genes[gene]);
    }

    ASSERT_TEST(mismatch >= min_mutation);
    ASSERT_TEST(mismatch <= max_mutation);

    genome_destroy(&genome);

    return true;
}

static bool test_random_in_pos_range(void)
{
    int random = random_in_pos_range(6, 3);
    ASSERT_TEST(random >= 3);
    ASSERT_TEST(random <= 6);

    random = random_in_pos_range(3, 6);
    ASSERT_TEST(random == -ERANGE);


    random = random_in_pos_range(-2, 6);
    ASSERT_TEST(random == -EINVAL);

    return true;
}

static bool test_genomes_mate(void)
{
    char *target_string = "UVWXYZ";
    genome_t target = genome_target_init(target_string);
    genome_t parent1 = genome_init(target.length);
    genome_t parent2 = genome_init(target.length);
    (void)memcpy(parent1.genes, "ABCDEF", target.length);
    (void)memcpy(parent2.genes, "GHIJKL", target.length);

    genome_t offspring = genome_init(target.length);

    genomes_mate(&target, &parent1, &parent2, &offspring);

    bool     all_from_parent1 = true;
    bool     all_from_parent2 = true;
    uint16_t mutation_count   = 0;

    for (uint16_t i = 0; i < target.length; i++)
    {        
        if (offspring.genes[i] != parent1.genes[i])
        {
            all_from_parent1 = false;
        }

        if (offspring.genes[i] != parent2.genes[i])
        {
            all_from_parent2 = false;
        }

        if ((offspring.genes[i] != parent1.genes[i]) && (offspring.genes[i] != parent2.genes[i]))
        {
            mutation_count++;
        }
    }

    // Not ALL of genes in the offspring are from parent 1
    ASSERT_TEST(!all_from_parent1);

    // Not ALL of genes in the offspring are from parent 2
    ASSERT_TEST(!all_from_parent2);

    // There have been no more than 1 mutations
    ASSERT_TEST(mutation_count <= 1U);

    genome_destroy(&parent1);
    genome_destroy(&parent2);
    genome_destroy(&offspring);
    return true;
}

int main(void)
{
    TEST(test_genome_target_init);
    TEST(test_genome_init);
    TEST(test_genome_destroy);
    TEST(test_genome_copy);
    TEST(test_genome_calculate_fitness_fully_unfit);
    TEST(test_genome_calculate_fitness_half_fit);
    TEST(test_genome_calculate_fitness_completely_fit);
    TEST(test_genomes_sort_by_fitness);
    TEST(test_genome_mutation);
    TEST(test_random_in_pos_range);
    TEST(test_genomes_mate);

    int test_result = display_test_summary();
    return test_result;
}