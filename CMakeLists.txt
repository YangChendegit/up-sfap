cmake_minimum_required(VERSION 3.14)
project(Periodic)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(Algorithm)
include_directories(common)
include_directories(datasets)
include_directories(Ours)

add_executable(main
        Algorithm/Abstract.h
        common/BitMap.h
        common/CuckooMap.h
        common/hash.h
        common/Heap.h
        common/MMap.h
        common/StreamSummary.h
        common/Util.h
        main.cpp)

