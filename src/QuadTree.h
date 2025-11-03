#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <memory>
#include "point.h"

// axis-aligned boundary rectangle for each QuadTree node
struct Rectangle {
    double x; // center x
    double y; // center y
    double halfWidth;
    double halfHeight;

    Rectangle(double centerX, double centerY, double hw, double hh)
        : x(centerX), y(centerY), halfWidth(hw), halfHeight(hh) {}

    bool contains(const Point& point) const {
        return (point.x >= x - halfWidth && point.x <= x + halfWidth &&
                point.y >= y - halfHeight && point.y <= y + halfHeight);
    }

    bool intersects(const Rectangle& range) const {
        return !(range.x - range.halfWidth > x + halfWidth ||
                 range.x + range.halfWidth < x - halfWidth ||
                 range.y - range.halfHeight > y + halfHeight ||
                 range.y + range.halfHeight < y - halfHeight);
    }
};

class QuadTree{
public:
    QuadTree(const Rectangle& boundary, int capacity);
    bool insert(const Point& point);
    void query(const Rectangle& range, std::vector<Point>& found) const;
    void clear();

private:
    Rectangle boundary;
    int capacity;
    std::vector<Point> points;
    bool divided;
    
    std::unique_ptr<QuadTree> northeast;
    std::unique_ptr<QuadTree> northwest;
    std::unique_ptr<QuadTree> southeast;
    std::unique_ptr<QuadTree> southwest;

    void subdivide();
};

#endif