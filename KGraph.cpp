#include<boost/dynamic_bitset.hpp>

#include "KGraph.h"
#include "Utils.h"

using namespace boost;

void KGraph::init() {
    vector<unsigned> random(k);
    for (unsigned id = 0; id < rowLen; id++) {
        getRandomArray(&random[0], k, rowLen - 1);
        graph[id].resize(k);
        for (unsigned j = 0; j < k; j++) {
            if (random[j] >= id)
                random[j]++;
            graph[id][j].id = random[j];
            graph[id][j].dist = distance(dataSet[id], dataSet[random[j]]);
            graph[id][j].flag = true;
        }
        sort(graph[id].begin(), graph[id].end());
    }
}

void KGraph::getKnnG(KnnGraph &knnGraph, bool flag, int sample_num) {
    knnGraph.clear();
    knnGraph.resize(rowLen);
    for (unsigned id = 0; id < rowLen; id++) {
        vector<unsigned> random;
        for (unsigned i = 0; i < k; i++)
            random.push_back(i);
        if (graph[id].size() > sample_num)
            random_shuffle(random.begin(), random.end());

        int selectedNum = 0;
        for (unsigned i = 0; i < sample_num; i++)
            if (graph[id][random[i]].flag == flag) {
                knnGraph[id].push_back(graph[id][random[i]].id);
                selectedNum++;
                graph[id][random[i]].flag = false;
            }
    }
}

void KGraph::reverseGraph(const KnnGraph &knnGraph, KnnGraph &rKnnGraph) {
    rKnnGraph.clear();
    rKnnGraph.resize(rowLen);
    for (unsigned id = 0; id < knnGraph.size(); id++)
        for (unsigned u: knnGraph[id])
            rKnnGraph[u].push_back(id);
}

bool KGraph::update(unsigned int id, Neighbor neighbor) {
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

int KGraph::join() {
    int changeNum = 0;
    int sampleNum = k * sampleRate;
    getKnnG(knnOld, false, k);
    getKnnG(knnNew, true, sampleNum);
    reverseGraph(knnOld, rKnnOld);
    reverseGraph(knnNew, rKnnNew);

    for (int i = 0; i < rowLen; i++) {
        vector<unsigned> oldNeighbors, newNeighbors;
        dynamic_bitset<> oldMap(rowLen), newMap(rowLen);
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

void KGraph::construct() {
    graph.resize(rowLen);
    init();
    for (unsigned times = 0; times < iterations; times++) {
        printf("%d\n", times);
        if (join() < delta * k * rowLen) break;
    }
}

void KGraph::setIterations(unsigned int it) {
    iterations = it;
}

void KGraph::setSampleRate(float f) {
    sampleRate = f;
}

void KGraph::setDelta(float d) {
    delta = d;
}

KnnGraph KGraph::getKnnGraph(vector<unsigned> dataId) {
    knnNew.clear();
    for (unsigned id:dataId) {
        Neighbors neighbors = graph[id];
        vector<unsigned> edges;
        for (Neighbor neighbor: neighbors)
            edges.push_back(neighbor.id);
        knnNew.push_back(edges);
    }
    return knnNew;
}
