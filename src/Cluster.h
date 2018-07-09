

#ifndef KMEANS_CLUSTER_H
#define KMEANS_CLUSTER_H

#include <cstdlib>
#include <algorithm>
#include <iostream>
#include "Common.h"
#include "Point.h"

// cluster maintain center point info
class Cluster {
public:
    static unsigned int ClusterIndex;

    unsigned int clusterIndex;

    Point center; // center point

    unsigned int pointNum; // number of point belong to this cluster

    double totalDistances[FEATURE_NUM]; // all point belonging to this cluster total distance

    double totalDistance;

    std::mutex mtx;

public:
    Cluster() : pointNum(0) {
        clusterIndex = ClusterIndex++;
    }

    /**!
     * init cluster center with random value
     */
    void initRandomCenter() {
        for (int i =0 ;i < FEATURE_NUM;i++) {
            center.setValue(i, ((double)(rand())/RAND_MAX * 1000.0));
        }
    }

    /**!
     * add a new point to cluster
     * @param point
     */
    void addAPoint(Point &point, double dis) {
        for (int i = 0;i < FEATURE_NUM;i += 8) {
            totalDistances[i] += point.vals[i];
            totalDistances[i + 1] += point.vals[i + 1];
            totalDistances[i + 2] += point.vals[i + 2];
            totalDistances[i + 3] += point.vals[i + 3];
            totalDistances[i + 4] += point.vals[i + 4];
            totalDistances[i + 5] += point.vals[i + 5];
            totalDistances[i + 6] += point.vals[i + 6];
            totalDistances[i + 7] += point.vals[i + 7];
        }
        pointNum++;
        totalDistance += dis;
    }

    /**!
     * thread safely
     * @param point
     * @param dis
     */
    void addAPointSafe(Point &point, double dis) {
        if (mtx.try_lock()) {   // only increase if currently not locked:
            for (int i = 0;i < FEATURE_NUM;i += 8) {
                totalDistances[i] += point.vals[i];
                totalDistances[i + 1] += point.vals[i + 1];
                totalDistances[i + 2] += point.vals[i + 2];
                totalDistances[i + 3] += point.vals[i + 3];
                totalDistances[i + 4] += point.vals[i + 4];
                totalDistances[i + 5] += point.vals[i + 5];
                totalDistances[i + 6] += point.vals[i + 6];
                totalDistances[i + 7] += point.vals[i + 7];
            }
            pointNum++;
            totalDistance += dis;
            mtx.unlock();
        }
    }


    /**!
     * clacate new center with total point sum and point count
     */
    void flushNewCenter() {
        for (int i = 0;i < FEATURE_NUM;i += 8) {
            center.setValue(i, totalDistances[i] / pointNum);
            center.setValue(i + 1, totalDistances[i + 1] / pointNum);
            center.setValue(i + 2, totalDistances[i + 2] / pointNum);
            center.setValue(i + 3, totalDistances[i + 3] / pointNum);
            center.setValue(i + 4, totalDistances[i + 4] / pointNum);
            center.setValue(i + 5, totalDistances[i + 5] / pointNum);
            center.setValue(i + 6, totalDistances[i + 6] / pointNum);
            center.setValue(i + 7, totalDistances[i + 7] / pointNum);
        }
    }

    /**!
     * print info
     */
    void report() {
        printf("cluster_%d point num:%d center distance:%.2lf\n",
            clusterIndex, pointNum, totalDistance);
    }

    /**!
     * clear cluster with zero value
     */
    void clearCluster() {
        pointNum = 0;
        totalDistance = 0;
        std::fill_n(totalDistances, FEATURE_NUM, 0.0);
    }
};


#endif //KMEANS_CLUSTER_H
