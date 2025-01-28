#include <iostream>
#include <cmath>
#include <iomanip>

#define DIMENSIONS 4
using namespace std;
struct Point
{
    float coord[DIMENSIONS]; // 0 = x, 1 = y, 2 = z, 3 = w
};

float defineDistance(Point p1, Point p2)
{
    int distance = 0;
    for(int i = 0; i < DIMENSIONS; i++)
    {
        distance += pow((p2.coord[i] - p1.coord[i]), 2);
    }
    return sqrt(distance);
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
