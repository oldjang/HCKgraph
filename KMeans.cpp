#include <boost/dynamic_bitset.hpp>

#include "KMeans.h"
#include "Utils.h"


void KMeans::init(unsigned int k, vector<unsigned> &dataId, unsigned int dataSize) {
    centroids.resize(k);
    int initId = getRandomInt(0, (int)dataSize - 1);
    centroids[0] = dataSet[dataId[initId]];

    int initId2 = getRandomInt(0, (int)dataSize - 1);
    centroids[1] = dataSet[dataId[initId2]];
//    boost::dynamic_bitset<> selected(dataSize);
//    dis.resize(dataSize);
//    vector<float> p(dataSize);
//    selected[initId] = true;
//    for (int id = 0; id < dataSize; id++) {
//        dis[id] = dist(centroids[0], dataSet[dataId[id]]);
//    }
//    for (int times = 1; times < k; times++) {
//        for (int id = 0; id < dataSize; id++) {
//            if (!selected[id])
//                p[id] = dis[id] * dis[id];
//            else p[id] = 0;
//            if (id > 0)
//                p[id] += p[id - 1];
//        }
//        double sum = p[dataSize - 1];
//        double randNum = getRandomReal();
//        int selectedId = -1;
//        for (int id = 0; id < dataSize; id++)
//            if (!selected[id]) {
//                if (p[id] / sum >= randNum) {
//                    selectedId = id;
//                    break;
//                }
//            }
//        selected[selectedId] = true;
//        centroids[times] = dataSet[dataId[selectedId]];
//        for (int id = 0; id < dataSize; id++)
//            if (!selected[id]) {
//                dis[id] = min(dis[id], dist(centroids[0], dataSet[dataId[id]]));
//            }
//    }

}

void KMeans::construct(unsigned int k, vector<unsigned> &dataId, unsigned dataSize) {
    belong.resize(dataSize);
    init(k, dataId, dataSize);
    bool flag = true;
    int times = 0;
    while (flag) {
        times++;
        flag = false;
        clusterNum.resize(k, 0);
        for (unsigned id = 0; id < k; id++)
            clusterNum[id] = 0;
        for (unsigned id = 0; id < dataSize; id++) {
            int minIndex = -1;
            float minDist = INT_MAX;
            for (int j = 0; j < k; j++) {
                float nowDist = dist(centroids[j], dataSet[dataId[id]]);
                if (minIndex == -1 || nowDist < minDist) {
                    minDist = nowDist;
                    minIndex = j;
                }
            }
            if (belong[id] != minIndex) {
                flag = true;
                belong[id] = minIndex;
            }
            clusterNum[minIndex]++;
        }

        for (unsigned id = 0; id < k; id++)
            for (unsigned c = 0; c < colLen; c++)
                centroids[id][c] = 0;
        for (int id = 0; id < dataSize; id++)
            for (unsigned c = 0; c < colLen; c++)
                centroids[belong[id]][c] += dataSet[dataId[id]][c];

        for (unsigned id = 0; id < k; id++)
            for (unsigned c = 0; c < colLen; c++)
                if (clusterNum[id] != 0)
                    centroids[id][c] /= clusterNum[id];
//        printf("%d\n", times); //test
        if (times > 2) break;
    }

    clusters.clear();
    clusters.resize(k);
    for (int id = 0; id < dataSize; id++) {
        clusters[belong[id]].push_back(dataId[id]);
//        assert(dataId[id]<=9999);
    }
}

DataSet KMeans::getCentroids() {
    return centroids;
}

Vector KMeans::getCentroid(unsigned int id) {
    return centroids[id];
}

unsigned KMeans::getBelong(unsigned int id) {
    return belong[id];
}

Cluster KMeans::getCluster(unsigned int id) {
    return clusters[id];
}
