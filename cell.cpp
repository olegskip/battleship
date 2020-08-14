#include "cell.h"

Cell::Cell(QWidget *widget, QPoint absPos, QPoint _relPos, unsigned int sideSize, CellOwner _cellOwner):
	QPushButton(widget), relPos(_relPos), cellOwner(_cellOwner)
{
	move(absPos);
	resize(sideSize, sideSize);
//	setText(QString::number(relPos.x()) + " " + QString::number(relPos.y()));
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
