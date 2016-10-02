#include "Utils.h"
#include "UIWidget.h"

#include <cmath>

#include <algorithm>
#include <vector>

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

pair<float, float> intersect(pair<int, int> p1, pair<int, int> p2, pair<int, int> p3, pair<int, int> p4, bool& ok) {
	int x1, x2, x3, x4, y1, y2, y3, y4;
	x1 = p1.first;
	x2 = p2.first;
	x3 = p3.first;
	x4 = p4.first;
	y1 = p1.second;
	y2 = p2.second;
	y3 = p3.second;
	y4 = p4.second;

	int tmp;
	tmp = (x4 - x3) * (y2 - y1) - (x2 - x1) * (y4 - y3);
	if (!tmp) {
		ok = false;
		return make_pair(0.0, 0.0);
	}

	float resX = ((x4 - x3) * (x1 * y2 - x2 * y1) - (x2 - x1) * (x3 * y4 - x4 * y3)) * 1.0 / tmp;
	float resY = ((y4 - y3) * (x1 * y2 - x2 * y1) - (y2 - y1) * (x3 * y4 - x4 * y3)) * 1.0 / tmp;

	if (resX >= min(x1, x2) && resX <= max(x1, x2) && resX >= min(x3, x4) && resX <= max(x3, x4) &&
		resY >= min(y1, y2) && resY <= max(y1, y2) && resY >= min(y3, y4) && resY <= max(y3, y4)) {
		ok = true;
		return make_pair(resX, resY);
	} else {
		ok = false;
		return make_pair(0.0, 0.0);
	}
}

bool isClockwise(const list< pair<int, int> >& ids) {
	int r = 0;

	list< pair<int, int> >::const_iterator it1 = ids.cbegin();
	list< pair<int, int> >::const_iterator it2 = ids.cbegin();
	++it2;
	for (; it2 != ids.cend(); ++it1, ++it2) {
		r += (it1->first - it2->first) * (it1->second + it2->second);
	}

	if (r < 0) {
		return false;
	} else {
		return true;
	}
}

float **matMul33(float **a, float **b) {
	float **c = new float*[3];
	for (int i = 0; i < 3; ++i) {
		c[i] = new float[3];
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			c[i][j] = 0.0;
			for (int k = 0; k < 3; ++k) {
				c[i][j] += a[i][k] * b[k][j]; 
			}
		}
	}

	return c;
}

float *matVecMul3(float a[3][3], float b[3]) {
	float *c = new float[3];

	for (int i = 0; i < 3; ++i) {
		c[i] = 0.0;
		for (int j = 0; j < 3; ++j) {
			c[i] += a[i][j] * b[j];
		}
	}

	return c;
}

float dot(pair<float, float> a, pair<float, float> b) {
	return a.first * b.first + a.second * b.second;
}

float cross(std::pair<float, float> a, std::pair<float, float> b) {
	return a.first * b.second - a.second* b.first;
}

float norm(pair<float, float> a) {
	return sqrt(a.first * a.first + a.second * a.second);
}

void getTranslationMatrix(float dx, float dy, float mat[3][3]) {
	float res[3][3] = {
		1.0, 0.0, dx,
		0.0, 1.0, dy,
		0.0, 0.0, 1.0
	};
	memcpy(mat, res, sizeof(res));
}

void getScaleMatrix(float cx, float cy, float sx, float sy, float mat[3][3]) {
	float moveMat1[3][3], moveMat2[3][3];
	getTranslationMatrix(cx, cy, moveMat1);
	getTranslationMatrix(-cx, -cy, moveMat2);

	float scaleMat[3][3] = {
		sx, 0.0, 0.0,
		0.0, sy, 0.0,
		0.0, 0.0, 1.0
	};

	float **res = matMul33(convertToPointer(moveMat1), convertToPointer(scaleMat));
	res = matMul33(res, convertToPointer(moveMat2));

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			mat[i][j] = res[i][j];
		}
	}
}

void getRotateMatrix(float cx, float cy, float theta, float mat[3][3]) {
	float moveMat1[3][3], moveMat2[3][3];
	getTranslationMatrix(cx, cy, moveMat1);
	getTranslationMatrix(-cx, -cy, moveMat2);

	float rotateMat[3][3] = {
		cos(theta), -sin(theta), 0.0,
		sin(theta), cos(theta), 0.0,
		0.0, 0.0, 1.0
	};

	float **res = matMul33(convertToPointer(moveMat1), convertToPointer(rotateMat));
	res = matMul33(res, convertToPointer(moveMat2));

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			mat[i][j] = res[i][j];
		}
	}
}

void getHorizontalFlipMatrix(float cx, float mat[3][3]) {
	float moveMat1[3][3], moveMat2[3][3];
	getTranslationMatrix(cx, 0, moveMat1);
	getTranslationMatrix(-cx, 0, moveMat2);

	float horizontalFlipMat[3][3] = {
		-1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	};

	float **res = matMul33(convertToPointer(moveMat1), convertToPointer(horizontalFlipMat));
	res = matMul33(res, convertToPointer(moveMat2));

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			mat[i][j] = res[i][j];
		}
	}
}

void getVerticalFlipMatrix(float cy, float mat[3][3]) {

	float moveMat1[3][3], moveMat2[3][3];
	getTranslationMatrix(0, cy, moveMat1);
	getTranslationMatrix(0, -cy, moveMat2);

	float verticalFlipMat[3][3] = {
		1.0, 0.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, 0.0, 1.0
	};

	float **res = matMul33(convertToPointer(moveMat1), convertToPointer(verticalFlipMat));
	res = matMul33(res, convertToPointer(moveMat2));

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			mat[i][j] = res[i][j];
		}
	}
}

list< pair<int, int> > drawLineBresenham(pair<int, int> p1, pair<int, int> p2) {
	list< pair<int, int> > ids;

	int x, y, dx, dy, sign;
	float k, e;
	int x1, y1, x2, y2;
	x1 = p1.first;
	y1 = p1.second;
	x2 = p2.first;
	y2 = p2.second;
	dx = x2 - x1;
	dy = y2 - y1;

	if (!dx) {
		if (y1 > y2) {
			swap(y1, y2);
		}

		for (int j = y1; j <= y2; ++j) {
			ids.push_back(make_pair(x1, j));
		}
		return ids;
	}

	k = dy * 1.0 / dx;
	e = -0.5;
	sign = k > 0 ? 1 : -1;
	k = abs(k);

	if (k >= -1 && k <= 1) {
		if (x1 > x2) {
			swap(x1, x2);
			swap(y1, y2);
			dx = -dx;
		}
		x = x1;
		y = y1;

		for (int i = 0; i <= dx; ++i) {
			ids.push_back(make_pair(x, y));
			++x;
			e += k;
			if (e >= 0) {
				y += sign;
				e -= 1;
			}
		}
	} else {
		if (y1 > y2) {
			swap(x1, x2);
			swap(y1, y2);
			dy = -dy;
		}
		x = x1;
		y = y1;

		k = 1.0 / k;
		for (int j = 0; j <= dy; ++j) {
			ids.push_back(make_pair(x, y));
			++y;
			e += k;
			if (e >= 0) {
				x += sign;
				e -= 1;
			}
		}
	}

	return ids;
}