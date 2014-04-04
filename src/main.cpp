//============================================================================
// Name        : my_http_server.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "include/httpserver.hpp"
#include <boost/lexical_cast.hpp>

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	try
	  {
		string address;
		string port;
		string document_root;
		size_t thread_count;

	    if (argc != 5)
	    {
	      std::cerr << "Usage: client <host> <port> <document root> <amount of threads>\n";
	      return 1;
	    }

	    address = argv[1];
	    port = argv[2];
	    document_root = argv[3];
	    thread_count = boost::lexical_cast<std::size_t>(argv[4]);

		//string address = "localhost";
		//string port = "8086";
		//string document_root = "/home/sofia/static/";
		//size_t thread_count = 1;


		Http_server http_server(address, port, thread_count, document_root);
		http_server.run();

		std::cout << "after run" <<std::endl;
	  }
	  catch (std::exception& e)
	  {
	    std::cerr << "Exception: " << e.what() << "\n";
	  }

	  return 0;
}
