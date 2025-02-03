// Nícolas dos Santos Carvalho - RA128660
// João Pedro Zen Sirino - RA124360
// Maringá, PR - Brasil
// 18/12/2024
// Resolução PCV com Métodos Heurísticos e Meta-Heurísticos
// https://github.com/CicolasCarvalho/MOA2024-CAIXEIRO-VIAJANTE

#include <stdio.h>
#include <string.h>
#include "time.h"

#include "./parser/parser.h"
#include "./algorithms/heuristics/nearest_neighbor/nearest_neighbor.h"
#include "./algorithms/heuristics/nearest_insertion/nearest_insertion.h"
#include "./algorithms/heuristics/pair_swap/pair_swap.h"
#include "./algorithms/heuristics/two_opt/two_opt.h"
#include "./algorithms/metaheuristics/genetic_algorithm/genetic_algorithm.h"
#include "./utils/utils.h"
#include "defs.h"

char *shift_arg(int *argc, char ***argv);

int main(int argc, char **argv) {
    omp_set_num_threads(8);
    unsigned int seed = time(NULL) + omp_get_thread_num() * 100;  // Garante uma semente única por thread
    srand(seed);

    #pragma omp parallel
    {
        unsigned int _seed = time(NULL) + omp_get_thread_num() * 100;  // Garante uma semente única por thread
        srand(_seed);
    }

    shift_arg(&argc, &argv);

    if (argc <= 2) {
        OUTPUT("Esperado um argumento contendo o arquivo de entrada\n./main [--nearest_neighbor|--nearest_insertion] [--pair_swap|--2opt|--none] <caminho>\n");
        return 0;
    }

    char *build_flag = shift_arg(&argc, &argv);
    char *opt_flag = shift_arg(&argc, &argv);
    char *file_path = shift_arg(&argc, &argv);

    Graph *graph = graph_from_tsp(file_path);
    Path *path = NULL;
    OUTPUT("\n---#----Graph Size: (%i)----#---\n", graph->vertices_num);

    if (strcmp(build_flag, "--nearest_neighbor") == 0) {
        double before = get_time();
        OUTPUT("\nNearest Neighbor:\n---#---------#---\n");

        path = build_nearest_neighbor(graph, 0);
        double distance = Graph_run_path(graph, path);

        if (!IS_BENCHMARK) {
            OUTPUT("Edges:\n\n");
            Path_print(path);
        }

        OUTPUT("Total Distance: %f\n", distance);
        OUTPUT("\nTempo decorrido: %lfs\n", get_time() - before);
    }

    if (strcmp(build_flag, "--nearest_insertion") == 0) {
        double before = get_time();
        OUTPUT("\nNearest Insertion:\n---#----------#---\n");

        path = build_nearest_insertion(graph, 0);
        double distance = Graph_run_path(graph, path);

        if (!IS_BENCHMARK) {
            OUTPUT("Edges:\n\n");
            Path_print(path);
        }

        OUTPUT("Total Distance: %f\n", distance);
        OUTPUT("\nTempo decorrido: %lfs\n", get_time() - before);
    }

    if (strcmp(build_flag, "--genetic_PMX") == 0) {
        double before = get_time();
        OUTPUT("\nGenetic Algorithm (PMX):\n---#----------#---\n");

        path = build_genetic_algorithm(graph, 0, PMX_CROSSOVER, GEN_SIZE, GEN_MAX_TIME);
        double distance = Graph_run_path(graph, path);

        if (!IS_BENCHMARK) {
            // OUTPUT("Edges:\n\n");
            // Path_print(path);
        }

        OUTPUT("Total Distance: %f\n", distance);
        OUTPUT("\nTempo decorrido: %lfs\n", get_time() - before);
    }

    if (strcmp(build_flag, "--genetic_OX1") == 0) {
        double before = get_time();
        OUTPUT("\nGenetic Algorithm (OX1):\n---#----------#---\n");

        path = build_genetic_algorithm(graph, 0, OX1_CROSSOVER, GEN_SIZE, GEN_MAX_TIME);
        double distance = Graph_run_path(graph, path);

        if (!IS_BENCHMARK) {
            // OUTPUT("Edges:\n\n");
            // Path_print(path);
        }

        OUTPUT("Total Distance: %f\n", distance);
        OUTPUT("\nTempo decorrido: %lfs\n", get_time() - before);
    }


    if (strcmp(opt_flag, "--pair_swap") == 0) {
        double before = get_time();
        OUTPUT("\nPair Swap:\n----------\n");

        apply_pair_swap(graph, path);
        double distance = Graph_run_path(graph, path);

        if (!IS_BENCHMARK) {
            OUTPUT("Edges:\n\n");
            Path_print(path);
        }

        OUTPUT("Total Distance: %f\n", distance);
        OUTPUT("\nTempo decorrido: %lfs\n", get_time() - before);
    }

    if (strcmp(opt_flag, "--2opt") == 0) {
        double before = get_time();
        OUTPUT("\n2opt:\n-----\n");

        apply_two_opt(graph, path);
        double distance = Graph_run_path(graph, path);

        if (!IS_BENCHMARK) {
            OUTPUT("Edges:\n\n");
            Path_print(path);
        }

        OUTPUT("Total Distance: %f\n", distance);
        OUTPUT("\nTempo decorrido: %lfs\n", get_time() - before);
    }

    if (path != NULL) Path_free(path);
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