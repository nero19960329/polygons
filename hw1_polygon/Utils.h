#ifndef UTILS_H
#define UTILS_H

#include <list>

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define round(x) floor((x) + 0.5)

extern int *HSVtoRGB(double h, double s, double v);

extern double intersect(int y, int maxX, std::pair<int, int> p1, std::pair<int, int> p2);

// Shoelace formula
extern bool isClockwise(const std::list< std::pair<int, int> >& ids);

template <class T>
extern void appendList(std::list<T>& src, const std::list<T>& add) {
	src.insert(src.end(), add.begin(), add.end());
}

#endif