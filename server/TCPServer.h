#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <boost/format.hpp>

#include <list>
#include <iomanip>


#include "room.h"


class TCPServer
{
public:
	TCPServer(boost::asio::io_service& io_context, unsigned int port);

private:
	void listen();

	boost::asio::ip::tcp::acceptor mAcceptor;
	boost::asio::ip::tcp::socket mSocket;
	std::list<std::shared_ptr<TCPConnection>> connections;

	std::list<std::shared_ptr<Room>> rooms;
	void createRoom(std::shared_ptr<TCPConnection> connection, std::string roomName);
	std::shared_ptr<Room> checkIsUserInRoom(std::shared_ptr<TCPConnection> connection);

	// std::vector<std::shared_ptr<TCPConnection>> connections;

};

#endif // TCPSERVER_H
