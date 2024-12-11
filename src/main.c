#include <stdio.h>
#include <string.h>

#include "./parser/parser.h"
#include "./algorithms/nearest/nearest.h"

char *shift_arg(int *argc, char ***argv);

int main(int argc, char **argv) {
    shift_arg(&argc, &argv);

    if (argc == 0) {
        LOG("Esperado um argumento contendo o arquivo de entrada\n./main [--nearest|] <caminho>\n");
        return 0;
    }

    char *flag;
    if (argc > 1) {
        flag = shift_arg(&argc, &argv);
    }

    char *path = shift_arg(&argc, &argv);
    Graph *graph = graph_from_tsp(path);

    if (strcmp(flag, "--nearest") == 0) {
        START_LOG("nearest insertion");
        build_nearest_neighbor(graph, 0);
        END_LOG("nearest insertion");
    }

    // free(flag);

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