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
		_strand(_io_service), //connection
		_socket(), //_socket(_io_service)  //connection
		_threads(),
		_parser()
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

void Client::handle_connect(const boost::system::error_code& error,
		  boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{

}


void Client::start_accept()
{
	//_socket.basic_socket(_io_service); //??????????
	/*if (!_socket.is_open()) //_socket.open();
	{
		_socket.open
	}*/

	_socket.reset(new boost::asio::ip::tcp::socket(_io_service));

	_acceptor.async_accept(*_socket, boost::bind(&Client::handle_accept, this, boost::asio::placeholders::error));
	std::cout<<"endof start_accept"<<std::endl;
}

void Client::handle_accept(const boost::system::error_code& error)
{
	if (!error)
	{
		std::cout<< "handle accept 1" << std::endl;
		this->start_connection();
	}

	std::cout<< "handle_accept 2" << std::endl;
	this->start_accept();

	std::cout<<"endof handle_accept"<<std::endl;
}

void Client::handle_stop()
{
	_io_service.stop();
	std::cout<<"endof handle_stop"<<std::endl;
}


void Client::run()
{
	  //std::vector<boost::shared_ptr<boost::thread> > threads;
	std::cout << "Client::run()" << std::endl;
	for (std::size_t i = 0; i < _thread_count; ++i)
	{
		boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &_io_service)));
		_threads.push_back(thread);
	}

	std::cout << "_threads.size() = "<< _threads.size() << std::endl;

	for (std::size_t i = 0; i < _threads.size(); ++i)
		_threads[i]->join();
}

//**************************************************************************************************

boost::asio::ip::tcp::socket& Client::socket()
{
	return *_socket;
}

void Client::start_connection()
{
	std::cout<< "start_connection" << std::endl;

	_socket->async_read_some(boost::asio::buffer(_buffer),
			_strand.wrap(
					boost::bind(&Client::handle_read, shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));

	//_socket->async_read_some(boost::asio::buffer(_buffer),_strand.wrap( boost::bind(&Client::handle_read, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
	std::cout<<"endof start_connection"<<std::endl;
}

void Client::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	std::cout<< "handle read 1" << std::endl;
	if (error) return;
	std::cout<< "handle read 2" << std::endl;

	bool result;

	std::cout<<_buffer<<std::endl;

	_parser.parse(_buffer);

	//boost::tribool result;
	//boost::tie(result, boost::tuples::ignore) = request_parser_.parse(request_, buffer_.data(), buffer_.data() + bytes_transferred);

	if (result)
	{
		//request_handler_.handle_request(request_, reply_);
	    //boost::asio::async_write(socket_, reply_.to_buffers(), strand_.wrap( boost::bind(&connection::handle_write, shared_from_this(),boost::asio::placeholders::error)));
	}
	else if (!result)
	{
		//reply_ = reply::stock_reply(reply::bad_request);
	    //boost::asio::async_write(socket_, reply_.to_buffers(),strand_.wrap(boost::bind(&connection::handle_write, shared_from_this(),boost::asio::placeholders::error)));
	}
	else
	{
		//socket_.async_read_some(boost::asio::buffer(buffer_),strand_.wrap(boost::bind(&connection::handle_read, shared_from_this(),boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred)));
	}

	std::cout<<"endof handle_read"<<std::endl;
}

void Client::handle_write(const boost::system::error_code& error)
{
	std::cout<< "hadle write" <<std::endl;
	if (!error)
	{
		std::cout<< "!error" <<std::endl;
		boost::system::error_code code_error;
	    _socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, code_error);
	    //_socket.close();
	}
	std::cout<<"endof handle_write"<<std::endl;
}
