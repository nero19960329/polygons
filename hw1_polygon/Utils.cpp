#include "Utils.h"

#include <cmath>

using namespace std;

int *HSVtoRGB(double h, double s, double v) {
	h *= 360.0;

	int tmp = floor(h / 60);
	double f = h / 60 - tmp;
	double p = v * (1 - s);
	double q = v * (1 - f * s);
	double t = v * (1 - (1 - f) * s);

	double* tmpArray = new double[3];
	int* res = new int[4];

	if (tmp == 0) {
		tmpArray[0] = v;
		tmpArray[1] = t;
		tmpArray[2] = p;
	} else if (tmp == 1) {
		tmpArray[0] = q;
		tmpArray[1] = v;
		tmpArray[2] = p;
	} else if (tmp == 2) {
		tmpArray[0] = p;
		tmpArray[1] = v;
		tmpArray[2] = t;
	} else if (tmp == 3) {
		tmpArray[0] = p;
		tmpArray[1] = q;
		tmpArray[2] = v;
	} else if (tmp == 4) {
		tmpArray[0] = t;
		tmpArray[1] = p;
		tmpArray[2] = v;
	} else {
		tmpArray[0] = v;
		tmpArray[1] = p;
		tmpArray[2] = q;
	}

	res[0] = tmpArray[0] * 256;
	res[1] = tmpArray[1] * 256;
	res[2] = tmpArray[2] * 256;
	res[3] = 255;

	delete[] tmpArray;

	return res;
}

double intersect(int y, int maxX, pair<int, int> p1, pair<int, int> p2) {
	int x1, y1, x2, y2;
	x1 = p1.first;
	y1 = p1.second;
	x2 = p2.first;
	y2 = p2.second;

	if (y < min(y1, y2) || y > max(y1, y2)) {
		return -2;
	}

	if (x1 > x2) {
		x1 = p2.first;
		y1 = p2.second;
		x2 = p1.first;
		y2 = p1.second;
	}

	if (x1 == x2) {
		if (x1 >= 0 && x1 <= maxX) {
			return x1;
		} else {
			return -2;
		}
	} else if (y1 == y2) {
		if (y == y1) {
			return -1;
		} else {
			return -2;
		}
	}

	double k = (y2 - y1) * 1.0 / (x2 - x1);
	double x = (y - y1) / k + x1;

	if (x >= x1 && x <= x2) {
		return x;
	} else {
		return -2;
	}
}