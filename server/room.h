#ifndef ROOM_H
#define ROOM_H

#include <array>
#include <memory>

#include "TCPConnection.h"


class Room
{
public:
	explicit Room(std::string _roomName, std::shared_ptr<TCPConnection> ownerConnection);

	const std::string roomName;

	std::weak_ptr<TCPConnection> &creator();
	std::weak_ptr<TCPConnection> &player();
	void resetPlayer();

	nlohmann::json getJsonInfo();

	bool operator == (const Room &anotherRoom);
	bool operator == (std::string roomName);

	void deleteRoom();

private:
	std::weak_ptr<TCPConnection> mCreator, mPlayer;
	int playersCount = 1;
};

#endif // ROOM_H
