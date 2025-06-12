// Andrew Law -- Project 1 -- Due: 09 June 2023
// Hex Dump -- This program will perform the same action as the 'xxd' command in Linux. It will give a hex dump of a given file or standard input (see project 1 document for more detials).

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define CHARS_FROM_FILE_LENGTH 16

// This function will read the character array recieved from the file and print the ascii values in hex.
void print_ascii_in_hex(char);

// This function recieves a row from the file and prints the xxd formatted output.
void print_formatted_row(int, char *);


int main(int argc, char *argv[]){
    
    const char *file_name = argv[1];
    char chars_from_file[CHARS_FROM_FILE_LENGTH];
    
    int rows_printed = 0;
    bool eof_reached = false;
    
    // Basic argument check.
    if(argc == 2){
        // Do nothing.
    }
    else if(argc > 2){
        printf("Too many arguments. Only one argument expected.\n");
    }
    else{
        printf("One argument expected.\n");
    }
    
    // Open the file.
    FILE *user_file;
    user_file = fopen(file_name,"r");
    
    // Do until the end of the file is reached (eof_reached == true).
    do{
        // Read 16 characters from the open file and place them in the chars_from_file array.
        short index = 0;
        char char_holder;
        do{   
            // Get a character from the file.
            char_holder = fgetc((FILE*)user_file);
            // If eof is reached save null char in remaining array indices, otherwise save char.
            if(char_holder == EOF){
                for(int j = index; j < CHARS_FROM_FILE_LENGTH; j++){ // Do for all remaining elements of array.
                    *(chars_from_file + j) = '\0';
                }
                eof_reached = true; // Set eof_reached tag to true when end of file is reached.
            }
            else{
                *(chars_from_file + index) = char_holder; // Save character read to current index.
            }     
            
            index++;
            
        }while(!eof_reached && index < CHARS_FROM_FILE_LENGTH); // Continue until enough chars from file fill the array or end of file is reached.
    
        print_formatted_row(rows_printed, chars_from_file); // Print a row.
    
        rows_printed++;
        
    }while(!eof_reached);
        
    // Close the file.
    fclose(user_file);
    
    return 0;
}



void print_ascii_in_hex(char ascii_char){
    
    // Convert ascii char to ascii int.
    int ascii_int = (int) ascii_char;
    
    // If the character entered is '\0' print 2 spaces to match hex output of 2 characters.
    if(ascii_int == 0){
        printf("  ");
    }
    else{
        printf("%02x", ascii_int); // Print 2 digit hex encoded ascii value.
    }
    
    return;
}



void print_formatted_row(int rows_printed, char chars_from_file[CHARS_FROM_FILE_LENGTH]){
    
    // Print the starting index with the xxd formatting (8 digits, hex).
    printf("%08x:", rows_printed << 4); // Multiplying the number of rows printed by 16 (number of ascii characters printed per row).
    
    // Print characters from array in hex with correct formatting.
    int char_count = 0;
    for(int i = 0; i < CHARS_FROM_FILE_LENGTH; i++){
        // Every fifth character is a space to mimic the xxd formatting.
        if(char_count % 5 == 0){
            printf(" ");
            print_ascii_in_hex(*(chars_from_file+i));
            char_count += 3; // += 3 to include space character and 2 characters used in hex encoded ascii int.
        }
        else{
            print_ascii_in_hex(*(chars_from_file+i));
            char_count += 2; // += 2 for 2 characters used in hex encoded ascii int.
        }   
    }
    
    // Print 2 spaces for xxd formatting.
    printf("  ");
    
    // Print ascii characters from array.
    for(int i = 0; i < CHARS_FROM_FILE_LENGTH; i++){
        // If the character is printable print the character as it is.
        if(isprint(*(chars_from_file+i))){ 
            printf("%c",*(chars_from_file+i));
        }
        // If the character is '\0' print nothing.
        else if(*(chars_from_file+i) == '\0'){ 
            // Print nothing.
        }
        // If it is a not printable (non-NULL) character print a '.'. 
        else{ 
            printf(".");
        }
    }
    
    // Print new-line for xxd formatting.
    printf("\n"); 
    
    return;
}