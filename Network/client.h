#pragma once
#include <QJsonObject>
#include <QTcpSocket>
#include <QPointer>
#include <QString>
#include <QObject>
#include <QTimer>


class Client: public QObject
{
	Q_OBJECT

public:
	Client();

	void connectToServer(const QString &ip, int port);
	void reconnect();
//	void checkConnection();

signals:
//	void checkConnectionResult(bool state);
	void connectionResult(bool state);
	void disconnected();

private:
	QPointer<QTcpSocket> socket;
	QString lastIp;
	int lastPort;

	void receiveData();

	QTimer connectionTimer;
	bool didTryToConnect = false;

//	QTimer checkConnectionTimer;
//	bool didSendPing = false;
};
