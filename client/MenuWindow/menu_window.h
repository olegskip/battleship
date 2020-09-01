#ifndef MENU_WINDOW_H
#define MENU_WINDOW_H

#include <QPushButton>
#include <QLineEdit>
#include <QPointer>
#include <QWidget>
#include <QLabel>

#include "animated_shadow.h"
#include "board.h"
//#include "clien


class MenuWindow: public QWidget
{
	Q_OBJECT

public:
	MenuWindow(QWidget *parent = nullptr);

	void gotConnectResult(bool connectState);

signals:
	void startGame(QJsonArray shipsData);
	void checkConnectionRequested();
	void reconnectRequested(const QString &ip, int port);

private:
	QPointer<QPushButton> startButton;
	QPointer<Board> selectionBoard;
	QPointer<QPushButton> randomSelectionBoardButton;

	QPointer<QLineEdit> serverIPEdit;
	QPointer<QLineEdit> serverPortEdit;
	QPointer<QLineEdit> playerName;
	QPointer<QPushButton> connectButton;
//	QPointer<QLineEdit> serverNameEdit;
//	QPointer<QLineEdit> serverPasswordEdit;
};

#endif // MENU_WINDOW_H
