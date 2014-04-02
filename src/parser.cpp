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


#include <string.h>
#include <stdlib.h>
#include <stddef.h>
//#include <string>

#include "include/parser.hpp"
#include "include/httpprotocol.hpp"

using namespace std;

Parser::Parser(string document_root) :
		_request(),
		_f_is_supported_protocol(true),
		_f_is_supported_method(true),
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
	_f_is_supported_method  = true;
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

	_response.headers.clear();
	_response.body.clear();
}

bool Parser::parse(const string& request)
{
	cout<< "Parser \n "<< request << endl;

	stringstream stream_request(request);

	string line;
	//1) парсим метод, урл и протокол
	//остальные заголовки ниже отдельно
	if (getline(stream_request, line)) parse_first_line(line);
	else return false; //&&&&&&

	cout <<"Method: "<< _request.method<<endl;
	cout <<"URL: " << _request.url<<endl;
	cout <<"Protocol: " << _request.protocol<<endl;

	//парсим остальные заголовки
	while (getline(stream_request, line))
	{
		vector<string> tokens;
		boost::split(tokens, line, boost::is_any_of(":"));
		vector<string>::iterator it = tokens.begin();
		if (is_valid_header(*it))
		{
			string header = *it;
			string value;

			it++;
			value.append(*it);

			for(it++; it != tokens.end(); it++)
			{
				value.push_back(':');
				value.append(*it);
			}

			_request.headers.insert(std::pair<string,string>(header, value));
			cout<<"** "<<header<< " => "<<_request.headers.at(header)<<endl;
		}
	}

	return true;
}

bool Parser::is_valid_url(string url)
{
	return true;
}

bool Parser::parse_first_line(string &str)
{
	vector<string> tokens;
	boost::split(tokens, str, boost::is_any_of(" \t\n\r"));
	vector<string>::iterator it = tokens.begin();
	cout<< "1: "<< *it<<endl;

	if (tokens.size() == 0)
	{
		_request.url = "";
		_request.protocol = "";
		_request.method = "";
		return false;
	}

	if (!((*it).compare(method_GET) == 0 || (*it).compare(method_POST) == 0 || (*it).compare(method_HEAD) == 0 )) _f_has_method = false;
	if ((*it).compare(method_POST) == 0)
	{
		_f_is_supported_method = true;
		_f_is_GET_method = true;
	}
	else if ((*it).compare(method_POST) == 0)
	{
		_f_is_supported_method = false;
		_f_is_POST_method = true;
	}
	else if ((*it).compare(method_HEAD) == 0)
	{
		_f_is_supported_method = false;
		_f_is_HEAD_method = true;
	}

	_request.method = *it;

	it++;
	if (it != tokens.end())
	{
		cout<< "2: "<< *it<<endl;
		//boost::regex regex("(\/[\w\s\.]+)+\/?");
		//boost::regex regex("/[0-9a-zA-Z_%\./-]*");
		boost::regex regex("/[0-9a-zA-Z_%\./\!\"#\&\'\*\,\:\;\<\=\>\?\[\|\^\`\{\|\}-]*");
		//boost::regex regex("/[\w%\/]*");
		boost::smatch match_result;
		if (boost::regex_match(*it,  match_result, regex))
		{
			cout << "correct";
			_f_has_url = true;
			_request.url = get_valid_url(*it);
		}
		else
		{
			cout<< "incorrect";
			_f_has_url = false;
			_request.url = "";
		}
	}
	else
	{
		_request.url = "";
		_request.protocol = "";
		return false;
	}

	it++;
	if (it != tokens.end())
	{
		//boost::regex regex("HTTP/?1\.[0|1]"));
		_request.protocol.append(*it);
	}
	else
	{
		_request.protocol = "";
		return false;
	}

	return true;
}

string Parser::get_valid_url(string &str)
{
	string result;

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
				cout << "parse encode"<<endl;
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
			//  /%20%20asda.jpeg
			cout<< "qweqwe"<<endl;
			slash_counter++;
			while (ptr != ptr_end && *ptr == '/') ptr++;

			if ((ptrdiff_t)(ptr_end - ptr) > 2 && *ptr == '.')
			{
				const char *cursor = ptr;
				int nesting_counter = 0;

				while ( (ptrdiff_t)(ptr_end - cursor) > 2 && strncmp (cursor, "../", 3) == 0)
				{
					nesting_counter++;
					if (nesting_counter > slash_counter)
						return "/";
					cursor += 3;
				}

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
	cout << url <<endl;

	return url;
}

response::status_type Parser::make_response()
{
	if (_f_has_method && _f_is_supported_method)
	{
		if (_f_is_GET_method)
		{
			//if (_f_has_protocol)
			if(!_f_has_url)
			{
				//405
				form_response(response::method_not_allowed);
				return response::method_not_allowed;
			}

			string path_to_file = _document_root;
			path_to_file.append(_request.url);
			if (_request.url[_request.url.size() - 1] == '/')
				path_to_file.append("index.html");

			std::ifstream is(path_to_file.c_str(), std::ios::in | std::ios::binary);
			if (!is)
			{
				//404
				form_response(response::not_found);
				return response::not_found;
			}

			char buffer[buffer_length];
			while (is.read(buffer, sizeof(buffer)).gcount() > 0)
				_response.body.append(buffer, is.gcount());

			//формируем заголовки. Тело ответа уже сформировано.
			form_response(response::ok);
			return response::ok;
		}
		else if (_f_is_POST_method)
		{
			//405
			form_response(response::method_not_allowed);
			return response::method_not_allowed;
		}
		else if (_f_is_HEAD_method)
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
		//405
		form_response(response::method_not_allowed);
		return response::method_not_allowed;
	}
}

void Parser::form_response(response::status_type status)
{
	if (status == response::not_found || status == response::internal_server_error || status == response::method_not_allowed)
		_response.body.append(get_content_string_by_status(status));

	cout << "Response body: " << _response.body <<endl;

	//char body_size [50];
	//itoa(_response.body.size(),body_size,10);

	_request.headers.insert(std::pair<string,string>("Date", "123123"));
	_request.headers.insert(std::pair<string,string>("Server", "1asdasdasd"));
	//_request.headers.insert(std::pair<string,string>("Content-Length", body_size));
	_request.headers.insert(std::pair<string,string>("Content-Type", "dfgdfg"));
	_request.headers.insert(std::pair<string,string>("Connection", "ololo"));
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
	default:
		return internal_server_error_string;
	}
}

//необходимо возращать 405 если: POST, не поддерживаемый протокол
//возращаем index.html если урл задан /  или содержится невалидный урл
