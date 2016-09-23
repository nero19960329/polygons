#ifndef UIWIDGET_H
#define UIWIDGET_H

#include <QWidget>

#include <vector>

#include "Polygon.h"

enum STATUS { STATUS_DRAWING, STATUS_DONE };

class UIWidget : public QWidget {
	Q_OBJECT

public:
	UIWidget(QWidget *parent);
	~UIWidget();

public:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);

private:
	void setBackground(QRgb);
	void drawLines(QRgb);
	void drawLineBresenham(std::pair<int, int>, std::pair<int, int>, QRgb);
	void fillPolygon(Polygon& polygon);

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
	QRgb lastColor;
	STATUS drawStatus;
	std::vector<Polygon> polygons;
	Polygon polygon;
};

#endif