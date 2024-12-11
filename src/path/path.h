#ifndef MOA2024_CAIXEIRO_VIAJANTE_PATH_H
#define MOA2024_CAIXEIRO_VIAJANTE_PATH_H

#include <stdint.h>
#include <stdlib.h>

#include "../graph/graph.h"

typedef struct {
    Graph *graph;

    size_t length;
    size_t *vertices;

    size_t starting_point;
    double distance;
} Path;

Path *Path_new(Graph *graph, size_t starting_point);
void Path_append(Path *path, size_t from_idx, size_t to_idx);
bool Path_has(Path *path, size_t idx);

#endif //MOA2024_CAIXEIRO_VIAJANTE_PATH_H
