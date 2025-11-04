#include <iostream>
#include <sciplot/sciplot.hpp>
#include <vector>
#include <iomanip>
#include <random>
#include "KDTree.h"
#include "QuadTree.h"
#include "Point.h"

using namespace sciplot;

int main()
{
    //Creating vector to hold points that will be added to trees
    std::vector<Point> points;

    //Generating random 1000 random points for testing

    // Gainesville bounding box
    double xmin = -82.50000;
    double xmax = -82.25000;
    double ymin = 29.55000;
    double ymax = 29.75000;

    //Approximate mile radius in lat/lon units
    double baserad = 0.01558;

    //RNG
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> lonDist(xmin, xmax);
    std::uniform_real_distribution<> latDist(ymin, ymax);

    //Generate values
    std::vector<double> latitudes;
    std::vector<double> longitudes;
    latitudes.reserve(1000);
    longitudes.reserve(1000);

    for (int i = 0; i < 1000; ++i)
    {
        double lat = latDist(gen);
        double lon = lonDist(gen);

        // Round to 5 decimal places
        lat = std::round(lat * 100000.0) / 100000.0;
        lon = std::round(lon * 100000.0) / 100000.0;

        latitudes.push_back(lat);
        longitudes.push_back(lon);

        points.push_back(Point(lon, lat));
    }

    //Create KDTree
    KDTree kd(points);

    //User input
    double inputlat;
    double inputlon;
    double radiusmult;

    std::cout << "Crime Rate Query\n\n";

    std::cout << "Enter target latitude (between 29.55 and 29.75): ";
    std::cin >> inputlat;

    std::cout << "Enter target longitude (between -82.5 and -82.25): ";
    std::cin >> inputlon;

    std::cout << "Enter search radius (miles): ";
    std::cin >> radiusmult;

    double radius = baserad * radiusmult;

    //Plot data points
    Plot2D plot;
    plot.drawPoints(longitudes, latitudes).lineColor("red").label("");  // Red points, no legend

    // Generate circle points
    std::vector<double> x, y;
    for (int i = 0; i <= 360; ++i)
    {
        double theta = i * M_PI / 180.0;  // Convert degrees to radians
        x.push_back(inputlon + radius * std::cos(theta));
        y.push_back(inputlat + radius * std::sin(theta));
    }

    plot.drawCurve(x ,y).lineColor("blue").label("");

    Figure fig({{plot}});
    fig.title("Points with Target Ring");

    Canvas canvas({{fig}});
    canvas.size(1200, 1200);
    canvas.title("Custom Axes Canvas");
    canvas.show();

    return 0;


}