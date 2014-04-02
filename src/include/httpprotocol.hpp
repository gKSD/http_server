/*
 * httpprotocol.hpp
 *
 *  Created on: 30.03.2014
 *      Author: sofia
 */

#ifndef HTTPPROTOCOL_HPP_
#define HTTPPROTOCOL_HPP_

#include <map>
#include <string>
#include <vector>

typedef std::map< std::string, std::string> str_to_str_map;

enum { max_length = 1024 , buffer_length = 8192};

enum size_paramters { MAX_LENGTH = 65535 };


const std::string method_GET = "GET";
const std::string method_POST = "POST";
const std::string method_HEAD = "HEAD";


const std::string ok_string = "HTTP/1.0 200 OK\r\n";
const std::string not_found_string = "HTTP/1.0 404 Not Found\r\n";
const std::string method_not_allowed_string = "HTTP/1.0 405 Method Not Allowed";
const std::string internal_server_error_string = "HTTP/1.0 500 Internal Server Error\r\n";



const std::string ok_content = "";
//const std::string not_found_content = "<html> <head></head> <body> <img src='/image.jpg'></body> </html>";
const std::string not_found_content = "<html> <head> <h1>404</h1> </head> <body></body> </html>";
const std::string method_not_allowed_content = "<html> <head><h1>405</h1></head> <body></body> </html>";
const std::string internal_server_error_content = "<html> <head><h1>500</h1></head> <body></body> </html>";


namespace response
{

enum status_type
{
	undefined = 0,
	ok = 200,
	not_found = 404,
	method_not_allowed = 405,
	internal_server_error = 500,
};

//std::string get_status_string (status_type type);
//std::string get_content_string_by_status(status_type type);

struct response
{
	std::string status_string;
	std::vector<std::string> headers;
	std::string body;
	std::string file_extension;
};


} //namespace response


std::string get_content_type(std::string extension);
bool is_valid_header (std::string header);

namespace request
{

struct request
{
	std::string method;
	std::string url;
	std::string protocol;
	str_to_str_map headers;
};

} //namespace request

#endif /* HTTPPROTOCOL_HPP_ */
