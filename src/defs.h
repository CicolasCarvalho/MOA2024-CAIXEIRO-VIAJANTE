#ifndef MOA2024_CAIXEIRO_VIAJANTE_DEFS_H
#define MOA2024_CAIXEIRO_VIAJANTE_DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//-debug-info-----------------------------------------------------------------------------------------------------------

#define IS_BENCHMARK 1
#define DEBUG_INFO !IS_BENCHMARK && 1

//-log-utilities--------------------------------------------------------------------------------------------------------

#if DEBUG_INFO
    #define PRINT_FILE_AND_LINE() printf("\033[0;90m%s:%i:\e[0m ", __FILE__, __LINE__)

    #define START_LOG(name) \
                PRINT_FILE_AND_LINE(); \
                printf("%s\n", name)
    #define LOG(...)                                \
                printf(__VA_ARGS__)
    #define END_LOG(name) \
                printf("\n")
                // PRINT_FILE_AND_LINE();
                // printf("'%s'\n", name)

    #define PRINT(...)                             \
                PRINT_FILE_AND_LINE(); \
                printf(__VA_ARGS__);                   \
                printf("\n")
    #define WARN(...)           \
                printf("\033[0;33mWARN:\033[0m ");    \
                PRINT(__VA_ARGS__)
    #define RAISE(...)          \
                printf("\033[0;31mERROR:\033[0m ");    \
                PRINT(__VA_ARGS__);  \
                exit(1)

    #define OUTPUT(...) \
                printf("\033[0;32m>>\033[0m ");        \
                printf(__VA_ARGS__)
#else
    #define PRINT_FILE_AND_LINE()

    #define START_LOG(name)
    #define LOG(...)
    #define END_LOG(name)

    #define PRINT(...)
    #define WARN(...)                               \
                printf("\033[0;33mWARN:\033[0m ");    \
                PRINT_FILE_AND_LINE();              \
                printf(__VA_ARGS__);                   \
                printf("\n")
    #define RAISE(...)          \
                printf("\033[0;31mERROR:\033[0m ");    \
                PRINT_FILE_AND_LINE();              \
                printf(__VA_ARGS__);                   \
                printf("\n"); \
                exit(1)

    #define OUTPUT(...) \
                printf(__VA_ARGS__)
#endif //DEBUG_INFO

//-bool-defs------------------------------------------------------------------------------------------------------------

#define bool uint8_t
#define true 1
#define false 0

//-program-config-------------------------------------------------------------------------------------------------------

#define MAX_LINE_SIZE 128
#define MAX_TOKEN_SIZE 32

#define NEAREST_NEIGHBOR_RANDOM_SIZE 10

// PMX
// #define GEN_LOG_INTERVAL 0
// #define GEN_SIZE 500
// #define GEN_MAX_STABLE 1000000
// #define GEN_MAX_TIME 18000
// #define GEN_ELITE_COUNT 50
// #define GEN_MUT_RATE 0.1
// #define GEN_MAX_RANK_SELECTION 5
// #define GEN_MAX 1000

#define GEN_LOG_INTERVAL 0
#define GEN_SIZE 100
#define GEN_MAX_STABLE 1000000
#define GEN_MAX_TIME 18000
#define GEN_ELITE_COUNT 10
#define GEN_MUT_RATE 0.25
#define GEN_MAX_RANK_SELECTION 2
#define GEN_MAX 2000
#define GEN_MULTI_CROSSOVER 1

#endif //MOA2024_CAIXEIRO_VIAJANTE_DEFS_H
