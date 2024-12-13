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
    path->length = 0;
    path->first_edge = NULL;

    Path_append(path, starting_point, 0);

    return path;
}

void Path_free(Path *path) {
    Edge *actual = path->first_edge->previous;
    while (actual != path->first_edge) {
        Edge *tmp = actual;
        actual = actual->previous;
        free(tmp);
    }
    free(path->first_edge);
    free(path->inclusion_map);
    free(path);
}

void Path_append(Path *path, size_t to_idx, double distance) {
    Edge *new_edge = Edge_new(to_idx, distance);

    if (path->first_edge == NULL) {
        new_edge->next = new_edge;
        new_edge->previous = new_edge;

        path->first_edge = new_edge;

        Path_add_vertex_to_map(path, to_idx);
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
    if (!IS_BENCHMARK && index > (size_t)path->length) {
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
}

void Path_print(Path *path) {
    START_LOG("path:");
    Edge *actual = path->first_edge;
    double total_distance = 0;

    do {
        // LOG("%li: ", actual);
        OUTPUT("(%li)\t->\t", actual->vertex);

        total_distance += actual->distance;
        actual = actual->next;
    } while (actual != path->first_edge);

    OUTPUT("(%li)\n\n", actual->vertex);
    OUTPUT("Total Distance: %f\n", total_distance);

    END_LOG("path");
}

double Path_update_distance(Graph *graph, Path *path) {
    Edge *actual = path->first_edge;
    double total_distance = 0;

    do {
        actual->distance = Coord_distance(
            Graph_get(graph, actual->vertex),
            Graph_get(graph, actual->next->vertex)
        );

        total_distance += actual->distance;
        actual = actual->next;
    } while (actual != path->first_edge);

    return total_distance;
}

//-static---------------------------------------------------------------------------------------------------------------

static void Path_add_vertex_to_map(Path *path, size_t idx) {
    size_t inclusion_idx = idx / 8;
    char *chunk = &path->inclusion_map[inclusion_idx];
    (*chunk) |= 0b1 << (idx % 8); // NOLINT(*-narrowing-conversions)

    PRINT(
            "%li: [%li]: (%i%i%i%i%i%i%i%i)",
            idx,
            inclusion_idx,
            ((*chunk) & 0b10000000) >> 7,
            ((*chunk) & 0b01000000) >> 6,
            ((*chunk) & 0b00100000) >> 5,
            ((*chunk) & 0b00010000) >> 4,
            ((*chunk) & 0b00001000) >> 3,
            ((*chunk) & 0b00000100) >> 2,
            ((*chunk) & 0b00000010) >> 1,
            ((*chunk) & 0b00000001)
    );
}