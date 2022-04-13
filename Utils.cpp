#include <fstream>

#include "Utils.h"

double getRandomReal() {
    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}

unsigned getRandomInt(int lowerBound, int upperBound) {
    mt19937 rng(random_device{}());
    uniform_int_distribution<unsigned> dist(lowerBound, upperBound);
    return dist(rng);
}

void getRandomArray(unsigned *addr, unsigned size, unsigned N) {
    if (N == size) {
        for (unsigned i = 0; i < size; ++i) {
            addr[i] = i;
        }
        return;
    }
    for (unsigned i = 0; i < size; ++i) {
        addr[i] = getRandomInt(0, N - size - 1);
    }
    sort(addr, addr + size);
    for (unsigned i = 1; i < size; ++i) {
        if (addr[i] <= addr[i - 1]) {
            addr[i] = addr[i - 1] + 1;
        }
    }
    unsigned off = getRandomInt(0, N - 1);
    for (unsigned i = 0; i < size; ++i) {
        addr[i] = (addr[i] + off) % N;
    }
}

double euclidean_distance(Vector const &vector1, Vector const &vector2) {
    float result = 0;
    for (unsigned id = 0; id < vector1.size(); id++) {
        float tmp = vector1[id] - vector2[id];
        result += tmp * tmp;
    }
    return sqrt(result);
}

DataSet getFromFile(char *filename, int rowLen, int colLen) {
    DataSet dataSet;
    dataSet.resize(rowLen);
    ifstream f(filename, ios::binary);
    for (int id = 0; id < rowLen; id++) {
        float tmp[256];
        int D;
        f.read((char *) &D, sizeof(int));
        f.read((char *) tmp, sizeof(float) * 128);
        for (int j = 0; j < D; j++)
            dataSet[id].push_back(tmp[j]);
    }
    return dataSet;
}

int diff(unsigned int a, unsigned int b) {
    if (a > b)
        swap(a, b);
    int l1 = 0, l2 = 0;
    unsigned ta = a, tb = b;
    while (ta > 0) {
        ta >>= 1;
        l1++;
    }
    while (tb > 0) {
        tb >>= 1;
        l2++;
    }
    b >>= (l2 - l1);
    int res = l1;
    a ^= b;
    while (a > 0) {
        a >>= 1;
        res--;
    }
    return res;
}
