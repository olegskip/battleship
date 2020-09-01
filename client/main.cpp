#include "MenuWindow/menu_window.h"
#include "main_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow mainWindow;
	mainWindow.show();

	return a.exec();
}
