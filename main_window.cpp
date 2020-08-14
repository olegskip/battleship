#include "main_window.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent):
	QWidget(parent)
{
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

	animatedShadow = QPointer<AnimatedShadow>(new AnimatedShadow(this, QColor(40, 40, 40), config::ANIMATED_SHADOW_DELAY, 1000, 0, 255));
	connect(animatedShadow, &AnimatedShadow::finished, this, [this]()
	{
		menuWindow->deleteLater();
		boxLayout->addWidget(gameWindow);
		animatedShadow->raise();
		QTimer::singleShot(1000, [this]()
		{
			popUpMessage->popUp("Слава Україні!", width());
		});
	});

	connect(menuWindow, &MenuWindow::startGame,  [this]()
	{
		animatedShadow->start(size());
//		client->ping();
	});

	resize(500, 600);

	popUpMessage = QPointer<PopUpMessage>(new PopUpMessage(this, 50));

	client = QPointer<Client>(new Client());
	connect(client, &Client::disconnected, [this]()
	{
		popUpMessage->popUp(config::SERVER_DISCONNECTED, width());
	});
	connect(client, &Client::connectionResult, [this](bool result)
	{
		if(result)
			popUpMessage->popUp(config::SERVER_CONNECTION_SUCCESSFUL, width());
		else
			popUpMessage->popUp(config::SERVER_CONNECTION_FAILED, width());
		popUpMessage->raise();
	});
}
