#include "coord.h"

double Coord_distance(Coord from, Coord to) {
    double x_delta = to.x - from.x;
    double y_delta = to.y - from.y;
    double distance = sqrt((x_delta * x_delta) + (y_delta * y_delta));

    return distance;
}