/*
 * parser.cpp
 *
 *  Created on: 29.03.2014
 *      Author: sofia
 */

#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <boost/regex.hpp>

#include <boost/lexical_cast.hpp>


#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
//#include <string>

#include "include/parser.hpp"
#include "include/httpprotocol.hpp"


Parser::Parser(std::string document_root) :
		_request(),
		_f_is_supported_protocol(true),
		_f_has_method(true),
		_f_has_url(true),
		_f_has_protocol(true),
		_f_is_GET_method(false),
		_f_is_POST_method(false),
		_f_is_HEAD_method(false),
		_document_root(document_root)
{}

void Parser::reset()
{
	_f_is_supported_protocol = true;
	_f_has_method = true;
	_f_has_url = true;
	_f_has_protocol = true;

	_f_is_GET_method = false;
	_f_is_POST_method = false;
	_f_is_HEAD_method = false;

	_request.headers.clear();
	_request.url.clear();
	_request.method.clear();
	_request.protocol.clear();
	_response.file_extension.clear();

	_response.headers.clear();
	_response.body.clear();
}

bool Parser::parse(const std::string& request)
{
	//std::cout<< "Parser \n "<< request << std::endl;

	std::stringstream stream_request(request);

	std::string line;
	//1) парсим метод, урл и протокол
	//остальные заголовки ниже отдельно
	if (getline(stream_request, line)) parse_first_line(line);
	else return false;

	//парсим остальные заголовки
	while (getline(stream_request, line))
	{
		std::vector<std::string> tokens;
		boost::split(tokens, line, boost::is_any_of(":"));
		std::vector<std::string>::iterator it = tokens.begin();
		if (is_valid_header(*it))
		{
			std::string header = *it;
			std::string value;

			it++;
			value.append(*it);

			for(it++; it != tokens.end(); it++)
			{
				value.push_back(':');
				value.append(*it);
			}

			_request.headers.insert(std::pair<std::string,std::string>(header, value));
		}
	}

	return true;
}

bool Parser::is_valid_url(std::string url)
{
	return true;
}

bool Parser::parse_first_line(std::string &str)
{
	std::vector<std::string> tokens;
	boost::split(tokens, str, boost::is_any_of(" \t\n\r"));
	std::vector<std::string>::iterator it = tokens.begin();

	if (tokens.size() == 0)
	{
		_request.url = "";
		_request.protocol = "";
		_request.method = "";

		_f_is_supported_protocol = false;

		_f_has_method = false;
		_f_has_url = false;
		_f_has_protocol = false;
		return false;
	}

	if (!((*it).compare(method_GET) == 0 || (*it).compare(method_POST) == 0 || (*it).compare(method_HEAD) == 0 )) _f_has_method = false;
	if ((*it).compare(method_GET) == 0)
	{
		_f_is_GET_method = true;
	}
	else if ((*it).compare(method_POST) == 0)
	{
		_f_is_POST_method = true;
	}
	else if ((*it).compare(method_HEAD) == 0)
	{
		_f_is_HEAD_method = true;
	}

	_request.method = *it;

	it++;
	if (it != tokens.end())
	{
		//std::cout<< "2: "<< *it<<std::endl;
		//boost::regex regex("(\/[\w\s\.]+)+\/?");
		//boost::regex regex("/[0-9a-zA-Z_%\./-]*");
		boost::regex regex("/[0-9a-zA-Z_%\./\!\"#\&\'\*\,\:\;\<\=\>\?\[\|\^\`\{\|\}-]*");
		//boost::regex regex("/[\w%\/]*");
		boost::smatch match_result;
		if (boost::regex_match(*it,  match_result, regex))
		{
			_request.url = get_valid_url(*it);

			if (_request.url == "")
				_f_has_url = false;
			else
			{
				_f_has_url = true;
				if (_request.url[_request.url.size() - 1] == '/')
					_request.url.append("index.html");

				//получаем расширение запрашиваемого файла
				size_t dot_pos = _request.url.rfind('.');
				size_t slash_pos = _request.url.rfind('/');
				if (dot_pos > slash_pos)
				{
					for (int j = dot_pos + 1; j < _request.url.length(); j++ )
						_response.file_extension.push_back(_request.url[j]);
				}
			}
		}
		else
		{
			_f_has_url = false;
			_request.url = "";
		}
	}
	else
	{
		_request.url = "";
		_request.protocol = "";
		_f_has_url = false;
		_f_has_protocol = false;
		return false;
	}

	it++;
	if (it != tokens.end())
	{
		boost::regex regex("HTTP/?1\.[0|1]");
		boost::smatch match_result;
		if (boost::regex_match(*it,  match_result, regex))
		{
			_request.protocol = *it;
			_f_has_protocol = true;
		}
		else
		{
			_request.protocol = "";
			_f_has_protocol = false;
		}
	}
	else
	{
		_request.protocol = "";
		_f_has_protocol = false;
		return false;
	}

	return true;
}

std::string Parser::get_valid_url(std::string &str)
{
	//std::string result;

	const char *ptr = str.c_str();
	const char *ptr_end =  ptr + str.length();

	char url[MAX_LENGTH] = {0};
	char *tmp = url;
	//char decode;
	char code[3];//, decode;

	int slash_counter = -1; //первый слеш мы не считаем /qq/ww/1.jpg

	memset(url, 0, MAX_LENGTH);

	while(ptr != ptr_end)
	{
		if (*ptr == '%')
		{
			if ((ptrdiff_t)(ptr_end - ptr) > 2)
			{
				//std::cout << "parse encode"<<std::endl;
				code[0] = *(++ptr);
				code[1] = *(++ptr);
				code[2] = '\0';
				long lcode = strtol(code, 0, 16);
				if ((lcode >= 0x21 &&  lcode <= 0x23) || lcode == 0x26 || lcode == 0x27 || lcode == 0x2a || lcode == 0x3b || lcode == 0x3c || lcode == 0x3e ||
						lcode == 0x3f || lcode == 0x60 || lcode == 0x7c)
				return "/";

				*tmp = (char) lcode; //ПРОВЕРИТЬ
				tmp++;
				ptr++;
				continue;
			}
		}
		else  if (*ptr == '?') break;
		else if (*ptr == '/')
		{
			slash_counter++;
			while (ptr != ptr_end && *ptr == '/') ptr++;

			if ((ptrdiff_t)(ptr_end - ptr) > 2 && *ptr == '.')
			{
				const char *cursor = ptr;
				int nesting_counter = 0;
				bool flag = true;

				while ( (ptrdiff_t)(ptr_end - cursor) > 2 && strncmp (cursor, "../", 3) == 0 && flag)
				{
					nesting_counter++;
					if (nesting_counter > slash_counter)
						return "";
					cursor += 3;
				}

				if (flag) break;

				while( ptr != cursor)
				{
					*tmp = *ptr;
					tmp++;
					ptr++;
				}
				continue;
			}

			*tmp = '/';
			tmp++;
			continue;
		}
		else if (*ptr == '!' || *ptr == ';' || *ptr == '\"' || *ptr == '#' || *ptr == '&' || *ptr == '\'' || *ptr == '*' ||
						*ptr == '<' || *ptr == '>' || *ptr == '?' || *ptr == '`' || *ptr == '|')
			return "/";

		*tmp = *ptr;
		tmp++;
		ptr++;
	}

	//result = url;
	return url;
}

response::status_type Parser::make_response()
{
	//std::cout << "Parser make response" <<std::endl;
	if (_f_has_method)
	{
		if (_f_is_GET_method || _f_is_HEAD_method)
		{
			if (!_f_has_protocol)
			{
				//400
				form_response(response::bad_request);
				return response::bad_request;
			}
			if(!_f_has_url)
			{
				//400
				form_response(response::bad_request);
				return response::bad_request;
			}

			std::string path_to_file = _document_root;
			path_to_file.append(_request.url);

			std::ifstream is(path_to_file.c_str(), std::ios::in | std::ios::binary);
			if (!is)
			{
				if (_request.url.find("index.html") != std::string::npos)
				{
					//403
					form_response(response::forbidden);
					return response::forbidden;
				}
				else
				{
					//404
					form_response(response::not_found);
					return response::not_found;
				}
			}

			char buffer[buffer_length];
			_response.body.clear();
			_response.body = "";
			while (is.read(buffer, sizeof(buffer)).gcount() > 0)
				_response.body.append(buffer, is.gcount());

			//формируем заголовки. Тело ответа уже сформировано.
			//200
			form_response(response::ok);
			return response::ok;
		}
		else if (_f_is_POST_method)
		{
			//405
			form_response(response::method_not_allowed);
			return response::method_not_allowed;
		}
		else
		{
			//405
			form_response(response::method_not_allowed);
			return response::method_not_allowed;
		}
	}
	else
	{
		//400
		form_response(response::bad_request);
		return response::bad_request;
	}
}

void Parser::form_response(response::status_type status)
{
	if (status == response::not_found || status == response::internal_server_error || status == response::method_not_allowed ||
			status == response::forbidden || status == response::not_found)
	{
		_response.file_extension = "html";
		_response.body = get_content_string_by_status(status);
	}

	_response.status_string.clear();
	_response.status_string = get_status_string(status);

	//char body_size [50];
	//sprintf(body_size, "%ld", _response.body.size());

	_response.headers.resize(8);
	//_response.headers.push_back("Date");
	//_response.headers.push_back("123123");

	_response.headers[0] = "Server";
	_response.headers[1] = "http_server";

	_response.headers[2] = "Content-Length";
	_response.headers[3] = boost::lexical_cast<std::string>(_response.body.size());//body_size;

	_response.headers[4] = "Content-Type";
	_response.headers[5] = get_content_type(_response.file_extension);

	_response.headers[6] = "Connection";
	_response.headers[7] = "close";

	/*
	 *  HTTP/1.1 200 OK
Server: nginx
Date: Wed, 02 Apr 2014 17:23:54 GMT
Content-Type: text/html; charset=utf-8
Transfer-Encoding: chunked
Connection: keep-alive
X-Powered-By: LiveStreet CMS
Expires: Thu, 19 Nov 1981 08:52:00 GMT
Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0
Pragma: no-cache
	 * */
}

std::string Parser::get_content_string_by_status(response::status_type type)
{
	switch (type)
	{
	case (response::ok):
			return ok_content;
	case (response::not_found):
			return not_found_content;
	case (response::method_not_allowed):
			return method_not_allowed_content;
	case (response::bad_request):
			return bad_request_content;
	case (response::forbidden):
			return forbidden_content;
	default:
			return internal_server_error_content;
	}
}

std::string Parser::get_status_string (response::status_type type)
{
	switch (type)
	{
	case (response::ok):
			return ok_string;
	case (response::not_found):
			return not_found_string;
	case (response::method_not_allowed):
			return method_not_allowed_string;
	case (response::bad_request):
				return bad_request_string;
	case (response::forbidden):
				return forbidden_string;
	default:
		return internal_server_error_string;
	}
}

std::vector<boost::asio::const_buffer> Parser::format_response_to_send_it_to_socket()
{
	std::vector<boost::asio::const_buffer> buffer;

	buffer.push_back(boost::asio::buffer(_response.status_string));

	std::size_t n = _response.headers.size();

	for (std::size_t i = 0; i < n; i += 2)
	{
		buffer.push_back(boost::asio::buffer(_response.headers[i]));
		buffer.push_back(boost::asio::buffer(name_value_separator));

		buffer.push_back(boost::asio::buffer(_response.headers[i + 1]));
		buffer.push_back(boost::asio::buffer(crlf));

		//header_string += _response.headers[i] + ": " + _response.headers[i + 1] + "\r\n";
	}

	//header_string += "\r\n";
	//header_string += _response.body;
	//header_string += "\r\n\r\n\0";

	//buffer.push_back(boost::asio::buffer(header_string));

	buffer.push_back(boost::asio::buffer(crlf));
	if (!_f_is_HEAD_method)
	{
		buffer.push_back(boost::asio::buffer(_response.body));
	}

	return buffer;
}



