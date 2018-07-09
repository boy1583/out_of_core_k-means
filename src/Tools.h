#ifndef KMEANS_TOOLS_H
#define KMEANS_TOOLS_H

#include <sys/time.h>

typedef struct timeval Timeval;

class Tools {
public:
    static struct timeval beginGetTime() {
        Timeval start;
        gettimeofday(&start, NULL);
        return start;
    }

    static double endGetTime(const Timeval &beginAt) {
        Timeval end;
        gettimeofday( &end, NULL);
        long timeuse = 1000000 * ( end.tv_sec - beginAt.tv_sec ) + end.tv_usec - beginAt.tv_usec;
        double duration = timeuse / 1000.0 / 1000.0;
        return duration;
    }
};


#endif //KMEANS_TOOLS_H
