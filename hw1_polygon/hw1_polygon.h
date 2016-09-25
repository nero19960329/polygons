#ifndef HW1_POLYGON_H
#define HW1_POLYGON_H

#include <QtWidgets/QMainWindow>
#include "ui_hw1_polygon.h"

#include "UIWidget.h"

#include <QGridLayout>
#include <QListView>
#include <QModelIndex>
#include <QPushButton>
#include <QStringList>
#include <QStringListModel>

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
	QListView *polygonListView;
	QStringListModel *polygonModel;
	QStringList *polygonList;
	QPushButton *fillPolygonButton;
	QPushButton *addInnerRingButton;

	int nowPolygon;
	bool innerMode;

public slots:
	void insertPolygonLabel(int);
	void polygonClicked(QModelIndex);
	void fillPolygon();
	void setInnerMode();
};

#endif // HW1_POLYGON_H
