#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>

#define DIMENSIONS 2
using namespace std;

struct Point
{
    float coord[DIMENSIONS]; // 0 = x, 1 = y, 2 = z, 3 = w...
};
struct Cluster
{
    Point centroid;
};

float defineDistance(Point p1, Point p2) // defines distance between two points in any number of dimensions
{
    float distance = 0;
    for (int i = 0; i < DIMENSIONS; i++)
    {
        distance += pow((p2.coord[i] - p1.coord[i]), 2);
    }
    return sqrt(distance);
}

void initializeCentroids(Cluster clusters[], Point data[], int k, int dataSize) // chooses a random point from the dataset
{
    for (int i = 0; i < k; i++)
    {
        clusters[i].centroid = data[rand() % dataSize];
    }
}

int assignCluster(Point p, Cluster clusters[], int k) // called in the main clustering func, assigns clusters to each point
{
    float minDistance = defineDistance(p, clusters[0].centroid);
    int clusterIndex = 0;
    for (int i = 0; i < k; i++)
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

void recalculateCentroids(Cluster clusters[], Point data[], int assignments[], int k, int dataSize) // recalculate the center of each cluster after the addition of new points
{
    for (int i = 0; i < k; i++)
    {
        Point newCentroid = {0};
        int count = 0;
        for (int j = 0; j < dataSize; j++)
        {
            if (assignments[j] == i)
            {
                for (int d = 0; d < DIMENSIONS; d++)
                {
                    newCentroid.coord[d] += data[j].coord[d];
                }
                count++;
            }
        }
        if (count > 0)
        {
            for (int d = 0; d < DIMENSIONS; d++)
            {
                clusters[i].centroid.coord[d] = newCentroid.coord[d] / count;
            }
        }
    }
}

void kMeansClustering(Point data[], int dataSize, int k, int maxIterations) // compares each point's coord with the cluster's centroid to assign each point its group
{
    Cluster clusters[k];
    int assignments[dataSize] = {0}; // array to store each point's clusters

    initializeCentroids(clusters, data, k, dataSize);

    for (int i = 0; i < maxIterations; i++)
    {
        bool hasChanged = false;

        for (int j = 0; j < dataSize; j++)
        {
            int newAssignment = assignCluster(data[j], clusters, k);
            if (newAssignment != assignments[j])
            {
                hasChanged = true;
                assignments[j] = newAssignment;
            }
        }

        recalculateCentroids(clusters, data, assignments, k, dataSize);

        if (!hasChanged)
        {
            break;
        }
    }
    cout << "----------------------------\n";
    for (int c = 0; c < k; c++)
    {
        cout << "Centroid " << c << ": (";
        for (int d = 0; d < DIMENSIONS; d++)
        {
            cout << clusters[c].centroid.coord[d] << (d < DIMENSIONS - 1 ? ", " : "");
        }
        cout << ")" << endl;
    }
    for (int j = 0; j < dataSize; j++)
    {
        cout << "Point " << j + 1 << " is in cluster " << assignments[j] << "\n";
    }
    cout << "----------------------------\n";
}
char defineCoordName(int d) // defines the coordinate's name, starts from x
{
    const char alphabet[] = "xyzabcdefghijklmnopqrstuvw";
    int size = sizeof(alphabet) - 1;

    if (d >= 0 && d < size)
    {
        return alphabet[d];
    }
}

int countLines(string file_name)  // count the amount of lines in the file
{
    ifstream file(file_name);
    if (!file.is_open())
    {
        cout << "Failed to open the file" << endl;
        return -1;
    }

    string line;
    int count = 0;

    while (getline(file, line))
    {
        count++;
    }

    file.close();
    return count;
}

void definePoints(Point data[], int dataSize)
{
    string file_name, line;
    cout << "Inform the name of the data file desired: " << endl;
    cin >> file_name;
    ifstream file(file_name);
    for(int c = 0; getline(file, line) && c < dataSize; c++)
    {
        stringstream lines(line);
        lines >> data[c].coord[0] >> data[c].coord[1] >> data[c].coord[2] >> data[c].coord[3];
        c++;
    }
}

int main()
{
    srand(time(NULL));
    int dataSize = countLines("data.txt"); // amount of points in the graph, can be read from the file or be input
    int k;                                // amount of clusters
    int maxIterations;                    // how many times the clustering func will be called
    Point data[dataSize];
    definePoints(data, dataSize);
    cout << "Enter the amount of clusters desired: ";
    cin >> k;
    cout << "Enter the amount of iterations desired: ";
    cin >> maxIterations;
    kMeansClustering(data, dataSize, k, maxIterations);
    return 0;
}