#include "genetic_algorithm.h"

//-definition-----------------------------------------------------------------------------------------------------------

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NullDereference"
#pragma ide diagnostic ignored "MemoryLeak"
typedef int32_t (*PopulationSortFn)(void *a, void *b);
typedef int32_t Gene;

typedef struct {
    Gene **chromosomes;
    double *fitness_table;
    size_t *ranking_table;
    size_t size;
    size_t length;
} Population;

//-declarations---------------------------------------------------------------------------------------------------------

static Population *Population_new(size_t population_size, int32_t path_length);
static void Population_free(Population *population);
static void Population_generate(Population *population, Graph *graph, size_t from);
static double Population_eval(Population *population, Graph *graph);
static void Population_crossover(Population *population, CrossoverAlgorithm crossoverAlgorithm);
static Gene *Population_selection(Population *population, SelectionAlgorithm selectionAlgorithm, int32_t *last_selection);
static void Population_mutate(Population *population, double rate);
static Path *Population_best_path(Population *population);

static Gene *Path_to_chromosome(Path *path);
static double Graph_run_chromosome(Graph *graph, Gene *chromossome, size_t length);
static void Chromosome_print(Gene *chromosome, size_t length);

static size_t pick_ranking_selection(Population *population, int32_t last_selection);
static size_t pick_tournament_selection(Population *population);
static void elite_selection(Population *population, Gene **new_chromosomes);
static void apply_PMX_crossover(Population *population);
static void apply_OX1_crossover(Population *population);

//-functions------------------------------------------------------------------------------------------------------------

Path *build_genetic_algorithm(Graph *graph, size_t from, CrossoverAlgorithm crossoverAlgorithm, int population_size, double max_time) {
    START_LOG("genetic_algorithm");
    Population *population = Population_new(population_size, graph->vertices_num);
    Population_generate(population, graph, from);

    int32_t actual_gen = 0;
    int32_t stable_gens = 0;
    double start_time = get_time();

    double last_log = 0;
    double previous_best = DBL_MAX;
    double average_fitness = 0;

    while (stable_gens < GEN_MAX_STABLE && get_time() < start_time + max_time && actual_gen < 100) {
        actual_gen++;
        double actual_best = Population_eval(population, graph);

        if (get_time() - last_log > GEN_LOG_INTERVAL) {
            average_fitness = 0;
            last_log = get_time();
            for (size_t i = 0; i < population->size; ++i) {
                size_t rank = population->ranking_table[i];
                // PRINT("%li, %li", i, rank);
                // OUTPUT("[%li]:  \t%lf\n", rank, population->fitness_table[rank]);
                average_fitness += population->fitness_table[rank] / (double)population->size;
            }

            OUTPUT("GEN %i:\n", actual_gen);
            OUTPUT("best:\t%lf\n", actual_best);
            OUTPUT("avg: \t%lf\n", average_fitness);
            OUTPUT("stable: %i\n\n", stable_gens);
        }

        Population_crossover(population, crossoverAlgorithm);
        Population_mutate(population, GEN_MUT_RATE);

        if (actual_best >= previous_best) {
            stable_gens++;
        } else {
            stable_gens = 0;
            previous_best = actual_best;
        }

    }

    Population_eval(population, graph);
    PRINT("end of genetic algorithm");
    Path *path = Population_best_path(population);

    Population_free(population);
    // Path_update_distance(graph, path);
    END_LOG("genetic_algorithm");
    return path;
}

//-static---------------------------------------------------------------------------------------------------------------

static Population *Population_new(size_t population_size, int32_t path_length) {
    Population *population = malloc(sizeof(Population));

    population->chromosomes = malloc(sizeof(Gene *) * population_size);
    population->fitness_table = malloc(sizeof(double) * population_size);
    population->ranking_table = malloc(sizeof(size_t) * population_size);
    population->size = population_size;
    population->length = path_length;

    return population;
}

static void Population_free(Population *population) {
    for (size_t i = 0; i < population->size; ++i) {
        free(population->chromosomes[i]);
    }
    free(population->chromosomes);
    free(population->fitness_table);
    free(population->ranking_table);
    free(population);
}

static void Population_generate(Population *population, Graph *graph, size_t from) {
    #pragma omp parallel
    {
        #pragma omp for
        for (size_t i = 0; i < GEN_SIZE; ++i) {
            Path *p = build_nearest_neighbor_random(graph, from);
            // apply_two_opt(graph, p);
            population->chromosomes[i] = Path_to_chromosome(p);

            free(p);
            OUTPUT("Chromosome (%i) generated\n", (int)i);

        }
    }
}

static int32_t compare_fitness(void *a, void *b, void *ctx) {
    Population *population = (Population *)ctx;
    int indexA = *(int *)a;
    int indexB = *(int *)b;
    return (population->fitness_table[indexA] > population->fitness_table[indexB]) -
           (population->fitness_table[indexA] < population->fitness_table[indexB]);
}

static double Population_eval(Population *population, Graph *graph) {
    double best_fitness = DBL_MAX;

    #pragma omp parallel
    {
        #pragma omp for
        for (size_t i = 0; i < population->size; ++i) {
            double actual_fitness = Graph_run_chromosome(graph, population->chromosomes[i], population->length);
            population->fitness_table[i] = actual_fitness;

            // PRINT("Chromosome (%i) evaluated: %lf", (int)i, actual_fitness);
        }
    }
    size_t idx[population->size];
    for (size_t i = 0; i < population->size; i++) idx[i] = i;

    // CLion reclamando da declaração "inexistente" do qsort_r
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-function-declaration"
    qsort_r(idx, population->size, sizeof(size_t), compare_fitness, population);
#pragma clang diagnostic pop

    for(size_t i = 0; i < population->size; ++i) {
        population->ranking_table[i] = idx[i];
    }

    best_fitness = population->fitness_table[
        population->ranking_table[0]
    ];

    return best_fitness;
}

static Gene *Population_selection(Population *population, SelectionAlgorithm selectionAlgorithm, int32_t *last_selection) {
    switch (selectionAlgorithm) {
        case RANKING_SELECTION:
            (*last_selection) = (int32_t)pick_ranking_selection(population, *last_selection);
            return population->chromosomes[(*last_selection)];

        case TOURNAMENT_SELECTION:
            return population->chromosomes[pick_tournament_selection(population)];

        default:
            RAISE("Selection (%i) not implemented", selectionAlgorithm);
    }
}

static size_t pick_ranking_selection(Population *population, int32_t last_selection) {
    int32_t sign = last_selection >= 0;
    uint64_t total_rank = ((GEN_MAX_RANK_SELECTION - sign) * (GEN_MAX_RANK_SELECTION + sign)) / 2;
    uint64_t pick = get_rand(total_rank);
    uint64_t sum = 0;

    for (size_t i = 0; i < GEN_MAX_RANK_SELECTION; i++) {
        if (last_selection >= 0 && (size_t)last_selection == population->ranking_table[i]) continue;

        sum += (GEN_MAX_RANK_SELECTION - i);
        if (sum >= pick) {
            // PRINT("pick randonly: [%li] (rand: %li)", population->ranking_table[i], pick);

            return population->ranking_table[i];
        }
    }

    return 0;
}

static size_t pick_tournament_selection(Population *population) {
    size_t best_index = -1;
    double best_fitness = DBL_MAX;

    for (int i = 0; i < GEN_MAX_RANK_SELECTION; ++i) {
        // Escolha aleatória de um indivíduo
        size_t idx = get_rand(population->size);
        double fitness = population->fitness_table[idx];

        if (fitness < best_fitness) {
            best_fitness = fitness;
            best_index = idx;
        }
    }

    return best_index;
}

static void Population_crossover(Population *population, CrossoverAlgorithm crossoverAlgorithm) {
    switch (crossoverAlgorithm) {
        case PMX_CROSSOVER:
            apply_PMX_crossover(population);
            break;
        case OX1_CROSSOVER:
            apply_OX1_crossover(population);
            break;
        default:
            RAISE("Crossover (%i) not implemented", crossoverAlgorithm);
    }
}

static void apply_PMX_crossover(Population *population) {
    Gene **new_chromosomes = malloc(sizeof(Gene *) * population->size);
    elite_selection(population, new_chromosomes);

    for (size_t i = GEN_ELITE_COUNT; i < population->size; i += 2) {
        int32_t last_selection = -1;
        Gene *parent1 = Population_selection(population, RANKING_SELECTION, &last_selection);
        Gene *parent2 = Population_selection(population, RANKING_SELECTION, &last_selection);

        size_t cut1 = get_rand(population->length - 1);
        size_t cut2 = cut1 + 1 + get_rand(population->length - cut1 - 1);

        Gene *child1 = malloc(sizeof(Gene) * population->length);
        Gene *child2 = malloc(sizeof(Gene) * population->length);
        memset(child1, -1, sizeof(Gene) * population->length);
        memset(child2, -1, sizeof(Gene) * population->length);

        // Copy segment from parents to children
        for (size_t j = cut1; j <= cut2; ++j) {
            child1[j] = parent1[j];
            child2[j] = parent2[j];
        }

        // Mapping process
        for (size_t j = cut1; j <= cut2; ++j) {
            Gene val1 = parent1[j];
            Gene val2 = parent2[j];

            if (val1 != val2) {
                for (size_t k = 0; k < population->length; ++k) {
                    if (child1[k] == -1 && parent2[k] == val1) {
                        child1[k] = val1;
                    }
                    if (child2[k] == -1 && parent1[k] == val2) {
                        child2[k] = val2;
                    }
                }
            }
        }

        // Fill in remaining genes
        for (size_t j = 0; j < population->length; ++j) {
            if (child1[j] == -1) child1[j] = parent2[j];
            if (child2[j] == -1) child2[j] = parent1[j];
        }

        new_chromosomes[i] = child1;
        if (i + 1 < population->size) {
            new_chromosomes[i + 1] = child2;
        }
    }

    for (size_t i = GEN_ELITE_COUNT; i < population->size; ++i) {
        free(population->chromosomes[population->ranking_table[i]]);
    }

    population->chromosomes = new_chromosomes;
}

static void apply_OX1_crossover(Population *population) {
    Gene **new_chromossomes = malloc(sizeof(Gene *) * population->size);

    elite_selection(population, new_chromossomes);

    for (size_t i = GEN_ELITE_COUNT; i < population->size; i += 2) {
        int32_t last_selection = -1;
        Gene *a = Population_selection(population, TOURNAMENT_SELECTION, &last_selection),
             *b = Population_selection(population, TOURNAMENT_SELECTION, &last_selection);

        // LOG("a: ");
        // Chromosome_print(a, population->length);
        // LOG("b: ");
        // Chromosome_print(b, population->length);

        size_t  i1 = get_rand(population->length - 1),
                i2 = (i1 + 1) + get_rand(population->length - (i1 + 1));
        InclusionMap segment_map1 = InclusionMap_new(),
                     segment_map2 = InclusionMap_new();

        Gene *c = malloc(sizeof(Gene) * population->length);
        Gene *d = malloc(sizeof(Gene) * population->length);
        for (size_t j = 0; j < population->length; ++j) {
            c[j] = -1;
            d[j] = -1;
        }

        // PRINT("[%li, %li]", i1, i2);
        for (size_t j = i1; j <= i2; ++j) {
            c[j] = a[j];
            d[j] = b[j];
            InclusionMap_set(&segment_map1, c[j]);
            InclusionMap_set(&segment_map2, d[j]);
        }

        size_t k = (i2 + 1) % population->length,
               l = k,
               j = k;
        for (size_t m = 0; m < population->length; ++m) {
            if (!(k >= i1 && k <= i2) && !InclusionMap_has(segment_map1, b[j])) {
                c[k] = b[j];
                InclusionMap_set(&segment_map1, b[j]);
                k = (k + 1) % population->length;
            }

            if (!(l >= i1 && l <= i2) && !InclusionMap_has(segment_map2, a[j])) {
                d[l] = a[j];
                InclusionMap_set(&segment_map2, a[j]);
                l = (l + 1) % population->length;
            }

            j = (j + 1) % population->length;
        }

        free(segment_map1.map);
        free(segment_map2.map);
        //
        // LOG("c: ");
        // Chromosome_print(c, population->length);
        //
        // LOG("d: ");
        // Chromosome_print(d, population->length);

        // PRINT("added [%li]", i);
        new_chromossomes[i] = c;
        if (i + 1 < population->size) {
            // PRINT("added [%li]", i + 1);
            new_chromossomes[i + 1] = d;
        }
    }

    for (size_t i = GEN_ELITE_COUNT; i < population->size; ++i) {
        size_t rank = population->ranking_table[i];
        free(population->chromosomes[rank]);
    }

    population->chromosomes = new_chromossomes;
}

static void Population_mutate(Population *population, double rate) {
    for (size_t i = GEN_ELITE_COUNT; i < population->size; ++i) {
        for (size_t j = 0; j < (size_t)((double)population->length * rate); ++j) {
            size_t a = get_rand(population->length - 1) + 1;
            size_t b = get_rand(population->length - 1) + 1;

            swap(
                &population->chromosomes[i][a],
                &population->chromosomes[i][b]
            );
        }
    }
}

static void elite_selection(Population *population, Gene **new_chromosomes) {
    // START_LOG("top chromosomes:");
    for (size_t i = 0; i < GEN_ELITE_COUNT; ++i) {
        size_t rank = population->ranking_table[i];
        // LOG("%li: [%li] (%lf)\n", i, rank, population->fitness_table[rank]);
        new_chromosomes[i] = population->chromosomes[rank];
    }
    // END_LOG("top chromosomes:");
}

static Path *Population_best_path(Population *population) {
    Gene *best_chromosome = population->chromosomes[population->ranking_table[0]];
    Path *path = Path_new(best_chromosome[0]);

    for (size_t i = 1; i < population->length; ++i) {
        Path_append(path, best_chromosome[i]);
    }

    return path;
}

static Gene *Path_to_chromosome(Path *path) {
    Gene *chromosome = malloc(sizeof(Gene) * path->length);
    Edge *actual = path->first_edge;

    // START_LOG("chromosome");
    for (size_t i = 0; i < (size_t)path->length; ++i) {
        chromosome[i] = (int32_t)actual->vertex;
        actual = actual->next;
        // OUTPUT("(%li) -> ", actual->vertex);
    }
    // END_LOG("chromosome");

    return chromosome;
}

static double Graph_run_chromosome(Graph *graph, Gene *chromossome, size_t length) {
    double total_distance = 0;

    for (size_t i = 1; i < length; ++i) {
        double distance = Coord_distance(
            Graph_get(graph, chromossome[i - 1]),
            Graph_get(graph, chromossome[i])
        );

        total_distance += distance;
    }

    total_distance += Coord_distance(
        Graph_get(graph, chromossome[length - 1]),
        Graph_get(graph, chromossome[0])
    );

    return total_distance;
}

static void Chromosome_print(Gene *chromosome, size_t length) {
    for (size_t j = 0; j < length; ++j) {
        OUTPUT("(%i) -> ", chromosome[j]);
    }
    OUTPUT("\n");
}

#pragma clang diagnostic pop