#include "nearest_insertion.h"

//-declarations---------------------------------------------------------------------------------------------------------

static size_t get_closest_vertex(Graph *graph, Path *path);
static size_t insertion_step(Graph *graph, Path *path, size_t vertex_idx);

//-functions------------------------------------------------------------------------------------------------------------

Path *build_nearest_insertion(Graph *graph, size_t from) {
    Path *path = Path_new(graph, from);
    Path_append(path, from + 1, Coord_distance(
            Graph_get(graph, from),
            Graph_get(graph, from + 1)
    ));
    Path_append(path, from + 2, Coord_distance(
        Graph_get(graph, from + 1),
        Graph_get(graph, from + 2)
    ));

    while(path->length < graph->vertices_num) {
        size_t closest_idx = get_closest_vertex(graph, path);
        PRINT("closest: %li", closest_idx);
        insertion_step(graph, path, closest_idx);
        // Path_print(path);
    }

    return path;
}

//-static---------------------------------------------------------------------------------------------------------------

static size_t get_closest_vertex(Graph *graph, Path *path) {
    size_t lowest_idx = 0;
    double lowest_distance = DBL_MAX;

    for (size_t i = 0; i < (size_t)graph->vertices_num; ++i) {
        // if (Path_has(path, i)) continue;

        Coord i_coord = Graph_get(graph, i);
        Edge *actual = path->first_edge;

        do {
            if (i == actual->vertex) break;

            Coord j_coord = Graph_get(graph, actual->vertex);
            double distance = Coord_distance(i_coord, j_coord);

            // PRINT("%li, (%li) -> (%li): %lf %lf", j, i, actual->vertex, distance, lowest_distance);

            if (distance < lowest_distance) {
                lowest_distance = distance;
                lowest_idx = i;
            }

            actual = actual->next;
        } while (actual != path->first_edge);
    }

    return lowest_idx;
}

static size_t insertion_step(Graph *graph, Path *path, size_t vertex_idx) {
    Coord vertex_coord = Graph_get(graph, vertex_idx);
    size_t lowest_idx = 0;
    double lowest_distance = DBL_MAX;

    Edge *actual = path->first_edge;
    size_t i = 0;

    do {
        Coord actual_coord = Graph_get(graph, actual->vertex);
        Coord next_coord = Graph_get(graph, actual->next->vertex);

        double total_distance =
                Coord_distance(actual_coord, vertex_coord) +
                Coord_distance(vertex_coord, next_coord);

        if (total_distance < lowest_distance) {
            lowest_distance = total_distance;
            lowest_idx = i;
        }

        i++;
        actual = actual->next;
    } while (actual != path->first_edge);

    Path_insert(path, lowest_idx + 1, vertex_idx, 0);

    // if (vertex_idx % 100 == 0) {
        PRINT("%li:\t(%li)", lowest_idx + 1, vertex_idx);
    // }
    return lowest_idx + 1;
}