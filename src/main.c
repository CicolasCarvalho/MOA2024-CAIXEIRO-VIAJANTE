#include <stdio.h>
#include <string.h>

#include "./parser/parser.h"
#include "./algorithms/heuristics/nearest_neighbor/nearest_neighbor.h"
#include "./algorithms/heuristics/nearest_insertion/nearest_insertion.h"
#include "./algorithms/heuristics/pair_swap/pair_swap.h"
#include "./algorithms/heuristics/two_opt/two_opt.h"
#include "defs.h"

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
    shift_arg(&argc, &argv);

    if (argc <= 1) {
        LOG("Esperado um argumento contendo o arquivo de entrada\n./main [--nearest_neighbor|--nearest_insertion] <caminho>\n");
        return 0;
    }

    char *flag = shift_arg(&argc, &argv);

    char *path = shift_arg(&argc, &argv);

    Graph *graph = graph_from_tsp(path);
    OUTPUT("\n---#----Graph Size: (%i)----#---\n", graph->vertices_num);

    if (strcmp(flag, "--nearest_neighbor") == 0 || IS_BENCHMARK) {
        double before = get_time();
        OUTPUT("\nNearest Neighbor:\n---#---------#---\n");

        Path *nearest_neighbor_path = build_nearest_neighbor(graph, 0);

        if (!IS_BENCHMARK) {
            OUTPUT("Edges:\n\n");
            Path_print(nearest_neighbor_path);
        }

        apply_two_opt(graph, nearest_neighbor_path);

        if (!IS_BENCHMARK) {
            OUTPUT("Edges:\n\n");
            Path_print(nearest_neighbor_path);
        }

        Path_free(nearest_neighbor_path);
        OUTPUT("\nTempo decorrido: %lfs\n", get_time() - before);
    }

    if (strcmp(flag, "--nearest_insertion") == 0 || IS_BENCHMARK) {
        double before = get_time();
        OUTPUT("\nNearest Insertion:\n---#----------#---\n");

        Path *nearest_insertion_path = build_nearest_insertion(graph, 0);

        if (!IS_BENCHMARK) {
            OUTPUT("Edges:\n\n");
            Path_print(nearest_insertion_path);
        }
        Path_free(nearest_insertion_path);
        OUTPUT("\nTempo decorrido: %lfs\n", get_time() - before);
    }

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