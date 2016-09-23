#ifndef POLYGON_H
#define POLYGON_H

#include <QRgb>

#include <list>

class Polygon {
public:
	std::list< std::pair<int, int> > v;
	QRgb color;

public:
	Polygon() {}
	void insertVertex(const std::pair<int, int>& p);
};

#endif