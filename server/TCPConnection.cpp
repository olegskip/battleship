#include "TCPConnection.h"

TCPConnection::TCPConnection(boost::asio::ip::tcp::socket socket): 
	ID(boost::uuids::random_generator()()), mSocket(std::move(socket))
{
	std::cout << "new connection, id = " << ID << std::endl;


	listen();
}

void TCPConnection::createRoomRespond(StatusCode statusCode, std::string comment)
{
	nlohmann::json json;
	json["request"] = "create_room";
	json["code"] = static_cast<int>(statusCode);
	if(comment != "")
		json["comment"] = comment;

	send(json);
}

void TCPConnection::send(const std::string &dataToSend)
{
	mSocket.send(boost::asio::buffer(std::to_string(dataToSend.length()) + dataToSend));
}

void TCPConnection::send(const nlohmann::json &jsonData)
{
	if(jsonData["request"] == "")
		throw std::invalid_argument("Json doesn't have request");

	std::cout << "send " << jsonData.dump() << "\tto " << ID << std::endl;
	send(jsonData.dump());
}

bool operator == (const TCPConnection &firstTCPConnection, const TCPConnection &secondTCPConnection)
{
	return firstTCPConnection.ID == secondTCPConnection.ID;
}

void TCPConnection::listen()
{
  	mSocket.async_read_some(boost::asio::buffer(dataBuffer), [this](boost::system::error_code errorCode, std::size_t /* length */) mutable
	{
		if(!errorCode) {
			std::cout << "received = " << dataBuffer << std::endl;

			size_t currentRequestSize = 0, currentRequestStartPos = 0;
			for(int i = 0; dataBuffer[i] != '\0'; ++i) {
				if(isdigit(dataBuffer[i])) {
					if(currentRequestSize == 0)
						currentRequestStartPos = i;

					currentRequestSize = std::stoi(std::to_string(currentRequestSize) + dataBuffer[i]);
				}
				else if(currentRequestSize != 0) {
					dataController(std::string(dataBuffer).substr(std::to_string(currentRequestSize).length() + currentRequestStartPos,
													currentRequestSize));
					i += currentRequestSize - 1;
					currentRequestSize = 0;
				}
			}
		}
		else 
			disconnectSignal();
		listen();
	});
}

void TCPConnection::dataController(std::string data)
{
	try
	{
		const auto json = nlohmann::json::parse(data);
		if(json["request"] == "create_room")
			createRoomRequest(json["room_name"]);
		else if(json["request"] == "join_room")
			joinRoomRequest(json["room_name"]);
		else if(json["request"] == "delete_room")
			deleteRoomRequest(json["room_name"]);
	}
	catch(const std::exception&)
	{
		
	}
}

TCPConnection::~TCPConnection()
{
	boost::system::error_code errorCode;
	mSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, errorCode);
	mSocket.close();
}
