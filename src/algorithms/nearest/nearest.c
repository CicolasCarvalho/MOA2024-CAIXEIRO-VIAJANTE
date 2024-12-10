#include "nearest.h"

//-declarations---------------------------------------------------------------------------------------------------------

static size_t nearest_step(Path *path, size_t source_idx);

//-functions------------------------------------------------------------------------------------------------------------

Path *build_nearest_neighbor(Graph *graph, size_t from) {
    Path *path = Path_new(graph);

    size_t actual_idx = from;

    for (size_t i = 0; i < graph->vertices_num; ++i) {
        size_t lowest_idx = nearest_step(path, actual_idx);

        PRINT("(%i) -> (%i)", actual_idx, lowest_idx);
    }
    
    return path;
}

//-static---------------------------------------------------------------------------------------------------------------

static size_t nearest_step(Path *path, size_t source_idx) {
    Coord source_coord = Graph_get(path->graph, source_idx);

    size_t lowest_idx = 0;
    double lowest_distance = DBL_MAX;

    for (size_t i = 0; i < path->graph->vertices_num; ++i) {
        if (i == source_idx || Path_has(path, i)) continue;

        Coord i_coord = Graph_get(path->graph, i);
        double distance = Coord_distance(source_coord, i_coord);

        if (distance < lowest_distance) {
            lowest_distance = distance;
            lowest_idx = i;
        }
    }

    Path_append(path, source_idx, lowest_idx);

    return lowest_idx;
}