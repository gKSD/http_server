//============================================================================
// Name        : my_http_server.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "include/client.hpp"

#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	try
	  {
	    /*if (argc != 3)
	    {
	      std::cerr << "Usage: client <host> <port>\n";
	      return 1;
	    }

	    string address = argv[1];
	    string port = argv[2];
	    size_t thread_count = 1;
	    */

		string address = "localhost";
		string port = "8086";
		size_t thread_count = 1;

	    //Client client(address, port, thread_count);

	    //client.run();

		boost::shared_ptr<Client> client(new Client(address, port, thread_count));
		client->run();
	  }
	  catch (std::exception& e)
	  {
	    std::cerr << "Exception: " << e.what() << "\n";
	  }

	  return 0;
}
