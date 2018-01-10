#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "thread.h"

using namespace Sync;


int main(void)
{
	std::cout << "I am a client" << std::endl;

	std::string message;
	std:: string response;
	std::string ipAddress = "127.0.0.1";
	int port = 2000;

	//Create and open socket on port 2001
	Socket client = Socket(ipAddress, port);
	client.Open();

	while(true) {
		
		std::cout<<"Please enter a message to send: ";
		std::cin>>message;
		
		//Check if the user entered done
		if(message == "done") {			
			break;
		}
		
		//Encode the message and send
		ByteArray send = ByteArray(message);
		int temp = client.Write(send);

		//Handle server response
		int input = client.Read(send);
		message = send.ToString();

		//Report the servers response
		std::cout<<"The server responsed with: " + message<<std::endl;

	} //End while

	//Close the socket
	client.Close();
	std::cout<<"Client is shutting down..."<<std::endl;

} //End main

