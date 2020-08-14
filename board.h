#pragma once
#include <QDateTime>
#include <QVector>
#include <QWidget>
#include <QPointer>
#include <QPair>
#include <QtMath>

#include "cell.h"
#include "ship.h"


class Board: public QWidget
{
	Q_OBJECT

public:
	Board(QWidget *parent, unsigned int cellSideSize);

	void random();

private:
	QVector<QVector<QPointer<Cell>>> cells;
	QVector<QPointer<Ship>> ships;

	const QPointer<Cell> findTheNearestCell(QPoint absPos);
	bool isCanPutShip(const Ship &movedShip, const Cell &cell);
	QVector<QPoint> getAllRelPoints(QPoint from, QPoint to) const;
};
