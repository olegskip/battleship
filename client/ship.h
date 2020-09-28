#ifndef SHIP_H
#define SHIP_H

#include <QApplication>
#include <QPushButton>
#include <QMouseEvent>
#include <QKeyEvent>


// --- Ship ---
class Ship: public QPushButton
{
	Q_OBJECT
public:
	explicit Ship(QWidget *parent, QPoint absPos, QPoint relPos, QSize cellSize, int _shipLevel, bool isVertical);

	const int shipLevel;

	QPoint startRelPos() const;
	QPoint endRelPos() const;
	static QPoint calculateEndRelPos(QPoint startRelPos, bool isVertical, int shipLevel);
	void setRelPos(QPoint newRelPos);
	void setVertical(bool verticalState);
	bool isVertical() const;
	void rotate();
	void resetVerticalState();

	void adjustSize(QSize cellSize);

signals:
	void adjustSizeRequsted();

private:
	bool mIsVertical = false;
	bool lastVerticalState = mIsVertical;

	QPoint mStartRelPos, mEndRelPos;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	QPoint offset;

	void keyPressEvent(QKeyEvent *event) override;

	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;
	bool isMoving = false;
};
#endif // SHIP_H
