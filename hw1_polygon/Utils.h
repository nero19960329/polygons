#ifndef UTILS_H
#define UTILS_H

#include <iostream>

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define round(x) floor((x) + 0.5)

extern int *HSVtoRGB(double h, double s, double v);
extern double intersect(int y, int maxX, std::pair<int, int> p1, std::pair<int, int> p2);

#endif