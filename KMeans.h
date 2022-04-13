#ifndef HCK_KMEANS_H
#define HCK_KMEANS_H

#include "Data.h"

using namespace std;

class KMeans {
private:
    DataSet const &dataSet;
    MetricType dist;
    unsigned rowLen;
    unsigned colLen;
    DataSet centroids;
    vector<unsigned> belong;
    vector<unsigned> clusterNum;
    vector<Cluster> clusters;
    vector<float> dis;

    void init(unsigned k, vector<unsigned> &dataId, unsigned dataSize);

public:
    KMeans(DataSet const &d, MetricType m, unsigned row, unsigned col) : dataSet(d), dist(m), rowLen(row),
                                                                         colLen(col) {}

    void construct(unsigned k, vector<unsigned> &dataId, unsigned dataSize);

    DataSet getCentroids();

    Vector getCentroid(unsigned id);

    unsigned getBelong(unsigned id);

    Cluster getCluster(unsigned id);
};


#endif //HCK_KMEANS_H
