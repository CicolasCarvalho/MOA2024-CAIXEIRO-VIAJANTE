#include "graph.h"

Graph *Graph_new(int32_t vertices_num) {
    Graph *grafo = malloc(sizeof *grafo);

    grafo->vertices_num = vertices_num;
    grafo->vertices = calloc(vertices_num * vertices_num, sizeof *grafo->vertices);

    return grafo;
}

void Graph_set(Graph *graph, size_t i, Coord value) {
    if (i >= graph->vertices_num) {
        RAISE("índice invalido '%i', graph com %i vertices", (int32_t)i, (int32_t)graph->vertices_num);
    }

    graph->vertices[i] = value;
}

Coord Graph_get(Graph *graph, size_t i) {
    if (i >= graph->vertices_num) {
        RAISE("índice invalido '%i', graph com %i vertices", (int32_t)i, (int32_t)graph->vertices_num);
    }

    return graph->vertices[i];
}