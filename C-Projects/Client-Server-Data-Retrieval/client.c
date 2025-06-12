// Andrew Law -- Project 3 -- Due: 20 June 2023
// Client-Server Programming -- This program is the client part of Project 3. This will serve as the interfacing point for a user and the server; it will send user input to the server for processing and response.

#include "csapp.h"
#include <stdbool.h>

int getUserAction();
void getRecordEntry(char*);
void getName(char*);
void getZipCode(char*);
void getSalary(char*);


int main(int argc, char *argv[]){
    int clientfd; //file descriptor to communicate with the server
    char *host, *port;
    char buffer[MAXLINE]; //MAXLINE = 8192 defined in csapp.h
    bool continue_running = true; // Boolean flag used to control while loop.
    
    if (argc != 3){
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    
    host = argv[1];
    port = argv[2];
    clientfd = Open_clientfd(host, port);
        
    
    // While loop to keep process running until connection is terminated (option 5 selected).
    while(continue_running){
        
        // Get requested action.
        int user_selection = getUserAction();
        char user_selection_holder[2]; // Used to hold the requested action in as a string for use with strcpy function.
        
        // Convert user_selection to string.
        sprintf(user_selection_holder, "%d", user_selection);
        
        // Resetting the buffer. Buffer will be used to store requested action and information needed to complete it.
        bzero(buffer,MAXLINE);
        
        // NOTE:
        // Buffer sent to the server will be of the following format:
        // [user_selection,information_needed_for_request], where the information needed for the request will change based on what action the user selects.
        // e.g. for user_selection == 3, buffer will contain the following:
        // [3,zip_code], where zip_code is an input from the user.
        
        // Copy user requested action to buffer.
        strcpy(buffer, user_selection_holder);
        strcat(buffer, ","); // Add comma for formatting.
        
        char information_for_request[MAXLINE - 3]; // This will store the output of the menu functions.
        
        // Get information needed for requested action.
        switch(user_selection){
            case 1: // Add Record.
                // Get entry infromation from user and add it to the buffer.
                bzero(information_for_request,MAXLINE-3); // Zero out the information string for new information collection.
                getRecordEntry(information_for_request);
                strcat(buffer, information_for_request);
                break;
                
            case 2: // Search by Name.
                // Get name (first and last) from user and add it to the buffer.
                bzero(information_for_request,MAXLINE-3); // Zero out the information string for new information collection.
                getName(information_for_request);
                strcat(buffer, information_for_request);
                break;
                
            case 3: // Search by Zip Code.
                // Get zip code from the user and add it to the buffer.
                bzero(information_for_request,MAXLINE-3); // Zero out the information string for new information collection.
                getZipCode(information_for_request);
                strcat(buffer, information_for_request);
                break;
                
            case 4: // Search by Salary.
                // Get salary and comparison operator from the user and add it to the buffer.
                bzero(information_for_request,MAXLINE-3); // Zero out the information string for new information collection.
                getSalary(information_for_request);
                strcat(buffer, information_for_request);
                break;
                
            case 5: // Terminate Connection.
                strcat(buffer, information_for_request);
                // Set while loop flag to false (end loop).
                continue_running = false;
                break;
                
            default:
                // Error (Should not be possible with previous error checking).
                // As a precaution program will terminate.
                printf("An error has occured. Please relaunch and try again. Terminating connection...");
                continue_running = false;
                break;
        }
        
        // Send action request and relevent information from user to server.
        write(clientfd,buffer,strlen(buffer));
        bzero(buffer,MAXLINE);
            
        // Waiting for server response.
            
        //the message will be stored in buffer variable.
        read(clientfd,buffer,MAXLINE);
        printf("Message from Server:\n");
        
        //displaying the message in buffer on the console
        Fputs(buffer,stdout);
    }
    
    Close(clientfd);
    
    return 0;
}


// This function will print the menu and get the user's action request. The action request is returned.
int getUserAction(){
    int user_selection = 0;
    bool valid_input = false;
    char new_line_holder;
    
    // Get a user input, if the user input is invalid prompt user again.
    while(valid_input != true){
        // Print menu and get user selection.
        printf("\n(1) Add record\n(2) Search by Name\n(3) Search by Zip Code\n(4) Search by Salary\n(5) Terminate\nSelect an option [1,2,3,4 or 5]: ");
        scanf("%d", &user_selection);
        scanf("%c", &new_line_holder);
        
        // If the user input is 1, 2, 3, 4, or 5 terminate loop.
        if(user_selection > 0 && user_selection < 6){
            valid_input = true;
        }
        else{ // Else prompt user again.
            printf("\nInvalid option.\nPlease try again.");
        }
    }
        
    return user_selection;
}


// This function gets a record for entry into server data file. This is used if option 1 is requested.
// NOTE:
// This function and the others following could likely be visually cleaned up with a couple helper functions that would minimize the number of lines within this function.
// However, I did not implement them for this project as I do not think it is completely necessary and this works as required (even though it looks a little clunky).
// Alternativly, a structure based implementation could be used.
// However, I also did not think it was completely necessary for this section of the program.
void getRecordEntry(char information_for_request[]){
    char information_holder[MAXLINE / 8]; // Arbitrary large number (large enough to ensure no overflow is typical cases).
    
    printf("\nEnter First Name: ");
    fgets(information_holder, (MAXLINE / 8), stdin); // Get the first name of the new entry.
    information_holder[strlen(information_holder)-1] = ','; // Replace ending newline char with a comma (remove \n from string).
    strcpy(information_for_request, information_holder); // Put the first name in the output string.
    
    printf("Enter Last Name: ");
    fgets(information_holder, (MAXLINE / 8), stdin); // Get the last name of the new entry.
    information_holder[strlen(information_holder)-1] = ','; // Replace ending newline char with a comma (remove \n from string).
    strcat(information_for_request, information_holder); // Add last name to output string.
    
    printf("Enter Zip Code: ");
    fgets(information_holder, (MAXLINE / 8), stdin); // Get the zip code of the new entry.
    information_holder[strlen(information_holder)-1] = ','; // Replace ending newline char with a comma (remove \n from string).
    strcat(information_for_request, information_holder); // Add zip code to output string.
    
    printf("Enter Department: ");
    fgets(information_holder, (MAXLINE / 8), stdin); // Get the department of the new entry.
    information_holder[strlen(information_holder)-1] = ','; // Replace ending newline char with a comma (remove \n from string).
    strcat(information_for_request, information_holder); // Add department to output string.
    
    printf("Enter Salary: ");
    fgets(information_holder, (MAXLINE / 8), stdin); // Get the salary of the new entry.
    strcat(information_for_request, information_holder); // Add salary to output string.
    
    return;
}


// Get a name for search. This is used if option 2 is requested.
void getName(char information_for_request[]){
    char information_holder[MAXLINE / 8]; // Arbitrary large number (large enough to ensure no overflow is typical cases).
    
    printf("\nEnter First Name: ");
    fgets(information_holder, (MAXLINE / 8), stdin); // Get the first name of the new entry.
    information_holder[strlen(information_holder)-1] = ','; // Replace ending newline char with a comma (remove \n from string).
    strcpy(information_for_request, information_holder); // Put the first name in the output string.
    
    printf("Enter Last Name: ");
    fgets(information_holder, (MAXLINE / 8), stdin); // Get the last name of the new entry.
    strcat(information_for_request, information_holder); // Add last name to output string.
    
    return;
}


// Get a Zip Code for search. This is used if option 3 is requested.
void getZipCode(char information_for_request[]){
    char information_holder[MAXLINE / 8]; // Arbitrary large number (large enough to ensure no overflow is typical cases).
    
    printf("\nEnter Zip Code: ");
    fgets(information_holder, (MAXLINE / 8), stdin); // Get the zip code of the new entry.
    strcat(information_for_request, information_holder); // Add zip code to output string.
    
    return;
}


// Get a Salary and a comparison operator. This is used if option 4 is requested.
void getSalary(char information_for_request[]){
    bool comparison_type_invalid = false; // Used in basic error checking
    char information_holder[MAXLINE / 8]; // Arbitrary large number (large enough to ensure no overflow is typical cases).
    char information_holder_2[MAXLINE / 8];
    int comparison_operator = 0;
    
    printf("\nEnter Salary: ");
    fgets(information_holder, (MAXLINE / 8), stdin); // Get the salary of the new entry.
    information_holder[strlen(information_holder)-1] = '\0'; // Replace ending newline char with a \0 (remove \n from string).
    
    // This will loop until the user inputs a valid comparison type.
    do{
        printf("Enter Comparison Type ['>','<','==','>=','<=']: ");
        fgets(information_holder_2, (MAXLINE / 8), stdin); // Get the comparison operator.
    
        // There is probably a better way to do this, but this way is simple and works well enough for this project.
        // Change the string input to a int 
        if(strcmp(information_holder_2,">\n") == 0){
            comparison_operator = 1;
            comparison_type_invalid = false;
        }
        else if(strcmp(information_holder_2,"<\n") == 0){
            comparison_operator = 2;
            comparison_type_invalid = false;
        }
        else if(strcmp(information_holder_2,">=\n") == 0){
            comparison_operator = 3;
            comparison_type_invalid = false;
        }
        else if(strcmp(information_holder_2,"<=\n") == 0){
            comparison_operator = 4;
            comparison_type_invalid = false;
        }
        else if(strcmp(information_holder_2,"==\n") == 0){
            comparison_operator = 5;
            comparison_type_invalid = false;
        }
        else{
            printf("Invalid comparison type. Please try again.\n");
            comparison_operator = 0;
            comparison_type_invalid = true;
        }
    }while(comparison_type_invalid);
    
    // Send the information for this request as [comparison_operator, salary].
    sprintf(information_holder_2, "%d", comparison_operator);
    strcat(information_for_request, information_holder_2); // Add comparison operator to output string.
    strcat(information_for_request, ","); // Add comma to output string.
    strcat(information_for_request, information_holder); // Add salary to output string.
    
    return;
}

