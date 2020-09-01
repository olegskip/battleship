#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QColor>


namespace config
{
	const int CELL_SIDE_SIZE = 30;
	const int CELL_BORDER_WIDTH = 2;
	const QColor CELL_BORDER_COLOR = QColor(220, 230, 230);

	const int POP_UP_MESSAGE_SHOW_DURATION = 2000; // in ms

	const int ANIMATED_SHADOW_DELAY = 250; // in ms
	const QColor ANIMATED_SHADOW_COLOR = QColor(200, 200, 200);

	const int SERVER_CONNECT_DURATION = 5000; // in ms

	const QString SERVER_CONNECTION_SUCCESSFUL = "Connected to the server sucessfully";
	const QString SERVER_CONNECTION_FAILED = "Failed to connect to the server";
	const QString SERVER_DISCONNECTED = "No connection with server, reconnecting...";

	const QString DEFAULT_SERVER_IP = "127.0.0.1";
	const int DEFAULT_SERVER_PORT = 8008;
}

#endif // CONFIG_H
