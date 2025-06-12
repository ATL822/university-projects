File: ReadMe.txt
Course: CSCI 4300 - Data Communication Networks
Author: Andrew T. Law
Due: 06 December 2023
Description: This file will explain how to use the programs included with my final project.
References: Referenced the winsock documentation found at https://learn.microsoft.com/en-us/windows/win32/winsock/winsock-reference  

This project is a two way messaging app that allows connected users to send 
messages back and forth between each other. This was implemented using a 
simple client-server model. The following are step-by-step instructions on 
the two programs' usages.

Step 1: user1 executes the server executable.
Step 2: user2 executes the client executable.
Step 3: user2 must enter the ip of user1 so that a connection can be established.
Step 4: user2 is in SEND mode to start with. They can send messages. 
        Type '#' alone to exit send mode.
Step 5: user1 receives '#'. This initiates SEND mode for user1. 
        They can send messages. Type '#' alone to exit send mode.
Step 6: Steps 4 and 5 repeat until one user terminates the connection.
        The connection is terminated by typing '-exit'.

When '-exit' is entered both program will be closed and program execution will be complete.