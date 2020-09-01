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
	MainWindow(QWidget *parent = nullptr);

private:
	QPointer<MenuWindow> menuWindow;
	QPointer<GameWindow> gameWindow;
	QPointer<QHBoxLayout> boxLayout;
	QPointer<AnimatedShadow> animatedShadow;

	QPointer<PopUpMessage> popUpMessage;

	QPointer<Client> client;
};

#endif // MAIN_WINDOW_H
