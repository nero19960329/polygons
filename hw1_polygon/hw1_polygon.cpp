#include "hw1_polygon.h"

hw1_polygon::hw1_polygon(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	/* ================================ Initliazation ================================ */
	
	uiWidget = new UIWidget(this);
	widget = new QWidget(this);
	mainLayout = new QGridLayout;

	/* =============================================================================== */

	/* ================================ Other operations ================================ */

	mainLayout->addWidget(uiWidget, 0, 0, 1, 1);

	widget->setLayout(mainLayout);

	/* ================================================================================== */

	this->setCentralWidget(widget);
}

hw1_polygon::~hw1_polygon() {

}
