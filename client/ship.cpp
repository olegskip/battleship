#include "ship.h"

// --- Ship---
Ship::Ship(QWidget *parent, QPoint absPos, QPoint relPos, QSize cellSize, int _shipLevel, bool isVertical):
	QPushButton(parent), shipLevel(_shipLevel)
{
	mIsVertical = isVertical;
	adjustSize(cellSize);
	calculateEndRelPos(mStartRelPos, mIsVertical, shipLevel);
	setRelPos(relPos);
	adjustSizeRequsted();

	assert((shipLevel >= 1 && shipLevel <= 4) && "Ship level must be [1; 4]");
	assert((mStartRelPos.x() >= 1 && mStartRelPos.x() <= 11) && "Ship start relative position x must be [1; 11]");
	assert((mStartRelPos.y() >= 1 && mStartRelPos.y() <= 11) && "Ship start relative position y must be [1; 11]");
	assert((mEndRelPos.x() >= 1 && mEndRelPos.x() <= 11) && "Ship end relative position x must be [1; 11]");
	assert((mEndRelPos.y() >= 1 && mEndRelPos.y() <= 11) && "Ship end relative position y must be [1; 11]");

	connect(this, &QPushButton::pressed, [this]()
	{
		isMoving = true;
	});
	connect(this, &QPushButton::clicked, [this]()
	{
		isMoving = false;
	});

	move(absPos);
	setStyleSheet("border: 3px solid red;");
	setFocusPolicy(Qt::ClickFocus);
}

QPoint Ship::startRelPos() const
{
	return mStartRelPos;
}

QPoint Ship::endRelPos() const
{
	return mEndRelPos;
}

QPoint Ship::calculateEndRelPos(QPoint startRelPos, bool isVertical, int shipLevel)
{
	return isVertical ? QPoint(startRelPos.x(), startRelPos.y() + shipLevel - 1) : QPoint(startRelPos.x() + shipLevel - 1, startRelPos.y());
}

void Ship::setRelPos(QPoint newRelPos) // virtual
{
	mStartRelPos = newRelPos;
	mEndRelPos = calculateEndRelPos(mStartRelPos, mIsVertical, shipLevel);
	lastVerticalState = mIsVertical;
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
	mEndRelPos = calculateEndRelPos(mStartRelPos, mIsVertical, shipLevel);
	emit adjustSizeRequsted();
}

void Ship::resetVerticalState()
{
	if(mIsVertical != lastVerticalState) {
		rotate();
		lastVerticalState = mIsVertical;
	}
}

void Ship::adjustSize(QSize cellSize)
{
	mIsVertical ? resize(cellSize.width(), cellSize.height() * shipLevel) :
				  resize(cellSize.width() * shipLevel, cellSize.height());
	offset = QPoint(-size().width() / 2, -size().height() / 2);
}

void Ship::mousePressEvent(QMouseEvent *event) // override
{
	if(event->buttons() & Qt::LeftButton) {
		offset = QPoint(-size().width() / 2, -size().height() / 2);
		mouseMoveEvent(event);
	}

	QPushButton::mousePressEvent(event);
}

void Ship::mouseMoveEvent(QMouseEvent *event) // override
{
	if(event->buttons() & Qt::LeftButton) {
		raise();

		QPushButton::move(mapToParent(event->pos() - QPoint(width() / 2, height() / 2)));
	}

	QPushButton::mouseMoveEvent(event);
}

void Ship::keyPressEvent(QKeyEvent *event) // override
{
	if(event->key() == Qt::Key_Shift && isMoving) {
		const QPoint cursorPosition = mapToParent(mapFromGlobal(QCursor::pos()));
		rotate();
		QPushButton::move(cursorPosition.x() - width() / 2, cursorPosition.y() - height() / 2);
	}

	QPushButton::keyPressEvent(event);
}

void Ship::enterEvent(QEvent *) // override
{
	QApplication::setOverrideCursor(Qt::OpenHandCursor);
}

void Ship::leaveEvent(QEvent *) // override
{
	QApplication::restoreOverrideCursor();
}
