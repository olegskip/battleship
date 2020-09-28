#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QPointer>
#include <QWidget>
#include <QVector>

#include "Network/client.h"
#include "config.h"
#include "cell.h"
#include "board.h"


class GameWindow: public QWidget
{
	Q_OBJECT

public:
	explicit GameWindow(QWidget *parent = nullptr);
	~GameWindow();

	void show(QJsonArray shipsJsonData);

signals:
	void backToMenuSignal();

private:
	using QWidget::show;

	QPointer<Board> board;

	QPointer<QPushButton> backButton;
};

#endif // GAME_WINDOW_H
