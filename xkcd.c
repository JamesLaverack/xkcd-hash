#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SHA3api_ref.h"
#define RANDOM_BUFFER_SIZE 100

void string_to_hex(char* str, uint_8t* hex) {
    char str_byte[3] = {};
    str_byte[2] = '\0';
    for(int i=0;i<128;i++) {
        strncpy(str_byte, str+i*2, 2);
        //printf("<%d>\n", i);
        sscanf(str_byte, "%x", (unsigned int *) &hex[i]);
        //hex[i] = htoi(str_byte);
    }
}

void print_hex(uint_8t* hex) {
    for(int i=0;i<128;i++) {
        printf("%x", hex[i]);
    }
}

int main(int argv, char **argc) {
    uint_8t *goal = malloc(128);
    uint_8t *hash = malloc(128);
    uint_8t *input = malloc(RANDOM_BUFFER_SIZE);
    char *input_hexed = malloc(RANDOM_BUFFER_SIZE*2+1);
    int best_distance = atoi(argc[2]);
    int distance;
    
    input_hexed[RANDOM_BUFFER_SIZE*2] = '\0';
    FILE* urandom = fopen("/dev/urandom", "r");
    
    // Parse input
    string_to_hex(argc[1], goal);

    // Report
    printf("Goal is ");
    print_hex(goal);
    printf("\n");
    
    while(1){
        // Loop and find a better one
        do{
            // Get a new random chunk
            fread(input, 1, RANDOM_BUFFER_SIZE, urandom);
            
            // Hex represent it
            for(int i=0;i<RANDOM_BUFFER_SIZE;i++) {
                sprintf(input_hexed+i*2, "%x", input[i]);
            }

            // Hash
            Hash(1024, input_hexed, strlen(input_hexed)*8, hash);
            
            // Distance from goal?
            distance = 0;
            for(int i=0;i<128;i++) {
                distance += __builtin_popcount(hash[i]^goal[i]);
            }
        }while(distance>=best_distance);
        
        // Report
        printf("======================   RESULT    ======================\n");
        printf("%s\n", input_hexed);
        printf("has distance %d.\n", distance);
        
        // Upload
        char* command_template = "curl -s \"http://almamater.xkcd.com/?edu=bristol.ac.uk\" --data \"hashable=";
        char buffer[1024] = {};
        strcpy(buffer, command_template);
        strcpy(buffer+strlen(command_template), input_hexed);
        strcpy(buffer+strlen(buffer), "\"\0");
        printf("====================== SERVER SAYS ======================\n");
        system(buffer);
        printf("\n=========================================================\n");
        
        // Update best
        best_distance = distance;
    }
    
    // Free up
    free(goal);
    free(hash);
    free(input);
}

