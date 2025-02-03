#ifndef MOA2024_CAIXEIRO_VIAJANTE_GENETIC_ALGORITHM_H
#define MOA2024_CAIXEIRO_VIAJANTE_GENETIC_ALGORITHM_H

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <omp.h>

#include "../../../graph/graph.h"
#include "../../../coord/coord.h"
#include "../../../path/path.h"
#include "../../../utils/utils.h"
#include "../../heuristics/nearest_neighbor_random/nearest_neighbor_random.h"
#include "../../heuristics/two_opt/two_opt.h"

typedef enum {
    PMX_CROSSOVER,
    OX1_CROSSOVER,
} CrossoverAlgorithm;

typedef enum {
    RANKING_SELECTION,
    TOURNAMENT_SELECTION
} SelectionAlgorithm;

Path *build_genetic_algorithm(Graph *graph, size_t from, CrossoverAlgorithm crossoverAlgorithm, int population_size, double max_time);

#endif //MOA2024_CAIXEIRO_VIAJANTE_GENETIC_ALGORITHM_H
