/*
 * httpprotocol.cpp
 *
 *  Created on: 30.03.2014
 *      Author: sofia
 */

#include "include/httpprotocol.hpp"

//const std::string ok_string = "HTTP/1.0 200 OK\r\n";
//const std::string not_found_string = "HTTP/1.0 404 Not Found\r\n";
//const std::string method_not_allowed_string = "HTTP/1.0 405 Method Not Allowed";
//const std::string internal_server_error_string = "HTTP/1.0 500 Internal Server Error\r\n";

//const std::string ok_content = "";
//const std::string not_found_content = "<html> <head></head> <body> <img src='/image.jpg'></body> </html>";
//const std::string not_found_content = "<html> <head> <h1>404</h1> </head> <body></body> </html>";
//const std::string method_not_allowed_content = "<html> <head><h1>405</h1></head> <body></body> </html>";
//const std::string internal_server_error_content = "<html> <head><h1>500</h1></head> <body></body> </html>";




bool is_valid_header (std::string header)
{
	return (header == "Host" || header == "User-Agent" || header == "Accept" || header == "Accept-Language" ||
			header == "Accept-Encoding" || header == "Cookie" || header == "Connection" || header == "Date");
}

/*struct content_type
{
  const char* extension;
  const char* mime_type;
};*/



//content_type content_type_vector[] = {{ "gif", "image/gif" },{ "htm", "text/html" },{ "html", "text/html" },{ "jpg", "image/jpeg" },{ "png", "image/png" },
//										{ "jpeg", "image/jpeg" },{ "swf", "application/x-shockwave-flash" },{ "js", "script/javascript" },{ "css", "text/css" },{ 0, 0 }};

std::string get_content_type(std::string extension)
{
	if (extension == "gif") return "image/gif";
	if (extension == "htm") return "text/html";
	if (extension == "html") return "text/html";
	if (extension == "jpg") return "image/jpeg";
	if (extension == "png") return "image/png";
	if (extension == "jpeg") return "image/jpeg";
	if (extension == "swf") return "application/x-shockwave-flash";
	if (extension == "js") return "text/javascript";
	if (extension == "css") return "text/css";

	return "application/octet-stream";
}


