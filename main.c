#include "gps.h"

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
        int lat_col = -1, lon_col = -1;
        int row_count = 0, tot_cols = 0, current_col = 0;
        int first_row_is_data = -1;     // -1 = not determined, 0 = features, 1 = data

        fgets(buffer, 1024, input_file);    // Read line
        // get total number of columns
        // get lat/long column number if exists (0 index!)
        data = strtok(buffer, ",");
        while (data != NULL) {
            data = trim(data);

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
        printf("Total columns: %d\n", tot_cols);
        
        // If lat/long columns not found, prompt user to enter them
        if (lat_col == -1 || lon_col == -1) {
            printf("Could not find latitude and longitude columns. Please enter the column numbers for latitude and longitude, separated by a space (The first column is \"1,\" etc.): ");
            scanf("%d %d", &lat_col, &lon_col);
        }

        // Count number of rows !!!!!!!!!!!-----------MIGHT BE WRONG-----------------!!!!!!!!!!!!!
        while (fgets(buffer, 1024, input_file)) {
            row_count++;
        }
        printf("Number of rows: %d\n", row_count);  


        // Parse data
        // If first row is data and not features, start there. Else start 2nd row.
        // Detect valid coordinate, then determine if 1st row is data. Then insert into data struture.   
        rewind(input_file);
        for (int i = 0; i < row_count; i++) {}

        // Determine if 1st row is data or features
        fgets(buffer, 1024, input_file);    // Read line
        printf("========== Determine first row data or features ==========\n");
        printf("Buffer: %s", buffer);
        data = strtok(buffer, ",");         // Tokenize 1st field
        
        while (data != NULL) {
            data = trim(data);
            if (checkValidLatitude(data) != 200) {
                // We've hit a coordinate. First row is data, not features.
                first_row_is_data = 1;
                rewind(input_file); // Start from the beginning
                break;
            };
            data = strtok(NULL, ",");   // to tokenize the same string to get the next token
            first_row_is_data = 0;      // First row is features
        }
        printf("First row is data: %d\n", first_row_is_data);
        printf("==========================================================\n");
        

        fclose(input_file);

    } else if (input_method == 1) {

    }

    return 0;
}