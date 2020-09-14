#include "room.h"

Room::Room(std::string _roomName, std::shared_ptr<TCPConnection> ownerConnection):
	roomName(_roomName)
{
	mCreator = std::weak_ptr<TCPConnection>(std::move(ownerConnection));
	// creator = 
}

std::weak_ptr<TCPConnection> &Room::creator()
{
	return mCreator;
}

bool Room::operator == (const Room &anotherRoom)
{
	return roomName == anotherRoom.roomName;
}

bool Room::operator == (std::string roomName)
{
	return this->roomName == roomName;
}
