#include "pair_swap.h"

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

    size_t i = 1, j;

    while (i_edge != path->first_edge->previous) {
        j_edge = i_edge->next;
        j_coord = Graph_get(graph, j_edge->vertex);
        j_source = Graph_get(graph, j_edge->previous->vertex);
        j_next = Graph_get(graph, j_edge->next->vertex);

        j = i + 1;
        while (j_edge != path->first_edge) {
            double actual_distance =
                    Coord_distance(i_source, i_coord)   + // (0 -> 1)
                    Coord_distance(i_coord, i_next)     + // (1 -> 2)
                    Coord_distance(j_source, j_coord)   + // (0 -> 1)
                    Coord_distance(j_coord, j_next)     ; // (1 -> 2)

            double new_distance =
                    Coord_distance(i_source, j_coord)   + // (0 -> 4) // i_edge
                    Coord_distance(j_coord, i_next)     + // (4 -> 2) // i_next
                    Coord_distance(j_source, i_coord)   + // (3 -> 1) // j_edge
                    Coord_distance(i_coord, j_next)     ; // (1 -> 5) // j_next

            if (j == i + 1) {
                actual_distance -= Coord_distance(j_source, j_coord);
                new_distance += Coord_distance(j_source, j_coord);
            }

            if (new_distance < actual_distance) {
                PRINT("(%li, %li): [%li, %li]: %f\t-> %f", i_edge->vertex, j_edge->vertex, i, j, actual_distance, new_distance);

                size_t tmp = i_edge->vertex;
                i_edge->vertex = j_edge->vertex;
                j_edge->vertex = tmp;

                i_coord = Graph_get(graph, i_edge->vertex);

                j_edge = i_edge->next->next;
                j = i + 2;
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

    END_LOG("pair_swap");
}