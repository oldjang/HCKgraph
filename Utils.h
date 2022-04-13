#ifndef HCK_UTILS_H
#define HCK_UTILS_H
#include <random>
#include "Data.h"

using namespace std;

double getRandomReal();
unsigned getRandomInt(int lowerBound,int upperBound);
void getRandomArray(unsigned *addr, unsigned size, unsigned N);
double euclidean_distance(Vector const &vector1,Vector const &vector2);
int diff(unsigned a,unsigned b);

DataSet getFromFile(char *filename,int rowLen,int colLen);


#endif //HCK_UTILS_H
