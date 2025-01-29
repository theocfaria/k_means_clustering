#include <iostream>
#include <cmath>
#include <iomanip>

#define DIMENSIONS 5
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
    float minDistance = defineDistance(p, clusters[0].centroid);
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

void recalculateCentroids(Cluster clusters[], Point data[], int assignments[], int k, int dataSize)
{
    for(int i = 0; i < k; i++)
    {
        Point newCentroid = {0};
        int count = 0;
        for(int j = 0; j < dataSize; j++)
        {
            if(assignments[j] == i)
            {
                for(int d = 0; d < DIMENSIONS; d++)
                {
                    newCentroid.coord[d] += data[j].coord[d];
                }
                count++;
            }
        }
        if(count > 0)
        {
            for(int d = 0; d < DIMENSIONS; d++)
            {
                clusters[i].centroid.coord[d] = newCentroid.coord[d] / count;
            }
        }
    }
}

void kMeansClustering(Point data[], int dataSize, int k, int maxIterations)
{
    Cluster clusters[k];
    int assignments[dataSize] = {0}; // array to store each point's clusters

    initializeCentroids(clusters, data, k, dataSize);

    for(int i = 0; i < maxIterations; i++)
    {
        bool hasChanged = false;
        
        for(int j = 0; j < dataSize; j++)
        {
            int newAssignment = assignCluster(data[j], clusters, k);
            if(newAssignment != assignments[j])
            {
                hasChanged = true;
                assignments[j] = newAssignment;
            }
        }

        recalculateCentroids(clusters, data, assignments, k, dataSize);

        if(!hasChanged)
        {
            break;
        }
    }
    for(int c = 0; c < k; c++) {
            cout << "Centroid " << c << ": (";
            for(int d = 0; d < DIMENSIONS; d++) {
                cout << clusters[c].centroid.coord[d] << (d < DIMENSIONS - 1 ? ", " : "");
            }
            cout << ")\n";
        }
        for(int j = 0; j < dataSize; j++) {
            cout << "Point " << j + 1 << " is in cluster " << assignments[j] << "\n";
        }
        cout << "----------------------------\n";
}
char defineCoordName(int d) {
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    int size = sizeof(alphabet) - 1; 
    
    if (d >= 0 && d < size) {
        return alphabet[d];
    }
}

int main() {
    srand(time(NULL));
    int dataSize, k, maxIterations;
    cout << "Inform the amount of points in the graph: ";
    cin >> dataSize;
    cout << "Inform the amount of clusters desired: ";
    cin >> k; 
    cout << "Inform the amount of iterations desired: ";
    cin >> maxIterations;

    Point data[dataSize];
    for (int i = 0; i < dataSize; i++) {
        for (int d = 0; d < DIMENSIONS; d++) {
            cout << "Inform the coordinates " << defineCoordName(d) << " of the point " << i + 1 << ": " << endl;  
            cin >> data[i].coord[d];
        }
    }

    kMeansClustering(data, dataSize, k, maxIterations);
    return 0;
}