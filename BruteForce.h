#ifndef HCK_BRUTEFORCE_H
#define HCK_BRUTEFORCE_H

#include "Data.h"

class BruteForce {
private:
    DataSet const &dataSet;
    MetricType distance;
    unsigned rowLen;
    unsigned colLen;

public:
    BruteForce(DataSet &d,unsigned r,unsigned c,MetricType m):dataSet(d),rowLen(r),colLen(c),distance(m){}
    vector<unsigned > getKnn(unsigned id,unsigned k);
};


#endif //HCK_BRUTEFORCE_H
