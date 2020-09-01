#ifndef SHIP_H
#define SHIP_H

#include <QApplication>
#include <QPushButton>
#include <QMouseEvent>
#include <QKeyEvent>


class Ship: public QPushButton
{
	Q_OBJECT
public:
	Ship(QWidget *parent, QPoint absPos, QPoint relPos, unsigned int _shipSideSize, unsigned int _shipLevel, bool isVertical);

	const unsigned int shipLevel;
	const unsigned int shipSideSize;

	void setVertical(bool verticalState);
	bool isVertical() const;
	void rotate();

	void setRelPos(QPoint newRelPos);
	QPoint startRelPos() const;
	QPoint endRelPos() const;
	static QPoint calculateEndRelPos(QPoint startRelPos, bool isVertical, unsigned int shipLevel);

private:
	void mouseMoveEvent(QMouseEvent *event) override;
	QPoint offset;

	void keyPressEvent(QKeyEvent *event);

	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;

	QPoint mStartRelPos, mEndRelPos;
	bool mIsVertical;
	bool isMoving = false; // only why true the user can rotate ship
};

#endif // SHIP_H
