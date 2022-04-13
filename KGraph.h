#ifndef HCK_KGRAPH_H
#define HCK_KGRAPH_H
#include "Data.h"

class KGraph {
private:
    vector<Neighbors> graph;
    DataSet const &dataSet;
    MetricType distance;
    unsigned int k;
    unsigned int rowLen;
    unsigned int colLen;
    unsigned int iterations = 100;
    float sampleRate = 1;
    float delta = 0;

    KnnGraph knnOld;
    KnnGraph knnNew;
    KnnGraph rKnnOld;
    KnnGraph rKnnNew;

    void init();
    void getKnnG(KnnGraph &knnGraph,bool flag,int sample_num);
    void reverseGraph(KnnGraph const &knnGraph,KnnGraph &rKnnGraph);
    bool update(unsigned id,Neighbor neighbor);

public:
    KGraph(DataSet const &d,unsigned K,unsigned r,unsigned c,MetricType dist):dataSet(d),k(K),rowLen(r),colLen(c),distance(dist){}
    void setIterations(unsigned it);
    void setSampleRate(float f);
    void setDelta(float d);
    void construct();
    KnnGraph getKnnGraph(vector<unsigned> dataId);
    int join();
};


#endif //HCK_KGRAPH_H
