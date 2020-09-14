#include "TCPConnection.h"


TCPConnection::TCPConnection(boost::asio::ip::tcp::socket socket): 
	mSocket(std::move(socket)), ID(boost::uuids::random_generator()())
{
	std::cout << "new connection" << std::endl;

	listen();
}

void TCPConnection::createRoomRespond(StatusCode statusCode, std::string comment)
{
	nlohmann::json json;
	json["request"] = "create_room";
	json["code"] = static_cast<int>(statusCode);
	json["comment"] = comment;

	send(json.dump());
}

bool operator == (const TCPConnection &firstTCPConnection, const TCPConnection &secondTCPConnection)
{
	return firstTCPConnection.ID == secondTCPConnection.ID;
}

void TCPConnection::listen()
{
	// auto self(shared_from_this());
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
	}
	catch(const std::exception&)
	{
		
	}
}

void TCPConnection::send(const std::string &dataToSend)
{
	mSocket.send(boost::asio::buffer(std::to_string(dataToSend.length()) + dataToSend));
	// mSocket.send("test")
}
