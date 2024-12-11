#include <stdio.h>
#include <string.h>

#include "./parser/parser.h"
#include "./algorithms/nearest_neighbor/nearest_neighbor.h"
#include "./algorithms/nearest_insertion/nearest_insertion.h"

#ifdef WIN32

#include <windows.h>
double get_time()
{
    LARGE_INTEGER t, f;
    QueryPerformanceCounter(&t);
    QueryPerformanceFrequency(&f);
    return (double)t.QuadPart/(double)f.QuadPart;
}

#else

#include <sys/time.h>
#include <sys/resource.h>

double get_time()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec*1e-6;
}

#endif

char *shift_arg(int *argc, char ***argv);

int main(int argc, char **argv) {
    // Graph *graph = Graph_new(4);
    // graph->vertices[0] = (Coord) {
    //         .x = 0,
    //         .y = 0
    // };
    // graph->vertices[1] = (Coord) {
    //         .x = 1,
    //         .y = 1
    // };
    // graph->vertices[3] = (Coord) {
    //         .x = 1,
    //         .y = 0
    // };
    // graph->vertices[2] = (Coord) {
    //         .x = 5,
    //         .y = 3
    // };
    //
    // Path *path = build_nearest_insertion(graph, 0);
    //
    // Path_print(path);
    // return 0;
    shift_arg(&argc, &argv);

    if (argc <= 1) {
        LOG("Esperado um argumento contendo o arquivo de entrada\n./main [--nearest_neighbor|] <caminho>\n");
        return 0;
    }

    char *flag = shift_arg(&argc, &argv);

    char *path = shift_arg(&argc, &argv);

    Graph *graph = graph_from_tsp(path);
    OUTPUT("Graph Size: %i\n", graph->vertices_num);

    double before = get_time();

    if (strcmp(flag, "--nearest_neighbor") == 0) {
        START_LOG("nearest_neighbor");
        OUTPUT("\nNearest Neighbor:\n---#---------#---\nEdges:\n\n");

        Path *nearest_neighbor_path = build_nearest_neighbor(graph, 0);
        // Path_print(nearest_neighbor_path);
        Path_free(nearest_neighbor_path);

        END_LOG("nearest_neighbor");
    } else if (strcmp(flag, "--nearest_insertion") == 0) {
        START_LOG("nearest_insertion");
        OUTPUT("\nNearest Insertion:\n---#----------#---\n");

        Path *nearest_insertion_path = build_nearest_insertion(graph, 0);
        // Path_print(nearest_insertion_path);
        Path_free(nearest_insertion_path);

        END_LOG("nearest_insertion");
    }

    OUTPUT("\nTempo decorrido: %lfs\n", get_time() - before);

    Graph_free(graph);
    return 0;
}

char *shift_arg(int *argc, char ***argv) {
    if (*argc == 0) {
        RAISE("Não há argumentos a serem pegos");
    }

    char *str = **argv;
    (*argv)++;
    (*argc)--;

    return str;
}