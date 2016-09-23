#ifndef HW1_POLYGON_H
#define HW1_POLYGON_H

#include <QtWidgets/QMainWindow>
#include "ui_hw1_polygon.h"

#include "UIWidget.h"

#include <QGridLayout>

class hw1_polygon : public QMainWindow {
	Q_OBJECT

public:
	hw1_polygon(QWidget *parent = 0);
	~hw1_polygon();

private:
	Ui::hw1_polygonClass ui;

	UIWidget *uiWidget;
	QWidget *widget;
	QGridLayout *mainLayout;
};

#endif // HW1_POLYGON_H
