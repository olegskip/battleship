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
	CONFLICT = 409
};

class TCPConnection
{
public:
	explicit TCPConnection(boost::asio::ip::tcp::socket socket);
	~TCPConnection();
	boost::signals2::signal<void()> disconnectSignal;

	boost::signals2::signal<void(std::string)> createRoomRequest;
	void createRoomRespond(StatusCode statusCode, std::string comment = "");

	boost::signals2::signal<void(std::string)> joinRoomRequest;
	boost::signals2::signal<void(std::string)> deleteRoomRequest;

	boost::signals2::signal<void(std::string)> updateRoomsInfo;

	void send(const std::string &dataToSend);
	void send(const nlohmann::json &jsonData);

	friend bool operator == (const TCPConnection &firstTCPConnection, const TCPConnection &secondTCPConnection);

	const boost::uuids::uuid ID;

private:
	void listen();
	void dataController(std::string data);

	boost::asio::ip::tcp::socket mSocket;
	
	static const unsigned int MAX_LENGHT = 8000;
	char dataBuffer[MAX_LENGHT];
};

#endif // TCPCONNECTION_H
