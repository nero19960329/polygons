#ifndef UIWIDGET_H
#define UIWIDGET_H

#include <QWidget>
#include <QWheelEvent>

#include <list>
#include <vector>
#include <unordered_map>

#include "Polygon.h"

namespace canvas {
	const static int width = 801;
	const static int height = 601;
	const static int wNum = 160;
	const static int hNum = 120;
	const static int wInterval = width / wNum;
	const static int hInterval = height / hNum;

	const static QRgb white = qRgb(255, 255, 255);
	const static QRgb grey = qRgb(200, 200, 200);
	const static QRgb black = qRgb(0, 0, 0);

	enum STATUS { STATUS_DRAWING, STATUS_DONE };

	class UIWidget : public QWidget {
		Q_OBJECT

	public:
		UIWidget(QWidget *parent, int *nowPolygon, int *hoverPolygon);
		~UIWidget();

	public:
		void paintEvent(QPaintEvent *);
		void mousePressEvent(QMouseEvent *);
		void mouseReleaseEvent(QMouseEvent *);
		void mouseMoveEvent(QMouseEvent *);
		void wheelEvent(QWheelEvent *);

	signals:
		void polygonInserted(int);
		void polygonSelected(int);
		void closeInnerMode();

	private:
		void setBackground(QRgb);
		void drawLines(QRgb);
		bool highlightPolygon(const Polygon &);
		void setPixelMap(int, int , int , const QRgb &);
		void removeFromPixelMap(int, int, int);
		void polygonTransition(Polygon &, float[3][3]);
		void polygonHorizontalFlip(Polygon &);
		void polygonVerticalFlip(Polygon &);
		void removePolygonPoints(Polygon &);
		void setPolygonPoints(Polygon &);

	public:
		void fillPolygon(int);
		bool highlightPolygon(int);
		void recoverPolygon(int);
		void setCurrentPolygon(int);
		void setDrawStatus(STATUS);
		void setInnerFlag(bool);
		void polygonTransition(int, float[3][3]);
		void polygonHorizontalFlip(int);
		void polygonVerticalFlip(int);
		void polygonCut(int, int);

	private:
		int *nowPolygon, *hoverPolygon;

		int selectedPolygon;
		int lastX, lastY;
		bool midButtonDown, rightButtonDown;

		QRgb pixelMap[wNum][hNum];
		std::list<int> *zIndexMap[wNum][hNum];
		std::unordered_map<QRgb, int> colorToIDMap;
		QImage *img;

		std::pair<int, int> hoverPoint;
		STATUS drawStatus;
		std::vector<Polygon*> polygons;
		Polygon *polygon;
		int topZIndex;

		bool innerFlag;
		std::list< std::pair<float, float> > tmpPointList;
	};
};

#endif