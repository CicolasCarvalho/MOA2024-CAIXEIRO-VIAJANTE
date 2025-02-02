#include "two_opt.h"

//-declarations---------------------------------------------------------------------------------------------------------

static void reverse_edges(Edge *from, Edge *to);

//-functions------------------------------------------------------------------------------------------------------------

void apply_two_opt(Graph *graph, Path *path) {
    // START_LOG("two_opt");

    Edge *i_edge = path->first_edge->next,
         *j_edge;

    Coord i_coord = Graph_get(graph, i_edge->vertex),
          j_coord;

    Coord i_source = Graph_get(graph, i_edge->previous->vertex),
          j_source;

    size_t i = 1, j;

    while (i_edge != path->first_edge->previous->previous) {
        j_edge = i_edge->next->next;
        j_coord = Graph_get(graph, j_edge->vertex);
        j_source = Graph_get(graph, j_edge->previous->vertex);

        j = i + 2;
        while (j_edge != path->first_edge) {
            double actual_distance =
                    Coord_distance(i_source, i_coord) + // (0 -> 1)
                    Coord_distance(j_source, j_coord) ; // (4 -> 5)

            double new_distance =
                    Coord_distance(i_source, j_source)  + // (0 -> 3) // i_edge
                    Coord_distance(i_coord, j_coord)    ; // (3 -> 0) // j_edge

            if (new_distance < actual_distance) {
                // PRINT("(%li, %li): [%li, %li]: %f\t-> %f", i_edge->vertex, j_edge->vertex, i, j, actual_distance, new_distance);

                reverse_edges(i_edge, j_edge->previous);

                i_coord = Graph_get(graph, i_edge->vertex);


                j_edge = i_edge->next->next;
                j = i + 2;
            } else {
                j_edge = j_edge->next;
                j++;
            }

            j_coord = Graph_get(graph, j_edge->vertex);
            j_source = Graph_get(graph, j_edge->previous->vertex);
        }

        i_edge = i_edge->next;
        i_coord = Graph_get(graph, i_edge->vertex);
        i_source = Graph_get(graph, i_edge->previous->vertex);
        i++;
    }

    // END_LOG("two_opt");
}

//-static---------------------------------------------------------------------------------------------------------------

static void reverse_edges(Edge *from, Edge *to) {
    // inverter a ordem das arestas
    Edge *k_edge = from;
    Edge *l_edge = to;
    while (k_edge != l_edge) {
        size_t tmp_vertex = k_edge->vertex;
        k_edge->vertex = l_edge->vertex;
        l_edge->vertex = tmp_vertex;

        l_edge = l_edge->previous;
        if (k_edge != l_edge) k_edge = k_edge->next;
    }
}
