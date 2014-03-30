/*
 * httpserver.hpp
 *
 *  Created on: 30.03.2014
 *      Author: sofia
 */

#ifndef HTTPSERVER_HPP_
#define HTTPSERVER_HPP_

#include "client.hpp"

class Http_server : private boost::noncopyable
{
public:
	Http_server();
	Http_server(const std::string& address, const std::string& port, std::size_t thread_count, const std::string& document_root);
	virtual ~Http_server();

	void run();

	boost::shared_ptr<Client> get_client() { return client;}

protected:

	boost::shared_ptr<Client> client;

};

#endif /* HTTPSERVER_HPP_ */
