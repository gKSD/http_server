/*
 * parser.cpp
 *
 *  Created on: 29.03.2014
 *      Author: sofia
 */

#include <iostream>
#include <sstream>

#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <boost/regex.hpp>


#include <string.h>
#include <stdlib.h>
#include <stddef.h>


#include "include/parser.hpp"

using namespace std;
Parser::Parser() :
		_request(),
		_f_is_valid_url(true),
		_f_is_supported_protocol(true),
		_f_is_supported_method(true),
		_f_has_method(true),
		_f_has_url(true),
		_f_has_protocol(true)
{

}

bool Parser::parse(const string& request)
{
	cout<< "Parser \n "<< request << endl;

	stringstream stream_request(request);

	string line;
	//getline(stream_request, qqq);

	int j = 0;
	while (getline(stream_request, line))
	{
		if (j == 0) {j++; parse_first_line(line);}
		cout << "********* "<< line<<endl;
	}


	//1) парсим метод, урл и протокол
	//остальные заголовки ниже отдельно
	int i = 0;
	const char *ptr = request.c_str();

	while(isspace(*ptr)) ptr++;

	while(isalpha(*ptr)) //parse method - GET or POST
	{
		cout<<*ptr<<endl;
		_request.method.push_back(*ptr);
		i++;
		ptr++;
	}

	while(isspace(*ptr)) ptr++;


	//parse url
	char url[MAX_LENGTH] = {0};
	char *tmp = url;
	char code[3];//, decode;

	memset(url, 0, MAX_LENGTH);

	while(isgraph(*ptr))
	{
		cout<<*ptr<<endl;
		//_request.uri.push_back(*ptr);

		if (*ptr == '%')
		{
			if (strlen(ptr) >= 2 && (strncmp(ptr + 1,  left_code_border, 2) >= 0 && strncmp(ptr + 1, rigth_code_border, 2) <= 0))
			{
				code[0] = *(++ptr);
				code[1] = *(++ptr);
				//long lcode = strtol(code, 0, 16);
				//decode = (char)lcode;
				//*tmp = decode;
				*tmp = (char) strtol(code, 0, 16);
				tmp++;
				ptr++;
				continue;
			}
		}

		*tmp = *ptr;
		tmp++;
		ptr++;
	}

	while(isspace(*ptr)) ptr++;
	while(isgraph(*ptr)) //parse protocol
	{
		cout<<*ptr<<endl;
		_request.protocol.push_back(*ptr);
		ptr++;
	}

	cout <<"Method: "<< _request.method<<endl;
	cout <<"URL: " << _request.url<<endl;
	cout <<"Protocol: " << _request.protocol<<endl;

	//парсим остальные заголовки
	while (*ptr)
	{
		string header;
		string value;

		while (!isgraph(*ptr)) ptr++;

		while(ptr && *ptr && *ptr != ':')
		{
			header.push_back(*ptr);
			ptr++;
		}

		ptr++;
		while (ptr && *ptr && isspace(*ptr)) ptr++;

		while(ptr && *ptr && *ptr != '\n')
		{
			value.push_back(*ptr);
			ptr++;
		}
		if (ptr && *ptr && *ptr == '\r') ptr++;

		if (is_valid_header(header))
		{
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

	if (!((*it).compare(method_GET) == 0 || (*it).compare(method_POST) == 0 || (*it).compare(method_HEAD) == 0 )) _f_has_method = false;
	if ((*it).compare(method_GET) != 0) _f_is_supported_method = false;

	_request.method = *it;

	it++;
	cout<< "2: "<< *it<<endl;
	//boost::regex regex("(\/[\w\s\.]+)+\/?");
	boost::regex regex("/[0-9a-zA-Z_%\./-]*");
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

		if (*ptr == '?') break;

		if (*ptr == '!' || *ptr == ';' || *ptr == '\"' || *ptr == '#' || *ptr == '&' || *ptr == '\'' || *ptr == '*' ||
						*ptr == '<' || *ptr == '>' || *ptr == '?' || *ptr == '`' || *ptr == '|')
			return "/";

		*tmp = *ptr;
		tmp++;
		ptr++;
	}
	cout << url <<endl;

	return url;
}



//необходимо возращать 405 если: POST, не поддерживаемый протокол
//возращаем index.html если урл задан /  или содержится невалидный урл
