#ifndef HW1_POLYGON_H
#define HW1_POLYGON_H

#include <QtWidgets/QMainWindow>
#include "ui_hw1_polygon.h"

#include "UIWidget.h"

#include <QAction>
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

	canvas::UIWidget *uiWidget;
	QWidget *widget;
	QGridLayout *mainLayout;
	QListView *polygonListView;
	QStringListModel *polygonModel;
	QStringList *polygonList;

	QAction *fillPolygonAction;
	QAction *addInnerRingAction;
	QAction *horizontalFlipAction;
	QAction *verticalFlipAction;
	QAction *cutAction;

	int hoverPolygon, nowPolygon;
	int cutPolygon1, cutPolygon2;
	bool innerMode, cutMode;

public slots:
	void insertPolygonLabel(int);
	void polygonClicked(QModelIndex);
	void polygonEntered(QModelIndex);
	void fillPolygon();
	void setInnerMode();
	void setSelectedPolygon(int);
	void horizontalFlip();
	void verticalFlip();
	void setCutMode();
	void cut();
};

#endif // HW1_POLYGON_H
