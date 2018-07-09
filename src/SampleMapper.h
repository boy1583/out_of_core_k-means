

#ifndef KMEANS_SAMPLEMAPPER_H
#define KMEANS_SAMPLEMAPPER_H

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include "Common.h"
#include "Point.h"

// use mmap to read huge sample file
class SampleMapper {
private:
    float *map[PIECE_NUM];
    unsigned long fileSize;
    unsigned long sampleSize;
    unsigned long pieceSize; // one piece size in byte
    unsigned long sampleInPiece; // sample num in one piece

public:
    SampleMapper() : map{0}, fileSize(0) {}

    ~SampleMapper() {
        for (int i = 0;i < PIECE_NUM;i++) {
            if (map[i]) {
                munmap(map[i], pieceSize);
            }
        }
    }

    bool mapFile(const char* path) {
        int fd = open(path, O_RDONLY);
        FILE* f = fdopen(fd,"r");
        fseek(f, 0, SEEK_END);
        fileSize = ftell(f);
        close(fd);
        sampleSize = fileSize / sizeof(float) / FEATURE_NUM;
        if (sampleSize % PIECE_NUM) {
            std::cout << "error in file size" << std::endl;
            return false;
        }
        std::cout << "file size: " << fileSize << std::endl;
        pieceSize = fileSize / PIECE_NUM;
        sampleInPiece = sampleSize / PIECE_NUM;
        for (int i = 0; i < PIECE_NUM; i++) {
            int fileHandler = open(path, O_RDONLY);
            // 将文件划分为64块
            map[i] = (float*)mmap(NULL, pieceSize, PROT_READ, MAP_PRIVATE, fileHandler, (pieceSize * i));
            if (map == MAP_FAILED) {
                perror("mmap error");
                return false;
            }
        }
        return true;
    }

    // get index point
    Point getPoint(unsigned int index) {
        // 计算分片
        unsigned int pieceIndex = index / sampleInPiece;
        unsigned int pieceOffset = index % sampleInPiece;
        pieceOffset *= FEATURE_NUM;
        // index * 1KB
        return getPoint(pieceIndex, pieceOffset);
    }

    Point getPoint(unsigned int pieceIndex, unsigned int pieceOffset) {
        Point point{};
        for (unsigned int i = 0 ;i < FEATURE_NUM;i++) {
            point.setValue(i, map[pieceIndex][pieceOffset + i]);
        }
        return point;
    }

    unsigned long getFileSize() {
        return fileSize;
    }

    unsigned long getTotalSampleNum() {
        return sampleSize;
    }

    unsigned long getSampleNumInPiece() {
        return sampleInPiece;
    }

};


#endif //KMEANS_SAMPLEMAPPER_H
