/*
 * parser.hpp
 *
 *  Created on: 29.03.2014
 *      Author: sofia
 */

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "httpprotocol.hpp"

class Parser
{
public:

	Parser();

	bool parse(const std::string& request);

	bool reset();

private:

	bool is_valid_url (std::string url);

	bool parse_first_line(std::string &str);
	std::string get_valid_url(std::string &str);

private:

	request::request _request;

	bool _f_is_valid_url;
	bool _f_is_supported_protocol;
	bool _f_is_supported_method;

	bool _f_has_method;
	bool _f_has_url;
	bool _f_has_protocol;

	response::response _response;
};


#endif /* PARSER_HPP_ */
