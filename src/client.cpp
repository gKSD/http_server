/*
 * client.cpp
 *
 *  Created on: 28.03.2014
 *      Author: sofia
 */

#include "include/client.hpp"
#include <iostream>


Client::Client(const std::string& address, const std::string& port, std::size_t thread_count,  const std::string& document_root):
		//_context(boost::asio::ssl::context::sslv23_client),
		//_socket(io_service, _context),
		_acceptor(_io_service),
		_port(port),
		_address(address),
		_thread_count(thread_count),
		_document_root(document_root),
		_processor(_document_root)
		//_strand(), //(_io_service), //connection
		//_socket(), //_socket(_io_service)  //connection
		//_threads(),
		//_parser()
{
	//_socket.reset(new connection(io_service_, request_handler_));

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

void Client::handle_connect(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{

}


void Client::start_accept()
{
	//_socket.basic_socket(_io_service); //??????????
	/*if (!_socket.is_open()) //_socket.open();
	{
		_socket.open
	}*/

	//_socket.reset(new boost::asio::ip::tcp::socket(_io_service));
	//_strand.reset(new boost::asio::io_service::strand(_io_service));

	_socket_connect.reset(new socket_connect(_io_service, _processor));

	_acceptor.async_accept(_socket_connect->socket(), boost::bind(&Client::handle_accept, this, boost::asio::placeholders::error));
}

void Client::handle_accept(const boost::system::error_code& error)
{
	if (!error)
	{
		_socket_connect->start_connection();
	}

	this->start_accept();
}

void Client::handle_stop()
{
	_io_service.stop();
}


void Client::run()
{
	std::vector<boost::shared_ptr<boost::thread> > threads;
	std::cout << "Client::run()" << std::endl;
	for (std::size_t i = 0; i < _thread_count; ++i)
	{
		boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &_io_service)));
		threads.push_back(thread);
	}

	std::cout << "threads.size() = "<< threads.size() << std::endl;

	for (std::size_t i = 0; i < threads.size(); ++i)
		threads[i]->join();
}
