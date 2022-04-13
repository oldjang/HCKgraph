#ifndef HCK_DATA_H
#define HCK_DATA_H
#include <vector>
#include <functional>

using namespace std;

typedef vector<float> Vector;
typedef vector<Vector> DataSet;
typedef vector<unsigned > Cluster;

typedef function<float(Vector const &, Vector const &)> MetricType;

struct Neighbor {
    uint32_t id;
    float dist;
    bool flag;  // whether this entry is a newly found one
    Neighbor() {}
    Neighbor(unsigned i, float d, bool f = true) : id(i), dist(d), flag(f) {}
};

static inline bool operator<(Neighbor const &n1, Neighbor const &n2) {
    return n1.dist < n2.dist;
}

typedef vector<Neighbor> Neighbors;

typedef vector<vector<unsigned >> KnnGraph;

#endif //HCK_DATA_H
