#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <time.h>

using std::string;

#define PI 3.1415926535897932384626433832795028841968

inline float degreesToRad(float degree){
    return degree*PI/180.0;
}

inline int sign(float f) { return f >= 0 ? 1 : -1; }

inline
double random_double( double min, double max){
    double range = max - min;
    return min + (double(rand()) / double(RAND_MAX)) * range;
}

inline
double random_float( float min, float max){
    float range = max - min;
    return min + (float(rand()) / double(RAND_MAX)) * range;
}

inline
int random_int(int min,  int max)
{
    int range = max - min;
    return min + int((double(rand())/double(RAND_MAX)) * range);
}

#endif
