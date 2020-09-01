#include "ship.h"

Ship::Ship(QWidget *parent, QPoint absPos, QPoint relPos, unsigned int _shipSideSize, unsigned int _shipLevel, bool isVertical):
	QPushButton(parent), shipLevel(_shipLevel), shipSideSize(_shipSideSize)
{
	setRelPos(relPos);
	assert((shipLevel >= 1 && shipLevel <= 4) && "Ship level must be [1; 4]");
	assert((mStartRelPos.x() >= 1 && mStartRelPos.x() <= 11) && "Ship start relative position x must be [1; 11]");
	assert((mStartRelPos.y() >= 1 && mStartRelPos.y() <= 11) && "Ship start relative position y must be [1; 11]");
	assert((mEndRelPos.x() >= 1 && mEndRelPos.x() <= 11) && "Ship end relative position x must be [1; 11]");
	assert((mEndRelPos.y() >= 1 && mEndRelPos.y() <= 11) && "Ship end relative position y must be [1; 11]");

	mIsVertical = !isVertical;
	rotate();

	move(absPos);
	setStyleSheet("border: 3px solid red;");
	setFocusPolicy(Qt::ClickFocus);

	connect(this, &QPushButton::pressed, this, [this]()
	{
		isMoving = true;
	});
	connect(this, &QPushButton::clicked, this, [this]()
	{
		isMoving = false;
	});
}

void Ship::setVertical(bool verticalState)
{
	if(mIsVertical != verticalState)
		rotate();
}

bool Ship::isVertical() const
{
	return mIsVertical;
}

void Ship::rotate()
{
	mIsVertical = !mIsVertical;
	mIsVertical ? resize(shipSideSize, shipSideSize * shipLevel) : resize(shipSideSize * shipLevel, shipSideSize);
	mEndRelPos = calculateEndRelPos(mStartRelPos, mIsVertical, shipLevel);
}

void Ship::setRelPos(QPoint newRelPos)
{
	mStartRelPos = newRelPos;
	mEndRelPos = calculateEndRelPos(mStartRelPos, mIsVertical, shipLevel);
}

QPoint Ship::startRelPos() const
{
	return mStartRelPos;
}

QPoint Ship::endRelPos() const
{
	return mEndRelPos;
}

QPoint Ship::calculateEndRelPos(QPoint startRelPos, bool isVertical, unsigned int shipLevel)
{
	return isVertical ? QPoint(startRelPos.x(), startRelPos.y() + shipLevel - 1) : QPoint(startRelPos.x() + shipLevel - 1, startRelPos.y());
}

void Ship::mouseMoveEvent(QMouseEvent *event) // override
{
	if(event->buttons() & Qt::LeftButton) {
		raise();
		QPushButton::move(mapToParent(event->pos() - QPoint(width() / 2, height() / 2)));
	}
	QPushButton::mouseMoveEvent(event);
}

void Ship::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Shift && isMoving) {
		const QPoint cursorPosition = mapToParent(mapFromGlobal(QCursor::pos()));
		rotate();
		QPushButton::move(cursorPosition.x() - width() / 2, cursorPosition.y() - height() / 2);
	}
	QPushButton::keyPressEvent(event);
}

void Ship::enterEvent(QEvent *event) // override
{
	QApplication::setOverrideCursor(Qt::OpenHandCursor);
}

void Ship::leaveEvent(QEvent *event) // override
{
	QApplication::restoreOverrideCursor();
}
