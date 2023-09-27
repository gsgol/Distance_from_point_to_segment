#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <algorithm>
#include <mutex>

#define exp 0.00001

//Calculates the dot product of two vectors by the entered coordinates
double dot(double x0, double y0, double x1, double y1)
{
    return x0 * x1 + y0 * y1;
}

//Determines the distance between two points by the entered coordinates
double dist(double x0, double y0, double x1, double y1)
{
    double xDiff = x1 - x0;
    double yDiff = y1 - y0;

    return std::pow((xDiff * xDiff + yDiff * yDiff), 0.5);
}

//Determines the distance from the point to the segment by the entered coordinates
double distToSegment(double xDot, double yDot, double xSegment0, double ySegment0, double xSegment1, double ySegment1)
{
    double xSegmentDiff = xSegment1 - xSegment0;
    double ySegmentDiff = ySegment0 - ySegment1;
    double segmentLen = dist(xSegment0, ySegment0, xSegment1, ySegment1);

    return fabs(((ySegmentDiff * xDot) + (xSegmentDiff * yDot) + (xSegment0 * ySegment1 - xSegment1 * ySegment0)) / segmentLen) ;
}

//Solves the problem of finding the distance from a point to a segment
double solve(std::pair<double, double> dotCoord, std::pair<double, double> segmentCoord0, std::pair<double, double> segmentCoord1)
{
    std::pair<double, double>vecCutDot0, vecCutDot1, vecCut;

    vecCutDot0.first = segmentCoord0.first  - dotCoord.first ;
    vecCutDot0.second = segmentCoord0.second - dotCoord.second;
    double dotSegmentLen0 = dist(dotCoord.first, dotCoord.second, segmentCoord0.first, segmentCoord0.second);

    vecCutDot1.first = segmentCoord1.first  - dotCoord.first ;
    vecCutDot1.second = segmentCoord1.second - dotCoord.second;
    double dotSegmentLen1 = dist(dotCoord.first, dotCoord.second, segmentCoord1.first, segmentCoord1.second);

    vecCut.first = segmentCoord1.first  - segmentCoord0.first ;
    vecCut.second = segmentCoord1.second - segmentCoord0.second;
    double segmentLen = dist(segmentCoord0.first, segmentCoord0.second, segmentCoord1.first, segmentCoord1.second);

    bool isBetween = false;

    if(dotCoord.first <= std::max(segmentCoord0.first, segmentCoord1.first) && dotCoord.first >= std::min(segmentCoord0.first, segmentCoord1.first) &&
    dotCoord.second <= std::max(segmentCoord0.second, segmentCoord1.second) && dotCoord.second >= std::min(segmentCoord0.second, segmentCoord1.second))
        isBetween = true;

    if(fabs(fabs(dot(vecCutDot0.first, vecCutDot0.second, vecCut.first, vecCut.second)) - fabs( dotSegmentLen0 * segmentLen )) <= exp && isBetween)
        return 0;

    if(dot(vecCutDot0.first, vecCutDot0.second, vecCut.first, vecCut.second) >= 0)
        return dist(dotCoord.first , dotCoord.second, segmentCoord0.first , segmentCoord0.second);

    if(dot(vecCut.first, vecCut.second, vecCutDot1.first, vecCutDot1.second) <= 0)
        return dist(dotCoord.first , dotCoord.second, segmentCoord1.first , segmentCoord1.second);

    return distToSegment(dotCoord.first , dotCoord.second, segmentCoord0.first , segmentCoord0.second, segmentCoord1.first , segmentCoord1.second);
}
int main()
{
    std::pair<double, double> dotCoord, segmentCoord0, segmentCoord1;

    std::cout << "Enter the coordinates of the point (x, y):\n";
    std::cin >> dotCoord.first >> dotCoord.second;

    std::cout << "Enter the coordinates of the ends of the segment(x0, y0, x1, y1):\n";
    std::cin >> segmentCoord0.first >> segmentCoord0.second >> segmentCoord1.first >> segmentCoord1.second;

    auto start = std::chrono::steady_clock::now();

    double ans = solve(dotCoord, segmentCoord0, segmentCoord1);

    auto end = std::chrono::steady_clock::now();

    std::cout << "The distance is: " << ans << "\n";

    std::cout << "The time is: "<< std::chrono::duration <double, std::milli> (end - start).count() << " ms\n";

    std::pair<double, double> newDot;
    int numOfVertices;

    std::cout << "Enter the coordinates of the point (x, y):\n";
    std::cin >> newDot.first >> newDot.second;

    std::cout<<"Enter number of vertices in the polygon\n";
    std::cin >> numOfVertices;

    std::vector<std::pair<double, double>> polygon(numOfVertices);
    std::cout << "Enter the coordinates of the vertices of the polygon (x, y):\n";

    for(size_t i = 0; i < numOfVertices; ++i)
    {
        std::cin >> polygon[i].first >> polygon[i].second;
    }

    double minDist = INT64_MAX;

    auto start1 = std::chrono::steady_clock::now();

    for(int i = 0; i < numOfVertices; i++)
    {
        minDist = fmin(minDist, solve(newDot, polygon[i], polygon[(i + 1) % 4]));
    }

    auto end1 = std::chrono::steady_clock::now();

    std::cout << "The distance is: " << minDist << "\n";

    std::cout << "The time is: " << std::chrono::duration <double, std::milli> (end1 - start1).count() << " ms";

    return 0;
}