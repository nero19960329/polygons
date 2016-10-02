#include "Polygon.h"
#include "UIWidget.h"

#include <QDebug>

using namespace std;

void Polygon::connectVertices() {
	boundary.clear();
	list< pair<int, int> >::iterator p1, p2;
	p1 = v.begin();
	p2 = v.begin();
	++p2;
	for (; p2 != v.end(); ++p1, ++p2) {
		if (p1->first == MIN_INT || p2->first == MIN_INT) {
			continue;
		}

		list< pair<int, int> > points = drawLineBresenham(*p1, *p2);
		for (auto point : points) {
			if (point.first >= 0 && point.first < canvas::wNum &&
				point.second >= 0 && point.second < canvas::hNum) {
					boundary.push_back(point);
			}
		}
	}
}

void Polygon::insertVertex(const pair<int, int>& p) {
	v.push_back(p);
}

void Polygon::addInteriorPoint(const pair<int, int>& p) {
	interior.push_back(p);
}

void Polygon::transition(float transMat[3][3]) {
	float tmp[3], *newPoint;
	for (auto vertex = v.begin(); vertex != v.end(); ++vertex) {
		if (vertex->first == MIN_INT) {
			continue;
		}

		tmp[0] = vertex->first;
		tmp[1] = vertex->second;
		tmp[2] = 1.0f;
		newPoint = matVecMul3(transMat, tmp);
		vertex->first = round(newPoint[0] / newPoint[2]);
		vertex->second = round(newPoint[1] / newPoint[2]);
	}

	connectVertices();

	if (interior.size()) {
		interior = list< pair<int, int> >();
		fill();
	}
}

void Polygon::fill() {
	for (int j = 0; j < canvas::hNum; ++j) {
		vector<int> intersections;
		list< pair<int, int> >::iterator p1, p2;
		p1 = v.begin();
		p2 = v.begin();
		++p2;
		for (; p2 != v.end(); ++p1, ++p2) {
			if (p1->first == MIN_INT || p2->first == MIN_INT) {
				continue;
			}

			float x = intersect(j, canvas::wNum, *p1, *p2);

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
			continue;
		}

		sort(intersections.begin(), intersections.end());

		for (int i = 0; i < len; i += 2) {
			for (int p = intersections[i]; p <= intersections[i + 1]; ++p) {
				addInteriorPoint(make_pair(p, j));
			}
		}
	}
}

void Polygon::computeCenter() {
	int cnt = 0;
	for (auto vertex : v) {
		++cnt;
		if (vertex.first == MIN_INT && vertex.second == MIN_INT) {
			break;
		}
	}
	cnt -= 2;

	center = make_pair(0.0, 0.0);
	int i = 0;
	for (auto vertex = v.begin(); i < cnt; ++i, ++vertex) {
		center.first += vertex->first;
		center.second += vertex->second;
	}
	center.first /= cnt;
	center.second /= cnt;
}

void Polygon::cut(Polygon &polygon) {
	typedef struct pointNode {
		pair<int, int> coor;
		bool isIntersection;
		bool isIn;
		bool isTraversed;
		list<pointNode*>::iterator ptr;

		pointNode() {}
		pointNode(pair<int, int> p, bool interFlag) : coor(p), isIntersection(interFlag), isTraversed(false) {}
	} pointNode;

	bool ok;

	list<pointNode*> p, q;
	for (auto point : v) {
		pointNode *tmp = new pointNode(point, false);
		p.push_back(tmp);
	}
	for (auto point : polygon.v) {
		pointNode *tmp = new pointNode(point, false);
		q.push_back(tmp);
	}

	list<pointNode*>::iterator it1, it2;
	it1 = p.begin();
	it2 = p.begin();
	while (it1 != p.end()) {
		if ((*it2)->coor.first == MIN_INT) {
			++it2;
			it1 = it2;
			continue;
		}
		auto tmpIt = it2;
		++it2;
		if ((*it2)->coor.first == MIN_INT) {
			(*tmpIt)->ptr = it1;
		}
	}

	it1 = q.begin();
	it2 = q.begin();
	while (it1 != q.end()) {
		if ((*it2)->coor.first == MIN_INT) {
			++it2;
			it1 = it2;
			continue;
		}
		auto tmpIt = it2;
		++it2;
		if ((*it2)->coor.first == MIN_INT) {
			(*tmpIt)->ptr = it1;
		}
	}

	list<pointNode*>::iterator p1, p2;
	p1 = p.begin();
	p2 = p.begin();
	++p2;
	for (; p2 != p.end(); ) {
		if ((*p1)->coor.first == MIN_INT || (*p2)->coor.first == MIN_INT) {
			p1 = p2;
			++p2;

			continue;
		}

		list<pointNode*>::iterator q1, q2;
		q1 = q.begin();
		q2 = q.begin();
		++q2;
		for (; q2 != q.end(); ) {
			if ((*q1)->coor.first == MIN_INT || (*q2)->coor.first == MIN_INT) {
				q1 = q2;
				++q2;
				continue;
			}

			pair<float, float> interPoint = intersect((*p1)->coor, (*p2)->coor, (*q1)->coor, (*q2)->coor, ok);
			if (ok) {
				list<pointNode*>::iterator tmp;
				pointNode *tmp1 = new pointNode(make_pair(round(interPoint.first), round(interPoint.second)), true);
				bool inFlag;
				pair<int, int> p1p2, q1q2;
				p1p2 = make_pair((*p2)->coor.first - (*p1)->coor.first, (*p2)->coor.second - (*p1)->coor.second);
				q1q2 = make_pair((*q2)->coor.first - (*q1)->coor.first, (*q2)->coor.second - (*q1)->coor.second);
				if (cross(p1p2, q1q2) > 0) {
					inFlag = true;
				} else {
					inFlag = false;
				}
				tmp1->isIn = inFlag;
				tmp = p1;
				auto lastIt = p1;
				++p1;
				while (lastIt != p2) {
					int x1, x2, x3, y1, y2, y3;
					x1 = (*lastIt)->coor.first;
					x2 = tmp1->coor.first;
					x3 = (*p1)->coor.first;
					y1 = (*lastIt)->coor.second;
					y2 = tmp1->coor.second;
					y3 = (*p1)->coor.second;
					if ((x1 - x2) * (x2 - x3) >= 0 && (y1 - y2) * (y2 - y3) >= 0) {
						p.insert(p1, tmp1);
						break;
					}
					++lastIt;
					++p1;
				}
				pointNode *tmp2 = new pointNode(make_pair(round(interPoint.first), round(interPoint.second)), true);
				tmp2->isIn = inFlag;
				q.insert(++q1, tmp2);
				tmp1->ptr = --q1;
				tmp2->ptr = ++lastIt;
				p1 = tmp;
			}

			q1 = q2;
			++q2;
		}

		p1 = p2;
		++p2;
	}

	list<pointNode*> resList;
	list<pointNode*>::iterator it;
	bool checkFlag = true;
	bool isP = true;
	while (true) {
		if (checkFlag) {
			bool flag = true;
			for (auto node = p.begin(); node != p.end(); ++node) {
				if ((*node)->isIntersection && !((*node)->isTraversed)) {
					flag = false;
					it = node;
					checkFlag = false;
					break;
				}
			}

			if (flag) {
				break;
			}
		} else {
			while (true) {
				if ((*it)->isTraversed) {
					resList.push_back(*it);
					pointNode *tmpNode = new pointNode(make_pair(MIN_INT, MIN_INT), false);
					resList.push_back(tmpNode);
					checkFlag = true;
					break;
				}
				(*it)->isTraversed = true;
				resList.push_back(*it);
				auto tmp = it;
				++it;
				if ((*it)->isIn == !isP) {
					(*it)->isTraversed = true;
					it = (*it)->ptr;
					isP = !isP;
				} else if ((*it)->coor.first == MIN_INT) {
					it = (*tmp)->ptr;
				}
			}
		}
	}

	v.clear();
	for (auto point : resList) {
		v.push_back(point->coor);
	}

	connectVertices();

	if (interior.size()) {
		interior = list< pair<int, int> >();
		fill();
	}
}