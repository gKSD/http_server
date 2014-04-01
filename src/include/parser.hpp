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

private:

	bool is_valid_url (std::string url);

private:

	request::request _request;
	bool _is_valid_url;
	bool _is_supported_protocol;

	response::response _response;
};


#endif /* PARSER_HPP_ */
