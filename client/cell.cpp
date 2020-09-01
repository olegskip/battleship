#include "cell.h"
#include "config.h"

#include <QFont>

Cell::Cell(QWidget *widget, QPoint absPos, QPoint _relPos, unsigned int sideSize, CellOwner _cellOwner):
	QPushButton(widget), relPos(_relPos), cellOwner(_cellOwner)
{
	move(absPos);
	resize(sideSize, sideSize);

	QFont newFont = font();
	newFont.setFamily("Roobert");
	newFont.setPixelSize(17);
	setFont(newFont);

	const int cellsSize = 11;
	if(relPos.x() == 0 && relPos.y() == 0)
		setStyleSheet(QString("border-width: 1px; border-style: solid; border-color: transparent %1 %1 transparent; color: white;").
							arg(config::CELL_BORDER_COLOR.name()));
	else if(relPos.x() == 0 && relPos.y() == cellsSize - 1)
		setStyleSheet(QString("border-width: 1px; border-style: solid; border-color: %1 %1 transparent transparent; color: white;").
							arg(config::CELL_BORDER_COLOR.name()));
	else if(relPos.x() == cellsSize - 1 && relPos.y() == cellsSize - 1)
		setStyleSheet(QString("border-width: 1px; border-style: solid; border-color: %1 transparent transparent %1; color: white;").
							arg(config::CELL_BORDER_COLOR.name()));
	else if(relPos.x() == cellsSize - 1 && relPos.y() == 0)
		setStyleSheet(QString("border-width: 1px; border-style: solid; border-color: transparent transparent %1 %1; color: white;").
							arg(config::CELL_BORDER_COLOR.name()));
	else if(relPos.x() == 0)
		setStyleSheet(QString("border-width: 1px; border-style: solid; border-color: %1 %1 %1 transparent; color: white;").
							arg(config::CELL_BORDER_COLOR.name()));
	else if(relPos.x() == cellsSize - 1)
		setStyleSheet(QString("border-width: 1px; border-style: solid; border-color: %1 transparent %1 %1; color: white;").
							arg(config::CELL_BORDER_COLOR.name()));
	else if(relPos.y() == 0)
		setStyleSheet(QString("border-width: 1px; border-style: solid; border-color: transparent %1 %1 %1; color: white;").
							arg(config::CELL_BORDER_COLOR.name()));
	else if(relPos.y() == cellsSize - 1)
		setStyleSheet(QString("border-width: 1px; border-style: solid; border-color: %1 %1 transparent %1; color: white;").
							arg(config::CELL_BORDER_COLOR.name()));
	else
		setStyleSheet(QString("border-width: 1px; border-style: solid; border-color: %1 %1 %1 %1; color: white;").
							arg(config::CELL_BORDER_COLOR.name()));
}

void Cell::setText(const QString &text)
{
	mIsCoord = true;
	QPushButton::setText(text);
}

bool Cell::isCoord() const
{
	return mIsCoord;
}
