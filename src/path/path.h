#ifndef MOA2024_CAIXEIRO_VIAJANTE_PATH_H
#define MOA2024_CAIXEIRO_VIAJANTE_PATH_H

#include <stdint.h>
#include <stdlib.h>

#include "../graph/graph.h"

typedef struct Edge Edge;

struct Edge {
    size_t vertex;
    double distance;

    Edge *next;
    Edge *previous;
};

typedef struct {
    char *inclusion_map;

    int32_t length;
    Edge *first_edge;
} Path;

Path *Path_new(Graph *graph, size_t starting_point);
void Path_free(Path *path);

void Path_append(Path *path, size_t to_idx, double distance);
void Path_insert(Path *path, size_t index, size_t to_idx, double distance);
bool Path_has(Path *path, size_t idx);
void Path_print(Path *path);
void Path_print_reverse(Path *path);

double Path_get_distance(Graph *graph, Path *path);
double Path_update_distance(Graph *graph, Path *path);

#endif //MOA2024_CAIXEIRO_VIAJANTE_PATH_H
