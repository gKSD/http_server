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

typedef std::map< std::string, std::string> str_to_str_map;

enum size_paramters { MAX_LENGTH = 65535 };

const char rigth_code_border[3] = "7A";
const char left_code_border[3] = "20";

const std::string method_GET = "GET";
const std::string method_POST = "POST";
const std::string method_HEAD = "HEAD";

namespace response
{

enum status_type
{
	ok = 200,
	not_found = 404,
	method_not_allowed = 405,
	internal_server_error = 500
};

std::string get_status_string (status_type type);


struct response
{
	str_to_str_map headers;
};

} //namespace response


std::string get_content_type(std::string extension);
bool is_valid_header (std::string header);

namespace request
{

struct request
{
	std::string method;
	std::string uri;
	std::string protocol;
	str_to_str_map headers;
};

} //namespace request

#endif /* HTTPPROTOCOL_HPP_ */
