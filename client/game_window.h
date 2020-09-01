#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QPointer>
#include <QWidget>
#include <QVector>

#include "Network/client.h"
#include "config.h"
#include "cell.h"


class GameWindow: public QWidget
{
	Q_OBJECT

public:
	GameWindow(QWidget *parent = nullptr);
	~GameWindow();

private:
	QPointer<QPushButton> startButton;
	bool isGameStarted = false;
};

#endif // GAME_WINDOW_H
