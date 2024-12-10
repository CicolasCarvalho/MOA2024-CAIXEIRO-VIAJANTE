#ifndef MOA2024_CAIXEIRO_VIAJANTE_COORD_H
#define MOA2024_CAIXEIRO_VIAJANTE_COORD_H

#include <stdint.h>
#include <math.h>

typedef struct {
    int32_t x;
    int32_t y;
} Coord;

double Coord_distance(Coord from, Coord to);

#endif //MOA2024_CAIXEIRO_VIAJANTE_COORD_H
