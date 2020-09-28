#ifndef ANIMATED_SHADOW_H
#define ANIMATED_SHADOW_H

#include <QPropertyAnimation>
#include <QApplication>
#include <QPointer>
#include <QLabel>
#include <QColor>
#include <QTimer>


class AnimatedShadow: public QLabel
{
	Q_OBJECT
	Q_PROPERTY(int opacity READ opacity WRITE setOpacity)

public:
	explicit AnimatedShadow(QWidget *parent, QColor color, int delay, int animationDuration);

	int opacity() const;
	void setOpacity(int opacity);

	void start(QSize windowSize);

signals:
	void finished();

private:
	const int startOpacity = 0, endOpacity = 255;
	int mOpacity;
	QColor color;
	void setColor(QColor color, int opacity = 0);

	QPointer<QPropertyAnimation> animation;
	bool isStarted = false;

	using QLabel::setStyleSheet;
	using QLabel::setText;
};

#endif // ANIMATED_SHADOW_H
