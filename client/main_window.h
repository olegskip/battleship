#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QHBoxLayout>
#include <QWidget>

#include "MenuWindow/menu_window.h"
#include "pop_up_message.h"
#include "game_window.h"


class MainWindow: public QWidget
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);


private:
	QPointer<QHBoxLayout> boxLayout;
	QPointer<MenuWindow> menuWindow;
	QPointer<GameWindow> gameWindow;
	QPointer<AnimatedShadow> animatedShadow;

	QPointer<PopUpMessage> popUpMessage;

	QPointer<Client> client;

	void createRoom(QString roomName);
	void joinRoom(QString roomName);
	QString playerRoomName = "";

	void startGame();
};

#endif // MAIN_WINDOW_H
