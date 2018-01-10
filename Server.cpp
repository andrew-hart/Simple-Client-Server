#include "thread.h"
#include "socketserver.h"
#include "socket.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <string>
#include <algorithm>

using namespace Sync;

//Variable to tell all if server is running
bool isRunning = true;

//Function to mannipulate client string
void mannipulate(Socket serverSocket) {
    std::cout<<"New client"<<std::endl;

    //Alter client strings while server is running
    while(isRunning) {
        //Store client message
        ByteArray clientMessageByte("");

        //Check for error and if so stop
        if(serverSocket.Read(clientMessageByte) == 0) {
            break;
        }

        //Manuiplate cleint message to all uppercase
        std::string clientMessageString = clientMessageByte.ToString();
        transform(clientMessageString.begin(), clientMessageString.end(), clientMessageString.begin(), ::toupper);

        //Send back to client
        ByteArray clientResponse(clientMessageString);
        serverSocket.Write(clientResponse);
    }

    //Close socket when not running
    serverSocket.Close();
    std::cout<<"Closing client connection"<<std::endl;
}

//Function that checks if user enters done
void check(SocketServer *serverSocket) {
    //Run until client enters done
    while(isRunning) {
        std::string response;
        std::cin>>response;

        //Shutdown if done is entered
        if(response.compare("done") == 0) {
            isRunning = false;
            serverSocket->Shutdown();
        }
    }
} 

int main(void)
{
    std::cout << "I am a server." << std::endl;

    int port = 2000;
    //Start the server
    SocketServer server(port);

    //Create main thread
    std::thread mainThread(check, &server);
    mainThread.detach();
    
    //Create new threads while running
    while(isRunning) {

        try {
            //Create new thread
            Socket serverSocket = server.Accept();
            std::thread newServer(mannipulate, serverSocket);
            newServer.detach();

        }
    	catch (TerminationException) {
            std::cout<<"Termination exception"<<std::endl;
            break;
        }
    }
    
    std::cout<<"Server is shutting down.."<<std::endl;
        
}
