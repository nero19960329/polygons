#include "hw1_polygon.h"

#include <QDebug>

using namespace canvas;

hw1_polygon::hw1_polygon(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	/* ================================ Initliazation ================================ */
	
	uiWidget = new UIWidget(this);
	widget = new QWidget(this);
	mainLayout = new QGridLayout;
	polygonListView = new QListView;
	polygonModel = new QStringListModel;
	polygonList = new QStringList;
	fillPolygonButton = new QPushButton(tr("Fill polygon"));
	addInnerRingButton = new QPushButton(tr("Enable inner mode"));
	horizontalFlipButton = new QPushButton(tr("Horizontal flip"));
	verticalFlipButton = new QPushButton(tr("Vertical flip"));
	cutButton = new QPushButton(tr("Cut"));

	nowPolygon = -1;
	innerMode = false;

	/* =============================================================================== */

	/* ================================ Connections ================================ */

	connect(uiWidget, SIGNAL(polygonInserted(int)), this, SLOT(insertPolygonLabel(int)));
	connect(polygonListView, SIGNAL(clicked(QModelIndex)), this, SLOT(polygonClicked(QModelIndex)));
	connect(fillPolygonButton, &QPushButton::released, this, &hw1_polygon::fillPolygon);
	connect(addInnerRingButton, &QPushButton::released, this, &hw1_polygon::setInnerMode);
	connect(uiWidget, SIGNAL(polygonSelected(int)), this, SLOT(setSelectedPolygon(int)));
	connect(horizontalFlipButton, &QPushButton::released, this, &hw1_polygon::horizontalFlip);
	connect(verticalFlipButton, &QPushButton::released, this, &hw1_polygon::verticalFlip);
	connect(cutButton, &QPushButton::released, this, &hw1_polygon::cut);

	/* ============================================================================= */

	/* ================================ Other operations ================================ */

	polygonListView->setFixedSize(100, 200);
	polygonListView->setModel(polygonModel);

	mainLayout->addWidget(uiWidget, 0, 0, 10, 5);
	mainLayout->addWidget(polygonListView, 0, 5, 1, 1);
	mainLayout->addWidget(fillPolygonButton, 1, 5, 1, 1);
	mainLayout->addWidget(addInnerRingButton, 2, 5, 1, 1);
	mainLayout->addWidget(horizontalFlipButton, 3, 5, 1, 1);
	mainLayout->addWidget(verticalFlipButton, 4, 5, 1, 1);
	mainLayout->addWidget(cutButton, 5, 5, 1, 1);

	widget->setLayout(mainLayout);

	/* ================================================================================== */

	this->setCentralWidget(widget);
}

hw1_polygon::~hw1_polygon() {

}

void hw1_polygon::insertPolygonLabel(int k) {
	polygonList->append(tr("Polygon ") + QString::number(k));
	polygonModel->setStringList(*polygonList);
}

void hw1_polygon::polygonClicked(QModelIndex index) {
	if (nowPolygon != -1) {
		uiWidget->recoverPolygon(nowPolygon);
	}

	QString str = index.data().toString().mid(8);
	bool ok;
	nowPolygon = str.toInt(&ok, 10);

	uiWidget->highlightPolygon(nowPolygon);
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
			addInnerRingButton->setText(tr("Disable inner mode"));
			uiWidget->setCurrentPolygon(nowPolygon);
			uiWidget->setInnerFlag(true);
		} else {
			addInnerRingButton->setText(tr("Enable inner mode"));
			uiWidget->setInnerFlag(false);
		}
	}
}

void hw1_polygon::setSelectedPolygon(int idx) {
	if (nowPolygon != -1 && idx != -1) {
		if (nowPolygon == idx) {
			return;
		} else {
			uiWidget->recoverPolygon(nowPolygon);
			uiWidget->highlightPolygon(idx);
		}
	} else {
		if (nowPolygon == idx) {
			return;
		} else if (nowPolygon != -1) {
			uiWidget->recoverPolygon(nowPolygon);
		} else {
			uiWidget->highlightPolygon(idx);
		}
	}
	nowPolygon = idx;
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

void hw1_polygon::cut() {
	uiWidget->polygonCut(0, 1);
}