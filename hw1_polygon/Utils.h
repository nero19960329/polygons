#ifndef UTILS_H
#define UTILS_H

#include <list>

#include "Polygon.h"

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define round(x) floor((x) + 0.5)

extern int *HSVtoRGB(double h, double s, double v);

extern double intersect(int y, int maxX, std::pair<int, int> p1, std::pair<int, int> p2);
extern std::pair<float, float> intersect(std::pair<int, int> p1, std::pair<int, int> p2, std::pair<int, int> p3, std::pair<int, int> p4, bool& ok);

// Shoelace formula
extern bool isClockwise(const std::list< std::pair<int, int> >& ids);

template <class T>
extern void appendList(std::list<T>& src, const std::list<T>& add) {
	src.insert(src.end(), add.begin(), add.end());
}

template <class T>
extern T **convertToPointer(T a[3][3]) {
	T **b = new T*[3];
	for (int i = 0; i < 3; ++i) {
		b[i] = new T[3];
		for (int j = 0; j < 3; ++j) {
			b[i][j] = a[i][j];
		}
	}

	return b;
}

extern float **matMul33(float **a, float **b);
extern float *matVecMul3(float a[3][3], float b[3]);
extern float dot(std::pair<float, float> a, std::pair<float, float> b);
extern float cross(std::pair<float, float> a, std::pair<float, float> b);
extern float norm(std::pair<float, float> a);

extern void getTranslationMatrix(float dx, float dy, float mat[3][3]);
extern void getScaleMatrix(float cx, float cy, float sx, float sy, float mat[3][3]);
extern void getRotateMatrix(float cx, float cy, float theta, float mat[3][3]);
extern void getHorizontalFlipMatrix(float cx, float mat[3][3]);
extern void getVerticalFlipMatrix(float cy, float mat[3][3]);

extern std::list< std::pair<int, int> > drawLineBresenham(std::pair<int, int> p1, std::pair<int, int> p2);

#endif