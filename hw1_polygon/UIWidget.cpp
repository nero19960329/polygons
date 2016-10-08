#include "UIWidget.h"
#include "Utils.h"

#include <QDebug>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QRgb>

#define GOLDEN_RATIO 0.618033988749895
#define DEFAULT_S 0.9
#define DEFAULT_V 0.7
#define PI 3.1415926

using namespace std;
using namespace canvas;

UIWidget::UIWidget(QWidget *parent, int *nowPolygon, int *hoverPolygon) : QWidget(parent) {
	setMouseTracking(true);
	setCursor(Qt::BlankCursor);
	setFixedSize(canvas::width, canvas::height);

	this->nowPolygon = nowPolygon;
	this->hoverPolygon = hoverPolygon;
	img = new QImage(canvas::width, canvas::height, QImage::Format_RGB32);
	drawStatus = STATUS_DONE;
	innerFlag = false;
	topZIndex = 0;
	midButtonDown = false;
	rightButtonDown = false;
	lastX = -1;
	lastY = -1;

	memset(zIndexMap, 0, sizeof(zIndexMap));
	setBackground(white);
	drawLines(grey);
}

UIWidget::~UIWidget() {

}

void UIWidget::setBackground(QRgb color) {
	for (int i = 0; i < wNum; ++i) {
		for (int j = 0; j < hNum; ++j) {
			pixelMap[i][j] = color;
		}
	}
}

void UIWidget::drawLines(QRgb color) {
	for (int i = 0; i < canvas::width; i += wInterval) {
		for (int j = 0; j < canvas::height; ++j) {
			img->setPixel(i, j, color);
		}
	}

	for (int j = 0; j < canvas::height; j += hInterval) {
		for (int i = 0; i < canvas::width; ++i) {
			img->setPixel(i, j, color);
		}
	}
}

bool UIWidget::highlightPolygon(const Polygon& polygon) {
	QRgb color = polygon.color;
	int r = min(255, qRed(color) * 1.0 / DEFAULT_V);
	int g = min(255, qGreen(color) * 1.0 / DEFAULT_V);
	int b = min(255, qBlue(color) * 1.0 / DEFAULT_V);

	for (auto id : polygon.boundary) {
		if (id.first >= 0 && id.first < wNum &&
			id.second >= 0 && id.second < hNum) {
				setPixelMap(polygon.zIndex, id.first, id.second, qRgb(r, g, b));
		}
	}

	return true;
}

void UIWidget::setPixelMap(int zIndex, int x, int y, const QRgb& color) {
	if (!zIndexMap[x][y]) {
		zIndexMap[x][y] = new list<int>();
	}

	list<int> *idxList = zIndexMap[x][y];
	if (!idxList->size() || zIndex > *(idxList->begin())) {
		pixelMap[x][y] = color;
		idxList->push_front(zIndex);
	} else if (zIndex == *(idxList->begin())) {
		pixelMap[x][y] = color;
	} else {
		auto idx = idxList->begin();
		for (; idx != idxList->end(); ++idx) {
			if (zIndex == *idx) {
				return;
			} else if (zIndex > *idx) {
				idxList->insert(idx, zIndex);
				return;
			}
		}

		idxList->insert(idx, zIndex);
	}
}

void UIWidget::removeFromPixelMap(int zIndex, int x, int y) {
	list<int> *idxList = zIndexMap[x][y];
	if (!idxList) {
		return;
	}

	if (zIndex > *(idxList->begin())) {
		return;
	} else if (zIndex == *(idxList->begin())) {
		idxList->erase(idxList->begin());

		if (idxList->size()) {
			pixelMap[x][y] = polygons[*(idxList->begin()) - 1]->color;
		} else {
			pixelMap[x][y] = white;
		}
	} else {
		for (auto idx = idxList->begin(); idx != idxList->end(); ++idx) {
			if (zIndex == *idx) {
				idxList->erase(idx);
				break;
			} else if (zIndex > *idx) {
				break;
			}
		}
	}

	if (!idxList->size()) {
		delete zIndexMap[x][y];
		zIndexMap[x][y] = NULL;
	}
}

void UIWidget::polygonTransition(Polygon& p, float transMat[3][3]) {
	removePolygonPoints(p);
	p.transition(transMat);
	setPolygonPoints(p);
	repaint();
}

void UIWidget::polygonHorizontalFlip(Polygon& p) {
	float transMat[3][3];
	Utils::getHorizontalFlipMatrix(p.center.first, transMat);
	polygonTransition(p, transMat);
	highlightPolygon(p);
	repaint();
}

void UIWidget::polygonVerticalFlip(Polygon& p) {
	float transMat[3][3];
	Utils::getVerticalFlipMatrix(p.center.second, transMat);
	polygonTransition(p, transMat);
	highlightPolygon(p);
	repaint();
}

void UIWidget::removePolygonPoints(Polygon& p) {
	for (auto vertex : p.boundary) {
		if (vertex.first >= 0 && vertex.first < wNum &&
			vertex.second >= 0 && vertex.second < hNum) {
				removeFromPixelMap(p.zIndex, vertex.first, vertex.second);
		}
	}

	for (auto vertex : p.interior) {
		if (vertex.first >= 0 && vertex.first < wNum &&
			vertex.second >= 0 && vertex.second < hNum) {
				removeFromPixelMap(p.zIndex, vertex.first, vertex.second);
		}
	}
}

void UIWidget::setPolygonPoints(Polygon& p) {
	for (auto vertex : p.boundary) {
		if (vertex.first >= 0 && vertex.first < wNum &&
			vertex.second >= 0 && vertex.second < hNum) {
				setPixelMap(p.zIndex, vertex.first, vertex.second, p.color);
		}
	}

	for (auto vertex : p.interior) {
		if (vertex.first >= 0 && vertex.first < wNum && 
			vertex.second >= 0 && vertex.second < hNum) {
				setPixelMap(p.zIndex, vertex.first, vertex.second, p.color);
		}
	}
}

void UIWidget::fillPolygon(int index) {
	polygons[index]->fill();
	for (auto vertex : polygons[index]->interior) {
		setPixelMap(polygons[index]->zIndex, vertex.first, vertex.second, polygons[index]->color);
	}
	highlightPolygon(index);
	repaint();
}

bool UIWidget::highlightPolygon(int index) {
	return highlightPolygon(*polygons[index]);
}

void UIWidget::recoverPolygon(int index) {
	for (auto id : polygons[index]->boundary) {
		setPixelMap(polygons[index]->zIndex, id.first, id.second, polygons[index]->color);
	}
}

void UIWidget::setCurrentPolygon(int index) {
	polygon = polygons[index];
}

void UIWidget::setDrawStatus(STATUS status) {
	drawStatus = status;
}

void UIWidget::setInnerFlag(bool flag) {
	innerFlag = flag;
}

void UIWidget::polygonTransition(int index, float transMat[3][3]) {
	polygonTransition(*polygons[index], transMat);
}

void UIWidget::polygonHorizontalFlip(int index) {
	polygonHorizontalFlip(*polygons[index]);
}

void UIWidget::polygonVerticalFlip(int index) {
	polygonVerticalFlip(*polygons[index]);
}

void UIWidget::polygonCut(int idx1, int idx2) {
	removePolygonPoints(*polygons[idx1]);
	removePolygonPoints(*polygons[idx2]);
	polygons[idx1]->cut(*polygons[idx2]);
	setPolygonPoints(*polygons[idx1]);
	highlightPolygon(idx1);
	repaint();
}

void UIWidget::paintEvent(QPaintEvent *) {
	for (int i = 0, pi = 1; i < wNum; ++i, pi += wInterval) {
		for (int j = 0, pj = 1; j < hNum; ++j, pj += hInterval) {
			for (int p = pi; p < pi + wInterval - 1; ++p) {
				for (int q = pj; q < pj + hInterval - 1; ++q) {
					img->setPixel(p, q, pixelMap[i][j]);
				}
			}
		}
	}

	if (hoverPoint.first != -1 && hoverPoint.second != -1) {
		QRgb black = qRgb(0, 0, 0);
		for (int i = hoverPoint.first * wInterval + 1; i < (hoverPoint.first + 1) * wInterval; ++i) {
			for (int j = hoverPoint.second * hInterval + 1; j < (hoverPoint.second + 1) * hInterval; ++j) {
				img->setPixel(i, j, black);
			}
		}
	}

	QPainter p(this);
	p.drawImage(QRect(0, 0, canvas::width, canvas::height), *img);
}

void UIWidget::mousePressEvent(QMouseEvent *event) {
	QPoint p = event->pos();

	if (event->button() == Qt::LeftButton) {
		if (drawStatus == STATUS_DONE) {
			drawStatus = STATUS_DRAWING;

			if (!innerFlag) {
				double nowColor = GOLDEN_RATIO * polygons.size();
				nowColor -= floor(nowColor);
				int *rgb = Utils::HSVtoRGB(nowColor, DEFAULT_S, DEFAULT_V);

				polygon = new Polygon();
				polygon->zIndex = ++topZIndex;
				polygon->color = qRgb(rgb[0], rgb[1], rgb[2]);
			}

			tmpPointList = list< pair<float, float> >();
			tmpPointList.push_back(make_pair(floor(p.x() / wInterval), floor(p.y() / hInterval)));
		} else if (drawStatus == STATUS_DRAWING) {
			float x, y;
			x = round(p.x() / wInterval);
			y = round(p.y() / hInterval);

			pair<float, float> lastPair = *(tmpPointList.rbegin());
			list< pair<int, int> > points = Utils::drawLineBresenham(lastPair, make_pair(x, y));
			for (auto point : points) {
				setPixelMap(polygon->zIndex, point.first, point.second, polygon->color);
			}
			Utils::appendList(polygon->boundary, points);
			tmpPointList.push_back(make_pair(x, y));

			repaint();
		}
	} else if (event->button() == Qt::RightButton) {
		if (drawStatus == STATUS_DRAWING) {
			drawStatus = STATUS_DONE;

			pair<float, float> firstPair = *(tmpPointList.begin());
			pair<float, float> lastPair = *(tmpPointList.rbegin());
			list< pair<int, int> > points = Utils::drawLineBresenham(lastPair, firstPair);
			for (auto point : points) {
				setPixelMap(polygon->zIndex, point.first, point.second, polygon->color);
			}
			Utils::appendList(polygon->boundary, points);
			tmpPointList.push_back(firstPair);

			if (Utils::isClockwise(tmpPointList) == !innerFlag) {
				tmpPointList.reverse();
			}

			tmpPointList.push_back(make_pair(FLT_MAX, FLT_MAX));
			Utils::appendList(polygon->v, tmpPointList);

			if (!innerFlag) {
				polygon->computeCenter();
				polygons.push_back(polygon);
				colorToIDMap[polygon->color] = polygons.size();

				int r = min(255, qRed(polygon->color) * 1.0 / DEFAULT_V);
				int g = min(255, qGreen(polygon->color) * 1.0 / DEFAULT_V);
				int b = min(255, qBlue(polygon->color) * 1.0 / DEFAULT_V);
				colorToIDMap[qRgb(r, g, b)] = polygons.size();

				if (*nowPolygon != -1 && polygons[*nowPolygon]->isVisible) {
					recoverPolygon(*nowPolygon);
				}
				highlightPolygon(*polygons[polygons.size() - 1]);
				emit(polygonInserted(polygons.size() - 1));
			} else {
				highlightPolygon(*polygon);
				emit(closeInnerMode());
			}

			repaint();
		} else {
			hoverPoint = make_pair(-1, -1);
			rightButtonDown = true;
			lastX = -1;
			lastY = -1;
		}
	} else if (event->button() == Qt::MiddleButton) {
		midButtonDown = true;
		lastX = -1;
		lastY = -1;
	}
}

void UIWidget::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::MiddleButton) {
		setCursor(Qt::BlankCursor);
		midButtonDown = false;
		lastX = -1;
		lastY = -1;
	} else if (event->button() == Qt::RightButton) {
		setCursor(Qt::BlankCursor);
		rightButtonDown = false;
		lastX = -1;
		lastY = -1;
	}
}

void UIWidget::mouseMoveEvent(QMouseEvent *event) {
	QPoint p = event->pos();

	int x, y;
	x = p.x() / wInterval;
	y = p.y() / hInterval;

	if (rightButtonDown) {
		if (*nowPolygon != -1) {
			setCursor(Qt::SizeAllCursor);
			if (lastX == -1 || lastY == -1) {
				lastX = x;
				lastY = y;
				return;
			}
			Polygon *polygon = polygons[*nowPolygon];
			polygon->computeCenter();
			pair<float, float> a = make_pair(x - polygon->center.first, y - polygon->center.second);
			pair<float, float> b = make_pair(lastX - polygon->center.first, lastY - polygon->center.second);
			lastX = x;
			lastY = y;
			float theta = acosf(Utils::dot(a, b) / (Utils::norm(a) * Utils::norm(b) + 1e-4));
			if (Utils::cross(a, b) > 0) {
				theta = -theta;
			}

			float transMat[3][3];
			Utils::getRotateMatrix(polygon->center.first, polygon->center.second, theta, transMat);
			polygonTransition(*polygons[*nowPolygon], transMat);
			highlightPolygon(*polygons[*nowPolygon]);
			repaint();
		}
	} else if (midButtonDown && *nowPolygon != -1) {
		setCursor(Qt::SizeAllCursor);
		hoverPoint = make_pair(-1, -1);
		if (lastX != -1 && lastY != -1) {
			float transMat[3][3];
			Utils::getTranslationMatrix(x - lastX, y - lastY, transMat);
			polygonTransition(*nowPolygon, transMat);
			highlightPolygon(*nowPolygon);
			repaint();
		}
		lastX = x;
		lastY = y;
	} else {
		if (x < 0 || x >= wNum || y < 0 || y >= hNum) {
			hoverPoint = make_pair(-1, -1);
		} else {
			emit(polygonSelected(colorToIDMap[pixelMap[x][y]] - 1));
			hoverPoint = make_pair(x, y);
		}
		repaint();
	}
}

void UIWidget::wheelEvent(QWheelEvent *event) {
	if (*nowPolygon != -1) {
		float tmp = event->delta() > 0 ? 0.1f: -0.1f;

		Polygon *polygon = polygons[*nowPolygon];
		polygon->computeCenter();
		float transMat[3][3];
		Utils::getScaleMatrix(polygon->center.first, polygon->center.second, 1.0 + tmp, 1.0 + tmp, transMat);
		polygonTransition(*polygons[*nowPolygon], transMat);
		highlightPolygon(*polygons[*nowPolygon]);
		repaint();
	}
}