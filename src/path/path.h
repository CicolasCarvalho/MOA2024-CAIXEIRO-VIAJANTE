#ifndef MOA2024_CAIXEIRO_VIAJANTE_PATH_H
#define MOA2024_CAIXEIRO_VIAJANTE_PATH_H

#include <stdint.h>
#include <stdlib.h>

#include "../defs.h"

#define INCLUSION_MAP_CHUNK 128

typedef struct Edge Edge;
typedef struct InclusionMap InclusionMap;

struct Edge {
    size_t vertex;

    Edge *next;
    Edge *previous;
};

struct InclusionMap {
    char *map;
    int32_t capacity;
};

typedef struct {
    InclusionMap inclusion_map;

    int32_t length;
    Edge *first_edge;
} Path;

Path *Path_new(size_t starting_point);
void Path_free(Path *path);

void Path_append(Path *path, size_t to_idx);
void Path_insert(Path *path, size_t index, size_t to_idx);
bool Path_has(Path *path, size_t idx);
void Path_print(Path *path);
void Path_print_reverse(Path *path);

InclusionMap InclusionMap_new(void);
void InclusionMap_set(InclusionMap *inclusion_map, size_t idx);
bool InclusionMap_has(InclusionMap inclusion_map, size_t idx);

#endif //MOA2024_CAIXEIRO_VIAJANTE_PATH_H
