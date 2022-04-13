#include <algorithm>
#include "BruteForce.h"

vector<unsigned> BruteForce::getKnn(unsigned int id, unsigned int k) {
    Neighbors neighbors;
    for (int i = 0; i < rowLen; i++)
        if (i != id)
            neighbors.push_back(Neighbor(i, distance(dataSet[id], dataSet[i]), true));
    sort(neighbors.begin(), neighbors.end());
    vector<unsigned> knn;
    for (int i = 0; i < k; i++)
        knn.push_back(neighbors[i].id);
    return knn;
}
