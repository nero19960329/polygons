#include "hw1_polygon.h"

#include <QDebug>
#include <QToolBar>

using namespace canvas;

hw1_polygon::hw1_polygon(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	/* ================================ Initliazation ================================ */
	
	uiWidget = new UIWidget(this, &nowPolygon, &hoverPolygon);
	widget = new QWidget(this);
	mainLayout = new QGridLayout;
	polygonListView = new QListView;
	polygonModel = new QStringListModel;
	polygonList = new QStringList;
	fillPolygonAction = new QAction(QStringLiteral("填充多边形"), parent);
	addInnerRingAction = new QAction(QStringLiteral("进入内环模式"), parent);
	horizontalFlipAction = new QAction(QStringLiteral("水平翻转"), parent);
	verticalFlipAction = new QAction(QStringLiteral("竖直翻转"), parent);
	cutAction = new QAction(QStringLiteral("裁剪"), this);

	nowPolygon = -1;
	hoverPolygon = -1;
	innerMode = false;
	cutMode = false;

	/* =============================================================================== */

	/* ================================ Connections ================================ */

	connect(uiWidget, SIGNAL(polygonInserted(int)), this, SLOT(insertPolygonLabel(int)));
	connect(polygonListView, SIGNAL(clicked(QModelIndex)), this, SLOT(polygonClicked(QModelIndex)));
	connect(polygonListView, SIGNAL(entered(QModelIndex)), this, SLOT(polygonEntered(QModelIndex)));
	connect(uiWidget, SIGNAL(polygonSelected(int)), this, SLOT(setSelectedPolygon(int)));
	connect(fillPolygonAction, &QAction::triggered, this, &hw1_polygon::fillPolygon);
	connect(addInnerRingAction, &QAction::triggered, this, &hw1_polygon::setInnerMode);
	connect(horizontalFlipAction, &QAction::triggered, this, &hw1_polygon::horizontalFlip);
	connect(verticalFlipAction, &QAction::triggered, this, &hw1_polygon::verticalFlip);
	connect(cutAction, &QAction::triggered, this, &hw1_polygon::setCutMode);
	connect(uiWidget, &UIWidget::closeInnerMode, this, &hw1_polygon::setInnerMode);

	/* ============================================================================= */

	/* ================================ Other operations ================================ */

	polygonListView->setMouseTracking(true);
	polygonListView->setFixedSize(100, 200);
	polygonListView->setModel(polygonModel);

	mainLayout->addWidget(uiWidget, 0, 0, 10, 5);
	mainLayout->addWidget(polygonListView, 0, 5, 1, 1);

	ui.mainToolBar->addAction(fillPolygonAction);
	ui.mainToolBar->addAction(addInnerRingAction);
	ui.mainToolBar->addAction(horizontalFlipAction);
	ui.mainToolBar->addAction(verticalFlipAction);
	ui.mainToolBar->addAction(cutAction);

	widget->setLayout(mainLayout);

	/* ================================================================================== */

	this->setCentralWidget(widget);
}

hw1_polygon::~hw1_polygon() {

}

void hw1_polygon::insertPolygonLabel(int k) {
	polygonList->append(tr("Polygon ") + QString::number(k));
	polygonModel->setStringList(*polygonList);

	nowPolygon = k;
}

void hw1_polygon::polygonClicked(QModelIndex index) {
	if (nowPolygon != -1) {
		uiWidget->recoverPolygon(nowPolygon);
	}

	QString str = index.data().toString().mid(8);
	bool ok;
	nowPolygon = str.toInt(&ok, 10);

	if (cutMode) {
		cutPolygon2 = nowPolygon;
		cut();
		cutMode = false;
	} else {
		uiWidget->highlightPolygon(nowPolygon);
		repaint();
	}
}

void hw1_polygon::polygonEntered(QModelIndex index) {
	if (hoverPolygon != -1 && hoverPolygon != nowPolygon) {
		uiWidget->recoverPolygon(hoverPolygon);
	}

	QString str = index.data().toString().mid(8);
	bool ok;
	hoverPolygon = str.toInt(&ok, 10);

	uiWidget->highlightPolygon(hoverPolygon);
	repaint();
}

void hw1_polygon::fillPolygon() {
	if (nowPolygon != -1) {
		uiWidget->fillPolygon(nowPolygon);
	}
}

void hw1_polygon::setInnerMode() {
	if (nowPolygon != -1) {
		innerMode = !innerMode;

		if (innerMode) {
			addInnerRingAction->setText(QStringLiteral("退出内环模式"));
			uiWidget->setCurrentPolygon(nowPolygon);
			uiWidget->setInnerFlag(true);
		} else {
			addInnerRingAction->setText(QStringLiteral("进入内环模式"));
			uiWidget->setInnerFlag(false);
		}
	}
}

void hw1_polygon::setSelectedPolygon(int idx) {
	if (hoverPolygon != -1 && hoverPolygon != nowPolygon && idx == -1) {
		uiWidget->recoverPolygon(hoverPolygon);
		repaint();
	}

	if (nowPolygon != -1 && idx != -1) {
		if (nowPolygon == idx) {
			return;
		} else {
			uiWidget->recoverPolygon(nowPolygon);
			uiWidget->highlightPolygon(idx);
			nowPolygon = idx;
		}
	} else {
		if (nowPolygon == idx) {
			return;
		} else if (nowPolygon != -1) {
			//uiWidget->recoverPolygon(nowPolygon);
		} else {
			uiWidget->highlightPolygon(idx);
			nowPolygon = idx;
		}
	}
	repaint();
}

void hw1_polygon::horizontalFlip() {
	if (nowPolygon != -1) {
		uiWidget->polygonHorizontalFlip(nowPolygon);
	}
}

void hw1_polygon::verticalFlip() {
	if (nowPolygon != -1) {
		uiWidget->polygonVerticalFlip(nowPolygon);
	}
}

void hw1_polygon::setCutMode() {
	cutMode = true;
	if (nowPolygon != -1) {
		cutPolygon1 = nowPolygon;
	} else {

	}
}

void hw1_polygon::cut() {
	uiWidget->polygonCut(cutPolygon1, cutPolygon2);
	polygonList->removeAll(tr("Polygon ") + QString::number(cutPolygon2));
	polygonModel->setStringList(*polygonList);
	nowPolygon = cutPolygon1;
	hoverPolygon = -1;
}