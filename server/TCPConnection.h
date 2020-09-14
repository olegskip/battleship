#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <boost/asio.hpp>
#include <boost/signals2/signal.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <nlohmann/json.hpp>

#include <iostream>
#include <memory>
#include <string>


enum class StatusCode
{
	OK = 200,

	BAD_REQUEST = 400,
	FORBIDDEN = 403,
	CONFLICT = 409
};

class TCPConnection: public std::enable_shared_from_this<TCPConnection>
{
public:
	TCPConnection(boost::asio::ip::tcp::socket socket);

	boost::signals2::signal<void(std::string)> createRoomRequest;
	void createRoomRespond(StatusCode statusCode, std::string comment = "");

	friend bool operator == (const TCPConnection &firstTCPConnection, const TCPConnection &secondTCPConnection);

private:
	void listen();
	void dataController(std::string data);
	void send(const std::string &dataToSend);

	boost::asio::ip::tcp::socket mSocket;
	
	static const unsigned int MAX_LENGHT = 8000;
	char dataBuffer[MAX_LENGHT];

	const boost::uuids::uuid ID;
};

#endif // TCPCONNECTION_H
