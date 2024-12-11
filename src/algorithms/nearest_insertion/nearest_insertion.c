#include "nearest_insertion.h"

//-declarations---------------------------------------------------------------------------------------------------------

static size_t get_closest_vertex(Graph *graph, Path *path);
static size_t insertion_step(Graph *graph, Path *path, size_t vertex_idx);

//-functions------------------------------------------------------------------------------------------------------------

Path *build_nearest_insertion(Graph *graph, size_t from) {
    Path *path = Path_new(graph, from);
    // TODO: check vertice bounds
    Path_append(path, from, from + 1);
    Path_append(path, from + 1, from + 2);
    Path_append(path, from + 2, from);

    while(path->length < graph->vertices_num) {
        size_t closest_idx = get_closest_vertex(graph, path);
        insertion_step(graph, path, closest_idx);
    }

    return path;
}

//-static---------------------------------------------------------------------------------------------------------------

static size_t get_closest_vertex(Graph *graph, Path *path) {
    size_t lowest_idx = 0;
    double lowest_distance = DBL_MAX;

    for (size_t i = 0; i < (size_t)graph->vertices_num; ++i) {
        Coord i_coord = Graph_get(graph, i);
        if (Path_has(path, i)) continue;

        for (size_t j = 0; j < (size_t)path->length; ++j) {
            Coord j_coord = Graph_get(graph, j);
            double distance = Coord_distance(i_coord, j_coord);

            if (distance < lowest_distance) {
                lowest_distance = distance;
                lowest_idx = i;
            }
        }
    }

    return lowest_idx;
}

static size_t insertion_step(Graph *graph, Path *path, size_t vertex_idx) {
    Coord vertex_coord = Graph_get(graph, vertex_idx);
    size_t lowest_idx = 0;
    double lowest_distance = DBL_MAX;

    for (size_t i = 0; i < (size_t)path->length - 1; ++i) {
        Coord actual_coord = Graph_get(graph, path->vertices[i]);
        Coord next_coord = Graph_get(graph, path->vertices[i + 1]);

        double total_distance =
                Coord_distance(actual_coord, vertex_coord) +
                Coord_distance(vertex_coord, next_coord);

        if (total_distance < lowest_distance) {
            lowest_distance = total_distance;
            lowest_idx = i;
        }
    }

    Path_insert(path, lowest_idx + 1, vertex_idx);

    PRINT("%li on %li", vertex_idx, lowest_idx + 1);
    return lowest_idx + 1;
}