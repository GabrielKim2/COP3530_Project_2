#include "QuadTree.h"
#include <iostream>

QuadTree::QuadTree(const Rectangle& boundary, int capacity)
    : boundary(boundary), capacity(capacity), divided(false) {}

bool QuadTree::insert(const Point& point) {
    if (!boundary.contains(point)) return false;

    if (points.size() < static_cast<size_t>(capacity)) {
        points.push_back(point);
        return true;
    }

    if (!divided) subdivide();

    if (northeast->insert(point)) return true;
    if (northwest->insert(point)) return true;
    if (southeast->insert(point)) return true;
    if (southwest->insert(point)) return true;

    return false;
}

void QuadTree::subdivide() {
    double x = boundary.x;
    double y = boundary.y;
    double hw = boundary.halfWidth / 2.0;
    double hh = boundary.halfHeight / 2.0;

    Rectangle neRect(x + hw, y - hh, hw, hh);
    northeast = std::make_unique<QuadTree>(neRect, capacity);

    Rectangle nwRect(x - hw, y - hh, hw, hh);
    northwest = std::make_unique<QuadTree>(nwRect, capacity);

    Rectangle seRect(x + hw, y + hh, hw, hh);
    southeast = std::make_unique<QuadTree>(seRect, capacity);

    Rectangle swRect(x - hw, y + hh, hw, hh);
    southwest = std::make_unique<QuadTree>(swRect, capacity);

    divided = true;
}

void QuadTree::query(const Rectangle& range, std::vector<Point>& found) const {
    if (!boundary.intersects(range)) return;

    for (const auto& point : points) {
        if (range.contains(point)) {
            found.push_back(point);
        }
    }

    if (divided) {
        northeast->query(range, found);
        northwest->query(range, found);
        southeast->query(range, found);
        southwest->query(range, found);
    }
}

void QuadTree::clear() {
    points.clear();
    if (divided) {
        northeast->clear();
        northwest->clear();
        southeast->clear();
        southwest->clear();
        divided = false;
    }
}