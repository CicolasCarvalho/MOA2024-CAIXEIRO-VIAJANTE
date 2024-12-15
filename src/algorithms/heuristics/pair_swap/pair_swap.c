#include "pair_swap.h"

//-declarations---------------------------------------------------------------------------------------------------------

// static void pair_swap(Path *path, size_t i, size_t j);

//-functions------------------------------------------------------------------------------------------------------------

void apply_pair_swap(Graph *graph, Path *path) {
    START_LOG("pair_swap");

    Edge *i_edge = path->first_edge->next,
         *j_edge;

    Coord i_coord = Graph_get(graph, i_edge->vertex),
          j_coord;

    Coord i_source = Graph_get(graph, i_edge->previous->vertex),
          j_source,
          i_next = Graph_get(graph, i_edge->next->vertex),
          j_next;

    // double actual_distance = Path_get_distance(graph, path);
    size_t i = 1, j;

    while (i_edge != path->first_edge->previous) {
        j_edge = i_edge->next;
        j_coord = Graph_get(graph, j_edge->vertex);
        j_source = Graph_get(graph, j_edge->previous->vertex);
        j_next = Graph_get(graph, j_edge->next->vertex);

        j = i + 1;
        while (j_edge != path->first_edge) {
            double actual_distance =
                    i_edge->distance        + // (0 -> 1)
                    i_edge->next->distance  + // (1 -> 2)
                    j_edge->distance        + // (3 -> 4)
                    j_edge->next->distance  ; // (4 -> 5)

            double new_distance =
                    Coord_distance(i_source, j_coord)   + // (0 -> 4) // i_edge
                    Coord_distance(j_coord, i_next)     + // (4 -> 2) // i_next
                    Coord_distance(j_source, i_coord)   + // (3 -> 1) // j_edge
                    Coord_distance(i_coord, j_next)     ; // (1 -> 5) // j_next

            if (j == i + 1) {
                actual_distance -= j_edge->distance;

                new_distance += j_edge->distance;
            }

            if (new_distance < actual_distance) {
                size_t tmp = i_edge->vertex;
                i_edge->vertex = j_edge->vertex;
                i_coord = Graph_get(graph, i_edge->vertex);
                j_edge->vertex = tmp;

                j_edge = i_edge->next;
                j = i + 2;

                Path_update_distance(graph, path);
                PRINT("(%li, %li): [%li, %li]: %f -> %f", j_edge->vertex, i_edge->vertex, i, j, actual_distance, new_distance);
            } else {
                j_edge = j_edge->next;
                j++;
            }

            j_coord = Graph_get(graph, j_edge->vertex);
            j_source = Graph_get(graph, j_edge->previous->vertex);
            j_next = Graph_get(graph, j_edge->next->vertex);
        }

        i_edge = i_edge->next;
        i_coord = Graph_get(graph, i_edge->vertex);
        i_source = Graph_get(graph, i_edge->previous->vertex);
        i_next = Graph_get(graph, i_edge->next->vertex);
        i++;
    }

    Path_update_distance(graph, path);

    END_LOG("pair_swap");
}

//-static---------------------------------------------------------------------------------------------------------------

// static void pair_swap(Path *path, size_t i, size_t j) {
//
// }