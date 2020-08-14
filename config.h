#pragma once
#include <QString>


namespace config
{
	const int CELL_SIDE_SIZE = 50;

	const int POP_UP_MESSAGE_SHOW_DURATION = 2000; // in ms

	const int ANIMATED_SHADOW_DELAY = 250; // in ms

	const int SERVER_CONNECT_DURATION = 5000; // in ms

	const QString SERVER_CONNECTION_SUCCESSFUL = "Connected to the server sucessfully";
	const QString SERVER_CONNECTION_FAILED = "Failed to connect to the server";
	const QString SERVER_DISCONNECTED = "No connection with server, reconnecting...";
}
