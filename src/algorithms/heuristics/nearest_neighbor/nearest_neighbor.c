#include "nearest_neighbor.h"

//-declarations---------------------------------------------------------------------------------------------------------

static size_t nearest_step(Graph *graph, Path *path, size_t source_idx);

//-functions------------------------------------------------------------------------------------------------------------

Path *build_nearest_neighbor(Graph *graph, size_t from) {
    START_LOG("nearest_neighbor");
    Path *path = Path_new(from);

    size_t actual_idx = from;

    while(path->length < graph->vertices_num) {
        size_t lowest_idx = nearest_step(graph, path, actual_idx);
        
        PRINT("(%li) -> (%li)", actual_idx, lowest_idx);
        actual_idx = lowest_idx;
    }

    // Path_update_distance(graph, path);
    END_LOG("nearest_neighbor");
    return path;
}

//-static---------------------------------------------------------------------------------------------------------------

static size_t nearest_step(Graph *graph, Path *path, size_t source_idx) {
    Coord source_coord = Graph_get(graph, source_idx);

    size_t lowest_idx = 0;
    double lowest_distance = DBL_MAX;

    for (size_t i = 0; i < (size_t)graph->vertices_num; ++i) {
        if (i == source_idx || Path_has(path, i)) {
            continue;
        }

        Coord i_coord = Graph_get(graph, i);
        double distance = Coord_distance(source_coord, i_coord);

        if (distance < lowest_distance) {
            lowest_distance = distance;
            lowest_idx = i;
        }
    }

    Path_append(path, lowest_idx);

    return lowest_idx;
}