#include <iostream>
#include <cmath>
#include <iomanip>

#define DIMENSIONS 4
using namespace std;

struct Point
{
    float coord[DIMENSIONS]; // 0 = x, 1 = y, 2 = z, 3 = w
};
struct Cluster
{
    Point centroid;
};

float defineDistance(Point p1, Point p2) // defines distance between two points in 4 dimensions; scalable function;
{
    float distance = 0;
    for(int i = 0; i < DIMENSIONS; i++)
    {
        distance += pow((p2.coord[i] - p1.coord[i]), 2);
    }
    return sqrt(distance);
}

void initializeCentroids(Cluster clusters[], Point data[], int k, int dataSize)
{
    for(int i = 0; i < k; i++)
    {
        clusters[i].centroid = data[rand() % dataSize]; // chooses a random point between 0 and (dataSize - 1)
    }
}

int assignCluster(Point p, Cluster clusters[], int k)
{
    float minDistance = defineDistance(p, clusters[0]. centroid);
    int clusterIndex = 0;
    for(int i = 0; i < k; i++)
    {
        float distance = defineDistance(p, clusters[i].centroid);
        if (distance < minDistance)
        {
            minDistance = distance;
            clusterIndex = i;
        }
    }
    return clusterIndex;
}

int main()
{
    Point p1, p2;
    for(int i = 0; i < DIMENSIONS; i++)
    {
        cin >> p1.coord[i];
        cin >> p2.coord[i];
    }
    cout << fixed << setprecision(2) << defineDistance(p1, p2);
}


