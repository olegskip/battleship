#ifndef CLIENT_H
#define CLIENT_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTcpSocket>
#include <QPointer>
#include <QString>
#include <QObject>
#include <QTimer>


enum class StatusCode
{
	OK = 200,

	BAD_REQUEST = 400,
	CONFLICT = 409
};


class Client: public QObject
{
	Q_OBJECT

public:
	explicit Client();

	void connectToServer(const QString &ip, int port);
	void reconnect();

	void sendCheckingBoard(QJsonArray shipsJsonData);
	void sendCreateRoom(QString roomName);
	void sendJoinRoom(QString roomName);
	void sendDeleteRoom(QString roomName);

signals:
//	void checkConnectionResult(bool state);
	void connectionResult(bool state);
	void disconnected();

	void createRoomRespond(StatusCode statusCode, QString serverComment);
	void joinRoomRespond(StatusCode statusCode, QString serverComment);
	void updateRooms(QJsonObject jsonObject);

private:
	QPointer<QTcpSocket> socket;
	QString lastIp;
	int lastPort;

	void sendJson(const QJsonObject &jsonObject);
	void receiveData();
	void dataController(QString data);

	QTimer connectionTimer;
	bool isTryingToConnect = false;

//	QTimer checkConnectionTimer;
//	bool didSendPing = false;
};

#endif // CLIENT_H
