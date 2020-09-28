#include "game_window.h"

GameWindow::GameWindow(QWidget *parent):
	QWidget(parent)
{
	setObjectName("GameWindow");
	setStyleSheet(QString("%1#%2{background-color: transparent;}").arg(metaObject()->className(), objectName()));

	backButton = QPointer<QPushButton>(new QPushButton(this));
	backButton->setText("Start!");
	backButton->move(650, 200);
	backButton->resize(100, 50);
	connect(backButton, &QPushButton::clicked, this, [this]()
	{
		emit backToMenuSignal();
	});
}

void GameWindow::show(QJsonArray shipsJsonData)
{
	board = QPointer<Board>(new Board(this));
	board->move(10, 10);
	board->resize(300, 300);
	QWidget::show();
}

GameWindow::~GameWindow()
{
}
