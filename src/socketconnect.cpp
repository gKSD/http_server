/*
 * socketconnect.cpp
 *
 *  Created on: 02.04.2014
 *      Author: sofia
 */

#include "include/socketconnect.hpp"

#include <vector>
#include <boost/bind.hpp>

socket_connect::socket_connect(boost::asio::io_service& io_service)
	:_strand(io_service),
	 _socket(io_service),
	 _parser()
{
	_parser->reset();
}

socket_connect::~socket_connect()
{

}


void socket_connect::start_connection()
{
	_socket.async_read_some(boost::asio::buffer(_buffer),
	_strand.wrap(boost::bind(&socket_connect::handle_read, shared_from_this(),boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred)));
}

void socket_connect::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	std::cout<< "handle read 1" << std::endl;
	if (error) return;

	std::cout<< "handle read 2" << std::endl;
	std::cout<<_buffer<<std::endl;

	_parser.parse(_buffer);
}

void socket_connect::handle_write(const boost::system::error_code& error)
{
  if (!error)
  {
    boost::system::error_code ignored_ec;
    _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }
}
