#include "TCPServer.h"
#include <iostream>


int main()
{	
	std::cout << "Starting server..." << std::endl;

	boost::asio::io_context io_context;
	TCPServer server(io_context, 8008);
	io_context.run();


	return 0;
}