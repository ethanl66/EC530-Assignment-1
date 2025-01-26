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
    // Degrees minutes seconds 32° 18' 23.1" N 122° 36' 52.5" W
    // Decimal minutes 32° 18.385' N 122° 36.875' W
    // Decimal degrees 32.30642° N 122.61458° W or +32.30642, -122.61458
    // Cardinal directions or signs
    value = trim(value);
    printf("checkCoordinate: %s --> ", value);

    double lat;
    char dir;
    int degs, mins;
    double secs;

    // Check decimal degrees format
    if (sscanf(value, "%lf %c", &lat, &dir) == 2) {
        if ((dir == 'N' || dir == 'S') && lat >= -90.0 && lat <= 90.0) {
            printf("Valid latitude (%f)\n", lat);
            return lat;   // Valid latitude
        }
    }

    // Check degrees minutes seconds format
    if (sscanf(value, "%d° %d' %lf\" %c", &degs, &mins, &secs, &dir) == 4) {
        lat = DMStoDD(degs, mins, secs, dir);
        if (lat >= -90.0 && lat <= 90.0) {
            printf("Valid latitude (%f)\n", lat);
            return lat;   // Valid latitude
        }
    }

    // Check decimal minutes format
    if (sscanf(value, "%d° %lf' %c", &degs, &lat, &dir) == 3) {
        lat = DDMtoDD(degs, lat, dir);
        if (lat >= -90.0 && lat <= 90.0) {
            printf("Valid latitude (%f)\n", lat);
            return lat;   // Valid latitude
        }
    }
    
    printf("Invalid coordinate\n");
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
    printf("checkCoordinate: %s --> ", value);

    double lat;
    char dir;
    int degs, mins;
    double secs;

    // Check decimal degrees format
    if (sscanf(value, "%lf %c", &lat, &dir) == 2) {
        if ((dir == 'E' || dir == 'W' || dir == 'e' || dir == 'w') && lat >= -180.0 && lat <= 180.0) {
            printf("Valid longitude (%f)\n", lat);
            return lat;   // Valid longitude
        }
    }

    // Check degrees minutes seconds format
    if (sscanf(value, "%d° %d' %lf\" %c", &degs, &mins, &secs, &dir) == 4) {
        lat = DMStoDD(degs, mins, secs, dir);
        if (lat >= -90.0 && lat <= 90.0) {
            printf("Valid latitude (%f)\n", lat);
            return lat;   // Valid latitude
        }
    }

    // Check decimal minutes format
    if (sscanf(value, "%d° %lf' %c", &degs, &lat, &dir) == 3) {
        lat = DDMtoDD(degs, lat, dir);
        if (lat >= -90.0 && lat <= 90.0) {
            printf("Valid latitude (%f)\n", lat);
            return lat;   // Valid latitude
        }
    }
    
    printf("Invalid coordinate\n");
    return 200;   // Invalid coordinate
}

#endif // GPS_H