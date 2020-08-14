#pragma once
#include <QPushButton>


enum class CellOwner
{
	PLAYER,
	BOT
};

class Cell: public QPushButton
{
public:
	Cell(QWidget *widget, QPoint absPos, QPoint _relPos, unsigned int sideSize, CellOwner _cellOwner);

	const QPoint relPos;
	const CellOwner cellOwner;

	void setText(const QString &text);
	bool isCoord() const;

private:
	bool mIsCoord = false;
};
