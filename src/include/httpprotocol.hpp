/*
 * httpprotocol.hpp
 *
 *  Created on: 30.03.2014
 *      Author: sofia
 */

#ifndef HTTPPROTOCOL_HPP_
#define HTTPPROTOCOL_HPP_

#include <map>

typedef std::map< std::string, std::string> str_to_str_map;

namespace response
{

enum status_type
{
	ok = 200,
	not_found = 404,
	method_not_allowed = 405,
	internal_server_error = 500
} status;


const std::string ok_string = "HTTP/1.0 200 OK\r\n";
const std::string not_found_string = "HTTP/1.0 404 Not Found\r\n";
const std::string method_not_allowed_string = "HTTP/1.0 405 Method Not Allowed";
const std::string internal_server_error_string = "HTTP/1.0 500 Internal Server Error\r\n";

static std::string get_status_string (status_type type)
{
	switch (type)
	{
	case (ok):
			return ok_string;
	case (not_found):
			return not_found_string;
	case (method_not_allowed):
			return method_not_allowed_string;
	default:
		return internal_server_error_string;
	}
}

str_to_str_map response;

} //namespace reply


struct content_type
{
  const char* extension;
  const char* mime_type;
};

content_type_vector[] =
{
  { "gif", "image/gif" },
  { "htm", "text/html" },
  { "html", "text/html" },
  { "jpg", "image/jpeg" },
  { "png", "image/png" },
  { "jpeg", "image/jpeg" },
  { "swf", "application/x-shockwave-flash" },
  { "js", "script/javascript" },
  { "css", "text/css" },
  { 0, 0 } // Marks end of list.
};

namespace request
{

str_to_str_map request;

} //namespace request

#endif /* HTTPPROTOCOL_HPP_ */
