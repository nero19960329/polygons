#ifndef POLYGON_H
#define POLYGON_H

#include <QRgb>

#include <list>

#include "Utils.h"

#define MIN_INT 0x80000000

class Polygon {
public:
	std::list< std::pair<float, float> > v;
	std::pair<float, float> center;
	QRgb color;
	std::list< std::pair<int, int> > boundary;
	std::list< std::pair<int, int> > interior;
	int zIndex;
	bool isVisible;

private:
	void connectVertices();

public:
	Polygon(): isVisible(true) {}
	void insertVertex(const std::pair<int, int> &);
	void addInteriorPoint(const std::pair<int, int> &);
	void transition(float[3][3]);
	void fill();
	void computeCenter();
	void cut(Polygon& p);
};

#endif