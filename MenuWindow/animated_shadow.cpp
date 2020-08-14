#include "animated_shadow.h"


AnimatedShadow::AnimatedShadow(QWidget *parent, QColor color, int delay, int animationDuration, int fromOpacity, int toOpacity):
	QLabel(parent), startOpacity(fromOpacity), endOpacity(toOpacity)
{
	assert(animationDuration > 0 && "Duration of the AnimatedShadow must be greater than 0");
	assert(fromOpacity >= 0 && "Start opacity of the AnimatedShadow must be greater than or equal to 0");
	assert(endOpacity <= 1000 && "End opacity of the AnimatedShadow must be greater than or equal to 1000");
	animation = QPointer<QPropertyAnimation>(new QPropertyAnimation(this, "opacity"));
	animation->setDuration(animationDuration);

	mOpacity = startOpacity;
	animation->setStartValue(startOpacity);
	animation->setEndValue(endOpacity);

	this->color = color;
	setColor(color, mOpacity);

	connect(animation, &QPropertyAnimation::finished, this, [this, delay]()
	{
		if(mOpacity == endOpacity)  {
			animation->setStartValue(endOpacity);
			animation->setEndValue(startOpacity);
			emit finished();
			QTimer::singleShot(delay, [this]()
			{
				animation->start();
			});
		}
		else {
			animation->setStartValue(startOpacity);
			animation->setEndValue(endOpacity);
			isStarted = false;
			lower();
		}
	});
}

int AnimatedShadow::opacity() const
{
	return mOpacity;
}

void AnimatedShadow::setOpacity(int opacity)
{
	setColor(color, opacity);
}

void AnimatedShadow::start(QSize windowSize)
{
	if(!isStarted) {
		isStarted = true;
		resize(windowSize);
		raise();
		animation->start();
	}
}

void AnimatedShadow::setColor(QColor color, int opacity)
{
	setStyleSheet(QString("background-color: rgb(%1, %2, %3, %4);").arg(QString::number(color.red()), QString::number(color.green()),
																	QString::number(color.blue()), QString::number(opacity)));
	mOpacity = opacity;
}
