/*
 * client.cpp
 *
 *  Created on: 28.03.2014
 *      Author: sofia
 */

#include "include/client.hpp"
#include <iostream>


Client::Client(const std::string& address, const std::string& port, std::size_t thread_count):
		//_context(boost::asio::ssl::context::sslv23_client),
		//_socket(io_service, _context),
		_acceptor(_io_service),
		_port(port),
		_address(address),
		_thread_count(thread_count)
{
	//_socket.set_verify_mode(boost::asio::ssl::verify_none);

	//boost::asio::ssl::context ctx(io_service, boost::asio::ssl::context::sslv23);
	//ctx.set_verify_mode(boost::asio::ssl::context::verify_peer);
	//ctx.load_verify_file("ca.pem");

	std::cout<< "Constructor"<<std::endl;
	//_socket.lowest_layer().async_connect(endpoint, boost::bind(&Client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator));

	boost::asio::ip::tcp::resolver resolver(_io_service);
	boost::asio::ip::tcp::resolver::query query(address, port);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

	_acceptor.open(endpoint.protocol());
	_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	_acceptor.bind(endpoint);
	_acceptor.listen();


	this->start_accept();
}

Client::~Client()
{
	// TODO Auto-generated destructor stub
}
void Client::handle_connect(const boost::system::error_code& error,
		  boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{}


void Client::start_accept()
{

}



