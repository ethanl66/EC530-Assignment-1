#ifndef GPS_H
#define GPS_H

#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct Coordinate {
    int row_num;
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
// ============ DD IS SHOWING UP WRONG! (CONVERSION OR WITHIN CHECKVALIDLATITUDE/LONGITUDE?)
double DMStoDD(int degrees, int minutes, double seconds, char direction) {
    double decimal = degrees + (minutes / 60.0) + (seconds / 3600.0);
    if (direction == 'S' || direction == 's' || direction == 'W' || direction == 'w') {
        decimal *= -1;
    }
    return decimal;
}

double DDMtoDD(int degrees, double decimal_minutes, char direction) {
    double decimal = degrees + (decimal_minutes / 60.0);
    if (direction == 'S' || direction == 's' || direction == 'W' || direction == 'w') {
        decimal *= -1;
    }
    return decimal;
}

// Checks if string is a valid latitude. Returns decimal degree value if valid, else returns 200.
double checkValidLatitude (char* value) {
    // Degrees minutes seconds 122° 36' 52.5" W
    // Decimal minutes 122° 36.875' W
    // Decimal degrees 122.61458° W or -122.61458
    // Cardinal directions or signs
    // ° or "degrees", ' or "minutes", " or "seconds"
    value = trim(value);
    printf("chkLat: %s --> ", value);

    double lat;
    char dir;
    char direction [5];
    double degs, mins, secs;
    char extra;

    // Check degrees minutes seconds format
    if (
        sscanf(value, "%d° %d' %lf\" %c%c", &degs, &mins, &secs, &dir, &extra) == 4 ||
        sscanf(value, "%d° %d' %lf\" %s%c", &degs, &mins, &secs, &direction, &extra) == 4 ||
        sscanf(value, "%d degrees %d minutes %lf seconds %c%c", &degs, &mins, &secs, &dir, &extra) == 4 ||
        sscanf(value, "%d degrees %d minutes %lf seconds %s%c", &degs, &mins, &secs, &direction, &extra) == 4 ||
        sscanf(value, "%d degs %d mins %lf secs %c%c", &degs, &mins, &secs, &dir, &extra) == 4 ||
        sscanf(value, "%d degs %d mins %lf secs %s%c", &degs, &mins, &secs, &direction, &extra) == 4 ||

        sscanf(value, "%d %d %lf %c%c", &degs, &mins, &secs, &dir, &extra) == 4 ||
        sscanf(value, "%d %d %lf %s%c", &degs, &mins, &secs, &direction, &extra) == 4 ||
        sscanf(value, "%d %d %lf%c", &degs, &mins, &secs, &extra) == 3
    ) {
        // Type format is right, now check content validity
        if (
            degs >= -90 && degs <= 90 && mins >= 0 && mins < 60 && secs >= 0 && secs < 60 &&
            (
                dir == 'N' || dir == 'S' || dir == 'n' || dir == 's' ||
                strcmp(direction, "North") == 0 || strcmp(direction, "South") == 0 ||
                strcmp(direction, "north") == 0 || strcmp(direction, "south") == 0 ||
                sscanf(value, "%d %d %lf", &degs, &mins, &secs) == 3
        )) {
            if (direction == "North" || direction == "south") {
                dir = 'N';
            }
            if (direction == "North" || direction == "south") {
                dir = 'S';
            }
            lat = DMStoDD(degs, mins, secs, dir);
            //("Valid longitude (%f)\n", lat);
            return lat;   // Valid longitude
        }
    }

    // Check decimal minutes format
    if (
        sscanf(value, "%d° %lf' %c%c", &degs, &mins, &dir, &extra) == 3 ||
        sscanf(value, "%d° %lf' %s%c", &degs, &mins, &direction, &extra) == 3 ||
        sscanf(value, "%d degrees %lf minutes %c%c", &degs, &mins, &dir, &extra) == 3 ||
        sscanf(value, "%d degrees %lf minutes %s%c", &degs, &mins, &direction, &extra) == 3 ||
        sscanf(value, "%d degs %lf mins %s%c", &degs, &mins, &direction, &extra) == 3 ||
        sscanf(value, "%d degs %lf mins %c%c", &degs, &mins, &dir, &extra) == 3 ||

        sscanf(value, "%d %lf %c%c", &degs, &mins, &dir, &extra) == 3 ||
        sscanf(value, "%d %lf %s%c", &degs, &mins, &direction, &extra) == 3 ||
        sscanf(value, "%d %lf%c", &degs, &mins, &extra) == 2
    ) {
        // Type format is right, now check content validity
        if (
            degs >= -90 && degs <= 90 && mins >= 0 && mins < 60 &&
            (
                dir == 'N' || dir == 'S' || dir == 'n' || dir == 's' ||
                strcmp(direction, "North") == 0 || strcmp(direction, "South") == 0 ||
                strcmp(direction, "north") == 0 || strcmp(direction, "south") == 0 ||
                sscanf(value, "%d %lf", &degs, &mins) == 2
        )) {
            if (direction == "North" || direction == "north") {
                dir = 'N';
            }
            if (direction == "South" || direction == "south") {
                dir = 'S';
            }
            lat = DDMtoDD(degs, mins, dir);
            //printf("Valid longitude (%f)\n", lat);
            return lat;   // Valid longitude
        }
    }

    // Check decimal degrees format
    if (
        sscanf(value, "%lf° %c%c", &degs, &dir, &extra) == 2 ||
        sscanf(value, "%lf° %s%c", &degs, &direction, &extra) == 2 ||
        sscanf(value, "%lf degrees %c%c", &degs, &dir, &extra) == 2 ||
        sscanf(value, "%lf degrees %s%c", &degs, &direction, &extra) == 2 ||
        sscanf(value, "%lf degs %s%c", &degs, &direction, &extra) == 2 ||
        sscanf(value, "%lf degs %c%c", &degs, &dir, &extra) == 2 ||

        sscanf(value, "%lf %c%c", &degs, &dir, &extra) == 2 ||
        sscanf(value, "%lf %s%c", &degs, &direction, &extra) == 2 ||
        sscanf(value, "%lf%c", &degs, &extra) == 1
    ) {
        // Type format is right, now check content validity
        if (
            degs >= -90 && degs <= 90 &&
            (
                dir == 'N' || dir == 'S' || dir == 'n' || dir == 's' ||
                strcmp(direction, "North") == 0 || strcmp(direction, "South") == 0 ||
                strcmp(direction, "north") == 0 || strcmp(direction, "south") == 0 ||
                sscanf(value, "%lf", &degs) == 1
        )) {
            if (direction == "South" || direction == "south") {
                dir = 'S';
            }
            if (dir == 'S' || dir == 's') {
                lat = -degs;
            }
            lat = degs;
            printf("Valid latitude (%f)\n", lat);
            return lat;   // Valid latitude
        }
    }

    //printf("Invalid coordinate\n");
    return 200;   // Invalid coordinate
}

// Checks if string is a valid longitude. Returns decimal degree value if valid, else returns 200.
double checkValidLongitude (char* value) {
    // Degrees minutes seconds 122° 36' 52.5" W
    // Decimal minutes 122° 36.875' W
    // Decimal degrees 122.61458° W or -122.61458
    // Cardinal directions or signs
    // ° or "degrees", ' or "minutes", " or "seconds"
    value = trim(value);
    printf("chkLon: %s --> ", value);

    double lon;
    char dir;
    char direction [5];
    double degs, mins, secs;
    char extra;

    // Check degrees minutes seconds format
    if (
        sscanf(value, "%d° %d' %lf\" %c%c", &degs, &mins, &secs, &dir, &extra) == 4 ||
        sscanf(value, "%d° %d' %lf\" %s%c", &degs, &mins, &secs, &direction, &extra) == 4 ||
        sscanf(value, "%d degrees %d minutes %lf seconds %c%c", &degs, &mins, &secs, &dir, &extra) == 4 ||
        sscanf(value, "%d degrees %d minutes %lf seconds %s%c", &degs, &mins, &secs, &direction, &extra) == 4 ||
        sscanf(value, "%d degs %d mins %lf secs %c%c", &degs, &mins, &secs, &dir, &extra) == 4 ||
        sscanf(value, "%d degs %d mins %lf secs %s%c", &degs, &mins, &secs, &direction, &extra) == 4 ||

        sscanf(value, "%d %d %lf %c%c", &degs, &mins, &secs, &dir, &extra) == 4 ||
        sscanf(value, "%d %d %lf %s%c", &degs, &mins, &secs, &direction, &extra) == 4 ||
        sscanf(value, "%d %d %lf%c", &degs, &mins, &secs, &extra) == 3
    ) {
        // Type format is right, now check content validity
        if (
            degs >= -180 && degs <= 180 && mins >= 0 && mins < 60 && secs >= 0 && secs < 60 &&
            (
                dir == 'E' || dir == 'W' || dir == 'e' || dir == 'w' ||
                strcmp(direction, "East") == 0 || strcmp(direction, "West") == 0 ||
                strcmp(direction, "east") == 0 || strcmp(direction, "west") == 0 ||
                sscanf(value, "%d %d %lf", &degs, &mins, &secs) == 3
        )) {
            if (direction == "East" || direction == "east") {
                dir = 'E';
            }
            if (direction == "West" || direction == "west") {
                dir = 'W';
            }
            lon = DMStoDD(degs, mins, secs, dir);
            //printf("Valid longitude (%f)\n", lon);
            return lon;   // Valid longitude
        }
    }

    // Check decimal minutes format
    if (
        sscanf(value, "%d° %lf' %c%c", &degs, &mins, &dir, &extra) == 3 ||
        sscanf(value, "%d° %lf' %s%c", &degs, &mins, &direction, &extra) == 3 ||
        sscanf(value, "%d degrees %lf minutes %c%c", &degs, &mins, &dir, &extra) == 3 ||
        sscanf(value, "%d degrees %lf minutes %s%c", &degs, &mins, &direction, &extra) == 3 ||
        sscanf(value, "%d degs %lf mins %s%c", &degs, &mins, &direction, &extra) == 3 ||
        sscanf(value, "%d degs %lf mins %c%c", &degs, &mins, &dir, &extra) == 3 ||

        sscanf(value, "%d %lf %c%c", &degs, &mins, &dir, &extra) == 3 ||
        sscanf(value, "%d %lf %s%c", &degs, &mins, &direction, &extra) == 3 ||
        sscanf(value, "%d %lf%c", &degs, &mins, &extra) == 2
    ) {
        // Type format is right, now check content validity
        if (
            degs >= -180 && degs <= 180 && mins >= 0 && mins < 60 &&
            (
                dir == 'E' || dir == 'W' || dir == 'e' || dir == 'w' ||
                strcmp(direction, "East") == 0 || strcmp(direction, "West") == 0 ||
                strcmp(direction, "east") == 0 || strcmp(direction, "west") == 0 ||
                sscanf(value, "%d %lf", &degs, &mins) == 2
        )) {
            if (direction == "East" || direction == "east") {
                dir = 'E';
            }
            if (direction == "West" || direction == "west") {
                dir = 'W';
            }
            lon = DDMtoDD(degs, mins, dir);
            //printf("Valid longitude (%f)\n", lon);
            return lon;   // Valid longitude
        }
    }

    // Check decimal degrees format
    if (
        sscanf(value, "%lf° %c%c", &degs, &dir, &extra) == 2 ||
        sscanf(value, "%lf° %s%c", &degs, &direction, &extra) == 2 ||
        sscanf(value, "%lf degrees %c%c", &degs, &dir, &extra) == 2 ||
        sscanf(value, "%lf degrees %s%c", &degs, &direction, &extra) == 2 ||
        sscanf(value, "%lf degs %s%c", &degs, &direction, &extra) == 2 ||
        sscanf(value, "%lf degs %c%c", &degs, &dir, &extra) == 2 ||

        sscanf(value, "%lf %c%c", &degs, &dir, &extra) == 2 ||
        sscanf(value, "%lf %s%c", &degs, &direction, &extra) == 2 ||
        sscanf(value, "%lf%c", &degs, &extra) == 1
    ) {
        // Type format is right, now check content validity
        if (
            degs >= -180 && degs <= 180 &&
            (
                dir == 'E' || dir == 'W' || dir == 'e' || dir == 'w' ||
                strcmp(direction, "East") == 0 || strcmp(direction, "West") == 0 ||
                strcmp(direction, "east") == 0 || strcmp(direction, "west") == 0 ||
                sscanf(value, "%lf", &degs) == 1
        )) {
            if (direction == "West" || direction == "west") {
                dir = 'W';
            }
            if (dir == 'W' || dir == 'w') {
                lon = -degs;
            }
            lon = degs;
            //printf("Valid longitude (%f)\n", lon);
            return lon;   // Valid longitude
        }
    }

    //printf("Invalid coordinate\n");
    return 200;   // Invalid coordinate
}

#endif // GPS_H