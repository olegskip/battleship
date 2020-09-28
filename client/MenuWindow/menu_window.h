#ifndef MENU_WINDOW_H
#define MENU_WINDOW_H

#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QPointer>
#include <QWidget>
#include <QLabel>

#include "animated_shadow.h"
#include "board.h"
#include "rooms_table.h"


class MenuWindow: public QWidget
{
	Q_OBJECT

public:
	explicit MenuWindow(QWidget *parent = nullptr);

	void gotConnectResult(bool connectState);

	// set enabled state of objects that depends on server connection state
	void setEnabledByConnection(bool isEnabled);
	void enableCreateRoomButton();

	void updateRooms(QJsonArray rooms);

	QJsonArray shipsJsonData() const;

signals:
	void checkConnectionRequested();
	void reconnectRequested(const QString &ip, int port);
	void createRoomRequested(QString roomName);
	void joinRoomRequested(QString roomName);

	void popUpRequested(QString text);

private:
	QPointer<QPushButton> startButton;
	QPointer<Board> selectionBoard;
	QPointer<QPushButton> randomSelectionBoardButton;

	QPointer<QLineEdit> serverIPEdit;
	QPointer<QLineEdit> serverPortEdit;
	QPointer<QPushButton> connectButton;

	QPointer<QPushButton> createRoomButton;
	void createRoom();

	QPointer<RoomsTable> roomsTable;
};

#endif // MENU_WINDOW_H
