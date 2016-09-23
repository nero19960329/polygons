#include "UIWidget.h"

#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QRgb>

#include <algorithm>

#include "Utils.h"

#define GOLDEN_RATIO 0.618033988749895
#define DEFAULT_S 0.9
#define DEFAULT_V 0.8

using namespace std;

UIWidget::UIWidget(QWidget *parent) : QWidget(parent) {
	QRgb white = qRgb(255, 255, 255);
	QRgb grey = qRgb(200, 200, 200);

	setMouseTracking(true);
	setCursor(Qt::BlankCursor);
	setFixedSize(width, height);

	img = new QImage(width, height, QImage::Format_RGB32);
	drawStatus = STATUS_DONE;

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
	for (int i = 0; i < width; i += wInterval) {
		for (int j = 0; j < height; ++j) {
			img->setPixel(i, j, color);
		}
	}

	for (int j = 0; j < height; j += hInterval) {
		for (int i = 0; i < width; ++i) {
			img->setPixel(i, j, color);
		}
	}
}

void UIWidget::drawLineBresenham(pair<int, int> p1, pair<int, int> p2, QRgb color) {
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
			pixelMap[x1][j] = color;
		}
		return;
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
			pixelMap[x][y] = color;
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
			pixelMap[x][y] = color;
			++y;
			e += k;
			if (e >= 0) {
				x += sign;
				e -= 1;
			}
		}
	}
}

void UIWidget::fillPolygon(Polygon& polygon) {
	for (int j = 0; j < hNum; ++j) {
		vector<int> intersections;
		list< pair<int, int> >::iterator p1, p2;
		p1 = polygon.v.begin();
		p2 = polygon.v.begin();
		++p2;
		for (; p2 != polygon.v.end(); ++p1, ++p2) {
			float x = intersect(j, wNum, *p1, *p2);

			if (x == -1) {
				intersections.push_back(p1->first);
				intersections.push_back(p2->first);
			} else if (x >= 0) {
				if (abs(x - p1->first) < 1e-6 && j == p1->second) {
					if (p2->second > p1->second) {
						intersections.push_back(p1->first);
					}
				} else if (abs(x - p2->first) < 1e-6 && j == p2->second) {
					if (p1->second > p2->second) {
						intersections.push_back(p2->first);
					}
				} else {
					intersections.push_back(round(x));
				}
			}
		}

		int len = intersections.size();
		if (!len || len % 2) {
			if (len % 2) {
				printf("j: %d, len: %d\n", j, len);
				printf("(%d, %d)\n", intersections[0], j);
				pixelMap[intersections[0]][j] = qRgb(0, 0, 0);
			}
			continue;
		}

		sort(intersections.begin(), intersections.end());

		for (int i = 0; i < len; i += 2) {
			for (int p = intersections[i]; p <= intersections[i + 1]; ++p) {
				pixelMap[p][j] = polygon.color;
			}
		}
	}
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

	QRgb black = qRgb(0, 0, 0);
	for (int i = hoverPoint.first * wInterval + 1; i < (hoverPoint.first + 1) * wInterval; ++i) {
		for (int j = hoverPoint.second * hInterval + 1; j < (hoverPoint.second + 1) * hInterval; ++j) {
			img->setPixel(i, j, black);
		}
	}

	QPainter p(this);
	p.drawImage(QRect(0, 0, width, height), *img);
}

void UIWidget::mousePressEvent(QMouseEvent *event) {
	QPoint p = event->pos();

	if (event->button() == Qt::LeftButton) {
		if (drawStatus == STATUS_DONE) {
			drawStatus = STATUS_DRAWING;

			double nowColor = GOLDEN_RATIO * polygons.size();
			nowColor -= floor(nowColor);
			int *rgb = HSVtoRGB(nowColor, DEFAULT_S, DEFAULT_V);

			polygon = Polygon();
			polygon.color = qRgb(rgb[0], rgb[1], rgb[2]);
			polygon.insertVertex(make_pair(p.x() / wInterval, p.y() / hInterval));
		} else {
			int x, y;
			x = p.x() / wInterval;
			y = p.y() / hInterval;

			pair<int, int> lastPair = *(polygon.v.rbegin());
			drawLineBresenham(lastPair, make_pair(x, y), polygon.color);
			polygon.insertVertex(make_pair(x, y));

			repaint();
		}
	} else if (event->button() == Qt::RightButton) {
		if (drawStatus == STATUS_DRAWING) {
			drawStatus = STATUS_DONE;

			pair<int, int> firstPair = *(polygon.v.begin());
			pair<int, int> lastPair = *(polygon.v.rbegin());
			drawLineBresenham(lastPair, firstPair, polygon.color);
			polygon.insertVertex(firstPair);
			polygons.push_back(polygon);

			fillPolygon(polygon);

			repaint();
		}
	}
}

void UIWidget::mouseMoveEvent(QMouseEvent *event) {
	QPoint p = event->pos();

	int x, y;
	x = p.x() / wInterval;
	y = p.y() / hInterval;

	if (x < 0 || x >= wNum || y < 0 || y >= hNum) {
		return;
	}

	hoverPoint = make_pair(x, y);

	repaint();
}