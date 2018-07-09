#include <iostream>
#include <cmath>
#include <thread>
#include <array>

#include "Cluster.h"
#include "SampleMapper.h"
#include "Tools.h"

#define ITERATION_NUM 4

SampleMapper mapper;
Cluster clusters[K];

char *path;

void printLine(SampleMapper &mapper, int line) {
    for (int i = 0;i < line;i++) {
        Point point = mapper.getPoint(i);
        for (int j = 0;j < FEATURE_NUM;j++) {
            std::cout << point.vals[j] << " ";
        }
        std::cout << std::endl;
    }
}

void thread(unsigned int pieceIndex) {
    for (unsigned int offset = 0; offset < mapper.getSampleNumInPiece(); offset++) {
        Point p = mapper.getPoint(pieceIndex, offset);
        float minMax = clusters[0].center.getDistance(p);
        int minClusterIndex = 0;
        for (int f = 1; f < K; f++) {
            float tmp = clusters[f].center.getDistance(p);
            if (minMax > tmp) {
                minMax = tmp;
                minClusterIndex = f;
            }
        }
        // mutex
        clusters[minClusterIndex].addAPointSafe(p, minMax);
    }
}

int multipleThread() {
    // std::cout << "Hello, World!" << std::endl;

    // one point is 4KB
    // 20G = 524 2880 points

    // init random center
    for (auto &c : clusters) {
        c.initRandomCenter();
        c.report();
    }

    // map data file
    mapper.mapFile(path);

    // unsigned long sampleNum = mapper.getTotalSampleNum();

    // running iteration
    int iterNum = ITERATION_NUM;
    Timeval begin;
    double totalTime = 0.0;
    double duration;
    while(iterNum--) {
        begin = Tools::beginGetTime();
        printf("running %d iteration...\n", ITERATION_NUM - iterNum);
        // clear point in cluster
        for (auto &c : clusters) {
            c.clearCluster();
        }
        // traverse point in disk, find a min distance cluster

        std::thread threads[PIECE_NUM];
        for (int i = 0;i < PIECE_NUM;i++) {
            // one thread for one piece
            threads[i] = std::thread(thread, i); // pass by reference
        }

        for (int i = 0;i < PIECE_NUM; i++) {
            threads[i].join();
        }

        // calcate new center in each cluster
        for (auto &c : clusters) {
            c.flushNewCenter();
            c.report();
        }
        duration = Tools::endGetTime(begin);
        totalTime += duration;
        printf("iteration_%d used %.2lfs\n", ITERATION_NUM - iterNum, duration);
    }

    // sammary
    printf("average time of each iteration: %.2lfs\n", totalTime / ITERATION_NUM);

    return 0;
}



int singleThread() {
    // std::cout << "Hello, World!" << std::endl;

    // one point is 4KB
    // 20G = 524 2880 points

    // init random center
    Cluster clusters[K];
    for (auto &c : clusters) {
        c.initRandomCenter();
        c.report();
    }

    // map data file
    SampleMapper mapper;
    mapper.mapFile(path);

    // unsigned long sampleNum = mapper.getTotalSampleNum();

    // running iteration
    int iterNum = ITERATION_NUM;
    Timeval begin;
    double duration;
    double totalTime = 0.0;
    while(iterNum--) {
        begin = Tools::beginGetTime();
        printf("running %d iteration...\n", ITERATION_NUM - iterNum);
        // clear point in cluster
        for (auto &c : clusters) {
            c.clearCluster();
        }
        // traverse point in disk, find a min distance cluster
        Point p[PIECE_NUM];
        float dis[K];
        for (unsigned int offset = 0; offset < mapper.getSampleNumInPiece(); offset++) {
            for (unsigned int pieceIndex = 0; pieceIndex < PIECE_NUM; pieceIndex++) {
                p[pieceIndex] = mapper.getPoint(pieceIndex, offset);
            }
            for (auto &i : p) {
                // find min center
                dis[0] = clusters[0].center.getDistance(i);
                dis[1] = clusters[1].center.getDistance(i);
                dis[2] = clusters[2].center.getDistance(i);
                dis[3] = clusters[3].center.getDistance(i);
                dis[4] = clusters[4].center.getDistance(i);
                dis[5] = clusters[5].center.getDistance(i);
                dis[6] = clusters[6].center.getDistance(i);
                dis[7] = clusters[7].center.getDistance(i);
                dis[8] = clusters[8].center.getDistance(i);
                dis[9] = clusters[9].center.getDistance(i);
                float minMax = dis[0];
                int minClusterIndex = 0;
                for (int f = 1; f < K; f++) {
                    if (minMax > dis[f]) {
                        minMax = dis[f];
                        minClusterIndex = f;
                    }
                }
                // found min cluster
                clusters[minClusterIndex].addAPoint(i, minMax);
            }
        }

        // calcate new center in each cluster
        for (auto &c : clusters) {
            c.flushNewCenter();
            c.report();
        }
        duration = Tools::endGetTime(begin);
        totalTime += duration;
        printf("iteration_%d used %.2lfs\n", ITERATION_NUM - iterNum, duration);
    }

    printf("average time of each iteration: %.2lfs\n", totalTime / ITERATION_NUM);

    return 0;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("usage: kmeans [path]\n");
        return  0;
    }
    path = argv[1];

    // singleThread();

    multipleThread();

    return 0;
}