#include <math.h>
#include <stdio.h>

struct Coordinate {
    double latitude;
    double longitude;
};

// Useful constants
#define EARTH_RADIUS_METERS 6371000 // meters
#define M_PI 3.14159265358979323846

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

int main () {
    // Init arrays with geolocations in decimal degrees
    struct Coordinate geoLocationArrOne [] = { {0.012, 1.234}, {100.123, 1.423}, {0.537, 110.523} };    // Points A, B, C
    struct Coordinate geoLocationArrTwo [] = { {3.012, 7.234}, {0.537, 133.523}, {120.123, 7.423} };    // Points A', C', B'

/*  // Haversine formula test
    // test known example:  51.5007, 0.1246 to 40.6892, 74.0445
    struct Coordinate London = {51.5007, 0.1246};
    struct Coordinate NewYork = {40.6892, 74.0445};
    double dist = haversine(London, NewYork);
    printf("%f km\n", dist);    // Correct: 5574.84 km
*/

    // For each location in array one, calculate the distance to each location in array two. Smallest distance wins. 
    for (int i = 0; i < sizeof(geoLocationArrOne) / sizeof(geoLocationArrOne[0]); i++) {
        double minDist = 99999;     // 12,756 farthest distance on Earth
        int closest_location;
        for (int j = 0; j < sizeof(geoLocationArrTwo) / sizeof(geoLocationArrTwo[0]); j++) {
            double dist = haversine(geoLocationArrOne[i], geoLocationArrTwo[j]);
            if (dist < minDist) {
                minDist = dist;
                closest_location = j;
            }
        }
        printf("Location %d in array 1 is closest to location %d in array 2\n", i, closest_location);
    }
    return 0;
}