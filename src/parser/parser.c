#include "parser.h"
#include "string.h"

//-definitions----------------------------------------------------------------------------------------------------------

typedef struct {
    int32_t dimension;
} HeaderInfo;

typedef void* LineReturn;

typedef struct {
    bool header_end;
    int32_t dimension;
} HeaderLine;

typedef struct {
    bool body_end;
    int32_t i;
    double x;
    double y;
} GraphLine;

//-declarations---------------------------------------------------------------------------------------------------------

static void shift_str_token(char **str, char dst[MAX_TOKEN_SIZE]);
static HeaderInfo parse_header(FILE *file);
static void parse_graph(FILE *file, Graph *graph);
static LineReturn interpret_line(char *line);

static char charhigh(char c);
static bool is_number(char c);

//-functions------------------------------------------------------------------------------------------------------------

Graph *graph_from_tsp(char *path) {
    FILE *graph_file = fopen(path, "r");
    if (!graph_file) {
        RAISE("Arquivo '%s' não encontrado", path);
    }

    HeaderInfo header = parse_header(graph_file);

    Graph *graph = Graph_new(header.dimension);

    parse_graph(graph_file, graph);

    return graph;
}

//-static---------------------------------------------------------------------------------------------------------------

static HeaderInfo parse_header(FILE *file) {
    char line_buffer[MAX_LINE_SIZE] = "\0";
    size_t len = 0;

    HeaderInfo header_info = (HeaderInfo) {
        .dimension = -1
    };

    char c;
    bool header_end = 0;
    while (!header_end) {
        c = charhigh((char)fgetc(file));

        if (c == '\n' || c == '\0') {
            PRINT("line: %s", line_buffer);
            HeaderLine *header_line = interpret_line(line_buffer);
            line_buffer[0] = '\0';
            len = 0;

            if (header_line != NULL) {
                if (header_line->header_end) {
                    header_end = true;
                } else if (header_line->dimension > 0) {
                    header_info.dimension = header_line->dimension;
                }

            }

            free(header_line);
        } else {
            line_buffer[len++] = c;
            line_buffer[len] = '\0';
        }
    }

    return header_info;
}

static void parse_graph(FILE *file, Graph *graph) {
    char line_buffer[MAX_LINE_SIZE] = "\0";
    size_t len = 0;

    char c;
    bool body_end = 0;
    while (!body_end) {
        c = charhigh((char)fgetc(file));

        if (c == '\n' || c == '\0') {
            PRINT("line: %s", line_buffer);
            GraphLine *graph_line = interpret_line(line_buffer);
            line_buffer[0] = '\0';
            len = 0;

            if (graph_line != NULL) {
                if (graph_line->body_end) {
                    body_end = true;
                } else if (graph_line->i > 0) {
                    PRINT("%i: {x = %f, y = %f}\n", graph_line->i - 1, graph_line->x, graph_line->y);
                    Graph_set(graph, graph_line->i - 1, (Coord){
                        .x = graph_line->x,
                        .y = graph_line->y
                    });
                }
            }

            free(graph_line);
        } else {
            line_buffer[len++] = c;
            line_buffer[len] = '\0';
        }
    }
}

static void shift_str_token(char **str, char dst[MAX_TOKEN_SIZE]) {
    dst[0] = '\0';
    size_t len = 0;

    // trim dos espaços iniciais
    while ((**str) == ' ') {
        (*str)++;
    }

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

static LineReturn interpret_line(char *line) {
    char token_buffer[MAX_TOKEN_SIZE];
    shift_str_token(&line, token_buffer);

    HeaderLine *header_line = malloc(sizeof(HeaderLine));
    header_line->header_end = false;
    header_line->dimension = -1;

    GraphLine *graph_line = malloc(sizeof(GraphLine));
    graph_line->body_end = false;
    graph_line->i = -1;

    if (strcmp(token_buffer, "NODE_COORD_SECTION") == 0) {
        header_line->header_end = true;
        free(graph_line);
        return header_line;
    }

    if (strcmp(token_buffer, "EOF") == 0) {
        graph_line->body_end = true;
        free(header_line);
        return graph_line;
    }

    if (strcmp(token_buffer, "DIMENSION") == 0) {
        shift_str_token(&line, token_buffer);

        if (*token_buffer != ':') {
            RAISE("excepted ':' after dimension");
        }

        shift_str_token(&line, token_buffer);

        header_line->dimension = atoi(token_buffer);
        free(graph_line);
        return header_line;
    }

    if (is_number(*token_buffer)) {
        int32_t i = atoi(token_buffer);

        shift_str_token(&line, token_buffer); // x
        double x = atof(token_buffer);

        shift_str_token(&line, token_buffer); // y
        double y = atof(token_buffer);

        graph_line->i = i;
        graph_line->x = x;
        graph_line->y = y;
        free(header_line);
        return graph_line;
    }

    free(header_line);
    free(graph_line);
    return NULL;
}

static char charhigh(char c) {
    return c >= 'a' && c <= 'z' ? c - 32 : c; // NOLINT(*-narrowing-conversions)
}

static bool is_number(char c) {
    return (c >= 48 && c <= 57);
}