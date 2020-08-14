#pragma once

#include <QPointer>
#include <QWidget>
#include <QVector>
#include <QDebug>

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
