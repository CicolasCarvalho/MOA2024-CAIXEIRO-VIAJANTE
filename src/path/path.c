#include "path.h"

Path *Path_new(Graph *graph, size_t starting_point) {
    Path *path = malloc(sizeof(Path));

    path->graph = graph;
    path->length = 0;
    path->vertices = malloc(sizeof(size_t) * (graph->vertices_num));
    path->starting_point = starting_point;
    path->distance = 0;

    return path;
}

void Path_free(Path *path) {
    free(path->vertices);
    free(path);
}

void Path_append(Path *path, size_t from_idx, size_t to_idx) {
    // Coord from_coord = Graph_get(path->graph, from_idx);
    // Coord to_coord = Graph_get(path->graph, to_idx);

    path->vertices[path->length++] = to_idx;

    // path->distance += Coord_distance(from_coord, to_coord);
}

void Path_insert(Path *path, size_t index, size_t to_idx) {
    if (path->length == path->graph->vertices_num) {
        RAISE("caminho já esta completo");
    }

    if (index > (size_t)path->length) {
        RAISE("indice fora dos limites do caminho [0; %i]", path->length);
    }

    for (size_t i = path->length; i > index; --i) {
        path->vertices[i] = path->vertices[i - 1];
    }

    path->length++;
    path->vertices[index] = to_idx;
}

bool Path_has(Path *path, size_t idx) {
    for (size_t i = 0; i < (size_t)path->length; ++i) {
        if (idx == path->vertices[i])
            return true;
    }

    return false;
}

void Path_print(Path *path) {
    START_LOG("path:");
    OUTPUT("(%li)\t->\t", path->starting_point);

    for (size_t i = 0; i < (size_t)path->length - 1; ++i) {
        OUTPUT("(%li)\t->\t", path->vertices[i]);
    }

    OUTPUT("(%li)\n", path->vertices[path->length - 1]);
    END_LOG("path");
}