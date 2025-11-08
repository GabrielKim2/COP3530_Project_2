#include <iostream>
#include <sciplot/sciplot.hpp>
#include <vector>
#include <iomanip>
#include <random>
#include <chrono>
#include <fstream>
#include "KDTree.h"
#include "QuadTree.h"
#include "Point.h"

using namespace sciplot;

int main()
{
    //Creating vector to hold points that will be added to trees
    std::vector<Point> points;

    /*
    //Reading Gainesville crime data from text file

    std::ifstream infile("data.txt");
    if (!infile) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    std::vector<Point> points;
    double lon;
    double lat

    while (infile >> lon >> lat) {
        points.push_back(Point(lon, lat);
    }


     */

    //Generating random 100000 random points for testing

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
    latitudes.reserve(100);
    longitudes.reserve(100);



    for (int i = 0; i < 100; ++i)
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
    //Insert data into kd tree and record time
    auto start_time = std::chrono::steady_clock::now();
    KDTree kd(points);
    auto end_time = std::chrono::steady_clock::now();

    auto kdInsertDuration = end_time - start_time;

    //Create QuadTree
    Rectangle boundary(-82.33, 29.65, 0.1, 0.1);
    QuadTree qd(boundary, 4);

    //Insert data into quad tree and record time
    start_time = std::chrono::steady_clock::now();
    for (Point& point : points) {
        qd.insert(point);
    }
    end_time = std::chrono::steady_clock::now();

    auto quadInsertDuration = end_time - start_time;

    //User input
    double inputlat;
    double inputlon;
    double radiusmult;

    std::cout << "Enter target latitude (between 29.55 and 29.75): ";
    std::cin >> inputlat;

    std::cout << "Enter target longitude (between -82.5 and -82.25): ";
    std::cin >> inputlon;

    std::cout << "Enter search radius (miles): ";
    std::cin >> radiusmult;

    double radius = baserad * radiusmult;
    double area = M_PI * radiusmult * radiusmult;

    Point target(inputlon, inputlat);

    //Run Operations on KDTree

    start_time = std::chrono::steady_clock::now();
    std::vector<Point> kdInRange = kd.rangeSearch(target, radius);
    end_time = std::chrono::steady_clock::now();

    auto kdSearchDuration = end_time - start_time;

    int kdcrimes = kdInRange.size();

    double kdcrimedensity = kdcrimes / area; //Measured in crimes per mile squared

    //Run Operations on QuadTree
    start_time = std::chrono::steady_clock::now();
    std::vector<Point> quadInRange;
    qd.query(Rectangle(target.x, target.y, radius, radius), quadInRange);
    end_time = std::chrono::steady_clock::now();

    auto quadSearchDuration = end_time - start_time;

    int quadcrimes = quadInRange.size();

    double quadcrimedensity = quadcrimes / area; //Measured in crimes per mile squared

    //Plot data points
    Plot2D plot;
    plot.xlabel("Longitude");
    plot.ylabel("Latitude");
    plot.drawPoints(longitudes, latitudes).lineColor("red").label("");  // Red points, no legend

    // Generate circle points
    std::vector<double> x, y;
    for (int i = 0; i <= 360; ++i)
    {
        double theta = i * M_PI / 180.0;  // Convert degrees to radians
        x.push_back(inputlon + radius * std::cos(theta));
        y.push_back(inputlat + radius * std::sin(theta));
    }

    plot.drawCurve(x ,y).lineColor("blue").label("").lineWidth(2);

    Figure fig({{plot}});
    fig.title("Points with Target Ring");

    Canvas canvas({{fig}});
    canvas.size(1500, 1300);
    canvas.title("Custom Axes Canvas");

    std::cout << "\nKD Tree Metrics:" << std::endl;
    std::cout << "==================" << std::endl;
    std::cout << "Crimes in given radius: " << kdcrimes << std::endl;
    std::cout << "Crimes Density (crimes/mile^2): " << kdcrimedensity << std::endl;
    std::cout << "Time to insert data into tree (ns): " << std::chrono::duration_cast<std::chrono::nanoseconds>(kdInsertDuration).count() << std::endl;
    std::cout << "Time to determine crimes in radius (ns): " << std::chrono::duration_cast<std::chrono::nanoseconds>(kdSearchDuration).count() << std::endl;

    std::cout << "\nQuad Tree Metrics:" << std::endl;
    std::cout << "==================" << std::endl;
    std::cout << "Crimes in given radius: " << quadcrimes << std::endl;
    std::cout << "Crimes Density: " << quadcrimedensity << std::endl;
    std::cout << "Time to insert data into tree (ns): " << std::chrono::duration_cast<std::chrono::nanoseconds>(quadInsertDuration).count() << std::endl;
    std::cout << "Time to determine crimes in radius (ns): " << std::chrono::duration_cast<std::chrono::nanoseconds>(quadSearchDuration).count() << std::endl;

    canvas.show();

    return 0;


}