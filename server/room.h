#ifndef ROOM_H
#define ROOM_H

#include <array>
#include <memory>

#include "TCPConnection.h"


class Room
{
public:
	Room(std::string _roomName, std::shared_ptr<TCPConnection> ownerConnection);

	const std::string roomName;

	std::weak_ptr<TCPConnection> &creator();


	bool operator == (const Room &anotherRoom);
	bool operator == (std::string roomName);

private:
	std::weak_ptr<TCPConnection> mCreator;
};

#endif // ROOM_H
