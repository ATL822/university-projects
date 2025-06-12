// File: client.cpp
// Course: CSCI 4300 - Data Communication Networks
// Author: Andrew T. Law
// Due: 06 Decemeber 2023
// Description: This program will connect to a server. It will prompt the user for a message to send to the server, 
// and send the message over a TCP connection.

#include <iostream>
#include <winsock2.h>

using namespace std;

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    int bytesReceived;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error initializing Winsock\n";
        return 1;
    }

    // Create a socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        cerr << "Error creating socket\n";
        WSACleanup();
        return 1;
    }

    // Set up the server address and port

	string server_ip;
	cout << "Enter the IP to connect to: ";
	cin >> server_ip;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // set the port (will use 8080 for this project).
    serverAddr.sin_addr.s_addr = inet_addr(server_ip.c_str()); // set the server ip using the user input.

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Error connecting to server\n";
        closesocket(clientSocket);
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
	bool in_send_mode = true;
	
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

    // Close the connection
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
