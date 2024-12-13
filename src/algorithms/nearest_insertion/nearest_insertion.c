#include "nearest_insertion.h"

//-definitions----------------------------------------------------------------------------------------------------------


//-declarations---------------------------------------------------------------------------------------------------------

static size_t update_closest_vertex(Graph *graph, double *distance_cache, size_t last_idx);
static size_t insertion_step(Graph *graph, Path *path, size_t vertex_idx);

//-functions------------------------------------------------------------------------------------------------------------

Path *build_nearest_insertion(Graph *graph, size_t from) {
    Path *path = Path_new(graph, from);
    size_t last_insertion = from;

    double *distance_cache = malloc(sizeof(double) * graph->vertices_num);
    for (size_t i = 0; i < (size_t)graph->vertices_num; ++i) {
        distance_cache[i] = DBL_MAX;
    }

    update_closest_vertex(graph, distance_cache, from);
    // preset subtour
    for (size_t i = from + 1; i < from + 3; ++i) {
        Path_append(path, i, Coord_distance(
            Graph_get(graph, i - 1),
            Graph_get(graph, i)
        ));
        last_insertion = update_closest_vertex(graph, distance_cache, i);
    }

    while(path->length < graph->vertices_num) {
        last_insertion = update_closest_vertex(graph, distance_cache, last_insertion);
        PRINT("closest: %li", last_insertion);
        insertion_step(graph, path, last_insertion);
        // Path_print(path);
    }

    free(distance_cache);

    return path;
}

//-static---------------------------------------------------------------------------------------------------------------

static size_t update_closest_vertex(Graph *graph, double *distance_cache, size_t last_idx) {
    if (distance_cache == NULL) {
        RAISE("distance cache is null");
    }

    Coord last_coord = Graph_get(graph, last_idx);

    double lowest_distance = DBL_MAX;
    size_t lowest_idx = 0;

    for (size_t i = 0; i < (size_t)graph->vertices_num; ++i) {
        Coord i_coord = Graph_get(graph, i);
        double distance = Coord_distance(i_coord, last_coord);

        if (distance < distance_cache[i])
            distance_cache[i] = distance;

        if (distance_cache[i] <= 0)
            continue;

        if (distance_cache[i] < lowest_distance) {
            lowest_distance = distance_cache[i];
            lowest_idx = i;
        }
    }

    return lowest_idx;
}

static size_t insertion_step(Graph *graph, Path *path, size_t vertex_idx) {
    Coord vertex_coord = Graph_get(graph, vertex_idx);
    size_t lowest_position = 0;
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
            lowest_position = i;
        }

        i++;
        actual = actual->next;
    } while (actual != path->first_edge);

    Path_insert(path, lowest_position + 1, vertex_idx, 0);

    // if (vertex_idx % 100 == 0) {
        PRINT("%li:\t(%li)", lowest_position + 1, vertex_idx);
    // }
    return lowest_position + 1;
}