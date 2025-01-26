#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gps.h"

char* trim(char* str) {
    char* end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator 
    end[1] = '\0';

    return str;
}

int main () {

    char input_file_name [100];
    
    // Prompt user for input method
    int input_method;
    printf("Enter '1' to input coordinates manually, or '2' to read from a .csv file: ");
    scanf("%d", &input_method);
    if (input_method != 1 && input_method != 2) {
        printf("Please enter '1' for manual input or '2' to read from a .csv file.\n");
        return 1;
    }
    if (input_method == 2) {
        // Get input file name
        printf("Make sure the .csv file is in the current directory. Enter the name of the .csv file: ");
        scanf("%s", input_file_name);

        // Open input file for reading
        FILE* input_file = fopen(input_file_name, "r");
        if (input_file == NULL) {
            printf("Error opening file. Make sure filename is spelled correctly and the file is located in the current directory.\n");
            return 1;
        } else {
            printf("File opened successfully.\n");
        }

        // Parse .csv file
        // First row: try to find features named "latitude" and "longitude" (or variants). 
        //  If found, read the corresponding columns. If not, prompt user to enter which columns they are.
        
        char buffer [1024];
        char* data;
        int lat_col, lon_col;
        int tot_cols, current_col = 0;

        fgets(buffer, 1024, input_file);    // Read line
        // get total number of columns
        // get lat/long column number if exists
        data = strtok(buffer, ",");
        while (data != NULL) {
            data = trim(data);
            printf("Column %d: %s\n", current_col, data);

            if (strcmp(data, "\"latitude\"") == 0 || 
                strcmp(data, "\"Latitude\"") == 0 || 
                strcmp(data, "\"lat\"") == 0 || 
                strcmp(data, "\"Lat\"") == 0 ||
                strcmp(data, "latitude") == 0 || 
                strcmp(data, "Latitude") == 0 || 
                strcmp(data, "lat") == 0 || 
                strcmp(data, "Lat") == 0 
                ) {
                lat_col = current_col;
            } else if ( strcmp(data, "\"longitude\"") == 0 || 
                        strcmp(data, "\"Longitude\"") == 0 || 
                        strcmp(data, "\"lon\"") == 0 || 
                        strcmp(data, "\"Lon\"") == 0 || 
                        strcmp(data, "\"lng\"") == 0 || 
                        strcmp(data, "\"Lng\"") == 0 ||
                        strcmp(data, "\"long\"") == 0 ||
                        strcmp(data, "\"Long\"") == 0 ||
                        strcmp(data, "longitude") == 0 || 
                        strcmp(data, "Longitude") == 0 || 
                        strcmp(data, "lon") == 0 || 
                        strcmp(data, "Lon") == 0 || 
                        strcmp(data, "lng") == 0 || 
                        strcmp(data, "Lng") == 0 ||
                        strcmp(data, "long") == 0 ||
                        strcmp(data, "Long") == 0
                        ) {
                lon_col = current_col;
            }
            data = strtok(NULL, ",");   // to tokenize the same string to get the next token
            current_col++;
            tot_cols++;
        }
        printf("Latitude column: %d\n", lat_col);
        printf("Longitude column: %d\n", lon_col);
        //data = strtok(buffer, ",");         // Split line by comma. Try to find one of the tokens as "latitude"
        // while (strtok(buffer, ",") != NULL) {
        //     if (strcmp(data, "latitude") == 0 || strcmp(data, "Latitude" || strcmp(data, "lat") == 0 || strcmp(data, "Lat") == 0)) {
        //         lat_col = col_count;
        //     } else if (strcmp(data, "longitude") == 0 || strcmp(data, "Longitude" || strcmp(data, "lon") == 0 || strcmp(data, "Lon") == 0)) {
        //         lon_col = col_count;
        //     }



        //     if (strcmp(data, "latitude") == 0 || strcmp(data, "lat") == 0) {
        //         lat_col = col_count;
        //     } else if (strcmp(data, "longitude") == 0 || strcmp(data, "lon") == 0) {
        //         lon_col = col_count;
        //     }
        // }

        fclose(input_file);

    } else if (input_method == 1) {

    }

    return 0;
}