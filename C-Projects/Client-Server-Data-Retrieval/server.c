// Andrew Law -- Project 3 -- Due: 20 June 2023
// Client-Server Programming -- This program is the server part of Project 3. This will process the user inputs and complete the action requested by the user. Some actions will request additional user input.

#include "csapp.h"
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>

// Function Prototypes
void serverFunction(int);
int getUserRequest(char*);
void getUserInformation(char*, char*);
void addToFile(const char*);
void searchFile(char*, int, char*);
void *thread(void *vargp);

// Number for assigning unique values as client ids.
unsigned int client_id;
// Number for keeping track of number of readers.
unsigned int reader_count;

// Semaphores
sem_t c_id; // Control access to client_id.
sem_t mutex; // Control access to reader_count.
sem_t writer_lock; // Control ability to write to the file.

// Structure used to hold information retrieved from the file.
struct recordEntry{
    char first_name[MAXLINE];
    char last_name[MAXLINE];
    char department[MAXLINE];
    char zip_code[MAXLINE];
    int salary;    
} recordEntry;

// This is from the templete found in lab 9.
int main(int argc, char *argv[]){
    
    client_id = 1; // Intialize client id to 1 for first client.
    
    // Networking Vairables
    int listenfd; // Listening port.
    int *connfdp; //file descriptor to communicate with the client
    socklen_t clientlen; 
    struct sockaddr_storage clientaddr; /* Enough space for any address */
    char client_hostname[MAXLINE], client_port[MAXLINE]; // Unique client values to store into global array later.
    pthread_t tid; // Thread id.
    
    // Semaphore initialization
    sem_init(&c_id, 0, 1);
    sem_init(&mutex, 0, 1);
    sem_init(&writer_lock, 0, 1);
    
    // Program usage error checking.
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    
    // Server listens for clients attempting to connect. 
    listenfd = Open_listenfd(argv[1]);
    
    // Server runs in the infinite loop.
    // To stop the server process, it needs to be killed using the Ctrl+C key.
    while (1) {
        
        clientlen = sizeof(struct sockaddr_storage);
        
        // wait for the connection from the client.
        connfdp = malloc(sizeof(int));
        *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to Client_%d: (%s, %s)\n", client_id, client_hostname, client_port);
        
        Pthread_create(&tid, NULL, thread, connfdp); // Create a thread to service the client.
        
        // TODO: - Modify the server function options to work with semaphore controls.
        //          - Writing can only be done when no one else is reading or writing (if global semaphore database_accessed_read == 0 && database_accessed_write == 0).
        //                                                                                                     (int value)                           (binary value)
        //          - Reading can be done anytime EXECPT for when someone is writing (if global semaphore database_accessed_write == 0).
        
    }
    exit(0);
}


// Server thread routine. This simply detatches the thread to run independently, and performs server functions for the client.
void *thread(void *vargp){
    
    P(&c_id); // Lock the client id.
    int id = client_id; // Assign the client id.
    client_id++; // Update id for next client.
    V(&c_id); // Unlock the client id.
    
    int connfd = *((int *)vargp);
    Pthread_detach(pthread_self());
    Free(vargp);
    serverFunction(connfd);
    Close(connfd);

    printf("Client_%d disconnected.\n", id); // Print disconnection message.
    
    return NULL;
}


// This function is the main processing point for information recieved from the client.
void serverFunction(int connfd){
    char buffer[MAXLINE]; //MAXLINE = 8192 defined in csapp.h
    char server_message[MAXLINE];
    char information_for_request[MAXLINE];
    int user_selection = 0;
    bool continue_running = true;
    
    while(continue_running){
        //resetting the buffer
        bzero(buffer,MAXLINE);
        read(connfd, buffer, MAXLINE);
        
        bzero(information_for_request,MAXLINE);
        user_selection = getUserRequest(buffer);
        getUserInformation(buffer, information_for_request);
        
        switch(user_selection){
            case 1:
                P(&writer_lock); // Wait for writer lock to be released. Then lock the writer lock to allow for writing.
                addToFile(information_for_request); // Add entry to records.csv.
                V(&writer_lock); // Release the writer lock.
                
                strcpy(server_message,"Record added Sucessfully!!\n");
                
                write(connfd,server_message,strlen(server_message));
                break;
                
            case 2:
                // This section used Lecture 18/19, slide 47 as reference. It is not an exact copy but it is similar. It will be used later and marked with a *** comment.
                sem_wait(&mutex); // Lock reader_count for modification.
                reader_count++; // Increase reader_count by 1.
                if(reader_count == 1){ // First in
                    sem_wait(&writer_lock); // If this user is the first to be reading, lock the ability to write.
                }
                sem_post(&mutex); // Unlock the reader_count.
                // End of referenced section.
                
                searchFile(information_for_request, 2, buffer); // Search file for entries with matching first and last name. 
                
                strcpy(server_message, buffer);
                strcat(server_message, "\n");
                
                write(connfd,server_message,strlen(server_message));
                
                // This section used Lecture 18/19, slide 47 as reference. This code is used again in later sections, it will be marked with a *** comment.
                sem_wait(&mutex); // Lock reader_count for modification.
                reader_count--; // Decrease reader_count by 1.
                if(reader_count == 0){ // Last out
                    sem_post(&writer_lock); // If this user was the last one reading, unlock the ability to write.
                }
                sem_post(&mutex);
                // End of reference section.
                
                break;
                
            case 3:
                // ***
                sem_wait(&mutex); // Lock reader_count for modification.
                reader_count++; // Increase reader_count by 1.
                if(reader_count == 1){ // First in
                    sem_wait(&writer_lock); // If this user is the first to be reading, lock the ability to write.
                }
                sem_post(&mutex); // Unlock the reader_count.
                
                searchFile(information_for_request, 3, buffer); // Search file for entries with matching zip code.
                
                strcpy(server_message, buffer);
                strcat(server_message, "\n");
                
                write(connfd,server_message,strlen(server_message));
                
                // ***
                sem_wait(&mutex); // Lock reader_count for modification.
                reader_count--; // Decrease reader_count by 1.
                if(reader_count == 0){ // Last out
                    sem_post(&writer_lock); // If this user was the last one reading, unlock the ability to write.
                }
                sem_post(&mutex);
                
                break;
                
            case 4:
                // ***
                sem_wait(&mutex); // Lock reader_count for modification.
                reader_count++; // Increase reader_count by 1.
                if(reader_count == 1){ // First in
                    sem_wait(&writer_lock); // If this user is the first to be reading, lock the ability to write.
                }
                sem_post(&mutex); // Unlock the reader_count.
                
                searchFile(information_for_request, 4, buffer); // Search file for entries with salary meeting search criteria.
                
                strcpy(server_message, buffer);
                strcat(server_message, "\n");
                
                write(connfd,server_message,strlen(server_message));
                
                // ***
                sem_wait(&mutex); // Lock reader_count for modification.
                reader_count--; // Decrease reader_count by 1.
                if(reader_count == 0){ // Last out
                    sem_post(&writer_lock); // If this user was the last one reading, unlock the ability to write.
                }
                sem_post(&mutex);
                
                break;
            
            case 5:
                // Terminate connection.
                strcpy(server_message,"Connection Closed!!\n");
                write(connfd,server_message,strlen(server_message));
                continue_running = false;
                break;
                
            default:
                // Error has occured.
                    // Send error message to the user.
                strcpy(server_message,"An error has occured. Unable to process your request. Please try again.\n");
                write(connfd,server_message,strlen(server_message));
                break;
        }
    }
    
    return;
}


// This function extracts the number indicating the action selected by the user. 
int getUserRequest(char* buffer){
    int user_selection;
    char user_selection_holder[2];
    
    // Extract the user selection from the buffer.
    user_selection_holder[0] = buffer[0];
    user_selection_holder[1] = '\0'; // Add a null char to the end.
    
    // Convert the user selection string to an int.
    user_selection = strtol(user_selection_holder, NULL, 0);
    
    return user_selection;
}


// This function extracts the information relevent to the action requested by the user.
void getUserInformation(char* buffer, char* information_for_request){
    int request_index = 0;
    
    // Transfer the information needed to perform user specified action to a new string (i.e., copy over buffer contents without the user_selection value or following comma).
        for(int buffer_index = 2; buffer_index < strlen(buffer); buffer_index++){ // The buffer_index starts at 2 to ignore the user_selection and following comma.
            // Copy over contents of buffer to new string.
            information_for_request[request_index] = buffer[buffer_index];
            request_index++;
        }
        
    return;
}


// This function will add a user inputted entry to the records.csv file.
void addToFile(const char* entry_information){

    // string for holding the formatted entry.
    char formatted_entry[MAXLINE];
    
    // Specify file name here.
    const char* file_name = "records.csv";
    
    // Open the file.
    FILE *user_file;
    user_file = fopen(file_name,"a");
    
    strcpy(formatted_entry, "\n");
    strcat(formatted_entry, entry_information);
    formatted_entry[strlen(formatted_entry)-1] = '\0'; // Change the newline that was retrieved from the user to null char.
    
    // Add entry_information to end of records.csv
    fprintf(user_file, formatted_entry);
    
    // Close the file.
    fclose(user_file);
    return;
}


// This function will search the .csv file for entries that correspond to the user specified search parameters. This function will add entries that match the comparison to the buffer for return to the client.
void searchFile(char* information_for_request, int user_selection, char* buffer){
    // Resetting the buffer
    bzero(buffer,MAXLINE);
    
    // Variable for keeping track of number of saved entries
    int saved_entries = 0;
    
    // Comparison variables
    char first_name_search_term[MAXLINE];
    char last_name_search_term[MAXLINE];
    char zip_code_search_term[MAXLINE];
    int salary_comparison_operator;
    int salary_search_term;
    
    // Array of structures for holding the entries from the file.
    struct recordEntry retrieved_entries[100]; // Arbitrary number.
    
    // Create a variable to hold a line retrieved from the file.
    char line_holder[MAXLINE];
    
    // Specify file name here.
    const char* file_name = "records.csv";
    
    // Open the file.
    FILE *user_file;
    user_file = fopen(file_name,"r");
    
    int entry_count = 0;
    // Get all the entries from the csv file and add them to an array of entry structs.
    while(fgets(line_holder, MAXLINE, user_file)){
        // Parse the created string for structure elements.
        sscanf(line_holder, "%[^,],%[^,],%[^,],%[^,],%d", retrieved_entries[entry_count].first_name, retrieved_entries[entry_count].last_name, retrieved_entries[entry_count].zip_code, retrieved_entries[entry_count].department, &retrieved_entries[entry_count].salary);
        
        entry_count++;
    }
    
    
    // If there are still lines left to get from the file, get a line from the file. This stores a line as a string in line_holder for use later.
    for(int i = 0; i < entry_count; i++){ 
        // Set up the comparison values based on which option was selected by the user.
        switch(user_selection){
            // Parse the informaiton_for_request string. Extract the parts that will be needed for comparison with file entries.
            case 2: 
                // Information_for_request sent as [first_name, last_name].
                sscanf(information_for_request, "%[^,], %s", first_name_search_term, last_name_search_term);
                // Compare via first_name, last_name
                if(strcmp(first_name_search_term,retrieved_entries[i].first_name) == 0 && strcmp(last_name_search_term,retrieved_entries[i].last_name) == 0){
                    if(saved_entries == 0){
                        entryStructToString(retrieved_entries[i], line_holder); // Convert the struct to a string for placement in the buffer.
                        strcpy(buffer, line_holder); // Since buffer is reset at start of function, need to copy over first entry.
                        saved_entries++;
                    }
                    else{
                        entryStructToString(retrieved_entries[i], line_holder); // Convert the struct to a string for placement in the buffer.
                        strcat(buffer, line_holder); // Concatenate further entries that match the search parameters.
                        saved_entries++;
                    }
                }
                // If there are no matching records at the end of comparing each, save a special "no-match" message.
                if(i == entry_count - 1 && saved_entries == 0){
                    strcpy(buffer, "No record found!!");
                }
                break;
                
            case 3:
                // Information_for_request sent as [zip_code].
                sscanf(information_for_request, "%s", zip_code_search_term);
                
                // Compare via zip_code
                if(strcmp(zip_code_search_term,retrieved_entries[i].zip_code) == 0){
                    if(saved_entries == 0){
                        entryStructToString(retrieved_entries[i], line_holder); // Convert the struct to a string for placement in the buffer.
                        strcpy(buffer, line_holder); // Since buffer is reset at start of function, need to copy over first entry.
                        saved_entries++;
                    }
                    else{
                        entryStructToString(retrieved_entries[i], line_holder); // Convert the struct to a string for placement in the buffer.
                        strcat(buffer, line_holder);
                        saved_entries++;
                    }
                }
                // If there are no matching records at the end of comparing each, save a special "no-match" message.
                if(i == entry_count - 1 && saved_entries == 0){
                    strcpy(buffer, "No record found!!");
                }
                break;
                
            case 4:
                // Information_for_request sent as [comparison_operator, salary].
                sscanf(information_for_request, "%d, %d", &salary_comparison_operator, &salary_search_term);
                
                switch(salary_comparison_operator){
                    // Error checking on the client-side ensures a valid comparison type will be sent.
                    case 1:
                        if(retrieved_entries[i].salary > salary_search_term){
                            if(saved_entries == 0){
                                entryStructToString(retrieved_entries[i], line_holder); // Convert the struct to a string for placement in the buffer.
                                strcpy(buffer, line_holder); // Since buffer is reset at start of function, need to copy over first entry.
                                saved_entries++;
                            }
                            else{
                                entryStructToString(retrieved_entries[i], line_holder); // Convert the struct to a string for placement in the buffer.
                                strcat(buffer, line_holder);
                                saved_entries++;
                            }
                        }
                        // If there are no matching records at the end of comparing each, save a special "no-match" message.
                        if(i == entry_count - 1 && saved_entries == 0){
                            strcpy(buffer, "No record found!!");
                        }
                        break;
                        
                    case 2:
                        if(retrieved_entries[i].salary < salary_search_term){
                            if(saved_entries == 0){
                                entryStructToString(retrieved_entries[i], line_holder); // Convert the struct to a string for placement in the buffer.
                                strcpy(buffer, line_holder); // Since buffer is reset at start of function, need to copy over first entry.
                                saved_entries++;
                            }
                            else{
                                entryStructToString(retrieved_entries[i], line_holder); // Convert the struct to a string for placement in the buffer.
                                strcat(buffer, line_holder);
                                saved_entries++;
                            }
                        }
                        // If there are no matching records at the end of comparing each, save a special "no-match" message.
                        if(i == entry_count - 1 && saved_entries == 0){
                            strcpy(buffer, "No record found!!");
                        }
                        break;
                        
                    case 3:
                        if(retrieved_entries[i].salary >= salary_search_term){
                            if(saved_entries == 0){
                                entryStructToString(retrieved_entries[i], line_holder); // Convert the struct to a string for placement in the buffer.
                                strcpy(buffer, line_holder); // Since buffer is reset at start of function, need to copy over first entry.
                                saved_entries++;
                            }
                            else{
                                entryStructToString(retrieved_entries[i], line_holder); // Convert the struct to a string for placement in the buffer.
                                strcat(buffer, line_holder);
                                saved_entries++;
                            }
                        }
                        // If there are no matching records at the end of comparing each, save a special "no-match" message.
                        if(i == entry_count - 1 && saved_entries == 0){
                            strcpy(buffer, "No record found!!");
                        }
                        break;
                        
                    case 4:
                        if(retrieved_entries[i].salary <= salary_search_term){
                            if(saved_entries == 0){
                                entryStructToString(retrieved_entries[i], line_holder); // Convert the struct to a string for placement in the buffer.
                                strcpy(buffer, line_holder); // Since buffer is reset at start of function, need to copy over first entry.
                                saved_entries++;
                            }
                            else{
                                entryStructToString(retrieved_entries[i], line_holder); // Convert the struct to a string for placement in the buffer.
                                strcat(buffer, line_holder);
                                saved_entries++;
                            }
                        }
                        // If there are no matching records at the end of comparing each, save a special "no-match" message.
                        if(i == entry_count - 1 && saved_entries == 0){
                            strcpy(buffer, "No record found!!");
                        }
                        break;
                        
                    case 5:
                        if(retrieved_entries[i].salary == salary_search_term){
                            if(saved_entries == 0){
                                entryStructToString(retrieved_entries[i], line_holder); // Convert the struct to a string for placement in the buffer.
                                strcpy(buffer, line_holder); // Since buffer is reset at start of function, need to copy over first entry.
                                saved_entries++;
                            }
                            else{
                                entryStructToString(retrieved_entries[i], line_holder); // Convert the struct to a string for placement in the buffer.
                                strcat(buffer, line_holder);
                                saved_entries++;
                            }
                        }
                        // If there are no matching records at the end of comparing each, save a special "no-match" message.
                        if(i == entry_count - 1 && saved_entries == 0){
                            strcpy(buffer, "No record found!!");
                        }
                        break;
                } // End of internal switch.
                break; // Case 4 break.
        }   // End of switch.
    } // End of while.
    
    // Close the file.
    fclose(user_file);
    
    return;
}


// This function will convert a structure retrieved from the records.csv file
void entryStructToString(struct recordEntry entry, char* output_string){
    // string for salary for conversion from int to string.
    char salary_string[MAXLINE];
    
    strcpy(output_string, "\n"); // Newline added for foramtting.
    strcat(output_string, entry.first_name);
    strcat(output_string, ",");  // Comma added for formatting.
    strcat(output_string, entry.last_name);
    strcat(output_string, ",");
    strcat(output_string, entry.zip_code);
    strcat(output_string, ",");
    strcat(output_string, entry.department);
    strcat(output_string, ",");
    // Convert salary int into a string for addition to buffer.
    sprintf(salary_string, "%d", entry.salary);
    strcat(output_string, salary_string);
    
    return;
}