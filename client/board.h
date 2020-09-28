#ifndef BOARD_H
#define BOARD_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QGridLayout>
#include <QDateTime>
#include <QVector>
#include <QWidget>
#include <QPointer>
#include <QtMath>
#include <QPair>

#include "cell.h"
#include "ship.h"


// --- Board ---
class Board: public QWidget
{
	Q_OBJECT

public:
	explicit Board(QWidget *parent);
	QJsonArray getShipsJsonData();

	void adjustShips();

	void random();

protected:
	QVector<QVector<QPointer<Cell>>> cells;
	QVector<QPointer<Ship>> ships;

	const QPointer<Cell> findTheNearestCell(QPoint absPos);
	bool isCanPutShip(const Ship &movedShip, const Cell &cell);
	QVector<QPoint> getAllRelPoints(QPoint from, QPoint to) const;

private:
	QPointer<QGridLayout> mainGridLayout;

	void resizeEvent(QResizeEvent *) override;
	void fromJsonData(QJsonArray ships);
};

#endif // BOARD_H
