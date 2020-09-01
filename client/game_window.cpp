#include "game_window.h"

GameWindow::GameWindow(QWidget *parent):
	QWidget(parent)
{
	setObjectName("GameWindow");
	setStyleSheet(QString("%1#%2{background-color: transparent;}").arg(metaObject()->className(), objectName()));

	startButton = QPointer<QPushButton>(new QPushButton(this));
	startButton->setText("Start!");
	startButton->move(650, 200);
	startButton->resize(100, 50);
	connect(startButton, &QPushButton::clicked, this, [this]()
	{
		if(!isGameStarted) {
			isGameStarted = true;
			startButton->setEnabled(!isGameStarted);
		}
	});

	resize(800, 650);
	setMaximumSize(size());
	setMaximumSize(size());
}

GameWindow::~GameWindow()
{

}
