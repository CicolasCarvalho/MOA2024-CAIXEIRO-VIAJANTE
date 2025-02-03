#include "path.h"

//-declarations---------------------------------------------------------------------------------------------------------

static Edge *Edge_new(size_t vertex) {
    Edge *edge = malloc(sizeof(Edge));

    edge->vertex = vertex;
    edge->next = NULL;
    edge->previous = NULL;

    return edge;
}

//-functions------------------------------------------------------------------------------------------------------------

Path *Path_new(size_t starting_point) {
    Path *path = malloc(sizeof(Path));

    path->length = 0;
    path->first_edge = NULL;
    path->inclusion_map = InclusionMap_new();

    Path_append(path, starting_point);

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
    free(path->inclusion_map.map);
    free(path);
}

void Path_append(Path *path, size_t to_idx) {
    Edge *new_edge = Edge_new(to_idx);

    if (path->first_edge == NULL) {
        new_edge->next = new_edge;
        new_edge->previous = new_edge;

        path->first_edge = new_edge;

        InclusionMap_set(&path->inclusion_map, to_idx);
        path->length++;
        return;
    }

    path->first_edge->previous->next = new_edge;

    new_edge->next = path->first_edge;
    new_edge->previous = path->first_edge->previous;

    path->first_edge->previous = new_edge;

    InclusionMap_set(&path->inclusion_map, to_idx);
    path->length++;
}

void Path_insert(Path *path, size_t index, size_t to_idx) {
    if (!IS_BENCHMARK && index > (size_t)path->length) {
        RAISE("indice fora dos limites do caminho [0; %i]", path->length);
    }

    if (index == (size_t)path->length) {
        Path_append(path, to_idx);
        return;
    }

    Edge *actual = path->first_edge;

    for (size_t i = 0; i < index; ++i) {
        actual = actual->next;
    }

    Edge *new_edge = Edge_new(to_idx);
    new_edge->previous = actual->previous;
    new_edge->next = actual;

    actual->previous->next = new_edge;
    actual->previous = new_edge;

    if (index == 0) path->first_edge = new_edge;

    InclusionMap_set(&path->inclusion_map, to_idx);
    path->length++;
}

bool Path_has(Path *path, size_t idx) {
    return InclusionMap_has(path->inclusion_map, idx);
}

void Path_print(Path *path) {
    START_LOG("path:");
    Edge *actual = path->first_edge;

    do {
        // LOG("%li: ", actual);
        OUTPUT("(%li)\t->\t", actual->vertex);

        actual = actual->next;
    } while (actual != path->first_edge);

    OUTPUT("(%li)\n\n", actual->vertex);

    END_LOG("path");
}

void Path_print_reverse(Path *path) {
    START_LOG("path:");
    Edge *actual = path->first_edge;

    do {
        // LOG("%li: ", actual);
        OUTPUT("(%li)\t<-\t", actual->vertex);

        actual = actual->previous;
    } while (actual != path->first_edge);

    OUTPUT("(%li)\n\n", actual->vertex);

    END_LOG("path");
}

InclusionMap InclusionMap_new(void) {
    InclusionMap inclusion_map;

    inclusion_map.map = calloc(INCLUSION_MAP_CHUNK, sizeof(char));
    inclusion_map.capacity = INCLUSION_MAP_CHUNK;

    return inclusion_map;
}

void InclusionMap_set(InclusionMap *inclusion_map, size_t idx) {
    size_t inclusion_idx = idx / 8;

    while (inclusion_idx >= (size_t)inclusion_map->capacity) {
        // PRINT("realloc inclusion map: %i -> %i", inclusion_map->capacity, inclusion_map->capacity * 2);
        void *tmp = realloc(inclusion_map->map, sizeof(char) * inclusion_map->capacity * 2);
        if (tmp == NULL) { RAISE("StackOverFlow!"); }
        inclusion_map->map = tmp;
        for (size_t i = inclusion_map->capacity; i < (size_t)inclusion_map->capacity * 2; ++i) {
            inclusion_map->map[i] = 0;
        }
        inclusion_map->capacity *= 2;
    }

    char *chunk = &(inclusion_map->map[inclusion_idx]);
    (*chunk) |= 0b1 << (idx % 8); // NOLINT(*-narrowing-conversions)

    // PRINT(
    //     "%li: [%li]: (%i%i%i%i%i%i%i%i)",
    //     idx,
    //     inclusion_idx,
    //     ((*chunk) & 0b10000000) >> 7,
    //     ((*chunk) & 0b01000000) >> 6,
    //     ((*chunk) & 0b00100000) >> 5,
    //     ((*chunk) & 0b00010000) >> 4,
    //     ((*chunk) & 0b00001000) >> 3,
    //     ((*chunk) & 0b00000100) >> 2,
    //     ((*chunk) & 0b00000010) >> 1,
    //     ((*chunk) & 0b00000001)
    // );
}

bool InclusionMap_has(InclusionMap inclusion_map, size_t idx) {
    size_t inclusion_idx = idx / 8;

    if (inclusion_idx >= (size_t)inclusion_map.capacity) return false;
    char chunk = inclusion_map.map[inclusion_idx];

    return chunk & (0b1 << (idx % 8));
}