/*
 * socketconnect.h
 *
 *  Created on: 02.04.2014
 *      Author: sofia
 */

#ifndef SOCKETCONNECT_H_
#define SOCKETCONNECT_H_

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "parser.hpp"


class socket_connect: public boost::enable_shared_from_this<socket_connect>, private boost::noncopyable
{
public:
	socket_connect(boost::asio::io_service& io_service, Parser &parser);
	virtual ~socket_connect();

	boost::asio::ip::tcp::socket& socket()
	{
	  return _socket;
	}

	void start_connection();

private:

	void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error);

	boost::asio::io_service::strand _strand;
	boost::asio::ip::tcp::socket _socket;

	char _buffer [buffer_length];
	Parser _parser;

};

#endif /* SOCKETCONNECT_H_ */
