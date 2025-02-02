#include "nearest_neighbor_random.h"

//-declarations---------------------------------------------------------------------------------------------------------

static size_t nearest_step(Graph *graph, Path *path, size_t source_idx);

//-functions------------------------------------------------------------------------------------------------------------

Path *build_nearest_neighbor_random(Graph *graph, size_t from) {
    // START_LOG("nearest_neighbor_random");
    Path *path = Path_new(from);

    size_t actual_idx = from;

    while(path->length < graph->vertices_num) {
        size_t lowest_idx = nearest_step(graph, path, actual_idx);
        
        actual_idx = lowest_idx;
    }

    // Path_update_distance(graph, path);
    // END_LOG("nearest_neighbor_random");
    return path;
}

//-static---------------------------------------------------------------------------------------------------------------

static size_t nearest_step(Graph *graph, Path *path, size_t source_idx) {
    Coord source_coord = Graph_get(graph, source_idx);

    int32_t lowest_idx[NEAREST_NEIGHBOR_RANDOM_SIZE];
    double lowest_distances[NEAREST_NEIGHBOR_RANDOM_SIZE];
    for (size_t i = 0; i < NEAREST_NEIGHBOR_RANDOM_SIZE; ++i) {
        lowest_idx[i] = -1;
        lowest_distances[i] = DBL_MAX;
    }

    for (size_t i = 0; i < (size_t)graph->vertices_num; ++i) {
        if (i == source_idx || Path_has(path, i)) {
            continue;
        }

        Coord i_coord = Graph_get(graph, i);
        double distance = Coord_distance(source_coord, i_coord);

        for (size_t j = 0; j < NEAREST_NEIGHBOR_RANDOM_SIZE; ++j) {
            if (lowest_idx[j] == -1 || distance < lowest_distances[j]) {
                for (size_t k = j + 1; k < NEAREST_NEIGHBOR_RANDOM_SIZE; ++k) {
                    lowest_idx[k] = lowest_idx[k - 1];
                    lowest_distances[k] = lowest_distances[k - 1];
                }

                lowest_idx[j] = i;
                lowest_distances[j] = distance;
                break;
            }
        }
    }

    int32_t random_idx;
    do {
        random_idx = lowest_idx[get_rand(NEAREST_NEIGHBOR_RANDOM_SIZE)];
        // PRINT("[%i]: %i", random_idx, random_idx);
    } while (random_idx < 0);
    Path_append(path, random_idx > 0 ? random_idx : -random_idx);

    return random_idx;
}