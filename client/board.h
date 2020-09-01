#ifndef BOARD_H
#define BOARD_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QVector>
#include <QWidget>
#include <QPointer>
#include <QtMath>
#include <QPair>

#include "cell.h"
#include "ship.h"


class Board: public QWidget
{
	Q_OBJECT

public:
	Board(QWidget *parent, unsigned int cellSideSize);

	void random();

	QJsonArray getShipsJsonData();

	QVBoxLayout mainVerticalLayout;
	QVector<QHBoxLayout> horizontalLayouts;

private:
	QVector<QVector<QPointer<Cell>>> cells;
	QVector<QPointer<Ship>> ships;

	const QPointer<Cell> findTheNearestCell(QPoint absPos);
	bool isCanPutShip(const Ship &movedShip, const Cell &cell);
	QVector<QPoint> getAllRelPoints(QPoint from, QPoint to) const;
};

#endif // BOARD_H
