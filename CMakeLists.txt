cmake_minimum_required(VERSION 3.8)
project(kmeans)

set(CMAKE_CXX_STANDARD 11)

add_executable(gen_data ./test/gen_data.cpp)

add_executable(kmeans ./src/main.cpp ./src/Cluster.cpp ./src/Point.cpp ./src/SampleMapper.cpp ./src/Tools.cpp)