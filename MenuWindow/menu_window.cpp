#include "menu_window.h"

#include <QRegExpValidator>

MenuWindow::MenuWindow(QWidget *parent):
	QWidget(parent)
{
	setObjectName("MenuWindow");
	startButton = QPointer<QPushButton>(new QPushButton(this));
	startButton->move(300, 400);
	startButton->setText("Start!");
	connect(startButton, &QPushButton::clicked, this, &MenuWindow::startGame);

	selectionBoard = QPointer<Board>(new Board(this, 30));
	randomSelectionBoardButton = QPointer<QPushButton>(new QPushButton(this));
	randomSelectionBoardButton->resize(70, 20);
	randomSelectionBoardButton->move(100, 550);
	randomSelectionBoardButton->setText("random");
	connect(randomSelectionBoardButton, &QPushButton::clicked, [this]()
	{
		selectionBoard->random();
	});

	serverIPEdit = QPointer<QLineEdit>(new QLineEdit(this));
	serverIPEdit->move(50, 400);
	serverIPEdit->setValidator(new QRegExpValidator(
								   QRegExp("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$")));
	serverIPEdit->setPlaceholderText("127.0.0.1");
	serverIPEdit->setToolTip("Server ip(without port)");

	serverPortEdit = QPointer<QLineEdit>(new QLineEdit(this));
	serverPortEdit->move(50, 430);
	serverPortEdit->setToolTip("From 1024 to 49151. Some ports can be used by your OS or another process");
	serverPortEdit->setPlaceholderText("8080");
	serverPortEdit->setValidator(new QRegExpValidator(
									 QRegExp("\\d{,5}")));

	playerName = QPointer<QLineEdit>(new QLineEdit(this));
	playerName->move(50, 460);
	playerName->setPlaceholderText("Your nickname");
	playerName->setToolTip("Only latins charasters, dot and underscore. Max lenght is 20. Must be unique");
	playerName->setValidator(new QRegExpValidator(
									 QRegExp("[a-z, A-Z, 0-9, _, .]{,20}")));

	checkConnectionButton = QPointer<QPushButton>(new QPushButton(this));
	checkConnectionButton->move(200, 430);
	checkConnectionButton->setText("Check");
	connect(checkConnectionButton, &QPushButton::clicked, [this]()
	{
		const QString ip = serverIPEdit->text().isEmpty() ? serverIPEdit->placeholderText() : serverIPEdit->text();
		const int port = serverPortEdit->text().isEmpty() ? serverPortEdit->placeholderText().toInt() : serverPortEdit->text().toInt();
		if(ip != lastIp || port != lastPort) {
			lastIp = ip;
			lastPort = port;
			emit reconnectRequested(ip, port);
		}
	});
}
