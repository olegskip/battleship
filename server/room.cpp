#include "room.h"

Room::Room(std::string _roomName, std::shared_ptr<TCPConnection> ownerConnection):
	roomName(_roomName)
{
	mCreator = std::weak_ptr<TCPConnection>(std::move(ownerConnection));
}

std::weak_ptr<TCPConnection> &Room::creator()
{
	return mCreator;
}

std::weak_ptr<TCPConnection> &Room::player()
{
	return mPlayer;
}

void Room::resetPlayer()
{
	mPlayer.reset();
	playersCount = 1;
}

nlohmann::json Room::getJsonInfo()
{
	nlohmann::json output;
	output["room_name"] = roomName;
	output["players_count"] = 1;

	return output;
}

bool Room::operator == (const Room &anotherRoom)
{
	return roomName == anotherRoom.roomName;
}

bool Room::operator == (std::string roomName)
{
	return this->roomName == roomName;
}

void Room::deleteRoom()
{
	mCreator.reset();
	mPlayer.reset();
}
