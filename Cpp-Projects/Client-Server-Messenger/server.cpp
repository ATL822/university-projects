// File: server.cpp
// Course: CSCI 4300 - Data Communication Networks
// Author: Andrew T. Law
// Due: 06 Decemeber 2023
// Description: This program will listen for clients attempting to connect. 
// Connects with them and then serves as an interface for one of the users to send messages with.

#include <iostream>
#include <winsock2.h>

using namespace std;

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char buffer[1024];
    int bytesReceived;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error initializing Winsock\n";
        return 1;
    }

    // Create a socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        cerr << "Error creating socket\n";
        WSACleanup();
        return 1;
    }

    // Set up the server address and port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // using port 8080 for this project.
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Error binding socket\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Start listening for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Error listening for connections\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Waiting for user to connect...\n";

    // Accept incoming connections
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen)) == INVALID_SOCKET) {
        cerr << "Error accepting connection\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Startup message
    cout << " _____________________________________________________________ \n"
		 <<	"|                     Connected to user!                      |\n"
		 << "| Usage Notes:                                                |\n"
		 << "|    - After receiving a message, you will enter SEND mode.   |\n"
		 << "|    - In SEND mode, you can send messages to the other user. |\n"
		 << "|    - Type '#' to exit SEND mode.                            |\n"
		 << "|    - Type '-exit' to terminate connection.                  |\n"
		 << "|_____________________________________________________________|\n";

    // Messaging loop
	string userIn;
	const char* message;
	bool end_connection_local = false;
    bool end_connection_extern = false;
	bool in_send_mode = false;
	
	while(!end_connection_local || !end_connection_extern){
		if(in_send_mode){ // Get message from user and send over connection.
			
			cout << "You: ";
			getline(cin, userIn);
			
			if(userIn == "-exit"){ // If the user enters '-exit' send message to other user, and exit loop.
				message = "Connected user entered '-exit'. Connection will terminate after your messages are sent.";
				send(clientSocket, message, strlen(message), 0);
				end_connection_local = true;
			}
			else if(userIn == "#"){ // If the user enter '#' exit send mode.
				message = userIn.c_str();
				send(clientSocket, message, strlen(message), 0);
				in_send_mode = false;
			}
			else{
				message = userIn.c_str();
				send(clientSocket, message, strlen(message), 0);
			}
		}
		else{ // Receive messages and display.
			
			bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

			if(bytesReceived <= 0){	// if nothing is recieved, the connection was terminated.
				cout << "Connection lost.\n";
				break;
			}
			
			buffer[bytesReceived] = '\0'; // add null char to send of cstring.

			if(buffer[0] == '#'){ // if connected user enters '#', your turn to be in send mode.
                if(end_connection_local){
                    cout << "Terminating connection.\n";
                    end_connection_extern = true;
                }
                else{
				    cout << "Entering SEND mode.\n";
				    in_send_mode = true;
                }
            }
			else{ // 
				cout << "Connected User: " << buffer << '\n';
			}
		}
	}

    // Close the connections
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
