#include <boost/dynamic_bitset.hpp>

#include "HCK.h"
#include "KMeans.h"
#include "Utils.h"

using namespace boost;

bool HCK::update(unsigned int id, Neighbor neighbor) {

    for (Neighbor n: graph[id])
        if (n.id == neighbor.id)
            return false;
    if (graph[id][k - 1].dist <= neighbor.dist)
        return false;
    int now = k - 1;

    while (now > 0) {
        if (graph[id][now - 1].dist > neighbor.dist) {
            graph[id][now] = graph[id][now - 1];
            now--;
        } else break;
    }
    graph[id][now] = neighbor;
    return true;
}

void HCK::divideDataSet(vector<unsigned int> &dataId, int depth, int belongId) {

    int dataSize = dataId.size();
    if (dataSize == 0)return;
    maxId = max(maxId, belongId);
    if (depth == mxDepth) {
        for (unsigned id = 0; id < dataSize; id++)
            belong[dataId[id]] = belongId;
        return;
    }

    kMeans->construct(2, dataId, dataId.size());

    vector<unsigned> dataId0 = kMeans->getCluster(0);
    vector<unsigned> dataId1 = kMeans->getCluster(1);


    divideDataSet(dataId0, depth + 1, belongId << 1);
    divideDataSet(dataId1, depth + 1, (belongId << 1) | 1);
}


void HCK::getInitNeighbor() {
    nodeSet.resize(maxId + 1);
    vector<int> setId(rowLen);
    for (int i = 0; i < rowLen; i++)
        setId[i] = i;
    random_shuffle(setId.begin(), setId.end());
    for (int i = 0; i < rowLen; i++) {
        int id = setId[i];
        graph[id].resize(k);
        int cnt = 0;
        int nowBelongId = belong[id];
        int res = min((int) k, i);
        vector<unsigned> &nowSet = nodeSet[nowBelongId];
        unsigned nowSetNum = min((unsigned) res, nowSet.size());
        int ts = max((int) k / mxDepth, 1);

        vector<unsigned> random(nowSetNum);
        while (res > 0 && nowBelongId > 1) {
            nowSet = nodeSet[nowBelongId];
            if (nowSet.size() > 0) {
                nowSetNum = min((unsigned) res, nowSet.size());
                nowSetNum = min(nowSetNum, (unsigned) ts);
                random.resize(nowSetNum);
                getRandomArray(&random[0], nowSetNum, nowSet.size());
                for (unsigned nowId: random)
                    graph[id][cnt++] = Neighbor(nowSet[nowId], distance(dataSet[nowSet[nowId]], dataSet[id]), true);
                res -= nowSetNum;
            }
            nowBelongId >>= 1;
        }
//        int re = max((int) k - (ts * 7), 0);
//        nowSetNum = min(nowSetNum, 3 * (unsigned) ts + min(re, 1));
//        vector<unsigned> random(nowSetNum);
//        if (nowSetNum > 0) {
//            getRandomArray(&random[0], nowSetNum, nowSet.size());
//            for (unsigned nowId: random)
//                graph[id][cnt++] = Neighbor(nowSet[nowId], distance(dataSet[nowSet[nowId]], dataSet[id]), true);
//            res -= nowSetNum;
//            if (nowSetNum > 3 * ts)re--;
//        } else re++;
//
//        while (res > 0 && nowBelongId > 1) {
//            nowBelongId ^= 1;
//            nowSet = nodeSet[nowBelongId];
//            if (nowSet.size() > 0) {
//                nowSetNum = min((unsigned) res, nowSet.size());
//                nowSetNum = min(nowSetNum, (unsigned) ts + min(re, 1));
//                random.resize(nowSetNum);
//                getRandomArray(&random[0], nowSetNum, nowSet.size());
//                for (unsigned nowId: random)
//                    graph[id][cnt++] = Neighbor(nowSet[nowId], distance(dataSet[nowSet[nowId]], dataSet[id]), true);
//                res -= nowSetNum;
//                if (nowSetNum > ts)re--;
//            } else re++;
//            nowBelongId >>= 1;
//        }

        nowBelongId = belong[id];
        while (nowBelongId > 0) {
            nodeSet[nowBelongId].push_back(id);
            nowBelongId >>= 1;
        }
        if (cnt < k) {
            int num = k - cnt;
            vector<unsigned> addedId(num);
            getRandomArray(&addedId[0], num, rowLen - i - 1);
            for (unsigned id1: addedId)
                graph[id][cnt++] = Neighbor(setId[id1 + i + 1], distance(dataSet[setId[id1 + i + 1]], dataSet[id]));
        }
        sort(graph[id].begin(), graph[id].end());
    }
}


void HCK::init() {
    nodeNum = rowLen;
    kMeans = new KMeans(dataSet, distance, rowLen, colLen);
    vector<unsigned> dataId(rowLen);
    for (int id = 0; id < rowLen; id++)
        dataId[id] = id;
    divideDataSet(dataId, 0, 1);
    getInitNeighbor();
}

void HCK::getKnnG(KnnGraph &knnGraph, bool flag, int sample_num) {
    knnGraph.clear();
    knnGraph.resize(nodeNum);
    for (unsigned id = 0; id < nodeNum; id++) {
        vector<unsigned> random;
        for (unsigned i = 0; i < graph[id].size(); i++)
            random.push_back(i);
        if (graph[id].size() > sample_num)
            random_shuffle(random.begin(), random.end());

        int selectedNum = 0;
        int mxNum = min(sample_num, (int) graph[id].size());
        for (unsigned i = 0; i < mxNum; i++)
            if (graph[id][random[i]].flag == flag) {
                knnGraph[id].push_back(graph[id][random[i]].id);
                selectedNum++;
                graph[id][random[i]].flag = false;
            }
    }
}

void HCK::reverseGraph(const KnnGraph &knnGraph, KnnGraph &rKnnGraph) {
    rKnnGraph.clear();
    rKnnGraph.resize(nodeNum);
    for (unsigned id = 0; id < knnGraph.size(); id++)
        for (unsigned u: knnGraph[id])
            rKnnGraph[u].push_back(id);
}


int HCK::join() {
    int changeNum = 0;
    int sampleNum = k * sampleRate;
    getKnnG(knnOld, false, k);
    getKnnG(knnNew, true, sampleNum);
    reverseGraph(knnOld, rKnnOld);
    reverseGraph(knnNew, rKnnNew);

    for (unsigned i = 0; i < rowLen; i++) {
        vector<unsigned> oldNeighbors, newNeighbors;
        dynamic_bitset<> oldMap(nodeNum), newMap(nodeNum);
        oldMap.reset();
        newMap.reset();

        for (unsigned id: knnOld[i]) {
            oldMap.set(id);
            oldNeighbors.push_back(id);
        }
        if (sampleNum < rKnnOld[i].size()) {
            random_shuffle(rKnnOld[i].begin(), rKnnOld[i].end());
            rKnnOld[i].resize(sampleNum);
        }
        for (unsigned id: rKnnOld[i])
            if (!oldMap.test(id)) {
                oldMap.set(id);
                oldNeighbors.push_back(id);
            }

        for (unsigned id: knnNew[i]) {
            newMap.set(id);
            newNeighbors.push_back(id);
        }
        if (sampleNum < rKnnNew[i].size()) {
            random_shuffle(rKnnNew[i].begin(), rKnnNew[i].end());
            rKnnNew[i].resize(sampleNum);
        }
        for (unsigned id: rKnnNew[i])
            if (!newMap.test(id)) {
                newMap.set(id);
                newNeighbors.push_back(id);
            }

        for (unsigned u1: newNeighbors) {
            for (unsigned u2: newNeighbors)
                if (u1 < u2) {
                    float dist = distance(dataSet[u1], dataSet[u2]);
                    changeNum += update(u1, Neighbor(u2, dist, true));
                    changeNum += update(u2, Neighbor(u1, dist, true));
                }
            for (unsigned u2: oldNeighbors)
                if (u1 != u2) {
                    float dist = distance(dataSet[u1], dataSet[u2]);
                    changeNum += update(u1, Neighbor(u2, dist, true));
                    changeNum += update(u2, Neighbor(u1, dist, true));
                }
        }
    }
    return changeNum;
}

void HCK::construct() {
    graph.resize(rowLen);
    belong.resize(rowLen);
    nodeDepth.resize(rowLen);
    init();
    for (unsigned times = 0; times < iterations; times++) {
        if (join() < delta * k * rowLen) break;
    }
}


void HCK::setIterations(unsigned int it) {
    iterations = it;
}

void HCK::setSampleRate(float f) {
    sampleRate = f;
}

void HCK::setDelta(float d) {
    delta = d;
}

KnnGraph HCK::getKnnGraph(vector<unsigned> dataId) {
    knnNew.clear();
    for (unsigned id: dataId) {
        Neighbors neighbors = graph[id];
        vector<unsigned> edges;
        for (Neighbor neighbor: neighbors)
            edges.push_back(neighbor.id);
        edges.resize(k);
        knnNew.push_back(edges);
    }
    return knnNew;
}

void HCK::setMaxDepth(unsigned int depth) {
    mxDepth = depth;
}
