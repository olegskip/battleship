#ifndef ROOMSTABLE_H
#define ROOMSTABLE_H

#include <QTableWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <QObject>


class RoomsTable: public QTableWidget
{
	Q_OBJECT

public:
	explicit RoomsTable(QWidget *parent = nullptr);

	void updateRooms(QJsonArray rooms);

signals:
	void joinRoomRequested(QString roomName);
};

#endif // ROOMSTABLE_H
