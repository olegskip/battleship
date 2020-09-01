#include "pop_up_message.h"
#include "config.h"

PopUpMessage::PopUpMessage(QWidget *parent, unsigned height):
	QLabel(parent)
{
	resize(width(), height);
	move(0, -QLabel::height());
	setStyleSheet("padding: 3px;"
				  "background-color: white;"
				  "color: #2f302f;");


	QFont newFont = font();
	newFont.setPixelSize(20);
	newFont.setFamily("Roobert Heavy");
	setFont(newFont);

	setWordWrap(true);
	setAlignment(Qt::AlignCenter);

	animation = QPointer<QPropertyAnimation>(new QPropertyAnimation(this, "pos"));
	animation->setDuration(250);
	connect(animation, &QPropertyAnimation::finished, this, [this]()
	{
		if(currentMessageType == MessageType::OPENINING)
			currentMessageType = MessageType::OPEN;
		else if(currentMessageType == MessageType::CLOSING) {
			currentMessageType = MessageType::CLOSED;
			if(!messagesQueue.empty())
				messagesQueue.pop();
		}

		if(currentMessageType == MessageType::OPEN) {
			QTimer::singleShot(config::POP_UP_MESSAGE_SHOW_DURATION, [this](){
				close();
			});
		}
		if(currentMessageType == MessageType::CLOSED && !messagesQueue.empty())
			open(messagesQueue.front(), -1);
	});
}

void PopUpMessage::popUp(const QString &messageText, unsigned int messageWidth)
{
	if(messagesQueue.empty() || (!messagesQueue.empty() && messagesQueue.back() != messageText)) {
		messagesQueue.push(messageText);
		if(messagesQueue.size() == 1)
			open(messageText, messageWidth);
	}
}

void PopUpMessage::open(const QString &messageText, unsigned int messageWidth)
{
	if(currentMessageType == MessageType::CLOSED) {
		animation->setStartValue(QPoint(0, -height()));
		animation->setEndValue(QPoint(0, 0));
		animation->start();
		setText(messageText);
		if(messageWidth != -1)
			resize(messageWidth, height());
		currentMessageType = MessageType::OPENINING;
	}
}

void PopUpMessage::close()
{
	if(currentMessageType == MessageType::OPEN) {
		animation->setStartValue(QPoint(0, 0));
		animation->setEndValue(QPoint(0, -height()));
		animation->start();
		currentMessageType = MessageType::CLOSING;
	}
}
