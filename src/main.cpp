#include <iostream>
#include <vector>
#include "kdtree.h"
#include "point.h"

int main() {
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

    return 0;
}