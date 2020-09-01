#ifndef POP_UP_MESSAGE_H
#define POP_UP_MESSAGE_H

#include <QPropertyAnimation>
#include <QLabel>
#include <QPointer>
#include <QTimer>
#include <QFont>
#include <queue>


enum class MessageType
{
	OPENINING,
	OPEN,
	CLOSING,
	CLOSED
};

class PopUpMessage: public QLabel
{
	Q_OBJECT

public:
	PopUpMessage(QWidget *parent, unsigned height);

	const QString messageText;
	const QSize messageSize;
	const QPoint pos;

	void popUp(const QString &messageText, unsigned int messageWidth);

private:
	QPointer<QPropertyAnimation> animation;

	void open(const QString &messageText, unsigned int messageWidth);
	void close();
	MessageType currentMessageType = MessageType::CLOSED;

	std::queue<QString> messagesQueue;
};

#endif // POP_UP_MESSAGE_H
