#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <boost/format.hpp>

#include <list>
#include <iomanip>


#include "room.h"


class TCPServer
{
public:
	explicit TCPServer(boost::asio::io_service& io_context, unsigned int port);

private:
	void listen();

	boost::asio::ip::tcp::acceptor mAcceptor;
	boost::asio::ip::tcp::socket mSocket;
	std::list<std::shared_ptr<TCPConnection>> connections;
	void deleteConnection(std::shared_ptr<TCPConnection> connection);

	std::list<std::shared_ptr<Room>> rooms;
	std::shared_ptr<Room> getRoom(std::string roomName);
	void createRoom(std::shared_ptr<TCPConnection> connection, std::string roomName);
	void joinRoom(std::shared_ptr<TCPConnection> connection, std::string roomName);
	void deleteRoomByConnection(std::shared_ptr<TCPConnection> connection, std::string roomName);

	std::shared_ptr<Room> checkIsOwnerOfRoom(std::shared_ptr<TCPConnection> connection);
	void removeConnectionFromRoom(std::shared_ptr<TCPConnection> connection);

	nlohmann::json getRoomsJsonInfo() const;
	void sendRoomsJsonInfo();
};

#endif // TCPSERVER_H
