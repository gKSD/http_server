/*
 * client.hpp
 *
 *  Created on: 28.03.2014
 *      Author: sofia
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

enum { max_length = 1024 };

class Client : private boost::noncopyable
{
public:

	Client(const std::string& address, const std::string& port, std::size_t thread_count);
	virtual ~Client();


private:
	void handle_connect(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

	void start_accept();

private:

	boost::asio::io_service _io_service;
	//boost::asio::ssl::context _context;
	//boost::asio::ssl::stream<boost::asio::ip::tcp::socket> _socket;
	boost::asio::ip::tcp::acceptor _acceptor; //слушает входящие соединения

	char _read_buffer [max_length];
	char _write_buffer [max_length];

	std::string _port;
	std::string _address;

	std::size_t _thread_count;
};

#endif /* CLIENT_HPP_ */
