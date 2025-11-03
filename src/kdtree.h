#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>
#include "point.h"

class KDTree {
private:
    struct Node {
        Point point;
        Node* left;
        Node* right;
        int depth;

        Node(const Point& pt, int d);
    };

    Node* root;
    size_t dimension;

    Node* build(std::vector<Point>& pts, int depth);
    void destroy(Node* node);
    void rangeSearchHelper(Node* node, const Point& target, double radius, std::vector<Point>& result) const;
    void nearestNeighborHelper(Node* node, const Point& target, Point& bestPoint, double& bestDist) const;

public:
    KDTree(const std::vector<Point>& points, size_t dim = 2);
    ~KDTree();

    std::vector<Point> rangeSearch(const Point& target, double radius) const;
    Point nearestNeighbor(const Point& target) const;
};

#endif
