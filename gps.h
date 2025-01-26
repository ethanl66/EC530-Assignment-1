#ifndef GPS_H
#define GPS_H

#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct Coordinate {
    double latitude;
    double longitude;
} Coordinate;


// Useful constants
#define EARTH_RADIUS_METERS 6371000 // meters
#define M_PI 3.14159265358979323846

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

double haversine(struct Coordinate geoLocationOne, struct Coordinate geoLocationTwo) {
    // Convert decimal degrees to radians
    double lat1 = geoLocationOne.latitude * M_PI / 180;
    double lon1 = geoLocationOne.longitude * M_PI / 180;
    double lat2 = geoLocationTwo.latitude * M_PI / 180;
    double lon2 = geoLocationTwo.longitude * M_PI / 180;

    // Calculate differences
    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    // Haversine formula
    double a = sin(dLat /2 ) * sin(dLat / 2) + sin(dLon / 2) * sin(dLon / 2) * cos(lat1) * cos(lat2);
    //double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double c = 2 * asin(sqrt(a));
    double d_m = EARTH_RADIUS_METERS * c; // meters
    double d_km = d_m / 1000; // kilometers

    return d_km;
}

// Checks if string is a valid coordinate. Returns decimal degree value if valid, else returns 200.
double checkCoordinate (char* data) {
    // Degrees minutes seconds 32° 18' 23.1" N 122° 36' 52.5" W
    // Decimal minutes 32° 18.385' N 122° 36.875' W
    // Decimal degrees 32.30642° N 122.61458° W or +32.30642, -122.61458
    // Cardinal directions or signs
    data = trim(data);
    printf("checkCoordinate: %s --> ", data);

    // Check for decimal degree format
    char* endptr;
    double value = strtod(data, &endptr);   // endptr is where the conversion stopped
    printf("%lf   ", value);

    if (endptr != data && (*endptr == '\0' || isspace((unsigned char) *endptr))) {    
        // if endptr == data, no conversion was performed
        
        // if *endptr == \0 or isspace, the entire string was converted
        if (value >= -180.0 && value <= 180.0) {
            printf("Valid decimal degree (%f, endptr: %s)\n", value, *endptr);
            return value;   // Valid longitude and potential latitude
        }
    }

    // Check for degrees minutes seconds format
    double degrees, minutes, seconds;
    char direction;

    if (sscanf(data, "%lf° %lf' %lf\" %c", &degrees, &minutes, &seconds, &direction) == 4 ||
        sscanf(data, "%lf° %lf' %c", &degrees, &minutes, &direction) == 3) {
        // Convert to decimal degrees
        double decimal_degrees = degrees + (minutes / 60.0) + (seconds / 3600.0);
        if (direction == 'S' || direction == 'W') {
            decimal_degrees *= -1;
        }
        if (decimal_degrees >= -180.0 && decimal_degrees <= 180.0) {
            printf("Valid degrees minutes seconds (%f)\n", decimal_degrees);
            return decimal_degrees;   // Valid longitude and potential latitude
        }
    }

    // Check for decimal minutes format
    if (sscanf(data, "%lf° %lf' %c", &degrees, &minutes, &direction) == 3) {
        // Convert to decimal degrees
        double decimal_degrees = degrees + (minutes / 60.0);
        if (direction == 'S' || direction == 'W') {
            decimal_degrees *= -1;
        }
        if (decimal_degrees >= -180.0 && decimal_degrees <= 180.0) {
            printf("Valid decimal minutes (%f)\n", decimal_degrees);
            return decimal_degrees;   // Valid longitude and potential latitude
        }
    }

    printf("Invalid coordinate\n");
    return 200;   // Invalid coordinate
}

int checkValidLat () {}
int checkValidLon () {}

#endif // GPS_H