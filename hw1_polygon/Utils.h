#ifndef UTILS_H
#define UTILS_H

#include <list>

#include "Polygon.h"

#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define round(x) floor((x) + 0.5)

class Utils {
public:
	static int *HSVtoRGB(double h, double s, double v);

	static double intersect(int y, int maxX, std::pair<float, float> p1, std::pair<float, float> p2);
	static std::pair<float, float> intersect(std::pair<float, float> p1, std::pair<float, float> p2, std::pair<float, float> p3, std::pair<float, float> p4, bool& ok);

	// Shoelace formula
	static bool isClockwise(const std::list< std::pair<float, float> >& ids);

	template <class T>
	static void appendList(std::list<T>& src, const std::list<T>& add) {
		src.insert(src.end(), add.begin(), add.end());
	}

	template <class T>
	static T **convertToPointer(T a[3][3]) {
		T **b = new T*[3];
		for (int i = 0; i < 3; ++i) {
			b[i] = new T[3];
			for (int j = 0; j < 3; ++j) {
				b[i][j] = a[i][j];
			}
		}

		return b;
	}

	static float **matMul33(float **a, float **b);
	static float *matVecMul3(float a[3][3], float b[3]);
	static float dot(std::pair<float, float> a, std::pair<float, float> b);
	static float cross(std::pair<float, float> a, std::pair<float, float> b);
	static float norm(std::pair<float, float> a);

	static void getTranslationMatrix(float dx, float dy, float mat[3][3]);
	static void getScaleMatrix(float cx, float cy, float sx, float sy, float mat[3][3]);
	static void getRotateMatrix(float cx, float cy, float theta, float mat[3][3]);
	static void getHorizontalFlipMatrix(float cx, float mat[3][3]);
	static void getVerticalFlipMatrix(float cy, float mat[3][3]);

	static std::list< std::pair<int, int> > drawLineBresenham(std::pair<float, float> p1, std::pair<float, float> p2);
};

#endif