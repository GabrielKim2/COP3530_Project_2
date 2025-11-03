#include <iostream>
#include <vector>
#include "kdtree.h"
#include "QuadTree.h"
#include "point.h"

int main() {
    /*
    std::vector<Point> points = {
        Point(2.0, 3.0, "A"),
        Point(5.0, 4.0, "B"),
        Point(9.0, 6.0, "C"),
        Point(4.0, 7.0, "D"),
        Point(8.0, 1.0, "E"),
        Point(7.0, 2.0, "F")
    };

    KDTree tree(points);

    Point target(9.0, 2.0, "Target");
    Point nearest = tree.nearestNeighbor(target);
    std::cout << "Nearest neighbor to " << target.toString() << " is " << nearest.label << " at " << nearest.toString() << std::endl;

    double radius = 3.0;
    std::vector<Point> inRange = tree.rangeSearch(target, radius);
    std::cout << "Points within radius " << radius << " of " << target.toString() << ":" << std::endl;
    for (const auto& pt : inRange) {
        std::cout << pt.label << " at " << pt.toString() << std::endl;
    }
    */

    Rectangle boundary(0.0, 0.0, 100.0, 100.0);
    QuadTree qt(boundary, 4);

    for (int i = 0; i < 20; ++i) {
        double x = rand() % 200 - 100; // Random x between -100 and 100
        double y = rand() % 200 - 100; // Random y between -100 and 100
        qt.insert(Point(x, y, "P" + std::to_string(i)));
    }

    Rectangle range(0, 0, 50, 50);
    std::vector<Point> found;
    qt.query(range, found);
    std::cout << "Points found in range:" << std::endl;
    for (const auto& pt : found) {
        std::cout << pt.label << " at " << pt.toString() << std::endl;
    }

    return 0;
}