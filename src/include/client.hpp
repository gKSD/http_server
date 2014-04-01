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
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/thread.hpp>

#include "parser.hpp"

enum { max_length = 1024 , buffer_length = 8192};

class Client : private boost::noncopyable,  public boost::enable_shared_from_this<Client>
{
public:

	Client(const std::string& address, const std::string& port, std::size_t thread_count,  const std::string& document_root);
	virtual ~Client();

	void run();

private:
	void handle_connect(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

	void start_accept();
	void handle_accept(const boost::system::error_code& error);
	void handle_stop();

	//connection
	boost::asio::ip::tcp::socket& socket();

	void start_connection();
	void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error);

private:

	boost::asio::io_service _io_service;
	//boost::asio::ssl::context _context;
	boost::asio::ip::tcp::acceptor _acceptor; //слушает входящие соединения

	char _buffer [buffer_length];

	std::string _port;
	std::string _address;
	std::size_t _thread_count;
	std::string _document_root;

	//from connection class
	boost::asio::io_service::strand _strand;
	//boost::asio::ip::tcp::socket _socket;
	boost::shared_ptr<boost::asio::ip::tcp::socket> _socket;
	//boost::shared_ptr<boost::asio::io_service::strand> _strand;


	std::vector<boost::shared_ptr<boost::thread> > _threads;

	Parser _parser;
};

#endif /* CLIENT_HPP_ */
