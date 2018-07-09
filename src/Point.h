

#ifndef KMEANS_POINT_H
#define KMEANS_POINT_H

#include "Common.h"

class Point {
public:
    float vals[FEATURE_NUM];

public:
    /**!
     * calcate distance between two points
     * @param point
     * @return
     */
    float getDistance(Point &point);

    /**!
     * set index feature value
     * @param index
     * @param val
     */
    void setValue(unsigned int index, float val);

    /**!
     * clear value, set all val to zero
     */
    void clear();
};


#endif //KMEANS_POINT_H
