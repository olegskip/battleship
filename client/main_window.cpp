#include "main_window.h"

MainWindow::MainWindow(QWidget *parent):
	QWidget(parent)
{
	setObjectName("MainWindow");
	setStyleSheet(QString("%1#%2{background-color: rgb(35, 35, 45);}").arg(metaObject()->className(), objectName()));

	menuWindow = QPointer<MenuWindow>(new MenuWindow());
	connect(menuWindow, &MenuWindow::reconnectRequested, [this](const QString &ip, int port)
	{
		client->connectToServer(ip, port);
	});
	connect(menuWindow, &MenuWindow::checkConnectionRequested, this, [this]()
	{
//		client->checkConnection();
	});

	gameWindow = QPointer<GameWindow>(new GameWindow());

	boxLayout = QPointer<QHBoxLayout>(new QHBoxLayout());
	boxLayout->addWidget(menuWindow);
	setLayout(boxLayout);
	boxLayout->setMargin(0);

	animatedShadow = QPointer<AnimatedShadow>(new AnimatedShadow(this, config::ANIMATED_SHADOW_COLOR,
																 config::ANIMATED_SHADOW_DELAY, 700));
	connect(animatedShadow, &AnimatedShadow::finished, this, [this]()
	{
		menuWindow->deleteLater();
		boxLayout->addWidget(gameWindow);
		animatedShadow->raise();
	});

	connect(menuWindow, &MenuWindow::startGame,  [this](QJsonArray shipsData)
	{
		client->sendCheckingBoard(shipsData);
		animatedShadow->start(size());
	});

	resize(500, 600);

	popUpMessage = QPointer<PopUpMessage>(new PopUpMessage(this, 50));

	client = QPointer<Client>(new Client());
	connect(client, &Client::disconnected, [this]()
	{
		popUpMessage->popUp(config::SERVER_DISCONNECTED, width());
	});
	connect(client, &Client::connectionResult, [this](bool connectResult)
	{
		if(connectResult)
			popUpMessage->popUp(config::SERVER_CONNECTION_SUCCESSFUL, width());
		else
			popUpMessage->popUp(config::SERVER_CONNECTION_FAILED, width());
		popUpMessage->raise();
		menuWindow->gotConnectResult(connectResult);
	});
}
