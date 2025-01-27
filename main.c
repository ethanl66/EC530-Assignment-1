#include "gps.h"

int main () {

    char input_file_name [100];
    char input_file_name_2 [100];
    int arr_1_len;
    int arr_2_len;
    Coordinate* array1;
    Coordinate* array2;

// ============ ARRAY 1 ============
    printf("Array 1:\n");
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
        //printf("Buffer: %s", buffer);
        data = strtok(buffer, ",");         // Tokenize 1st field
        
        while (data != NULL) {
            data = trim(data);
            if (checkValidLatitude(data) != 200 || checkValidLongitude(data) != 200) {
                // We've hit a coordinate. First row is data, not features.
                first_row_is_data = 1;
                rewind(input_file); // Start from the beginning
                break;
            };
            data = strtok(NULL, ",");   // to tokenize the same string to get the next token
            first_row_is_data = 0;      // First row is features
        }
        rewind(input_file);
        printf("First row is data: %d\n", first_row_is_data);
        printf("==========================================================\n");

        
        // The data structure is an array of Coordinates (row #, lat, lon)
        array1 = malloc (row_count * sizeof(Coordinate));
        arr_1_len = row_count;
        

        // If 1st row is data, we start parsing from the 1st row. If not, we start from the 2nd row.
        if (first_row_is_data) {
            fgets(buffer, 1024, input_file);    // Go to 2nd line if 1st row is data
        }

        // Parse data
        int first_col_to_read, last_col_to_read, lat_first;
        if (lat_col > lon_col) {
            last_col_to_read = lat_col;
            first_col_to_read = lon_col;
            lat_first = 0;
        } else {
            last_col_to_read = lon_col;
            first_col_to_read = lat_col;
            lat_first = 1;
        }

        //for (int i = 0; i < row_count; i++) {       // For each row
        for (int i = 0; i < 10; i++) {       // For 10 rows
            fgets(buffer, 1024, input_file);        // read line
            data = strtok(buffer, ",");             // Tokenize 1st field
            //printf("Data1: %s\n", data);
            for (int j = 0; j < first_col_to_read - 1; j++) {   // strtok number of times until first_col_to_read (alr strtok'd once)
                
                    //data = trim(data);
                    data = strtok(NULL, ",");       // to tokenize the same string to get the next token
                    //printf("Data2: %s\n", data);
                
            }   
            
            // strtok should now be on the right spot for the first lat/long value
            data = trim(data);
            data = strtok(NULL, ",");
            //printf("Lat/long: %s\n", data);
            if (lat_first) {
                array1[i].latitude = checkValidLatitude(data);
            } else {
                array1[i].longitude = checkValidLongitude(data);
            }
            array1[i].row_num = i+1;
            
            // get to 2nd value to read, but reading takes 1
            for (int j = 0; j < last_col_to_read - first_col_to_read - 1; j++) {    
                data = strtok(NULL, ",");
            }
            
            // strtok should now be on the right spot for the second lat/long value
            data = trim(data);
            data = strtok(NULL, ",");
            //printf("Lat/long: %s\n", data);
            if (!lat_first) {
                array1[i].latitude = checkValidLatitude(data);
            } else {
                array1[i].longitude = checkValidLongitude(data);
            }
            array1[i].row_num = i+1;
        }


        // Print first 10 rows data structure
        for (int i = 0; i < 10; i++) {
            printf("Row %d: %f, %f\n", array1[i].row_num, array1[i].latitude, array1[i].longitude);
        }


        fclose(input_file);

    } else if (input_method == 1) {

        double lat, lon;
        int arr_len;
        double temp_lat, temp_lon;

        // Prompt user for coordinates
        printf("How many coordinates do you want to enter?\n");
        scanf("%d", &arr_len);
        while (arr_len < 1) {
            printf("Please enter a number greater than 0:\n");
            scanf("%d", &arr_len);
        }
        array1 = malloc (arr_len * sizeof(struct Coordinate));
        arr_1_len = arr_len;

        int count = 0;
        int len = arr_len;
        while (len > 0) {
            printf("Please enter a latitude in decimal degrees:\n");
            scanf("%lf", &temp_lat);
            printf("Array 1: Please enter a longitude in decimal degrees:\n");
            scanf("%lf", &temp_lon);
            array1[count].row_num = count + 1;
            array1[count].latitude = temp_lat;
            array1[count].longitude = temp_lon;
            len--;
            count++;
        }
    }





    // ============ ARRAY 2 ============
    printf("Array 2\n");
    // Prompt user for input method
    //int input_method;
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
        arr_2_len = row_count;


        // Parse data
        // If first row is data and not features, start there. Else start 2nd row.
        // Detect valid coordinate, then determine if 1st row is data. Then insert into data struture.   
        rewind(input_file);
        for (int i = 0; i < row_count; i++) {}

        // Determine if 1st row is data or features
        fgets(buffer, 1024, input_file);    // Read line
        printf("========== Determine first row data or features ==========\n");
        //printf("Buffer: %s", buffer);
        data = strtok(buffer, ",");         // Tokenize 1st field
        
        while (data != NULL) {
            data = trim(data);
            if (checkValidLatitude(data) != 200 || checkValidLongitude(data) != 200) {
                // We've hit a coordinate. First row is data, not features.
                first_row_is_data = 1;
                rewind(input_file); // Start from the beginning
                break;
            };
            data = strtok(NULL, ",");   // to tokenize the same string to get the next token
            first_row_is_data = 0;      // First row is features
        }
        rewind(input_file);
        printf("First row is data: %d\n", first_row_is_data);
        printf("==========================================================\n");

        
        // The data structure is an array of Coordinates (row #, lat, lon)
        array2 = malloc (row_count * sizeof(Coordinate));
        

        // If 1st row is data, we start parsing from the 1st row. If not, we start from the 2nd row.
        if (first_row_is_data) {
            fgets(buffer, 1024, input_file);    // Go to 2nd line if 1st row is data
        }

        // Parse data
        int first_col_to_read, last_col_to_read, lat_first;
        if (lat_col > lon_col) {
            last_col_to_read = lat_col;
            first_col_to_read = lon_col;
            lat_first = 0;
        } else {
            last_col_to_read = lon_col;
            first_col_to_read = lat_col;
            lat_first = 1;
        }

        //for (int i = 0; i < row_count; i++) {       // For each row
        for (int i = 0; i < 10; i++) {       // For 10 rows
            fgets(buffer, 1024, input_file);        // read line
            data = strtok(buffer, ",");             // Tokenize 1st field
            //printf("Data1: %s\n", data);
            for (int j = 0; j < first_col_to_read - 1; j++) {   // strtok number of times until first_col_to_read (alr strtok'd once)
                
                    //data = trim(data);
                    data = strtok(NULL, ",");       // to tokenize the same string to get the next token
                    //printf("Data2: %s\n", data);
                
            }   
            
            // strtok should now be on the right spot for the first lat/long value
            data = trim(data);
            data = strtok(NULL, ",");
            //printf("Lat/long: %s\n", data);
            if (lat_first) {
                array2[i].latitude = checkValidLatitude(data);
            } else {
                array2[i].longitude = checkValidLongitude(data);
            }
            array2[i].row_num = i+1;
            
            // get to 2nd value to read, but reading takes 1
            for (int j = 0; j < last_col_to_read - first_col_to_read - 1; j++) {    
                data = strtok(NULL, ",");
            }
            
            // strtok should now be on the right spot for the second lat/long value
            data = trim(data);
            data = strtok(NULL, ",");
            //printf("Lat/long: %s\n", data);
            if (!lat_first) {
                array2[i].latitude = checkValidLatitude(data);
            } else {
                array2[i].longitude = checkValidLongitude(data);
            }
            array2[i].row_num = i+1;
        }


        // Print first 10 rows data structure
        for (int i = 0; i < 10; i++) {
            printf("Row %d: %f, %f\n", array2[i].row_num, array2[i].latitude, array2[i].longitude);
        }


        fclose(input_file);

    } else if (input_method == 1) {

        double lat, lon;
        int arr_len;
        double temp_lat, temp_lon;

        // Prompt user for coordinates
        printf("How many coordinates do you want to enter?\n");
        scanf("%d", &arr_len);
        while (arr_len < 1) {
            printf("Please enter a number greater than 0:\n");
            scanf("%d", &arr_len);
        }
        array2 = malloc (arr_len * sizeof(struct Coordinate));
        arr_2_len = arr_len;

        int count = 0;
        int len = arr_len;
        while (len > 0) {
            printf("Please enter a latitude in decimal degrees:\n");
            scanf("%lf", &temp_lat);
            printf("Array 1: Please enter a longitude in decimal degrees:\n");
            scanf("%lf", &temp_lon);
            array2[count].row_num = count + 1;
            array2[count].latitude = temp_lat;
            array2[count].longitude = temp_lon;
            len--;
            count++;
        }
    }


    // For each location in array one, calculate the distance to each location in array two. Smallest distance wins. 
    for (int i = 0; i < arr_1_len; i++) {
        double minDist = 99999;     // 12,756 farthest distance on Earth
        int closest_location;
        for (int j = 0; j < arr_2_len; j++) {
            double dist = haversine(array1[i], array2[j]);
            if (dist < minDist) {
                //printf("%lf", dist);
                minDist = dist;
                closest_location = j;
            }
        }
        printf("Location %d in array 1 is closest to location %d in array 2\n", array1[i].row_num, array2[closest_location].row_num);
    }

    return 0;
}


