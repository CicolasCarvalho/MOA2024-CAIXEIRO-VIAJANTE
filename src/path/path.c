#include "path.h"

Path *Path_new(Graph *graph) {
    Path *path = malloc(sizeof(Path));

    path->graph = graph;
    path->length = 0;
    path->vertices = malloc(sizeof(int32_t) * (graph->vertices_num - 1));

    return path;
}

void Path_append(Path *path, size_t from_idx, size_t to_idx) {
    Coord from_coord = Graph_get(path->graph, from_idx);
    Coord to_coord = Graph_get(path->graph, to_idx);

    path->vertices[path->length++] = to_idx;

    path->distance += Coord_distance(from_coord, to_coord);
}

bool Path_has(Path *path, size_t idx) {
    for (size_t i = 0; i < path->length; ++i) {
        if (idx == path->vertices[i])
            return true;
    }

    return false;
}