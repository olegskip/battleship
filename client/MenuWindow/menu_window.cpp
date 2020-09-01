#include "menu_window.h"
#include "config.h"

#include <QRegExpValidator>

MenuWindow::MenuWindow(QWidget *parent):
	QWidget(parent)
{
	setObjectName("MenuWindow");
	setStyleSheet(QString("%1#%2{background-color: transparent;}").arg(metaObject()->className(), objectName()));

	startButton = QPointer<QPushButton>(new QPushButton(this));
	startButton->move(300, 400);
	startButton->setText("Start!");
	startButton->setEnabled(false);
	connect(startButton, &QPushButton::clicked, [this]()
	{
		startGame(selectionBoard->getShipsJsonData());
	});

	selectionBoard = QPointer<Board>(new Board(this, config::CELL_SIDE_SIZE));
	randomSelectionBoardButton = QPointer<QPushButton>(new QPushButton(this));
	randomSelectionBoardButton->resize(config::CELL_SIDE_SIZE * 11, 25);
	randomSelectionBoardButton->move(selectionBoard->x(), config::CELL_SIDE_SIZE * 11 + 10);
	randomSelectionBoardButton->setText("random");
	connect(randomSelectionBoardButton, &QPushButton::clicked, [this]()
	{
		selectionBoard->random();
	});

	serverIPEdit = QPointer<QLineEdit>(new QLineEdit(this));
	serverIPEdit->move(50, 400);
	serverIPEdit->setValidator(new QRegExpValidator(
								   QRegExp("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$")));
	serverIPEdit->setPlaceholderText(config::DEFAULT_SERVER_IP);
	serverIPEdit->setToolTip("Server ip(without port)");

	serverPortEdit = QPointer<QLineEdit>(new QLineEdit(this));
	serverPortEdit->move(50, 430);
	serverPortEdit->setToolTip("From 1024 to 49151. Some ports can be used by your OS or another process");
	serverPortEdit->setPlaceholderText(QString::number(config::DEFAULT_SERVER_PORT));
	serverPortEdit->setValidator(new QRegExpValidator(
									 QRegExp("\\d{,5}")));

	playerName = QPointer<QLineEdit>(new QLineEdit(this));
	playerName->move(50, 460);
	playerName->setPlaceholderText("Your nickname");
	playerName->setToolTip("Only latins charasters, dot and underscore. Max lenght is 20. Must be unique");
	playerName->setValidator(new QRegExpValidator(
									 QRegExp("[a-z, A-Z, 0-9, _, .]{,20}")));

	connectButton = QPointer<QPushButton>(new QPushButton(this));
	connectButton->move(200, 430);
	connectButton->setText("Check");
	connect(connectButton, &QPushButton::clicked, [this]()
	{
		const QString ip = serverIPEdit->text().isEmpty() ? serverIPEdit->placeholderText() : serverIPEdit->text();
		const int port = serverPortEdit->text().isEmpty() ? serverPortEdit->placeholderText().toInt() : serverPortEdit->text().toInt();
		connectButton->setEnabled(false);
		startButton->setEnabled(false);
		emit reconnectRequested(ip, port);
	});
}

void MenuWindow::gotConnectResult(bool connectState)
{
	connectButton->setEnabled(true);
	startButton->setEnabled(connectState);
}
