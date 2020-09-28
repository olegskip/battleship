#include "client.h"
#include "config.h"

#include <QtMath>
#include <QByteArray>

Client::Client()
{
	socket = QPointer<QTcpSocket>(new QTcpSocket());

	connect(socket, &QTcpSocket::readyRead, this, &Client::receiveData);
	connect(socket, &QTcpSocket::disconnected, [this]()
	{
		if(!isTryingToConnect) {
			emit disconnected();
			reconnect();
		}
	});

	connectionTimer.setInterval(500);
	connect(&connectionTimer, &QTimer::timeout, [this]()
	{
		static int attempts;
		if(isTryingToConnect) {
			++attempts;
			// if didn't connected and time is out
			if(socket->state() == QTcpSocket::UnconnectedState) {
				if(attempts == qCeil(config::SERVER_CONNECT_DURATION / connectionTimer.interval())) {
					connectionResult(false);
					connectionTimer.stop();
					attempts = 0;
				}
				else
					reconnect();
			}
			else if(socket->state() == QTcpSocket::ConnectedState) {
				emit connectionResult(true);
				connectionTimer.stop();
				attempts = 0;
				isTryingToConnect = false;
			}
		}
		else
			isTryingToConnect = false;
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
		isTryingToConnect = true;
		socket->disconnectFromHost();
		socket->connectToHost(ip, port);
		lastIp = ip;
		lastPort = port;
		connectionTimer.start();
	}
}

void Client::reconnect()
{
	qDebug() << "reconnect";
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
	sendJson(jsonObject);
}

void Client::sendCreateRoom(QString roomName)
{
	QJsonObject jsonObject;
	jsonObject["request"] = "create_room";
	jsonObject["room_name"] = roomName;
	sendJson(jsonObject);
}

void Client::sendJoinRoom(QString roomName)
{
	QJsonObject jsonObject;
	jsonObject["request"] = "join_room";
	jsonObject["room_name"] = roomName;
	sendJson(jsonObject);
}

void Client::sendDeleteRoom(QString roomName)
{
	QJsonObject jsonObject;
	jsonObject["request"] = "delete_room";
	jsonObject["room_name"] = roomName;
	sendJson(jsonObject);
}

void Client::sendJson(const QJsonObject &jsonObject)
{
	if(jsonObject["request"] == "")
		throw std::invalid_argument("Json object has no request field");

	const auto jsonByteArray = QJsonDocument(jsonObject).toJson(QJsonDocument::Compact);
	qDebug() << "send" << QByteArray::number(jsonByteArray.length()) + jsonByteArray;
	socket->write(QByteArray::number(jsonByteArray.length()) + jsonByteArray);
}

void Client::receiveData()
{
	const QString allReceivedData = socket->readAll();
	qDebug() << "received = " << allReceivedData;

	size_t currentRequestSize = 0, currentRequestStartPos = 0;
	for(int i = 0; i < allReceivedData.size(); ++i) {
		if(allReceivedData[i].isDigit()) {
			if(currentRequestSize == 0)
				currentRequestStartPos = i;

			currentRequestSize = QString(QString::number(currentRequestSize) + allReceivedData[i]).toInt();
		}
		else if(currentRequestSize != 0){
			dataController(allReceivedData.mid(QString::number(currentRequestSize).length() + currentRequestStartPos,
											   currentRequestSize));
			i += currentRequestSize - 1;
			currentRequestSize = 0;
		}
	}
}

void Client::dataController(QString data)
{
	qDebug() << "dataController" << data;
	const auto dataJson = QJsonDocument::fromJson(data.toUtf8());

	if(dataJson["request"] == "create_room")
		emit createRoomRespond(static_cast<StatusCode>(dataJson["code"].toInt()), dataJson["comment"].toString());
	else if(dataJson["request"] == "join_room")
		emit joinRoomRespond(static_cast<StatusCode>(dataJson["code"].toInt()), dataJson["comment"].toString());
	else if(dataJson["request"] == "update_rooms")
		emit updateRooms(dataJson.object());
}
