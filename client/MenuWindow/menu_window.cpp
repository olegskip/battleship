#include "menu_window.h"
#include "config.h"

#include <QRegExpValidator>
#include <QRegularExpression>
#include <QInputDialog>

MenuWindow::MenuWindow(QWidget *parent):
	QWidget(parent)
{
	setObjectName("MenuWindow");
	setStyleSheet(QString("%1#%2{background-color: transparent; border: 10px solid green;}").arg(metaObject()->className(), objectName()));

	startButton = QPointer<QPushButton>(new QPushButton(this));
	startButton->move(300, 400);
	startButton->setText("Start!");

	selectionBoard = QPointer<Board>(new Board(this));

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

	connectButton = QPointer<QPushButton>(new QPushButton(this));
	connectButton->move(200, 400);
	connectButton->setText("Connect");
	connect(connectButton, &QPushButton::clicked, [this]()
	{
		const QString ip = serverIPEdit->text().isEmpty() ? serverIPEdit->placeholderText() : serverIPEdit->text();
		const int port = serverPortEdit->text().isEmpty() ? serverPortEdit->placeholderText().toInt() : serverPortEdit->text().toInt();
		connectButton->setEnabled(false);
		setEnabledByConnection(false);
		emit reconnectRequested(ip, port);
	});

	createRoomButton = QPointer<QPushButton>(new QPushButton(this));
	createRoomButton->move(200, 430);
	createRoomButton->setText("Create room");

	connect(createRoomButton, &QPushButton::clicked, this, &MenuWindow::createRoom);

	roomsTable = QPointer<RoomsTable>(new RoomsTable(this));
	roomsTable->move(450, 300);
	connect(roomsTable, &RoomsTable::joinRoomRequested, this, [this](QString roomName)
	{
		emit joinRoomRequested(roomName);
	});

	setEnabledByConnection(false);
	// startGame(selectionBoard->getShipsJsonData());
}

void MenuWindow::gotConnectResult(bool connectState)
{
	connectButton->setEnabled(true);
	startButton->setEnabled(connectState);
	createRoomButton->setEnabled(connectState);
}

void MenuWindow::setEnabledByConnection(bool isEnabled)
{
	startButton->setEnabled(isEnabled);
	createRoomButton->setEnabled(isEnabled);
}

void MenuWindow::enableCreateRoomButton()
{
	createRoomButton->setEnabled(true);
}

void MenuWindow::updateRooms(QJsonArray rooms)
{
	roomsTable->updateRooms(rooms);
}

QJsonArray MenuWindow::shipsJsonData() const
{
	return selectionBoard->getShipsJsonData();
}

void MenuWindow::createRoom()
{
	bool inputDialogResult;
	const QString text = QInputDialog::getText(this, config::WINDOW_SHORT_NAME,
											   "Room name. Only letters, digits, dot and underscore. Lenght must be [6, 20]",
											   QLineEdit::Normal, "", &inputDialogResult);
	const auto regExp = QRegularExpression("[a-z, A-Z, 0-9, _, .]{6,20}").match(text);

	if(inputDialogResult) {
		if(!text.isEmpty() && regExp.hasMatch() && regExp.capturedLength() == text.length()) {
			createRoomButton->setEnabled(false);
			emit createRoomRequested(text);
		}

		else
			popUpRequested("Incorrect room name");
	}
}
