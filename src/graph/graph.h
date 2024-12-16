#ifndef MOA2024_CAIXEIRO_VIAJANTE_GRAPH_H
#define MOA2024_CAIXEIRO_VIAJANTE_GRAPH_H

#include <stdint.h>
#include <stdlib.h>

#include "../defs.h"
#include "../coord/coord.h"
#include "../path/path.h"

typedef struct {
    int32_t vertices_num;
    Coord *vertices;
} Graph;

Graph *Graph_new(int32_t vertices_num);
void Graph_free(Graph *graph);

void Graph_set(Graph *graph, size_t i, Coord value);
Coord Graph_get(Graph *graph, size_t i);

double Graph_run_path(Graph *graph, Path *path);

#endif //MOA2024_CAIXEIRO_VIAJANTE_GRAPH_H
