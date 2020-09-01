#include "config.h"
#include "board.h"


Board::Board(QWidget *parent, unsigned int cellSideSize):
	QWidget(parent)
{
	qsrand(QDateTime::currentMSecsSinceEpoch() / 1000);

	for(int x = 0; x < 11; ++x) {
		cells.push_back(QVector<QPointer<Cell>>());
		for(int y = 0; y < 11; ++y) {
			cells.back().push_back(QPointer<Cell>(new Cell(this, QPoint(x, y) * int(cellSideSize),
														  QPoint(x, y), cellSideSize, CellOwner::PLAYER)));
		}
	}

	cells[0][0]->setText("");
	for(int x = 1; x < 11; ++x)
		cells[x][0]->setText(QString(char(64 + x)));
	for(int y = 1, n = 0; y < 11; ++y, ++n)
		cells[0][y]->setText(QString::number(y));

	for(auto &vec: cells) {
		for(auto &cell: vec) {

		}
	}

	// fill by ships
	ships.push_back(QPointer<Ship>(new Ship(this, cells[1][1]->pos(), QPoint(1, 1), 30, 4, false)));
	for(int i = 1; i < 5; i += 2) {
		ships.push_back(QPointer<Ship>(new Ship(this, cells[i][3]->pos(), QPoint(i, 3),
												30, 3, true)));
	}
	for(int i = 1; i < 7; i += 2) {
		ships.push_back(QPointer<Ship>(new Ship(this, cells[i][7]->pos(), QPoint(i, 7),
												30, 2, true)));
	}
	for(int i = 1; i < 9; i += 2) {
		ships.push_back(QPointer<Ship>(new Ship(this, cells[i][10]->pos(), QPoint(i, 10),
												30, 1, true)));
	}
	for(auto &ship: ships) {
		connect(ship, &QPushButton::clicked, this, [this, &ship]()
		{
			const auto &theNearestCell = findTheNearestCell(ship->pos());
			if(!isCanPutShip(*ship, *theNearestCell)) {
				ship->move(cells[ship->startRelPos().x()][ship->startRelPos().y()]->pos());
			}
			else {
				ship->setRelPos(theNearestCell->relPos);
				ship->move(theNearestCell->pos());
			}
		});
	}
	random();
//	setLayout(mainVerticalLayout);
}

void Board::random()
{
	QVector<QPoint> shipsAllPoints;

	for(auto &ship: ships) {
		QPoint newShipStartRelPos;
		bool newShipIsVertical;
		QPoint newShipEndRelPos;
		QVector<QPoint> newShipAllPoints;
		bool isCanPutNewShip = true;
		do {
			newShipStartRelPos = QPoint(1 + qrand() % 10, 1 + qrand() % 10);
			newShipIsVertical = bool(qrand() % 2);
			newShipEndRelPos = Ship::calculateEndRelPos(newShipStartRelPos, newShipIsVertical, ship->shipLevel);
			newShipAllPoints = getAllRelPoints(newShipStartRelPos, newShipEndRelPos);
			isCanPutNewShip = true;
			for(QPoint newShipPoint: newShipAllPoints) {
				if(newShipPoint.x() < 1 || newShipPoint.y() < 1 || newShipPoint.x() >= cells.size() ||
						newShipPoint.y() >= cells.size()) {
					isCanPutNewShip = false;
					break;
				}

				for(QPoint shipPoint: shipsAllPoints) {
					const float distance = qSqrt(qPow(shipPoint.x() - newShipPoint.x(), 2) + qPow(shipPoint.y() - newShipPoint.y(), 2));
					if(distance <= 1 || (std::abs(shipPoint.x() - newShipPoint.x()) == std::abs(shipPoint.y() - newShipPoint.y()) &&
											 std::abs(shipPoint.x() - newShipPoint.x()) == 1)) {
						isCanPutNewShip = false;
						break;
					}
				}
				if(!isCanPutNewShip)
					break;
			}
		} while(!isCanPutNewShip);

		if(isCanPutNewShip) {
			std::copy(newShipAllPoints.begin(), newShipAllPoints.end(), std::back_inserter(shipsAllPoints));
			ship->setRelPos(newShipStartRelPos);
			ship->move(cells[newShipStartRelPos.x()][newShipStartRelPos.y()]->pos());
			ship->setVertical(newShipIsVertical);
		}
		else {
			ship->setRelPos(QPoint(0, 0));
			ship->move(0, 0);
		}
	}
}

QJsonArray Board::getShipsJsonData()
{
	QJsonArray jsonArray;

	for(const auto &ship: ships) {
		QJsonObject currentShipJsonObject;
		currentShipJsonObject["ship_level"] = int(ship->shipLevel);
		currentShipJsonObject["start_rel_pos"] = QJsonArray({ship->startRelPos().x(), ship->startRelPos().y()});
		currentShipJsonObject["end_rel_pos"] = QJsonArray({ship->endRelPos().x(), ship->endRelPos().y()});

		jsonArray.append(currentShipJsonObject);
	}

	return jsonArray;
}

const QPointer<Cell> Board::findTheNearestCell(QPoint absPos)
{
	QPair<QPointer<Cell> /* index */, float /* distance */> shipWithMinimumDistance = {QPointer<Cell>(), std::numeric_limits<float>::max()};
	for(auto &vec: cells) {
		for(auto &cell: vec) {
			const float distance = qPow(absPos.x() - cell->pos().x(), 2) + qPow(absPos.y() - cell->pos().y(), 2);
			if(distance < shipWithMinimumDistance.second)
				shipWithMinimumDistance = {cell, distance};
		}
	}

	return shipWithMinimumDistance.first;
}

bool Board::isCanPutShip(const Ship &movedShip, const Cell &cell)
{
	const QPoint newEndRelPos = Ship::calculateEndRelPos(cell.relPos, movedShip.isVertical(), movedShip.shipLevel);
	const QVector<QPoint> allRelPoints = getAllRelPoints(cell.relPos, newEndRelPos);
	if(std::any_of(allRelPoints.begin(), allRelPoints.end(), [this](QPoint point)
	{
		return point.x() < 1 || point.y() < 1 || point.x() >= cells.size() || point.y() >= cells.size();
	}))
		return false;

	for(const auto &ship: ships) {
		if(ship->startRelPos() == movedShip.startRelPos()) continue;

		const QVector<QPoint> shipAllPoints = getAllRelPoints(ship->startRelPos(), ship->endRelPos());
		for(QPoint cellPoint: allRelPoints) {
			if(std::any_of(shipAllPoints.begin(), shipAllPoints.end(), [cellPoint](QPoint shipPoint)
			{
				const float distance = qSqrt(qPow(shipPoint.x() - cellPoint.x(), 2) + qPow(shipPoint.y() - cellPoint.y(), 2));
				return distance <= 1 || (std::abs(shipPoint.x() - cellPoint.x()) == std::abs(shipPoint.y() - cellPoint.y()) &&
										 std::abs(shipPoint.x() - cellPoint.x()) == 1);
			}))
				return false;
		}
	}
	return true;
}

QVector<QPoint> Board::getAllRelPoints(QPoint from, QPoint to) const
{
	QVector<QPoint> output;

	if(from.x() == to.x()) {
		for(int y = from.y(); y <= to.y(); ++y)
			output.push_back(QPoint(from.x(), y));
	}
	else if(from.y() == to.y()) {
		for(int x = from.x(); x <= to.x(); ++x)
			output.push_back(QPoint(x, from.y()));
	}

	return output;
}
