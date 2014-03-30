/*
 * httpserver.cpp
 *
 *  Created on: 30.03.2014
 *      Author: sofia
 */

#include "include/httpserver.hpp"

Http_server::Http_server() {
	// TODO Auto-generated constructor stub

}

Http_server::Http_server(const std::string& address, const std::string& port, std::size_t thread_count,  const std::string& document_root) :
		client()
{
	client.reset(new Client(address,port,thread_count, document_root));
	//client = new Client(address,port,thread_count);
}

Http_server::~Http_server() {
	// TODO Auto-generated destructor stub
}

void Http_server::run ()
{
	client->run();
}

