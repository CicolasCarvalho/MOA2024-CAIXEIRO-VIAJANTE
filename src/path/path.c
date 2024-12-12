#include "path.h"

//-declarations---------------------------------------------------------------------------------------------------------

static Edge *Edge_new(size_t vertex, double distance) {
    Edge *edge = malloc(sizeof(Edge));

    edge->vertex = vertex;
    edge->distance = distance;
    edge->next = NULL;
    edge->previous = NULL;

    return edge;
}

static void Path_add_vertex_to_map(Path *path, size_t idx);

//-functions------------------------------------------------------------------------------------------------------------


Path *Path_new(Graph *graph, size_t starting_point) {
    Path *path = malloc(sizeof(Path));

    path->inclusion_map = calloc(graph->vertices_num, sizeof(char));
    path->graph = graph;
    path->length = 0;
    path->first_edge = NULL;

    Path_append(path, starting_point, 0);

    return path;
}

void Path_free(Path *path) {
    // free(path->vertices);
    free(path);
}

void Path_append(Path *path, size_t to_idx, double distance) {
    Edge *new_edge = Edge_new(to_idx, distance);

    if (path->first_edge == NULL) {
        new_edge->next = new_edge;
        new_edge->previous = new_edge;

        path->first_edge = new_edge;

        path->length++;
        return;
    }

    path->first_edge->previous->next = new_edge;

    new_edge->next = path->first_edge;
    new_edge->previous = path->first_edge->previous;

    path->first_edge->previous = new_edge;

    Path_add_vertex_to_map(path, to_idx);
    path->length++;
}

void Path_insert(Path *path, size_t index, size_t to_idx, double distance) {
    if (path->length == path->graph->vertices_num) {
        RAISE("caminho já esta completo");
    }

    if (index > (size_t)path->length) {
        RAISE("indice fora dos limites do caminho [0; %i]", path->length);
    }

    if (index == (size_t)path->length) {
        Path_append(path, to_idx, distance);
        return;
    }

    Edge *actual = path->first_edge;

    for (size_t i = 0; i < index; ++i) {
        actual = actual->next;
    }

    Edge *new_edge = Edge_new(to_idx, distance);
    new_edge->previous = actual->previous;
    new_edge->next = actual;

    actual->previous->next = new_edge;
    actual->previous = new_edge;
    // *actual = new_edge;

    if (index == 0) path->first_edge = new_edge;

    Path_add_vertex_to_map(path, to_idx);
    path->length++;
}

bool Path_has(Path *path, size_t idx) {
    size_t inclusion_idx = idx / 8;
    char chunk = path->inclusion_map[inclusion_idx];
    return chunk & (0b1 << (idx % 8));

    // if (path->first_edge == NULL) return false;
    //
    // Edge *actual = path->first_edge;
    //
    // do {
    //     if (actual->vertex == idx) return true;
    //
    //     actual = actual->next;
    // } while(actual != path->first_edge);
    //
    // return false;
}

void Path_print(Path *path) {
    START_LOG("path:");
    Edge *actual = path->first_edge;

    do {
        // LOG("%li: ", actual);
        OUTPUT("(%li)\t->\t", actual->vertex);

        actual = actual->next;
    } while (actual != path->first_edge);

    OUTPUT("(%li)\n", actual->vertex);

    END_LOG("path");
}

//-static---------------------------------------------------------------------------------------------------------------

static void Path_add_vertex_to_map(Path *path, size_t idx) {
    size_t inclusion_idx = idx / 8;
    char *chunk = &path->inclusion_map[inclusion_idx];
    (*chunk) |= (0b1 << (idx % 8));
}