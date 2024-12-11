#ifndef MOA2024_CAIXEIRO_VIAJANTE_NEAREST_INSERTION_H
#define MOA2024_CAIXEIRO_VIAJANTE_NEAREST_INSERTION_H

#include <stdint.h>
#include <float.h>

#include "../../graph/graph.h"
#include "../../coord/coord.h"
#include "../../path/path.h"

Path *build_nearest_insertion(Graph *graph, size_t from);

#endif //MOA2024_CAIXEIRO_VIAJANTE_NEAREST_INSERTION_H
