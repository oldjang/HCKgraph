#include "test.h"
#include <boost/dynamic_bitset.hpp>

using namespace boost;

float testRecall(KnnGraph knnGraph1, KnnGraph knnGraph2, unsigned rowLen, unsigned testNum, unsigned colLen) {
    float sum = (float) testNum * colLen, cnt = 0;
    for (int i = 0; i < testNum; i++) {
        dynamic_bitset<> flag(rowLen);
        for (int j = 0; j < colLen; j++)
            flag.set(knnGraph1[i][j]);
        for (int j = 0; j < colLen; j++)
            if (j == 0 || knnGraph2[i][j] != knnGraph2[i][j - 1])
                cnt += flag.test(knnGraph2[i][j]);
    }
    return cnt / sum;
}
