#include "TCPServer.h"

TCPServer::TCPServer(boost::asio::io_context &io_context, unsigned int port): 
	mAcceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), mSocket(io_context)
{
	listen();
}

void TCPServer::listen()
{
	mAcceptor.async_accept(mSocket, [this](boost::system::error_code errorCode)
	{
		if(!errorCode) {
			connections.push_back(std::make_shared<TCPConnection>(std::move(mSocket)));
			std::cout << "connections count = " << connections.size() << std::endl;
			connections.back()->disconnectSignal.connect(std::bind(&TCPServer::deleteConnection, this, connections.back()));
			connections.back()->createRoomRequest.connect(std::bind(&TCPServer::createRoom, this, connections.back(), std::placeholders::_1));
			connections.back()->createRoomRequest.connect(std::bind(&TCPServer::joinRoom, this, connections.back(), std::placeholders::_1));
			connections.back()->deleteRoomRequest.connect(std::bind(&TCPServer::deleteRoomByConnection, this, connections.back(),  std::placeholders::_1));

			if(rooms.size() > 0)
				connections.back()->send(getRoomsJsonInfo());
		}

		listen();
	});
}

void TCPServer::deleteConnection(std::shared_ptr<TCPConnection> connection)
{
	bool wasRoomDeleted = false;

	// remove player room
	std::shared_ptr<Room> room = checkIsOwnerOfRoom(connection);
	if(room) {
		rooms.remove(room);
		wasRoomDeleted = true;
	}

	// remove disconnected connection
	connections.remove(connection);

	if(wasRoomDeleted)
		sendRoomsJsonInfo();
}

void TCPServer::createRoom(std::shared_ptr<TCPConnection> connection, std::string roomName)
{
	auto findResult = std::find_if(rooms.begin(), rooms.end(), [&roomName](const std::shared_ptr<Room> &room)
	{
		return *room == roomName;
	});

	if(findResult == rooms.end()) {
		std::shared_ptr<Room> previousUserRoom = checkIsOwnerOfRoom(connection);

		rooms.remove(previousUserRoom);

		const std::string comment = previousUserRoom ? (boost::format("You entered a new room. "
					"If your previous room %1% is empty, then it will be deleted.") % std::quoted(previousUserRoom->roomName)).str() : "";

		rooms.push_back(std::make_shared<Room>(roomName, connection));

		connection->createRoomRespond(StatusCode::OK, comment);
		sendRoomsJsonInfo();
	}
	else
		connection->createRoomRespond(StatusCode::CONFLICT, "Room name is already taken");
}

void TCPServer::joinRoom(std::shared_ptr<TCPConnection> connection, std::string roomName)
{
	removeConnectionFromRoom(connection);
}

void TCPServer::deleteRoomByConnection(std::shared_ptr<TCPConnection> connection, std::string roomName)
{
	for(auto room: rooms) {
		if(room->roomName == roomName && *room->creator().lock() == *connection) {
			rooms.remove(room);
			sendRoomsJsonInfo();
			break;
		}
	}
}

std::shared_ptr<Room> TCPServer::checkIsOwnerOfRoom(std::shared_ptr<TCPConnection> connection)
{
	for(auto room: rooms) {
		if(*room->creator().lock() == *connection)
			return room;
	}

	return std::shared_ptr<Room>();
}

void TCPServer::removeConnectionFromRoom(std::shared_ptr<TCPConnection> connection)
{
	bool wereRoomDeleted = false;
	for(auto &room: rooms) {
		// if the user owns of a room - then delete that room and send rooms updating
		if(*room->creator().lock() == *connection) {
			wereRoomDeleted = true;
			room->deleteRoom();
		}
		else if(*room->player().lock() == *connection)
			room->resetPlayer();

	}
	if(wereRoomDeleted)
		sendRoomsJsonInfo();
}

nlohmann::json TCPServer::getRoomsJsonInfo() const
{
	nlohmann::json roomsJsonInfo;
	roomsJsonInfo["request"] = "update_rooms";
	roomsJsonInfo["rooms"] = nlohmann::json::array();

	for(auto room: rooms)
		roomsJsonInfo["rooms"].push_back(room->getJsonInfo());

	return roomsJsonInfo;
}

void TCPServer::sendRoomsJsonInfo()
{
	const nlohmann::json roomsJsonInfo = getRoomsJsonInfo();

	for(auto connection: connections)
		connection->send(roomsJsonInfo);
}
