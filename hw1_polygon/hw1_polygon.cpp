#include "hw1_polygon.h"

#include <QDebug>

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

	nowPolygon = -1;
	innerMode = false;

	/* =============================================================================== */

	/* ================================ Connections ================================ */

	connect(uiWidget, SIGNAL(polygonInserted(int)), this, SLOT(insertPolygonLabel(int)));
	connect(polygonListView, SIGNAL(clicked(QModelIndex)), this, SLOT(polygonClicked(QModelIndex)));
	connect(fillPolygonButton, &QPushButton::released, this, &hw1_polygon::fillPolygon);
	connect(addInnerRingButton, &QPushButton::released, this, &hw1_polygon::setInnerMode);

	/* ============================================================================= */

	/* ================================ Other operations ================================ */

	polygonListView->setFixedSize(100, 200);
	polygonListView->setModel(polygonModel);

	mainLayout->addWidget(uiWidget, 0, 0, 5, 5);
	mainLayout->addWidget(polygonListView, 0, 5, 1, 1);
	mainLayout->addWidget(fillPolygonButton, 1, 5, 1, 1);
	mainLayout->addWidget(addInnerRingButton, 2, 5, 1, 1);

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
			uiWidget->setDrawStatus(STATUS_INNER);
		} else {
			addInnerRingButton->setText(tr("Enable inner mode"));
			uiWidget->setDrawStatus(STATUS_DONE);
		}
	}
}