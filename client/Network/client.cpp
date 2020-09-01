#include "client.h"
#include "config.h"

#include <QtMath>

Client::Client()
{
	socket = QPointer<QTcpSocket>(new QTcpSocket());

	connect(socket, &QTcpSocket::readyRead, this, &Client::receiveData);
	connect(socket, &QTcpSocket::disconnected, [this]()
	{
		if(!didTryToConnect) {
			reconnect();
			emit disconnected();
		}
	});


	connectionTimer.setInterval(500);
	connect(&connectionTimer, &QTimer::timeout, [this]()
	{
		static int i;
		if(didTryToConnect) {
			++i;
			if(socket->state() == QTcpSocket::UnconnectedState) {
				if(i == qCeil(config::SERVER_CONNECT_DURATION / connectionTimer.interval())) {
					connectionResult(false);
					connectionTimer.stop();
					i = 0;
				}
			}
			else if(socket->state() == QTcpSocket::ConnectedState) {
				emit connectionResult(true);
				connectionTimer.stop();
				i = 0;
			}
		}
		else
			didTryToConnect = false;
	});

//	checkConnectionTimer.setInterval(10000);
//	connect(&checkConnectionTimer, &QTimer::timeout, [this]()
//	{
//		if(!didSendPing)
//			emit checkConnectionResult(false);
//		else
//			didSendPing = false;

//		checkConnectionTimer.stop();
//	});
}

void Client::connectToServer(const QString &ip, int port)
{
	connectionTimer.stop();
	if(lastIp == ip && lastPort == port && socket->state() == QTcpSocket::ConnectedState)
		emit connectionResult(true);
	else {
		didTryToConnect = true;
		socket->disconnectFromHost();
		socket->connectToHost(ip, port);
		lastIp = ip;
		lastPort = port;
		connectionTimer.start();
	}
}

void Client::reconnect()
{
	connectToServer(lastIp, lastPort);
}

//void Client::checkConnection()
//{
//	didSendPing = false;
//	socket->write("ping");

//	checkConnectionTimer.start();
//}

void Client::sendCheckingBoard(QJsonArray shipsJsonData)
{
	QJsonObject jsonObject;
	jsonObject["request"] = "check_ships";
	jsonObject["ships"] = shipsJsonData;
	socket->write(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
}

void Client::receiveData()
{
//	if(socket->readAll() == "pong" && didSendPing) {
//		checkConnectionResult(true);
//		didSendPing = false;
//		checkConnectionTimer.stop();
//	}
}
