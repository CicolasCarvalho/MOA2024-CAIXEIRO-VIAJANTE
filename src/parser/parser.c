#include "parser.h"

//-definitions----------------------------------------------------------------------------------------------------------

typedef struct {
    int32_t dimension;
} HeaderInfo;

//-declarations---------------------------------------------------------------------------------------------------------

static void shift_str_token(char **str, char dst[MAX_TOKEN_SIZE]);
static HeaderInfo parse_header(FILE *file);
static void parse_graph(FILE *file, Graph *graph, HeaderInfo header);

static char charhigh(char c);
static int8_t get_line(FILE *file, char dst[MAX_LINE_SIZE]);

//-functions------------------------------------------------------------------------------------------------------------

Graph *graph_from_tsp(char *path) {
    FILE *graph_file = fopen(path, "r");
    if (!graph_file) {
        RAISE("Arquivo '%s' não encontrado", path);
    }

    HeaderInfo header = parse_header(graph_file);

    Graph *graph = Graph_new(header.dimension);

    parse_graph(graph_file, graph, header);

    return graph;
}

//-static---------------------------------------------------------------------------------------------------------------

static HeaderInfo parse_header(FILE *file) {
    char *line_buffer = malloc(MAX_LINE_SIZE);
    line_buffer[0] = '\0';
    char token_buffer[MAX_TOKEN_SIZE] = "\0";

    HeaderInfo header_info = (HeaderInfo) {
        .dimension = -1
    };

    bool header_end = 0;
    while (!header_end && get_line(file, line_buffer) != 0) {
        do {
            shift_str_token(&line_buffer, token_buffer);

            if (
                strcmp(token_buffer, "NODE_COORD_SECTION") == 0 ||
                strcmp(token_buffer, "EOF") == 0
            ) {
                header_end = 1;
            }

            if (strcmp(token_buffer, "DIMENSION") == 0) {
                shift_str_token(&line_buffer, token_buffer);

                if (*token_buffer != ':') {
                    RAISE("excepted ':' after dimension");
                }

                shift_str_token(&line_buffer, token_buffer);

                header_info.dimension = atoi(token_buffer);
            }
        } while (*line_buffer != '\0');
    }

    OUTPUT("Graph Dimension: %i\n", header_info.dimension);

    return header_info;
}

static void parse_graph(FILE *file, Graph *graph, HeaderInfo header) {
    char *line_buffer = malloc(MAX_LINE_SIZE);
    line_buffer[0] = '\0';
    char token_buffer[MAX_TOKEN_SIZE] = "\0";

    int32_t i = 0;
    while (get_line(file, line_buffer) != 0 && i < header.dimension) {
        shift_str_token(&line_buffer, token_buffer); // index
        i = atoi(token_buffer);

        shift_str_token(&line_buffer, token_buffer); // x
        int32_t x = atoi(token_buffer);

        shift_str_token(&line_buffer, token_buffer); // y
        int32_t y = atoi(token_buffer);

        OUTPUT("%i: {x = %i, y = %i}\n", i - 1, x, y);
        Graph_set(graph, i - 1, (Coord){
                .x = x,
                .y = y
        });
    }
}

static void shift_str_token(char **str, char dst[MAX_TOKEN_SIZE]) {
    dst[0] = '\0';
    uint8_t len = 0;

    // trim dos espaços iniciais
    while ((**str) == ' ') (*str)++;

    while ((**str) != '\0' && (**str) != ' ') {
        if (len > MAX_TOKEN_SIZE) {
            RAISE("buffer size overflow");
        }

        char c = charhigh(**str);

        if (c == ':' && len > 0) {
            break;
        }

        dst[len++] = c;
        dst[len] = '\0';
        (*str)++;

        if (c == ':' && len == 1) {
            break;
        }
    }

    PRINT("token: '%s'", dst);
}

static char charhigh(char c) {
    return c >= 'a' && c <= 'z' ? c - 32 : c; // NOLINT(*-narrowing-conversions)
}

static int8_t get_line(FILE *file, char dst[MAX_LINE_SIZE]) {
    dst[0] = '\0';
    uint8_t len = 0;

    char c;

    while ((c = fgetc(file)) != '\0' && c != '\n' && c != EOF) {
        dst[len++] = c;
        dst[len] = '\0';
    }

    return len;
}