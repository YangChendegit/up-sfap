#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <sys/stat.h>
#include <fstream>

#include <chrono>
#include <vector>

#include "Baseline.h"
#include "Ours.h"
#include "Abstract.h"
#include "MMap.h"

class BenchMark{
public:

    typedef std::vector<Abstract*> AbsVector;

    BenchMark(const char* _PATH)
            {
        }
    ~BenchMark(){
    }

    void TopKError(double alpha){
    }

private:

};

#endif
