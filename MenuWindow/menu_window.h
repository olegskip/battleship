#pragma once
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

signals:
	void startGame();
	void checkConnectionRequested();
	void reconnectRequested(const QString &ip, int port);

private:
	QPointer<QPushButton> startButton;
	QPointer<Board> selectionBoard;
	QPointer<QPushButton> randomSelectionBoardButton;

	QPointer<QLineEdit> serverIPEdit;
	QPointer<QLineEdit> serverPortEdit;
	QPointer<QLineEdit> playerName;
	QPointer<QPushButton> checkConnectionButton;
	QString lastIp;
	int lastPort;
//	QPointer<QLineEdit> serverNameEdit;
//	QPointer<QLineEdit> serverPasswordEdit;
};
