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
			connections.back()->createRoomRequest.connect(boost::bind(&TCPServer::createRoom, this, connections.back(), _1));
		}

		listen();
	});
}

void TCPServer::createRoom(std::shared_ptr<TCPConnection> connection, std::string roomName)
{
	auto findResult = std::find_if(rooms.begin(), rooms.end(), [&roomName](const std::shared_ptr<Room> &room)
	{
		return *room == roomName;
	});
	
	if(findResult == rooms.end()) {
		std::shared_ptr<Room> previousUserRoom = checkIsUserInRoom(connection);
		const std::string comment = previousUserRoom ? (boost::format("You entered a new room. "
					"If your previous room %1% is empty, then it will be deleted.") % std::quoted(previousUserRoom->roomName)).str() : "";

		std::cout << comment << std::endl;

		rooms.push_back(std::make_shared<Room>(roomName, connection));

		connection->createRoomRespond(StatusCode::OK, comment);
	}
	else
		connection->createRoomRespond(StatusCode::CONFLICT, "Room name is already taken");
}

std::shared_ptr<Room> TCPServer::checkIsUserInRoom(std::shared_ptr<TCPConnection> connection)
{
	for(auto room: rooms) {
		if(*room->creator().lock() == *connection)
			return room;
	}

	return std::shared_ptr<Room>();
}
