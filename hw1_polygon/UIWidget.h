#ifndef UIWIDGET_H
#define UIWIDGET_H

#include <QWidget>

#include <list>
#include <vector>

#include "Polygon.h"

enum STATUS { STATUS_DRAWING, STATUS_DONE, STATUS_INNER, STATUS_INNERDRAWING };

class UIWidget : public QWidget {
	Q_OBJECT

public:
	UIWidget(QWidget *parent);
	~UIWidget();

public:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);

signals:
	void polygonInserted(int);

private:
	void setBackground(QRgb);
	void drawLines(QRgb);
	std::list< std::pair<int, int> > drawLineBresenham(std::pair<int, int>, std::pair<int, int>, QRgb);
	void fillPolygon(const Polygon& polygon);
	void highlightPolygon(const Polygon& polygon);

public:
	void fillPolygon(int index);
	void highlightPolygon(int index);
	void recoverPolygon(int index);
	void setCurrentPolygon(int);
	void setDrawStatus(STATUS);

private:
	const static int width = 801;
	const static int height = 601;
	const static int wNum = 160;
	const static int hNum = 120;
	const static int wInterval = width / wNum;
	const static int hInterval = height / hNum;

	QRgb pixelMap[wNum][hNum];
	QImage *img;

	std::pair<int, int> hoverPoint;
	STATUS drawStatus;
	std::vector<Polygon*> polygons;
	Polygon *polygon;

	std::list< std::pair<int, int> > tmpPointList;
};

#endif