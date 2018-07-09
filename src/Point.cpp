

#include <cmath>
#include <algorithm>
#include "Point.h"

float Point::getDistance(Point &point) {
    float tmp = 0.0;
    for (int i = 0;i < FEATURE_NUM;i += 8) {
        // parall opt
        tmp += pow(point.vals[i] - vals[i], 2.0);
        tmp += pow(point.vals[i + 1] - vals[i + 1], 2.0);
        tmp += pow(point.vals[i + 2] - vals[i + 2], 2.0);
        tmp += pow(point.vals[i + 3] - vals[i + 3], 2.0);
        tmp += pow(point.vals[i + 4] - vals[i + 4], 2.0);
        tmp += pow(point.vals[i + 5] - vals[i + 5], 2.0);
        tmp += pow(point.vals[i + 6] - vals[i + 6], 2.0);
        tmp += pow(point.vals[i + 7] - vals[i + 7], 2.0);
    }
    return tmp;
}

void Point::setValue(unsigned int index, float val) {
    vals[index] = val;
}

void Point::clear() {
    std::fill_n(vals, FEATURE_NUM, 0.0);
}