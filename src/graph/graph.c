#include "graph.h"

Graph *Graph_new(int32_t vertices_num) {
    Graph *grafo = malloc(sizeof(Graph));

    grafo->vertices_num = vertices_num;
    grafo->vertices = malloc(sizeof(Coord) * vertices_num);

    return grafo;
}

void Graph_free(Graph *graph) {
    free(graph->vertices);
    free(graph);
}

void Graph_set(Graph *graph, size_t i, Coord value) {
    // if (i >= (size_t)graph->vertices_num) {
    //     RAISE("índice invalido '%i', graph com %i vertices", (int32_t)i, (int32_t)graph->vertices_num);
    // }

    graph->vertices[i] = value;
}

Coord Graph_get(Graph *graph, size_t i) {
    // if (i >= (size_t)graph->vertices_num) {
    //     RAISE("índice invalido '%i', graph com %i vertices", (int32_t)i, (int32_t)graph->vertices_num);
    // }

    return graph->vertices[i];
}

double Graph_run_path(Graph *graph, Path *path) {
    Edge *actual = path->first_edge;
    double total_distance = 0;

    do {
        double distance = Coord_distance(
            Graph_get(graph, actual->previous->vertex),
            Graph_get(graph, actual->vertex)
        );

        total_distance += distance;

        actual = actual->next;
    } while (actual != path->first_edge);

    return total_distance;
}

// double Path_update_distance(Graph *graph, Path *path) {
//     Edge *actual = path->first_edge;
//     double total_distance = 0;
//
//     do {
//         double new = Coord_distance(
//             Graph_get(graph, actual->previous->vertex),
//             Graph_get(graph, actual->vertex)
//         );
//
//         actual->distance = new;
//
//         total_distance += actual->distance;
//         actual = actual->next;
//     } while (actual != path->first_edge);
//
//     return total_distance;
// }