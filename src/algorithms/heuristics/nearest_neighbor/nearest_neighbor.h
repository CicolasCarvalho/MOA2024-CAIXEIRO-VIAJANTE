#ifndef MOA2024_CAIXEIRO_VIAJANTE_NEAREST_NEIGHBOR_H
#define MOA2024_CAIXEIRO_VIAJANTE_NEAREST_NEIGHBOR_H

#include <stdlib.h>
#include <stdint.h>
#include <float.h>

#include "../../../graph/graph.h"
#include "../../../coord/coord.h"
#include "../../../path/path.h"

Path *build_nearest_neighbor(Graph *graph, size_t from);

#endif //MOA2024_CAIXEIRO_VIAJANTE_NEAREST_NEIGHBOR_H

