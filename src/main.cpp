//============================================================================
// Name        : my_http_server.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "include/httpserver.hpp"

#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	try
	  {
	    /*if (argc != 4)
	    {
	      std::cerr << "Usage: client <host> <port>\n";
	      return 1;
	    }

	    string address = argv[1];
	    string port = argv[2];
	    string document_root = argv[3];
	    size_t thread_count = 1;
	    */

		string address = "localhost";
		string port = "8086";
		string document_root = "/home/sofia/static/"; //"/home/sofia/programs/eclipse/my_http_server/doc_root";
		size_t thread_count = 1;

	    //Client client(address, port, thread_count, document_root);
	    //client.run();

		//boost::shared_ptr<Client> client(new Client(address, port, thread_count, document_root));
		//client->run();

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
