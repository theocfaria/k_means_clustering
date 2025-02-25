#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>

#define DIMENSIONS 4
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

void initializeCentroids(Cluster clusters[], Point data[], int k, int dataSize) // k-means++ initialization
{

    clusters[0].centroid = data[rand() % dataSize]; // choose the first centroid randomly

    for (int i = 1; i < k; i++) // for the remaining centroids
    {
        float distSquared[dataSize]; // squared distance from each point to the nearest centroid
        float totalDist = 0.0f;

        for (int j = 0; j < dataSize; j++) // calculate the distance to the nearest centroid for each point
        {
            distSquared[j] = defineDistance(data[j], clusters[0].centroid);
            for (int c = 1; c < i; c++) // compare with other centroids
            {
                float distance = defineDistance(data[j], clusters[c].centroid);
                distSquared[j] = min(distSquared[j], distance);
            }
            distSquared[j] *= distSquared[j];
            totalDist += distSquared[j];
        }

        float randChoice = (rand() / (float)RAND_MAX) * totalDist; // random value for selection
        float cumulativeDist = 0.0f;                               // cumulative sum of squared distances
        int chosenPoint = 0;

        for (int j = 0; j < dataSize; j++) // choose the point based on proportional probability
        {
            cumulativeDist += distSquared[j]; // accumulate squared distances
            if (cumulativeDist >= randChoice) // when the cumulative sum exceeds the random value
            {
                chosenPoint = j; // selected point as the next centroid
                break;
            }
        }

        clusters[i].centroid = data[chosenPoint];
    }
}

int assignCluster(Point p, Cluster clusters[], int k) // called in the main clustering func, assigns clusters to each point
{
    float minDistance = defineDistance(p, clusters[0].centroid); // defines the smallest distance between a point and the first cluster's centroid
    int clusterIndex = 0;
    for (int i = 0; i < k; i++)
    {
        float distance = defineDistance(p, clusters[i].centroid); // compares the smallest distance to the next's centroid
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
    for (int i = 0; i < k; i++) // iterates over each cluster
    {
        Point newCentroid;
        for (int d = 0; d < DIMENSIONS; d++) // initializes all centroid's coordinates as 0
        {
            newCentroid.coord[d] = 0;
        }
        int count = 0;                     // counts all the points that belongs to the current cluster
        for (int j = 0; j < dataSize; j++) // iterates over all the points from the data
        {
            if (assignments[j] == i) // checks if the point belongs to the current cluster
            {
                for (int d = 0; d < DIMENSIONS; d++)
                {
                    newCentroid.coord[d] += data[j].coord[d]; // sums the current point's coordinates to the centroid
                }
                count++;
            }
        }
        if (count > 0) // avoids division by 0 checking if the cluster is empty
        {
            for (int d = 0; d < DIMENSIONS; d++)
            {
                clusters[i].centroid.coord[d] = newCentroid.coord[d] / count; // defines new centroid's coordinates dividing the total sum by the amout of points
            }
        }
    }
}

void kMeansClustering(Point data[], int dataSize, int k, int maxIterations) // compares each point's coord with the cluster's centroid to assign each point its group
{
    Cluster clusters[k];
    int assignments[dataSize] = {0}; // array to store each point's clusters

    initializeCentroids(clusters, data, k, dataSize); // initializes the cluster's centroids

    for (int i = 0; i < maxIterations; i++) // main loop 
    {
        bool hasChanged = false; // variable to check if the assignment has changed

        for (int j = 0; j < dataSize; j++) 
        {
            int newAssignment = assignCluster(data[j], clusters, k); // gets the closest cluster to each point; assignCluster() returns the cluster index
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
    for (int c = 0; c < k; c++) // prints the centroid's coordinates
    {
        cout << "Centroid " << c << ": (";
        for (int d = 0; d < DIMENSIONS; d++)
        {
            cout << clusters[c].centroid.coord[d] << (d < DIMENSIONS - 1 ? ", " : "");
        }
        cout << ")" << endl;
    }
    for (int j = 0; j < dataSize; j++) // prints each point's final assignment
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

int countLines(string file_name) // count the amount of lines in the file
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

void definePoints(Point data[], int dataSize, string file_name)
{
    string line;
    ifstream file(file_name);
    for (int c = 0; getline(file, line) && c < dataSize; c++)
    {
        stringstream lines(line);
        for (int i = 0; i < DIMENSIONS; i++)
        {
            lines >> data[c].coord[i];
        }
    }
}

int main()
{
    srand(time(NULL));
    string file_name;
    cout << "Inform the name of the data file desired: " << endl;
    cin >> file_name;
    int dataSize = countLines(file_name); // amount of points in the graph, can be read from the file or be input
    int k;                                // amount of clusters
    int maxIterations;                    // how many times the clustering func will be called
    Point data[dataSize];
    definePoints(data, dataSize, file_name);
    cout << "Enter the amount of clusters desired: ";
    cin >> k;
    cout << "Enter the amount of iterations desired: ";
    cin >> maxIterations;
    kMeansClustering(data, dataSize, k, maxIterations);
    return 0;
}