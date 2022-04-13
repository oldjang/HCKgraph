#ifndef HCK_HCK_H
#define HCK_HCK_H

#include "Data.h"
#include "KMeans.h"

class HCK {
public:
    vector<Neighbors> graph;
    DataSet &dataSet;
    MetricType distance;
    unsigned int k;
    unsigned int rowLen;
    unsigned int colLen;
    vector<vector<unsigned> >nodeSet;
    vector<int> nodeDepth;

    unsigned int iterations = 100;
    float sampleRate = 1;
    float delta = 0;

    KnnGraph knnOld;
    KnnGraph knnNew;
    KnnGraph rKnnOld;
    KnnGraph rKnnNew;
    KMeans *kMeans;

    unsigned int minSize = 30;
    unsigned int nodeNum;
    int maxId = 0;
    int mxDepth = 10;

    void init();
    void divideDataSet(vector<unsigned> &dataId,int depth,int belongId);
    void getKnnG(KnnGraph &knnGraph,bool flag,int sample_num);
    void reverseGraph(KnnGraph const &knnGraph,KnnGraph &rKnnGraph);
    bool update(unsigned id,Neighbor neighbor);
    void getInitNeighbor();


public:
    vector<int> belong;
    int join();
    HCK(DataSet &d,unsigned K,unsigned r,unsigned c,MetricType dist):dataSet(d),k(K),rowLen(r),colLen(c),distance(dist){}
    void setIterations(unsigned it);
    void setSampleRate(float f);
    void setDelta(float d);
    void setMaxDepth(unsigned depth);
    void construct();
    KnnGraph getKnnGraph(vector<unsigned> dataId);

};


#endif //HCK_HCK_H
