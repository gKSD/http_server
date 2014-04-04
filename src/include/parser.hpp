/*
 * parser.hpp
 *
 *  Created on: 29.03.2014
 *      Author: sofia
 */

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "httpprotocol.hpp"

#include <boost/asio.hpp>

class Parser //httpprocessor
{
public:

	Parser( std::string document_root);
	void reset();

	bool parse(const std::string& request);
	response::status_type make_response();

	std::vector<boost::asio::const_buffer> format_response_to_send_it_to_socket();


private:

	bool is_valid_url (std::string url);

	bool parse_first_line(std::string &str);
	std::string get_valid_url(std::string &str);

	void form_response(response::status_type status);

	std::string get_content_string_by_status(response::status_type type);
	std::string get_status_string (response::status_type type);

private:

	request::request _request;

	bool _f_is_supported_protocol;

	bool _f_has_method;
	bool _f_has_url;
	bool _f_has_protocol;

	bool _f_is_GET_method;
	bool _f_is_POST_method;
	bool _f_is_HEAD_method;

	response::response _response;

	std::string _document_root;
};


#endif /* PARSER_HPP_ */
