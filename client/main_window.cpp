#include "main_window.h"

MainWindow::MainWindow(QWidget *parent):
	QWidget(parent)
{
	QCoreApplication::setApplicationName(config::WINDOW_NAME);

	setObjectName("WINDOW_NAME");
	setStyleSheet(QString("%1#%2{background-color: rgb(35, 35, 45);}").arg(metaObject()->className(), objectName()));

	menuWindow = QPointer<MenuWindow>(new MenuWindow());
	connect(menuWindow, &MenuWindow::reconnectRequested, [this](const QString &ip, int port)
	{
		client->connectToServer(ip, port);
	});
	connect(menuWindow, &MenuWindow::checkConnectionRequested, [this]()
	{
//		client->checkConnection();
	});
	connect(menuWindow, &MenuWindow::createRoomRequested, this, &MainWindow::createRoom);
	connect(menuWindow, &MenuWindow::joinRoomRequested, this, &MainWindow::joinRoom);

	connect(menuWindow, &MenuWindow::popUpRequested, [this](QString roomName)
	{
		popUpMessage->popUp(roomName);
	});


	gameWindow = QPointer<GameWindow>(new GameWindow());
	connect(gameWindow, &GameWindow::backToMenuSignal, [this]()
	{
		animatedShadow->start(size());
		connect(animatedShadow, &AnimatedShadow::finished, [this]()
		{
			gameWindow->hide();
			menuWindow->show();
			client->sendDeleteRoom(playerRoomName);
			disconnect(animatedShadow, &AnimatedShadow::finished, nullptr, nullptr);
		});
	});
	gameWindow->hide();

	boxLayout = QPointer<QHBoxLayout>(new QHBoxLayout());
	boxLayout->setMargin(0);
	boxLayout->addWidget(menuWindow);
	boxLayout->addWidget(gameWindow);
	setLayout(boxLayout);

	animatedShadow = QPointer<AnimatedShadow>(new AnimatedShadow(this, config::ANIMATED_SHADOW_COLOR,
																 config::ANIMATED_SHADOW_DELAY, 700));

	resize(800, 600);

	popUpMessage = QPointer<PopUpMessage>(new PopUpMessage(this, 70));

	client = QPointer<Client>(new Client());
	connect(client, &Client::disconnected, [this]()
	{
		popUpMessage->popUp("No connection with server, reconnecting...");
		menuWindow->setEnabledByConnection(false);
	});
	connect(client, &Client::connectionResult, [this](bool connectResult)
	{
		if(connectResult)
			popUpMessage->popUp("Connected to the server sucessfully");
		else
			popUpMessage->popUp("Failed to connect to the server");
		popUpMessage->raise();
		menuWindow->gotConnectResult(connectResult);
	});
	connect(client, &Client::updateRooms, [this](QJsonObject jsonObject)
	{
		menuWindow->updateRooms(jsonObject["rooms"].toArray());
	});
}

void MainWindow::createRoom(QString roomName)
{
	client->sendCreateRoom(roomName);
	connect(client, &Client::createRoomRespond, [this, roomName](StatusCode statusCode, QString serverComment)
	{
		const bool createRoomResult = statusCode == StatusCode::OK;

		if(createRoomResult)
			playerRoomName = roomName;

		if(!createRoomResult) {
			QString popUpText = "Room wasn't created.";
			if(!serverComment.isEmpty())
				popUpText += QString("\nServer comment: %1").arg(serverComment);
			popUpMessage->popUp(popUpText);
		}

		menuWindow->enableCreateRoomButton();
		startGame();

		disconnect(client, &Client::createRoomRespond, nullptr, nullptr);
	});
}

void MainWindow::joinRoom(QString roomName)
{
	if(playerRoomName == roomName)
		popUpMessage->popUp("This is your room");
	else {
		client->sendJoinRoom(roomName);
		connect(client, &Client::joinRoomRespond, [this, roomName](StatusCode statusCode)
		{
			if(statusCode == StatusCode::OK) {
				playerRoomName = roomName;
				startGame();
			}

			disconnect(client, &Client::joinRoomRespond, nullptr, nullptr);
		});
	}
}

void MainWindow::startGame()
{
//	client->sendCheckingBoard(menuWindow->shipsJsonData());
	animatedShadow->start(size());
	connect(animatedShadow, &AnimatedShadow::finished, this, [this]()
	{
		menuWindow->hide();
		gameWindow->show(menuWindow->shipsJsonData());
		disconnect(animatedShadow, &AnimatedShadow::finished, nullptr, nullptr);
	});

}
